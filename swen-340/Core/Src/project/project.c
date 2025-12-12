/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: V-Karch
 */

#include "project.h"
#include "GPIO.h"
#include "systick.h"
#include "main.h"
#include "input_handler.h"
#include "led_control.h"
#include "interrupts.h"

uint8_t LED_Status = 0;
uint8_t MODE_STATUS = 1;
uint8_t playing_toggle = 0;
char buffer_input[7];
uint8_t buffer_reset = 0;
uint32_t buffer_index = 0;
uint8_t song_number = 0;
button S1 = {0};

#include <stdint.h>
#include "printf.h"
#include "systick.h" // IMPORTANT: ADJUST RVR TO BE 79
#include "tone.h"
#include "dac.h"

void run_project() {
    DAC_Init();
    DAC_Start();

    S1.last_push_time = 0;
    display_menu();
    printf(">>> ");

    while (1) {
        read_input_string(buffer_input, 7);
        if (MODE_STATUS) {
            handle_user_input(buffer_input);
        } else {
            if (S1.double_press_timeout < get_total_count() && S1.activate_single_press && !S1.is_pressed) {
                S1.double_press_timeout = 0x7FFFFFFF;
                S1.activate_single_press = 0;
                LED_Status = (LED_Status == 1) ? 2 : 1;
            }
        }
        handle_LED();
    }
}

