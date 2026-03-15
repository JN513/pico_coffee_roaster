#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>
#include <stdint.h>

#define ENC_A 3
#define ENC_B 4

static const uint32_t debounce_us = 10000;
static uint32_t last_time = 0;
extern QueueHandle_t encoderQueue; // criado no main
static const uint8_t quad_table[16] = {
    0, +1, -1, 0,
    -1, 0, 0, +1,
    +1, 0, 0, -1,
    0, -1, +1, 0
};
volatile uint8_t prev_state = 0;

void encoder_isr(uint gpio, uint32_t events) {
    uint32_t now = time_us_32();
    if (now - last_time < debounce_us) return;
    last_time = now;

    uint8_t a = gpio_get(ENC_A);
    uint8_t b = gpio_get(ENC_B);
    uint8_t curr = (b << 1) | a;

    uint8_t index = (prev_state << 2) | curr;
    int8_t movement = quad_table[index];

    prev_state = curr;

    xQueueSendFromISR(encoderQueue, &movement, NULL);
}