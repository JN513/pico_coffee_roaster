#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "config.h"
#include "encoder.h"

extern QueueHandle_t encoderQueue;

static const uint32_t debounce_us = ENCODER_DEBOUNCE_TIME;
volatile uint32_t last_time = 0;

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
void TaskEncoder(void *p)
{
    uint8_t prev =
        (gpio_get(ENCODER_CLK_PIN) << 1) |
        gpio_get(ENCODER_DT_PIN);

    uint8_t last_read = prev;

    while (1)
    {
        uint8_t a1 = gpio_get(ENCODER_DT_PIN);
        uint8_t b1 = gpio_get(ENCODER_CLK_PIN);

        uint8_t first = (b1 << 1) | a1;

        vTaskDelay(pdMS_TO_TICKS(1));

        uint8_t a2 = gpio_get(ENCODER_DT_PIN);
        uint8_t b2 = gpio_get(ENCODER_CLK_PIN);

        uint8_t second = (b2 << 1) | a2;

        // debounce: só aceita se igual
        if (first != second)
        {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        uint8_t curr = second;

        // evita repetir mesmo estado
        if (curr == last_read)
        {
            vTaskDelay(pdMS_TO_TICKS(1));
            continue;
        }

        last_read = curr;

        uint8_t state = (prev << 2) | curr;

        int move = 0;

        switch (state)
        {
        case 0b0001:
        case 0b0111:
        case 0b1110:
        case 0b1000:
            move = 1;
            break;

        case 0b0010:
        case 0b0100:
        case 0b1101:
        case 0b1011:
            move = -1;
            break;
        }

        prev = curr;

        if (move != 0)
        {
            xQueueSend(
                encoderQueue,
                &move,
                0
            );
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
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

    // gpio_set_irq_enabled_with_callback(
    //     ENCODER_DT_PIN,
    //     GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
    //     true,
    //     &encoder_isr
    // );
// 
    // gpio_set_irq_enabled(
    //     ENCODER_CLK_PIN,
    //     GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
    //     true
    // );

}