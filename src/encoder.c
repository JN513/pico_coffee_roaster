#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "config.h"
#include "encoder.h"

extern QueueHandle_t encoderQueue;

static const uint32_t debounce_us = ENCODER_DEBOUNCE_TIME;
uint32_t last_time = 0;

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

    uint8_t a = gpio_get(ENCODER_DT_PIN);
    uint8_t b = gpio_get(ENCODER_CLK_PIN);

    uint8_t curr = (b << 1) | a;

    uint8_t index = (prev_state << 2) | curr;

    int movement = quad_table[index];

    prev_state = curr;

    xQueueSendFromISR(
        encoderQueue,
        &movement,
        NULL
    );

}

void encoder_init() {

    gpio_init(ENCODER_DT_PIN);
    gpio_set_dir(ENCODER_DT_PIN, GPIO_IN);
    gpio_pull_up(ENCODER_DT_PIN);

    gpio_init(ENCODER_CLK_PIN);
    gpio_set_dir(ENCODER_CLK_PIN, GPIO_IN);
    gpio_pull_up(ENCODER_CLK_PIN);

    gpio_init(ENCODER_BTN_PIN);
    gpio_set_dir(ENCODER_BTN_PIN, GPIO_IN);

    prev_state = (gpio_get(ENCODER_CLK_PIN) << 1) | gpio_get(ENCODER_DT_PIN);

    gpio_set_irq_enabled_with_callback(
        ENCODER_DT_PIN,
        GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
        true,
        &encoder_isr
    );

    gpio_set_irq_enabled(
        ENCODER_CLK_PIN,
        GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
        true
    );

}