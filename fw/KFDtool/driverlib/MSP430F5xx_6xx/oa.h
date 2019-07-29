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
// oa.h - Driver for the OA Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_OA_H__
#define __MSP430WARE_OA_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_OA_0__

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
// The following are values that can be passed to the positiveInputMask
// parameter for functions: OA_openSwitch(), and OA_closeSwitch().
//
//*****************************************************************************
#define OA_POSITIVE_INPUT_TERMINAL_SWITCH0                                 PSW0
#define OA_POSITIVE_INPUT_TERMINAL_SWITCH1                                 PSW1
#define OA_POSITIVE_INPUT_TERMINAL_SWITCH2                                 PSW2
#define OA_POSITIVE_INPUT_TERMINAL_SWITCH3                                 PSW3
#define OA_POSITIVE_INPUT_TERMINAL_NONE                                    0x00

//*****************************************************************************
//
// The following are values that can be passed to the negativeInputMask
// parameter for functions: OA_openSwitch(), and OA_closeSwitch().
//
//*****************************************************************************
#define OA_NEGATIVE_INPUT_TERMINAL_SWITCH0                                 NSW0
#define OA_NEGATIVE_INPUT_TERMINAL_SWITCH1                                 NSW1
#define OA_NEGATIVE_INPUT_TERMINAL_SWITCH2                                 NSW2
#define OA_NEGATIVE_INPUT_TERMINAL_SWITCH3                                 NSW3
#define OA_NEGATIVE_INPUT_TERMINAL_SWITCH4                                 NSW4
#define OA_NEGATIVE_INPUT_TERMINAL_NONE                                    0x00

//*****************************************************************************
//
// The following are values that can be passed to the groundMask parameter for
// functions: OA_openSwitch(), and OA_closeSwitch().
//
//*****************************************************************************
#define OA_GROUND_SWITCH0                                                  GSW0
#define OA_GROUND_SWITCH1                                                  GSW1
#define OA_GROUND_NONE                                                     0x00

//*****************************************************************************
//
// The following are values that can be passed to the inputTerminal parameter
// for functions: OA_getSwitchStatus().
//
//*****************************************************************************
#define OA_POSITIVE_INPUT_TERMINAL_SWITCHES                                0x00
#define OA_NEGATIVE_INPUT_TERMINAL_SWITCHES                                0x01
#define OA_GROUND_SWITCHES                                                 0x02

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the OA_getRailToRailInputReadyStatus() function.
//
//*****************************************************************************
#define OA_INPUT_READY                                                 OARRIRDY
#define OA_INPUT_NOT_READY                                                 0x00

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the OA_getRailToRailInputStatus() function.
//
//*****************************************************************************
#define OA_INPUT_ENABLED                                                  OARRI
#define OA_INPUT_DISABLED                                                (0x00)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the OA_getAmplifierModeStatus() function.
//
//*****************************************************************************
#define OA_AMPLIFIER_MODE_ENABLED                                           OAM
#define OA_AMPLIFIER_MODE_DISABLED                                       (0x00)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Opens the positive input terminal, negative input terminal and
//! ground switch of the OA Module per user selection
//!
//! \param baseAddress is the base address of the OA module.
//! \param positiveInputMask
//!        Mask value is the logical OR of any of the following:
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH0
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH1
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH2
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH3
//!        - \b OA_POSITIVE_INPUT_TERMINAL_NONE
//! \param negativeInputMask
//!        Mask value is the logical OR of any of the following:
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH0
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH1
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH2
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH3
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH4
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_NONE
//! \param groundMask
//!        Mask value is the logical OR of any of the following:
//!        - \b OA_GROUND_SWITCH0
//!        - \b OA_GROUND_SWITCH1
//!        - \b OA_GROUND_NONE
//!
//! \return None
//
//*****************************************************************************
extern void OA_openSwitch(uint16_t baseAddress,
                          uint8_t positiveInputMask,
                          uint8_t negativeInputMask,
                          uint8_t groundMask);

//*****************************************************************************
//
//! \brief Closes the positive input terminal, negative input terminal and
//! ground switch of the OA Module per user selection.
//!
//! \param baseAddress is the base address of the OA module.
//! \param positiveInputMask
//!        Mask value is the logical OR of any of the following:
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH0
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH1
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH2
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCH3
//!        - \b OA_POSITIVE_INPUT_TERMINAL_NONE
//! \param negativeInputMask
//!        Mask value is the logical OR of any of the following:
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH0
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH1
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH2
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH3
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCH4
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_NONE
//! \param groundMask
//!        Mask value is the logical OR of any of the following:
//!        - \b OA_GROUND_SWITCH0
//!        - \b OA_GROUND_SWITCH1
//!        - \b OA_GROUND_NONE
//!
//! \return None
//
//*****************************************************************************
extern void OA_closeSwitch(uint16_t baseAddress,
                           uint8_t positiveInputMask,
                           uint8_t negativeInputMask,
                           uint8_t groundMask);

//*****************************************************************************
//
//! \brief Gets the positive input terminal, negative input terminal and ground
//! switch status of the OA Module per user selection.
//!
//! \param baseAddress is the base address of the OA module.
//! \param inputTerminal
//!        Valid values are:
//!        - \b OA_POSITIVE_INPUT_TERMINAL_SWITCHES
//!        - \b OA_NEGATIVE_INPUT_TERMINAL_SWITCHES
//!        - \b OA_GROUND_SWITCHES
//!
//! \return Logical OR of the switches that are closed in the selected
//!         inputTerminal
//
//*****************************************************************************
extern uint8_t OA_getSwitchStatus(uint16_t baseAddress,
                                  uint8_t inputTerminal);

//*****************************************************************************
//
//! \brief Gets the rail to rail input ready status
//!
//! \param baseAddress is the base address of the OA module.
//!
//! \return One of the following:
//!         - \b OA_INPUT_READY
//!         - \b OA_INPUT_NOT_READY
//
//*****************************************************************************
extern uint8_t OA_getRailToRailInputReadyStatus(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Gets the rail to rail input status
//!
//! \param baseAddress is the base address of the OA module.
//!
//! \return One of the following:
//!         - \b OA_INPUT_ENABLED
//!         - \b OA_INPUT_DISABLED
//
//*****************************************************************************
extern uint8_t OA_getRailToRailInputStatus(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables the rail to rail input
//!
//! \param baseAddress is the base address of the OA module.
//!
//! \return None
//
//*****************************************************************************
extern void OA_enableRailToRailInput(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the rail to rail input
//!
//! \param baseAddress is the base address of the OA module.
//!
//! \return None
//
//*****************************************************************************
extern void OA_disableRailToRailInput(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the amplifier mode
//!
//! \param baseAddress is the base address of the OA module.
//!
//! \return None
//
//*****************************************************************************
extern void OA_disableAmplifierMode(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables the amplifier mode
//!
//! \param baseAddress is the base address of the OA module.
//!
//! \return None
//
//*****************************************************************************
extern void OA_enableAmplifierMode(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Gets the amplifier mode status
//!
//! \param baseAddress is the base address of the OA module.
//!
//! \return One of the following:
//!         - \b OA_AMPLIFIER_MODE_ENABLED
//!         - \b OA_AMPLIFIER_MODE_DISABLED
//
//*****************************************************************************
extern uint8_t OA_getAmplifierModeStatus(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_OA_H__
