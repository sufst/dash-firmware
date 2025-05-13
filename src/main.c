// main.c
#include "LCD.h"
#include "Seg.h"
#include "CAN.h"
#include "config.h"
#include "i2c.h"
#include "animation.h"

typedef enum {
  STATE_STARTUP,
  STATE_WAIT_TSON,
  STATE_WAIT_R2D,
  STATE_DRIVING
} DisplayState_t;

DisplayState_t display_state = STATE_STARTUP;


void main(void) {
  OSCCON1 = 0x60; // HFINTOSC, no divider
  OSCFRQ = 0x02;  // 4 MHz

  i2c_init();
  can_init();
  lcd_init(); 
  seg_init();
  button_init();

  lcd_set_cursor(0, 6);  //Startup screen
  lcd_print("SUFST");
  lcd_set_cursor(1, 5);
  lcd_print("STAG 11");
  __delay_ms(2000);
  lcd_clear();  


  while (1) {
    can_process_messages();
    DashboardData_t data;
    can_get_dashboard_data(&data);

    // Display soc on 7-segment display always
        seg_display_digits(data.bms_soc);

    if (!data.TSON) {
      if (display_state != STATE_WAIT_TSON) {
        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_print("Press TSON");
        display_state = STATE_WAIT_TSON;
      }
      if (!animation_is_active()) animation_start();
      animation_update();

    } else if (!data.R2D) {
        if (display_state != STATE_WAIT_R2D) {
          lcd_clear();
          lcd_set_cursor(0, 0);
          lcd_print("Press Brake+R2D");
          display_state = STATE_WAIT_R2D;
        }
        if (!animation_is_active()) animation_start();
        animation_update();

      } else {
        display_state = STATE_DRIVING;
        animation_stop();

        // Display on LCD (e.g., Line 1: Speed, Line 2: RPM)
        lcd_set_cursor(0, 0);
        lcd_print("BMS Temp: %d", data.bms_temp);
        lcd_write_char(223); // degree symbol
        lcd_write_char(67);  // uppercase C
        lcd_set_cursor(1, 0);
        lcd_print("RPM: %d", data.placeholder);

        }
  }
}
