#include <stdio.h>
#include <string.h>
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "display.h"
#include "profiles.h"
#include "system_state.h"
#include "control.h"
#include "fc_detector.h"


// Free RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

extern uint32_t millis();

bool unit_F = false;
QueueHandle_t encoderQueue;

void TaskDisplay_Control(void *p){
    init_display();

    while(1) {
        switch (g_state.mode)
        {
        case SYS_START:
            print_info("Aguarde...", "Sistema iniciando");
            break;
        case SYS_IDLE:
            print_info("Sistema em espera", "");
            break;
        case SYS_MENU:
            print_menu(g_state.profile_id, g_state.profile_name, g_state.profile_duration, g_state.profile_type);
            break;
        case SYS_PREHEAT:
            update_display_info(g_state.bt, g_state.et, 5, g_state.profile_name, g_state.profile_id);
            break;
        case SYS_ROAST:
            update_display_info(g_state.bt, g_state.et, g_state.roast_stage, g_state.profile_name, g_state.profile_id);  
            break;
        case SYS_COOLDOWN:
            update_display_info(g_state.bt, g_state.et, 4, g_state.profile_name, g_state.profile_id);
            break;
        case SYS_EMERGENCY:
            print_info("Emergencia!!!", "Parando tarefas");
            break;
        default: update_display_info(g_state.bt, g_state.et, g_state.roast_stage, g_state.profile_name, g_state.profile_id);
            break;
        }

        vTaskDelay(200);
    }
}

void command_read() {
    // Formato TC4: Ambiência, Temp1, Temp2, Saída1, Saída2
    printf("0.00,%.2f,%.2f,0.00,%d\n", g_state.et, g_state.bt, g_state.target);
}

void handle_serial_command() {
    char buf[64];
    int i = 0;

    // Lê caracteres disponíveis de forma não bloqueante
    while (true) {
        int c = getchar_timeout_us(0);
        if (c == PICO_ERROR_TIMEOUT || c == '\n' || i >= 63) {
            if (i > 0) {
                buf[i] = '\0';
                break;
            }
            return;
        }
        buf[i++] = (char)c;
    }

    // Parsing dos comandos
    if (strncmp(buf, "CHAN;", 5) == 0) {
        printf("#OK\n");
    } 
    else if (strncmp(buf, "UNITS;", 6) == 0) {
        if (buf[6] == 'F') {
            unit_F = true;
            printf("#OK Fahrenheit\n");
        } else {
            unit_F = false;
            printf("#OK Celsius\n");
        }
    } 
    else if (strncmp(buf, "READ", 4) == 0) {
        command_read();
    }
}

void TaskArtisan_uart(void *p){
    while (1)
    {
        handle_serial_command();
        vTaskDelay(100);
    }
}

void TaskUi(void *p) {
    int move;

    gpio_init(BTN1_PIN);
    gpio_set_dir(BTN1_PIN, GPIO_IN);

    gpio_init(BTN2_PIN);
    gpio_set_dir(BTN2_PIN, GPIO_IN);


    g_state.profile_duration = get_profile_finish_time(g_state.profile_id);
    g_state.profile_name = get_profile_name(g_state.profile_id);
    g_state.profile_type = get_profile_type(g_state.profile_id);

    while (1)
    {
        if(g_state.mode == SYS_IDLE){
            vTaskDelay(1000);
            g_state.mode = SYS_MENU;
        } else if (g_state.mode == SYS_MENU) {
            if (gpio_get(BTN1_PIN) == 0) {

            }
            if (gpio_get(BTN2_PIN) == 0) {

            }
            if(gpio_get(ENCODER_BTN_PIN) == 0) {
                printf("Botão pressionado\n");
                g_state.mode = SYS_PREHEAT;
            }

            if (xQueueReceive(encoderQueue, &move, 0)){
                if (g_state.mode == SYS_MENU) {
                    g_state.profile_id += move;
                    if (g_state.profile_id < 0)
                        g_state.profile_id = 0;

                    if (g_state.profile_id >= PROFILE_COUNT)
                        g_state.profile_id = PROFILE_COUNT - 1;

                    g_state.profile_duration = get_profile_finish_time(g_state.profile_id);
                    g_state.profile_name = get_profile_name(g_state.profile_id);
                    g_state.profile_type = get_profile_type(g_state.profile_id);
                }
            }
        } else if (g_state.mode == SYS_PREHEAT || g_state.mode == SYS_ROAST) {
            if (gpio_get(BTN1_PIN) == 0) {
                g_state.stop_flag = 1;
                vTaskDelay(50);
                //printf("Botão de parada pressionado\n");
            }
            if (gpio_get(BTN2_PIN) == 0) {
                printf("Botão de emergência pressionado!\n");
                emergency_shutdown();
            }
            if(gpio_get(ENCODER_BTN_PIN) == 0) {
                
            }
        }

        vTaskDelay(10);
    }
}

void TaskFC(void *p) {
    fc_fft_init();

    while (1) {

        sample_audio();

        run_fft();

        float e = crack_energy();

        if (detect_fc(e))
        {
            printf("FIRST CRACK\n");
        }

        vTaskDelay(10);

    }

}

void TaskEncoder(void *p)
{
    gpio_init(ENCODER_DT_PIN);
    gpio_set_dir(ENCODER_DT_PIN, GPIO_IN);
    gpio_pull_up(ENCODER_DT_PIN);

    gpio_init(ENCODER_CLK_PIN);
    gpio_set_dir(ENCODER_CLK_PIN, GPIO_IN);
    gpio_pull_up(ENCODER_CLK_PIN);

    gpio_init(ENCODER_BTN_PIN);
    gpio_set_dir(ENCODER_BTN_PIN, GPIO_IN);

    uint8_t prev =
        (gpio_get(ENCODER_CLK_PIN) << 1) |
        gpio_get(ENCODER_DT_PIN);

    uint8_t last_read = prev;

    while (1) {
        uint8_t a1 = gpio_get(ENCODER_DT_PIN);
        uint8_t b1 = gpio_get(ENCODER_CLK_PIN);

        uint8_t first = (b1 << 1) | a1;

        vTaskDelay(pdMS_TO_TICKS(1));

        uint8_t a2 = gpio_get(ENCODER_DT_PIN);
        uint8_t b2 = gpio_get(ENCODER_CLK_PIN);

        uint8_t second = (b2 << 1) | a2;

        // debounce: só aceita se igual
        if (first != second) {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        uint8_t curr = second;

        // evita repetir mesmo estado
        if (curr == last_read) {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        last_read = curr;

        uint8_t state = (prev << 2) | curr;

        int move = 0;

        switch (state) {
            case 0b0001:
            case 0b0111:
            case 0b1110:
            case 0b1000:
                move = 1;
                break;

            case 0b0010:
            case 0b0100:
            case 0b1101:
            case 0b1011:
                move = -1;
                break;
        }

        prev = curr;

        if (curr == 0 && move != 0) {
            xQueueSend(
                encoderQueue,
                &move,
                0
            );
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}
