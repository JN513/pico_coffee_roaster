#include "max31865.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <math.h>

// Registradores
#define REG_CONFIG      0x00
#define REG_RTD_MSB     0x01
#define REG_FAULT_STAT  0x07

// Configuração
#define CONFIG_BIAS     0x80
#define CONFIG_1SHOT    0x20
#define CONFIG_3WIRE    0x10
#define CONFIG_FAULTCLR 0x02
#define CONFIG_50HZ     0x01

// Constantes IEC 60751 (PT100)
#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7
#define RTD_C -4.183e-12

static inline void cs_select(max31865_t *dev, bool enable) {
    gpio_put(dev->cs_pin, enable ? 0 : 1);
}

static uint8_t spi_rw(max31865_t *dev, uint8_t data) {
    uint8_t rx;
    spi_write_read_blocking(dev->spi, &data, &rx, 1);
    return rx;
}

static void write_reg(max31865_t *dev, uint8_t reg, uint8_t value) {
    cs_select(dev, true);
    spi_rw(dev, reg | 0x80);
    spi_rw(dev, value);
    cs_select(dev, false);
}

static uint16_t read_rtd(max31865_t *dev) {
    uint8_t msb, lsb;

    // Liga bias
    write_reg(dev, REG_CONFIG, dev->config | CONFIG_BIAS);
    sleep_ms(10);

    // One-shot
    write_reg(dev, REG_CONFIG, dev->config | CONFIG_BIAS | CONFIG_1SHOT);
    sleep_ms(70);

    cs_select(dev, true);
    spi_rw(dev, REG_RTD_MSB);
    msb = spi_rw(dev, 0xFF);
    lsb = spi_rw(dev, 0xFF);
    cs_select(dev, false);

    // Desliga bias
    write_reg(dev, REG_CONFIG, dev->config);

    return ((uint16_t)msb << 8 | lsb) >> 1;
}

// Callendar–Van Dusen
static float rtd_to_celsius(float rtd_ohms) {
    float Z1 = -RTD_A;
    float Z2 = RTD_A * RTD_A - (4 * RTD_B);
    float Z3 = (4 * RTD_B) / 100.0f;
    float Z4 = 2 * RTD_B;

    float temp = (sqrtf(Z2 + (Z3 * rtd_ohms)) + Z1) / Z4;

    if (temp >= 0)
        return temp;

    // Correção para temperaturas negativas
    float r = rtd_ohms / 100.0f;
    return -242.02f + 2.2228f * r + 2.5859e-3f * r * r
           - 4.8260e-6f * r * r * r
           - 2.8183e-8f * r * r * r * r
           + 1.5243e-10f * r * r * r * r * r;
}

void max31865_init(
    max31865_t *dev,
    spi_inst_t *spi,
    uint sck_pin,
    uint mosi_pin,
    uint miso_pin,
    uint cs_pin,
    uint rdy_pin,
    float rref_ohms
) {
    dev->spi = spi;
    dev->cs_pin = cs_pin;
    dev->rref = rref_ohms;
    dev->config = CONFIG_3WIRE | CONFIG_50HZ;

    spi_init(spi, 5 * 1000 * 1000);
    spi_set_format(spi, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);

    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(miso_pin, GPIO_FUNC_SPI);

    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);

    write_reg(dev, REG_CONFIG, dev->config | CONFIG_FAULTCLR);
}

bool max31865_read_celsius(max31865_t *dev, float *temperature) {
    uint16_t raw = read_rtd(dev);
    if (raw == 0) return false;

    float resistance = (raw * dev->rref) / 32768.0f;
    *temperature = rtd_to_celsius(resistance);
    return true;
}