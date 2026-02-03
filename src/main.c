#include <stdio.h>
#include <string.h>
#include "config.h"
#include "motor.h"
#include "temperature.h"
#include "pico/stdlib.h"


int main () {
    stdio_init_all();

    sleep_ms(2000);

    printf("Iniciando sistema");

    init_thermistors();

    init_motor();

    init_resistance_control();

    printf("Sistema iniciado");

    set_motor_power(100);

    set_resistance_power(50);

    while (true)
    {
        sleep_ms(1000);
        printf("Rodando");
    }
    
}