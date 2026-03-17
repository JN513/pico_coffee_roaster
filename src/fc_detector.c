
#include "config.h"
#include <math.h>
#include <stdio.h>

#include "kiss_fftr.h"

#include "hardware/adc.h"
#include "pico/stdlib.h"

#define FFT_SIZE 256

static kiss_fftr_cfg cfg;

static float input[FFT_SIZE];

static kiss_fft_cpx output[FFT_SIZE/2 + 1];


void fc_fft_init() {

    cfg = kiss_fftr_alloc(
        FFT_SIZE,
        0,
        NULL,
        NULL
    );

    adc_gpio_init(MICROPHONE_PIN);
    adc_select_input(2);
}


void sample_audio() {
    for (int i = 0; i < FFT_SIZE; i++) {
        input[i] = (float)adc_read();
        sleep_us(100);
    }

}


void run_fft() {
    kiss_fftr(
        cfg,
        input,
        output
    );

}


float crack_energy() {
    float e = 0;

    // bins ~1kHz–4kHz

    for (int i = 10; i < 60; i++) {

        float re = output[i].r;
        float im = output[i].i;

        e += sqrtf(
            re * re +
            im * im
        );

    }

    return e;
}

bool detect_fc(float e) {
    static float avg = 0;

    avg = avg * 0.9 + e * 0.1;

    if (e > avg * 2.5)
        return true;

    return false;
}