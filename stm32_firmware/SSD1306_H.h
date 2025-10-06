#ifndef __SSD1306_H
#define __SSD1306_H

#include "stm32f4xx.h"
#include "I2C_Driver_H.h"
#include "font_H.h"
#include <stdint.h>

#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT   64
#define SSD1306_I2C_ADDR 0x3C

void SSD1306_init(I2C_TypeDef *I2Cx);
void SSD1306_write_command(I2C_TypeDef *I2Cx, uint8_t cmd);
void SSD1306_write_data(I2C_TypeDef *I2Cx, uint8_t* data, uint16_t size);

void SSD1306_clear(void);
void SSD1306_update(I2C_TypeDef *I2Cx);
void SSD1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void SSD1306_draw_char(uint8_t x, uint8_t y, char c);
void SSD1306_draw_string(uint8_t x, uint8_t y, const char* str);

#endif