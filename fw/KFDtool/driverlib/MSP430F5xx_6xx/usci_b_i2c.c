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
// usci_b_i2c.c - Driver for the usci_b_i2c Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup usci_b_i2c_api usci_b_i2c
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_USCI_Bx__
#include "usci_b_i2c.h"

#include <assert.h>

void USCI_B_I2C_initMaster(uint16_t baseAddress, USCI_B_I2C_initMasterParam *param)
{
    uint16_t preScalarValue;

    //Disable the USCI module and clears the other bits of control register
    HWREG8(baseAddress + OFS_UCBxCTL1) = UCSWRST;

    /*
     * Configure as I2C master mode.
     * UCMST = Master mode
     * UCMODE_3 = I2C mode
     * UCSYNC = Synchronous mode
     */
    HWREG8(baseAddress + OFS_UCBxCTL0) = UCMST + UCMODE_3 + UCSYNC;

    //Configure I2C clock source
    HWREG8(baseAddress + OFS_UCBxCTL1) = (param->selectClockSource + UCSWRST );

    /*
     * Compute the clock divider that achieves the fastest speed less than or
     * equal to the desired speed.  The numerator is biased to favor a larger
     * clock divider so that the resulting clock is always less than or equal
     * to the desired clock, never greater.
     */
    preScalarValue = (unsigned short)(param->i2cClk / param->dataRate);
    HWREG16(baseAddress + OFS_UCBxBRW) = preScalarValue;
}
void USCI_B_I2C_initSlave (uint16_t baseAddress,
    uint8_t slaveAddress
    )
{
    //Disable the USCI module
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCSWRST;

    //Clear USCI master mode
    HWREG8(baseAddress + OFS_UCBxCTL0) &= ~UCMST;

    //Confiugre I2C as Slave and Synchronous mode
    HWREG8(baseAddress + OFS_UCBxCTL0) = UCMODE_3 + UCSYNC;

    //Set up the slave address.
    HWREG16(baseAddress + OFS_UCBxI2COA) = slaveAddress;
}

void USCI_B_I2C_enable (uint16_t baseAddress)
{
    //Reset the UCSWRST bit to enable the USCI Module
    HWREG8(baseAddress + OFS_UCBxCTL1) &= ~(UCSWRST);
}

void USCI_B_I2C_disable (uint16_t baseAddress)
{
    //Set the UCSWRST bit to disable the USCI Module
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCSWRST;
}

void USCI_B_I2C_setSlaveAddress (uint16_t baseAddress,
    uint8_t slaveAddress
    )
{
    //Set the address of the slave with which the master will communicate.
    HWREG16(baseAddress + OFS_UCBxI2CSA) = (slaveAddress);
}

void USCI_B_I2C_setMode (uint16_t baseAddress,
    uint8_t mode
    )
{
    HWREG8(baseAddress + OFS_UCBxCTL1) &= ~USCI_B_I2C_TRANSMIT_MODE;
    HWREG8(baseAddress + OFS_UCBxCTL1) |= mode;
}

void USCI_B_I2C_slavePutData (uint16_t baseAddress,
    uint8_t transmitData
    )
{
    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = transmitData;
}

uint8_t USCI_B_I2C_slaveGetData (uint16_t baseAddress)
{
    //Read a byte.
    return (HWREG8(baseAddress + OFS_UCBxRXBUF));
}

uint8_t USCI_B_I2C_isBusBusy (uint16_t baseAddress)
{
    //Return the bus busy status.
    return (HWREG8(baseAddress + OFS_UCBxSTAT) & UCBBUSY);
}

uint8_t USCI_B_I2C_isBusy (uint16_t baseAddress)
{
    //Return the busy status.
    if ((HWREG8(baseAddress + OFS_UCBxIFG) & (UCTXIFG + UCRXIFG))){
        return (USCI_B_I2C_BUS_BUSY);
    } else   {
        return (USCI_B_I2C_BUS_NOT_BUSY);
    }
}

uint8_t USCI_B_I2C_masterIsStopSent (uint16_t baseAddress)
{
    //Return the bus busy status.
    return (HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTP);
}

uint8_t USCI_B_I2C_masterIsStartSent (uint16_t baseAddress)
{
    //Return if master has sent start
    return (HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTT);
}

void USCI_B_I2C_masterSendStart (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTT;
}

void USCI_B_I2C_enableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Enable the interrupt masked bit
    HWREG8(baseAddress + OFS_UCBxIE) |= mask;
}

void USCI_B_I2C_disableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Disable the interrupt masked bit
    HWREG8(baseAddress + OFS_UCBxIE) &= ~(mask);
}

void USCI_B_I2C_clearInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Clear the I2C interrupt source.
    HWREG8(baseAddress + OFS_UCBxIFG) &= ~(mask);
}

