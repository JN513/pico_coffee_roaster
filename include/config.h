#ifndef __CONFIG_H__
#define __CONFIG_H__

// --- Configurações de Hardware ---
#define ADC_PIN_A             26 // Termistor A (ET)
#define ADC_PIN_B             27 // Termistor B (BT)
#define MOTOR_PIN             19 // 22
#define ZERO_PHASE_DETECT_PIN 22 // 21
#define TRIAC_PIN             21 // 20

// --- Calibragem do Thermistor ---
#define SERIES_RESISTOR       10000.0f  
#define NOMINAL_RES           100000.0f  
#define NOMINAL_TEMP          25.0f
#define BETA                  3950.0f

#define ALPHA                 0.2 // Fator de suavização do filtro irr da temperatura

#define MAX_PIN_SCK           10
#define MAX_PIN_MOSI          11
#define MAX_PIN_MISO          8
#define MAX_PIN_CS            9
#define MAX_PIN_RDY           14

#define RELEY_PIN             20 // 18
#define FAN_PIN               18

#define BTN1_PIN              3
#define BTN2_PIN              6

#define ENCODER_BTN_PIN       2
#define ENCODER_DT_PIN        4
#define ENCODER_CLK_PIN       5

#define OLED_SDA_PIN          0
#define OLED_SCL_PIN          1

#define PRE_HEAT_DURATION     60 // Duração do pré-aquecimento em segundos
#define PRE_HEAT_TEMP         150 // Temperatura alvo para o pré-aquecimento

#define CONTROL_INTERVAL_MS   200 // Intervalo de controle em milissegundos

#define MAX_TEMP              250 // Temperatura máxima de segurança

#define COOLDOWN_MIN_TEMP     35

#define DISPLAY_STACK_SIZE    1024
#define ARTISAN_STACK_SIZE    1024
#define UI_TASK_STACK_SIZE    512 

#define DISPLAY_TASK_PRIORITY 2
#define ARTISAN_TASK_PRIORITY 4
#define UI_TASK_PRIORITY      3 

#define ENCODER_QUEUE_SIZE    10
#define ENCODER_DEBOUNCE_TIME 10000

#define KP                    3.5
#define KI                    0.02
#define KD                    2.0

#endif // !__CONFIG_H__
