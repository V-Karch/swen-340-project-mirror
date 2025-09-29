/*
 * activity6.h
 *
 *  Created on: Sep 29, 2025
 *      Author: luna
 */

#ifndef INC_ACTIVITIES_ACTIVITY6_H_
#define INC_ACTIVITIES_ACTIVITY6_H_

typedef struct {
	// Starting from 0xE000E010
	uint32_t SYST_CSR;
	uint32_t SYST_RVR;
	uint32_t SYST_CVR;
	uint32_t SYST_CALIB;
} SysTick;

void run_activity_6();

#endif /* INC_ACTIVITIES_ACTIVITY6_H_ */
