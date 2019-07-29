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
// adc10_a.c - Driver for the adc10_a Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup adc10_a_api adc10_a
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_ADC10_A__
#include "adc10_a.h"

#include <assert.h>

bool ADC10_A_init (uint16_t baseAddress,
    uint16_t sampleHoldSignalSourceSelect,
    uint8_t clockSourceSelect,
    uint16_t clockSourceDivider)
{
    bool retVal = STATUS_SUCCESS;

    //Turn OFF ADC10_A Module & Clear Interrupt Registers
    HWREG16(baseAddress + OFS_ADC10IFG)  &= 0x0000; //Reset ALL interrupt flags

    //Set ADC10_A Control 1
    HWREG16(baseAddress + OFS_ADC10CTL1) =
        sampleHoldSignalSourceSelect //Setup the Sample-and-Hold Source
        + (clockSourceDivider & ADC10DIV_7) //Set Clock Divider
        + clockSourceSelect; //Setup Clock Source

    //Set ADC10_A Control 2
    HWREG16(baseAddress + OFS_ADC10CTL2) =
        (clockSourceDivider & (ADC10PDIV_1 | ADC10PDIV_2)) //Set Clock Pre-Divider
        + ADC10RES; //Default resolution to 10-bits

    return ( retVal) ;
}

void ADC10_A_enable (uint16_t baseAddress)
{
    //Reset the ADC10ON bit to enable the ADC10_A Module
    HWREG16(baseAddress + OFS_ADC10CTL0) |= ADC10ON;
}

void ADC10_A_disable (uint16_t baseAddress)
{
    //Set the ADC10ON bit to disable the ADC10_A Module
    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~ADC10ON;
}

void ADC10_A_setupSamplingTimer (uint16_t baseAddress,
    uint16_t clockCycleHoldCount,
    uint16_t multipleSamplesEnabled)
{
    HWREG16(baseAddress + OFS_ADC10CTL1) |= ADC10SHP;

    //Reset and Set CB Control 0 Bits
    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~(ADC10SHT_15 + ADC10MSC);
    HWREG16(baseAddress + OFS_ADC10CTL0) |= clockCycleHoldCount
                                          + multipleSamplesEnabled;
}

void ADC10_A_disableSamplingTimer (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_ADC10CTL1) &= ~(ADC10SHP);
}

void ADC10_A_configureMemory (uint16_t baseAddress,
    uint8_t inputSourceSelect,
    uint8_t positiveRefVoltageSourceSelect,
    uint8_t negativeRefVoltageSourceSelect)
{
    //Make sure the ENC bit is cleared before configuring a Memory Buffer Control
    assert( !(HWREG16(baseAddress + OFS_ADC10CTL0) & ADC10ENC) );

    if(!(HWREG16(baseAddress + OFS_ADC10CTL0) & ADC10ENC))
    {
        assert(inputSourceSelect <= ADC10_A_INPUT_A15);
        assert(positiveRefVoltageSourceSelect <= ADC10_A_VREFPOS_INT);
        assert(negativeRefVoltageSourceSelect <= ADC10_A_VREFNEG_EXT);
        
        //Reset and Set the Memory Buffer Control Bits
        HWREG8(baseAddress + OFS_ADC10MCTL0) = inputSourceSelect
                                               + positiveRefVoltageSourceSelect +
                                               negativeRefVoltageSourceSelect;
    }
}

void ADC10_A_enableInterrupt (uint16_t baseAddress,
    uint8_t interruptMask)
{
    HWREG16(baseAddress + OFS_ADC10IE) |= interruptMask;
}

void ADC10_A_disableInterrupt (uint16_t baseAddress,
    uint8_t interruptMask)
{
    HWREG16(baseAddress + OFS_ADC10IE) &= ~(interruptMask);
}

void ADC10_A_clearInterrupt (uint16_t baseAddress,
    uint8_t interruptFlagMask)
{
    HWREG16(baseAddress + OFS_ADC10IFG) &= ~(interruptFlagMask);
}

uint16_t ADC10_A_getInterruptStatus (uint16_t baseAddress,
    uint8_t interruptFlagMask)
{
    return ( HWREG16(baseAddress + OFS_ADC10IFG) & interruptFlagMask );
}

void ADC10_A_startConversion (uint16_t baseAddress,
    uint8_t conversionSequenceModeSelect)
{
    //Reset the ENC bit to set the conversion mode sequence
    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~(ADC10ENC);

    HWREG16(baseAddress + OFS_ADC10CTL1) |= conversionSequenceModeSelect;
    HWREG16(baseAddress + OFS_ADC10CTL0) |= ADC10ENC + ADC10SC;
}

void ADC10_A_disableConversions (uint16_t baseAddress, bool preempt)
{
    if (ADC10_A_PREEMPTCONVERSION == preempt){
        HWREG16(baseAddress + OFS_ADC10CTL1) &= ~(ADC10CONSEQ_3);
        //Reset conversion sequence mode to single-channel, single-conversion
    } else if ( ~(HWREG16(baseAddress + OFS_ADC10CTL1) & ADC10CONSEQ_3) ){
        //To prevent preemoption of a single-channel, single-conversion we must
        //wait for the ADC core to finish the conversion.
        while (HWREG16(baseAddress + OFS_ADC10CTL1) & ADC10BUSY) ;
    }

    HWREG16(baseAddress + OFS_ADC10CTL0) &= ~(ADC10ENC);
}

int16_t ADC10_A_getResults (uint16_t baseAddress)
{
    return ( HWREG16(baseAddress + OFS_ADC10MEM0) );
}

void ADC10_A_setResolution (uint16_t baseAddress,
    uint8_t resolutionSelect)
{
    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10RES);
    HWREG16(baseAddress + OFS_ADC10CTL2) |= resolutionSelect;
}

void ADC10_A_setSampleHoldSignalInversion (uint16_t baseAddress,
    uint16_t invertedSignal)
{
    HWREG16(baseAddress + OFS_ADC10CTL1) &= ~(ADC10ISSH);
    HWREG16(baseAddress + OFS_ADC10CTL1) |= invertedSignal;
}

void ADC10_A_setDataReadBackFormat (uint16_t baseAddress,
    uint16_t readBackFormat)
{
    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10DF);
    HWREG16(baseAddress + OFS_ADC10CTL2) |= readBackFormat;
}

void ADC10_A_enableReferenceBurst (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_ADC10CTL2) |= ADC10REFBURST;
}

void ADC10_A_disableReferenceBurst (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10REFBURST);
}

void ADC10_A_setReferenceBufferSamplingRate (uint16_t baseAddress,
    uint16_t samplingRateSelect)
{
    HWREG16(baseAddress + OFS_ADC10CTL2) &= ~(ADC10SR);
    HWREG16(baseAddress + OFS_ADC10CTL2) |= samplingRateSelect;
}

void ADC10_A_setWindowComp (uint16_t baseAddress,
    uint16_t highThreshold,
    uint16_t lowThreshold)
{
    HWREG16(baseAddress + OFS_ADC10HI) = highThreshold;
    HWREG16(baseAddress + OFS_ADC10LO) = lowThreshold;
}

uint32_t ADC10_A_getMemoryAddressForDMA (uint16_t baseAddress)
{
    return ( baseAddress + OFS_ADC10MEM0 );
}

uint16_t ADC10_A_isBusy (uint16_t baseAddress)
{
    return (HWREG16(baseAddress + OFS_ADC10CTL1) & ADC10BUSY);
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for adc10_a_api
//! @}
//
//*****************************************************************************
