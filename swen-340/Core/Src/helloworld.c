#include "UART.h"
#include <stdio.h>

void hello() {
	uint8_t buffer[20];
	uint8_t length = sprintf((char*)buffer, "Hello World\r\n");
	USART_Write(USART2, buffer, length);
	return;
}
