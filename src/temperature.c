#include <stdio.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "temperature.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/clocks.h" 

volatile int current_power = 0;

// Temperature Read
float adc_to_temp(uint16_t adc_val) {
    if (adc_val == 0) return 0; // Evita divisão por zero
    
    float v = adc_val * 3.3f / 4095.0f;
    float r = SERIES_RESISTOR * (3.3f / v - 1.0f);

    float steinhart = r / NOMINAL_RES;
    steinhart = logf(steinhart);
    steinhart /= BETA;
    steinhart += 1.0f / (NOMINAL_TEMP + 273.15f);
    steinhart = 1.0f / steinhart;
    steinhart -= 273.15f; // Celsius

    return steinhart;
}


void init_thermistors(){
    adc_init();
    adc_gpio_init(ADC_PIN_A);
    adc_gpio_init(ADC_PIN_B);
}

float read_tempA(){
    adc_select_input(0);
    return adc_to_temp(adc_read());
}

float read_tempB(){
    adc_select_input(1);
    return adc_to_temp(adc_read());
}

float filter(float new_value, float *state){
    if(!isfinite(new_value))
        return *state;

    *state = ALPHA * new_value + (1 - ALPHA) * (*state);
    return *state;
}

// ===== Gera pulso no MOC (chamado pelo alarme) =====
int64_t alarm_callback(alarm_id_t id, void *user_data) {
    gpio_put(TRIAC_PIN, 1);
    busy_wait_us_32(200);      // espera não bloqueante do core
    gpio_put(TRIAC_PIN, 0);
    return 0;
}

// ===== Interrupção do zero-cross =====
void zero_cross_callback(uint gpio, uint32_t events) {
    uint32_t p = current_power;

    // 60 Hz → semiciclo = 8333 µs
    int atraso = (100 - p) * 83;

    if (atraso < 250) atraso = 250;
    if (atraso > 8000) return;

    add_alarm_in_us(atraso, alarm_callback, NULL, true);
}


void init_resistance_control(){
    gpio_init(RELEY_PIN);
    gpio_set_dir(RELEY_PIN, GPIO_OUT);
    gpio_put(RELEY_PIN, 0);

    gpio_init(ZERO_PHASE_DETECT_PIN);
    gpio_set_dir(ZERO_PHASE_DETECT_PIN, GPIO_IN);

    gpio_init(TRIAC_PIN);
    gpio_set_dir(TRIAC_PIN, GPIO_OUT);
    gpio_put(TRIAC_PIN, 0);

    gpio_set_irq_enabled_with_callback(
        ZERO_PHASE_DETECT_PIN,
        GPIO_IRQ_EDGE_RISE,
        true,
        &zero_cross_callback
    );
}

void set_resistance_power(int power){
    if(power > 0) {
        gpio_put(RELEY_PIN, 1);
    } else {
        gpio_put(RELEY_PIN, 0);
    }

    if (power < 0) power = 0;
    if (power > 100) power = 100;

    current_power = power;
}