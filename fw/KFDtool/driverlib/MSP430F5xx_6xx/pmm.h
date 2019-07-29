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
// pmm.h - Driver for the PMM Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_PMM_H__
#define __MSP430WARE_PMM_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PMM__

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
// The following are values that can be passed to the level parameter for
// functions: PMM_setVCoreUp(), PMM_setVCoreDown(), and PMM_setVCore().
//
//*****************************************************************************
#define PMM_CORE_LEVEL_0                                             PMMCOREV_0
#define PMM_CORE_LEVEL_1                                             PMMCOREV_1
#define PMM_CORE_LEVEL_2                                             PMMCOREV_2
#define PMM_CORE_LEVEL_3                                             PMMCOREV_3

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: PMM_getInterruptStatus() as well as returned by the
// PMM_getInterruptStatus() function.
//
//*****************************************************************************
#define PMM_SVSMLDLYIFG                                             SVSMLDLYIFG
#define PMM_SVMLIFG                                                     SVMLIFG
#define PMM_SVMLVLRIFG                                               SVMLVLRIFG
#define PMM_SVSMHDLYIFG                                             SVSMHDLYIFG
#define PMM_SVMHIFG                                                     SVMHIFG
#define PMM_SVMHVLRIFG                                               SVMHVLRIFG
#define PMM_PMMBORIFG                                                 PMMBORIFG
#define PMM_PMMRSTIFG                                                 PMMRSTIFG
#define PMM_PMMPORIFG                                                 PMMPORIFG
#define PMM_SVSHIFG                                                     SVSHIFG
#define PMM_SVSLIFG                                                     SVSLIFG
#define PMM_PMMLPM5IFG                                               PMMLPM5IFG

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Enables the low-side SVS circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsL(void);

//*****************************************************************************
//
//! \brief Disables the low-side SVS circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsL(void);

//*****************************************************************************
//
//! \brief Enables the low-side SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvmL(void);

//*****************************************************************************
//
//! \brief Disables the low-side SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvmL(void);

//*****************************************************************************
//
//! \brief Enables the high-side SVS circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsH(void);

//*****************************************************************************
//
//! \brief Disables the high-side SVS circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsH(void);

//*****************************************************************************
//
//! \brief Enables the high-side SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvmH(void);

//*****************************************************************************
//
//! \brief Disables the high-side SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvmH(void);

//*****************************************************************************
//
//! \brief Enables the low-side SVS and SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsLSvmL(void);

//*****************************************************************************
//
//! \brief Disables the low-side SVS and SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsLSvmL(void);

//*****************************************************************************
//
//! \brief Enables the high-side SVS and SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsHSvmH(void);

//*****************************************************************************
//
//! \brief Disables the high-side SVS and SVM circuitry
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsHSvmH(void);

//*****************************************************************************
//
//! \brief Enables the POR signal generation when a low-voltage event is
//! registered by the low-side SVS
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsLReset(void);

//*****************************************************************************
//
//! \brief Disables the POR signal generation when a low-voltage event is
//! registered by the low-side SVS
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsLReset(void);

//*****************************************************************************
//
//! \brief Enables the interrupt generation when a low-voltage event is
//! registered by the low-side SVM
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvmLInterrupt(void);

//*****************************************************************************
//
//! \brief Disables the interrupt generation when a low-voltage event is
//! registered by the low-side SVM
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvmLInterrupt(void);

//*****************************************************************************
//
//! \brief Enables the POR signal generation when a low-voltage event is
//! registered by the high-side SVS
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsHReset(void);

//*****************************************************************************
//
//! \brief Disables the POR signal generation when a low-voltage event is
//! registered by the high-side SVS
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsHReset(void);

//*****************************************************************************
//
//! \brief Enables the interrupt generation when a low-voltage event is
//! registered by the high-side SVM
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvmHInterrupt(void);

//*****************************************************************************
//
//! \brief Disables the interrupt generation when a low-voltage event is
//! registered by the high-side SVM
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIE register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvmHInterrupt(void);

//*****************************************************************************
//
//! \brief Clear all interrupt flags for the PMM
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b PMMIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_clearPMMIFGS(void);

//*****************************************************************************
//
//! \brief Enables supervisor low side in LPM with twake-up-fast from LPM2,
//! LPM3, and LPM4
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsLInLPMFastWake(void);

//*****************************************************************************
//
//! \brief Enables supervisor low side in LPM with twake-up-slow from LPM2,
//! LPM3, and LPM4
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsLInLPMSlowWake(void);

//*****************************************************************************
//
//! \brief Disables supervisor low side in LPM with twake-up-fast from LPM2,
//! LPM3, and LPM4
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsLInLPMFastWake(void);