uint8_t USCI_B_I2C_getInterruptStatus (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Return the interrupt status of the request masked bit.
    return (HWREG8(baseAddress + OFS_UCBxIFG) & mask);
}

void USCI_B_I2C_masterSendSingleByte (uint16_t baseAddress,
    uint8_t txData
    )
{
    //Store current TXIE status
    uint8_t txieStatus = HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE;

    //Disable transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) &= ~(UCTXIE);

    //Send start condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTR + UCTXSTT;

    //Poll for transmit interrupt flag.
    while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;

    //Poll for transmit interrupt flag.
    while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    //Clear transmit interrupt flag before enabling interrupt again
    HWREG8(baseAddress + OFS_UCBxIFG) &= ~(UCTXIFG);

    //Reinstate transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) |= txieStatus;
}

bool USCI_B_I2C_masterSendSingleByteWithTimeout (uint16_t baseAddress,
    uint8_t txData,
    uint32_t timeout
    )
{
    // Creating variable for second timeout scenario
    uint32_t timeout2 = timeout;

    //Store current TXIE status
    uint8_t txieStatus = HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE;

    //Disable transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) &= ~(UCTXIE);

    //Send start condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTR + UCTXSTT;

    //Poll for transmit interrupt flag.
    while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) && --timeout) ;

    //Check if transfer timed out
    if (timeout == 0){
        return (STATUS_FAIL);
    }

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;

    //Poll for transmit interrupt flag.
    while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) && --timeout2) ;

    //Check if transfer timed out
    if (timeout2 == 0){
        return (STATUS_FAIL);
    }

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    //Clear transmit interrupt flag before enabling interrupt again
    HWREG8(baseAddress + OFS_UCBxIFG) &= ~(UCTXIFG);

    //Reinstate transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) |= txieStatus;

    return (STATUS_SUCCESS);
}

void USCI_B_I2C_masterSendMultiByteStart (uint16_t baseAddress,
    uint8_t txData
    )
{
    //Store current transmit interrupt enable
    uint8_t txieStatus = HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE;

    //Disable transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) &= ~(UCTXIE);

    //Send start condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTR +  UCTXSTT;

    //Poll for transmit interrupt flag.
    while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;

    //Reinstate transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) |= txieStatus;
}

bool USCI_B_I2C_masterSendMultiByteStartWithTimeout (uint16_t baseAddress,
    uint8_t txData,
    uint32_t timeout
    )
{
    //Store current transmit interrupt enable
    uint8_t txieStatus = HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE;

    //Disable transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) &= ~(UCTXIE);

    //Send start condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTR +  UCTXSTT;

    //Poll for transmit interrupt flag.
    while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) && --timeout) ;

    //Check if transfer timed out
    if (timeout == 0){
        return (STATUS_FAIL);
    }

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;

    //Reinstate transmit interrupt enable
    HWREG8(baseAddress + OFS_UCBxIE) |= txieStatus;

    return(STATUS_SUCCESS);
}

void USCI_B_I2C_masterSendMultiByteNext (uint16_t baseAddress,
    uint8_t txData
    )
{
    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE)){
        //Poll for transmit interrupt flag.
        while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;
    }

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;
}

bool USCI_B_I2C_masterSendMultiByteNextWithTimeout (uint16_t baseAddress,
    uint8_t txData,
    uint32_t timeout
    )
{
    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE)){
        //Poll for transmit interrupt flag.
        while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) && --timeout);

        //Check if transfer timed out
        if (timeout == 0){
            return (STATUS_FAIL);
        }
    }

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;

    return(STATUS_SUCCESS);
}

void USCI_B_I2C_masterSendMultiByteFinish (uint16_t baseAddress,
    uint8_t txData
    )
{
    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE)){
        //Poll for transmit interrupt flag.
        while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;
    }

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;

    //Poll for transmit interrupt flag.
    while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
}

bool USCI_B_I2C_masterSendMultiByteFinishWithTimeout (uint16_t baseAddress,
    uint8_t txData,
    uint32_t timeout
    )
{
    uint32_t timeout2 = timeout;

    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE)){
        //Poll for transmit interrupt flag.
        while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) && --timeout) ;

        //Check if transfer timed out
        if (timeout == 0){
            return (STATUS_FAIL);
        }
    }

    //Send single byte data.
    HWREG8(baseAddress + OFS_UCBxTXBUF) = txData;

    //Poll for transmit interrupt flag.
    while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) && --timeout2) ;

    //Check if transfer timed out
    if (timeout2 == 0){
        return (STATUS_FAIL);
    }

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    return(STATUS_SUCCESS);
}

void USCI_B_I2C_masterSendMultiByteStop (uint16_t baseAddress)
{
    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE)){
        //Poll for transmit interrupt flag.
        while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) ;
    }

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
}

