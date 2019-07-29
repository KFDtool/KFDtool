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
// pmap.c - Driver for the pmap Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup pmap_api pmap
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PORT_MAPPING__
#include "pmap.h"

#include <assert.h>

void PMAP_initPorts ( uint16_t baseAddress,
    PMAP_initPortsParam *param)
{

    //Store current interrupt state, then disable all interrupts
    uint16_t globalInterruptState = __get_SR_register() & GIE;
    __disable_interrupt();

    //Get write-access to port mapping registers:
    HWREG16(baseAddress + OFS_PMAPKEYID) = PMAPPW;

    //Enable/Disable reconfiguration during runtime
    HWREG8(baseAddress + OFS_PMAPCTL) &= ~PMAPRECFG;
    HWREG8(baseAddress + OFS_PMAPCTL) |= param->portMapReconfigure;

    //Configure Port Mapping:
    uint16_t i;
    for (i = 0; i < param->numberOfPorts * 8; i++)
    {
        param->PxMAPy[i] = param->portMapping[i];
    }

    //Disable write-access to port mapping registers:
    HWREG8(baseAddress + OFS_PMAPKEYID) = 0;

    //Restore previous interrupt state
    __bis_SR_register(globalInterruptState);
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for pmap_api
//! @}
//
//*****************************************************************************
