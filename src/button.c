#include "config.h"
#include "button.h"

void button_init(){
// configure button pins
TRISAbits.TRISA0 = 1; // RA0 as input
ANSELAbits.ANSELA0 = 0; // disable analogue
INTCON0bits.GIE = 1; // enable global interrupts

}

void __interrupt() button_isr() {
    if (INTCON0bits.INTF) { // Check if interrupt was triggered by INT0
        // Handle button press
        if (PORTAbits.RA0 == 0) { // Check if button is pressed
            // Perform action for button press
            // For example, toggle an LED or send a message
        }
        INTCON0bits.INTF = 0; // Clear the interrupt flag
    }
}

