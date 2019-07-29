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
// usci_a_spi.h - Driver for the USCI_A_SPI Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_USCI_A_SPI_H__
#define __MSP430WARE_USCI_A_SPI_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_USCI_Ax__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "inc/hw_memmap.h"
//*****************************************************************************
//
//! \brief Used in the USCI_A_SPI_initMaster() function as the param parameter.
//
//*****************************************************************************
typedef struct USCI_A_SPI_initMasterParam {
    //! Selects Clock source.
    //! \n Valid values are:
    //! - \b USCI_A_SPI_CLOCKSOURCE_ACLK
    //! - \b USCI_A_SPI_CLOCKSOURCE_SMCLK
    uint8_t selectClockSource;
    //! Is the frequency of the selected clock source
    uint32_t clockSourceFrequency;
    //! Is the desired clock rate for SPI communication
    uint32_t desiredSpiClock;
    //! Controls the direction of the receive and transmit shift register.
    //! \n Valid values are:
    //! - \b USCI_A_SPI_MSB_FIRST
    //! - \b USCI_A_SPI_LSB_FIRST [Default]
    uint8_t msbFirst;
    //! Is clock phase select.
    //! \n Valid values are:
    //! - \b USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT [Default]
    //! - \b USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
    uint8_t clockPhase;
    //! \n Valid values are:
    //! - \b USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
    //! - \b USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
    uint8_t clockPolarity;
} USCI_A_SPI_initMasterParam;

//*****************************************************************************
//
//! \brief Used in the USCI_A_SPI_changeMasterClock() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct USCI_A_SPI_changeMasterClockParam {
    //! Is the frequency of the selected clock source
    uint32_t clockSourceFrequency;
    //! Is the desired clock rate for SPI communication
    uint32_t desiredSpiClock;
} USCI_A_SPI_changeMasterClockParam;


//*****************************************************************************
//
// The following are values that can be passed to the clockPhase parameter for
// functions: USCI_A_SPI_initSlave(), and
// USCI_A_SPI_changeClockPhasePolarity(); the param parameter for functions:
// USCI_A_SPI_initMaster().
//
//*****************************************************************************
#define USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT             0x00
#define USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT           UCCKPH

//*****************************************************************************
//
// The following are values that can be passed to the msbFirst parameter for
// functions: USCI_A_SPI_initSlave(); the param parameter for functions:
// USCI_A_SPI_initMaster().
//
//*****************************************************************************
#define USCI_A_SPI_MSB_FIRST                                              UCMSB
#define USCI_A_SPI_LSB_FIRST                                               0x00

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: USCI_A_SPI_initMaster(); the clockPolarity parameter for
// functions: USCI_A_SPI_initSlave(), and
// USCI_A_SPI_changeClockPhasePolarity().
//
//*****************************************************************************
#define USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH                         UCCKPL
#define USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW                            0x00

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: USCI_A_SPI_initMaster().
//
//*****************************************************************************
#define USCI_A_SPI_CLOCKSOURCE_ACLK                                UCSSEL__ACLK
#define USCI_A_SPI_CLOCKSOURCE_SMCLK                              UCSSEL__SMCLK

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: USCI_A_SPI_enableInterrupt(), USCI_A_SPI_disableInterrupt(),
// USCI_A_SPI_getInterruptStatus(), and USCI_A_SPI_clearInterrupt() as well as
// returned by the USCI_A_SPI_getInterruptStatus() function.
//
//*****************************************************************************
#define USCI_A_SPI_TRANSMIT_INTERRUPT                                    UCTXIE
#define USCI_A_SPI_RECEIVE_INTERRUPT                                     UCRXIE

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the USCI_A_SPI_isBusy() function.
//
//*****************************************************************************
#define USCI_A_SPI_BUSY                                                  UCBUSY
#define USCI_A_SPI_NOT_BUSY                                                0x00

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the SPI Master block.
//!
//! Upon successful initialization of the SPI master block, this function will
//! have set the bus speed for the master, but the SPI Master block still
//! remains disabled and must be enabled with USCI_A_SPI_enable()
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param param is the pointer to struct for master initialization.
//!
//! Modified bits are \b UCCKPH, \b UCCKPL, \b UC7BIT and \b UCMSB of \b
//! UCAxCTL0 register; bits \b UCSSELx and \b UCSWRST of \b UCAxCTL1 register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern bool USCI_A_SPI_initMaster(uint16_t baseAddress,
                                  USCI_A_SPI_initMasterParam *param);

//*****************************************************************************
//
//! \brief Initializes the SPI Master clock.At the end of this function call,
//! SPI module is left enabled.
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param param is the pointer to struct for master clock setting.
//!
//! Modified bits of \b UCAxBRW register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_changeMasterClock(uint16_t baseAddress,
                                         USCI_A_SPI_changeMasterClockParam *param);

//*****************************************************************************
//
//! \brief Initializes the SPI Slave block.
//!
//! Upon successful initialization of the SPI slave block, this function will
//! have initialized the slave block, but the SPI Slave block still remains
//! disabled and must be enabled with USCI_A_SPI_enable()
//!
//! \param baseAddress is the base address of the SPI Slave module.
//! \param msbFirst controls the direction of the receive and transmit shift
//!        register.
//!        Valid values are:
//!        - \b USCI_A_SPI_MSB_FIRST
//!        - \b USCI_A_SPI_LSB_FIRST [Default]
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity
//!        Valid values are:
//!        - \b USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//!
//! Modified bits are \b UCMSB, \b UCMST, \b UC7BIT, \b UCCKPL, \b UCCKPH and
//! \b UCMODE of \b UCAxCTL0 register; bits \b UCSWRST of \b UCAxCTL1 register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern bool USCI_A_SPI_initSlave(uint16_t baseAddress,
                                 uint8_t msbFirst,
                                 uint8_t clockPhase,
                                 uint8_t clockPolarity);

