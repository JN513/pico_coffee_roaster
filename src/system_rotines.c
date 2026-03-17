#include <stdio.h>
#include <string.h>
#include "config.h"
#include "pico/stdlib.h"
#include "display.h"
#include "profiles.h"
#include "system_state.h"
#include "encoder.h"
#include "control.h"

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
            break;
        case SYS_PREHEAT:
            update_display_info(g_state.bt, g_state.et, 5, g_state.profile_name);
            break;
        case SYS_ROAST:
            update_display_info(g_state.bt, g_state.et, g_state.roast_stage, g_state.profile_name);  
            break;
        case SYS_COOLDOWN:
            update_display_info(g_state.bt, g_state.et, 4, g_state.profile_name);
            break;
        case SYS_EMERGENCY:
            print_info("Emergencia!!!", "Parando tarefas");
            break;
        default: update_display_info(g_state.bt, g_state.et, g_state.roast_stage, g_state.profile_name);
            break;
        }

        vTaskDelay(200);
    }
}

void command_read() {
    // Formato TC4: Ambiência, Temp1, Temp2, Saída1, Saída2
    printf("0.00,%.2f,%.2f,0.00,0.00\n", g_state.bt, g_state.et);
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

    encoder_init();

    while (1)
    {
        if (xQueueReceive(
                encoderQueue,
                &move,
                0))
        {
            if (g_state.mode == SYS_MENU) {
                g_state.profile_id += move;
                if (g_state.profile_id < 0)
                    g_state.profile_id = 0;

                if (g_state.profile_id >= PROFILE_COUNT)
                    g_state.profile_id = PROFILE_COUNT - 1;
            }
        }
        if (g_state.mode == SYS_MENU) {
            if (gpio_get(BTN1_PIN) == 0) {

            }
            if (gpio_get(BTN2_PIN) == 0) {

            }
            if(gpio_get(ENCODER_BTN_PIN)) {
                g_state.mode = SYS_PREHEAT;
            }
        }
        if (g_state.mode == SYS_PREHEAT || g_state.mode == SYS_ROAST) {
            if (gpio_get(BTN1_PIN) == 0) {
                g_state.stop_flag = 1;
            }
            if (gpio_get(BTN2_PIN) == 0) {
                printf("Botão de emergência pressionado!\n");
                emergency_shutdown();
            }
            if(gpio_get(ENCODER_BTN_PIN)) {
                
            }
        }

        vTaskDelay(100);
    }
}