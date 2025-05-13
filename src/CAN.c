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
    C1NBTCFGUbits.TSEG1 = 30; //Nominal Time Segment 1 of 30
    C1NBTCFGHbits.TSEG2 = 7; //Nominal Time Segment 2 of 7
    C1NBTCFGLbits.SJW = 7; //match TSEG2 for stability



    // Enable FIFO1, Receive mode
    C1FIFOCON1Lbits.TXEN = 0; // Recieves message object
    C1FIFOCON1Tbits.FSIZE = 0x01; // Size = 2 messages
    C1FIFOCON1Hbits.FRESET = 0;    // Clear reset


    // Set filter for specific message IDs (e.g., 0x100 for speed, 0x101 for RPM)

    C1MASK0Lbits.MSID = 0x7FF;     // Mask: Accept any 11-bit ID (currently all 1s to accept any message)

    C1FLTOBJ0 = 0x201;     // Filter for ID 0x100 (speed)
    C1FLTCON0Lbits.FLTEN0 = 1;      // Enable filter 0
    C1FLTOBJ1 = 0x202;     // Filter for ID 0x101 (RPM)
    C1FLTCON0Hbits.FLTEN1 = 1;      // Enable filter 1

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
    uint32_t msg_addr = ((uint32_t)C1FIFOUA1T << 24) |
                        ((uint32_t)C1FIFOUA1U << 16) |
                        ((uint32_t)C1FIFOUA1H << 8) |
                        C1FIFOUA1L;

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
        case 0x201: // BMS Current, Voltage and SOC
            if (msg.dlc >= 8) {
                // Big-endian, motorola format extraction
                dashboard_data.BMS_pack_current  = (msg.data[0] << 8) | msg.data[1]; // Startbit 8, length 16
                dashboard_data.BMS_inst_volt     = (msg.data[2] << 8) | msg.data[3]; // Startbit 24, length 16
                dashboard_data.bms_soc           = (msg.data[4]);                    // Startbit 32, length 8
            }
            break;
        
        case 0x202: // BMS Temperatures
            if (msg.dlc >= 8) {
                // Big-endian, motorola format extraction
                dashboard_data.bms_high_temp     = (msg.data[0]);                    // Startbit 0, length 8
                dashboard_data.bms_low_temp      = (msg.data[1]);                    // Startbit 8, length 8
                dashboard_data.bms_avg_temp      = (msg.data[2]);                    // Startbit 16, length 8
                dashboard_data.bms_internal_temp = (msg.data[3]);                    // Startbit 24, length 8
            }
            break;
    }

    C1FIFOCON1Hbits.UINC = 1; // Increment FIFO tail to process next message
}

void can_get_dashboard_data(DashboardData_t *data) {
    *data = dashboard_data; // Copy data to caller
}