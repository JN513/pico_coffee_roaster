#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "config.h"
#include "encoder.h"

extern QueueHandle_t encoderQueue;

#define ENC_A ENCODER_DT_PIN
#define ENC_B ENCODER_CLK_PIN

static const uint32_t debounce_us = 10000;
static uint32_t last_time = 0;

static const uint8_t quad_table[16] = {
    0, +1, -1, 0,
    -1, 0, 0, +1,
    +1, 0, 0, -1,
    0, -1, +1, 0
};

volatile uint8_t prev_state = 0;


void encoder_isr(uint gpio, uint32_t events)
{

    uint32_t now = time_us_32();

    if (now - last_time < debounce_us)
        return;

    last_time = now;

    uint8_t a = gpio_get(ENC_A);
    uint8_t b = gpio_get(ENC_B);

    uint8_t curr = (b << 1) | a;

    uint8_t index = (prev_state << 2) | curr;

    int8_t movement = quad_table[index];

    prev_state = curr;

    xQueueSendFromISR(
        encoderQueue,
        &movement,
        NULL
    );

}


void encoder_init() {

    gpio_init(ENC_A);
    gpio_set_dir(ENC_A, GPIO_IN);
    gpio_pull_up(ENC_A);

    gpio_init(ENC_B);
    gpio_set_dir(ENC_B, GPIO_IN);
    gpio_pull_up(ENC_B);

    gpio_init(ENCODER_BTN_PIN);
    gpio_set_dir(ENCODER_BTN_PIN, GPIO_IN);

    prev_state = (gpio_get(ENC_B) << 1) | gpio_get(ENC_A);

    gpio_set_irq_enabled_with_callback(
        ENC_A,
        GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
        true,
        &encoder_isr
    );

    gpio_set_irq_enabled(
        ENC_B,
        GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
        true
    );

}