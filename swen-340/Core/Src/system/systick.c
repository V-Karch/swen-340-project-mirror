#include "systick.h"
#include "tone.h"

SYSTICK* systick = (SYSTICK*)0xE000E010;
uint32_t TOTAL_COUNT = 0;

uint32_t get_total_count() {
	return TOTAL_COUNT;
}

// This function is to Initialize SysTick registers
void init_systick()
{
	systick->SYST_CSR = 0;
	systick->SYST_RVR = 79;
	systick->SYST_CSR |= (1 << 1);
	systick->SYST_CSR |= (1 << 2);
	systick->SYST_CSR |= 1;
	// Use the SysTick global structure pointer to do the following in this
	// exact order with separate lines for each step:
	//
	// Disable SysTick by clearing the CTRL (CSR) register.
	// Set the LOAD (RVR) to 8 million to give us a 100 milliseconds timer.
	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	// Set the enable bit in the CTRL (CSR) to start the timer.
}

// This fuction is to create delay using SysTick timer counter
void delay_systick()
{
	for (int i = 0; i < 1000000; i++) {
		while (!(systick->SYST_CSR & (1 << 16))) {
			// wait.....
		}
	}
	// Using the SysTick global structure pointer do the following:
	// Create a for loop that loops 10 times
	// Inside that for loop check the COUNTFLAG bit in the CTRL (CSR)
	// register in a loop. When that bit is set exit this inner loop
	// to do another pass in the outer loop of 10.
}

void SysTick_Handler(void) {
	volatile uint8_t some = (systick->SYST_CSR & (1 << 16)); // Clear on read

	some = some; // Variable is now used... :D
	TOTAL_COUNT++;
}
