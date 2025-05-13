#ifndef POWER_H
#define POWER_H

#define POWER_BUFFER_SIZE 60  //seconds
#define POWER_BAR_WIDTH 9

void power_update(uint16_t voltage_dv, uint16_t current_da);
uint32_t power_get_instant_kw(void);
uint32_t power_get_average_kw(void);

#endif