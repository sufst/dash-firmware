#ifndef CAN_H
#define CAN_H


// Define the message object structure for RAM
typedef struct {
    uint32_t id_word;    // Word 0: ID field (SID[10:0] in bits 31:21)
    uint32_t ctrl_word;  // Word 1: Control field (DLC[3:0] in bits 19:16)
    uint8_t data[8];     // Words 2-3: Data bytes (8 bytes for CAN 2.0)
} CAN_RxMessage_t;


// Simplified CAN Message Structure
typedef struct {
    uint32_t id;        // 11-bit or 29-bit identifier
    uint8_t dlc;        // Data Length Code (0-8)
    uint8_t data[8];    // Up to 8 data bytes
} CAN_Message_t;

// Parsed Data Structure (for dashboard)
typedef struct {
    //msg_id 0x201
    uint16_t BMS_pack_current; //factor 0.1
    uint16_t BMS_inst_volt;    //factor 0.1
    uint8_t bms_soc;           //factor 0.5
    //msg_id 0x202
    uint8_t bms_high_temp;     
    uint8_t bms_low_temp;
    uint8_t bms_avg_temp;
    uint8_t bms_internal_temp;
    //msg_id 0x101
    uint8_t VCU_CTRL_State;
    
    uint8_t Power;
    uint8_t power_saving_mode;
    uint8_t PDM_battery_voltage;
    // Add other parameters as needed
} DashboardData_t;

// Function Prototypes
void can_init(void);
void can_process_messages(void);
void can_get_dashboard_data(DashboardData_t *data);

#endif