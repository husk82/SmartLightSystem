#include "stm32f446xx.h"
#include "GPIO_Driver_H.h"
#include "EXTI_Driver.h"
#include "I2C_Driver_H.h"
#include "TSL2591_H.h"
#include "TIM_Driver_H.h"
#include "USART_Driver_H.h"
#include "ESP_H.h"
#include "SSD1306_H.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// ===== Pin Definitions =====
#define USER_BUTTON_PIN 13
#define USER_BUTTON_PORT_CODE 2 // Port C
#define LED1_PIN 6
#define LED2_PIN 7
#define I2C_SCL_PIN_LIGHT 8
#define I2C_SDA_PIN_LIGHT 9
#define I2C_SCL_PIN_OLED 10
#define I2C_SDA_PIN_OLED 3
#define ESP_TX_PIN 9
#define ESP_RX_PIN 10

#define LUX_MIN 0
#define LUX_MAX 1500

// ===== Global Variables =====
volatile bool button_pressed = false;
uint32_t latest_lux = 0;

// ===== Helper Functions =====
uint32_t scale_lux_to_pwm(uint32_t lux) 
{
	if (lux < LUX_MIN) lux = LUX_MIN;
	if (lux > LUX_MAX) lux = LUX_MAX;
	return 999 - (lux - LUX_MIN) * 999 / (LUX_MAX - LUX_MIN);
}

void EXTI15_10_IRQHandler(void)
{
  if (EXTI_is_pending(USER_BUTTON_PIN))   // Check pending using driver
  {
		EXTI_clear_pending(USER_BUTTON_PIN); // Clear pending using driver
    button_pressed = true;
  }
}

// ===== Button Initialization =====
void User_button_Init(void) 
{
	GPIO_config_t btn_cfg = {
		.pin_no = USER_BUTTON_PIN,
		.mode = GPIO_MODE_INPUT,
		.pull = GPIO_PULL_UP
	};
	GPIO_init(GPIOC, &btn_cfg);

	EXTI_config_t exti_cfg = {  
		.pin_no       = USER_BUTTON_PIN,  
		.port_code    = USER_BUTTON_PORT_CODE,  
		.trigger_type = FALLING_EDGE  
	};  
	EXTI_init(&exti_cfg);  
}

// ===== Using button flag as callback to TIM ====
void send_lux_callback(void)
{
	button_pressed = true;
}

// ===== TIM Periodic Initialization =====
TIM_Periodic_Config_t tim2_cfg = {TIM2, 16000-1, 600000-1, send_lux_callback};

// ===== LED PWM Initialization =====
void LED_PWM_Init(TIM_PWM_Config_t *pwm1, TIM_PWM_Config_t *pwm2) 
{
	GPIO_config_t led1 = {LED1_PIN, GPIO_MODE_ALTFN, GPIO_OUTPUT_PP, GPIO_SPEED_LOW, GPIO_NO_PUPD, GPIO_AF2};
	GPIO_init(GPIOA, &led1);
	pwm1->Instance = TIM3; pwm1->Prescaler = 89; pwm1->Period = 999; pwm1->Channel = TIM_CHANNEL_1;
	TIM_PWM_init(pwm1);

	GPIO_config_t led2 = {LED2_PIN, GPIO_MODE_ALTFN, GPIO_OUTPUT_PP, GPIO_SPEED_LOW, GPIO_NO_PUPD, GPIO_AF2};  
	GPIO_init(GPIOA, &led2);  
	pwm2->Instance = TIM3; pwm2->Prescaler = 89; pwm2->Period = 999; pwm2->Channel = TIM_CHANNEL_2;  
	TIM_PWM_init(pwm2);  

}

// ===== I2C Initialization =====
void I2C_InitAll(void) 
{
	GPIO_config_t scl_light = {I2C_SCL_PIN_LIGHT, GPIO_MODE_ALTFN, GPIO_OUTPUT_OD, GPIO_SPEED_HIGH, GPIO_PULL_UP, GPIO_AF4};
	GPIO_config_t sda_light = {I2C_SDA_PIN_LIGHT, GPIO_MODE_ALTFN, GPIO_OUTPUT_OD, GPIO_SPEED_HIGH, GPIO_PULL_UP, GPIO_AF4};
	GPIO_config_t scl_oled = {I2C_SCL_PIN_OLED, GPIO_MODE_ALTFN, GPIO_OUTPUT_OD, GPIO_SPEED_HIGH, GPIO_PULL_UP, GPIO_AF4};
	GPIO_config_t sda_oled = {I2C_SDA_PIN_OLED, GPIO_MODE_ALTFN, GPIO_OUTPUT_OD, GPIO_SPEED_HIGH, GPIO_PULL_UP, GPIO_AF4};

	GPIO_init(GPIOB, &scl_light);  
	GPIO_init(GPIOB, &sda_light);  
	GPIO_init(GPIOB, &scl_oled);  
	GPIO_init(GPIOB, &sda_oled);  

	I2C_init(I2C1, 16000000, 100000);  
	I2C_init(I2C2, 16000000, 100000);  

}

