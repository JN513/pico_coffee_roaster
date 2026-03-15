#include <stdio.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "control.h"
#include "temperature.h"
#include "max31865.h"
#include "motor.h"
#include "profiles.h"

float integral = 0;
float last_error = 0;

void control_temperature(int target, float temperature) {
    float error = target - temperature;

    integral += error;

    if (integral > 800) integral = 800;
    if (integral < -800) integral = -800;

    float derivative = error - last_error;
    last_error = error;


    float pid_resistance = KP * error + KI * integral + KD * derivative;

    int base_power;

    if (temperature < 120)
        base_power = 40;
    else if (temperature < 150)
        base_power = 50;
    else
        base_power = 60;

    int resistance;
    
    resistance = base_power + pid_resistance;
    
    if (resistance > 100) resistance = 100;
    
    if (resistance < 0) resistance = 0;


    set_resistance_power(resistance);
}


float get_bt_target(int seconds, ProfilePoint * profile_array, 
    int profile_size)
{
    for (int i = 0; i < profile_size-1; i++) {
        if (seconds >= profile_array[i].time &&
            seconds < profile_array[i+1].time) {

            float t1 = profile_array[i].time;
            float t2 = profile_array[i+1].time;

            float v1 = profile_array[i].bt;
            float v2 = profile_array[i+1].bt;

            float ratio = (seconds - t1) / (t2 - t1);

            return v1 + ratio * (v2 - v1);
        }
    }

    return profile_array[profile_size-1].bt;
}

int get_current_stage(int seconds, ProfilePoint * profile_array, 
    int profile_size) {
    for (int i = 0; i < profile_size-1; i++) {
        if (seconds >= profile_array[i].time &&
            seconds < profile_array[i+1].time) {
            return i;
        }
    }
    return profile_size-1;
}


void emergency_shutdown() {
    set_resistance_power(0);
    set_motor_power(100);
    printf("EMERGENCY SHUTDOWN ACTIVATED!\n");
    while(1) {
        sleep_ms(1000);
    }
}
