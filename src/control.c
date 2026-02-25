#include <stdio.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "control.h"


float pid_compute(PID *pid, float setpoint, float measured, float dt)
{
    float error = setpoint - measured;

    // Proporcional
    float P = pid->kp * error;

    // Integral
    pid->integral += error * dt;
    float I = pid->ki * pid->integral;

    // Derivativo
    float D = pid->kd * (error - pid->last_error) / dt;

    float output = P + I + D;

    // Anti windup
    if (output > pid->out_max) {
        output = pid->out_max;
        pid->integral -= error * dt;
    }
    if (output < pid->out_min) {
        output = pid->out_min;
        pid->integral -= error * dt;
    }

    pid->last_error = error;

    return output;
}


float get_bt_target(int seconds)
{
    for (int i = 0; i < PROFILE_SIZE-1; i++) {
        if (seconds >= profile[i].time &&
            seconds < profile[i+1].time) {

            float t1 = profile[i].time;
            float t2 = profile[i+1].time;

            float v1 = profile[i].bt;
            float v2 = profile[i+1].bt;

            float ratio = (seconds - t1) / (t2 - t1);

            return v1 + ratio * (v2 - v1);
        }
    }

    return profile[PROFILE_SIZE-1].bt;
}

int fan_base_by_phase(int seconds)
{
    if (seconds < 120)   return 45; // secagem
    if (seconds < 300)   return 60; // maillard
    return 75;                  // desenvolvimento
}


