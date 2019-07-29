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
// usci_a_spi.c - Driver for the usci_a_spi Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup usci_a_spi_api usci_a_spi
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_USCI_Ax__
#include "usci_a_spi.h"

#include <assert.h>

bool USCI_A_SPI_initMaster(uint16_t baseAddress, USCI_A_SPI_initMasterParam *param)
{
    //Disable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCSWRST;

    //Reset OFS_UCAxCTL0 values
    HWREG8(baseAddress + OFS_UCAxCTL0) &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB +
        UCMST + UCMODE_3 + UCSYNC);

    //Reset OFS_UCAxCTL1 values
    HWREG8(baseAddress + OFS_UCAxCTL1) &= ~(UCSSEL_3);

    //Select Clock
    HWREG8(baseAddress + OFS_UCAxCTL1) |= param->selectClockSource;

    HWREG16(baseAddress + OFS_UCAxBRW) =
        (uint16_t)(param->clockSourceFrequency / param->desiredSpiClock);

    /*
     * Configure as SPI master mode.
     * Clock phase select, polarity, msb
     * UCMST = Master mode
     * UCSYNC = Synchronous mode
     * UCMODE_0 = 3-pin SPI
     */
    HWREG8(baseAddress + OFS_UCAxCTL0) |= (
        param->msbFirst +
        param->clockPhase +
        param->clockPolarity +
        UCMST +
        UCSYNC +
        UCMODE_0
        );
    //No modulation
    HWREG8(baseAddress + OFS_UCAxMCTL) = 0;

    return ( STATUS_SUCCESS) ;
}

void USCI_A_SPI_changeMasterClock(uint16_t baseAddress,
    USCI_A_SPI_changeMasterClockParam *param)
{
    //Disable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCSWRST;

    HWREG8(baseAddress + OFS_UCAxBRW) =
        (uint16_t)(param->clockSourceFrequency / param->desiredSpiClock);

    //Reset the UCSWRST bit to enable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) &= ~(UCSWRST);
}
bool USCI_A_SPI_initSlave (uint16_t baseAddress,
    uint8_t msbFirst,
    uint8_t clockPhase,
    uint8_t clockPolarity
    )
{

    //Disable USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1)  |= UCSWRST;

    //Reset OFS_UCAxCTL0 register
    HWREG8(baseAddress + OFS_UCAxCTL0) &= ~(UCMSB +
                                            UC7BIT +
                                            UCMST +
                                            UCCKPL +
                                            UCCKPH +
                                            UCMODE_3
                                            );

    //Clock polarity, phase select, msbFirst, SYNC, Mode0
    HWREG8(baseAddress + OFS_UCAxCTL0) |= (clockPhase +
                                        clockPolarity +
                                        msbFirst +
                                        UCSYNC +
                                        UCMODE_0
                                        );

    return ( STATUS_SUCCESS) ;
}

void USCI_A_SPI_changeClockPhasePolarity (uint16_t baseAddress,
    uint8_t clockPhase,
    uint8_t clockPolarity
    )
{

  //Disable the USCI Module
  HWREG8(baseAddress + OFS_UCAxCTL1) |= UCSWRST;

  HWREG8(baseAddress + OFS_UCAxCTL0) &= ~(UCCKPH + UCCKPL);

  HWREG8(baseAddress + OFS_UCAxCTL0) |= (
        clockPhase +
        clockPolarity
          );

  //Reset the UCSWRST bit to enable the USCI Module
  HWREG8(baseAddress + OFS_UCAxCTL1) &= ~(UCSWRST);
}

void USCI_A_SPI_transmitData ( uint16_t baseAddress,
    uint8_t transmitData
    )
{
    HWREG8(baseAddress + OFS_UCAxTXBUF) = transmitData;
}

uint8_t USCI_A_SPI_receiveData (uint16_t baseAddress)
{
    return ( HWREG8(baseAddress + OFS_UCAxRXBUF)) ;
}

void USCI_A_SPI_enableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    HWREG8(baseAddress + OFS_UCAxIE) |= mask;
}

void USCI_A_SPI_disableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    HWREG8(baseAddress + OFS_UCAxIE) &= ~mask;
}

uint8_t USCI_A_SPI_getInterruptStatus (uint16_t baseAddress,
    uint8_t mask
    )
{
    return ( HWREG8(baseAddress + OFS_UCAxIFG) & mask );
}

void USCI_A_SPI_clearInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    HWREG8(baseAddress + OFS_UCAxIFG) &=  ~mask;
}

void USCI_A_SPI_enable (uint16_t baseAddress)
{
    //Reset the UCSWRST bit to enable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) &= ~(UCSWRST);
}

void USCI_A_SPI_disable (uint16_t baseAddress)
{
    //Set the UCSWRST bit to disable the USCI Module
    HWREG8(baseAddress + OFS_UCAxCTL1) |= UCSWRST;
}

uint32_t USCI_A_SPI_getReceiveBufferAddressForDMA (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCAxRXBUF );
}

uint32_t USCI_A_SPI_getTransmitBufferAddressForDMA (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCAxTXBUF );
}

uint8_t USCI_A_SPI_isBusy (uint16_t baseAddress)
{
    //Return the bus busy status.
    return (HWREG8(baseAddress + OFS_UCAxSTAT) & UCBUSY);
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for usci_a_spi_api
//! @}
//
//*****************************************************************************
