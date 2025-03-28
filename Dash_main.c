#include "config.h"
#include "i2c.h"
#include "can.h"
#include "LCD.h"
#include "Seg.h"

void main(void) {
    OSCCON1 = 0x60;  // HFINTOSC, no divider
    OSCFRQ = 0x02;   // 4 MHz

    i2c_init();
    can_init();

    lcd_init();  // Assume this initializes the 20x4 LCD
    seg_init();  // Assume this initializes the 7-segment

    while (1) {
        can_process_messages();
        DashboardData_t data;
        can_get_dashboard_data(&data);

        // Display on LCD (e.g., Line 1: Speed, Line 2: RPM)
        lcd_set_cursor(0, 0);
        lcd_print("Battery Temp: %d", data.bms_temp);
        lcd_write_char(223);
        lcd_write_char("C");
        lcd_set_cursor(1, 0);
        lcd_print("RPM: %d", data.placeholder);

        // Display on 7-segment (e.g., speed)
        seg_display_digits(data.bms_soc);
    }
}