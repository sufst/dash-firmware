#include <stdio.h>
#include <stdint.h>
#include "CAN_handler.h"
#include "mcc_generated_files/mcc.h"

//Static variable for dashboard data
static volatile DashboardData_t dashboard_data = {0};

CAN_MSG_OBJ rx_msg;  // RX Message Object
uint8_t nrMsg = 0; // number of messages available



void ProcessCANMessage(void) {
    
    //Commenting this out since this has turned into an ISR
    //printf("\nMessage Received\n");
    
    uint8_t rx_buff[DLC_8]; // RX data bytes buffer
    
    if (true == CAN1_Receive(&rx_msg)) {
        switch (rx_msg.msgId) // BMS Current, Voltage and SOC
        {
            case 0x201:

                for (uint8_t i = 0; i < rx_msg.field.dlc; i++) {
                    rx_buff[i] = rx_msg.data[i];
                }

                if (rx_msg.field.dlc >= 8) {
                    // Big-endian, Motorola format
                    dashboard_data.BMS_pack_current = (rx_buff[0] << 8) | rx_buff[1]; // Startbit 8, length 16
                    dashboard_data.BMS_inst_volt = (rx_buff[2] << 8) | rx_buff[3]; // Startbit 24, length 16
                    dashboard_data.bms_soc = (rx_buff[4]); // Startbit 32, length 8

                    //Commenting this out since this has turned into an ISR
                    //printf("BMS Current: %d, BMS Voltage: %d, BMS SOC: %d\n", dashboard_data.BMS_pack_current / 10, dashboard_data.BMS_inst_volt / 10, dashboard_data.bms_soc);
                }
                break;

            case 0x202: // BMS Temperatures

                for (uint8_t i = 0; i < rx_msg.field.dlc; i++) {
                    rx_buff[i] = rx_msg.data[i];
                }

                if (rx_msg.field.dlc >= 8) {
                    // Big-endian, Motorola format
                    dashboard_data.bms_high_temp = rx_buff[0];
                    dashboard_data.bms_low_temp = rx_buff[1];
                    dashboard_data.bms_avg_temp = rx_buff[2];
                    dashboard_data.bms_internal_temp = rx_buff[3];

                    //Commenting this out since this has turned into an ISR
                    //printf("BMS Temps - High: %d, Low %d, Average: %d, Internal: %d\n", dashboard_data.bms_high_temp, dashboard_data.bms_low_temp, dashboard_data.bms_avg_temp, dashboard_data.bms_internal_temp);

                }
                break;

            case 0x101: // VCU control State

                for (uint8_t i = 0; i < rx_msg.field.dlc; i++) {
                    rx_buff[i] = rx_msg.data[i];
                }

                if (rx_msg.field.dlc >= 4) {
                    //little-endian, Intel format
                    dashboard_data.VCU_CTRL_State = rx_buff[2]; //Startbit 16, length 8

                    //Commenting this out since this has turned into an ISR
                    //printf("VCU State: %d\n", dashboard_data.VCU_CTRL_State);
                }
                break;

            default:
                //Commenting this out since this has turned into an ISR
                //printf("Ignore unknown msgID\n")
        }
    }
}


void GetDashboardData(DashboardData_t *data) {
    C1FIFOCON1Lbits.TFNRFNIE = 0;  // Disable CAN FIFO1 RX interrupt
    *data = dashboard_data;  // Copy safely
    C1FIFOCON1Lbits.TFNRFNIE = 1;  // Re-enable interrupt
}