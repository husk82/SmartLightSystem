#include "I2C_Driver_H.h"

#define TIMEOUT 10000

I2C_Status_t I2C_init(I2C_TypeDef *I2Cx, uint32_t pclk1, uint32_t i2c_speed)
{
	if (!I2Cx || pclk1 == 0 || i2c_speed == 0 || i2c_speed > 400000)
		return I2C_ERR_INVALID_SPEED;

	if (I2Cx == I2C1)
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	else if (I2Cx == I2C2)
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	else if (I2Cx == I2C3)
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;

	I2Cx->CR1 |= I2C_CR1_SWRST;
	I2Cx->CR1 &= ~I2C_CR1_SWRST;

	I2Cx->CR2 = (pclk1 / 1000000U);

	if (i2c_speed <= 100000) {
		I2Cx->CCR = pclk1 / (i2c_speed * 2);
		I2Cx->TRISE = (pclk1 / 1000000U) + 1;
	} else {
		I2Cx->CCR = (pclk1 / (3 * i2c_speed)) | I2C_CCR_FS;
		I2Cx->TRISE = ((pclk1 / 1000000U) * 300 / 1000) + 1;
	}

	I2Cx->CR1 |= I2C_CR1_PE;

	return I2C_OK;
}

I2C_Status_t I2C_start(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t direction)
{
	uint32_t timeout = TIMEOUT;

	I2Cx->CR1 |= I2C_CR1_START;
	while (!(I2Cx->SR1 & I2C_SR1_SB) && --timeout);
	if (!timeout) return I2C_ERR_TIMEOUT;

	I2Cx->DR = (addr << 1) | direction;

	timeout = TIMEOUT;
	while (!(I2Cx->SR1 & I2C_SR1_ADDR) && --timeout);
	if (!timeout) return I2C_ERR_TIMEOUT;

	(void)I2Cx->SR1;
	(void)I2Cx->SR2;

	return I2C_OK;
}

void I2C_write(I2C_TypeDef *I2Cx, uint8_t data)
{
	while (!(I2Cx->SR1 & I2C_SR1_TXE));
	I2Cx->DR = data;
	while (!(I2Cx->SR1 & I2C_SR1_BTF));
}

uint8_t I2C_read_ack(I2C_TypeDef *I2Cx)
{
	I2Cx->CR1 |= I2C_CR1_ACK;
	while (!(I2Cx->SR1 & I2C_SR1_RXNE));
	return I2Cx->DR;
}

uint8_t I2C_read_nack(I2C_TypeDef *I2Cx)
{
	I2Cx->CR1 &= ~I2C_CR1_ACK;
	I2C_stop(I2Cx);
	while (!(I2Cx->SR1 & I2C_SR1_RXNE));
	return I2Cx->DR;
}

void I2C_stop(I2C_TypeDef *I2Cx)
{
	I2Cx->CR1 |= I2C_CR1_STOP;
}