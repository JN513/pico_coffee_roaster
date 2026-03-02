#ifndef MAX31865_H
#define MAX31865_H

#include <stdint.h>
#include <stdbool.h>
#include "hardware/spi.h"

// Estrutura do dispositivo
typedef struct {
    spi_inst_t *spi;
    uint cs_pin;
    uint rdy_pin;
    uint8_t config;
    float rref;
} max31865_t;

// Inicialização
void max31865_init(
    max31865_t *dev,
    spi_inst_t *spi,
    uint sck_pin,
    uint mosi_pin,
    uint miso_pin,
    uint cs_pin,
    uint rdy_pin,
    float rref_ohms
);

// Leitura de temperatura em graus Celsius
bool max31865_read_celsius(max31865_t *dev, float *temperature);

#endif