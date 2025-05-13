#include "power.h"
#include "config.h"
#include <string.h> // for memset


static uint32_t power_buffer[POWER_BUFFER_SIZE];
static uint8_t buffer_index = 0;
static uint8_t buffer_filled = 0;

static uint32_t instant_power_mw = 0;

// Update called on each CAN message
void power_update(uint16_t voltage_dv, uint16_t current_da) {
    instant_power_mw = (uint32_t)voltage_dv * (uint32_t)current_da *10;

    // Save into circular buffer
    power_buffer[buffer_index] = instant_power_mw;
    buffer_index = (buffer_index + 1) % POWER_BUFFER_SIZE;

    if (buffer_filled < POWER_BUFFER_SIZE)
        buffer_filled++;
}

uint32_t power_get_instant_kw(void) {
    return instant_power_mw / 1000000; // Convert to kW
}

uint32_t power_get_average_kw(void) {
    if (buffer_filled == 0)
        return 0.0f;

    uint64_t sum = 0;
    for (uint8_t i = 0; i < buffer_filled; ++i)
        sum += power_buffer[i];

    return (uint32_t)(sum / buffer_filled / 1000000); // Convert to kW
}

// Generate LCD bar for current power
const char* power_get_bar(void) {
    static char bar[POWER_BAR_WIDTH + 1];
    memset(bar, ' ', POWER_BAR_WIDTH);
    bar[POWER_BAR_WIDTH] = '\0';

    // Define a max power level for full bar 54kW 6kW per bar
    const uint32_t max_power_mw = 54000 * 1000;
    uint8_t filled = (instant_power_mw * POWER_BAR_WIDTH) / max_power_mw;

    if (filled > POWER_BAR_WIDTH)
        filled = POWER_BAR_WIDTH;

    for (uint8_t i = 0; i < filled; ++i)
        bar[i] = 0xFF; // Full block character (may vary by LCD font)

    return bar;
}