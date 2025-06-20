/* 
 * File:   LCD.h
 * Author: Adam
 *
 * Created on June 11, 2025, 4:50 PM
 */

#ifndef LCD_H
#define	LCD_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define LCD_ADDRESS 0x38

// LCD Command Codes
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_DEC_CURSOR      0x04
#define LCD_INC_CURSOR      0x06
#define LCD_SHIFT_DISP_RIGHT 0x05
#define LCD_SHIFT_DISP_LEFT 0x07
#define LCD_DISP_OFF_CURSOR_OFF 0x08
#define LCD_DISP_OFF_CURSOR_ON  0x0A
#define LCD_DISP_ON_CURSOR_OFF  0x0C
#define LCD_DISP_ON_CURSOR_ON 0x0E
#define LCD_DISP_ON_CURSOR_BLINK 0x0F
#define LCD_SHIFT_CURSOR_LEFT  0x10
#define LCD_SHIFT_CURSOR_RIGHT 0x14
#define LCD_SHIFT_DISP_LEFT_ALL 0x18
#define LCD_SHIFT_DISP_RIGHT_ALL 0x1C
#define LCD_FORCE_CURSOR_LINE1 0x80
#define LCD_FORCE_CURSOR_LINE2 0xC0
#define LCD_FUNCTION_SET_2LINE 0x28

// PCF8574 control bits (no backlight control since P3 is disconnected)
#define LCD_EN              0x04  // Enable bit
#define LCD_RW              0x02  // Read/Write bit
#define LCD_RS              0x01  // Register select bit

// Low Level Function prototypes
void LCD_send_nibble_cmd(uint8_t nibble);
void LCD_send_byte(uint8_t byte, uint8_t rs);
void I2C1_SetBuffer_4bitmode(uint8_t data, uint8_t rs);

// High Level Function prototypes
void LCD_init(void);
void LCD_clear(void);
void LCD_clear_row(uint8_t row);
void LCD_home(void);
void LCD_set_cursor(uint8_t row, uint8_t col);
void LCD_write_char(char c);
void LCD_write_string(const char *str);
void LCD_print(const char *str, ...);
void LCD_shift_cursor_left(void);
void LCD_shift_cursor_right(void);
void LCD_shift_display_left(void);
void LCD_shift_display_right(void);
void LCD_display_off_cursor_off(void);
void LCD_display_off_cursor_on(void);
void LCD_display_on_cursor_off(void);
void LCD_display_on_cursor_on(void);
void LCD_display_on_cursor_blink(void);

#endif	/* LCD_H */