//*****************************************************************************
//
//! \brief Changes the SPI clock phase and polarity.At the end of this function
//! call, SPI module is left enabled.
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity
//!        Valid values are:
//!        - \b USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//!
//! Modified bits are \b UCCKPL and \b UCCKPH of \b UCAxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_changeClockPhasePolarity(uint16_t baseAddress,
                                                uint8_t clockPhase,
                                                uint8_t clockPolarity);

//*****************************************************************************
//
//! \brief Transmits a byte from the SPI Module.
//!
//! This function will place the supplied data into SPI transmit data register
//! to start transmission
//!
//! \param baseAddress is the base address of the SPI module.
//! \param transmitData data to be transmitted from the SPI module
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_transmitData(uint16_t baseAddress,
                                    uint8_t transmitData);

//*****************************************************************************
//
//! \brief Receives a byte that has been sent to the SPI Module.
//!
//! This function reads a byte of data from the SPI receive data Register.
//!
//! \param baseAddress is the base address of the SPI module.
//!
//! \return Returns the byte received from by the SPI module, cast as an
//!         uint8_t.
//
//*****************************************************************************
extern uint8_t USCI_A_SPI_receiveData(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables individual SPI interrupt sources.
//!
//! Enables the indicated SPI interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the SPI module.
//! \param mask is the bit mask of the interrupt sources to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b USCI_A_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIE register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_enableInterrupt(uint16_t baseAddress,
                                       uint8_t mask);

//*****************************************************************************
//
//! \brief Disables individual SPI interrupt sources.
//!
//! Disables the indicated SPI interrupt sources. Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the SPI module.
//! \param mask is the bit mask of the interrupt sources to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b USCI_A_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIE register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_disableInterrupt(uint16_t baseAddress,
                                        uint8_t mask);

//*****************************************************************************
//
//! \brief Gets the current SPI interrupt status.
//!
//! This returns the interrupt status for the SPI module based on which flag is
//! passed.
//!
//! \param baseAddress is the base address of the SPI module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b USCI_A_SPI_RECEIVE_INTERRUPT
//!
//! \return The current interrupt status as the mask of the set flags
//!         Return Logical OR of any of the following:
//!         - \b USCI_A_SPI_TRANSMIT_INTERRUPT
//!         - \b USCI_A_SPI_RECEIVE_INTERRUPT
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint8_t USCI_A_SPI_getInterruptStatus(uint16_t baseAddress,
                                             uint8_t mask);

//*****************************************************************************
//
//! \brief Clears the selected SPI interrupt status flag.
//!
//! \param baseAddress is the base address of the SPI module.
//! \param mask is the masked interrupt flag to be cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_A_SPI_TRANSMIT_INTERRUPT
//!        - \b USCI_A_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_clearInterrupt(uint16_t baseAddress,
                                      uint8_t mask);

//*****************************************************************************
//
//! \brief Enables the SPI block.
//!
//! This will enable operation of the SPI block.
//!
//! \param baseAddress is the base address of the USCI SPI module.
//!
//! Modified bits are \b UCSWRST of \b UCAxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_enable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the SPI block.
//!
//! This will disable operation of the SPI block.
//!
//! \param baseAddress is the base address of the USCI SPI module.
//!
//! Modified bits are \b UCSWRST of \b UCAxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_A_SPI_disable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the address of the RX Buffer of the SPI for the DMA module.
//!
//! Returns the address of the SPI RX Buffer. This can be used in conjunction
//! with the DMA to store the received data directly to memory.
//!
//! \param baseAddress is the base address of the SPI module.
//!
//! \return the address of the RX Buffer
//
//*****************************************************************************
extern uint32_t USCI_A_SPI_getReceiveBufferAddressForDMA(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the address of the TX Buffer of the SPI for the DMA module.
//!
//! Returns the address of the SPI TX Buffer. This can be used in conjunction
//! with the DMA to obtain transmitted data directly from memory.
//!
//! \param baseAddress is the base address of the SPI module.
//!
//! \return the address of the TX Buffer
//
//*****************************************************************************
extern uint32_t USCI_A_SPI_getTransmitBufferAddressForDMA(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Indicates whether or not the SPI bus is busy.
//!
//! This function returns an indication of whether or not the SPI bus is
//! busy.This function checks the status of the bus via UCBBUSY bit
//!
//! \param baseAddress is the base address of the SPI module.
//!
//! \return USCI_A_SPI_BUSY if the SPI module transmitting or receiving is
//!         busy; otherwise, returns USCI_A_SPI_NOT_BUSY.
//!         Return one of the following:
//!         - \b USCI_A_SPI_BUSY
//!         - \b USCI_A_SPI_NOT_BUSY
//!         \n indicating if the USCI_A_SPI is busy
//
//*****************************************************************************
extern uint8_t USCI_A_SPI_isBusy(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_USCI_A_SPI_H__
