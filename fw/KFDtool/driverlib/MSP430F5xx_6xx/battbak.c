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
// battbak.c - Driver for the battbak Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup battbak_api battbak
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_BATTERY_CHARGER__
#include "battbak.h"

#include <assert.h>

uint16_t BattBak_unlockBackupSubSystem (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_BAKCTL) &= ~(LOCKBAK);
    return (HWREG8(baseAddress + OFS_BAKCTL) & LOCKBAK);
}

void BattBak_enableBackupSupplyToADC (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_BAKCTL) |= BAKADC;
}

void BattBak_disableBackupSupplyToADC (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_BAKCTL) &= ~(BAKADC);
}

void BattBak_switchToBackupSupplyManually (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_BAKCTL) |= BAKSW;
}

void BattBak_disable (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_BAKCTL) |= BAKDIS;
}

void BattBak_initAndEnableCharger (uint16_t baseAddress,
    uint8_t chargerEndVoltage,
    uint8_t chargeCurrent)
{
    HWREG16(baseAddress +
        OFS_BAKCHCTL) = CHPWD + chargerEndVoltage + chargeCurrent + CHEN;
}

void BattBak_disableCharger (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_BAKCHCTL) = CHPWD;
}

void BattBak_setBackupRAMData (uint16_t baseAddress,
    uint8_t backupRAMSelect,
    uint16_t data)
{
    HWREG16(baseAddress + backupRAMSelect) = data;
}

uint16_t BattBak_getBackupRAMData (uint16_t baseAddress,
    uint8_t backupRAMSelect)
{
    return ( HWREG16(baseAddress + backupRAMSelect) );
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for battbak_api
//! @}
//
//*****************************************************************************
