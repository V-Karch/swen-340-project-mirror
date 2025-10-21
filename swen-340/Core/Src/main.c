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
#include "systick.h"
	
int main(void){
	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	init_systick();

	run_project();
//	while (1) {
//		LED_On();
//		delay_systick();
//		LED_Off();
//		delay_systick();
//	}
}
