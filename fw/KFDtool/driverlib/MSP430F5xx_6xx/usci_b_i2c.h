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
// usci_b_i2c.h - Driver for the USCI_B_I2C Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_USCI_B_I2C_H__
#define __MSP430WARE_USCI_B_I2C_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_USCI_Bx__

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
//! \brief Used in the USCI_B_I2C_initMaster() function as the param parameter.
//
//*****************************************************************************
typedef struct USCI_B_I2C_initMasterParam {
    //! Is the clocksource.
    //! \n Valid values are:
    //! - \b USCI_B_I2C_CLOCKSOURCE_ACLK
    //! - \b USCI_B_I2C_CLOCKSOURCE_SMCLK
    uint8_t selectClockSource;
    //! Is the rate of the clock supplied to the I2C module.
    uint32_t i2cClk;
    //! Set up for selecting data transfer rate.
    //! \n Valid values are:
    //! - \b USCI_B_I2C_SET_DATA_RATE_400KBPS
    //! - \b USCI_B_I2C_SET_DATA_RATE_100KBPS
    uint32_t dataRate;
} USCI_B_I2C_initMasterParam;


//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: USCI_B_I2C_initMaster().
//
//*****************************************************************************
#define USCI_B_I2C_CLOCKSOURCE_ACLK                                UCSSEL__ACLK
#define USCI_B_I2C_CLOCKSOURCE_SMCLK                              UCSSEL__SMCLK

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: USCI_B_I2C_initMaster().
//
//*****************************************************************************
#define USCI_B_I2C_SET_DATA_RATE_400KBPS                                 400000
#define USCI_B_I2C_SET_DATA_RATE_100KBPS                                 100000

//*****************************************************************************
//
// The following are values that can be passed to the mode parameter for
// functions: USCI_B_I2C_setMode().
//
//*****************************************************************************
#define USCI_B_I2C_TRANSMIT_MODE                                           UCTR
#define USCI_B_I2C_RECEIVE_MODE                                            0x00

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: USCI_B_I2C_enableInterrupt(), USCI_B_I2C_disableInterrupt(),
// USCI_B_I2C_clearInterrupt(), and USCI_B_I2C_getInterruptStatus() as well as
// returned by the USCI_B_I2C_getInterruptStatus() function.
//
//*****************************************************************************
#define USCI_B_I2C_STOP_INTERRUPT                                       UCSTPIE
#define USCI_B_I2C_START_INTERRUPT                                      UCSTTIE
#define USCI_B_I2C_RECEIVE_INTERRUPT                                     UCRXIE
#define USCI_B_I2C_TRANSMIT_INTERRUPT                                    UCTXIE
#define USCI_B_I2C_NAK_INTERRUPT                                       UCNACKIE
#define USCI_B_I2C_ARBITRATIONLOST_INTERRUPT                             UCALIE

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the USCI_B_I2C_isBusy() function and the
// USCI_B_I2C_isBusBusy() function.
//
//*****************************************************************************
#define USCI_B_I2C_BUS_BUSY                                             UCBBUSY
#define USCI_B_I2C_BUS_NOT_BUSY                                            0x00

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the USCI_B_I2C_masterIsStartSent() function.
//
//*****************************************************************************
#define USCI_B_I2C_SENDING_START                                        UCTXSTT
#define USCI_B_I2C_START_SEND_COMPLETE                                     0x00

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the USCI_B_I2C_masterIsStopSent() function.
//
//*****************************************************************************
#define USCI_B_I2C_SENDING_STOP                                         UCTXSTP
#define USCI_B_I2C_STOP_SEND_COMPLETE                                      0x00

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the I2C Master block.
//!
//! This function initializes operation of the I2C Master block. Upon
//! successful initialization of the I2C block, this function will have set the
//! bus speed for the master; however I2C module is still disabled till
//! USCI_B_I2C_enable is invoked. If the parameter \e dataRate is
//! USCI_B_I2C_SET_DATA_RATE_400KBPS, then the master block will be set up to
//! transfer data at 400 kbps; otherwise, it will be set up to transfer data at
//! 100 kbps.
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param param is the pointe to struct for master initialization.
//!
//! Modified bits are \b UCBxBR0 of \b UCBxBR1 register; bits \b UCSSELx and \b
//! UCSWRST of \b UCBxCTL1 register; bits \b UCMST, \b UCMODE_3 and \b UCSYNC
//! of \b UCBxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_initMaster(uint16_t baseAddress,
                                  USCI_B_I2C_initMasterParam *param);

