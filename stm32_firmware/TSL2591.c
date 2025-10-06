#include "TSL2591_H.h"

void TSL2591_write(I2C_TypeDef *I2Cx, uint8_t reg, uint8_t data) 
{
	uint8_t cmd = TSL2591_COMMAND_BIT | reg;
	I2C_start(I2Cx, TSL2591_ADDR, 0);
	I2C_write(I2Cx, cmd);
	I2C_write(I2Cx, data);
	I2C_stop(I2Cx);
}

uint8_t TSL2591_read(I2C_TypeDef *I2Cx, uint8_t reg) 
{
	uint8_t cmd = TSL2591_COMMAND_BIT | reg;
	uint8_t val;

	I2C_start(I2Cx, TSL2591_ADDR, 0);
	I2C_write(I2Cx, cmd);
	I2C_start(I2Cx, TSL2591_ADDR, 1);
	val = I2C_read_nack(I2Cx);
	I2C_stop(I2Cx);

	return val;
}

void TSL2591_init(I2C_TypeDef *I2Cx) 
{
	// Power on and enable ALS
	TSL2591_write(I2Cx, TSL2591_ENABLE_REG, TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN);
	// Set integration time & gain (default: medium gain)
	TSL2591_write(I2Cx, TSL2591_CONTROL_REG, 0x20); // Medium gain, 100ms
}

uint16_t TSL2591_read_ch0(I2C_TypeDef *I2Cx) 
{
	uint8_t low = TSL2591_read(I2Cx, TSL2591_CH0_LOW);
	uint8_t high = TSL2591_read(I2Cx, TSL2591_CH0_HIGH);
	return (high << 8) | low;
}

uint16_t TSL2591_read_ch1(I2C_TypeDef *I2Cx) 
{
   uint8_t low = TSL2591_read(I2Cx, TSL2591_CH1_LOW);
   uint8_t high = TSL2591_read(I2Cx, TSL2591_CH1_HIGH);
   return (high << 8) | low;
}

uint32_t TSL2591_calculate_lux(uint16_t ch0, uint16_t ch1) 
{
	float atime = 100.0f;   // ms (integration time)
  float again = 25.0f;    // gain (25x for medium gain)

  float cpl = (atime * again) / 408.0f;

	float lux1 = ((float)ch0 - 1.64f * (float)ch1) / cpl;
	float lux2 = (0.59f * (float)ch0 - 0.86f * (float)ch1) / cpl;

  float lux = lux1 > lux2 ? lux1 : lux2;

	return (lux < 0) ? 0 : lux;
	
}

uint8_t TSL2591_get_status(I2C_TypeDef *I2Cx) {
    return TSL2591_read(I2Cx, TSL2591_STATUS_REG); // STATUS register
}
