/**
  CAN1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    can1.c

  @Summary
    This is the generated driver implementation file for the CAN1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for CAN1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  1.1.1
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above or later
        MPLAB             :  MPLAB X 6.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include "can1.h"

#define RX_FIFO_MSG_DATA                (8U)
#define NUM_OF_RX_FIFO                  (1U)

#define SID_LOW_WIDTH                   (8U)
#define SID_HIGH_MASK                   (0x07U)
#define EID_LOW_WIDTH                   (5U)
#define EID_LOW_POSN                    (3U)
#define EID_LOW_MASK                    (0xF8U)
#define EID_MID_WIDTH                   (8U)
#define EID_HIGH_WIDTH                  (5U)
#define EID_HIGH_MASK                   (0x1FU)
#define IDE_POSN                        (4U)
#define RTR_POSN                        (5U)
#define BRS_POSN                        (6U)
#define FDF_POSN                        (7U)

#define DLCToPayloadBytes(x)            (DLC_BYTES[(x)])
#define PLSIZEToPayloadBytes(x)         (DLCToPayloadBytes(8u + (x)))

struct CAN_FIFOREG
{
    uint8_t CONL;
    uint8_t CONH;
    uint8_t CONU;
    uint8_t CONT;
    uint8_t STAL;
    uint8_t STAH;
    uint8_t STAU;
    uint8_t STAT;
    uint32_t UA;
};

typedef enum
{
    CAN_RX_MSG_NOT_AVAILABLE = 0U,
    CAN_RX_MSG_AVAILABLE = 1U,
    CAN_RX_MSG_OVERFLOW = 8U
} CAN_RX_FIFO_STATUS;


struct CAN1_RX_FIFO
{
    CAN1_RX_FIFO_CHANNELS channel;
    volatile uint8_t fifoHead;
};

//CAN RX FIFO Message object data field 
static volatile uint8_t rxMsgData[RX_FIFO_MSG_DATA];

static struct CAN1_RX_FIFO rxFifos[] = 
{
    {FIFO1, 0u}
};

static volatile struct CAN_FIFOREG * const FIFO = (struct CAN_FIFOREG *)&C1TXQCONL;
static const uint8_t DLC_BYTES[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U};

static void (*CAN1_FIFO1NotEmptyHandler)(void);

static void DefaultFIFO1NotEmptyHandler(void)
{
}

void CAN1_RX_FIFO_ResetInfo(void)
{
    uint8_t index;

    for (index = 0; index < NUM_OF_RX_FIFO; index++)
    {
        rxFifos[index].fifoHead = 0;
    }
}

static void CAN1_RX_FIFO_Configuration(void)
{
    // TXEN disabled; RTREN disabled; RXTSEN disabled; TXATIE disabled; RXOVIE disabled; TFERFFIE disabled; TFHRFHIE disabled; TFNRFNIE enabled; 
    C1FIFOCON1L = 0x01;
    
    // FRESET enabled; TXREQ disabled; UINC disabled; 
    C1FIFOCON1H = 0x04;
    
    // TXAT Unlimited number of retransmission attempts; TXPRI 1; 
    C1FIFOCON1U = 0x60;
    
    // PLSIZE 8; FSIZE 6; 
    C1FIFOCON1T = 0x05;
    
    CAN1_SetFIFO1NotEmptyHandler(DefaultFIFO1NotEmptyHandler);
    
    C1INTUbits.RXIE = 1;
    
    PIR4bits.CANRXIF = 0;
    PIE4bits.CANRXIE = 1;
}

static void CAN1_RX_FIFO_FilterMaskConfiguration(void)
{
    // FLTEN0 enabled; F0BP FIFO 1; 
    C1FLTOBJ0L = 0x01;
    C1FLTOBJ0H = 0x02;
    C1FLTOBJ0U = 0x00;
    C1FLTOBJ0T = 0x00;
    C1MASK0L = 0xFC;
    C1MASK0H = 0x04;
    C1MASK0U = 0x00;
    C1MASK0T = 0x40;
    C1FLTCON0L = 0x81; 
    
}


static void CAN1_BitRateConfiguration(void)
{
    // SJW 7; 
    C1NBTCFGL = 0x07;
    
    // TSEG2 7; 
    C1NBTCFGH = 0x07;
    
    // TSEG1 30; 
    C1NBTCFGU = 0x1E;
    
    // BRP 0; 
    C1NBTCFGT = 0x00;
    
}


void CAN1_Initialize(void)
{
    /* Enable the CAN module */
    C1CONHbits.ON = 1;
    
    if (CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE))
    {
        /* Initialize the C1FIFOBA with the start address of the CAN FIFO message object area. */
        C1FIFOBA = 0x3800;
        
        // CLKSEL0 disabled; PXEDIS enabled; ISOCRCEN enabled; DNCNT 0; 
        C1CONL = 0x60;

        // ON enabled; FRZ disabled; SIDL disabled; BRSDIS enabled; WFT T11 Filter; WAKFIL enabled; 
        C1CONH = 0x97;

        // TXQEN disabled; STEF disabled; SERR2LOM disabled; ESIGM disabled; RTXAT disabled; 
        C1CONU = 0x00;

        CAN1_BitRateConfiguration();
        CAN1_RX_FIFO_Configuration();
        CAN1_RX_FIFO_FilterMaskConfiguration();
        CAN1_RX_FIFO_ResetInfo();
        CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);    
    }
}

