//animation.c

#include "config.h"
#include "animation.h"
#include "Seg.h"
#include "Seg.c"

// Structure to define a step in the spinning animation (digit and segment)
typedef struct {
    uint8_t digit;    // 0 for D0 (units), 1 for D1 (tens)
    uint8_t pattern;  // Segment pattern (e.g., SEG_A, SEG_B, etc.)
} Step;

// Spinning animation pattern (single continuous circle: D0A -> D0B -> D0C -> D0D -> D1D -> D1E -> D1F -> D1A)
static const Step pattern[] = {
    {0, SEG_A},  // D0A: Digit 0, Segment A (top)
    {0, SEG_B},  // D0B: Digit 0, Segment B (top-right)
    {0, SEG_C},  // D0C: Digit 0, Segment C (bottom-right)
    {0, SEG_D},  // D0D: Digit 0, Segment D (bottom)
    {1, SEG_D},  // D1D: Digit 1, Segment D (bottom)
    {1, SEG_E},  // D1E: Digit 1, Segment E (bottom-left)
    {1, SEG_F},  // D1F: Digit 1, Segment F (top-left)
    {1, SEG_A}   // D1A: Digit 1, Segment A (top)
};
#define SPINNING_STEPS (sizeof(pattern) / sizeof(pattern[0]))  // 8 steps

// Animation state variables
static volatile uint8_t animation_active = 0;  // Flag to indicate if animation is running
static volatile uint8_t animation_step = 0;  // Current step in the animation

// External reference to display_buffer from seg.c
extern uint8_t display_buffer[4];

// Timer0 setup for ~100 ms interrupts (assuming 4 MHz clock, Fosc/4 = 1 MHz, 1 us per instruction cycle)
static void timer0_init(void) {
    // Configure Timer0 as 8-bit timer with prescaler
    T0CON0 = 0x80;  // Enable Timer0, 8-bit mode, postscaler 1:1
    T0CON1 = 0x47;  // Fosc/4 clock source, prescaler 1:128, sync off
    
    // Timer0 counts from TMR0L to 256, then overflows
    // Tick time = 1 us * 128 = 128 us
    // 256 ticks * 128 us = 32,768 us (32.768 ms)
    // Use a software counter to get ~100 ms
    
    TMR0L = 0;  // Start at 0, count to 256
    PIR3bits.TMR0IF = 0;  // Clear Timer0 interrupt flag
    PIE3bits.TMR0IE = 0;  // Disable Timer0 interrupt initially
}

// Timer0 Interrupt Service Routine
void __interrupt() animation_isr(void) {
    static uint8_t timer_counter = 0;  // Software counter to achieve ~100 ms
    
    if (PIR3bits.TMR0IF) {
        timer_counter++;
        if (timer_counter >= 3) {  // 3 overflows * 32.768 ms ≈ 98.304 ms (close to 100 ms)
            timer_counter = 0;
            
            if (animation_active) {
                // Update the animation step
                // Clear both digits
                display_buffer[0] = 0x00;  // Clear upper byte for digit 1 (tens)
                display_buffer[1] = 0x00;  // Clear lower byte for digit 1 (tens)
                display_buffer[2] = 0x00;  // Clear upper byte for digit 0 (units)
                display_buffer[3] = 0x00;  // Clear lower byte for digit 0 (units)
                
                // Set the segment for the current digit
                if (pattern[animation_step].digit == 0) {
                    // Digit 0 (units)
                    display_buffer[3] = pattern[animation_step].pattern;
                } else {
                    // Digit 1 (tens)
                    display_buffer[1] = pattern[animation_step].pattern;
                }
                
                seg_write_display();
                
                // Advance to the next step
                animation_step++;
                if (animation_step >= SPINNING_STEPS) {
                    animation_step = 0;  // Loop back to the first step
                }
            }
        }
        PIR3bits.TMR0IF = 0;  // Clear Timer0 interrupt flag
    }
}

void animation_start(void) {
    // Stop any existing animation
    animation_stop();
    
    // Reset animation state
    animation_step = 0;
    animation_active = 1;
    
    // Initialize Timer0 if not already done
    timer0_init();
    
    // Enable Timer0 interrupt to start the animation
    PIE3bits.TMR0IE = 1;
    INTCON0bits.GIE = 1;  // Enable global interrupts
}

void animation_stop(void) {
    animation_active = 0;
    animation_step = 0;
    PIE3bits.TMR0IE = 0;  // Disable Timer0 interrupt
    seg_clear();
}
