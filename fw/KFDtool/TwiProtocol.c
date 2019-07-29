// KFDtool
// Copyright 2019 Daniel Dugger

#include "msp430.h"
#include "driverlib.h"
#include "hal.h"

#include "TwiProtocol.h"

#define BIT_TIME FCPU/4000
#define HALF_BIT_TIME BIT_TIME/2
#define SIG_TIME BIT_TIME*4

#define ENABLE_KFD_RX_INT GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3); GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN3);
#define DISABLE_KFD_RX_INT GPIO_disableInterrupt(GPIO_PORT_P1, GPIO_PIN3);

#define KFD_RX_IS_BUSY GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == GPIO_INPUT_PIN_LOW
#define KFD_RX_IS_IDLE GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == GPIO_INPUT_PIN_HIGH

#define SEN_RX_IS_CONN GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN5) == GPIO_INPUT_PIN_LOW
#define SEN_RX_IS_DISC GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN5) == GPIO_INPUT_PIN_HIGH

uint16_t busySending;
uint16_t timerType;
uint16_t rxBitsLeft;
uint16_t txNumLeft;
uint16_t bitCount;
uint16_t TXByte;
uint16_t RXByte;
uint16_t hasReceived;

uint8_t reverseByte(uint8_t b)
{
    const uint8_t table[] = {
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
        0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
        0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
        0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
        0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
        0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
        0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
        0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
        0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
        0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
        0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
        0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
        0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
        0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
        0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
        0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
        0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };

    return table[b];
}

/*
uint8_t reverseByte(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}
*/

uint16_t isEvenParity(uint16_t inByte)
{
    uint16_t numOnes = 0;

    uint16_t i;

    for (i = 0; i < 8; i++)
    {
        if (inByte & 0x01)
        {
            numOnes++;
        }

        inByte = inByte >> 1;
    }

    if (numOnes % 2)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void twiInit(void)
{
    halGpio1Low();
    ENABLE_KFD_RX_INT
    halSenTxConn();
}

uint8_t twiSelfTest(void)
{
    uint16_t error = 0; // the first error encountered should be returned
    uint8_t result = 0x00;

    // disable normal operation
    DISABLE_KFD_RX_INT
    halGpio1High();

    // test case 1 - KFD shorted to GNDISO
    // KFD_RX should be IDLE (5VISO)
    if (!error)
    {
        halKfdTxIdle();
        halSenTxDisc();
        halDelayMs(10);

        if (KFD_RX_IS_BUSY)
        {
            error = 1;
            result = 0x01;
        }

        halKfdTxIdle();
        halSenTxDisc();
    }

    // test case 2 - SEN shorted to GNDISO
    // SEN_RX should be DISC (5VISO)
    if (!error)
    {
        halKfdTxIdle();
        halSenTxDisc();
        halDelayMs(10);

        if (SEN_RX_IS_CONN)
        {
            error = 1;
            result = 0x02;
        }

        halKfdTxIdle();
        halSenTxDisc();
    }

    // test case 3 - KFD shorted to 5VISO
    // KFD_RX should be BUSY (GNDISO)
    if (!error)
    {
        halKfdTxBusy();
        halSenTxDisc();
        halDelayMs(10);

        if (KFD_RX_IS_IDLE)
        {
            error = 1;
            result = 0x03;
        }

        halKfdTxIdle();
        halSenTxDisc();
    }

    // test case 4 - SEN shorted to 5VISO
    // SEN_RX should be CONN (GNDISO)
    if (!error)
    {
        halKfdTxIdle();
        halSenTxConn();
        halDelayMs(10);

        if (SEN_RX_IS_DISC)
        {
            error = 1;
            result = 0x04;
        }

        halKfdTxIdle();
        halSenTxDisc();
    }

    // test cases 5 and 6 _should_ be identical in theory
    // but strange things happen in the real world...

    // test case 5 - KFD/SEN shorted
    // SEN_RX should be DISC (5VISO) while KFL_TX is BUSY (GNDISO)
    if (!error)
    {
        halKfdTxBusy();
        halSenTxDisc();
        halDelayMs(10);

        if (SEN_RX_IS_CONN)
        {
            error = 1;
            result = 0x05;
        }

        halKfdTxIdle();
        halSenTxDisc();
    }

    // test case 6 - SEN/KFD shorted
    // KFD_RX should be IDLE (5VISO) while SEN_TX is CONN (GNDISO)
    if (!error)
    {
        halKfdTxIdle();
        halSenTxConn();
        halDelayMs(10);

        if (KFD_RX_IS_BUSY)
        {
            error = 1;
            result = 0x06;
        }

        halKfdTxIdle();
        halSenTxDisc();
    }

    // return to normal operation
    halKfdTxIdle();
    halGpio1Low();
    ENABLE_KFD_RX_INT
    halSenTxConn();

    return result;
}

uint16_t twiReceiveByte(uint8_t *c)
{
    if (hasReceived == 0)
    {
        return 0;
    }

    *c = reverseByte(RXByte);
    hasReceived = 0;

    return 1;
}

void twiSendKeySig(void)
{
    DISABLE_KFD_RX_INT
    halGpio1High();

    busySending = 1;
    timerType = 1;
    txNumLeft = 105;

    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, SIG_TIME); // set timer period
    TA0CCTL0 |= CCIE; // enable timer interrupt
    TA0CTL |= TASSEL_2 + MC_1 + TACLR; // clock source SMCLK, count up mode, clear TAR

    while (busySending); // wait for completion

    halGpio1Low();
    ENABLE_KFD_RX_INT
}

