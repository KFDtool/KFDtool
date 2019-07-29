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
// tec.h - Driver for the TEC Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_TEC_H__
#define __MSP430WARE_TEC_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_TEV0__

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

#include "inc/hw_memmap.h"
//*****************************************************************************
//
//! \brief Used in the TEC_initExternalFaultInput() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct TEC_initExternalFaultInputParam {
    //! Is the selected external fault
    //! \n Valid values are:
    //! - \b TEC_EXTERNAL_FAULT_0
    //! - \b TEC_EXTERNAL_FAULT_1
    //! - \b TEC_EXTERNAL_FAULT_2
    //! - \b TEC_EXTERNAL_FAULT_3
    //! - \b TEC_EXTERNAL_FAULT_4
    //! - \b TEC_EXTERNAL_FAULT_5
    //! - \b TEC_EXTERNAL_FAULT_6
    uint8_t selectedExternalFault;
    //! Is the selected signal type
    //! \n Valid values are:
    //! - \b TEC_EXTERNAL_FAULT_SIGNALTYPE_EDGE_SENSITIVE [Default]
    //! - \b TEC_EXTERNAL_FAULT_SIGNALTYPE_LEVEL_SENSITIVE
    uint16_t signalType;
    //! Is the selected signal hold
    //! \n Valid values are:
    //! - \b TEC_EXTERNAL_FAULT_SIGNAL_NOT_HELD [Default]
    //! - \b TEC_EXTERNAL_FAULT_SIGNAL_HELD
    uint8_t signalHold;
    //! Is the selected signal type
    //! \n Valid values are:
    //! - \b TEC_EXTERNAL_FAULT_POLARITY_FALLING_EDGE_OR_LOW_LEVEL [Default]
    //! - \b TEC_EXTERNAL_FAULT_POLARITY_RISING_EDGE_OR_HIGH_LEVEL
    uint8_t polarityBit;
} TEC_initExternalFaultInputParam;


//*****************************************************************************
//
// The following are values that can be passed to the signalHold parameter for
// functions: TEC_initExternalClearInput().
//
//*****************************************************************************
#define TEC_EXTERNAL_CLEAR_SIGNAL_NOT_HELD                                 0x00
#define TEC_EXTERNAL_CLEAR_SIGNAL_HELD                              TECEXCLRHLD

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: TEC_initExternalFaultInput().
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_SIGNAL_NOT_HELD                                 0x00
#define TEC_EXTERNAL_FAULT_SIGNAL_HELD                              TECXFLTHLD0

//*****************************************************************************
//
// The following are values that can be passed to the polarityBit parameter for
// functions: TEC_initExternalClearInput().
//
//*****************************************************************************
#define TEC_EXTERNAL_CLEAR_POLARITY_FALLING_EDGE_OR_LOW_LEVEL              0x00
#define TEC_EXTERNAL_CLEAR_POLARITY_RISING_EDGE_OR_HIGH_LEVEL       TECEXCLRPOL

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: TEC_initExternalFaultInput().
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_POLARITY_FALLING_EDGE_OR_LOW_LEVEL              0x00
#define TEC_EXTERNAL_FAULT_POLARITY_RISING_EDGE_OR_HIGH_LEVEL       TECXFLTPOL0

//*****************************************************************************
//
// The following are values that can be passed to the signalType parameter for
// functions: TEC_initExternalClearInput().
//
//*****************************************************************************
#define TEC_EXTERNAL_CLEAR_SIGNALTYPE_EDGE_SENSITIVE                       0x00
#define TEC_EXTERNAL_CLEAR_SIGNALTYPE_LEVEL_SENSITIVE               TECEXCLRLVS

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: TEC_initExternalFaultInput().
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_SIGNALTYPE_EDGE_SENSITIVE                       0x00
#define TEC_EXTERNAL_FAULT_SIGNALTYPE_LEVEL_SENSITIVE               TECXFLTLVS0

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: TEC_initExternalFaultInput().
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_0                                                  0
#define TEC_EXTERNAL_FAULT_1                                                  1
#define TEC_EXTERNAL_FAULT_2                                                  2
#define TEC_EXTERNAL_FAULT_3                                                  3
#define TEC_EXTERNAL_FAULT_4                                                  4
#define TEC_EXTERNAL_FAULT_5                                                  5
#define TEC_EXTERNAL_FAULT_6                                                  6

