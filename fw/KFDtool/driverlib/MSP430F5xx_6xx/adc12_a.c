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
// adc12_a.c - Driver for the adc12_a Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup adc12_a_api adc12_a
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_ADC12_PLUS__
#include "adc12_a.h"

#include <assert.h>

bool ADC12_A_init (uint16_t baseAddress,
    uint16_t sampleHoldSignalSourceSelect,
    uint8_t clockSourceSelect,
    uint16_t clockSourceDivider)
{
    //Make sure the ENC bit is cleared before initializing the ADC12_A
    HWREG8(baseAddress + OFS_ADC12CTL0_L) &= ~ADC12ENC;

    bool retVal = STATUS_SUCCESS;

    //Turn OFF ADC12_A Module & Clear Interrupt Registers
    HWREG16(baseAddress + OFS_ADC12CTL0) &= ~(ADC12ON + ADC12OVIE + ADC12TOVIE
                                            + ADC12ENC + ADC12SC);
    HWREG16(baseAddress + OFS_ADC12IE)   &= 0x0000; //Reset ALL interrupt enables
    HWREG16(baseAddress + OFS_ADC12IFG)  &= 0x0000; //Reset ALL interrupt flags

    //Set ADC12_A Control 1
    HWREG16(baseAddress + OFS_ADC12CTL1) =
        sampleHoldSignalSourceSelect //Setup the Sample-and-Hold Source
        + (clockSourceDivider & ADC12DIV_7) //Set Clock Divider
        + clockSourceSelect; //Setup Clock Source

    //Set ADC12_A Control 2
    HWREG16(baseAddress + OFS_ADC12CTL2) =
        (clockSourceDivider & ADC12PDIV) //Set Clock Pre-Divider
        + ADC12RES_2; //Default resolution to 12-bits

    return ( retVal) ;
}

void ADC12_A_enable (uint16_t baseAddress)
{
    //Enable the ADC12_A Module
    HWREG8(baseAddress + OFS_ADC12CTL0_L) |= ADC12ON;
}

void ADC12_A_disable (uint16_t baseAddress)
{
    //Disable ADC12_A module
    HWREG8(baseAddress + OFS_ADC12CTL0_L) &= ~ADC12ON;
}

void ADC12_A_setupSamplingTimer (uint16_t baseAddress,
    uint16_t clockCycleHoldCountLowMem,
    uint16_t clockCycleHoldCountHighMem,
    uint16_t multipleSamplesEnabled)
{
    HWREG16(baseAddress + OFS_ADC12CTL1) |= ADC12SHP;

    //Reset clock cycle hold counts and msc bit before setting them
    HWREG16(baseAddress + OFS_ADC12CTL0) &=
        ~(ADC12SHT0_15 + ADC12SHT1_15 + ADC12MSC);

    //Set clock cycle hold counts and msc bit
    HWREG16(baseAddress + OFS_ADC12CTL0) |= clockCycleHoldCountLowMem
                                          + (clockCycleHoldCountHighMem << 4)
                                          + multipleSamplesEnabled;
}


void ADC12_A_disableSamplingTimer (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_ADC12CTL1) &= ~(ADC12SHP);
}


void ADC12_A_configureMemory(uint16_t baseAddress,
    ADC12_A_configureMemoryParam *param)
{
    //Make sure the ENC bit is cleared before configuring a Memory Buffer Control
    assert( !(HWREG16(baseAddress + OFS_ADC12CTL0) & ADC12ENC) );

    if(!(HWREG16(baseAddress + OFS_ADC12CTL0) & ADC12ENC))
    {
        //Set the offset in respect to ADC12MCTL0
        uint16_t memoryBufferControlOffset =
            (OFS_ADC12MCTL0 + param->memoryBufferControlIndex);

        //Reset the memory buffer control and Set the input source
        HWREG8(baseAddress + memoryBufferControlOffset) =
            param->inputSourceSelect //Set Input Source
            + param->positiveRefVoltageSourceSelect //Set Vref+
            + param->negativeRefVoltageSourceSelect //Set Vref-
            + param->endOfSequence; //Set End of Sequence
    }
}
void ADC12_A_enableInterrupt (uint16_t baseAddress,
    uint32_t interruptMask)
{
    if (interruptMask & ADC12_A_CONVERSION_TIME_OVERFLOW_IE) {
        HWREG16(baseAddress + OFS_ADC12CTL0) |= ADC12TOVIE;
        interruptMask &= ~ADC12_A_CONVERSION_TIME_OVERFLOW_IE;
    }
    if (interruptMask & ADC12_A_OVERFLOW_IE) {
        HWREG16(baseAddress + OFS_ADC12CTL0) |= ADC12OVIE;
        interruptMask &= ~ADC12_A_OVERFLOW_IE;
    }

    HWREG16(baseAddress + OFS_ADC12IE) |= interruptMask;
}

