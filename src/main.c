#include <stdio.h>
#include "config.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "system_state.h"

// Free RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


extern QueueHandle_t encoderQueue;
extern uint32_t millis();
extern void core1_main();
extern void TaskDisplay_Control(void *p);
extern void TaskArtisan_uart(void *p);
extern void TaskUi(void *p);

int main () {
    stdio_init_all();

    sleep_ms(2000);

    encoderQueue = xQueueCreate(10, sizeof(uint32_t));
    if (!encoderQueue) panic("encoderQueue");

    multicore_launch_core1(core1_main);
    xTaskCreate(TaskDisplay_Control, "display", 1024, NULL, 2, NULL);
    xTaskCreate(TaskArtisan_uart, "artisan", 1024, NULL, 4, NULL);
    xTaskCreate(TaskUi, "ui_control", 1024, NULL, 3, NULL);
    vTaskStartScheduler();

    while (1) {
        if (gpio_get(BTN1_PIN) == 0) {
            //roast_loop(14); // 11
            //set_motor_power(0);
        }
        if (gpio_get(BTN2_PIN) == 0) {
            //update_display_info(g_state.bt, g_state.et, 6, "Emergencia");
            printf("Botão de emergência pressionado!\n");
            //emergency_shutdown();
        }
        sleep_ms(100);
    }

    // se chegar aqui, erro
    printf("Erro: scheduler retornou\n");
    while (1) tight_loop_contents();
    return 0;
}