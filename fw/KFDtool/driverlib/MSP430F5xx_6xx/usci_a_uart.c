/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// usci_a_uart.c - Driver for the usci_a_uart Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup usci_a_uart_api usci_a_uart
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_USCI_Ax__
#include "usci_a_uart.h"

#include <assert.h>

bool USCI_A_UART_init(uint16_t baseAddress, USCI_A_UART_initParam *param)
{
    bool retVal = STATUS_SUCCESS;

    //Disable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCSWRST;

    //Clock source select
    HWREG8(baseAddress + OFS_UCAxCTL1) &= ~UCSSEL_3;
    HWREG8(baseAddress + OFS_UCAxCTL1) |= param->selectClockSource;

    //MSB, LSB select
    HWREG8(baseAddress + OFS_UCAxCTL0) &= ~UCMSB;
    HWREG8(baseAddress + OFS_UCAxCTL0) |= param->msborLsbFirst;


    //UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    HWREG8(baseAddress + OFS_UCAxCTL0) &= ~UCSPB;
    HWREG8(baseAddress + OFS_UCAxCTL0) |= param->numberofStopBits;


    //Parity
    switch (param->parity){
        case USCI_A_UART_NO_PARITY:
            //No Parity
            HWREG8(baseAddress + OFS_UCAxCTL0) &= ~UCPEN;
            break;
        case USCI_A_UART_ODD_PARITY:
            //Odd Parity
            HWREG8(baseAddress + OFS_UCAxCTL0) |= UCPEN;
            HWREG8(baseAddress + OFS_UCAxCTL0) &= ~UCPAR;
            break;
        case USCI_A_UART_EVEN_PARITY:
            //Even Parity
            HWREG8(baseAddress + OFS_UCAxCTL0) |= UCPEN;
            HWREG8(baseAddress + OFS_UCAxCTL0) |= UCPAR;
            break;
    }

    //Modulation Control Registers
    HWREG16(baseAddress + OFS_UCAxBRW ) = param->clockPrescalar;
    HWREG8(baseAddress + OFS_UCAxMCTL) = ((param->firstModReg<<4) +
                                        (param->secondModReg <<1) +
                                        param->overSampling );

    //Asynchronous mode & 8 bit character select & clear mode
    HWREG8(baseAddress + OFS_UCAxCTL0) &=  ~(UCSYNC +
                                             UC7BIT +
                                             UCMODE_3
                                             );

    //Configure  UART mode.
    HWREG8(baseAddress + OFS_UCAxCTL0) |= param->uartMode ;

    //Reset UCRXIE, UCBRKIE, UCDORM, UCTXADDR, UCTXBRK
    HWREG8(baseAddress + OFS_UCAxCTL1)  &= ~(UCRXEIE + UCBRKIE + UCDORM +
                                             UCTXADDR + UCTXBRK
                                             );
    return (retVal) ;
}
void USCI_A_UART_transmitData ( uint16_t baseAddress,
    uint8_t transmitData
    )
{
    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCAxIE) & UCTXIE)){
        //Poll for transmit interrupt flag
        while (!(HWREG8(baseAddress + OFS_UCAxIFG) & UCTXIFG));
    }

    HWREG8(baseAddress + OFS_UCAxTXBUF) = transmitData;
}

uint8_t USCI_A_UART_receiveData (uint16_t baseAddress)
{
    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCAxIE) & UCRXIE)){
        //Poll for receive interrupt flag
        while (!(HWREG8(baseAddress + OFS_UCAxIFG) & UCRXIFG));
    }

    return ( HWREG8(baseAddress + OFS_UCAxRXBUF)) ;
}

void USCI_A_UART_enableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    uint8_t locMask;

    locMask = (mask & (USCI_A_UART_RECEIVE_INTERRUPT
        | USCI_A_UART_TRANSMIT_INTERRUPT));
    HWREG8(baseAddress + OFS_UCAxIE) |= locMask;

    locMask = (mask & (USCI_A_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT
        | USCI_A_UART_BREAKCHAR_INTERRUPT));
    HWREG8(baseAddress + OFS_UCAxCTL1) |= locMask;
}

void USCI_A_UART_disableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    uint8_t locMask;

    if(locMask = (mask & (USCI_A_UART_RECEIVE_INTERRUPT
        | USCI_A_UART_TRANSMIT_INTERRUPT))) {
        HWREG8(baseAddress + OFS_UCAxIE) &= ~locMask;
    }

    if(locMask = (mask & (USCI_A_UART_RECEIVE_ERRONEOUSCHAR_INTERRUPT
        | USCI_A_UART_BREAKCHAR_INTERRUPT))) {
        HWREG8(baseAddress + OFS_UCAxCTL1) &= ~locMask;
    }
}

uint8_t USCI_A_UART_getInterruptStatus (uint16_t baseAddress,
    uint8_t mask)
{
    return ( HWREG8(baseAddress + OFS_UCAxIFG) & mask );
}

void USCI_A_UART_clearInterrupt (uint16_t baseAddress, uint8_t mask)
{
    //Clear the UART interrupt source.
    HWREG8(baseAddress + OFS_UCAxIFG) &= ~(mask);
}

void USCI_A_UART_enable (uint16_t baseAddress)
{
    //Reset the UCSWRST bit to enable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) &= ~(UCSWRST);
}

void USCI_A_UART_disable (uint16_t baseAddress)
{
    //Set the UCSWRST bit to disable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCSWRST;
}

uint8_t USCI_A_UART_queryStatusFlags (uint16_t baseAddress,
    uint8_t mask)
{
    return ( HWREG8(baseAddress + OFS_UCAxSTAT) & mask );
}

void USCI_A_UART_setDormant (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCDORM;
}

void USCI_A_UART_resetDormant (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_UCAxCTL1) &= ~UCDORM;
}

void USCI_A_UART_transmitAddress (uint16_t baseAddress,
    uint8_t transmitAddress)
{
    //Set UCTXADDR bit
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCTXADDR;

    //Place next byte to be sent into the transmit buffer
    HWREG8(baseAddress + OFS_UCAxTXBUF) = transmitAddress;
}

void USCI_A_UART_transmitBreak (uint16_t baseAddress)
{
    //Set UCTXADDR bit
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCTXBRK;

    //If current mode is automatic baud-rate detection
    if (USCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE ==
        (HWREG8(baseAddress + OFS_UCAxCTL0) &
         USCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE)){
        HWREG8(baseAddress + OFS_UCAxTXBUF) = USCI_A_UART_AUTOMATICBAUDRATE_SYNC;
    } else   {
        HWREG8(baseAddress + OFS_UCAxTXBUF) = DEFAULT_SYNC;
    }

    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCAxIE) & UCTXIE)){
        //Poll for transmit interrupt flag
        while (!(HWREG8(baseAddress + OFS_UCAxIFG) & UCTXIFG));
    }
}

uint32_t USCI_A_UART_getReceiveBufferAddressForDMA (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCAxRXBUF );
}

uint32_t USCI_A_UART_getTransmitBufferAddressForDMA (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCAxTXBUF );
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for usci_a_uart_api
//! @}
//
//*****************************************************************************