bool USCI_B_I2C_masterSendMultiByteStopWithTimeout (uint16_t baseAddress,
    uint32_t timeout)
{
    //If interrupts are not used, poll for flags
    if (!(HWREG8(baseAddress + OFS_UCBxIE) & UCTXIE)){
        //Poll for transmit interrupt flag.
        while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCTXIFG)) && --timeout) ;

        //Check if transfer timed out
        if (timeout == 0){
            return (STATUS_FAIL);
        }
    }

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    return (STATUS_SUCCESS);
}

void USCI_B_I2C_masterReceiveMultiByteStart (uint16_t baseAddress)
{
    //Set USCI in Receive mode
    HWREG8(baseAddress + OFS_UCBxCTL1) &= ~UCTR;
    //Send start
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTT;
}

uint8_t USCI_B_I2C_masterReceiveMultiByteNext (uint16_t baseAddress)
{
    return (HWREG8(baseAddress + OFS_UCBxRXBUF));
}

uint8_t USCI_B_I2C_masterReceiveMultiByteFinish (uint16_t baseAddress)
{
    uint8_t receiveData;

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    //Capture data from receive buffer after setting stop bit due to
    //MSP430 I2C critical timing.
    receiveData = HWREG8(baseAddress + OFS_UCBxRXBUF);

    //Wait for Stop to finish
    while (HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTP);

    //Wait for RX buffer
    while (!(HWREG8(baseAddress + OFS_UCBxIFG) & UCRXIFG));

    return receiveData;
}

bool USCI_B_I2C_masterReceiveMultiByteFinishWithTimeout (uint16_t baseAddress,
    uint8_t *rxData,
    uint32_t timeout
    )
{
    uint32_t timeout2 = timeout;

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    //Capture data from receive buffer after setting stop bit due to
    //MSP430 I2C critical timing.
    *rxData = (HWREG8(baseAddress + OFS_UCBxRXBUF));

    //Wait for Stop to finish
    while ((HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTP) && --timeout);

    //Check if transfer timed out
    if (timeout == 0){
        return (STATUS_FAIL);
    }

    // Wait for RX buffer
    while ((!(HWREG8(baseAddress + OFS_UCBxIFG) & UCRXIFG)) && --timeout2) ;

    //Check if transfer timed out
    if (timeout2 == 0){
        return (STATUS_FAIL);
    }

    return (STATUS_SUCCESS);
}

void USCI_B_I2C_masterReceiveMultiByteStop (uint16_t baseAddress)
{
    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;
}

void USCI_B_I2C_masterReceiveSingleStart (uint16_t baseAddress)
{
    //local variable to store GIE status
    uint16_t gieStatus;

    //Store current SR register
    gieStatus = __get_SR_register() & GIE;

    //Disable global interrupt
    __disable_interrupt();

    //Set USCI in Receive mode
    HWREG8(baseAddress + OFS_UCBxCTL1) &= ~UCTR;

    //Send start condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTT;

    //Poll for Start bit to complete
    while (HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTT) ;

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    //Reinstate SR register
    __bis_SR_register(gieStatus);
}

bool USCI_B_I2C_masterReceiveSingleStartWithTimeout (uint16_t baseAddress,
    uint32_t timeout
    )
{
    //local variable to store GIE status
    uint16_t gieStatus;

    //Store current SR register
    gieStatus = __get_SR_register() & GIE;

    //Disable global interrupt
    __disable_interrupt();

    //Set USCI in Receive mode
    HWREG8(baseAddress + OFS_UCBxCTL1) &= ~UCTR;

    //Send start condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTT;

    //Poll for Start bit to complete
    while (((HWREG8(baseAddress + OFS_UCBxCTL1) & UCTXSTT)) && --timeout);

    //Check if transfer timed out
    if (timeout == 0){
        return (STATUS_FAIL);
    }

    //Send stop condition.
    HWREG8(baseAddress + OFS_UCBxCTL1) |= UCTXSTP;

    //Reinstate SR register
    __bis_SR_register(gieStatus);

    return (STATUS_SUCCESS);
}

uint8_t USCI_B_I2C_masterReceiveSingle (uint16_t baseAddress)
{
    //Polling RXIFG0 if RXIE is not enabled
    if(!(HWREG8(baseAddress + OFS_UCBxIE) & UCRXIE)) {
        while(!(HWREG8(baseAddress + OFS_UCBxIFG) & UCRXIFG));
    }

    //Read a byte.
    return (HWREG8(baseAddress + OFS_UCBxRXBUF));
}

uint32_t USCI_B_I2C_getReceiveBufferAddressForDMA (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCBxRXBUF );
}

uint32_t USCI_B_I2C_getTransmitBufferAddressForDMA (uint16_t baseAddress)
{
    return ( baseAddress + OFS_UCBxTXBUF );
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for usci_b_i2c_api
//! @}
//
//*****************************************************************************
