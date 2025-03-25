#include <xc.h>
#include "i2c.h"
#include "can.h"

#include <pic18f47q84.h>

void main(void) {
    OSCCON1 = 0x60;  // HFINTOSC, no divider
    OSCFRQ = 0x08;   // 64 MHz

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
        lcd_write_string("Speed: %d kph", data.speed_kph);
        lcd_set_cursor(1, 0);
        lcd_write_string("RPM: %d", data.rpm);

        // Display on 7-segment (e.g., speed)
        seg_display_digits(data.speed_kph);
    }
}