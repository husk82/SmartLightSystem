#include "SSD1306_H.h"
#include <string.h>

static uint8_t buffer[1024];  // Framebuffer: 128 x 64 / 8

// Write a command to SSD1306
void SSD1306_write_command(I2C_TypeDef *I2Cx, uint8_t cmd) {
	I2C_start(I2Cx, SSD1306_I2C_ADDR, 0);
	I2C_write(I2Cx, 0x00);   // Command prefix
	I2C_write(I2Cx, cmd);
	I2C_stop(I2Cx);
}

// Write data bytes to SSD1306
void SSD1306_write_data(I2C_TypeDef *I2Cx, uint8_t* data, uint16_t size) {
	I2C_start(I2Cx, SSD1306_I2C_ADDR, 0);
	I2C_write(I2Cx, 0x40);  // Data prefix

	for (uint16_t i = 0; i < size; i++) {
			I2C_write(I2Cx, data[i]);
	}

	I2C_stop(I2Cx);
}

// Initialize SSD1306
void SSD1306_init(I2C_TypeDef *I2Cx) {
	SSD1306_write_command(I2Cx, 0xAE); // Display OFF
	SSD1306_write_command(I2Cx, 0x20); SSD1306_write_command(I2Cx, 0x00); // Horizontal addressing mode
	SSD1306_write_command(I2Cx, 0xB0); // Page 0
	SSD1306_write_command(I2Cx, 0xC8); // COM scan direction
	SSD1306_write_command(I2Cx, 0x00); // Low column start
	SSD1306_write_command(I2Cx, 0x10); // High column start
	SSD1306_write_command(I2Cx, 0x40); // Start line address
	SSD1306_write_command(I2Cx, 0x81); SSD1306_write_command(I2Cx, 0xFF); // Contrast
	SSD1306_write_command(I2Cx, 0xA1); // Segment remap
	SSD1306_write_command(I2Cx, 0xA6); // Normal display
	SSD1306_write_command(I2Cx, 0xA8); SSD1306_write_command(I2Cx, 0x3F); // Multiplex ratio
	SSD1306_write_command(I2Cx, 0xA4); // Resume from RAM content
	SSD1306_write_command(I2Cx, 0xD3); SSD1306_write_command(I2Cx, 0x00); // Display offset
	SSD1306_write_command(I2Cx, 0xD5); SSD1306_write_command(I2Cx, 0xF0); // Display clock divide
	SSD1306_write_command(I2Cx, 0xD9); SSD1306_write_command(I2Cx, 0x22); // Pre-charge
	SSD1306_write_command(I2Cx, 0xDA); SSD1306_write_command(I2Cx, 0x12); // COM pins
	SSD1306_write_command(I2Cx, 0xDB); SSD1306_write_command(I2Cx, 0x20); // VCOM detect
	SSD1306_write_command(I2Cx, 0x8D); SSD1306_write_command(I2Cx, 0x14); // Charge pump
	SSD1306_write_command(I2Cx, 0xAF); // Display ON

	SSD1306_clear();
	SSD1306_update(I2Cx);
}

// Clear framebuffer
void SSD1306_clear(void) {
	memset(buffer, 0x00, sizeof(buffer));
}

// Send framebuffer to display
void SSD1306_update(I2C_TypeDef *I2Cx) {
	for (uint8_t page = 0; page < 8; page++) {
		SSD1306_write_command(I2Cx, 0xB0 + page);  // Set page start
		SSD1306_write_command(I2Cx, 0x00);         // Set lower column
		SSD1306_write_command(I2Cx, 0x10);         // Set higher column
		SSD1306_write_data(I2Cx, &buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
	}
}

// Draw one pixel to framebuffer
void SSD1306_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;

	uint16_t index = x + (y / 8) * SSD1306_WIDTH;

	if (color)
		buffer[index] |= (1 << (y % 8));
	else
		buffer[index] &= ~(1 << (y % 8));
}

// Draw char
void SSD1306_draw_char(uint8_t x, uint8_t y, char c) {
    if (c < 32 || c > 126) c = '?'; // handle unsupported chars
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t col = font8x8[c - 32][i];
        for (uint8_t j = 0; j < 8; j++) {
            SSD1306_draw_pixel(x + i, y + j, (col >> j) & 0x01);
        }
    }
}

// Draw string
void SSD1306_draw_string(uint8_t x, uint8_t y, const char* str) {
    while (*str) {
        SSD1306_draw_char(x, y, *str++);
        x += 8; // 8 pixels width + 1 space
    }
}