/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "LCD.h"
#include "CAN_handler.h"
#include "main.h"
#include "animation.h"
#include "7seg.h"
#include "power.h"
/*
                         Main application
 */



void vcu_number(DashboardData_t* data) {
  if (data->VCU_CTRL_State < 10) {
      LCD_set_cursor(0,15);
      LCD_print("%u", data->VCU_CTRL_State);
  } else {
      LCD_set_cursor(0,14);
      LCD_print("%u", data->VCU_CTRL_State);
  }
}

void wait_animation(void) {
        // If the animation is not active, start it
        // If it is active, update it
 if (!animation_active) animation_start();
        else animation_update();
}

void display_ts_on_state(InfoState_t current_state, DashboardData_t* data);

DashboardData_t data = {0}; //global

volatile uint32_t R2Duptime_seconds = 0; //global
volatile uint16_t button_hold_ticks = 0;
volatile uint8_t button_press_active = 0;
volatile uint8_t reverse_active = 0;



void main(void)
{
    // Initialize system (calls I2C, oscillator, pin manager)
    SYSTEM_Initialize();

    
    LCD_init();
    seg_init();
    timer0_init();
    
    // Register custom interrupt handler for RA0
    IOCAF0_SetInterruptHandler(ButtonPressHandler);
    TMR3_SetInterruptHandler(ButtonHoldTimer);
    
    TMR1_SetInterruptHandler(RealTimeClockHandler);
    TMR0_SetInterruptHandler(animation_isr);

    // Enable global interrupts
    INTERRUPT_GlobalInterruptEnable();
    
    
    // Startup Screen
      
    LCD_clear();
    LCD_set_cursor(0,6);
    LCD_write_string("SUFST");
    LCD_set_cursor(1,5);
    LCD_write_string("STAG 11");
    seg_display_digits(69);
    printf("STARTUP\n");
    __delay_ms(2000);
    
    
    while(1){
        ProcessCANMessages();
        GetDashboardData(&data);
        
        seg_display_digits(data.bms_soc);
        
        switch (data.VCU_CTRL_State)
        {
            case 0:
                if (display_state != STATE_TS_BUTTON_WAIT)
                {
                    LCD_clear();
                    LCD_set_cursor(0,0);
                    LCD_print("PRESS TSON");
                    vcu_number(&data);
                    display_state = STATE_TS_BUTTON_WAIT;
                }
                //wait_animation();
                break;
            
            case 1:
                if(display_state != STATE_WAIT_NEG_AIR)
                {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0,0);
                    LCD_print("NEG AIR");
                    vcu_number(&data);
                    display_state = STATE_WAIT_NEG_AIR;
                }
                break;
                
            case 2:
                if (display_state != STATE_PRECHARGE_WAIT) 
                {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("PRECHARGE");
                    vcu_number(&data);
                    display_state = STATE_PRECHARGE_WAIT;
                }
                break;

            case 3:
                if (display_state != STATE_R2D_WAIT) 
                {
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("Press R2D+BPS");
                    vcu_number(&data);
                    
                    display_state = STATE_R2D_WAIT;
                }
                LCD_set_cursor(1, 0);
                LCD_print("Volts: %03dV", (data.BMS_inst_volt / 10));
                //wait_animation();
                break;
                
            case 4:
                if (display_state != STATE_TS_ON)
                {
                    animation_stop();
                    LCD_clear();
                    display_state = STATE_TS_ON;
                }
                power_sample_instant(data.BMS_inst_volt, data.BMS_pack_current);
                display_tson_state(current_state, &data);
                break;
                
            case 5:
                if (display_state != STATE_R2D_OFF) {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("R2D OFF");
                    vcu_number(&data);
                    display_state = STATE_R2D_OFF;
                }
                break;

            case 6:
                if (display_state != STATE_RTD_OFF_WAIT) {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("RTD OFF WAIT");
                    vcu_number(&data);
                    display_state = STATE_RTD_OFF_WAIT;
                }
                break;

            case 7:
                if (display_state != STATE_TS_ACTIVATION_FAILURE) {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("TS Activation");
                    LCD_set_cursor(1, 0);
                    LCD_print("Failure");
                    vcu_number(&data);
                    display_state = STATE_TS_ACTIVATION_FAILURE;
                }
                break;

            case 8:
                if (display_state != STATE_TS_RUN_FAULT) {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("TS Run Fault");
                    vcu_number(&data);
                    display_state = STATE_TS_RUN_FAULT;
                }
                break;

            case 9:
                if (display_state != STATE_SPIN) {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("SPIN");
                    vcu_number(&data);
                    display_state = STATE_SPIN;
                }
                break;

            case 10:
                if (display_state != STATE_APPS_SCS_FAULT) {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("APPS SCS");
                    LCD_set_cursor(1, 0);
                    LCD_print("Fault");
                    vcu_number(&data);
                    display_state = STATE_APPS_SCS_FAULT;
                }
                break;

            case 11:
                if (display_state != STATE_APPS_BPS_FAULT) {
                    animation_stop();
                    LCD_clear();
                    LCD_set_cursor(0, 0);
                    LCD_print("APPS BPS");
                    LCD_set_cursor(1, 0);
                    LCD_print("Fault");
                    vcu_number(&data);
                    display_state = STATE_APPS_BPS_FAULT;
                }
                break;
                  
        
        
        

        }
    }
}

