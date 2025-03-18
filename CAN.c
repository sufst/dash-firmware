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
    C1CON = 0x00;          // Reset CAN module
    C1CONbits.REQOP = 0b100; // Configuration mode
    while (C1CONbits.OPMODE != 0b100); // Wait for mode

    // Configure bit rate (e.g., 500 kbps with 64 MHz clock)
    // Fcan = Fosc / (2 * (BRP + 1) * (SJW + 1) * (SEG1 + SEG2 + 3))
    // BRP = 7, SJW = 0, SEG1 = 6, SEG2 = 7 for 500 kbps
    C1CFG1 = 0x83;         // BRP = 7, SJW = 0
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
    if (C1FIFOCON1bits.FNRM == 0) { // Check if FIFO has messages
        return; // No messages to process
    }

    // Read the received message
    CAN_Message_t msg;
    msg.id = (C1RXF1SID & 0x7FF);    // 11-bit ID
    msg.dlc = C1RXF1DLC & 0x0F;      // Data length
    for (uint8_t i = 0; i < msg.dlc; i++) {
        msg.data[i] = C1RXF1B[i];    // Read data bytes
    }

    // Parse specific messages
    switch (msg.id) {
        case 0x100: // Speed message
            if (msg.dlc >= 2) {
                dashboard_data.speed_kph = (msg.data[0] << 8) | msg.data[1]; // 16-bit speed
            }
            break;
        case 0x101: // RPM message
            if (msg.dlc >= 2) {
                dashboard_data.rpm = (msg.data[0] << 8) | msg.data[1]; // 16-bit RPM
            }
            break;
        // Add cases for other messages (e.g., temperature)
        default:
            break;
    }

    // Clear FIFO flag (manual polling, no interrupt)
    C1FIFOCON1bits.UINC = 1; // Increment FIFO pointer
}

void can_get_dashboard_data(DashboardData_t *data) {
    *data = dashboard_data; // Copy data to caller
}