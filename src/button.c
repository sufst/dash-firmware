#include "button.h"
#include <xc.h>  // Required for PIC register access

#define BUTTON_PORT PORTAbits.RA1  // RA1 pin
#define DEBOUNCE_DELAY 5           // Number of stable cycles before confirming press (adjustable)

static bool button_state = false;
static bool last_raw_state = false;
static uint8_t stable_count = 0;

void Button_Init(void) {
    TRISAbits.TRISA1 = 1;  // Set RA1 as input
    button_state = false;
    stable_count = 0;
}

void Button_Update(void) {
    bool current_raw = BUTTON_PORT == 0; // Active-low logic

    if (current_raw == last_raw_state) {
        if (stable_count < DEBOUNCE_DELAY) {
            stable_count++;
            if (stable_count >= DEBOUNCE_DELAY) {
                button_state = current_raw;
            }
        }
    } else {
        stable_count = 0;
    }

    last_raw_state = current_raw;
}

bool Button_IsPressed(void) {
    return button_state;
}