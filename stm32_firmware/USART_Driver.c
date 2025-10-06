#include "USART_Driver_H.h"

void USART_init(USART_TypeDef *USARTx, USART_Config_t *usart_cfg) 
{
	// Enable clock for USART1 or USART2 etc
	if (USARTx == USART1) RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	else if (USARTx == USART2) RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	else if (USARTx == USART6) RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

	// Disable USART before configuring
	USARTx->CR1 &= ~USART_CR1_UE;

	// Configure baud rate
	uint32_t periph_clk = 16000000; // APB clock (adjust if needed)
	USARTx->BRR = (periph_clk + (usart_cfg->BaudRate/2)) / usart_cfg->BaudRate;

	// Configure word length
	if (usart_cfg->WordLength == 9) USARTx->CR1 |= USART_CR1_M;
	else USARTx->CR1 &= ~USART_CR1_M;

	// Configure parity
	if (usart_cfg->Parity == USART_PARITY_EVEN) {
		USARTx->CR1 |= USART_CR1_PCE;
		USARTx->CR1 &= ~USART_CR1_PS;
	} else if (usart_cfg->Parity == USART_PARITY_ODD) {
		USARTx->CR1 |= USART_CR1_PCE | USART_CR1_PS;
	} else {
		USARTx->CR1 &= ~USART_CR1_PCE;
	}

	// Configure stop bits
	if (usart_cfg->StopBits == USART_STOPBITS_1) USARTx->CR2 &= ~USART_CR2_STOP;
	else if (usart_cfg->StopBits == USART_STOPBITS_2) USARTx->CR2 |= USART_CR2_STOP_1;

	// Configure mode
	USARTx->CR1 |= ((usart_cfg->Mode & 0x01) ? USART_CR1_TE : 0) |
							 ((usart_cfg->Mode & 0x02) ? USART_CR1_RE : 0);

	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

void USART_SendChar(USART_TypeDef *USARTx, char ch) 
{
	while (!(USARTx->SR & USART_SR_TXE));
	USARTx->DR = (ch & 0xFF);
}

void USART_SendString(USART_TypeDef *USARTx, const char *str) 
{
	while (*str) {
		USART_SendChar(USARTx, *str++);
	}
}

char USART_ReceiveChar(USART_TypeDef *USARTx)
{
	while (!(USARTx->SR & USART_SR_RXNE));
	return (char)(USARTx->DR & 0xFF);
}

bool USART_ReceiveString(USART_TypeDef *USARTx, char *buffer, uint16_t len, char terminator) 
{
	uint16_t i = 0;
	char ch;
	while (i < len - 1) 
	{
		ch = USART_ReceiveChar(USARTx);
		if (ch == terminator) break;
		buffer[i++] = ch;
	}
	buffer[i] = '\0';
	return i > 0;
}