#include "can.h"
#include "i2c.h"  // For eventual I2C transmission

#include <pic18f47q84.h>

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


    // Configure bit rate (e.g., 500 kbps with 64 MHz clock)
    // Fcan = Fosc / (2 * (BRP + 1) * (SJW + 1) * (SEG1 + SEG2 + 3))
    // BRP = 7, SJW = 0, SEG1 = 6, SEG2 = 7 for 500 kbps
    C1NBTCFGL = 0x83;         // BRP = 7, SJW = 0
    C1CFG2 = 0xA4;         // SEG1 = 6, SEG2 = 7, SAM = 1

    // Configure FIFO for receive
    C1FIFOCON1 = 0x82;     // Enable FIFO1, Receive mode
    C1FIFOCON1bits.TXEN = 0;
    C1FIFOCON1bits.FSIZE = 0x01; // Size = 2 messages
    C1FIFOCON1bits.FNRB = 0x00;  // FIFO number

    // Set filter for specific message IDs (e.g., 0x100 for speed, 0x101 for RPM)
    C1FLTCON0 = 0x03;      // Enable 2 filters
    C1RXM0SID = 0x7FF;     // Mask: Accept any 11-bit ID (adjust for your needs)
    C1FLT0SID = 0x100;     // Filter for ID 0x100 (speed)
    C1FLT1SID = 0x101;     // Filter for ID 0x101 (RPM)

    // Exit Configuration mode, enter Normal mode
    C1CONbits.REQOP = 0b000; // Normal mode
    while (C1CONbits.OPMODE != 0b000); // Wait for mode
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
        case 0x100:
            if (msg.dlc >= 2) {
                dashboard_data.speed_kph = (msg.data[0] << 8) | msg.data[1];
            }
            break;
        case 0x101:
            if (msg.dlc >= 2) {
                dashboard_data.rpm = (msg.data[0] << 8) | msg.data[1];
            }
            break;
    }

    C1FIFOCON1Hbits.UINC = 1; // Increment FIFO tail to process next message
}

void can_get_dashboard_data(DashboardData_t *data) {
    *data = dashboard_data; // Copy data to caller
}