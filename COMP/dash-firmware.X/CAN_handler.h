/* 
 * File:   CAN_handler.h
 * Author: Adam
 *
 * Created on June 15, 2025, 12:53 PM
 */

#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include <stdbool.h>

// Structure for dashboard data
typedef struct {
    uint16_t BMS_pack_current;    // BMS pack current (16-bit, Motorola)
    uint16_t BMS_inst_volt;       // BMS instantaneous voltage (16-bit, Motorola)
    uint8_t bms_soc;              // BMS state of charge (8-bit)
    uint8_t bms_high_temp;        // BMS high temperature (8-bit)
    uint8_t bms_low_temp;         // BMS low temperature (8-bit)
    uint8_t bms_avg_temp;         // BMS average temperature (8-bit)
    uint8_t bms_internal_temp;    // BMS internal temperature (8-bit)
    uint8_t VCU_CTRL_State;       // VCU control state (8-bit, Intel)
} DashboardData_t;

void ProcessCANMessage(void);
void GetDashboardData(DashboardData_t *data);

#endif // CAN_HANDLER_H

