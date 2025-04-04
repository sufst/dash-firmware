// i2c.c
#include "config.h"
#include "i2c.h"


// Initialize I2C1 module
void i2c_init(void) {
    // Configure I2C1 pins (RC4 = SDA, RC3 = SCL on PIC18F47Q84)
    TRISC = 0b00011000;  // SDA and SCL as input
    
    I2C1CON0 = 0x00;       //
    I2C1CON1 = 0x00;       // Clear control registers
    I2C1CON2 = 0x00;       //

    I2C1CON0bits.MODE = 0b100; // Host mode, 7-bit address
    
    
    I2C1CLK = 0x02;   // Clock selection (HFINTOSC - 4MHz)
    I2C1BAUD = 9;     //Set baud rate for 100kHz @ 4MHz Fosc (Fosc/(4 * BaudRate)) - 1 = (4MHz/(4 * 100kHz)) - 1
    
    // Enable I2C1
    I2C1CON0bits.EN = 1;
}

// Send data byte over I2C
void i2c_send_byte(uint8_t data) {
    I2C1TXB = data;        // Load data into transmit buffer
    while (!I2C1STAT1bits.TXBE);  // Wait for transmit buffer empty
}

// Start I2C transmission
void i2c_start(void) {
    while (!I2C1STAT0bits.BFRE);  // Wait if bus is busy
    I2C1CON0bits.S = 1;    // Generate Start condition
    while (!I2C1CON0bits.S);  // Wait for Start condition to complete
}

// Stop I2C transmission
void i2c_stop(void) {
    while (!I2C1STAT0bits.BFRE);  // Wait if bus is busy
    I2C1CON1bits.P = 1;    // Generate Stop condition
    while (I2C1CON1bits.P);   // Wait for Stop condition to complete
}

// Wait for I2C operation to complete
void i2c_wait(void) {
    while (I2C1STAT0bits.BFRE);  // Wait for bus to be free
}