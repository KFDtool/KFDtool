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
// sd24_b.c - Driver for the sd24_b Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup sd24_b_api sd24_b
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_SD24_B__
#include "sd24_b.h"

#include <assert.h>

void SD24_B_init(uint16_t baseAddress, SD24_B_initParam *param)
{
    // Reset all interrupts and flags
    HWREG16(baseAddress + OFS_SD24BIE)   &= 0x0000; //Reset ALL interrupt enables
    HWREG16(baseAddress + OFS_SD24BIFG)  &= 0x0000; //Reset ALL interrupt flags
    HWREG16(baseAddress + OFS_SD24BTRGCTL) &= ~(SD24TRGIE | SD24TRGIFG);

    // Turn off all group conversions
    HWREG16(baseAddress + OFS_SD24BCTL1) &= ~(SD24GRP0SC | SD24GRP1SC
            | SD24GRP2SC | SD24GRP3SC);

    // Configure SD24_B
    HWREG16(baseAddress + OFS_SD24BCTL0) &= ~((SD24DIV4 | SD24DIV3 | SD24DIV2
            | SD24DIV1 | SD24DIV0) | SD24PDIV_7 | SD24SSEL_3 | SD24REFS);
    HWREG16(baseAddress + OFS_SD24BCTL0) |= (param->clockSourceSelect |
        param->clockPreDivider | param->clockDivider | param->referenceSelect);

    return;
}

void SD24_B_initConverter(uint16_t baseAddress,
    SD24_B_initConverterParam *param)
{
    uint16_t address = baseAddress + (OFS_SD24BCCTL0 + (param->converter * 0x08));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(SD24ALGN | SD24SNGL | SD24SCS__GROUP3);

    HWREG16(address) |= (param->alignment | param->startSelect |
            (((uint16_t) param->conversionMode) << 8));
}

void SD24_B_initConverterAdvanced(uint16_t baseAddress,
    SD24_B_initConverterAdvancedParam *param)
{
    // Getting correct SD24BCCTLx register
    uint16_t address = baseAddress + (OFS_SD24BCCTL0 + (param->converter * 0x08));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(SD24ALGN | SD24SNGL | SD24DF1 | SD24DF0 | SD24SCS__GROUP3 );

    HWREG16(address) |= (param->alignment | param->startSelect | param->dataFormat |
            (((uint16_t) param->conversionMode) << 8));

    // Getting correct SDBINTCTLx register
    address = baseAddress + (OFS_SD24BINCTL0 + (param->converter * 0x08));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(SD24GAIN_128 | SD24INTDLY_3);

    HWREG16(address) |= (param->gain | param->sampleDelay);

    // Getting correct SDBOSRx register
    address = baseAddress + (OFS_SD24BOSR0 + (param->converter * 0x08));

    // Clearing previous settings for configuration
    HWREG16(address) &= ~(OSR10 | OSR9 | OSR8 | OSR7 | OSR6 | OSR5 | OSR4 |
            OSR3 | OSR2 | OSR1 | OSR0);

    HWREG16(address) |= param->oversampleRatio;
}
void SD24_B_setConverterDataFormat(uint16_t baseAddress,
        uint8_t converter,
        uint8_t dataFormat) {

    uint16_t address = baseAddress + (OFS_SD24BCCTL0_L +
            (converter * 0x08));
    // Clearing previous settings for configuration
    HWREG8(address) &= ~(SD24DF0 | SD24DF1);

    HWREG8(address) |= dataFormat;
}

void SD24_B_startGroupConversion(uint16_t baseAddress,
        uint8_t group)
{
    switch(group) {
    case SD24_B_GROUP0:
        HWREG16(baseAddress + OFS_SD24BCTL1) |= SD24GRP0SC; break;
    case SD24_B_GROUP1:
        HWREG16(baseAddress + OFS_SD24BCTL1) |= SD24GRP1SC; break;
    case SD24_B_GROUP2:
        HWREG16(baseAddress + OFS_SD24BCTL1) |= SD24GRP2SC; break;
    case SD24_B_GROUP3:
        HWREG16(baseAddress + OFS_SD24BCTL1) |= SD24GRP3SC; break;
    }
}

void SD24_B_stopGroupConversion(uint16_t baseAddress,
        uint8_t group)
{
    switch(group) {
    case SD24_B_GROUP0:
        HWREG16(baseAddress + OFS_SD24BCTL1) &= ~(SD24GRP0SC); break;
    case SD24_B_GROUP1:
        HWREG16(baseAddress + OFS_SD24BCTL1) &= ~(SD24GRP1SC); break;
    case SD24_B_GROUP2:
        HWREG16(baseAddress + OFS_SD24BCTL1) &= ~(SD24GRP2SC); break;
    case SD24_B_GROUP3:
        HWREG16(baseAddress + OFS_SD24BCTL1) &= ~(SD24GRP3SC); break;
    }
}

