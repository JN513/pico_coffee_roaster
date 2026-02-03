#include <stdio.h>
#include <string.h>
#include "config.h"
#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"


uint slice_num;
const uint16_t WRAP = 2500;   // ~5 kHz

void init_motor(){
    gpio_set_function(MOTOR_PIN, GPIO_FUNC_PWM);

    slice_num = pwm_gpio_to_slice_num(MOTOR_PIN);

    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, 10.0f);
    pwm_config_set_wrap(&config, WRAP);

    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(MOTOR_PIN, 0);
}

void set_motor_power(unsigned int percent) {
    if (percent > 100) percent = 100;

    // zona morta típica de 24V
    //if (percent > 0 && percent < 15)
    //    percent = 15;

    uint16_t level = (percent * WRAP) / 100;

    pwm_set_gpio_level(MOTOR_PIN, level);
}