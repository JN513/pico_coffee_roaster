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

    encoderQueue = xQueueCreate(ENCODER_QUEUE_SIZE, sizeof(uint32_t));
    if (!encoderQueue) panic("encoderQueue");

    multicore_launch_core1(core1_main);
    xTaskCreate(TaskDisplay_Control, "display", DISPLAY_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, NULL);
    xTaskCreate(TaskArtisan_uart, "artisan", ARTISAN_STACK_SIZE, NULL, ARTISAN_TASK_PRIORITY, NULL);
    xTaskCreate(TaskUi, "ui_control", UI_TASK_STACK_SIZE, NULL, UI_TASK_PRIORITY, NULL);
    vTaskStartScheduler();

    // se chegar aqui, erro
    printf("Erro: scheduler retornou\n");
    while (1) tight_loop_contents();
    return 0;
}