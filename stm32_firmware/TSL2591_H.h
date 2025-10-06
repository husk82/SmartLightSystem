#ifndef TSL2591_H
#define TSL2591_H

#include "stm32f446xx.h"
#include "I2C_Driver_H.h"
#include <math.h>

// I2C address for TSL2591
#define TSL2591_ADDR         0x29

// Command bits
#define TSL2591_COMMAND_BIT  0xA0

// Registers
#define TSL2591_ENABLE_REG   0x00
#define TSL2591_CONTROL_REG  0x01
#define TSL2591_STATUS_REG   0x13
#define TSL2591_CH0_LOW      0x14
#define TSL2591_CH0_HIGH     0x15
#define TSL2591_CH1_LOW      0x16
#define TSL2591_CH1_HIGH     0x17

// Enables
#define TSL2591_ENABLE_POWERON  0x01
#define TSL2591_ENABLE_AEN      0x02

#define TSL2591_ADC_MAX 37888
#define TSL2591_ADC_MARGIN 10

void TSL2591_init(I2C_TypeDef *I2Cx);
uint16_t TSL2591_read_ch0(I2C_TypeDef *I2Cx);
uint16_t TSL2591_read_ch1(I2C_TypeDef *I2Cx);
uint32_t TSL2591_calculate_lux(uint16_t ch0, uint16_t ch1);
uint8_t TSL2591_get_status(I2C_TypeDef *I2Cx);

#endif