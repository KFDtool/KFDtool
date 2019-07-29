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
// dac12_a.c - Driver for the dac12_a Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup dac12_a_api dac12_a
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_DAC12_2__
#include "dac12_a.h"

#include <assert.h>

bool DAC12_A_init(uint16_t baseAddress, DAC12_A_initParam *param)
{
    baseAddress += param->submoduleSelect;    //Add 0x10 to base address IF
                                              //DAC12_A_1 is selected.
    HWREG16(baseAddress + OFS_DAC12_0CTL1) &= ~(DAC12OG + DAC12DFJ);

    //Reset and Set DAC12_A Control 0 Bits
    HWREG16(baseAddress + OFS_DAC12_0CTL0) = param->outputSelect
                                           + param->positiveReferenceVoltage
                                           + param->amplifierSetting
                                           + param->conversionTriggerSelect;

    if (DAC12_A_VREFx1 == param->outputVoltageMultiplier){
        HWREG16(baseAddress + OFS_DAC12_0CTL0) |= DAC12IR;
    } else if (DAC12_A_VREFx2 == param->outputVoltageMultiplier){
        HWREG16(baseAddress + OFS_DAC12_0CTL1) |= DAC12OG;
    }
    //else if(DAC12_A_VREFx3 == outputVoltageMultiplier)
    //Both DAC12IR and DAC12OG values == 0

    return ( STATUS_SUCCESS) ;
}void DAC12_A_setAmplifierSetting (uint16_t baseAddress,
    uint8_t submoduleSelect,
    uint8_t amplifierSetting)
{
    //Reset amplifier setting to set it
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) &= ~(DAC12AMP_7);
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) |= amplifierSetting;
}

void DAC12_A_disable (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
  //Reset amplifier setting to turn DAC12_A off completely
  HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) &= ~(DAC12AMP_7);
}

void DAC12_A_enableGrouping (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_DAC12_0CTL0) |= DAC12GRP;
}

void DAC12_A_disableGrouping (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_DAC12_0CTL0) &= ~(DAC12GRP);
}

void DAC12_A_enableInterrupt (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) |= DAC12IE;
}

void DAC12_A_disableInterrupt (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) &= ~(DAC12IE);
}

uint16_t DAC12_A_getInterruptStatus (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    return (HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) & DAC12IFG);
}

void DAC12_A_clearInterrupt (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) &= ~(DAC12IFG);
}

void DAC12_A_calibrateOutput (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    //Unlock Calibration
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CALCTL) = DAC12PW;

    //Start Calibration
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) |= DAC12CALON;

    //Wait for Calibration to Finish
    while (HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) & DAC12CALON);

    //Lock Calibration
    HWREG16(baseAddress + submoduleSelect +
        OFS_DAC12_0CALCTL) = DAC12PW + DAC12LOCK;
}

uint16_t DAC12_A_getCalibrationData (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    return ((uint16_t)(HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CALDAT))) ;
}

void DAC12_A_setCalibrationOffset (uint16_t baseAddress,
    uint8_t submoduleSelect,
    uint16_t calibrationOffsetValue)
{
    //Unlock Calibration
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CALCTL) = DAC12PW;

    //Set Calibration Offset
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CALDAT) =
        calibrationOffsetValue;

    //Lock Calibration
    HWREG16(baseAddress + submoduleSelect +
        OFS_DAC12_0CALCTL) = DAC12PW + DAC12LOCK;
}

void DAC12_A_enableConversions (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) |= DAC12ENC;
}

void DAC12_A_setData (uint16_t baseAddress,
    uint8_t submoduleSelect,
    uint16_t data)
{
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0DAT) = data;
}

void DAC12_A_disableConversions (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
    HWREG16(baseAddress + submoduleSelect + OFS_DAC12_0CTL0) &= ~(DAC12ENC);
}

void DAC12_A_setResolution (uint16_t baseAddress,
    uint8_t submoduleSelect,
    uint16_t resolutionSelect)
{
    //Store the ENC bit status
    uint16_t conversionsEnabledStatus =
        ( HWREG16(baseAddress + OFS_DAC12_0CTL0) & (DAC12ENC) );

    baseAddress += submoduleSelect;           //Add 0x10 to base address IF
                                              //DAC12_A_1 is selected.

    if (DAC12_A_RESOLUTION_8BIT == resolutionSelect){
        HWREG16(baseAddress + OFS_DAC12_0CTL0) |= DAC12RES;
    } else if (DAC12_A_RESOLUTION_12BIT == resolutionSelect){
        HWREG16(baseAddress + OFS_DAC12_0CTL0) &= ~(DAC12RES);
    }

    //Restore the ENC bit status
    HWREG16(baseAddress + OFS_DAC12_0CTL0) |= conversionsEnabledStatus;
}

void DAC12_A_setInputDataFormat (uint16_t baseAddress,
    uint8_t submoduleSelect,
    uint8_t inputJustification,
    uint8_t inputSign)
{
    //Store the ENC bit status
    uint16_t conversionsEnabledStatus =
        ( HWREG16(baseAddress + OFS_DAC12_0CTL0) & (DAC12ENC) );

    baseAddress += submoduleSelect;           //Add 0x10 to base address IF
                                              //DAC12_A_1 is selected.

    if (DAC12_A_JUSTIFICATION_LEFT == inputJustification){
        HWREG16(baseAddress + OFS_DAC12_0CTL1) |= DAC12DFJ;
    } else if (DAC12_A_JUSTIFICATION_RIGHT == inputJustification){
        HWREG16(baseAddress + OFS_DAC12_0CTL1) &= ~(DAC12DFJ);
    }

    if (DAC12_A_SIGNED_2SCOMPLEMENT == inputSign){
        HWREG16(baseAddress + OFS_DAC12_0CTL0) |= DAC12DF;
    } else if (DAC12_A_UNSIGNED_BINARY == inputSign){
        HWREG16(baseAddress + OFS_DAC12_0CTL0) &= ~(DAC12DF);
    }

    //Restore the ENC bit status
    HWREG16(baseAddress + OFS_DAC12_0CTL0) |= conversionsEnabledStatus;
}

uint32_t DAC12_A_getDataBufferMemoryAddressForDMA (uint16_t baseAddress,
    uint8_t submoduleSelect)
{
  return ( baseAddress + submoduleSelect + OFS_DAC12_0DAT );
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for dac12_a_api
//! @}
//
//*****************************************************************************
