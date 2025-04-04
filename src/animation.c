// animation.c
#include "config.h"
#include "LCD.h"
#include "animation.h"


// Animation state variables
static volatile uint8_t animation_active = 0;  // Flag to indicate if animation is running
static volatile uint8_t animation_tick = 0;    // Flag to indicate a step is due
static uint8_t position = 0;                   // Current column of the dot (0 to 15)
static int8_t direction = 1;                   // 1 for right, -1 for left
static uint8_t gie_state = 0;                  // To store GIE state

// Timer0 setup for ~100 ms interrupts (4 MHz clock, Fosc/4 = 1 MHz)
void timer0_init(void) {
    T0CON0 = 0x80;  // Enable Timer0, 8-bit mode, postscaler 1:1
    T0CON1 = 0x47;  // Fosc/4, prescaler 1:128, sync off
    // Overflow: 256 ticks * 128 us = 32,768 us (32.768 ms)
    // 3 overflows * 32.768 ms ≈ 98.304 ms (~100 ms)
    TMR0L = 0;
    TMR0IF = 0;
    TMR0IE = 0;
}

void __interrupt(irq(TMR0)) animation_isr() {
    static uint8_t timer_counter = 0;
    
    if (TMR0IF && TMR0IE) {
        timer_counter++;
        if (timer_counter >= 3) {  // ~100 ms
            timer_counter = 0;
            if(animation_active) {
                animation_tick = 1;
            }
        }
        TMR0IF = 0;
    }
}

void __interrupt(irq(default)) undefined_isr() {
    //do nothing, there shouldnt be any other interrupts anyway. This just gets rid of annoying compiler error
}

void animation_start(void) {
    animation_stop(); //stop existing animation(s)
    
    position = 0;
    direction = 1;
    animation_active = 1;
    
    // Clear the bottom row of the LCD
    lcd_clear_row(1);
    
    gie_state = INTCON0bits.GIE;
    TMR0IE = 1;
    INTCON0bits.GIE = 1;
}

void animation_stop(void) {
    animation_active = 0;
    position = 0;
    direction = 1;
    TMR0IE = 0;
    
    // Clear the bottom row when stopping
    lcd_clear_row(1);
    
    if(!gie_state) {
        INTCON0bits.GIE = 0;
    }
}

void animation_update(void) {
    if(animation_tick && animation_active) {
        animation_tick = 0;
        
        // Clear the bottom row
        lcd_clear_row(1);
        
        switch (position) {
        case 0:
            lcd_set_cursor(1, 0);
            lcd_print("Oo.");
            break;
        case 1:
            lcd_set_cursor(1, 0);
            lcd_print("oOo.");
            break;
        case 14:
            lcd_set_cursor(1, 12);
            lcd_print(".oOo");
            break;
        case 15:
            lcd_set_cursor(1, 13);
            lcd_print(".oO");
            break;
        default:
            lcd_set_cursor(1, position - 2);
            lcd_print(".oOo.");
        }
        
        // Update position and direction
        if(position == 15) {
            direction = -1;  // Move left
        } else if(position == 0) {
            direction = 1;   // Move right
        }
        position += direction;
    }
}

uint8_t animation_is_active(void) {
    return animation_active;
}
