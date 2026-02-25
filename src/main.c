#include <stdio.h>
#include <string.h>
#include "config.h"
#include "motor.h"
#include "control.h"
#include "temperature.h"
#include "pico/stdlib.h"


void roast_control_loop()
{
    PID pid_et = {
        .kp = 2.0,
        .ki = 0.05,
        .kd = 5.0,
        .out_min = 0,
        .out_max = 100
    };

    float filtA = 25;
    float filtB = 25;

    int seconds = 0;

    const float DT = 0.3; // 300 ms

    while (1) {

        // ---- LEITURAS ----
        float bt = filter(read_tempA(), &filtA);
        float et = filter(read_tempB(), &filtB);

        // ---- SEGURANÇA ----
        if (et > 260 || bt > 230) {
            set_resistance_power(0);
            set_motor_power(100);
            printf("EMERGENCIA!\n");
            break;
        }

        // ---- ALVO DA MASSA ----
        float bt_target = get_bt_target(seconds);

        // ---- CASCATA ----
        // ET deve ficar um pouco acima da BT
        float et_setpoint = bt_target + 30.0;

        // ---- PID DO AR ----
        float power = pid_compute(&pid_et, et_setpoint, et, DT);

        set_resistance_power((int)power);

        // ---- VENTILADOR ----
        int fan = fan_base_by_phase(seconds);

        // correções simples
        float ror = (bt - filtA) / DT;

        if (ror > 0.3) fan += 5;
        if (ror < 0.1) fan -= 5;

        if (fan < 30) fan = 30;
        if (fan > 100) fan = 100;

        set_motor_power(fan);

        // ---- LOG ----
        printf("%d, BT=%.1f, ET=%.1f, PWR=%d, FAN=%d\n",
            seconds, bt, et, (int)power, fan);

        sleep_ms((int)(DT * 1000));
        seconds += DT;
    }
}


int main () {
    stdio_init_all();

    sleep_ms(2000);

    printf("Iniciando sistema");

    init_thermistors();

    init_motor();

    set_resistance_power(50);

    init_resistance_control();

    printf("Sistema iniciado");

    set_motor_power(70);
    
    sleep_ms(2000);

    printf("Iniciando torra\n");

    //roast_control_loop();

    while(1);
}