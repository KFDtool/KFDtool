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
// ref.c - Driver for the ref Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup ref_api ref
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_REF__
#include "ref.h"

#include <assert.h>

void Ref_setReferenceVoltage (uint16_t baseAddress,
    uint8_t referenceVoltageSelect)
{
    HWREG8(baseAddress + OFS_REFCTL0_L) &= ~(REFVSEL_3);
    HWREG8(baseAddress + OFS_REFCTL0_L) |= referenceVoltageSelect;
}

void Ref_disableTempSensor (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_REFCTL0_L) |= REFTCOFF;
}

void Ref_enableTempSensor (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_REFCTL0_L) &= ~(REFTCOFF);
}

void Ref_enableReferenceVoltageOutput (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_REFCTL0_L) |= REFOUT;
}

void Ref_disableReferenceVoltageOutput (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_REFCTL0_L) &= ~(REFOUT);
}

void Ref_enableReferenceVoltage (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_REFCTL0_L) |= REFON;
}

void Ref_disableReferenceVoltage (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_REFCTL0_L) &= ~(REFON);
}

uint16_t Ref_getBandgapMode (uint16_t baseAddress)
{
    return (HWREG16((baseAddress) + OFS_REFCTL0) & BGMODE);
}

bool Ref_isBandgapActive (uint16_t baseAddress)
{
    if (HWREG16((baseAddress) + OFS_REFCTL0) & REFBGACT){
        return (REF_ACTIVE) ;
    } else   {
        return (REF_INACTIVE) ;
    }
}

uint16_t Ref_isRefGenBusy (uint16_t baseAddress)
{
    return (HWREG16((baseAddress) + OFS_REFCTL0) & REFGENBUSY);
}

bool Ref_isRefGenActive (uint16_t baseAddress)
{
    if (HWREG16((baseAddress) + OFS_REFCTL0) & REFGENACT){
        return (REF_ACTIVE) ;
    } else   {
        return (REF_INACTIVE) ;
    }
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for ref_api
//! @}
//
//*****************************************************************************