//*****************************************************************************
//
// The following are values that can be passed to the channelEventBlock
// parameter for functions: TEC_enableExternalFaultInput(), and
// TEC_disableExternalFaultInput(); the mask parameter for functions:
// TEC_clearExternalFaultStatus(), and TEC_getExternalFaultStatus() as well as
// returned by the TEC_getExternalFaultStatus() function.
//
//*****************************************************************************
#define TEC_CE0                                                     TECXFLT0STA
#define TEC_CE1                                                     TECXFLT1STA
#define TEC_CE2                                                     TECXFLT2STA
#define TEC_CE3                                                     TECXFLT3STA
#define TEC_CE4                                                     TECXFLT4STA
#define TEC_CE5                                                     TECXFLT5STA
#define TEC_CE6                                                     TECXFLT6STA

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: TEC_clearInterrupt(), TEC_getInterruptStatus(),
// TEC_enableInterrupt(), and TEC_disableInterrupt() as well as returned by the
// TEC_getInterruptStatus() function.
//
//*****************************************************************************
#define TEC_EXTERNAL_FAULT_INTERRUPT                                 TECXFLTIFG
#define TEC_EXTERNAL_CLEAR_INTERRUPT                                TECEXCLRIFG
#define TEC_AUXILIARY_CLEAR_INTERRUPT                               TECAXCLRIFG

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the TEC_getExternalClearStatus() function.
//
//*****************************************************************************
#define TEC_EXTERNAL_CLEAR_DETECTED                                  TECXCLRSTA
#define TEC_EXTERNAL_CLEAR_NOT_DETECTED                                    0x00

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Configures the Timer Event Control External Clear Input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param signalType is the selected signal type
//!        Valid values are:
//!        - \b TEC_EXTERNAL_CLEAR_SIGNALTYPE_EDGE_SENSITIVE [Default]
//!        - \b TEC_EXTERNAL_CLEAR_SIGNALTYPE_LEVEL_SENSITIVE
//! \param signalHold is the selected signal hold
//!        Valid values are:
//!        - \b TEC_EXTERNAL_CLEAR_SIGNAL_NOT_HELD [Default]
//!        - \b TEC_EXTERNAL_CLEAR_SIGNAL_HELD
//! \param polarityBit is the selected signal type
//!        Valid values are:
//!        - \b TEC_EXTERNAL_CLEAR_POLARITY_FALLING_EDGE_OR_LOW_LEVEL [Default]
//!        - \b TEC_EXTERNAL_CLEAR_POLARITY_RISING_EDGE_OR_HIGH_LEVEL
//!
//! Modified bits of \b TECxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_initExternalClearInput(uint16_t baseAddress,
                                       uint8_t signalType,
                                       uint8_t signalHold,
                                       uint8_t polarityBit);

//*****************************************************************************
//
//! \brief Configures the Timer Event Control External Fault Input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param param is the pointer to struct for external fault input
//!        initialization.
//!
//! Modified bits of \b TECxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_initExternalFaultInput(uint16_t baseAddress,
                                       TEC_initExternalFaultInputParam *param);

//*****************************************************************************
//
//! \brief Enable the Timer Event Control External fault input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param channelEventBlock selects the channel event block
//!        Valid values are:
//!        - \b TEC_CE0
//!        - \b TEC_CE1
//!        - \b TEC_CE2
//!        - \b TEC_CE3 - (available on TEC5 TEC7)
//!        - \b TEC_CE4 - (available on TEC5 TEC7)
//!        - \b TEC_CE5 - (only available on TEC7)
//!        - \b TEC_CE6 - (only available on TEC7)
//!
//! Modified bits of \b TECxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_enableExternalFaultInput(uint16_t baseAddress,
                                         uint8_t channelEventBlock);

//*****************************************************************************
//
//! \brief Disable the Timer Event Control External fault input
//!
//! \param baseAddress is the base address of the TEC module.
//! \param channelEventBlock selects the channel event block
//!        Valid values are:
//!        - \b TEC_CE0
//!        - \b TEC_CE1
//!        - \b TEC_CE2
//!        - \b TEC_CE3 - (available on TEC5 TEC7)
//!        - \b TEC_CE4 - (available on TEC5 TEC7)
//!        - \b TEC_CE5 - (only available on TEC7)
//!        - \b TEC_CE6 - (only available on TEC7)
//!
//! Modified bits of \b TECxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_disableExternalFaultInput(uint16_t baseAddress,
                                          uint8_t channelEventBlock);

