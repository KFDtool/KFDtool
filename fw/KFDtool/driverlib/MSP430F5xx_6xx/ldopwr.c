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
// ldopwr.c - Driver for the ldopwr Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup ldopwr_api ldopwr
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PU__
#include "ldopwr.h"

#include <assert.h>

void LDOPWR_unLockConfiguration ( uint16_t baseAddress )
{
    HWREG16(baseAddress + OFS_LDOKEYPID) = 0x9628;
}

void LDOPWR_lockConfiguration ( uint16_t baseAddress )
{
    HWREG16(baseAddress + OFS_LDOKEYPID) = 0x0000;
}

void LDOPWR_enablePort_U_inputs (uint16_t baseAddress )
{
    HWREG8(baseAddress + OFS_PUCTL_H) |= PUIPE_H;
}

void LDOPWR_disablePort_U_inputs (uint16_t baseAddress )
{
    HWREG8(baseAddress + OFS_PUCTL_H) &= ~PUIPE_H;
}

void LDOPWR_enablePort_U_outputs (uint16_t baseAddress )
{
    HWREG8(baseAddress + OFS_PUCTL_L) |= PUOPE;
}

void LDOPWR_disablePort_U_outputs (uint16_t baseAddress )
{
    HWREG8(baseAddress + OFS_PUCTL_L) &= ~PUOPE;
}

uint8_t LDOPWR_getPort_U1_inputData (uint16_t baseAddress )
{
    return ((HWREG8(baseAddress + OFS_PUCTL_L) & PUIN1) >> 3 );
}

uint8_t LDOPWR_getPort_U0_inputData (uint16_t baseAddress )
{
    return ((HWREG8(baseAddress + OFS_PUCTL_L) & PUIN0) >> 2 );
}

uint8_t LDOPWR_getPort_U1_outputData (uint16_t baseAddress )
{
    return ((HWREG8(baseAddress + OFS_PUCTL_L) & PUOUT1) >> 1 );
}

uint8_t LDOPWR_getPort_U0_outputData (uint16_t baseAddress )
{
    return (HWREG8(baseAddress + OFS_PUCTL_L) & PUOUT0);
}

void LDOPWR_setPort_U1_outputData (uint16_t baseAddress,
    uint8_t value
    )
{
    if (LDOPWR_PORTU_PIN_HIGH == value){
        HWREG8(baseAddress + OFS_PUCTL_L) |= PUOUT1;
    } else   {
        HWREG8(baseAddress + OFS_PUCTL_L) &= ~PUOUT1;
    }
}

void LDOPWR_setPort_U0_outputData (uint16_t baseAddress,
    uint8_t value
    )
{
    if (LDOPWR_PORTU_PIN_HIGH == value){
        HWREG8(baseAddress + OFS_PUCTL_L) |= PUOUT0;
    } else   {
        HWREG8(baseAddress + OFS_PUCTL_L) &= ~PUOUT0;
    }
}

void LDOPWR_togglePort_U1_outputData (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_PUCTL_L) ^= PUOUT1;
}

void LDOPWR_togglePort_U0_outputData (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_PUCTL_L) ^= PUOUT0;
}

void LDOPWR_enableInterrupt (uint16_t baseAddress,
    uint16_t mask
    )
{
    HWREG8(baseAddress + OFS_LDOPWRCTL_H) |= mask;
}

void LDOPWR_disableInterrupt (uint16_t baseAddress,
    uint16_t mask
    )
{
    HWREG8(baseAddress + OFS_LDOPWRCTL_H) &= ~mask;
}

void LDOPWR_enable (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_LDOPWRCTL_H) |= LDOOEN_H;
}

void LDOPWR_disable (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_LDOPWRCTL_H) &= ~LDOOEN_H;
}

uint8_t LDOPWR_getInterruptStatus (uint16_t baseAddress,
    uint16_t mask
    )
{
    return (HWREG8(baseAddress + OFS_LDOPWRCTL_L) & mask);
}

void LDOPWR_clearInterrupt (uint16_t baseAddress,
    uint16_t mask
    )
{
    HWREG8(baseAddress + OFS_LDOPWRCTL_L) &=  ~mask;
}

uint8_t LDOPWR_isLDOInputValid (uint16_t baseAddress)
{
    return (HWREG8(baseAddress + OFS_LDOPWRCTL_L) & LDOBGVBV);
}

void LDOPWR_enableOverloadAutoOff (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_LDOPWRCTL_L) |= OVLAOFF_L;
}

void LDOPWR_disableOverloadAutoOff (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_LDOPWRCTL_L) &= ~OVLAOFF_L;
}

uint8_t LDOPWR_getOverloadAutoOffStatus (uint16_t baseAddress)
{
    return (HWREG8(baseAddress + OFS_LDOPWRCTL_L) & OVLAOFF_L);
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for ldopwr_api
//! @}
//
//*****************************************************************************
