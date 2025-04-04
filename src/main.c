// main.c
#include "LCD.h"
#include "Seg.h"
#include "CAN.h"
#include "config.h"
#include "i2c.h"

void main(void) {
  OSCCON1 = 0x60; // HFINTOSC, no divider
  OSCFRQ = 0x02;  // 4 MHz

  i2c_init();
  can_init();

  lcd_init(); 
  seg_init(); 

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

    if (!data.TSON) {
      lcd_set_cursor(0, 0);
      lcd_print(" Waiting for ON");
    } else {
      if (!data.R2D) {
        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_print(" Press Brake &");
        lcd_set_cursor(1, 0);
        lcd_print(" Ready to Drive");

      } else {

        // Display on LCD (e.g., Line 1: Speed, Line 2: RPM)
        lcd_set_cursor(0, 0);
        lcd_print("BMS Temp: %d", data.bms_temp);
        lcd_write_char(223); // degree symbol
        lcd_write_char(67);  // uppercase C
        lcd_set_cursor(1, 0);
        lcd_print("RPM: %d", data.placeholder);

        // Display on 7-segment (e.g., speed)
        seg_display_digits(data.bms_soc);

      }
    }
  }
}