void display_tson_state(InfoState_t current_state, DashboardData_t* data)
{
    static InfoState_t last_state = NORMAL; // Track the last state to detect changes

    // Clear LCD only when the state changes
    if (current_state != last_state) {
        LCD_clear();
        last_state = current_state;
    }
    switch (current_state) {
        case NORMAL:
            LCD_set_cursor(0,1);
            LCD_print("%02d", data->bms_high_temp);
            LCD_write_char(223); // degree symbol
            LCD_write_char('C'); // uppercase C
            LCD_set_cursor(0,10);
            LCD_print("%02lu:%02lu", R2Duptime_seconds / 60, R2Duptime_seconds % 60);
            LCD_set_cursor(1,1);
            LCD_print("%02dkW", power_get_instant_kw());
            LCD_set_cursor(1, 6);
            LCD_print("%s", power_get_bar());
            break;
            
        case TEMPS:
            LCD_set_cursor(0, 1);
            LCD_print("H:%02d", data->bms_high_temp);
            LCD_write_char(223);
            LCD_write_char('C');
            LCD_print(" A:%02d", data->bms_avg_temp);
            LCD_write_char(223);
            LCD_write_char('C');

            LCD_set_cursor(1, 1);
            LCD_print("L:%02d", data->bms_low_temp);
            LCD_write_char(223);
            LCD_write_char('C');
            LCD_print(" I:%02d", data->bms_internal_temp);
            LCD_write_char(223);
            LCD_write_char('C');
            break;
            
        case POWER:
            LCD_set_cursor(0, 1);
            LCD_print("%02d", data->bms_high_temp);
            LCD_write_char(223);
            LCD_write_char('C');
            LCD_print(" Pwr:%02dkW", power_get_instant_kw());
            LCD_set_cursor(1, 6);
            LCD_print("Avg:%02dkW", power_get_average_kw());
            break;

            
    }
}

void ButtonPressHandler(void) {
    
    
    if (PORTAbits.RA0 == 0) { // Button Pressed
        button_press_active = 1;
        button_hold_ticks = 0;
        TMR3_StartTimer();  
    } else { // Button Released
        TMR3_StopTimer();
        
        if (button_press_active){
            button_press_active = 0;
            
            if(button_hold_ticks >= 125) {
                //Long Press action
                printf("Long Button Press");
            } else {
                //Short Press action
                printf("Short Button Press");
                // Cycle through InfoState_t values: NORMAL -> TEMPS -> POWER -> NORMAL
                switch (current_state) {
                case NORMAL: current_state = TEMPS;  break;
                case TEMPS:  current_state = POWER;  break;
                case POWER:  current_state = NORMAL; break;
                default:     current_state = NORMAL; break;
                }
            }
        }
    }
}

void ButtonHoldTimer(void) {
    if (button_press_active) {
        button_hold_ticks++;
    }
}


void RealTimeClockHandler(void){
    
    // sample for average power
    DashboardData_t data = {0};
    GetDashboardData(&data);
    power_sample_average(data.BMS_inst_volt, data.BMS_pack_current);
    
    //button press timer
    //clock
    if (data.VCU_CTRL_State >= 4){
        R2Duptime_seconds++;
    } else if (data.VCU_CTRL_State == 3) {
        R2Duptime_seconds = 0;
    }
    
   
}


/**
 End of File
*/