CAN_OP_MODE_STATUS CAN1_OperationModeSet(const CAN_OP_MODES requestMode)
{
    CAN_OP_MODE_STATUS status = CAN_OP_MODE_REQUEST_SUCCESS;
    CAN_OP_MODES opMode = CAN1_OperationModeGet();

    if (CAN_CONFIGURATION_MODE == opMode
            || CAN_DISABLE_MODE == requestMode
            || CAN_CONFIGURATION_MODE == requestMode)
    {
        C1CONTbits.REQOP = requestMode;
        
        while (C1CONUbits.OPMOD != requestMode)
        {
            //This condition is avoiding the system error case endless loop
            if (1 == C1INTHbits.SERRIF)
            {
                status = CAN_OP_MODE_SYS_ERROR_OCCURED;
                break;
            }
        }
    }
    else
    {
        status = CAN_OP_MODE_REQUEST_FAIL;
    }

    return status;
}

CAN_OP_MODES CAN1_OperationModeGet(void)
{
    return C1CONUbits.OPMOD;
}

static uint8_t GetRxFifoDepth(uint8_t validChannel)
{
    return 1U + (FIFO[validChannel].CONT & _C1FIFOCON1T_FSIZE_MASK);
}

static CAN_RX_FIFO_STATUS GetRxFifoStatus(uint8_t validChannel)
{
    return FIFO[validChannel].STAL & (CAN_RX_MSG_AVAILABLE | CAN_RX_MSG_OVERFLOW);
}

static void ReadMessageFromFifo(uint8_t *rxFifoObj, CAN_MSG_OBJ *rxCanMsg)
{
    uint32_t msgId;
    uint8_t status = rxFifoObj[4];
    const uint8_t payloadOffsetBytes =
              4U    // ID
            + 1U    // FDF, BRS, RTR, ...
            + 1U    // FILHIT, ...
            + 2U;   // Unimplemented
    
    rxCanMsg->field.dlc = status;
    rxCanMsg->field.idType = (status & (1UL << IDE_POSN)) ? CAN_FRAME_EXT : CAN_FRAME_STD;
    rxCanMsg->field.frameType = (status & (1UL << RTR_POSN)) ? CAN_FRAME_RTR : CAN_FRAME_DATA;
    rxCanMsg->field.brs = (status & (1UL << BRS_POSN)) ? CAN_BRS_MODE : CAN_NON_BRS_MODE;
    rxCanMsg->field.formatType = (status & (1UL << FDF_POSN)) ? CAN_FRAME_EXT : CAN_FRAME_STD;
       
    msgId = rxFifoObj[1] & SID_HIGH_MASK;
    msgId <<= SID_LOW_WIDTH;
    msgId |= rxFifoObj[0];
    if (CAN_FRAME_EXT == rxCanMsg->field.idType)
    {
        msgId <<= EID_HIGH_WIDTH;
        msgId |= (rxFifoObj[3] & EID_HIGH_MASK);
        msgId <<= EID_MID_WIDTH;
        msgId |= rxFifoObj[2];
        msgId <<= EID_LOW_WIDTH;
        msgId |= (rxFifoObj[1] & EID_LOW_MASK) >> EID_LOW_POSN;
    }
    rxCanMsg->msgId = msgId;
    
    memcpy(rxMsgData, rxFifoObj + payloadOffsetBytes, DLCToPayloadBytes(rxCanMsg->field.dlc));
    rxCanMsg->data = rxMsgData;
}

