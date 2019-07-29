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
// ctsd16.c - Driver for the ctsd16 Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup ctsd16_api ctsd16
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_CTSD16__
#include "ctsd16.h"

#include <assert.h>

void CTSD16_init(uint16_t baseAddress, uint16_t chargePumpBurstRequest, 
    uint8_t referenceSelect)
{
    // Reset all interrupts and flags
    HWREG16(baseAddress + OFS_CTSD16IE)   &= 0x0000; //Reset ALL interrupt enables
    HWREG16(baseAddress + OFS_CTSD16IFG)  &= 0x0000; //Reset ALL interrupt flags

    // Configure CTSD16
    HWREG16(baseAddress + OFS_CTSD16CTL) &= ~(CTSD16RRIBURST | CTSD16OFFG |
        CTSD16REFS);

    HWREG16(baseAddress + OFS_CTSD16CTL) |= chargePumpBurstRequest | referenceSelect;

    return;
}

void CTSD16_initConverter(uint16_t baseAddress, uint8_t converter, 
    uint16_t conversionMode, uint8_t inputChannel)
{
    uint16_t address;
    address = baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x06));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(CTSD16SC | CTSD16SNGL);

    HWREG16(address) |= conversionMode;

    address += 0x2;

    HWREG16(address) &= ~(CTSD16INCH4 | CTSD16INCH3 | CTSD16INCH2 |
            CTSD16INCH1 | CTSD16INCH0);

    HWREG16(address) |= inputChannel;
}

void CTSD16_initConverterAdvanced(uint16_t baseAddress,
    CTSD16_initConverterAdvancedParam *param)
{
    uint16_t address;

    // Getting correct CTSD16CCTLx register
    address = baseAddress + (OFS_CTSD16CCTL0 + (param->converter * 0x06));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(CTSD16SC | CTSD16SNGL | CTSD16GRP | CTSD16OSR__32 |
        CTSD16DF);

    HWREG16(address) |= (param->groupEnable | param->conversionMode | param->oversampleRatio | param->dataFormat);

    // Getting correct CTSD16INCTLx register
    address += 0x2;

    HWREG16(address) &= ~(CTSD16RRI | CTSD16INTDLY | CTSD16GAIN2 |
        CTSD16GAIN1 | CTSD16GAIN0 | CTSD16INCH4 | CTSD16INCH3 | CTSD16INCH2 |
        CTSD16INCH1 | CTSD16INCH0);

    HWREG16(address) |= (param->railToRailInput | param->interruptDelay |
        param->gain | param->inputChannel);

}

void CTSD16_startConverterConversion(uint16_t baseAddress,
        uint8_t converter)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x06));

    // Setting CTSD16SC bit to start conversion
    HWREG16(address) |= CTSD16SC;
}

void CTSD16_stopConverterConversion(uint16_t baseAddress,
        uint8_t converter)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x06));

    // Setting CTSD16SC bit to start conversion
    HWREG16(address) &= ~(CTSD16SC);
}

void CTSD16_setConverterDataFormat(uint16_t baseAddress,
        uint8_t converter,
        uint8_t dataFormat)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x06));

    // Clearing previous settings for configuration
    HWREG8(address) &= ~CTSD16DF;

    HWREG8(address) |= dataFormat;
}

void CTSD16_setInputChannel(uint16_t baseAddress,
        uint8_t converter,
        uint8_t inputChannel)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16INCTL0 + (converter * 0x06));

    // Clear previous settings
    HWREG16(address) &= ~(CTSD16INCH4 | CTSD16INCH3 | CTSD16INCH2 |
            CTSD16INCH1 | CTSD16INCH0);

    HWREG16(address) |= inputChannel;
}

void CTSD16_setRailToRailInput(uint16_t baseAddress,
        uint8_t converter,
        uint16_t railToRailInput)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16INCTL0 + (converter * 0x06));

    // Clear previous settings
    HWREG16(address) &= ~CTSD16RRI;

    HWREG16(address) |= railToRailInput;
}

void CTSD16_setInterruptDelay(uint16_t baseAddress,
        uint8_t converter,
        uint16_t interruptDelay)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16INCTL0 + (converter * 0x06));

    // Clear previous settings
    HWREG16(address) &= ~CTSD16INTDLY;

    HWREG16(address) |= interruptDelay;

}

void CTSD16_setOversampling(uint16_t baseAddress,
        uint8_t converter,
        uint16_t oversampleRatio)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x06));

    // Setting CTSD16SC bit to start conversion
    HWREG16(address) |= oversampleRatio;
}

void CTSD16_setGain(uint16_t baseAddress,
        uint8_t converter,
        uint8_t gain)
{
    uint16_t address;

    address = baseAddress + (OFS_CTSD16INCTL0 + (converter * 0x06));

    // Clear previous settings
    HWREG16(address) &= ~(CTSD16GAIN2 | CTSD16GAIN1 | CTSD16GAIN0);

    HWREG16(address) |= gain;
}

uint32_t CTSD16_getResults(uint16_t baseAddress,
                            uint8_t converter)
{
    volatile uint16_t OSR;

    uint16_t address = baseAddress + (OFS_CTSD16MEM0 + (converter * 0x02));

    // Get high word result
    HWREG16(baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x02))) &= ~(CTSD16LSBACC);
    uint32_t highResult = (uint32_t)HWREG16(address);

    // Get low word result
    HWREG16(baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x02))) |= CTSD16LSBACC;
    uint16_t lowResult = HWREG16(address);
    HWREG16(baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x02))) &= ~(CTSD16LSBACC);

    // Determine the OSR and combine the high and low result words as appropriate
    OSR = HWREG16(baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x02)))  & (CTSD16OSR0 | CTSD16OSR1);

    if(OSR == CTSD16_OVERSAMPLE_256)
    {
        return (highResult << 8) | lowResult;
    }
    else if(OSR == CTSD16_OVERSAMPLE_128)
    {
        return (highResult << 5) | lowResult;
    }
    else if(OSR == CTSD16_OVERSAMPLE_64)
    {
        return (highResult << 2) | lowResult;
    }
    else        // OSR = CTSD16_OVERSAMPLE_32
    {
        return (highResult);
    }
}

uint16_t CTSD16_getHighWordResults(uint16_t baseAddress,
                                    uint8_t converter)
{
    // Calculate address of MEM results
    uint16_t address = baseAddress + (OFS_CTSD16MEM0 + (converter * 0x02));

    // Get high word result
    HWREG16(baseAddress + (OFS_CTSD16CCTL0 + (converter * 0x02))) &= ~(CTSD16LSBACC);
    uint16_t highResult = HWREG16(address);

    return(highResult);
}

uint16_t CTSD16_isRailToRailInputReady(uint16_t baseAddress)
{
    return HWREG16(baseAddress + OFS_CTSD16CTL) & CTSD16RRIRDY;
}

void CTSD16_enableInterrupt (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    //Enable Interrupt
    HWREG16(baseAddress + OFS_CTSD16IE) |= (mask << converter);

}

void CTSD16_disableInterrupt (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    //Enable Interrupt
    HWREG16(baseAddress + OFS_CTSD16IE) &= ~(mask << converter);

}

void CTSD16_clearInterrupt (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    HWREG16(baseAddress + OFS_CTSD16IFG) &= ~(mask << converter);
}

uint16_t CTSD16_getInterruptStatus (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    return ( HWREG16(baseAddress + OFS_CTSD16IFG) & (mask << converter) );
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for ctsd16_api
//! @}
//
//*****************************************************************************
