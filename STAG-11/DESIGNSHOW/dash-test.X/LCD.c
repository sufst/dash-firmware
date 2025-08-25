#include "mcc_generated_files/mcc.h"
#include <stdint.h>
#include "LCD.h"


void I2C1_SetBuffer_4bitmode(uint8_t data, uint8_t rs ){
    uint8_t buffer[4];
    uint8_t data_high = (data >> 4) & 0x0F;
    uint8_t data_low = data & 0x0F;
    

        buffer[0] = (uint8_t)(data_high << 4) | LCD_EN | (rs ? LCD_RS : 0);   
        buffer[1] = (uint8_t)(data_high << 4) | (rs ? LCD_RS : 0);            
        buffer[2] = (uint8_t)(data_low << 4) | LCD_EN | (rs ? LCD_RS : 0);    
        buffer[3] = (uint8_t)(data_low << 4) | (rs ? LCD_RS : 0);
    
   
    I2C1_SetBuffer(buffer, 4);
}

void LCD_send_nibble_cmd(uint8_t data_high){
     
    uint8_t buffer[2];
    
    // First byte: high nibble with enable on
    buffer[0] = (uint8_t)(data_high << 4) | LCD_EN;   
    // Second byte: high nibble with enable off
    buffer[1] = (uint8_t)(data_high << 4);            
    
        I2C1_Open(LCD_ADDRESS);
       
        I2C1_SetBuffer(buffer, 2);
       
        I2C1_MasterWrite();

        // Optional: Close communication
        I2C1_Close();
        __delay_us(50);
}

void LCD_send_byte(uint8_t data, uint8_t rs){
    
        I2C1_Open(LCD_ADDRESS);
    
        I2C1_SetBuffer_4bitmode(data, rs);
        I2C1_MasterWrite();
        
        I2C1_Close();
        __delay_us(50);
    
   
    
}
void LCD_init(){
    
    LCD_send_nibble_cmd(0x03); // 8 bit mode
    __delay_ms(5);
    LCD_send_nibble_cmd(0x03); // Again for confirmation
    __delay_ms(150);
    LCD_send_nibble_cmd(0x02); // 4 bit mode
    
    LCD_send_byte(LCD_FUNCTION_SET_2LINE, 0);
    LCD_send_byte(LCD_DISP_ON_CURSOR_OFF, 0);
    LCD_send_byte(LCD_CLEAR, 0);
    LCD_send_byte(LCD_HOME, 0);
    LCD_send_byte(LCD_INC_CURSOR, 0);
    
   
}

void LCD_clear() {
    LCD_send_byte(LCD_CLEAR, 0);
    __delay_ms(2);
}

void LCD_clear_row(uint8_t row) {
     LCD_set_cursor(row, 0);
     LCD_print("                ");
}

void LCD_home() {
    LCD_send_byte(LCD_HOME, 0);
    __delay_ms(2);
}

void LCD_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? LCD_FORCE_CURSOR_LINE1 : LCD_FORCE_CURSOR_LINE2;
    address += col;
    LCD_send_byte(address | LCD_FORCE_CURSOR_LINE1, 0);
}

void LCD_write_char(char c) {
    LCD_send_byte(c, 1);  // RS = 1 for data
}

void LCD_write_string(const char *str) {
    while(*str) {
        LCD_send_byte(*str++, 1);
    }
}

void LCD_print(const char *str, ...) {
    char buffer[17];
    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);
    va_end(args);
    LCD_write_string(buffer);
}

void LCD_shift_cursor_left(void) {
    LCD_send_byte(LCD_SHIFT_CURSOR_LEFT, 0);  // RS = 0 for command
}

void LCD_shift_cursor_right(void) {
    LCD_send_byte(LCD_SHIFT_CURSOR_RIGHT, 0);  // RS = 0 for command
}

void LCD_shift_display_left(void) {
    LCD_send_byte(LCD_SHIFT_DISP_LEFT, 0);  // RS = 0 for command
}

void LCD_shift_display_right(void) {
    LCD_send_byte(LCD_SHIFT_DISP_RIGHT, 0);  // RS = 0 for command
}

void LCD_display_off_cursor_off(void) {
    LCD_send_byte(LCD_DISP_OFF_CURSOR_OFF, 0);  // RS = 0 for command
}

void LCD_display_off_cursor_on(void) {
    LCD_send_byte(LCD_DISP_OFF_CURSOR_ON, 0);  // RS = 0 for command
}

void LCD_display_on_cursor_off(void) {
    LCD_send_byte(LCD_DISP_ON_CURSOR_OFF, 0);  // RS = 0 for command
}

void LCD_display_on_cursor_blink(void) {
    LCD_send_byte(LCD_DISP_ON_CURSOR_BLINK, 0);  // RS = 0 for command
}