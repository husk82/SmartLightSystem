#include "TIM_Driver_H.h"

static void enable_tim_clock(TIM_TypeDef *instance)
{
	// Enable Timer clock based on the timer instance
	if (instance == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	} else if (instance == TIM2) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	} else if (instance == TIM3) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	} else if (instance == TIM4) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	} else if (instance == TIM5) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	} else if (instance == TIM6) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	} else if (instance == TIM7) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	} else if (instance == TIM8) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	} else if (instance == TIM9) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	} else if (instance == TIM10) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	} else if (instance == TIM11) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
	} else if (instance == TIM12) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
	} else if (instance == TIM13) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
	} else if (instance == TIM14) {
			RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	}
}

void TIM_PWM_init(TIM_PWM_Config_t *config)
{
	// Enable Timer clock
	enable_tim_clock(config->Instance);
	
	// Configure prescaler and auto-reload
  config->Instance->PSC = config->Prescaler;
  config->Instance->ARR = config->Period;
	
	// Set PWM Mode 1
	switch (config->Channel) {
		case TIM_CHANNEL_1:
			config->Instance->CCMR1 &= ~TIM_CCMR1_OC1M;
			config->Instance->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);  // PWM mode 1
			config->Instance->CCMR1 |= TIM_CCMR1_OC1PE;
			config->Instance->CCER |= TIM_CCER_CC1E;
			break;
		case TIM_CHANNEL_2:
			config->Instance->CCMR1 &= ~TIM_CCMR1_OC2M;
			config->Instance->CCMR1 |= (6 << TIM_CCMR1_OC2M_Pos);
			config->Instance->CCMR1 |= TIM_CCMR1_OC2PE;
			config->Instance->CCER |= TIM_CCER_CC2E;
			break;
		case TIM_CHANNEL_3:
			config->Instance->CCMR2 &= ~TIM_CCMR2_OC3M;
			config->Instance->CCMR2 |= (6 << TIM_CCMR2_OC3M_Pos);
			config->Instance->CCMR2 |= TIM_CCMR2_OC3PE;
			config->Instance->CCER |= TIM_CCER_CC3E;
			break;
		case TIM_CHANNEL_4:
			config->Instance->CCMR2 &= ~TIM_CCMR2_OC4M;
			config->Instance->CCMR2 |= (6 << TIM_CCMR2_OC4M_Pos);
			config->Instance->CCMR2 |= TIM_CCMR2_OC4PE;
			config->Instance->CCER |= TIM_CCER_CC4E;
			break;
	}
	
	// Enable auto-reload preload
  config->Instance->CR1 |= TIM_CR1_ARPE;

  // Enable counter
  config->Instance->CR1 |= TIM_CR1_CEN;
}

void TIM_PWM_set_duty(TIM_PWM_Config_t *config, uint32_t dutyCycle) {
	if (dutyCycle > config->Period)
			dutyCycle = config->Period;

	switch (config->Channel) {
		case TIM_CHANNEL_1:
			config->Instance->CCR1 = dutyCycle;
			break;
		case TIM_CHANNEL_2:
			config->Instance->CCR2 = dutyCycle;
			break;
		case TIM_CHANNEL_3:
			config->Instance->CCR3 = dutyCycle;
			break;
		case TIM_CHANNEL_4:
			config->Instance->CCR4 = dutyCycle;
			break;
	}
}


