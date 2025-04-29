#include "config.h"
#include "CAN.h"

// Global variable to store parsed data
static DashboardData_t dashboard_data = {0};

void can_init(void) {
    // Configure CAN pins (e.g., RB0 = CANRX, RB1 = CANTX)
    TRISBbits.TRISB3 = 1;  // CANRX as input
    TRISBbits.TRISB4 = 0;  // CANTX as output
    ANSELBbits.ANSELB3 = 0;  // Digital mode
    ANSELBbits.ANSELB4 = 0;
    RB3PPS = 0x1A;         // RB3 -> CANRX
    CANRXPPS = 0x0B;       // RB3 -> CANRX input
    RB4PPS = 0x19;         // RB4 -> CANTX

    // Initialize CAN module
    C1CONTbits.REQOP = 0b100; // Configuration mode
    while (C1CONUbits.OPMOD != 0b100); // Wait for mode

    C1CONUbits.TXQEN = 0;   //These should be all the correct config bits for C1CON
    C1CONUbits.STEF = 0;
    C1CONHbits.ON = 1;
    C1CONHbits.WFT = 0;
    C1CONHbits.WAKFIL = 0;
    C1CONLbits.CLKSEL0 = 0;
    C1CONLbits.PXEDIS = 1;
    C1CONLbits.ISOCRCEN = 0;
    C1CONLbits.DNCNT = 0;


    //Configuring the bitrate of the CAN module
    C1NBTCFGTbits.BRP = 0; // Nominal Baud Rate Prescaler of 0
    C1NBTCFGUbits.TSEG1 = 5; //Nominal Time Segment 1 of 5
    C1NBTCFGHbits.TSEG2 = 2; //Nominal Time Segment 2 of 2
    C1NBTCFGLbits.SJW = 1; //Nominal Time Segment 2 of 2



    // Enable FIFO1, Receive mode
    C1FIFOCON1Lbits.TXEN = 0; // Recieves message object
    C1FIFOCON1Tbits.FSIZE = 0x01; // Size = 2 messages
    C1FIFOCON1Hbits.FRESET = 0;    // Clear reset


    // Set filter for specific message IDs (e.g., 0x100 for speed, 0x101 for RPM)
    C1FLTCON0Lbits.FLTEN0 = 1;      // Enable filter 0
    C1FLTCON0Hbits.FLTEN1 = 1;      // Enable filter 1
    //C1FLTCON0Ubits.FLTEN2 = 1;      // Enable filter 2
    //C1FLTCON0Tbits.FLTEN3 = 1;      // Enable filter 3
    C1MASK0Lbits.MSID = 0x7FF;     // Mask: Accept any 11-bit ID (currently all 1s to accept any message)
    C1FLTOBJ0 = 0x100;     // Filter for ID 0x100 (speed)
    C1FLTOBJ1 = 0x101;     // Filter for ID 0x101 (RPM)

    // Exit Configuration mode, enter Normal mode
    C1CONTbits.REQOP = 0b000; // Normal mode
    while (C1CONUbits.OPMOD != 0b000); // Wait for mode
}

void can_process_messages(void) {
    if (!C1FIFOSTA1Lbits.TFNRFNIF) { // Check if FIFO is not empty
        return; // No messages to process
    }

    CAN_Message_t msg;
    // Get the RAM address of the next message in FIFO1
    uint32_t msg_addr = ((uint32_t)C1FIFOUA1T << 24) | ((uint32_t)C1FIFOUA1U << 16) |
                        ((uint32_t)C1FIFOUA1H << 8) | C1FIFOUA1L;

    // Cast the address to a pointer to the message object
    CAN_RxMessage_t *rx_msg = (CAN_RxMessage_t *)msg_addr;

    // Extract SID (bits 31:21 of id_word)
    msg.id = (rx_msg->id_word >> 21) & 0x7FF;

    // Extract DLC (bits 19:16 of ctrl_word)
    msg.dlc = (rx_msg->ctrl_word >> 16) & 0x0F;

    // Copy data bytes
    for (uint8_t i = 0; i < msg.dlc; i++) {
        msg.data[i] = rx_msg->data[i];
    }

    // Process the message
    switch (msg.id) {
        case 0x201: // BMS Signals
            if (msg.dlc >= 5) {
            // Extract BMS_SOC (8 bits)
            dashboard_data.bms_soc = msg.data[0];

            // Extract BMS_Pack_Current (16 bits, Motorola)
            uint16_t raw_current = (msg.data[1] << 8) | msg.data[2];
            dashboard_data.pack_current = raw_current * 0.1f;

            // Extract BMS_Pack_Inst_Voltage (16 bits, Motorola)
            uint16_t raw_voltage = (msg.data[3] << 8) | msg.data[4];
            dashboard_data.pack_voltage = raw_voltage * 0.1f;

            // Calculate instantaneous power
            dashboard_data.instant_power = dashboard_data.pack_current * dashboard_data.pack_voltage;
    }
    break;
        case 0x202: // BMS Average Temperature ()
            if (msg.dlc >= 1) {
                dashboard_data.bms_temp = msg.data[0];
            }
            break;
    }

    C1FIFOCON1Hbits.UINC = 1; // Increment FIFO tail to process next message
}

void can_get_dashboard_data(DashboardData_t *data) {
    *data = dashboard_data; // Copy data to caller
}