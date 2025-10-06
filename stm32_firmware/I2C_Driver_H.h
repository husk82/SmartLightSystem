#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f4xx.h"
#include <stdint.h>

// Directions
#define I2C_WRITE 0
#define I2C_READ  1

// Error codes
typedef enum {
	I2C_OK = 0,
	I2C_ERR_TIMEOUT,
	I2C_ERR_NULLPTR,
	I2C_ERR_INVALID_SPEED
} I2C_Status_t;

// APIs
I2C_Status_t I2C_init(I2C_TypeDef *I2Cx, uint32_t pclk1, uint32_t i2c_speed);
I2C_Status_t I2C_start(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t direction);
void I2C_write(I2C_TypeDef *I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef *I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef *I2Cx);
void I2C_stop(I2C_TypeDef *I2Cx);

#endif