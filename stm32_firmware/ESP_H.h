#ifndef ESP_H
#define ESP_H

#include <stdint.h>
#include <stdbool.h>
#include "USART_Driver_H.h"
#include "SECRETS_H.h"   // Include secrets here

// Initialize ESP8266 (AT mode)
bool ESP_Init(void);

// Connect to WiFi using credentials from secrets.h
bool ESP_ConnectWiFi(void);

// Status of Wifi
bool ESP_WiFiStatus(void);

// Open TCP connection
bool ESP_OpenTCP(const char *host, uint16_t port);

// Send data (string) to server
bool ESP_SendData(const char *data, uint16_t len);

// Close TCP connection
bool ESP_CloseTCP(void);

#endif