//*****************************************************************************
//
//! \brief Initializes the I2C Slave block.
//!
//! This function initializes operation of the I2C as a Slave mode. Upon
//! successful initialization of the I2C blocks, this function will have set
//! the slave address but the I2C module is still disabled till
//! USCI_B_I2C_enable is invoked.
//!
//! \param baseAddress is the base address of the I2C Slave module.
//! \param slaveAddress 7-bit slave address
//!
//! Modified bits of \b UCBxI2COA register; bits \b UCSWRST of \b UCBxCTL1
//! register; bits \b UCMODE_3 and \b UCSYNC of \b UCBxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_initSlave(uint16_t baseAddress,
                                 uint8_t slaveAddress);

//*****************************************************************************
//
//! \brief Enables the I2C block.
//!
//! This will enable operation of the I2C block.
//!
//! \param baseAddress is the base address of the USCI I2C module.
//!
//! Modified bits are \b UCSWRST of \b UCBxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_enable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the I2C block.
//!
//! This will disable operation of the I2C block.
//!
//! \param baseAddress is the base address of the USCI I2C module.
//!
//! Modified bits are \b UCSWRST of \b UCBxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_disable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets the address that the I2C Master will place on the bus.
//!
//! This function will set the address that the I2C Master will place on the
//! bus when initiating a transaction.
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param slaveAddress 7-bit slave address
//!
//! Modified bits of \b UCBxI2CSA register; bits \b UCSWRST of \b UCBxCTL1
//! register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_setSlaveAddress(uint16_t baseAddress,
                                       uint8_t slaveAddress);

//*****************************************************************************
//
//! \brief Sets the mode of the I2C device
//!
//! When the receive parameter is set to USCI_B_I2C_TRANSMIT_MODE, the address
//! will indicate that the I2C module is in receive mode; otherwise, the I2C
//! module is in send mode.
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param mode indicates whether module is in transmit/receive mode
//!        Valid values are:
//!        - \b USCI_B_I2C_TRANSMIT_MODE
//!        - \b USCI_B_I2C_RECEIVE_MODE [Default]
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_setMode(uint16_t baseAddress,
                               uint8_t mode);

//*****************************************************************************
//
//! \brief Transmits a byte from the I2C Module.
//!
//! This function will place the supplied data into I2C transmit data register
//! to start transmission Modified bit is UCBxTXBUF register
//!
//! \param baseAddress is the base address of the I2C module.
//! \param transmitData data to be transmitted from the I2C module
//!
//! Modified bits of \b UCBxTXBUF register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_slavePutData(uint16_t baseAddress,
                                    uint8_t transmitData);

