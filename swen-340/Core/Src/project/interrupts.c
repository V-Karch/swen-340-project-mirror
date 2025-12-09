#include "interrupts.h"
#include "input_handler.h"
#include "project.h"
#include <stdio.h>
#include "main.h"
#include "GPIO.h"

extern uint8_t MODE_STATUS;
extern uint8_t LED_Status;
extern button S1;

void EXTI15_10_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(B1_Pin);
    if (MODE_STATUS == 1) {
        MODE_STATUS = 0;
        printf("\r\n***MANUAL OVERRIDE MODE ACTIVE***\r\n>>>");
    } else {
        MODE_STATUS = 1;
        printf("\r\n***REMOTE MODE ACTIVE***\r\n>>>");
    }
}

void EXTI9_5_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(S1_Pin);
    if (MODE_STATUS == 1) return;
    if (S1.last_push_time + 15000 > get_total_count()) return;

    S1.is_pressed = HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin);

    if (S1.last_push_time + 1000000 > get_total_count() && S1.is_pressed == 1) {
        next_song_display();
        S1.double_press_timeout = 0x7FFFFFFF;
        S1.activate_single_press = 0;
    } else if (S1.last_push_time + 1000000 < get_total_count() && S1.is_pressed == 0) {
        LED_Status = 0;
        S1.double_press_timeout = 0x7FFFFFFF;
        S1.activate_single_press = 0;
    } else if (S1.is_pressed == 1) {
        S1.activate_single_press = 1;
        S1.double_press_timeout = get_total_count() + 1000000;
        S1.last_push_time = get_total_count();
        return;
    }
}
