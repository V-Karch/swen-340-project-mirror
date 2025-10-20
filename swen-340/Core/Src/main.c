// main.c with demo
// Updated and refactored Larry Kiser copyright 2021


// include project specific header files
#include "error_handler.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "printf.h"
#include "project.h"
	
int main(void){
	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();

	// Project Part 1 Stuff
	display_menu();
	char buffer_input[7];
	read_input_string(buffer_input, 7);
	printf("%s\r\n", buffer_input);
}