//*****************************************************************************
//
//! \brief Receives a byte that has been sent to the I2C Module.
//!
//! This function reads a byte of data from the I2C receive data Register.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return Returns the byte received from by the I2C module, cast as an
//!         uint8_t.
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_slaveGetData(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Indicates whether or not the I2C bus is busy.
//!
//! This function returns an indication of whether or not the I2C bus is
//! busy.This function checks the status of the bus via UCBBUSY bit in UCBxSTAT
//! register.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return Returns USCI_B_I2C_BUS_BUSY if the I2C Master is busy; otherwise,
//!         returns USCI_B_I2C_BUS_NOT_BUSY.
//!         Return one of the following:
//!         - \b USCI_B_I2C_BUS_BUSY
//!         - \b USCI_B_I2C_BUS_NOT_BUSY
//!         \n indicating if the USCI_B_I2C is busy
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_isBusBusy(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief DEPRECATED - Function may be removed in future release. Indicates
//! whether or not the I2C module is busy.
//!
//! This function returns an indication of whether or not the I2C module is
//! busy transmitting or receiving data. This function checks if the Transmit
//! or receive flag is set.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return Returns USCI_B_I2C_BUS_BUSY if the I2C module is busy; otherwise,
//!         returns USCI_B_I2C_BUS_NOT_BUSY.
//!         Return one of the following:
//!         - \b USCI_B_I2C_BUS_BUSY
//!         - \b USCI_B_I2C_BUS_NOT_BUSY
//!         \n indicating if the USCI_B_I2C is busy
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_isBusy(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Indicates whether STOP got sent.
//!
//! This function returns an indication of whether or not STOP got sent This
//! function checks the status of the bus via UCTXSTP bit in UCBxCTL1 register.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return Returns USCI_B_I2C_STOP_SEND_COMPLETE if the I2C Master finished
//!         sending STOP; otherwise, returns USCI_B_I2C_SENDING_STOP.
//!         Return one of the following:
//!         - \b USCI_B_I2C_SENDING_STOP
//!         - \b USCI_B_I2C_STOP_SEND_COMPLETE
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_masterIsStopSent(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Indicates whether START got sent.
//!
//! This function returns an indication of whether or not START got sent This
//! function checks the status of the bus via UCTXSTT bit in UCBxCTL1 register.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return Returns USCI_B_I2C_START_SEND_COMPLETE if the I2C Master finished
//!         sending START; otherwise, returns USCI_B_I2C_SENDING_START.
//!         Return one of the following:
//!         - \b USCI_B_I2C_SENDING_START
//!         - \b USCI_B_I2C_START_SEND_COMPLETE
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_masterIsStartSent(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief This function is used by the Master module to initiate START
//!
//! This function is used by the Master module to initiate STOP
//!
//! \param baseAddress is the base address of the I2C Master module.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterSendStart(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables individual I2C interrupt sources.
//!
//! Enables the indicated I2C interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the I2C module.
//! \param mask is the bit mask of the interrupt sources to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_B_I2C_STOP_INTERRUPT - STOP condition interrupt
//!        - \b USCI_B_I2C_START_INTERRUPT - START condition interrupt
//!        - \b USCI_B_I2C_RECEIVE_INTERRUPT - Receive interrupt
//!        - \b USCI_B_I2C_TRANSMIT_INTERRUPT - Transmit interrupt
//!        - \b USCI_B_I2C_NAK_INTERRUPT - Not-acknowledge interrupt
//!        - \b USCI_B_I2C_ARBITRATIONLOST_INTERRUPT - Arbitration lost
//!           interrupt
//!
//! Modified bits of \b UCBxIE register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_enableInterrupt(uint16_t baseAddress,
                                       uint8_t mask);

//*****************************************************************************
//
//! \brief Disables individual I2C interrupt sources.
//!
//! Disables the indicated I2C interrupt sources. Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the I2C module.
//! \param mask is the bit mask of the interrupt sources to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_B_I2C_STOP_INTERRUPT - STOP condition interrupt
//!        - \b USCI_B_I2C_START_INTERRUPT - START condition interrupt
//!        - \b USCI_B_I2C_RECEIVE_INTERRUPT - Receive interrupt
//!        - \b USCI_B_I2C_TRANSMIT_INTERRUPT - Transmit interrupt
//!        - \b USCI_B_I2C_NAK_INTERRUPT - Not-acknowledge interrupt
//!        - \b USCI_B_I2C_ARBITRATIONLOST_INTERRUPT - Arbitration lost
//!           interrupt
//!
//! Modified bits of \b UCBxIE register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_disableInterrupt(uint16_t baseAddress,
                                        uint8_t mask);

//*****************************************************************************
//
//! \brief Clears I2C interrupt sources.
//!
//! The I2C interrupt source is cleared, so that it no longer asserts. The
//! highest interrupt flag is automatically cleared when an interrupt vector
//! generator is used.
//!
//! \param baseAddress is the base address of the I2C Slave module.
//! \param mask is a bit mask of the interrupt sources to be cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_B_I2C_STOP_INTERRUPT - STOP condition interrupt
//!        - \b USCI_B_I2C_START_INTERRUPT - START condition interrupt
//!        - \b USCI_B_I2C_RECEIVE_INTERRUPT - Receive interrupt
//!        - \b USCI_B_I2C_TRANSMIT_INTERRUPT - Transmit interrupt
//!        - \b USCI_B_I2C_NAK_INTERRUPT - Not-acknowledge interrupt
//!        - \b USCI_B_I2C_ARBITRATIONLOST_INTERRUPT - Arbitration lost
//!           interrupt
//!
//! Modified bits of \b UCBxIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_clearInterrupt(uint16_t baseAddress,
                                      uint8_t mask);

//*****************************************************************************
//
//! \brief Gets the current I2C interrupt status.
//!
//! This returns the interrupt status for the I2C module based on which flag is
//! passed. mask parameter can be logic OR of any of the following selection.
//!
//! \param baseAddress is the base address of the I2C module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b USCI_B_I2C_STOP_INTERRUPT - STOP condition interrupt
//!        - \b USCI_B_I2C_START_INTERRUPT - START condition interrupt
//!        - \b USCI_B_I2C_RECEIVE_INTERRUPT - Receive interrupt
//!        - \b USCI_B_I2C_TRANSMIT_INTERRUPT - Transmit interrupt
//!        - \b USCI_B_I2C_NAK_INTERRUPT - Not-acknowledge interrupt
//!        - \b USCI_B_I2C_ARBITRATIONLOST_INTERRUPT - Arbitration lost
//!           interrupt
//!
//! \return the masked status of the interrupt flag
//!         Return Logical OR of any of the following:
//!         - \b USCI_B_I2C_STOP_INTERRUPT STOP condition interrupt
//!         - \b USCI_B_I2C_START_INTERRUPT START condition interrupt
//!         - \b USCI_B_I2C_RECEIVE_INTERRUPT Receive interrupt
//!         - \b USCI_B_I2C_TRANSMIT_INTERRUPT Transmit interrupt
//!         - \b USCI_B_I2C_NAK_INTERRUPT Not-acknowledge interrupt
//!         - \b USCI_B_I2C_ARBITRATIONLOST_INTERRUPT Arbitration lost
//!         interrupt
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_getInterruptStatus(uint16_t baseAddress,
                                             uint8_t mask);

//*****************************************************************************
//
//! \brief Does single byte transmission from Master to Slave
//!
//! This function is used by the Master module to send a single byte.This
//! function does the following: - Sends START; - Transmits the byte to the
//! Slave; - Sends STOP
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the data byte to be transmitted
//!
//! Modified bits of \b UCBxTXBUF register, bits of \b UCBxIFG register, bits
//! of \b UCBxCTL1 register and bits of \b UCBxIE register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterSendSingleByte(uint16_t baseAddress,
                                            uint8_t txData);

//*****************************************************************************
//
//! \brief Does single byte transmission from Master to Slave with timeout
//!
//! This function is used by the Master module to send a single byte. This
//! function does the following: - Sends START; - Transmits the byte to the
//! Slave; - Sends STOP
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the data byte to be transmitted
//! \param timeout is the amount of time to wait until giving up
//!
//! Modified bits of \b UCBxTXBUF register, bits of \b UCBxIFG register, bits
//! of \b UCBxCTL1 register and bits of \b UCBxIE register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the transmission process.
//
//*****************************************************************************
extern bool USCI_B_I2C_masterSendSingleByteWithTimeout(uint16_t baseAddress,
                                                       uint8_t txData,
                                                       uint32_t timeout);

//*****************************************************************************
//
//! \brief Starts multi-byte transmission from Master to Slave
//!
//! This function is used by the Master module to send a single byte. This
//! function does the following: - Sends START; - Transmits the first data byte
//! of a multi-byte transmission to the Slave
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the first data byte to be transmitted
//!
//! Modified bits of \b UCBxTXBUF register, bits of \b UCBxIFG register, bits
//! of \b UCBxCTL1 register and bits of \b UCBxIE register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterSendMultiByteStart(uint16_t baseAddress,
                                                uint8_t txData);

