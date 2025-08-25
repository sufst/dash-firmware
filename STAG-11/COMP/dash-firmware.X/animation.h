/* 
 * File:   animation.h
 * Author: Adam
 *
 * Created on 15 June 2025, 20:06
 */

#ifndef ANIMATION_H
#define	ANIMATION_H

#include <stdint.h>

// Function prototypes
void timer0_init(void);
void animation_start(void);
void animation_update(void);
void animation_stop(void);
void animation_isr(void);

extern volatile uint8_t animation_active;


#endif	/* ANIMATION_H */

