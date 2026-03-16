#include <stdio.h>
#include <string.h>
#include "config.h"
#include "motor.h"
#include "control.h"
#include "temperature.h"
#include "pico/stdlib.h"
#include "max31865.h"
#include "time.h"
#include "display.h"
#include "profiles.h"

// Free RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"


max31865_t sensor;

uint32_t millis()
{
    return to_ms_since_boot(get_absolute_time());
}

float temp;
float ar_temp;
bool unit_F = false;

void pre_heat(){
    printf("Iniciando pré-aquecimento\n");
    uint32_t last = 0;

    while (1)
    {
        if (millis() - last > CONTROL_INTERVAL_MS)
        {
            last = millis();

            max31865_read_celsius(&sensor, &temp);
            ar_temp = read_tempA();
            control_temperature(PRE_HEAT_TEMP, temp);

            printf("Temp: %.2f °C - Pré aquecendo!\n", temp);


            if(temp >= PRE_HEAT_TEMP) {
                printf("Pre-aquecimento concluído!\n");
                break;
            }
        }
    }
}

void cooldown() {
    printf("Iniciando cooldown...\n");
    set_motor_power(100);
    set_resistance_power(0);

    while (temp > COOLDOWN_MIN_TEMP) {
        max31865_read_celsius(&sensor, &temp);
        ar_temp = read_tempA();
        printf("Temp: %.2f °C - Resfriando...\n", temp);
        sleep_ms(1000);
    }
    printf("Cooldown concluído!\n");
}

void roast_loop(int profile_id){
    char * profile_name = get_profile_name(profile_id);

    printf("Iniciando perfil de torra: %s\n", profile_name);

    set_motor_power(100);
    update_display(temp, ar_temp, 5, profile_name); // temp grao, temp ar, stage id, profile name
    pre_heat();
    update_display(temp, ar_temp, 0, profile_name);
    sleep_ms(2000);

    int target = 150;

    uint32_t last = 0;


    printf("Usando profile: %s\n", profile_name);

    ProfilePoint * profile_array = get_profile_pointer(profile_id);
    int profile_size = get_profile_size(profile_id);

    int segundos;
    int finish_time = get_profile_finish_time(profile_id);

    set_motor_power(100);

    int initial_segundos = millis() / 1000;

    while (1)
    {
        if (millis() - last > CONTROL_INTERVAL_MS)
        {
            last = millis();
            segundos = millis() / 1000 - initial_segundos;

            target = get_bt_target(segundos, profile_array, profile_size);

            max31865_read_celsius(&sensor, &temp);
            ar_temp = read_tempA();
            control_temperature(target, temp);


            printf("Temp: %.2f °C, Target: %d °C ", temp, target);
            print_stage(get_current_stage(segundos, profile_array, profile_size));

            if (temp > MAX_TEMP) {
                emergency_shutdown();
                break;
            }

            if (segundos >= finish_time) {
                printf("Perfil de torra concluído!\n");
                break;
            }

            if (segundos >= 960){
                printf("Aquecimento concluído! Iniciando cooldown...\n");
                break;
            }
        }
    }

    cooldown();
}

void command_read() {
    // Formato TC4: Ambiência, Temp1, Temp2, Saída1, Saída2
    printf("0.00,%.2f,%.2f,0.00,0.00\n", temp, ar_temp);
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

int main () {
    stdio_init_all();

    sleep_ms(2000);

    printf("Iniciando sistema");

    init_thermistors();

    init_display();

    max31865_init(
        &sensor,
        spi1,
        MAX_PIN_SCK,
        MAX_PIN_MOSI,
        MAX_PIN_MISO,
        MAX_PIN_CS,
        MAX_PIN_RDY,
        430.0f   // Rref em ohms (valor típico do breakout)
    );

    init_motor();

    set_motor_power(100);

    printf("Motor ligado\n");

    sleep_ms(10000);

    printf("Iniciando resistência\n");

    init_resistance_control();

    set_resistance_power(0);

    printf("Sistema iniciado\n");

    set_motor_power(0);

    gpio_init(BTN1_PIN);
    gpio_set_dir(BTN1_PIN, GPIO_IN);

    gpio_init(BTN2_PIN);
    gpio_set_dir(BTN2_PIN, GPIO_IN);

    sleep_ms(2000);

    max31865_read_celsius(&sensor, &temp);
    ar_temp = read_tempA();

    while (1) {
        if (gpio_get(BTN1_PIN) == 0) {
            roast_loop(1);
            set_motor_power(0);
        }
        if (gpio_get(BTN2_PIN) == 0) {
            update_display(temp, ar_temp, 6, "Emergencia");
            printf("Botão de emergência pressionado!\n");
            emergency_shutdown();
        }
        sleep_ms(100);
    }
}