//*****************************************************************************
//
//! \brief Starts multi-byte transmission from Master to Slave with timeout
//!
//! This function is used by the Master module to send a single byte. This
//! function does the following: - Sends START; - Transmits the first data byte
//! of a multi-byte transmission to the Slave
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the first data byte to be transmitted
//! \param timeout is the amount of time to wait until giving up
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the transmission process.
//
//*****************************************************************************
extern bool USCI_B_I2C_masterSendMultiByteStartWithTimeout(uint16_t baseAddress,
                                                           uint8_t txData,
                                                           uint32_t timeout);

//*****************************************************************************
//
//! \brief Continues multi-byte transmission from Master to Slave
//!
//! This function is used by the Master module continue each byte of a multi-
//! byte transmission. This function does the following: -Transmits each data
//! byte of a multi-byte transmission to the Slave
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the next data byte to be transmitted
//!
//! Modified bits of \b UCBxTXBUF register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterSendMultiByteNext(uint16_t baseAddress,
                                               uint8_t txData);

//*****************************************************************************
//
//! \brief Continues multi-byte transmission from Master to Slave with timeout
//!
//! This function is used by the Master module continue each byte of a multi-
//! byte transmission. This function does the following: -Transmits each data
//! byte of a multi-byte transmission to the Slave
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the next data byte to be transmitted
//! \param timeout is the amount of time to wait until giving up
//!
//! Modified bits of \b UCBxTXBUF register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the transmission process.
//
//*****************************************************************************
extern bool USCI_B_I2C_masterSendMultiByteNextWithTimeout(uint16_t baseAddress,
                                                          uint8_t txData,
                                                          uint32_t timeout);

