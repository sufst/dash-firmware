#include "mcc_generated_files/mcc.h"
#include "7seg.h"

// Lookup table for 7-segment patterns (0-9, common cathode)
static const uint8_t seg_patterns[] = {
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,      // 0
    SEG_B | SEG_C,                                     // 1
    SEG_A | SEG_B | SEG_D | SEG_E | SEG_G,             // 2
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,             // 3
    SEG_B | SEG_C | SEG_F | SEG_G,                     // 4
    SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,             // 5
    SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,     // 6
    SEG_A | SEG_B | SEG_C | SEG_F,                             // 7
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G, // 8
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G      // 9
};

static uint8_t display_buffer[4] = {0};

static void seg_send_command(uint8_t command)
{
    uint8_t buffer[1];
    buffer[0] = (uint8_t)command;
    I2C1_Open(SEG_ADDRESS);
    I2C1_SetBuffer(buffer, 1);
    I2C1_MasterWrite();
    I2C1_Close();
    __delay_us(50);
}

static void seg_write_display(void)
{
    uint8_t tx_buffer[1 + sizeof(display_buffer)];
    tx_buffer[0] = 0x00;
            
    for (uint8_t i = 0; i < sizeof(display_buffer); i++ ) {
        tx_buffer[1+i] = display_buffer[i];
    }
    
    I2C1_Open(SEG_ADDRESS);
    I2C1_SetBuffer(tx_buffer , sizeof(tx_buffer));
    I2C1_MasterWrite();
    I2C1_Close();
}

void seg_init(void)
{
    seg_send_command(HT16K33_OSCILLATOR_ON);
    seg_send_command(HT16K33_DISPLAY_ON);
    seg_send_command(0xEF);
    seg_clear();
}

void seg_display_digits(uint8_t value)
{
    if (value > 99) value = 99;
    
    uint8_t tens = value / 10;
    uint8_t units = value % 10;
    
    display_buffer[0] = seg_patterns[tens];
    display_buffer[1] = 0x00; // Clear upper byte
    
    display_buffer[2] = seg_patterns[units];  
    display_buffer[3] = 0x00; // Clear lower byte
    
    seg_write_display();
}

void seg_display_dp(uint8_t position, uint8_t state)
{
    uint8_t target_buffer_index;
    
    if (position == 0) {
        target_buffer_index = 2; //units digit
    } else if (position == 1) {
        target_buffer_index = 0;
    } else {
        return; //invalid position
    }
    
    if (state) {
        display_buffer[target_buffer_index] |= SEG_DP;
    } else {
        display_buffer[target_buffer_index] &= ~SEG_DP;
    }
        seg_write_display();
}

void seg_clear(void) {
    for (uint8_t i = 0; i < 4; i++) {
        display_buffer[i] = 0x00;
    }
    seg_write_display();
}

