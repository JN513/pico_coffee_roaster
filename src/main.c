#include <stdio.h>
#include <string.h>
#include "config.h"
#include "motor.h"
#include "control.h"
#include "temperature.h"
#include "pico/stdlib.h"
#include "max31865.h"
#include "time.h"

max31865_t sensor;

// RTD está na massa
// nao e bem linerar


uint32_t millis()
{
    return to_ms_since_boot(get_absolute_time());
}

float temp;

float kp_motor = 3.5;
float ki_motor = 0.02;
float kd_motor = 2.0;

float kp_resistance = 3.5;
float ki_resistance = 0.02;
float kd_resistance = 2.0;

float integral = 0;
float last_error = 0;

int fan_power = 100;

void control_temperature(int target)
{
    //float temp;
    max31865_read_celsius(&sensor, &temp);

    float error = target - temp;

    // ===== PID =====

    integral += error;

    if (integral > 800) integral = 800;
    if (integral < -800) integral = -800;

    float derivative = error - last_error;
    last_error = error;

    float pid_motor = kp_motor * error + ki_motor * integral + kd_motor * derivative;

    // limitar PID
    if (pid_motor > 100) pid_motor = 100;
    if (pid_motor < -100) pid_motor = -100;

    float pid_resistance = kp_resistance * error + ki_resistance * integral + kd_resistance * derivative;

    int base_power;

    if (temp < 120)
        base_power = 40;
    else if (temp < 150)
        base_power = 50;
    else
        base_power = 60;

    int resistance;

    // =========================
    // MODO 1 — abaixo de 180°C
    // =========================

    if (temp < 245)
    {
        fan_power = 100;

        resistance = (int)(base_power + pid_resistance);

        if (resistance > 100) resistance = 100;
        if (resistance < 0) resistance = 0;
    }

    // =========================
    // MODO 2 — acima de 180°C
    // =========================

    else
    {
        resistance = 100;

        // PID controla fan
        // erro positivo -> precisa aquecer -> menos vento
        // erro negativo -> precisa esfriar -> mais vento

        fan_power -= (int)(pid_motor * 0.5);

        if (fan_power > 100) fan_power = 100;
        if (fan_power < 70) fan_power = 70;
    }

    set_resistance_power(resistance);
    set_motor_power(fan_power);
}

void emergency_shutdown() {
    set_resistance_power(0);
    set_motor_power(100);
    printf("EMERGENCY SHUTDOWN ACTIVATED!\n");
    while(1) {
        sleep_ms(1000);
    }
}

void cooldown() {
    set_resistance_power(0);
    set_motor_power(100);
}


void pre_heat(){
    int target = 150;
    uint32_t last = 0;
    int initial_segundos = millis() / 1000;
    int segundos;

    const int pre_heat_duration = 60; // 1 minuto

    while (1)
    {
        if (millis() - last > 200)
        {
            last = millis();
            segundos = millis() / 1000 - initial_segundos;

            control_temperature(target);

            printf("Temp: %.2f °C, PID  M: %.2f PID R: %.2f, Fan: %d%% - Pré aquecendo!\n", 
                temp, kp_motor * (target - temp), kp_resistance * (target - temp), fan_power);


            if(temp > 150 && segundos > pre_heat_duration) {
                printf("Pre-aquecimento concluído!\n");
                break;
            }
        }
    }
}

void print_stage(int stage) {
    switch(stage) {
        case 0: printf("- Fase: Secagem\n"); break;
        case 1: printf("- Fase: Maillard\n"); break;
        case 2: printf("- Fase: Desenvolvimento\n"); break;
        case 3: printf("- Fase: Finalização\n"); break;
        case 4: printf("- Fase: Refrigeração\n"); break;
    }
}


int main () {
    stdio_init_all();

    sleep_ms(2000);

    printf("Iniciando sistema");

    init_thermistors();

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

    //set_resistance_power(100);

    init_resistance_control();

    set_resistance_power(0);

    printf("Sistema iniciado\n");

    sleep_ms(2000);

    printf("Iniciando torra\n");


    int target = 150;

    uint32_t last = 0;

    //cooldown();

    printf("Iniciando pré-aquecimento\n");

    pre_heat();

    sleep_ms(2000);

    printf("Iniciando perfil de torra\n");

    int initial_segundos = millis() / 1000;
    int segundos;

    while (1)
    {
        if (millis() - last > 200)
        {
            last = millis();
            segundos = millis() / 1000 - initial_segundos;

            target = get_bt_target(segundos);

            control_temperature(target);

            //printf("Temp: %.2f °C, PID  M: %.2f PID R: %.2f, Fan: %d%%\n", temp, kp_motor * (target - temp), kp_resistance * (target - temp), fan_power);

            printf("Temp: %.2f °C, Target: %d °C, Fan: %d%% ", temp, target, fan_power);
            print_stage(get_stage(segundos));

            if (temp > 250) {
                emergency_shutdown();
                break;
            }

            if (segundos > 960) { // 16 minutos
                printf("Perfil de torra concluído! Iniciando cooldown...\n");
                cooldown();
                break;
            }
        }
    }
}