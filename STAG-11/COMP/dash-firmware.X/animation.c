#include "animation.h"
#include "LCD.h"
#include "mcc_generated_files/mcc.h"


volatile uint8_t animation_active = 0;
volatile uint8_t animation_tick = 0;
volatile uint8_t position = 0;
volatile int8_t direction = 1;


void timer0_init(void) 
{
    T0CON0 = 0x80; // enable timer 0, 8 bit mode, postscaler 1:1
    T0CON1 = 0x47; // Fosc/4 prescaler 1:128, sync off
            
            
    TMR0L = 0;
    TMR0IF = 0;
    TMR0IE = 0;
    
    INTCON0bits.GIE = 1; // ensure global interrupts enabled
}

static uint8_t timer_counter = 0;

void animation_isr() 
{
        timer_counter++;
        if (timer_counter >=25) // ~100 ms
        {
            timer_counter = 0;
            if(animation_active) {
                animation_tick = 1;
            }
        }
        TMR0IF = 0;
    }

void animation_start(void) 
{
    animation_stop(); //stop existing animations
    
    position = 0;
    direction = 1;
    animation_active = 1;
    
    LCD_clear_row(1);
    TMR0IE = 1;
}

void animation_stop(void)
{
    animation_active = 0;
    position = 0;
    direction = 1;
    TMR0IE = 0;
    LCD_clear_row(1);
}

void animation_update(void) {
    if(animation_tick && animation_active) 
    {
        LCD_clear_row(1);
            LCD_set_cursor(1, position - 2);
                
            LCD_write_custom_char(0);
            LCD_write_custom_char(1);
            LCD_write_custom_char(2);
            
        }
        
        if(position == 13)
        {
            direction = -1;
        }
        else if (position == 3)
        {
            direction = 1;
        }
        position += direction;
        animation_tick = 0;
}
