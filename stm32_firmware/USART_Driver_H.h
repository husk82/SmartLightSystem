#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "stm32f446xx.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint32_t BaudRate;
  uint8_t WordLength;   // 8 or 9 bits
  uint8_t StopBits;     // 1 or 2
  uint8_t Parity;       // 0: none, 1: even, 2: odd
  uint8_t Mode;         // TX, RX, or TXRX
  uint8_t HWFlowCtrl;   // None, RTS, CTS, RTS/CTS
} USART_Config_t;

// Mode options
#define USART_MODE_TX       0x01
#define USART_MODE_RX       0x02
#define USART_MODE_TXRX     0x03

// Parity
#define USART_PARITY_DISABLE 0
#define USART_PARITY_EVEN    1
#define USART_PARITY_ODD     2

// Stop bits
#define USART_STOPBITS_1     0
#define USART_STOPBITS_2     1

// Hardware Flow
#define USART_HW_FLOW_CTRL_NONE 0
#define USART_HW_FLOW_CTRL_RTS  1
#define USART_HW_FLOW_CTRL_CTS  2
#define USART_HW_FLOW_CTRL_RTSCTS 3

void USART_init(USART_TypeDef *USARTx, USART_Config_t *usart_cfg);
void USART_SendChar(USART_TypeDef *USARTx, char ch);
void USART_SendString(USART_TypeDef *USARTx, const char *str);
char USART_ReceiveChar(USART_TypeDef *USARTx);
bool USART_ReceiveString(USART_TypeDef *USARTx, char *buffer, uint16_t len, char terminator);

#endif