void SD24_B_startConverterConversion(uint16_t baseAddress,
        uint8_t converter)
{
    uint16_t address = baseAddress + (OFS_SD24BCCTL0 + (converter * 0x08));

    // Clearing trigger generation select
    HWREG16(address) &= ~(SD24SCS_7);

    // Setting SD24SC bit to start conversion
    HWREG16(address) |= SD24SC;
}

void SD24_B_stopConverterConversion(uint16_t baseAddress,
        uint8_t converter)
{
    uint16_t address = baseAddress + (OFS_SD24BCCTL0 + (converter * 0x08));

    // Clearing trigger generation select
    HWREG16(address) &= ~(SD24SCS_7);

    // Setting SD24SC bit to start conversion
    HWREG16(address) &= ~(SD24SC);
}

void SD24_B_configureDMATrigger(uint16_t baseAddress,
        uint16_t interruptFlag)
{
    // Clearing previous settings
    HWREG16(baseAddress + OFS_SD24BCTL1) &= ~(SD24DMA3 | SD24DMA2 |
            SD24DMA1 | SD24DMA0);

    HWREG16(baseAddress + OFS_SD24BCTL1) |= interruptFlag;
}

void SD24_B_setInterruptDelay(uint16_t baseAddress,
        uint8_t converter,
        uint8_t sampleDelay)
{
    uint16_t address = baseAddress + (OFS_SD24BINCTL0 + (converter * 0x08));

    // Clear previous settings
    HWREG16(address) &= ~(SD24INTDLY_3);

    HWREG16(address) |= sampleDelay;
}

void SD24_B_setConversionDelay(uint16_t baseAddress,
        uint8_t converter,
        uint16_t cycleDelay)
{
    uint16_t address = baseAddress + (OFS_SD24BPRE0 + (converter * 0x08));

    // Clear previous settings
    HWREG16(address) &= ~(0x3FF);

    HWREG16(address) |= cycleDelay;
}

void SD24_B_setOversampling(uint16_t baseAddress,
        uint8_t converter,
        uint16_t oversampleRatio)
{
    uint16_t address = baseAddress + (OFS_SD24BOSR0 + (converter * 0x08));

    // Clear previous settings
    HWREG16(address) &= ~(OSR10 | OSR9 | OSR8 | OSR7 | OSR6 | OSR5 | OSR4 |
            OSR3 | OSR2 | OSR1 | OSR0);

    HWREG16(address) |= oversampleRatio;
}

void SD24_B_setGain(uint16_t baseAddress,
        uint8_t converter,
        uint8_t gain)
{
    uint16_t address = baseAddress + (OFS_SD24BINCTL0 + (converter * 0x08));

    // Clear previous settings
    HWREG16(address) &= ~(SD24GAIN_128);

    HWREG16(address) |= gain;
}

uint32_t SD24_B_getResults(uint16_t baseAddress,
        uint8_t converter)
{
    // Calculating address to low word
    uint16_t address = baseAddress + (OFS_SD24BMEML0 + (converter * 0x04));

    // Getting low word result
    uint16_t lowResult = HWREG16(address);

    // Getting high word result and concatenate with low word
    uint32_t result = (((uint32_t) HWREG16(address + 0x02) ) << 16) + lowResult;

    return result;
}

uint16_t SD24_B_getHighWordResults(uint16_t baseAddress,
        uint8_t converter)
{
    // Calculating address
    uint16_t address = baseAddress + (OFS_SD24BMEMH0 + (converter * 0x04));

    // Getting high word result
    uint16_t result = HWREG16(address);

    return result;
}

void SD24_B_enableInterrupt (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    //Enable Interrupt
    HWREG16(baseAddress + OFS_SD24BIE) |= (mask << converter);
}

void SD24_B_disableInterrupt (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    HWREG16(baseAddress + OFS_SD24BIE) &= ~(mask << converter);
}

void SD24_B_clearInterrupt (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    HWREG16(baseAddress + OFS_SD24BIFG) &= ~(mask << converter);
}

uint16_t SD24_B_getInterruptStatus (uint16_t baseAddress,
    uint8_t converter,
    uint16_t mask)
{
    return ( HWREG16(baseAddress + OFS_SD24BIFG) & (mask << converter) );
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for sd24_b_api
//! @}
//
//*****************************************************************************
