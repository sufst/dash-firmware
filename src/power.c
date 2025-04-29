#include "LCD.h"
#include "Seg.h"
#include "CAN.h"
#include "config.h"
#include "i2c.h"
#include "power.h"

static float instant_power = 0.0f;
static float total_power = 0.0f;
static unsigned int sample_count = 0;

void power_update(const DashboardData_t *data) {
    instant_power = data->pack_current * data->pack_voltage;

    // Update sum and count (for average)
    total_power += instant_power;
    sample_count++;
}

float power_get_instant(void) {
    return instant_power;
}

float power_get_average(void) {
    if (sample_count == 0) return 0.0f;
    return total_power / sample_count;
}

void power_reset_average(void) {
    total_power = 0.0f;
    sample_count = 0;
}