//*****************************************************************************
//
//! \brief Disables supervisor low side in LPM with twake-up-slow from LPM2,
//! LPM3, and LPM4
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsLInLPMSlowWake(void);

//*****************************************************************************
//
//! \brief Enables supervisor high side in LPM with tpd = 20 s(1)
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsHInLPMNormPerf(void);

//*****************************************************************************
//
//! \brief Enables supervisor high side in LPM with tpd = 2.5 s(1)
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_enableSvsHInLPMFullPerf(void);

//*****************************************************************************
//
//! \brief Disables supervisor high side in LPM with tpd = 20 s(1)
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsHInLPMNormPerf(void);

//*****************************************************************************
//
//! \brief Disables supervisor high side in LPM with tpd = 2.5 s(1)
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMHCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_disableSvsHInLPMFullPerf(void);

//*****************************************************************************
//
//! \brief Optimized to provide twake-up-fast from LPM2, LPM3, and LPM4 with
//! least power
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_optimizeSvsLInLPMFastWake(void);

//*****************************************************************************
//
//! \brief Optimized to provide tpd = 2.5 s(1) in LPM with least power
//!
//!
//! Modified bits of \b PMMCTL0 register and bits of \b SVSMLCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMM_optimizeSvsHInLPMFullPerf(void);

//*****************************************************************************
//
//! \brief Increase Vcore by one level
//!
//! \param level level to which Vcore needs to be increased
//!        Valid values are:
//!        - \b PMM_CORE_LEVEL_0 [Default]
//!        - \b PMM_CORE_LEVEL_1
//!        - \b PMM_CORE_LEVEL_2
//!        - \b PMM_CORE_LEVEL_3
//!
//! Modified bits of \b PMMCTL0 register, bits of \b PMMIFG register, bits of
//! \b PMMRIE register, bits of \b SVSMHCTL register and bits of \b SVSMLCTL
//! register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern uint16_t PMM_setVCoreUp(uint8_t level);

//*****************************************************************************
//
//! \brief Decrease Vcore by one level
//!
//! \param level level to which Vcore needs to be decreased
//!        Valid values are:
//!        - \b PMM_CORE_LEVEL_0 [Default]
//!        - \b PMM_CORE_LEVEL_1
//!        - \b PMM_CORE_LEVEL_2
//!        - \b PMM_CORE_LEVEL_3
//!
//! Modified bits of \b PMMCTL0 register, bits of \b PMMIFG register, bits of
//! \b PMMRIE register, bits of \b SVSMHCTL register and bits of \b SVSMLCTL
//! register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint16_t PMM_setVCoreDown(uint8_t level);

//*****************************************************************************
//
//! \brief Set Vcore to expected level
//!
//! \param level level to which Vcore needs to be decreased/increased
//!        Valid values are:
//!        - \b PMM_CORE_LEVEL_0 [Default]
//!        - \b PMM_CORE_LEVEL_1
//!        - \b PMM_CORE_LEVEL_2
//!        - \b PMM_CORE_LEVEL_3
//!
//! Modified bits of \b PMMCTL0 register, bits of \b PMMIFG register, bits of
//! \b PMMRIE register, bits of \b SVSMHCTL register and bits of \b SVSMLCTL
//! register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool PMM_setVCore(uint8_t level);

//*****************************************************************************
//
//! \brief Returns interrupt status
//!
//! \param mask is the mask for specifying the required flag
//!        Mask value is the logical OR of any of the following:
//!        - \b PMM_SVSMLDLYIFG
//!        - \b PMM_SVMLIFG
//!        - \b PMM_SVMLVLRIFG
//!        - \b PMM_SVSMHDLYIFG
//!        - \b PMM_SVMHIFG
//!        - \b PMM_SVMHVLRIFG
//!        - \b PMM_PMMBORIFG
//!        - \b PMM_PMMRSTIFG
//!        - \b PMM_PMMPORIFG
//!        - \b PMM_SVSHIFG
//!        - \b PMM_SVSLIFG
//!        - \b PMM_PMMLPM5IFG
//!
//! \return Logical OR of any of the following:
//!         - \b PMM_SVSMLDLYIFG
//!         - \b PMM_SVMLIFG
//!         - \b PMM_SVMLVLRIFG
//!         - \b PMM_SVSMHDLYIFG
//!         - \b PMM_SVMHIFG
//!         - \b PMM_SVMHVLRIFG
//!         - \b PMM_PMMBORIFG
//!         - \b PMM_PMMRSTIFG
//!         - \b PMM_PMMPORIFG
//!         - \b PMM_SVSHIFG
//!         - \b PMM_SVSLIFG
//!         - \b PMM_PMMLPM5IFG
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint16_t PMM_getInterruptStatus(uint16_t mask);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_PMM_H__
