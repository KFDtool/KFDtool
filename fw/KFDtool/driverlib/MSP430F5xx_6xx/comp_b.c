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
// comp_b.c - Driver for the comp_b Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup comp_b_api comp_b
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_COMPB__
#include "comp_b.h"

#include <assert.h>

bool Comp_B_init(uint16_t baseAddress, Comp_B_initParam *param)
{
    bool retVal = STATUS_SUCCESS;

    //Reset COMPB Control 1 & Interrupt Registers for initialization (OFS_CBCTL3
    //is not reset because it controls the input buffers of the analog signals
    //and may cause parasitic effects if an analog signal is still attached and
    //the buffer is re-enabled
    HWREG16(baseAddress + OFS_CBCTL0) &= 0x0000;
    HWREG16(baseAddress + OFS_CBINT)  &= 0x0000;

    //Clear reference voltage and reference source
    HWREG16(baseAddress + OFS_CBCTL2) &= ~(CBRS_3 | CBREFL_3);

    //Set the Positive Terminal
    if(COMP_B_VREF != param->positiveTerminalInput) {
        //Enable Positive Terminal Input Mux and Set it to the appropriate input
        HWREG16(baseAddress + OFS_CBCTL0) |= CBIPEN + param->positiveTerminalInput;

        //Disable the input buffer
        HWREG16(baseAddress + OFS_CBCTL3) |= (1 << param->positiveTerminalInput);
    }
    else {
        //Reset and Set COMPB Control 2 Register
        //Set Vref to go to (+)terminal
        HWREG16(baseAddress + OFS_CBCTL2) &= ~(CBRSEL);
    }

    //Set the Negative Terminal
    if (COMP_B_VREF != param->negativeTerminalInput) {
        //Enable Negative Terminal Input Mux and Set it to the appropriate input
        HWREG16(baseAddress + OFS_CBCTL0) |= CBIMEN + (param->negativeTerminalInput << 8);

        //Disable the input buffer
        HWREG16(baseAddress + OFS_CBCTL3) |= (1 << param->negativeTerminalInput);
    }
    else {
        //Reset and Set COMPB Control 2 Register
        //Set Vref to go to (-) terminal
        HWREG16(baseAddress + OFS_CBCTL2) |= CBRSEL;
    }

    //Reset and Set COMPB Control 1 Register
    HWREG16(baseAddress + OFS_CBCTL1) =
        param->powerModeSelect //Set the power mode
        + param->outputFilterEnableAndDelayLevel //Set the filter enable bit and delay
        + param->invertedOutputPolarity; //Set the polarity of the output

    return (retVal);
}

void Comp_B_configureReferenceVoltage(uint16_t baseAddress,
    Comp_B_configureReferenceVoltageParam *param)
{
    //Set to VREF0
    HWREG16(baseAddress + OFS_CBCTL1) &= ~(CBMRVS);

    //Reset COMPB Control 2 Bits (Except for CBRSEL which is set in Comp_Init())
    HWREG16(baseAddress + OFS_CBCTL2) &= CBRSEL;

    //Set Voltage Source (Vcc | Vref, resistor ladder or not)
    if (COMP_B_VREFBASE_VCC == param->supplyVoltageReferenceBase) {
        HWREG16(baseAddress + OFS_CBCTL2) |= CBRS_1; //Vcc with resistor ladder
    }
    else if (param->lowerLimitSupplyVoltageFractionOf32 == 32) {
        //If the lower limit is 32, then the upper limit has to be 32 due to the
        //assertion that upper must be >= to the lower limit. If the numerator is
        //equal to 32, then the equation would be 32/32 == 1, therefore no resistor
        //ladder is needed
        HWREG16(baseAddress + OFS_CBCTL2) |= CBRS_3; //Vref, no resistor ladder
    }
    else {
        HWREG16(baseAddress + OFS_CBCTL2) |= CBRS_2; //Vref with resistor ladder
    }

    //Set COMPD Control 2 Register
    HWREG16(baseAddress + OFS_CBCTL2) |=
        param->supplyVoltageReferenceBase //Set Supply Voltage Base
        + ((param->upperLimitSupplyVoltageFractionOf32 - 1) << 8) //Set Supply Voltage Num.
        + (param->lowerLimitSupplyVoltageFractionOf32 - 1);

    HWREG16(baseAddress + OFS_CBCTL2) &= ~(CBREFACC);
    HWREG16(baseAddress + OFS_CBCTL2) |= param->referenceAccuracy;
}

void Comp_B_enableInterrupt(uint16_t baseAddress,
    uint16_t interruptMask)
{
    //Set the Interrupt enable bit
    HWREG16(baseAddress + OFS_CBINT) |= interruptMask;
}

void Comp_B_disableInterrupt(uint16_t baseAddress,
    uint16_t interruptMask)
{
    HWREG16(baseAddress + OFS_CBINT) &= ~(interruptMask);
}

void Comp_B_clearInterrupt(uint16_t baseAddress,
    uint16_t interruptFlagMask)
{
    HWREG16(baseAddress + OFS_CBINT) &= ~(interruptFlagMask);
}

uint8_t Comp_B_getInterruptStatus(uint16_t baseAddress,
    uint16_t interruptFlagMask)
{
    return(HWREG16(baseAddress + OFS_CBINT) & interruptFlagMask);
}

void Comp_B_setInterruptEdgeDirection(uint16_t baseAddress,
    uint16_t edgeDirection)
{
    //Set the edge direction that will trigger an interrupt
    if(COMP_B_RISINGEDGE == edgeDirection) {
        HWREG16(baseAddress + OFS_CBCTL1) &= ~(CBIES);
    }
    else if(COMP_B_FALLINGEDGE == edgeDirection) {
        HWREG16(baseAddress + OFS_CBCTL1) |= CBIES;
    }
}

void Comp_B_toggleInterruptEdgeDirection(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CBCTL1) ^= CBIES;
}

void Comp_B_enable(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CBCTL1) |= CBON;
}

void Comp_B_disable(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CBCTL1) &= ~(CBON);
}

void Comp_B_shortInputs(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CBCTL1) |= CBSHORT;
}

void Comp_B_unshortInputs(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CBCTL1) &= ~(CBSHORT);
}

void Comp_B_disableInputBuffer(uint16_t baseAddress,
    uint8_t inputPort)
{
    HWREG16(baseAddress + OFS_CBCTL3) |= (1 << inputPort);
}

void Comp_B_enableInputBuffer(uint16_t baseAddress,
    uint8_t inputPort)
{
    HWREG16(baseAddress + OFS_CBCTL3) &= ~(1 << inputPort);
}

void Comp_B_swapIO(uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_CBCTL1) ^= CBEX;
}

uint16_t Comp_B_outputValue(uint16_t baseAddress)
{
    return (HWREG16(baseAddress + OFS_CBCTL1) & CBOUT);

}

void Comp_B_selectReferenceVoltage(uint16_t baseAddress, uint16_t selectType,
    uint16_t selectVRef)
{
    HWREG16(baseAddress + OFS_CBCTL1) &= ~(CBMRVS | CBMRVL);
    if(selectType == COMP_B_VREF_MANUAL_SELECT) {
        HWREG16(baseAddress + OFS_CBCTL1) |= CBMRVS;
        if(selectVRef == COMP_B_SELECT_VREF1) {
            HWREG16(baseAddress + OFS_CBCTL1) |= CBMRVL;
        }
    }
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for comp_b_api
//! @}
//
//*****************************************************************************