//*****************************************************************************
//
//! \brief Enable the Timer Event Control External Clear Input
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified bits of \b TECxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_enableExternalClearInput(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disable the Timer Event Control External Clear Input
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified bits of \b TECxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_disableExternalClearInput(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enable the Timer Event Control Auxiliary Clear Signal
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified bits of \b TECxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_enableAuxiliaryClearSignal(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disable the Timer Event Control Auxiliary Clear Signal
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified bits of \b TECxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_disableAuxiliaryClearSignal(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Clears the Timer Event Control Interrupt flag
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked interrupt flag to be cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag
//!        - \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!        - \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! Modified bits of \b TECxINT register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_clearInterrupt(uint16_t baseAddress,
                               uint8_t mask);

//*****************************************************************************
//
//! \brief Gets the current Timer Event Control interrupt status.
//!
//! This returns the interrupt status for the module based on which flag is
//! passed.
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag
//!        - \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!        - \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! \return Logical OR of any of the following:
//!         - \b TEC_EXTERNAL_FAULT_INTERRUPT External fault interrupt flag
//!         - \b TEC_EXTERNAL_CLEAR_INTERRUPT External clear interrupt flag
//!         - \b TEC_AUXILIARY_CLEAR_INTERRUPT Auxiliary clear interrupt flag
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint8_t TEC_getInterruptStatus(uint16_t baseAddress,
                                      uint8_t mask);

//*****************************************************************************
//
//! \brief Enables individual Timer Event Control interrupt sources.
//!
//! Enables the indicated Timer Event Control interrupt sources. Only the
//! sources that are enabled can be reflected to the processor interrupt;
//! disabled sources have no effect on the processor. Does not clear interrupt
//! flags.
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the bit mask of the interrupt sources to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag
//!        - \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!        - \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! Modified bits of \b TECxINT register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_enableInterrupt(uint16_t baseAddress,
                                uint8_t mask);

//*****************************************************************************
//
//! \brief Disables individual Timer Event Control interrupt sources.
//!
//! Disables the indicated Timer Event Control interrupt sources. Only the
//! sources that are enabled can be reflected to the processor interrupt;
//! disabled sources have no effect on the processor.
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the bit mask of the interrupt sources to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b TEC_EXTERNAL_FAULT_INTERRUPT - External fault interrupt flag
//!        - \b TEC_EXTERNAL_CLEAR_INTERRUPT - External clear interrupt flag
//!        - \b TEC_AUXILIARY_CLEAR_INTERRUPT - Auxiliary clear interrupt flag
//!
//! Modified bits of \b TECxINT register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_disableInterrupt(uint16_t baseAddress,
                                 uint8_t mask);

//*****************************************************************************
//
//! \brief Gets the current Timer Event Control External Fault Status
//!
//! This returns the Timer Event Control fault status for the module.
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b TEC_CE0
//!        - \b TEC_CE1
//!        - \b TEC_CE2
//!        - \b TEC_CE3 - (available on TEC5 TEC7)
//!        - \b TEC_CE4 - (available on TEC5 TEC7)
//!        - \b TEC_CE5 - (only available on TEC7)
//!        - \b TEC_CE6 - (only available on TEC7)
//!
//! \return Logical OR of any of the following:
//!         - \b TEC_CE0
//!         - \b TEC_CE1
//!         - \b TEC_CE2
//!         - \b TEC_CE3 (available on TEC5 TEC7)
//!         - \b TEC_CE4 (available on TEC5 TEC7)
//!         - \b TEC_CE5 (only available on TEC7)
//!         - \b TEC_CE6 (only available on TEC7)
//!         \n indicating the external fault status of the masked channel event
//!         blocks
//
//*****************************************************************************
extern uint8_t TEC_getExternalFaultStatus(uint16_t baseAddress,
                                          uint8_t mask);

//*****************************************************************************
//
//! \brief Clears the Timer Event Control External Fault Status
//!
//! \param baseAddress is the base address of the TEC module.
//! \param mask is the masked status flag be cleared
//!        Mask value is the logical OR of any of the following:
//!        - \b TEC_CE0
//!        - \b TEC_CE1
//!        - \b TEC_CE2
//!        - \b TEC_CE3 - (available on TEC5 TEC7)
//!        - \b TEC_CE4 - (available on TEC5 TEC7)
//!        - \b TEC_CE5 - (only available on TEC7)
//!        - \b TEC_CE6 - (only available on TEC7)
//!
//! Modified bits of \b TECxINT register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_clearExternalFaultStatus(uint16_t baseAddress,
                                         uint8_t mask);

//*****************************************************************************
//
//! \brief Gets the current Timer Event Control External Clear Status
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! \return One of the following:
//!         - \b TEC_EXTERNAL_CLEAR_DETECTED
//!         - \b TEC_EXTERNAL_CLEAR_NOT_DETECTED
//!         \n indicating the status of the external clear
//
//*****************************************************************************
extern uint8_t TEC_getExternalClearStatus(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Clears the Timer Event Control External Clear Status
//!
//! \param baseAddress is the base address of the TEC module.
//!
//! Modified bits of \b TECxINT register.
//!
//! \return None
//
//*****************************************************************************
extern void TEC_clearExternalClearStatus(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_TEC_H__
