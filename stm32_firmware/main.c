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

	// Global variables for PINS
	uint8_t LED1_PIN = 6;
	uint8_t LED2_PIN = 7;
	uint8_t I2C_SCL_PIN_LIGHT = 8;
	uint8_t I2C_SDA_PIN_LIGHT = 9;
	uint8_t I2C_SCL_PIN_OLED = 10;
	uint8_t I2C_SDA_PIN_OLED = 3;
	uint32_t LUX_MIN = 0;
	uint32_t LUX_MAX = 1500;
	uint8_t ESP_TX_PIN = 9;  
  uint8_t ESP_RX_PIN = 10; 
	
	// === Helper Functions === //
	uint32_t scale_lux_to_pwm(uint32_t lux) {
    if (lux < LUX_MIN) lux = LUX_MIN;
    if (lux > LUX_MAX) lux = LUX_MAX;
    
    return 999 - (lux - LUX_MIN) * 999 / (LUX_MAX - LUX_MIN);
	}
	
	// ====== MAIN ====== //
	int main (void)
	{
	// --- Led Pin Config---
	// Configure PA6 as TIM3 led1
	GPIO_config_t led1;
	led1.pin_no    = LED1_PIN;
	led1.mode      = GPIO_MODE_ALTFN;
	led1.oType     = GPIO_OUTPUT_PP;
	led1.speed     = GPIO_SPEED_LOW;
	led1.pull      = GPIO_NO_PUPD;
	led1.altFunc   = GPIO_AF2;  

	GPIO_init(GPIOA, &led1);

	// Configure PA7 as TIM3 led2
	GPIO_config_t led2;
	led2.pin_no    = LED2_PIN;
	led2.mode      = GPIO_MODE_ALTFN;
	led2.oType     = GPIO_OUTPUT_PP;
	led2.speed     = GPIO_SPEED_LOW;
	led2.pull      = GPIO_NO_PUPD;
	led2.altFunc   = GPIO_AF2;

	GPIO_init(GPIOA, &led2);

	// --- TIM Congif ---
	// Config PA6 as TIM3 Channel 1
	TIM_PWM_Config_t pwm1;
	pwm1.Instance = TIM3;
	pwm1.Prescaler = 89;        // 90 MHz / 90 = 1 MHz
	pwm1.Period = 999;          // 1 MHz / 1000 = 1 kHz PWM
	pwm1.Channel = TIM_CHANNEL_1;

	TIM_PWM_init(&pwm1);

	// Config PA7 as TIM3 Channel 2
	TIM_PWM_Config_t pwm2;
	pwm2.Instance = TIM3;
	pwm2.Prescaler = 89;        // 90 MHz / 90 = 1 MHz
	pwm2.Period = 999;          // 1 MHz / 1000 = 1 kHz PWM
	pwm2.Channel = TIM_CHANNEL_2;

	TIM_PWM_init(&pwm2);

	// --- I2C Pin Config --
	// Configure PB8 as I2C SCL
	GPIO_config_t scl_light;
	scl_light.pin_no = I2C_SCL_PIN_LIGHT;
	scl_light.mode = GPIO_MODE_ALTFN;
	scl_light.oType = GPIO_OUTPUT_OD;
	scl_light.speed = GPIO_SPEED_HIGH;
	scl_light.pull = GPIO_PULL_UP;
	scl_light.altFunc = GPIO_AF4;

	GPIO_init(GPIOB, &scl_light);

	// Configure PB9 as I2C SCL
	GPIO_config_t sda_light;
	sda_light.pin_no = I2C_SDA_PIN_LIGHT;
	sda_light.mode = GPIO_MODE_ALTFN;
	sda_light.oType = GPIO_OUTPUT_OD;
	sda_light.speed = GPIO_SPEED_HIGH;
	sda_light.pull = GPIO_PULL_UP;
	sda_light.altFunc = GPIO_AF4;

	GPIO_init(GPIOB, &sda_light);

	// Configure PB10 as I2C SCL
	GPIO_config_t scl_oled;
	scl_oled.pin_no = I2C_SCL_PIN_OLED;
	scl_oled.mode = GPIO_MODE_ALTFN;
	scl_oled.oType = GPIO_OUTPUT_OD;
	scl_oled.speed = GPIO_SPEED_HIGH;
	scl_oled.pull = GPIO_PULL_UP;
	scl_oled.altFunc = GPIO_AF4;

	GPIO_init(GPIOB, &scl_oled);

	// Configure PB3 as I2C SDA
	GPIO_config_t sda_oled;
	sda_oled.pin_no = I2C_SDA_PIN_OLED;
	sda_oled.mode = GPIO_MODE_ALTFN;
	sda_oled.oType = GPIO_OUTPUT_OD;
	sda_oled.speed = GPIO_SPEED_HIGH;
	sda_oled.pull = GPIO_PULL_UP;
	sda_oled.altFunc = GPIO_AF4;

	GPIO_init(GPIOB, &sda_oled);
	
	// Configure PA9 as USART1 TX
	GPIO_config_t tx_pin;
	
	tx_pin.pin_no = ESP_TX_PIN;
	tx_pin.mode = GPIO_MODE_ALTFN;
	tx_pin.oType = GPIO_OUTPUT_PP;
	tx_pin.speed = GPIO_SPEED_HIGH;
	tx_pin.pull = GPIO_NO_PUPD;
	tx_pin.altFunc = GPIO_AF7;   // USART1 TX
	
	GPIO_init(GPIOA, &tx_pin);
	
	// Configure PA10 as USART1 TX
	GPIO_config_t rx_pin;
	
	rx_pin.pin_no = ESP_RX_PIN;
	rx_pin.mode = GPIO_MODE_ALTFN;
	rx_pin.oType = GPIO_OUTPUT_PP;
	rx_pin.speed = GPIO_SPEED_HIGH;
	rx_pin.pull = GPIO_NO_PUPD;
	rx_pin.altFunc = GPIO_AF7;   // USART1 RX
	
	GPIO_init(GPIOA, &rx_pin);

	// Initiating I2C communication
	I2C_init(I2C1, 16000000, 100000);
	I2C_init(I2C2, 16000000, 100000);

	// Init sensor
	TSL2591_init(I2C1);

	// Init OLED
	SSD1306_init(I2C2);
	
	// Init USART
	USART_Config_t usart_cfg = {115200, 8, USART_STOPBITS_1, USART_PARITY_DISABLE, USART_MODE_TXRX, USART_HW_FLOW_CTRL_NONE};
  USART_init(USART1, &usart_cfg); 
	
	// --- ESP Init Status ---
  bool esp_ok = ESP_Init(); // your ESP_Init should use USART1
  char esp_status[16];
  if (esp_ok) sprintf(esp_status, "ESP: OK");
  else sprintf(esp_status, "ESP: FAIL");
	
	// --- ESP Connect to wifi ----
	if (esp_ok) ESP_ConnectWiFi();
	char wifi_status[16];
	if (ESP_WiFiStatus()) sprintf(wifi_status, "WiFi: OK");
	else sprintf(wifi_status, "WiFi: FAIL");

	while (1)
	{	
		char buffer1[32];
		uint16_t ch0 = TSL2591_read_ch0(I2C1);
		uint16_t ch1 = TSL2591_read_ch1(I2C1);

		if (ch0 >= TSL2591_ADC_MAX || ch1 >= TSL2591_ADC_MAX)  // ALS Saturation bit
		{
			sprintf(buffer1, "DATA OVERFLOW");
		}
		else 
		{
			uint32_t lux = TSL2591_calculate_lux(ch0, ch1);

			uint32_t duty = scale_lux_to_pwm(lux);
				
			TIM_PWM_set_duty(&pwm1, duty);  
			TIM_PWM_set_duty(&pwm2, 999 - duty); 

			sprintf(buffer1, "Lux: %u", lux);
		}

		for (volatile int i = 0; i < 100000; ++i); // Small delay

		// Display Hello World
		SSD1306_clear();
		SSD1306_draw_string(0, 0, buffer1);
		SSD1306_draw_string(0, 8, esp_status);
		SSD1306_draw_string(0, 16, wifi_status);
		SSD1306_update(I2C2);
	}

	return 0;
	}

