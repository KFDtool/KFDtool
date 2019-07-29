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
// lcd_b.c - Driver for the lcd_b Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup lcd_b_api lcd_b
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_LCD_B__
#include "lcd_b.h"

#include <assert.h>

//*****************************************************************************
//
// Initialization parameter instance
//
//*****************************************************************************
const LCD_B_initParam LCD_B_INIT_PARAM = {
        LCD_B_CLOCKSOURCE_ACLK,
        LCD_B_CLOCKDIVIDER_1,
        LCD_B_CLOCKPRESCALAR_1,
        LCD_B_STATIC,
        LCD_B_SEGMENTS_DISABLED
};

static void setLCDFunction(uint16_t baseAddress, uint8_t index, uint16_t value)
{
    switch (index){
    case 0:
        HWREG16(baseAddress + OFS_LCDBPCTL0) |= value;
        break;
    case 1:
        HWREG16(baseAddress + OFS_LCDBPCTL1) |= value;
        break;
    case 2:
        HWREG16(baseAddress + OFS_LCDBPCTL2) |= value;
        break;
    case 3:
        HWREG16(baseAddress + OFS_LCDBPCTL3) |= value;
        break;
    default:break;
    }
}

void LCD_B_init(uint16_t baseAddress, LCD_B_initParam *initParams)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~(LCDMX0 | LCDMX1 | LCDSSEL | LCDSON
      | LCDDIV_31);

    HWREG16(baseAddress + OFS_LCDBCTL0) |= initParams->muxRate;
    HWREG16(baseAddress + OFS_LCDBCTL0) |= initParams->clockSource;
    HWREG16(baseAddress + OFS_LCDBCTL0) |= initParams->segments;
    HWREG16(baseAddress + OFS_LCDBCTL0) |= initParams->clockDivider;
    HWREG16(baseAddress + OFS_LCDBCTL0) |= initParams->clockPrescalar;
}

void LCD_B_on(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) |= LCDON;
}

void LCD_B_off(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
}

void LCD_B_clearInterrupt(uint16_t baseAddress, uint16_t mask)
{
    HWREG8(baseAddress + OFS_LCDBCTL1_L) &= ~(mask>>8);
}

uint16_t LCD_B_getInterruptStatus(uint16_t baseAddress, uint16_t mask)
{
    return (HWREG8(baseAddress + OFS_LCDBCTL1_L) & (mask>>8));
}

void LCD_B_enableInterrupt (uint16_t baseAddress, uint16_t mask)
{
    HWREG16(baseAddress + OFS_LCDBCTL1) |= mask;
}

void LCD_B_disableInterrupt (uint16_t baseAddress, uint16_t mask)
{
    HWREG16(baseAddress + OFS_LCDBCTL1) &= ~mask;
}

void LCD_B_clearAllMemory(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDBMEMCTL) |= LCDCLRM;
}
 
void LCD_B_clearAllBlinkingMemory(uint16_t baseAddress)
{
   HWREG16(baseAddress + OFS_LCDBMEMCTL) |= LCDCLRBM;
}

void LCD_B_selectDisplayMemory(uint16_t baseAddress, uint16_t displayMemory)
{
    HWREG16(baseAddress + OFS_LCDBMEMCTL) &= ~LCDDISP;
    HWREG16(baseAddress + OFS_LCDBMEMCTL) |= displayMemory;
}

void LCD_B_setBlinkingControl (uint16_t baseAddress,
                          uint8_t clockDivider,
                          uint8_t clockPrescalar,
                          uint8_t mode)
{
    HWREG16(baseAddress + OFS_LCDBBLKCTL) &= ~(LCDBLKDIV0 | LCDBLKDIV1 | LCDBLKDIV2 
      | LCDBLKPRE0 | LCDBLKPRE1 | LCDBLKPRE2 | LCDBLKMOD0 | LCDBLKMOD1);
    
    HWREG16(baseAddress + OFS_LCDBBLKCTL) |= clockDivider | clockPrescalar | mode;
}                            

void LCD_B_enableChargePump(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDBVCTL) |= LCDCPEN;
}

void LCD_B_disableChargePump(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~LCDCPEN;
}

void LCD_B_selectBias(uint16_t baseAddress, uint16_t bias)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~LCD2B;

    HWREG16(baseAddress + OFS_LCDBVCTL) |= bias;
}

void LCD_B_selectChargePumpReference(uint16_t baseAddress, uint16_t reference)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~VLCDREF_3;

    HWREG16(baseAddress + OFS_LCDBVCTL) |= reference;
}

