#ifndef __CONFIG_H__
#define __CONFIG_H__

// --- Configurações de Hardware ---
#define ADC_PIN_A             26 // Termistor A (ET)
#define ADC_PIN_B             27 // Termistor B (BT)
#define MOTOR_PIN             22
#define ZERO_PHASE_DETECT_PIN 21
#define TRIAC_PIN             20

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

#define RELEY_PIN             18
#define FAN_PIN               19


#endif // !__CONFIG_H__
