#include <stdio.h>
#include "config.h"
#include "motor.h"
#include "control.h"
#include "temperature.h"
#include "pico/stdlib.h"
#include "max31865.h"
#include "time.h"
#include "profiles.h"
#include "system_state.h"

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


void pre_heat(){
    //printf("Iniciando pré-aquecimento\n");
    uint32_t last = 0;

    while (1)
    {
        if (millis() - last > CONTROL_INTERVAL_MS)
        {
            if(g_state.stop_flag || g_state.mode == SYS_EMERGENCY){
                return;
            }
            last = millis();

            max31865_read_celsius(&sensor, &g_state.bt);
            g_state.et = read_tempA();
            control_temperature(PRE_HEAT_TEMP, g_state.bt);

            //printf("Temp: %.2f °C - Pré aquecendo!\n", g_state.bt);


            if(g_state.bt >= PRE_HEAT_TEMP) {
                //printf("Pre-aquecimento concluído!\n");
                break;
            }
        }
    }
}

void cooldown() {
    //printf("Iniciando cooldown...\n");
    set_motor_power(100);
    set_resistance_power(0);

    while (g_state.bt > COOLDOWN_MIN_TEMP) {
        max31865_read_celsius(&sensor, &g_state.bt);
        g_state.et = read_tempA();
        //printf("Temp: %.2f °C - Resfriando...\n", g_state.bt);
        sleep_ms(1000);
    }
    //printf("Cooldown concluído!\n");
}

void roast_loop(int profile_id){
    //char * profile_name = get_profile_name(profile_id);

    //printf("Iniciando perfil de torra: %s\n", profile_name);

    set_motor_power(100);
    //update_display_info(g_state.bt, g_state.et, 5, profile_name); // temp grao, temp ar, stage id, profile name
    pre_heat();
    //update_display_info(g_state.bt, g_state.et, 0, profile_name);
    sleep_ms(2000);

    int target = 150;

    uint32_t last = 0;


    //printf("Usando profile: %s\n", profile_name);

    ProfilePoint * profile_array = get_profile_pointer(profile_id);
    int profile_size = get_profile_size(profile_id);

    int segundos;
    int finish_time = get_profile_finish_time(profile_id);

    int use_variable_fan = get_profile_use_variable_fan(profile_id);

    set_motor_power(100);

    int initial_segundos = millis() / 1000;

    while (1)
    {
        if(g_state.mode == SYS_EMERGENCY) return;
        if(g_state.stop_flag){
            g_state.stop_flag = 0;
            break;
        }
        if (millis() - last > CONTROL_INTERVAL_MS)
        {
            last = millis();
            segundos = millis() / 1000 - initial_segundos;

            target = get_bt_target(segundos, profile_array, profile_size);

            max31865_read_celsius(&sensor, &g_state.bt);
            g_state.et = read_tempA();
            control_temperature(target, g_state.bt);


            //printf("Temp: %.2f °C, Target: %d °C ", g_state.bt, target);
            int current_stage = get_current_stage(segundos, profile_array, profile_size);
            //print_stage(current_stage);

            int power = motor_speed_by_phase(current_stage);

            if (g_state.bt > MAX_TEMP) {
                emergency_shutdown();
                break;
            }

            if (segundos >= finish_time) {
                //printf("Perfil de torra concluído!\n");
                break;
            }

            if (segundos >= 960){
                //printf("Aquecimento concluído! Iniciando cooldown...\n");
                break;
            }

            if(use_variable_fan)
                set_motor_power(power);
        }
    }

    g_state.mode = SYS_COOLDOWN;
    cooldown();
}


void core1_main(){
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

    init_resistance_control();

    set_resistance_power(0);

    printf("Sistema iniciado\n");

    set_motor_power(0);

    sleep_ms(2000);

    max31865_read_celsius(&sensor, &g_state.bt);
    g_state.et = read_tempA();
    
    g_state.mode = SYS_IDLE;

    sleep_ms(100);

    while (1)
    {
        if(g_state.mode == SYS_PREHEAT){
            roast_loop(g_state.profile_id);
            if(g_state.mode != SYS_EMERGENCY) {
                g_state.mode = SYS_IDLE;
                set_motor_power(0);
            }
        }

        sleep_ms(200);
    }
}