static bool Receive(uint8_t index, CAN1_RX_FIFO_CHANNELS channel, CAN_MSG_OBJ *rxCanMsg)
{
    bool status = false;
    CAN_RX_FIFO_STATUS rxMsgStatus = GetRxFifoStatus(channel);

    if (CAN_RX_MSG_AVAILABLE == (rxMsgStatus & CAN_RX_MSG_AVAILABLE))
    {
        uint8_t *rxFifoObj = (uint8_t *) FIFO[channel].UA;

        if (rxFifoObj != NULL)
        {
            ReadMessageFromFifo(rxFifoObj, rxCanMsg);
            FIFO[channel].CONH |= _C1FIFOCON1H_UINC_MASK;

            rxFifos[index].fifoHead += 1;
            if (rxFifos[index].fifoHead >= GetRxFifoDepth(channel))
            {
                rxFifos[index].fifoHead = 0;
            }

            if (CAN_RX_MSG_OVERFLOW == (rxMsgStatus & CAN_RX_MSG_OVERFLOW))
            {
                FIFO[channel].STAL &= ~_C1FIFOSTA1L_RXOVIF_MASK;
            }

            status = true;
        }
    }
    
    return status;
}

bool CAN1_Receive(CAN_MSG_OBJ *rxCanMsg)
{
    uint8_t index;
    bool status = false;
    
    for (index = 0; index < NUM_OF_RX_FIFO; index++)
    {
        status = Receive(index, rxFifos[index].channel, rxCanMsg);
        
        if (status)
        {
            break;
        }
    }
    
    return status;
}

bool CAN1_ReceiveFrom(const CAN1_RX_FIFO_CHANNELS channel, CAN_MSG_OBJ *rxCanMsg)
{
    uint8_t index;
    bool status = false;
    
    for (index = 0; index < NUM_OF_RX_FIFO; index++)
    {
        if (channel == rxFifos[index].channel)
        {
            status = Receive(index, channel, rxCanMsg);
            break;
        }
    }
    
    return status;
}

uint8_t CAN1_ReceivedMessageCountGet(void)
{
    uint8_t index, totalMsgObj = 0;

    for (index = 0; index < NUM_OF_RX_FIFO; index++)
    {
        CAN1_RX_FIFO_CHANNELS channel = rxFifos[index].channel;
        CAN_RX_FIFO_STATUS rxMsgStatus = GetRxFifoStatus(channel);

        if (CAN_RX_MSG_AVAILABLE == (rxMsgStatus & CAN_RX_MSG_AVAILABLE))
        {
            uint8_t numOfMsg, fifoDepth = GetRxFifoDepth(channel);
            
            if (CAN_RX_MSG_OVERFLOW == (rxMsgStatus & CAN_RX_MSG_OVERFLOW))
            {
                numOfMsg = fifoDepth;
            }
            else
            {
                uint8_t fifoTail = FIFO[channel].STAH & _C1FIFOSTA1H_FIFOCI_MASK;
                uint8_t fifoHead = rxFifos[index].fifoHead;

                if (fifoTail < fifoHead)
                {
                    numOfMsg = ((fifoTail + fifoDepth) - fifoHead); // wrap
                }
                else if (fifoTail > fifoHead)
                {
                    numOfMsg = fifoTail - fifoHead;
                }
                else
                {
                    numOfMsg = fifoDepth;
                }
            }

            totalMsgObj += numOfMsg;
        }
    }

    return totalMsgObj;
}


bool CAN1_IsBusOff(void)
{
    return C1TRECUbits.TXBO;
}

bool CAN1_IsRxErrorPassive(void)
{
    return C1TRECUbits.RXBP;
}

bool CAN1_IsRxErrorWarning(void)
{
    return C1TRECUbits.RXWARN;
}

bool CAN1_IsRxErrorActive(void)
{
    return !CAN1_IsRxErrorPassive();
}

bool CAN1_IsTxErrorPassive(void)
{
    return C1TRECUbits.TXBP;
}

bool CAN1_IsTxErrorWarning(void)
{
    return C1TRECUbits.TXWARN;
}

bool CAN1_IsTxErrorActive(void)
{
    return !CAN1_IsTxErrorPassive();
}

void CAN1_Sleep(void)
{
    C1INTHbits.WAKIF = 0;
    C1INTTbits.WAKIE = 1;
    
    CAN1_OperationModeSet(CAN_DISABLE_MODE);
}


void CAN1_SetFIFO1NotEmptyHandler(void (*handler)(void))
{
    CAN1_FIFO1NotEmptyHandler = handler;
}


void CAN1_RXI_ISR(void)
{
    if (1 == C1FIFOSTA1Lbits.TFNRFNIF)
    {
        CAN1_FIFO1NotEmptyHandler();
        // flag readonly
    }
    
}

