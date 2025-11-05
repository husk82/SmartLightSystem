#include "ESP_H.h"
#include <string.h>
#include <stdio.h>

#define ESP_USART USART1
#define ESP_BUFFER_LEN 256

static char esp_rx_buffer[ESP_BUFFER_LEN];

// --- UART helpers using your USART driver ---
static void ESP_UART_SendString(const char *str) {
    USART_SendString(ESP_USART, str);
}

static char* ESP_UART_ReadResponse(uint32_t timeout_ms) {
    uint32_t tick = 0;
    uint16_t idx = 0;

    while (tick < timeout_ms && idx < ESP_BUFFER_LEN - 1) {
        if (ESP_USART->SR & USART_SR_RXNE) {
            esp_rx_buffer[idx++] = USART_ReceiveChar(ESP_USART);
        } else {
            tick++;
        }
    }

    esp_rx_buffer[idx] = '\0';
    return esp_rx_buffer;
}

static bool ESP_SendAT(const char *cmd, const char *expected, uint32_t timeout) {
    ESP_UART_SendString(cmd);
    ESP_UART_SendString("\r\n");
    char *resp = ESP_UART_ReadResponse(timeout);
    return (strstr(resp, expected) != NULL);
}

// --- High-level functions ---
bool ESP_Init(void) {
    // Check communication
    if (!ESP_SendAT("AT", "OK", 50000)) return false;
    // Set station mode
    if (!ESP_SendAT("AT+CWMODE=1", "OK", 50000)) return false;
    return true;
}

bool ESP_ConnectWiFi(void) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", WIFI_SSID, WIFI_PASSWORD);
    return ESP_SendAT(cmd, "WIFI GOT IP", 200000);
}

bool ESP_WiFiStatus(void) {
    // Send the AT command to query current WiFi connection
    if (!ESP_SendAT("AT+CWJAP?", "No AP", 100000)) {
        // "No AP" not found ? WiFi connected
        return true;
    } 
    // "No AP" found ? WiFi not connected
    return false;
}

bool ESP_OpenTCP(const char *host, uint16_t port) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);
    return ESP_SendAT(cmd, "OK", 100000);
}

bool ESP_SendData(const char *data, uint16_t len) {
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%d", len);
    if (!ESP_SendAT(cmd, ">", 50000)) return false;

    ESP_UART_SendString(data);

    char *resp = ESP_UART_ReadResponse(100000);
    // Check if ESP confirms send
    return (strstr(resp, "SEND OK") != NULL);
}

bool ESP_CloseTCP(void) {
    return ESP_SendAT("AT+CIPCLOSE", "OK", 50000);
}