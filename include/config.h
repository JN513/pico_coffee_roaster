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


#endif // !__CONFIG_H__
