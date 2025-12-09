#include "led_control.h"
#include "project.h"

extern uint8_t LED_Status;

void handle_LED() {
    switch (LED_Status) {
        case 0: LED_Off(); break;
        case 1: LED_On(); break;
        case 2:
            if ((get_total_count() % 2000000) / 1000000)
                LED_On();
            else
                LED_Off();
            break;
    }
}
