/* 
 * File:   7seg.h
 * Author: Adam
 *
 * Created on June 16, 2025, 2:03 PM
 */

#ifndef SEG_H
#define	SEG_H

#include <stdint.h>

#define SEG_ADDRESS 0x70

// Define 7-segment display patterns (common cathode, adjust for common anode if needed)
#define SEG_A 0x01
#define SEG_B 0x02 
#define SEG_C 0x04
#define SEG_D 0x08
#define SEG_E 0x10 
#define SEG_F 0x20 
#define SEG_G 0x40
#define SEG_DP 0x80  // Decimal point

// HT16K33 Commands
#define HT16K33_OSCILLATOR_ON  0x21  // Turn on oscillator
#define HT16K33_DISPLAY_ON     0x81  // Display on, no blinking
#define HT16K33_DISPLAY_OFF     0x80  // Display off


void seg_init(void);
void seg_display_digits(uint8_t value);
void seg_display_dp(uint8_t position, uint8_t state);
void seg_clear(void);
#endif	/* 7SEG_H */

