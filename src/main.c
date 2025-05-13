// main.c
#include "LCD.h"
#include "Seg.h"
#include "CAN.h"
#include "config.h"
#include "i2c.h"
#include "animation.h"

typedef enum {
  STATE_STARTUP,
  STATE_TS_BUTTON_WAIT,
  STATE_WAIT_NEG_AIR,
  STATE_PRECHARGE_WAIT,
  STATE_R2D_WAIT,
  STATE_TS_ON,
} DisplayState_t;

typedef enum {
  NORMAL,
  TEMPS,
  POWER,
} InfoState_t;

extern volatile InfoState_t current_state = NORMAL;

DisplayState_t display_state = STATE_STARTUP;


void main(void) {
  OSCCON1 = 0x60; // HFINTOSC, no divider
  OSCFRQ = 0x02;  // 4 MHz

  i2c_init();
  can_init();
  lcd_init(); 
  seg_init();
  button_init();
  timer0_init();

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

    switch (data.VCU_CTRL_State) {
      case 0:
        if (display_state != STATE_TS_BUTTON_WAIT) {
          lcd_clear();
          lcd_set_cursor(0, 0);
          lcd_print("Press TSON");
          lcd_set_cursor(0, 15);
          lcd_print("%u", data.VCU_CTRL_State);
          display_state = STATE_TS_BUTTON_WAIT;
        }
        if (!animation_is_active()) animation_start();
        animation_update();
        break;
      
      case 1:
        if (display_state != STATE_WAIT_NEG_AIR) {
          animation_stop();
          lcd_clear();
          lcd_set_cursor(0, 0);
          lcd_print("NEG AIR");
          lcd_set_cursor(0, 15);
          lcd_print("%u", data.VCU_CTRL_State);
          display_state = STATE_WAIT_NEG_AIR;
        }
        break;

      case 2:
        if (display_state != STATE_PRECHARGE_WAIT) {
          lcd_clear();
          lcd_set_cursor(0, 0);
          lcd_print("PRECHARGE");
          lcd_set_cursor(0, 15);
          lcd_print("%u", data.VCU_CTRL_State);
          display_state = STATE_PRECHARGE_WAIT;
        }
        break;

      case 3:
        if (display_state != STATE_R2D_WAIT) {
          lcd_clear();
          lcd_set_cursor(0, 0);
          lcd_print("Press R2D+BPS");
          lcd_set_cursor(0, 15);
          lcd_print("%u", data.VCU_CTRL_State);
          display_state = STATE_R2D_WAIT;
        }
        if (!animation_is_active()) animation_start();
        animation_update();
        break;

      case 4:
        if (display_state != STATE_TS_ON) {
          animation_stop();
          lcd_clear();
          display_state = STATE_TS_ON;
        }

        power_update();
        switch (current_state) {

          case NORMAL:
          lcd_set_cursor(0, 1);
          lcd_print("%d", data.bms_avg_temp);
          lcd_write_char(223); // degree symbol
          lcd_write_char(67);  // uppercase C
          //put the time since R2D pressed on top line
          lcd_set_cursor(1, 1);
          lcd_print("%dkW", power_get_instant_kw());
          lcd_set_cursor(1, 6);
          lcd_print(power_get_bar());
          break;

          case TEMPS:
          lcd_set_cursor(0, 1);
          lcd_print("%d", data.bms_avg_temp);
          lcd_write_char(223); // degree symbol
          lcd_write_char(67);  // uppercase C
          lcd_print(" %d", data.bms_high_temp);
          lcd_write_char(223); // degree symbol
          lcd_write_char(67);  // uppercase C
          lcd_set_cursor(1, 1);
          lcd_print("%d", data.bms_internal_temp);
          lcd_write_char(223); // degree symbol
          lcd_write_char(67);  // uppercase C
          lcd_print(" %d", data.bms_low_temp);
          lcd_write_char(223); // degree symbol
          lcd_write_char(67);  // uppercase C
          break;

          case POWER:
          lcd_set_cursor(0, 1);
          lcd_print("%d", data.bms_avg_temp);
          lcd_write_char(223); // degree symbol
          lcd_write_char(67);  // uppercase C
          lcd_print("Pwr:%dkW", power_get_instant_kw());
          lcd_set_cursor(1, 7);
          lcd_print("Avg:%dkW", power_get_average_kw());
          break;

        }

        break;

      case 5:
      
    }
  }
}

