// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>        // PIC-specific definitions
#include <pic18f47q84.h>
#include <stdint.h>    // Standard integer types (e.g., uint8_t)
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>



#define _XTAL_FREQ 4000000  // Clock frequency for delay functions
#define LCD_ADDRESS 0x20    // Example I2C address for LCD
#define SEG_ADDRESS 0x70    // Example I2C address for 7-segment display

#endif