void ADC12_A_disableInterrupt (uint16_t baseAddress,
    uint32_t interruptMask)
{
    if (interruptMask & ADC12_A_CONVERSION_TIME_OVERFLOW_IE) {
        HWREG16(baseAddress + OFS_ADC12CTL0) &= ~(ADC12TOVIE);
        interruptMask &= ~ADC12_A_CONVERSION_TIME_OVERFLOW_IE;
    }
    if (interruptMask & ADC12_A_OVERFLOW_IE) {
        HWREG16(baseAddress + OFS_ADC12CTL0) &= ~(ADC12OVIE);
        interruptMask &= ~ADC12_A_OVERFLOW_IE;
    }

    HWREG16(baseAddress + OFS_ADC12IE) &= ~(interruptMask);
}

void ADC12_A_clearInterrupt (uint16_t baseAddress,
    uint16_t memoryInterruptFlagMask)
{
    HWREG16(baseAddress + OFS_ADC12IFG) &= ~(memoryInterruptFlagMask);
}

uint16_t ADC12_A_getInterruptStatus (uint16_t baseAddress,
    uint16_t memoryInterruptFlagMask)
{
    return ( HWREG16(baseAddress + OFS_ADC12IFG) & memoryInterruptFlagMask );
}

void ADC12_A_startConversion (uint16_t baseAddress,
    uint16_t startingMemoryBufferIndex,
    uint8_t conversionSequenceModeSelect)
{
    //Reset the ENC bit to set the starting memory address and conversion mode
    //sequence
    HWREG8(baseAddress + OFS_ADC12CTL0_L) &= ~(ADC12ENC);
    //Reset the bits about to be set
    HWREG16(baseAddress + OFS_ADC12CTL1) &= ~(ADC12CSTARTADD_15 + ADC12CONSEQ_3);

    HWREG8(baseAddress + OFS_ADC12CTL1_H) |= (startingMemoryBufferIndex << 4);
    HWREG8(baseAddress + OFS_ADC12CTL1_L) |= conversionSequenceModeSelect;
    HWREG8(baseAddress + OFS_ADC12CTL0_L) |= ADC12ENC + ADC12SC;
}

void ADC12_A_disableConversions (uint16_t baseAddress, bool preempt)
{
    if (ADC12_A_PREEMPTCONVERSION == preempt) {
        HWREG8(baseAddress + OFS_ADC12CTL1_L) &= ~(ADC12CONSEQ_3);
        //Reset conversion sequence mode to single-channel, single-conversion
    } 
    else if (~(HWREG8(baseAddress + OFS_ADC12CTL1_L) & ADC12CONSEQ_3)) {
        //To prevent preemoption of a single-channel, single-conversion we must
        //wait for the ADC core to finish the conversion.
        while (ADC12_A_isBusy(baseAddress)) ;
    }

    HWREG8(baseAddress + OFS_ADC12CTL0_L) &= ~(ADC12ENC);
}

uint16_t ADC12_A_getResults (uint16_t baseAddress, uint8_t memoryBufferIndex)
{
    //(0x20 + (memoryBufferIndex * 2)) == offset of ADC12MEMx
    return ( HWREG16(baseAddress + (0x20 + (memoryBufferIndex * 2))) );
}

void ADC12_A_setResolution (uint16_t baseAddress,
    uint8_t resolutionSelect)
{
    HWREG8(baseAddress + OFS_ADC12CTL2_L) &= ~(ADC12RES_3);
    HWREG8(baseAddress + OFS_ADC12CTL2_L) |= resolutionSelect;
}

void ADC12_A_setSampleHoldSignalInversion (uint16_t baseAddress,
    uint16_t invertedSignal)
{
    HWREG16(baseAddress + OFS_ADC12CTL1) &= ~(ADC12ISSH);
    HWREG16(baseAddress + OFS_ADC12CTL1) |= invertedSignal;
}

void ADC12_A_setDataReadBackFormat (uint16_t baseAddress,
    uint8_t readBackFormat)
{
    HWREG8(baseAddress + OFS_ADC12CTL2_L) &= ~(ADC12DF);
    HWREG8(baseAddress + OFS_ADC12CTL2_L) |= readBackFormat;
}

void ADC12_A_enableReferenceBurst (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_ADC12CTL2_L) |= ADC12REFBURST;
}

void ADC12_A_disableReferenceBurst (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_ADC12CTL2_L) &= ~(ADC12REFBURST);
}

void ADC12_A_setReferenceBufferSamplingRate (uint16_t baseAddress,
    uint8_t samplingRateSelect)
{
    HWREG8(baseAddress + OFS_ADC12CTL2_L) &= ~(ADC12SR);
    HWREG8(baseAddress + OFS_ADC12CTL2_L) |= samplingRateSelect;
}

uint32_t ADC12_A_getMemoryAddressForDMA (uint16_t baseAddress,
    uint8_t memoryIndex)
{
    return ( baseAddress + (0x20 + (memoryIndex * 2)) );
}

uint16_t ADC12_A_isBusy (uint16_t baseAddress)
{
    return (HWREG8(baseAddress + OFS_ADC12CTL1_L) & ADC12BUSY);
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for adc12_a_api
//! @}
//
//*****************************************************************************
