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
// tec.c - Driver for the tec Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup tec_api tec
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_TEV0__
#include "tec.h"

#include <assert.h>

void TEC_initExternalClearInput (uint16_t baseAddress,
                                        uint8_t signalType,
                                        uint8_t signalHold,
                                        uint8_t polarityBit
                                      )
{
    HWREG8(baseAddress + OFS_TEC0XCTL2_L)  &= ~(TEC_EXTERNAL_CLEAR_SIGNALTYPE_LEVEL_SENSITIVE +
                                                TEC_EXTERNAL_CLEAR_SIGNAL_HELD +
                                                TEC_EXTERNAL_CLEAR_POLARITY_RISING_EDGE_OR_HIGH_LEVEL
                                                );

    HWREG8(baseAddress + OFS_TEC0XCTL2_L)  |= (signalType +
                                                signalHold +
                                                polarityBit
                                                );
}

void TEC_initExternalFaultInput(uint16_t baseAddress,
    TEC_initExternalFaultInputParam *param)
{
    HWREG8(baseAddress + OFS_TEC0XCTL2_L)  &= ~((TEC_EXTERNAL_FAULT_SIGNALTYPE_LEVEL_SENSITIVE << param->selectedExternalFault)+
                                                (TEC_EXTERNAL_FAULT_POLARITY_RISING_EDGE_OR_HIGH_LEVEL << param->selectedExternalFault)+
                                                (TEC_EXTERNAL_FAULT_SIGNAL_HELD << param->selectedExternalFault )
                                                );

    HWREG8(baseAddress + OFS_TEC0XCTL2_L)  |= ((param->signalType << param->selectedExternalFault)+
                                               (param->polarityBit << param->selectedExternalFault) +
                                               (param->signalHold << param->selectedExternalFault )
                                               );
}

void TEC_enableExternalFaultInput (uint16_t baseAddress,
                                        uint8_t channelEventBlock
                                        )
{
    HWREG8(baseAddress + OFS_TEC0XCTL0_H)  |= (1 << channelEventBlock );
}

void TEC_disableExternalFaultInput (uint16_t baseAddress,
                                        uint8_t channelEventBlock
                                        )
{
    HWREG8(baseAddress + OFS_TEC0XCTL0_H)  &= ~(1 << channelEventBlock );
}

void TEC_enableExternalClearInput (uint16_t baseAddress )
{
   HWREG8(baseAddress + OFS_TEC0XCTL2_L)  |= TECEXCLREN;
}

void TEC_disableExternalClearInput (uint16_t baseAddress )
{
   HWREG8(baseAddress + OFS_TEC0XCTL2_L)  &= ~TECEXCLREN;
}

void TEC_enableAuxiliaryClearSignal (uint16_t baseAddress )
{
   HWREG8(baseAddress + OFS_TEC0XCTL2_L)  |= TECAXCLREN;
}

void TEC_disableAuxiliaryClearSignal (uint16_t baseAddress )
{
   HWREG8(baseAddress + OFS_TEC0XCTL2_L)  &= ~TECAXCLREN;
}

void TEC_clearInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    HWREG8(baseAddress + OFS_TEC0XINT_L)  &= ~mask;
}

uint8_t TEC_getInterruptStatus (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Return the interrupt status of the request masked bit.
    return (HWREG8(baseAddress + OFS_TEC0XINT_L) & mask);
}

void TEC_enableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Enable the interrupt masked bit
    HWREG8(baseAddress + OFS_TEC0XINT_H) |= mask;
}

void TEC_disableInterrupt (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Disable the interrupt masked bit
    HWREG8(baseAddress + OFS_TEC0XINT_H) &= ~(mask);
}

uint8_t TEC_getExternalFaultStatus (uint16_t baseAddress,
    uint8_t mask
    )
{
    //Return the interrupt status of the request masked bit.
    return (HWREG8(baseAddress + OFS_TEC0STA_L) & mask);
}

void TEC_clearExternalFaultStatus (uint16_t baseAddress,
    uint8_t mask
    )
{
    HWREG8(baseAddress + OFS_TEC0STA_L)  &= ~mask;
}

uint8_t TEC_getExternalClearStatus (uint16_t baseAddress)

{
    //Return the interrupt status of the request masked bit.
    return (HWREG8(baseAddress + OFS_TEC0STA_L) & TECXCLRSTA);
}

void TEC_clearExternalClearStatus (uint16_t baseAddress)
{
   HWREG8(baseAddress + OFS_TEC0STA_L)  &= ~TECXCLRSTA;
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for tec_api
//! @}
//
//*****************************************************************************
