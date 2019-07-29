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
// oa.c - Driver for the oa Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup oa_api oa
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_OA_0__
#include "oa.h"

#include <assert.h>

void OA_openSwitch( uint16_t baseAddress,
                    uint8_t positiveInputMask,
                    uint8_t negativeInputMask,
                    uint8_t groundMask
                  )
{
    HWREG16(baseAddress + OFS_OA0PSW) &= ~positiveInputMask;
    HWREG16(baseAddress + OFS_OA0NSW) &= ~negativeInputMask;
    HWREG16(baseAddress + OFS_OA0GSW) &= ~groundMask;
}

void OA_closeSwitch(uint16_t baseAddress,
                    uint8_t positiveInputMask,
                    uint8_t negativeInputMask,
                    uint8_t groundMask
                  )
{

    HWREG16(baseAddress + OFS_OA0PSW) |= positiveInputMask;
    HWREG16(baseAddress + OFS_OA0NSW) |= negativeInputMask;
    HWREG16(baseAddress + OFS_OA0GSW) |= groundMask;
}

uint8_t OA_getSwitchStatus(uint16_t baseAddress,
                        uint8_t inputTerminal
                        )
{
    uint8_t returnValue = 0;

    switch(inputTerminal)
    {
        case OA_POSITIVE_INPUT_TERMINAL_SWITCHES:
            returnValue = (HWREG16(baseAddress + OFS_OA0PSW) & (PSW3 |
                                                            PSW2 |
                                                            PSW1 |
                                                            PSW0)
                                                            );
            break;
        case OA_NEGATIVE_INPUT_TERMINAL_SWITCHES:
            returnValue = (HWREG16(baseAddress + OFS_OA0NSW) & (NSW4 |
                                                            NSW3 |
                                                            NSW2 |
                                                            NSW1 |
                                                            NSW0)
                                                            );
            break;
        case OA_GROUND_SWITCHES:
            returnValue = (HWREG16(baseAddress + OFS_OA0GSW) & (GSW1 |
                                                            GSW0)
                                                            );
            break;
    }

    return returnValue;

}

uint8_t OA_getRailToRailInputReadyStatus(uint16_t baseAddress)
{
    return ((HWREG8(baseAddress + OFS_OA0CTL0) & OARRIRDY));
}

uint8_t OA_getRailToRailInputStatus(uint16_t baseAddress)
{
    return ((HWREG8(baseAddress + OFS_OA0CTL0) & OARRI));
}

void OA_enableRailToRailInput(uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_OA0CTL0) |= OARRI;
}

void OA_disableRailToRailInput(uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_OA0CTL0) &= ~OARRI;
}

void OA_disableAmplifierMode(uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_OA0CTL0) &= ~OAM;
}

void OA_enableAmplifierMode(uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_OA0CTL0) |= OAM;
}

uint8_t OA_getAmplifierModeStatus(uint16_t baseAddress)
{
    return ((HWREG8(baseAddress + OFS_OA0CTL0) & OAM));
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for oa_api
//! @}
//
//*****************************************************************************
