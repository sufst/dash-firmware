#include "config.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "LCD.h"

#define BUTTON_PIN PORTAbits.RA0


void button_init(void) {
    TRISAbits.TRISA0 = 1;     // RA0 as input
    ANSELAbits.ANSELA0 = 0;   // Disable analog function on RA0

    INTCONbits.INTE = 1;      // Enable INT0 external interrupt
    INTCONbits.INTF = 0;      // Clear INT0 interrupt flag
    OPTION_REGbits.INTEDG = 0; // Trigger on falling edge (active low)
    INTCONbits.GIE = 1;       // Enable global interrupts
}

// Interrupt Service Routine (ISR) for INT0 (RA0)
void __interrupt() button_isr(void) {
    if (INTCONbits.INTF) {
        if (PORTAbits.RA0 == 0) {

            // Cycle through the states
            switch (current_state) {
                case STATE_NORMAL:
                    current_state = STATE_TEMPS;
                    break;
                case STATE_TEMPS:
                    current_state = STATE_POWER;
                    break;
                case STATE_POWER:
                    current_state = STATE_NORMAL;
                    break;
            }

            __delay_ms(20); // debounce
        }

        INTCONbits.INTF = 0;
    }
}

