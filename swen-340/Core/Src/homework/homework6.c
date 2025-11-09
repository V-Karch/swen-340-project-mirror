#include "printf.h"
#include "UART.h"

void USART2_IRQHandler(void) {
	char input = USART_Read(USART2);
	printf("%c\r\n", input);
}

void UART2_IRQ_Init() {
	UART2_Init();
	USART2->CR1 |= (1 << 5);
	NVIC_EnableIRQ(USART2_IRQn);
}
