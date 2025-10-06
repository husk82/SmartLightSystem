#ifndef TIM_Driver_H
#define TIM_Driver_H
#include "stm32f4xx.h"  

typedef enum {
    TIM_CHANNEL_1 = 0,
    TIM_CHANNEL_2,
    TIM_CHANNEL_3,
    TIM_CHANNEL_4
} TIM_Channel_t;

typedef struct {
    TIM_TypeDef *Instance;
    uint32_t Prescaler;
    uint32_t Period;
    TIM_Channel_t Channel;
} TIM_PWM_Config_t;

void TIM_PWM_init(TIM_PWM_Config_t *config);
void TIM_PWM_set_duty(TIM_PWM_Config_t *config, uint32_t dutyCycle);  // 0 to Period

#endif  