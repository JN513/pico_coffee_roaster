#include "display.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "config.h"

ssd1306_t disp;

void init_display() {
    i2c_init(i2c0, 400000); // 400 kHz
    gpio_set_function(OLED_SDA_PIN, GPIO_FUNC_I2C); // SDA
    gpio_set_function(OLED_SCL_PIN, GPIO_FUNC_I2C); // SCL
    gpio_pull_up(OLED_SDA_PIN);
    gpio_pull_up(OLED_SCL_PIN);

    disp.external_vcc=false;

    ssd1306_init(&disp, 128, 64, 0x3C, i2c0);

    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 1, "Hello Roaster!");
    ssd1306_show(&disp);
}

void update_display(float tempA, float tempB, int stage) {
    ssd1306_clear(&disp);

    char line1[20];
    char line2[20];
    char line3[20];

    snprintf(line1, sizeof(line1), "Temp Massa: %.2f C", tempA);
    snprintf(line2, sizeof(line2), "Temp Ar: %.2f C", tempB);
    snprintf(line3, sizeof(line3), "Stage: %d", stage);

    ssd1306_draw_string(&disp, 0, 0, 1, line1);
    ssd1306_draw_string(&disp, 0, 10, 1, line2);
    ssd1306_draw_string(&disp, 0, 20, 1, line3);

    ssd1306_show(&disp);
}