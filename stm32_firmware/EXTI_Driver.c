#include "EXTI_Driver.h"

void EXTI_init(EXTI_config_t *exti_config)
{
	
	// Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Map EXTI line to correct port
	uint8_t index = exti_config->pin_no / 4;
	uint8_t pos = (exti_config->pin_no % 4) * 4;  
	SYSCFG->EXTICR[index] &= ~(0xF << pos);
	SYSCFG->EXTICR[index] |= (exti_config->port_code << pos);
	
	// Enable EXTI mask
	EXTI->IMR |= (1 << exti_config->pin_no);
	
	// Configure trigger
	if (exti_config->trigger_type == RISING_EDGE)
		EXTI->RTSR |= (1 << exti_config->pin_no);
	else if (exti_config->trigger_type == FALLING_EDGE)
		EXTI->FTSR |= (1 << exti_config->pin_no);
	else 
	{
		EXTI->RTSR |= (1 << exti_config->pin_no);
		EXTI->FTSR |= (1 << exti_config->pin_no);
	}
	
	// Enable NVIC
	if (exti_config->pin_no <= 4)
		NVIC_EnableIRQ(EXTI0_IRQn + exti_config->pin_no);
	else if (exti_config->pin_no <= 9)
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	else
		NVIC_EnableIRQ(EXTI15_10_IRQn);
}

bool EXTI_is_pending(uint8_t pin_no)
{
    return (EXTI->PR & (1 << pin_no)) != 0;
}

void EXTI_clear_pending(uint8_t pin_no)
{
	if (EXTI->PR & (1 << pin_no))
		EXTI->PR |= (1 << pin_no);
}

