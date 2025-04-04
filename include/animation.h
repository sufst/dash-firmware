#ifndef ANIMATION_H
#define ANIMATION_H


// Function prototypes
void timer0_init(void);
void animation_start(void);
void animation_update(void);
void animation_stop(void);
uint8_t animation_is_active(void);

#endif // ANIMATION_H