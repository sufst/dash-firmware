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
    uint8_t bms_soc;       // Speed in km/h
    uint8_t bms_temp;      // Engine RPM
    uint8_t placeholder;   // Temperature in °C
    uint8_t R2D;           // Ready to drive VCU signal - 1 or 0
    uint8_t TSON;  
    float pack_current;     // in Amps
    float pack_voltage;     // in Volts
    float instant_power;    // in Watts
    // Add other parameters as needed
} DashboardData_t;



// Function Prototypes
void can_init(void);
void can_process_messages(void);
void can_get_dashboard_data(DashboardData_t *data);

#endif