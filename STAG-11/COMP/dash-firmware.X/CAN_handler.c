#include <stdio.h>
#include <stdint.h>
#include "CAN_handler.h"
#include "mcc_generated_files/mcc.h"

//Static variable for dashboard data
static DashboardData_t dashboard_data = {0};

CAN_MSG_OBJ rx_msg;  // RX Message Object
uint8_t rx_buff[DLC_8]; // RX data bytes buffer
uint8_t nrMsg = 0; // number of messages available




void ProcessCANMessages(void) {
    
    nrMsg = CAN1_ReceivedMessageCountGet();
    if (nrMsg > 0)
    {
        
        if(true == CAN1_Receive(&rx_msg))
        {
            printf("\nMessage Received\n");
                printf("TX_PD ID: 0x%03lX | DLC: %d | DATA:", rx_msg.msgId, rx_msg.field.dlc);
                    for (uint8_t i = 0; i < rx_msg.field.dlc; i++) {
                        printf(" %02X", rx_msg.data[i]);
                    }
                    printf("\n");
            switch (rx_msg.msgId)  // BMS Current, Voltage and SOC
            {
                case 0x201:
                
                    for(uint8_t i=0; i<rx_msg.field.dlc; i++)
                    {
                        rx_buff[i] = rx_msg.data[i];
                    }

                    if(rx_msg.field.dlc >= 8)
                    {
                        // Big-endian, Motorola format
                        dashboard_data.BMS_pack_current = (rx_buff[0] << 8) | rx_buff[1]; // Startbit 8, length 16
                        dashboard_data.BMS_inst_volt    = (rx_buff[2] << 8) | rx_buff[3]; // Startbit 24, length 16
                        dashboard_data.bms_soc          = (rx_buff[4]);                   // Startbit 32, length 8

                        printf("BMS Current: %d, BMS Voltage: %d, BMS SOC: %d\n", dashboard_data.BMS_pack_current / 10, dashboard_data.BMS_inst_volt / 10, dashboard_data.bms_soc);
                    }
                    break;
                    
                case 0x202: // BMS Temperatures
                    
                    for(uint8_t i=0; i<rx_msg.field.dlc; i++)
                    {
                        rx_buff[i] = rx_msg.data[i];
                    }

                    if(rx_msg.field.dlc >= 8)
                    {
                        // Big-endian, Motorola format
                        dashboard_data.bms_high_temp     = rx_buff[0];
                        dashboard_data.bms_low_temp      = rx_buff[1];
                        dashboard_data.bms_avg_temp      = rx_buff[2];
                        dashboard_data.bms_internal_temp = rx_buff[3];
                        
                        printf("BMS Temps - High: %d, Low %d, Average: %d, Internal: %d\n", dashboard_data.bms_high_temp, dashboard_data.bms_low_temp, dashboard_data.bms_avg_temp, dashboard_data.bms_internal_temp);
                                
                    }
                    break;
                    
                case 0x101: // VCU control State
                    
                    for(uint8_t i=0; i<rx_msg.field.dlc; i++)
                    {
                        rx_buff[i] = rx_msg.data[i];
                    }

                    if(rx_msg.field.dlc >= 4)
                    {
                        //little-endian, Intel format
                        dashboard_data.VCU_CTRL_State = rx_buff[2]; //Startbit 16, length 8
                        
                        printf("VCU State: %d\n", dashboard_data.VCU_CTRL_State);
                    }
                    break;
                   
                default: 
                    printf("Ignore unknown msgID\n");
            }
        }
    }
}

void GetDashboardData(DashboardData_t *data) {
    printf("GDD\n");
    *data = dashboard_data; // Copy data to caller
}