//*****************************************************************************
//
//! \brief Finishes multi-byte transmission from Master to Slave
//!
//! This function is used by the Master module to send the last byte and STOP.
//! This function does the following: - Transmits the last data byte of a
//! multi-byte transmission to the Slave; - Sends STOP
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the last data byte to be transmitted in a multi-byte
//!        transmission
//!
//! Modified bits of \b UCBxTXBUF register and bits of \b UCBxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterSendMultiByteFinish(uint16_t baseAddress,
                                                 uint8_t txData);

//*****************************************************************************
//
//! \brief Finishes multi-byte transmission from Master to Slave with timeout
//!
//! This function is used by the Master module to send the last byte and STOP.
//! This function does the following: - Transmits the last data byte of a
//! multi-byte transmission to the Slave; - Sends STOP
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param txData is the last data byte to be transmitted in a multi-byte
//!        transmission
//! \param timeout is the amount of time to wait until giving up
//!
//! Modified bits of \b UCBxTXBUF register and bits of \b UCBxCTL1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the transmission process.
//
//*****************************************************************************
extern bool USCI_B_I2C_masterSendMultiByteFinishWithTimeout(uint16_t baseAddress,
                                                            uint8_t txData,
                                                            uint32_t timeout);

//*****************************************************************************
//
//! \brief Send STOP byte at the end of a multi-byte transmission from Master
//! to Slave
//!
//! This function is used by the Master module send STOP at the end of a multi-
//! byte transmission. This function does the following: - Sends a STOP after
//! current transmission is complete
//!
//! \param baseAddress is the base address of the I2C Master module.
//!
//! Modified bits are \b UCTXSTP of \b UCBxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterSendMultiByteStop(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Send STOP byte at the end of a multi-byte transmission from Master
//! to Slave with timeout
//!
//! This function is used by the Master module send STOP at the end of a multi-
//! byte transmission. This function does the following: - Sends a STOP after
//! current transmission is complete
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param timeout is the amount of time to wait until giving up
//!
//! Modified bits are \b UCTXSTP of \b UCBxCTL1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the transmission process.
//
//*****************************************************************************
extern bool USCI_B_I2C_masterSendMultiByteStopWithTimeout(uint16_t baseAddress,
                                                          uint32_t timeout);

