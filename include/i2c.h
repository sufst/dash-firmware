// i2c.h
#ifndef I2C_H
#define I2C_H

#include <stdint.h>

// Function prototype
void i2c_init(void);
void i2c_send_byte(uint8_t data);
void i2c_start(void);
void i2c_stop(void);
void i2c_wait(void);

#endif