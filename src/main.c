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

max31865_t sensor;

uint32_t millis()
{
    return to_ms_since_boot(get_absolute_time());
}

float temp;

void pre_heat(){
    printf("Iniciando pré-aquecimento\n");
    uint32_t last = 0;

    while (1)
    {
        if (millis() - last > CONTROL_INTERVAL_MS)
        {
            last = millis();

            max31865_read_celsius(&sensor, &temp);
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

    while (temp > 40) {
        max31865_read_celsius(&sensor, &temp);
        printf("Temp: %.2f °C - Resfriando...\n", temp);
        sleep_ms(1000);
    }
    printf("Cooldown concluído!\n");
}

void roast_loop(){
    printf("Iniciando perfil de torra\n");

    set_motor_power(100);
    update_display(temp, temp, -2);
    pre_heat();
    update_display(temp, temp, 0);
    sleep_ms(2000);


    int target = 150;

    uint32_t last = 0;

    int segundos;
    int finish_time = get_finish_time();

    set_motor_power(100);

    int initial_segundos = millis() / 1000;

    while (1)
    {
        if (millis() - last > CONTROL_INTERVAL_MS)
        {
            last = millis();
            segundos = millis() / 1000 - initial_segundos;

            target = get_bt_target(segundos);

            max31865_read_celsius(&sensor, &temp);
            control_temperature(target, temp);


            printf("Temp: %.2f °C, Target: %d °C ", temp, target);
            print_stage(get_current_stage(segundos));

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

    while (1) {
        if (gpio_get(BTN1_PIN) == 0) {
            roast_loop();
        }
        if (gpio_get(BTN2_PIN) == 0) {
            update_display(temp, temp, -1);
            printf("Botão de emergência pressionado!\n");
            emergency_shutdown();
        }
        sleep_ms(100);
    }
}