// ===== USART Initialization =====
void USART_InitAll(void) 
{
	GPIO_config_t tx_pin = {ESP_TX_PIN, GPIO_MODE_ALTFN, GPIO_OUTPUT_PP, GPIO_SPEED_HIGH, GPIO_NO_PUPD, GPIO_AF7};
	GPIO_config_t rx_pin = {ESP_RX_PIN, GPIO_MODE_ALTFN, GPIO_OUTPUT_PP, GPIO_SPEED_HIGH, GPIO_NO_PUPD, GPIO_AF7};
	GPIO_init(GPIOA, &tx_pin);
	GPIO_init(GPIOA, &rx_pin);

	USART_Config_t usart_cfg = {115200, 8, USART_STOPBITS_1, USART_PARITY_DISABLE, USART_MODE_TXRX, USART_HW_FLOW_CTRL_NONE};  
	USART_init(USART1, &usart_cfg);  

}

// ===== Main =====
int main(void) 
{
	TIM_Periodic_set_global(&tim2_cfg);
	TIM_Periodic_init(&tim2_cfg);
	TIM_PWM_Config_t pwm1, pwm2;
	LED_PWM_Init(&pwm1, &pwm2);  
	I2C_InitAll();  
	TSL2591_init(I2C1);  
	SSD1306_init(I2C2);  
	USART_InitAll();  
	User_button_Init();  

	bool esp_ok = ESP_Init();  
	if (esp_ok) ESP_ConnectWiFi();  

	char buffer1[32], esp_status[16], wifi_status[16],post_status[16];  

	while (1) 
	{  
		// --- Read Sensor ---  
		uint16_t ch0 = TSL2591_read_ch0(I2C1);  
		uint16_t ch1 = TSL2591_read_ch1(I2C1);  

		if (ch0 >= TSL2591_ADC_MAX || ch1 >= TSL2591_ADC_MAX) 
		{  
			sprintf(buffer1, "DATA OVERFLOW");  
			latest_lux = LUX_MAX;  
		} 
		else 
		{  
			latest_lux = TSL2591_calculate_lux(ch0, ch1);  
			uint32_t duty = scale_lux_to_pwm(latest_lux);  
			TIM_PWM_set_duty(&pwm1, duty);  
			TIM_PWM_set_duty(&pwm2, 999 - duty);  
			sprintf(buffer1, "Lux: %lu", latest_lux);  
		}  

		// --- Update Status ---  
		sprintf(esp_status, esp_ok ? "ESP: OK" : "ESP: FAIL");  
		sprintf(wifi_status, ESP_WiFiStatus() ? "WiFi: OK" : "WiFi: FAIL");  
		strcpy(post_status, "POST IDLE");		

		// --- Update OLED ---   
		SSD1306_clear();
		SSD1306_draw_string(0, 0, buffer1);  
		SSD1306_draw_string(0, 8, esp_status);  
		SSD1306_draw_string(0, 16, wifi_status);   
		SSD1306_draw_string(0, 24, post_status); 
		
		// --- Send Data on Button Press ---  
		if (button_pressed) {  
			char json_payload[32];  
			int json_len = snprintf(json_payload, sizeof(json_payload), "{\"lux\": %lu}", latest_lux);  

			char post_data[128];  
			int post_len = snprintf(post_data, sizeof(post_data),  
				"POST /api/lux HTTP/1.1\r\n"  
				"Host: 192.168.2.22\r\n"  
				"Content-Type: application/json\r\n"  
				"Content-Length: %d\r\n\r\n%s",  
				json_len, json_payload);  

			if (ESP_OpenTCP("192.168.2.22", 5050)) {  
				if (ESP_SendData(post_data, post_len)) 
				{
					sprintf(post_status, "POST Sent");
				} else {
					sprintf(post_status, "POST Failed");
				}			
				ESP_CloseTCP();  
			}  
			// Show message on OLED 
			SSD1306_draw_string(0, 24, post_status);
			SSD1306_draw_string(0, 32, "POST Initiated!!");		
			button_pressed = false;  
		} 
		SSD1306_update(I2C2); 

		// --- Small Delay ---  
		for (volatile int i = 0; i < 500000; ++i);
		
	}  
	return 0;  

}