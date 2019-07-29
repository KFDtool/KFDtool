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
// battbak.h - Driver for the BATTBAK Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_BATTBAK_H__
#define __MSP430WARE_BATTBAK_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_BATTERY_CHARGER__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the BattBak_unlockBackupSubSystem() function.
//
//*****************************************************************************
#define BATTBAK_UNLOCKFAILURE                                         (LOCKBAK)
#define BATTBAK_UNLOCKSUCCESS                                             (0x0)

//*****************************************************************************
//
// The following are values that can be passed to the chargerEndVoltage
// parameter for functions: BattBak_initAndEnableCharger().
//
//*****************************************************************************
#define BATTBAK_CHARGERENDVOLTAGE_VCC                                 (BAKCHV0)
#define BATTBAK_CHARGERENDVOLTAGE2_7V                                 (BAKCHV1)

//*****************************************************************************
//
// The following are values that can be passed to the chargeCurrent parameter
// for functions: BattBak_initAndEnableCharger().
//
//*****************************************************************************
#define BATTBAK_CHARGECURRENT_5KOHM                                   (BAKCHC0)
#define BATTBAK_CHARGECURRENT_10KOHM                                  (BAKCHC1)
#define BATTBAK_CHARGECURRENT_20KOHM                        (BAKCHC0 + BAKCHC1)

//*****************************************************************************
//
// The following are values that can be passed to the backupRAMSelect parameter
// for functions: BattBak_setBackupRAMData(), and BattBak_getBackupRAMData().
//
//*****************************************************************************
#define BATTBAK_RAMSELECT_0                                            (0x0000)
#define BATTBAK_RAMSELECT_1                                            (0x0002)
#define BATTBAK_RAMSELECT_2                                            (0x0004)
#define BATTBAK_RAMSELECT_3                                            (0x0006)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Unlocks any pending backup input pins and RTC_B interrupts to be
//! serviced.
//!
//! This function unlocks the ability to view and service any pending backup
//! input pin interrupts, as well as pending RTC_B interrupts. The backup sub-
//! system can only be unlocked when the backup domain has settled, so this
//! function returns the status of the unlock bit after it has been to be
//! verified by user code. Please note, the backup sub-system should only be
//! unlocked after modifying the RTC_B registers.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//!
//! \return One of the following:
//!         - \b BATTBAK_UNLOCKFAILURE backup system has not yet settled
//!         - \b BATTBAK_UNLOCKSUCCESS successfully unlocked
//!         \n indicating if the backup system has been successfully unlocked
//
//*****************************************************************************
extern uint16_t BattBak_unlockBackupSubSystem(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables the backup supply to be measured by the ADC battery monitor
//! input.
//!
//! This function enables the backup supply signal to be monitored by the ADC
//! battery supply monitor input, to allow a measurement of the voltage from
//! the backup battery.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//!
//! \return None
//
//*****************************************************************************
extern void BattBak_enableBackupSupplyToADC(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the backup supply input to the ADC module.
//!
//! This function disables the ability to monitor the backup supply voltage
//! from the ADC battery monitor input.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//!
//! \return None
//
//*****************************************************************************
extern void BattBak_disableBackupSupplyToADC(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Manually switches to backup supply.
//!
//! This function uses software to manually switch to the backup battery
//! supply. Once this bit is set, it will be automatically reset by a POR and
//! the system returns to an automatic switch to backup supply.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//!
//! \return None
//
//*****************************************************************************
extern void BattBak_switchToBackupSupplyManually(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables backup battery system.
//!
//! This function disables the battery backup system from being used. The
//! battery backup system is re-enabled after a power cycle.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//!
//! \return None
//
//*****************************************************************************
extern void BattBak_disable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Initializes and enables the backup battery charger.
//!
//! This function initializes the backup battery charger with the selected
//! settings.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//! \param chargerEndVoltage is the maximum voltage to charge the backup
//!        battery to.
//!        Valid values are:
//!        - \b BATTBAK_CHARGERENDVOLTAGE_VCC - charges backup battery up to
//!           Vcc
//!        - \b BATTBAK_CHARGERENDVOLTAGE2_7V - charges backup battery up to
//!           2.7V OR up to Vcc if Vcc is less than 2.7V.
//!        \n Modified bits are \b BAKCHVx of \b BAKCHCTL register.
//! \param chargeCurrent is the maximum current to charge the backup battery
//!        at.
//!        Valid values are:
//!        - \b BATTBAK_CHARGECURRENT_5KOHM
//!        - \b BATTBAK_CHARGECURRENT_10KOHM
//!        - \b BATTBAK_CHARGECURRENT_20KOHM
//!        \n Modified bits are \b BAKCHCx of \b BAKCHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void BattBak_initAndEnableCharger(uint16_t baseAddress,
                                         uint8_t chargerEndVoltage,
                                         uint8_t chargeCurrent);

//*****************************************************************************
//
//! \brief Disables and resets backup battery charger settings.
//!
//! This function clears all backup battery charger settings and disables it.
//! To re-enable the charger, a call to BattBak_initAndEnableCharger() is
//! required.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//!
//! \return None
//
//*****************************************************************************
extern void BattBak_disableCharger(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets data into the selected backup RAM space.
//!
//! This function sets the given 16-bit data into the selected backup RAM
//! space.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//! \param backupRAMSelect is the backup RAM space to set data into.
//!        Valid values are:
//!        - \b BATTBAK_RAMSELECT_0
//!        - \b BATTBAK_RAMSELECT_1
//!        - \b BATTBAK_RAMSELECT_2
//!        - \b BATTBAK_RAMSELECT_3
//! \param data is the data to set into the selected backup RAM space.
//!
//! \return None
//
//*****************************************************************************
extern void BattBak_setBackupRAMData(uint16_t baseAddress,
                                     uint8_t backupRAMSelect,
                                     uint16_t data);

//*****************************************************************************
//
//! \brief Returns the data from the selected backup RAM space.
//!
//! This function returns the 16-bit data currently residing in the selected
//! backup RAM space.
//!
//! \param baseAddress is the base address of the BATTBAK module.
//! \param backupRAMSelect is the backup RAM space to read out from.
//!        Valid values are:
//!        - \b BATTBAK_RAMSELECT_0
//!        - \b BATTBAK_RAMSELECT_1
//!        - \b BATTBAK_RAMSELECT_2
//!        - \b BATTBAK_RAMSELECT_3
//!
//! \return Data residing in the selected backup RAM space.
//
//*****************************************************************************
extern uint16_t BattBak_getBackupRAMData(uint16_t baseAddress,
                                         uint8_t backupRAMSelect);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_BATTBAK_H__