void twiSendPhyByte(uint8_t byteToSend)
{
    DISABLE_KFD_RX_INT
    halGpio1High();

    busySending = 1;
    timerType = 2;
    txNumLeft = 4;

    TXByte = reverseByte(byteToSend);

    if (isEvenParity(byteToSend) == 0)
    {
        TXByte |= 0x100; // unset even parity bit
    }

    TXByte = TXByte << 1; // add start bit
    bitCount = 10;

    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, BIT_TIME); // set timer period
    TA0CCTL0 |= CCIE; // enable timer interrupt
    TA0CTL |= TASSEL_2 + MC_1 + TACLR; // clock source SMCLK, count up mode, clear TAR

    while (busySending); // wait for completion

    halGpio1Low();
    ENABLE_KFD_RX_INT
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    DISABLE_KFD_RX_INT
    halGpio1High();

    timerType = 0;
    rxBitsLeft = 10;
    RXByte = 0;

    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, HALF_BIT_TIME); // set timer period
    TA0CCTL0 |= CCIE; // enable timer interrupt
    TA0CTL |= TASSEL_2 + MC_1 + TACLR; // clock source SMCLK, count up mode, clear TAR
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    if (timerType == 0) // receive byte mode
    {
        Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, BIT_TIME); // set timer period

        if (rxBitsLeft == 0)
        {
            TA0CCTL0 &= ~CCIE; // disable timer interrupt
            Timer_A_stop(TIMER_A0_BASE); // stop timer
            halGpio1Low();
            ENABLE_KFD_RX_INT
            RXByte = RXByte >> 1; // remove start bit
            RXByte &= 0xFF; // remove parity bit
            // TODO check parity bit
            hasReceived = 1;
        }
        else
        {
            halGpio2Toggle();

            if (KFD_RX_IS_IDLE)
            {
                RXByte |= 0x400; // set the value in the RXByte
            }

            RXByte = RXByte >> 1; // shift the bits down
            rxBitsLeft--;
        }
    }
    else if (timerType == 1) // send key signature mode
    {
        if (txNumLeft == 0)
        {
            TA0CCTL0 &= ~CCIE; // disable timer interrupt
            Timer_A_stop(TIMER_A0_BASE); // stop timer
            halKfdTxIdle();
            busySending = 0;
        }
        else
        {
            if (txNumLeft > 5)
            {
                halKfdTxBusy();
            }
            else if (txNumLeft <= 5)
            {
                halKfdTxIdle();
            }

            txNumLeft--;
        }
    }
    else if (timerType == 2) // send byte mode
    {
        if (bitCount == 0)
        {
            halKfdTxBusy();

            if (txNumLeft == 0)
            {
                TA0CCTL0 &= ~CCIE; // disable timer interrupt
                Timer_A_stop(TIMER_A0_BASE); // stop timer
                halKfdTxIdle();
                busySending = 0;
            }
            else
            {
                txNumLeft--;
            }
        }
        else
        {
            if (TXByte & 0x01)
            {
                halKfdTxIdle();
            }
            else
            {
                halKfdTxBusy();
            }

            TXByte = TXByte >> 1;
            bitCount--;
        }
    }
}