//*****************************************************************************
//
//! \brief Starts multi-byte reception at the Master end
//!
//! This function is used by the Master module initiate reception of a single
//! byte. This function does the following: - Sends START
//!
//! \param baseAddress is the base address of the I2C Master module.
//!
//! Modified bits are \b UCTXSTT of \b UCBxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterReceiveMultiByteStart(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Starts multi-byte reception at the Master end one byte at a time
//!
//! This function is used by the Master module to receive each byte of a multi-
//! byte reception. This function reads currently received byte
//!
//! \param baseAddress is the base address of the I2C Master module.
//!
//! \return Received byte at Master end.
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_masterReceiveMultiByteNext(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Finishes multi-byte reception at the Master end
//!
//! This function is used by the Master module to initiate completion of a
//! multi-byte reception. This function does the following: - Receives the
//! current byte and initiates the STOP from Master to Slave
//!
//! \param baseAddress is the base address of the I2C Master module.
//!
//! Modified bits are \b UCTXSTP of \b UCBxCTL1 register.
//!
//! \return Received byte at Master end.
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_masterReceiveMultiByteFinish(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Finishes multi-byte reception at the Master end with timeout
//!
//! This function is used by the Master module to initiate completion of a
//! multi-byte reception. This function does the following: - Receives the
//! current byte and initiates the STOP from Master to Slave
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param rxData is a pointer to the location to store the received byte at
//!        master end
//! \param timeout is the amount of time to wait until giving up
//!
//! Modified bits are \b UCTXSTP of \b UCBxCTL1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the transmission process.
//
//*****************************************************************************
extern bool USCI_B_I2C_masterReceiveMultiByteFinishWithTimeout(uint16_t baseAddress,
                                                               uint8_t *rxData,
                                                               uint32_t timeout);

//*****************************************************************************
//
//! \brief Sends the STOP at the end of a multi-byte reception at the Master
//! end
//!
//! This function is used by the Master module to initiate STOP
//!
//! \param baseAddress is the base address of the I2C Master module.
//!
//! Modified bits are \b UCTXSTP of \b UCBxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterReceiveMultiByteStop(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Initiates a single byte Reception at the Master End
//!
//! This function sends a START and STOP immediately to indicate Single byte
//! reception
//!
//! \param baseAddress is the base address of the I2C Master module.
//!
//! Modified bits are \b GIE of \b SR register; bits \b UCTXSTT and \b UCTXSTP
//! of \b UCBxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void USCI_B_I2C_masterReceiveSingleStart(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Initiates a single byte Reception at the Master End with timeout
//!
//! This function sends a START and STOP immediately to indicate Single byte
//! reception
//!
//! \param baseAddress is the base address of the I2C Master module.
//! \param timeout is the amount of time to wait until giving up
//!
//! Modified bits are \b GIE of \b SR register; bits \b UCTXSTT and \b UCTXSTP
//! of \b UCBxCTL1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the transmission process.
//
//*****************************************************************************
extern bool USCI_B_I2C_masterReceiveSingleStartWithTimeout(uint16_t baseAddress,
                                                           uint32_t timeout);

//*****************************************************************************
//
//! \brief Receives a byte that has been sent to the I2C Master Module.
//!
//! This function reads a byte of data from the I2C receive data Register.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return Returns the byte received from by the I2C module, cast as an
//!         uint8_t.
//
//*****************************************************************************
extern uint8_t USCI_B_I2C_masterReceiveSingle(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the address of the RX Buffer of the I2C for the DMA module.
//!
//! Returns the address of the I2C RX Buffer. This can be used in conjunction
//! with the DMA to store the received data directly to memory.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return the address of the RX Buffer
//
//*****************************************************************************
extern uint32_t USCI_B_I2C_getReceiveBufferAddressForDMA(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the address of the TX Buffer of the I2C for the DMA module.
//!
//! Returns the address of the I2C TX Buffer. This can be used in conjunction
//! with the DMA to obtain transmitted data directly from memory.
//!
//! \param baseAddress is the base address of the I2C module.
//!
//! \return the address of the TX Buffer
//
//*****************************************************************************
extern uint32_t USCI_B_I2C_getTransmitBufferAddressForDMA(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_USCI_B_I2C_H__
