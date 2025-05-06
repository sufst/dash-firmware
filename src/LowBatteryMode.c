#include "LowBatteryMode.h"
#include "LCD.h"         
#include "utils.h"       

#define LOW_BATTERY_THRESHOLD 20        // to define the threshold value for low battery alert
#define FLASH_INTERVAL_MS     500       // time between flashing LCD 

static bool lcd_flash_on = false;       // store the state of the LCD 
static uint32_t last_flash_time = 0;    // record the time when last time the LCD flashed

void battery_monitor_update(uint8_t soc) {

    if (soc <= LOW_BATTERY_THRESHOLD) {                // when the battery SOC is lower than the threshold it will run the loop
  
        uint32_t now = millis();                       // record the time 
                                                       
        if (now - last_flash_time >= FLASH_INTERVAL_MS) {  // if the interval satisfies the FLASH INTERVAL then change the LCD state to make sure it is flashing

            last_flash_time = now;                     // refresh the time 
            lcd_flash_on = !lcd_flash_on;              // change the state of the LCD 

            lcd_clear();                               // clear the LCD 

            if (lcd_flash_on) {
                lcd_set_cursor(0, 0);
                lcd_print("LOW BATTERY");              // show the warning
            }
        }
    }
}


//battery_monitor_update(dashboard_data.bms_soc);

//the sentence above should be added to the main c file and be ran periodly
//the reason why its here is to make life easier when changing the program
//this is the sentence to run this function or this file so please DO ADD IT TO MAIN FUNCTION