/*
 * adc_example.c
 *
 *  Created on: Apr 13, 2022
 *      Author: bruce
 */

#include "adc.h"
#include "dac.h"
#include "adc_demo.h"

uint8_t one_second_elapsed = FALSE;
uint32_t counter = 0;

/***
 * Demo uses the ADC to read the values set by the DAC.
 * 
 * For the project, you will not use the ADC.
*/
void run_adc_dac_demo()
{
	uint16_t counter = 0;
	// printf ("Start!\n\r");
	init_systick();
	DAC_Init ();
	ADC_Init ();

	DAC_Start ();

	uint32_t adc_value = 0;

	while (1)
	{
		if (one_second_elapsed /*&& (old_adc_value != new_adc_value)*/) {
			ADC_Start ();
			adc_value = ADC_Read_Value_Blocking ();
			// printf ("ADC read: 0x%x\n\r", (unsigned int)adc_value);
			// Rediculous way to eliminate warning. Delete this line when printf is working.
			adc_value = adc_value;

			// Let power count up in a cycle. This is to make reading it more interesting.
			DAC_Set_Value ((counter) % 0x1000);
			counter++;
		}

	}
}