void LCD_B_setVLCDSource(uint16_t baseAddress,
    uint16_t vlcdSource,
    uint16_t v2v3v4Source,
    uint16_t v5Source)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~VLCDEXT;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~LCDREXT;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~LCDEXTBIAS;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~R03EXT;

    HWREG16(baseAddress + OFS_LCDBVCTL) |= vlcdSource;
    HWREG16(baseAddress + OFS_LCDBVCTL) |= v2v3v4Source;
    HWREG16(baseAddress + OFS_LCDBVCTL) |= v5Source;
}

void LCD_B_setVLCDVoltage(uint16_t baseAddress,
    uint16_t voltage)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;
    HWREG16(baseAddress + OFS_LCDBVCTL) &= ~VLCD_15;

    HWREG16(baseAddress + OFS_LCDBVCTL) |= voltage;
}

void LCD_B_setPinAsLCDFunction (uint16_t baseAddress, uint8_t pin)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;

    uint8_t idx = pin>>4;
    uint16_t val = 1<<(pin & 0xF);

    setLCDFunction(baseAddress, idx, val);

}    

void LCD_B_setPinAsPortFunction (uint16_t baseAddress, uint8_t pin)
{
    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;

    uint8_t idx = pin>>4;
    uint16_t val = 1<<(pin & 0xF);

    switch (idx){
        case 0:
            HWREG16(baseAddress + OFS_LCDBPCTL0) &= ~val;
            break;
        case 1:
            HWREG16(baseAddress + OFS_LCDBPCTL1) &= ~val;
            break;
        case 2:
            HWREG16(baseAddress + OFS_LCDBPCTL2) &= ~val;
            break;
        case 3:
            HWREG16(baseAddress + OFS_LCDBPCTL3) &= ~val;
            break;
        default:break;
    }

}

void LCD_B_setPinAsLCDFunctionEx(uint16_t baseAddress,
    uint8_t startPin,
    uint8_t endPin)
{
    uint8_t startIdx = startPin>>4;
    uint8_t endIdx = endPin>>4;
    uint8_t startPos = startPin & 0xF;
    uint8_t endPos = endPin & 0xF;
    uint16_t val = 0;
    uint8_t i = 0;

    HWREG16(baseAddress + OFS_LCDBCTL0) &= ~LCDON;

    if (startIdx == endIdx) {
        val = (0xFFFF>>(15-endPos)) & (0xFFFF<<startPos);

        setLCDFunction(baseAddress, startIdx, val);

    }
    else {
        val = 0xFFFF>>(15-endPos);
        setLCDFunction(baseAddress, endIdx, val);

        for (i=endIdx-1; i>startIdx; i--) {
            setLCDFunction(baseAddress, i, 0xFFFF);
        }

        val = 0xFFFF<<startPos;
        setLCDFunction(baseAddress, startIdx, val);
    }
}

void LCD_B_setMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{    
    HWREG8(baseAddress + OFS_LCDM1 + memory) = mask;
}

void LCD_B_updateMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDM1 + memory) |= mask;
}

void LCD_B_toggleMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDM1 + memory) ^= mask;
}

void LCD_B_clearMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{
    HWREG8(baseAddress + OFS_LCDM1 + memory) &= ~mask;
}

void LCD_B_setBlinkingMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{    
    HWREG8(baseAddress + OFS_LCDBM1 + memory) = mask;
}

void LCD_B_updateBlinkingMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{    
    HWREG8(baseAddress + OFS_LCDBM1 + memory) |= mask;
}

void LCD_B_toggleBlinkingMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{    
    HWREG8(baseAddress + OFS_LCDBM1 + memory) ^= mask;
}

void LCD_B_clearBlinkingMemory(uint16_t baseAddress, uint8_t memory, uint8_t mask)
{    
    HWREG8(baseAddress + OFS_LCDBM1 + memory) &= ~mask;
}

void LCD_B_configureChargePump(uint16_t baseAddress, uint16_t autoDisabled)
{
    HWREG16(baseAddress + OFS_LCDBCPCTL) &= ~(LCDCPDIS7 | LCDCPDIS6 | LCDCPDIS5
        | LCDCPDIS4 | LCDCPDIS3 | LCDCPDIS2 | LCDCPDIS1 | LCDCPDIS0);

    HWREG16(baseAddress + OFS_LCDBCPCTL) |= autoDisabled;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for lcd_b_api
//! @}
//
//*****************************************************************************
