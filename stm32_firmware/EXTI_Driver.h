#ifndef EXTI_DRIVER_H
#define EXTI_DRIVER_H

#include "stm32f446xx.h"
#include <stdbool.h>

#define RISING_EDGE 0
#define FALLING_EDGE 1

typedef struct 
{
	uint8_t pin_no;
	uint8_t port_code;
	uint8_t trigger_type;
} EXTI_config_t;

typedef void (*EXTI_Callback_t)(void);

void EXTI_init(EXTI_config_t *exti_config);
bool EXTI_is_pending(uint8_t pin_no);
void EXTI_clear_pending(uint8_t pin_no);

#endif