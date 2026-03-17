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


void update_display_info(float tempA, float tempB, int stage, char * profile_name) {
    ssd1306_clear(&disp);

    char line1[20];
    char line2[20];
    char line3[20];
    char line4[20];
    char line5[20];
    char line6[20];

    snprintf(line1, sizeof(line1), "Realizando torra ");
    snprintf(line2, sizeof(line2), "com perfil:");
    snprintf(line3, sizeof(line3), "%s", profile_name);
    snprintf(line4, sizeof(line4), "Temp Massa: %.2f C", tempA);
    snprintf(line5, sizeof(line5), "Temp Ar: %.2f C", tempB);


    switch (stage) {
        case 0: snprintf(line6, sizeof(line6), "Fase: Secagem"); break;
        case 1: snprintf(line6, sizeof(line6), "Fase: Maillard"); break;
        case 2: snprintf(line6, sizeof(line6), "Fase: Desenvolvimen"); break;
        case 3: snprintf(line6, sizeof(line6), "Fase: Finalizacao"); break;
        case 4: snprintf(line6, sizeof(line6), "Fase: Refrigeracao"); break;
        case 5: snprintf(line6, sizeof(line6), "Fase: Pre aqueci..."); break;
        case 6: snprintf(line6, sizeof(line6), "Emergencia!!!"); break;
    }

    ssd1306_draw_string(&disp, 0, 00, 1, line1);
    ssd1306_draw_string(&disp, 0, 10, 1, line2);
    ssd1306_draw_string(&disp, 0, 20, 1, line3);
    ssd1306_draw_string(&disp, 0, 30, 1, line4);
    ssd1306_draw_string(&disp, 0, 40, 1, line5);
    ssd1306_draw_string(&disp, 0, 50, 1, line6);

    ssd1306_show(&disp);
}

void print_info(char *line1, char *line2){
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 1, "Pico Roaster!");
    ssd1306_draw_string(&disp, 0, 10, 1, line1);
    ssd1306_draw_string(&disp, 0, 20, 1, line2);

    ssd1306_show(&disp);
}

void print_menu(int profile_id, char * profile_name, int profile_duration, RoastType profile_type) {
    ssd1306_clear(&disp);

    char line1[20];
    char line2[20];
    char line3[20];
    char line4[20];
    char line5[20];
    char line6[20];

    snprintf(line1, sizeof(line1), "Perfil n: %d Nome:", profile_id);
    snprintf(line2, sizeof(line2), "%s", profile_name);
    snprintf(line3, sizeof(line3), "Tipo de Torra:");

    switch (profile_type) {
        case ROAST_LIGHT:
            snprintf(line4, sizeof(line4), "Torra clara");
            break;
        case ROAST_MEDIUM:
            snprintf(line4, sizeof(line4), "Torra media");
            break;
        case ROAST_MEDIUM_DARK:
            snprintf(line4, sizeof(line4), "Torra media escura");
            break;
        case ROAST_DARK:
            snprintf(line4, sizeof(line4), "Torra escura");
            break;
    }

    snprintf(line5, sizeof(line5), "Tempo de torra:");
    snprintf(line6, sizeof(line6), "%d minutos", profile_duration/60);

    ssd1306_draw_string(&disp, 0, 00, 1, line1);
    ssd1306_draw_string(&disp, 0, 10, 1, line2);
    ssd1306_draw_string(&disp, 0, 20, 1, line3);
    ssd1306_draw_string(&disp, 0, 30, 1, line4);
    ssd1306_draw_string(&disp, 0, 40, 1, line5);
    ssd1306_draw_string(&disp, 0, 50, 1, line6);

    ssd1306_show(&disp);
}