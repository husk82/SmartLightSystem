#ifndef GPIO_Driver_H
#define GPIO_Driver_H

#include "stm32f446xx.h"

//
// ===== GPIO MODE MACROS =====
//
#define GPIO_MODE_INPUT        0x00
#define GPIO_MODE_OUTPUT       0x01
#define GPIO_MODE_ALTFN        0x02
#define GPIO_MODE_ANALOG       0x03

//
// ===== GPIO OUTPUT TYPE =====
//
#define GPIO_OUTPUT_PP         0x00  // Push-Pull
#define GPIO_OUTPUT_OD         0x01  // Open-Drain

//
// ===== GPIO SPEED MACROS =====
//
#define GPIO_SPEED_LOW         0x00
#define GPIO_SPEED_MEDIUM      0x01
#define GPIO_SPEED_FAST        0x02
#define GPIO_SPEED_HIGH        0x03

//
// ===== GPIO PULL-UP / PULL-DOWN =====
//
#define GPIO_NO_PUPD           0x00
#define GPIO_PULL_UP           0x01
#define GPIO_PULL_DOWN         0x02

//
// ===== GPIO ALTERNATE FUNCTION MACROS =====
//
#define GPIO_AF0    0x00
#define GPIO_AF1    0x01
#define GPIO_AF2    0x02
#define GPIO_AF3    0x03
#define GPIO_AF4    0x04
#define GPIO_AF5    0x05
#define GPIO_AF6    0x06
#define GPIO_AF7    0x07
#define GPIO_AF8    0x08
#define GPIO_AF9    0x09
#define GPIO_AF10   0x0A
#define GPIO_AF11   0x0B
#define GPIO_AF12   0x0C
#define GPIO_AF13   0x0D
#define GPIO_AF14   0x0E
#define GPIO_AF15   0x0F

//
// ===== GPIO PIN STATE =====
//
#define GPIO_PIN_RESET         0
#define GPIO_PIN_SET           1

// ==========================
// Struct for pin configuration
// ==========================
typedef struct
{
	uint8_t pin_no;       		// 0 - 15
	uint8_t mode;            // Input, Output, Alt, Analog
	uint8_t oType;           // Output type: Push-Pull or Open-Drain
	uint8_t speed;           // Speed: Low to High
	uint8_t pull;            // Pull-up/Pull-down
	uint8_t altFunc;         // Alternate function (0 - 15)
} GPIO_config_t;

// ==========================
// API Prototypes
// ==========================

void GPIO_init(GPIO_TypeDef *GPIOx, GPIO_config_t *pinConfig);
void GPIO_write_pin(GPIO_TypeDef *GPIOx, uint8_t pin_no, uint8_t value);
uint8_t GPIO_read_pin(GPIO_TypeDef *GPIOx, uint8_t pin_no);
void GPIO_toggle_pin(GPIO_TypeDef *GPIOx, uint8_t pin_no);
void GPIO_deinit(GPIO_TypeDef *GPIOx, uint8_t pin_no);

#endif