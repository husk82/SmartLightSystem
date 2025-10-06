#include "GPIO_Driver_H.h"
#include <stdint.h>

//
// ==== Static Helper Functions ====
//

// 1. Configure mode: Input, Output, AltFunc, Analog
static void gpio_config_mode(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode)
{
	GPIOx->MODER &= ~(0x3 << (2 * pin_no));        // Clear
	GPIOx->MODER |= (mode << (2 * pin_no));        // Set
}

// 2. Configure speed: Low, Medium, High, Very High
static void gpio_config_speed(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t speed)
{
	GPIOx->OSPEEDR &= ~(0x3 << (2 * pin_no));
	GPIOx->OSPEEDR |= (speed << (2 * pin_no));
}

// 3. Configure output type: Push-pull (0) or Open-drain (1)
static void gpio_config_otype(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t type)
{
	GPIOx->OTYPER &= ~(1 << pin_no);
	GPIOx->OTYPER |= (type << pin_no);
}

// 4. Configure pull-up/pull-down
static void gpio_config_pupd(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t pupd)
{
	GPIOx->PUPDR &= ~(0x3 << (2 * pin_no));
	GPIOx->PUPDR |= (pupd << (2 * pin_no));
}

// 5. Alternate function config (AFR[0] = pins 0-7, AFR[1] = pins 8-15)
static void gpio_config_alt_func(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t af)
{
	uint8_t reg_idx = pin_no / 8;
	uint8_t bit_pos = (pin_no % 8) * 4;

	GPIOx->AFR[reg_idx] &= ~(0xF << bit_pos);  // Clear
	GPIOx->AFR[reg_idx] |= (af << bit_pos);    // Set AF
}

// .6 GPIOx clock enabbler
static void GPIO_clock_enable(GPIO_TypeDef *GPIOx)
{
	if (GPIOx == GPIOA)
		RCC->AHB1ENR |= (1 << 0);
	else if (GPIOx == GPIOB)
		RCC->AHB1ENR |= (1 << 1);
	else if (GPIOx == GPIOC)
		RCC->AHB1ENR |= (1 << 2);
	else if (GPIOx == GPIOD)
		RCC->AHB1ENR |= (1 << 3);
	else if (GPIOx == GPIOE)
		RCC->AHB1ENR |= (1 << 4);
	else if (GPIOx == GPIOF)
		RCC->AHB1ENR |= (1 << 5);
	else if (GPIOx == GPIOG)
		RCC->AHB1ENR |= (1 << 6);
	else if (GPIOx == GPIOH)
		RCC->AHB1ENR |= (1 << 7);
}

//
// ==== APIs ====
//


uint8_t GPIO_read_pin(GPIO_TypeDef *GPIOx, uint8_t pin_no)
{
	uint8_t value;
	value = ((GPIOx->IDR >> pin_no) & 0x00000001);
	
	return value;
}

void GPIO_write_pin(GPIO_TypeDef *GPIOx, uint8_t pin_no, uint8_t value)
{
	if (value)
		GPIOx->ODR |= (1 << pin_no);
	else
		GPIOx->ODR &= ~(1 << pin_no);
}

void GPIO_toggle_pin(GPIO_TypeDef *GPIOx, uint8_t pin_no)
{
	GPIOx->ODR ^= (1 << pin_no);
}

// Main GPIO init function
void GPIO_init(GPIO_TypeDef *GPIOx, GPIO_config_t *pinConfig)
{
	// Enable GPIOx clock
	GPIO_clock_enable(GPIOx);
	
	// Extract values from the structure
	uint8_t pin_no   = pinConfig->pin_no;
	uint8_t mode     = pinConfig->mode;
	uint8_t otype    = pinConfig->oType;
	uint8_t speed    = pinConfig->speed;
	uint8_t pupd     = pinConfig->pull;
	uint8_t alt_func = pinConfig->altFunc;
	
	gpio_config_mode(GPIOx, pin_no, mode);

	if (mode == GPIO_MODE_OUTPUT || mode == GPIO_MODE_ALTFN)  // Output or Alternate Function
	{
		gpio_config_otype(GPIOx, pin_no, otype);
		gpio_config_speed(GPIOx, pin_no, speed);
	}

	gpio_config_pupd(GPIOx, pin_no, pupd);

	if (mode == GPIO_MODE_ALTFN)  // Alternate Function mode
	{
		gpio_config_alt_func(GPIOx, pin_no, alt_func);
	}
}

// Deinitialization
void GPIO_deinit(GPIO_TypeDef *GPIOx, uint8_t pin_no)
{
    GPIOx->MODER &= ~(0x3 << (2 * pin_no));
    GPIOx->OTYPER &= ~(1 << pin_no);
    GPIOx->OSPEEDR &= ~(0x3 << (2 * pin_no));
    GPIOx->PUPDR &= ~(0x3 << (2 * pin_no));

    uint8_t reg_idx = pin_no / 8;
    uint8_t bit_pos = (pin_no % 8) * 4;
    GPIOx->AFR[reg_idx] &= ~(0xF << bit_pos);
}

