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
// timer_d.h - Driver for the TIMER_D Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_TIMER_D_H__
#define __MSP430WARE_TIMER_D_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_TxD7__

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
// The following is a parameter used for Timer_D_getCounterValue that
// determines the maximum difference in counts of the TDxR register for a
// majority vote.
//
//*****************************************************************************
#define TIMER_D_THRESHOLD                                                    50

//*****************************************************************************
//
//! \brief Used in the Timer_D_initUpMode() function as the param parameter.
//
//*****************************************************************************
typedef struct Timer_D_initUpModeParam {
    //! Selects Clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
    //! - \b TIMER_D_CLOCKSOURCE_ACLK
    //! - \b TIMER_D_CLOCKSOURCE_SMCLK
    //! - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
    uint16_t clockSource;
    //! Is the divider for clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
    uint16_t clockSourceDivider;
    //! Is the selected clock mode register values.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
    //! - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
    //! - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
    uint16_t clockingMode;
    //! Is the specified timer period. This is the value that gets written into
    //! the CCR0. Limited to 16 bits [uint16_t]
    uint16_t timerPeriod;
    //! Is to enable or disable timer interrupt
    //! \n Valid values are:
    //! - \b TIMER_D_TDIE_INTERRUPT_ENABLE
    //! - \b TIMER_D_TDIE_INTERRUPT_DISABLE [Default]
    uint16_t timerInterruptEnable_TDIE;
    //! Is to enable or disable timer CCR0 captureComapre interrupt.
    //! \n Valid values are:
    //! - \b TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE
    //! - \b TIMER_D_CCIE_CCR0_INTERRUPT_DISABLE [Default]
    uint16_t captureCompareInterruptEnable_CCR0_CCIE;
    //! Decides if timer clock divider, count direction, count need to be
    //! reset.
    //! \n Valid values are:
    //! - \b TIMER_D_DO_CLEAR
    //! - \b TIMER_D_SKIP_CLEAR [Default]
    uint16_t timerClear;
} Timer_D_initUpModeParam;

//*****************************************************************************
//
//! \brief Used in the Timer_D_initCompareMode() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct Timer_D_initCompareModeParam {
    //! Selects the Capture register being used.
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
    uint16_t compareRegister;
    //! Is to enable or disable timer captureComapre interrupt.
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE
    //! - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_DISABLE [Default]
    uint16_t compareInterruptEnable;
    //! Specifies the output mode.
    //! \n Valid values are:
    //! - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
    //! - \b TIMER_D_OUTPUTMODE_SET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
    //! - \b TIMER_D_OUTPUTMODE_SET_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE
    //! - \b TIMER_D_OUTPUTMODE_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
    //! - \b TIMER_D_OUTPUTMODE_RESET_SET
    uint16_t compareOutputMode;
    //! Is the count to be compared with in compare mode
    uint16_t compareValue;
} Timer_D_initCompareModeParam;

//*****************************************************************************
//
//! \brief Used in the Timer_D_initContinuousMode() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct Timer_D_initContinuousModeParam {
    //! Selects Clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
    //! - \b TIMER_D_CLOCKSOURCE_ACLK
    //! - \b TIMER_D_CLOCKSOURCE_SMCLK
    //! - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
    uint16_t clockSource;
    //! Is the divider for clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
    uint16_t clockSourceDivider;
    //! Is the selected clock mode register values.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
    //! - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
    //! - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
    uint16_t clockingMode;
    //! Is to enable or disable timer interrupt
    //! \n Valid values are:
    //! - \b TIMER_D_TDIE_INTERRUPT_ENABLE
    //! - \b TIMER_D_TDIE_INTERRUPT_DISABLE [Default]
    uint16_t timerInterruptEnable_TDIE;
    //! Decides if timer clock divider, count direction, count need to be
    //! reset.
    //! \n Valid values are:
    //! - \b TIMER_D_DO_CLEAR
    //! - \b TIMER_D_SKIP_CLEAR [Default]
    uint16_t timerClear;
} Timer_D_initContinuousModeParam;

//*****************************************************************************
//
//! \brief Used in the Timer_D_initHighResGeneratorInRegulatedMode() function
//! as the param parameter.
//
//*****************************************************************************
typedef struct Timer_D_initHighResGeneratorInRegulatedModeParam {
    //! Selects Clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
    //! - \b TIMER_D_CLOCKSOURCE_ACLK
    //! - \b TIMER_D_CLOCKSOURCE_SMCLK
    //! - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
    uint16_t clockSource;
    //! Is the divider for clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
    uint16_t clockSourceDivider;
    //! Is the selected clock mode register values.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
    //! - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
    //! - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
    uint16_t clockingMode;
    //! Selects the high resolution multiply factor.
    //! \n Valid values are:
    //! - \b TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_8x
    //! - \b TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_16x
    uint8_t highResClockMultiplyFactor;
    //! Selects the high resolution divider.
    //! \n Valid values are:
    //! - \b TIMER_D_HIGHRES_CLK_DIVIDER_1
    //! - \b TIMER_D_HIGHRES_CLK_DIVIDER_2
    //! - \b TIMER_D_HIGHRES_CLK_DIVIDER_4
    //! - \b TIMER_D_HIGHRES_CLK_DIVIDER_8
    uint8_t highResClockDivider;
} Timer_D_initHighResGeneratorInRegulatedModeParam;

//*****************************************************************************
//
//! \brief Used in the Timer_D_initUpDownMode() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct Timer_D_initUpDownModeParam {
    //! Selects Clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
    //! - \b TIMER_D_CLOCKSOURCE_ACLK
    //! - \b TIMER_D_CLOCKSOURCE_SMCLK
    //! - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
    uint16_t clockSource;
    //! Is the divider for clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
    uint16_t clockSourceDivider;
    //! Is the selected clock mode register values.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
    //! - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
    //! - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
    uint16_t clockingMode;
    //! Is the specified timer period
    uint16_t timerPeriod;
    //! Is to enable or disable timer interrupt
    //! \n Valid values are:
    //! - \b TIMER_D_TDIE_INTERRUPT_ENABLE
    //! - \b TIMER_D_TDIE_INTERRUPT_DISABLE [Default]
    uint16_t timerInterruptEnable_TDIE;
    //! Is to enable or disable timer CCR0 captureComapre interrupt.
    //! \n Valid values are:
    //! - \b TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE
    //! - \b TIMER_D_CCIE_CCR0_INTERRUPT_DISABLE [Default]
    uint16_t captureCompareInterruptEnable_CCR0_CCIE;
    //! Decides if timer clock divider, count direction, count need to be
    //! reset.
    //! \n Valid values are:
    //! - \b TIMER_D_DO_CLEAR
    //! - \b TIMER_D_SKIP_CLEAR [Default]
    uint16_t timerClear;
} Timer_D_initUpDownModeParam;

//*****************************************************************************
//
//! \brief Used in the Timer_D_combineTDCCRToOutputPWM() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct Timer_D_combineTDCCRToOutputPWMParam {
    //! Selects Clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
    //! - \b TIMER_D_CLOCKSOURCE_ACLK
    //! - \b TIMER_D_CLOCKSOURCE_SMCLK
    //! - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
    uint16_t clockSource;
    //! Is the divider for clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
    uint16_t clockSourceDivider;
    //! Is the selected clock mode register values.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
    //! - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
    //! - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
    uint16_t clockingMode;
    //! Is the specified timer period
    uint16_t timerPeriod;
    //! Selects desired CCR registers to combine
    //! \n Valid values are:
    //! - \b TIMER_D_COMBINE_CCR1_CCR2
    //! - \b TIMER_D_COMBINE_CCR3_CCR4 - (available on Timer_D5, Timer_D7)
    //! - \b TIMER_D_COMBINE_CCR5_CCR6 - (available only on Timer_D7)
    uint16_t combineCCRRegistersCombination;
    //! Specifies the output mode.
    //! \n Valid values are:
    //! - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
    //! - \b TIMER_D_OUTPUTMODE_SET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
    //! - \b TIMER_D_OUTPUTMODE_SET_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE
    //! - \b TIMER_D_OUTPUTMODE_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
    //! - \b TIMER_D_OUTPUTMODE_RESET_SET
    uint16_t compareOutputMode;
    //! Specifies the dutycycle for the generated waveform
    uint16_t dutyCycle1;
    //! Specifies the dutycycle for the generated waveform
    uint16_t dutyCycle2;
} Timer_D_combineTDCCRToOutputPWMParam;

//*****************************************************************************
//
//! \brief Used in the Timer_D_initCaptureMode() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct Timer_D_initCaptureModeParam {
    //! Selects the Capture register being used. Refer to datasheet to ensure
    //! the device has the capture compare register being used
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
    uint16_t captureRegister;
    //! Is the capture mode selected.
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTUREMODE_NO_CAPTURE [Default]
    //! - \b TIMER_D_CAPTUREMODE_RISING_EDGE
    //! - \b TIMER_D_CAPTUREMODE_FALLING_EDGE
    //! - \b TIMER_D_CAPTUREMODE_RISING_AND_FALLING_EDGE
    uint16_t captureMode;
    //! Decides the Input Select
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTURE_INPUTSELECT_CCIxA [Default]
    //! - \b TIMER_D_CAPTURE_INPUTSELECT_CCIxB
    //! - \b TIMER_D_CAPTURE_INPUTSELECT_GND
    //! - \b TIMER_D_CAPTURE_INPUTSELECT_Vcc
    uint16_t captureInputSelect;
    //! Decides if capture source should be synchronized with timer clock
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTURE_ASYNCHRONOUS [Default]
    //! - \b TIMER_D_CAPTURE_SYNCHRONOUS
    uint16_t synchronizeCaptureSource;
    //! Is to enable or disabel capture interrupt
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTURE_INTERRUPT_ENABLE
    //! - \b TIMER_D_CAPTURE_INTERRUPT_DISABLE [Default]
    uint16_t captureInterruptEnable;
    //! Specifies the output mode.
    //! \n Valid values are:
    //! - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
    //! - \b TIMER_D_OUTPUTMODE_SET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
    //! - \b TIMER_D_OUTPUTMODE_SET_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE
    //! - \b TIMER_D_OUTPUTMODE_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
    //! - \b TIMER_D_OUTPUTMODE_RESET_SET
    uint16_t captureOutputMode;
    //! Specifies single/dual capture mode.
    //! \n Valid values are:
    //! - \b TIMER_D_SINGLE_CAPTURE_MODE - value],
    //! - \b TIMER_D_DUAL_CAPTURE_MODE
    uint8_t channelCaptureMode;
} Timer_D_initCaptureModeParam;

//*****************************************************************************
//
//! \brief Used in the Timer_D_outputPWM() function as the param parameter.
//
//*****************************************************************************
typedef struct Timer_D_outputPWMParam {
    //! Selects Clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
    //! - \b TIMER_D_CLOCKSOURCE_ACLK
    //! - \b TIMER_D_CLOCKSOURCE_SMCLK
    //! - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
    uint16_t clockSource;
    //! Is the divider for clock source.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
    //! - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
    uint16_t clockSourceDivider;
    //! Is the selected clock mode register values.
    //! \n Valid values are:
    //! - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
    //! - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
    //! - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
    uint16_t clockingMode;
    //! Is the specified timer period
    uint16_t timerPeriod;
    //! Selects the compare register being used.
    //! \n Valid values are:
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
    //! - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
    uint16_t compareRegister;
    //! Specifies the output mode.
    //! \n Valid values are:
    //! - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
    //! - \b TIMER_D_OUTPUTMODE_SET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
    //! - \b TIMER_D_OUTPUTMODE_SET_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE
    //! - \b TIMER_D_OUTPUTMODE_RESET
    //! - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
    //! - \b TIMER_D_OUTPUTMODE_RESET_SET
    uint16_t compareOutputMode;
    //! Specifies the dutycycle for the generated waveform
    uint16_t dutyCycle;
} Timer_D_outputPWMParam;


//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initContinuousMode(), Timer_D_initUpMode(),
// Timer_D_initUpDownMode(), Timer_D_outputPWM(),
// Timer_D_initHighResGeneratorInRegulatedMode(), and
// Timer_D_combineTDCCRToOutputPWM().
//
//*****************************************************************************
#define TIMER_D_CLOCKSOURCE_DIVIDER_1                                      0x00
#define TIMER_D_CLOCKSOURCE_DIVIDER_2                                      0x08
#define TIMER_D_CLOCKSOURCE_DIVIDER_3                                      0x02
#define TIMER_D_CLOCKSOURCE_DIVIDER_4                                      0x10
#define TIMER_D_CLOCKSOURCE_DIVIDER_5                                      0x04
#define TIMER_D_CLOCKSOURCE_DIVIDER_6                                      0x05
#define TIMER_D_CLOCKSOURCE_DIVIDER_7                                      0x06
#define TIMER_D_CLOCKSOURCE_DIVIDER_8                                      0x18
#define TIMER_D_CLOCKSOURCE_DIVIDER_10                                     0x0C
#define TIMER_D_CLOCKSOURCE_DIVIDER_12                                     0x0D
#define TIMER_D_CLOCKSOURCE_DIVIDER_14                                     0x0E
#define TIMER_D_CLOCKSOURCE_DIVIDER_16                                     0x0F
#define TIMER_D_CLOCKSOURCE_DIVIDER_20                                     0x14
#define TIMER_D_CLOCKSOURCE_DIVIDER_24                                     0x15
#define TIMER_D_CLOCKSOURCE_DIVIDER_28                                     0x16
#define TIMER_D_CLOCKSOURCE_DIVIDER_32                                     0x17
#define TIMER_D_CLOCKSOURCE_DIVIDER_40                                     0x1C
#define TIMER_D_CLOCKSOURCE_DIVIDER_48                                     0x1D
#define TIMER_D_CLOCKSOURCE_DIVIDER_56                                     0x1E
#define TIMER_D_CLOCKSOURCE_DIVIDER_64                                     0x1F

//*****************************************************************************
//
// The following are values that can be passed to the timerMode parameter for
// functions: Timer_D_startCounter().
//
//*****************************************************************************
#define TIMER_D_STOP_MODE                                                  MC_0
#define TIMER_D_UP_MODE                                                    MC_1
#define TIMER_D_CONTINUOUS_MODE                                            MC_2
#define TIMER_D_UPDOWN_MODE                                                MC_3

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initContinuousMode(), Timer_D_initUpMode(), and
// Timer_D_initUpDownMode().
//
//*****************************************************************************
#define TIMER_D_DO_CLEAR                                                  TDCLR
#define TIMER_D_SKIP_CLEAR                                                 0x00

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initContinuousMode(), Timer_D_initUpMode(),
// Timer_D_initUpDownMode(), Timer_D_outputPWM(),
// Timer_D_initHighResGeneratorInRegulatedMode(), and
// Timer_D_combineTDCCRToOutputPWM().
//
//*****************************************************************************
#define TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK                        TDSSEL__TACLK
#define TIMER_D_CLOCKSOURCE_ACLK                                   TDSSEL__ACLK
#define TIMER_D_CLOCKSOURCE_SMCLK                                 TDSSEL__SMCLK
#define TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK               TDSSEL__INCLK

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initContinuousMode(), Timer_D_initUpMode(),
// Timer_D_initUpDownMode(), Timer_D_outputPWM(),
// Timer_D_initHighResGeneratorInRegulatedMode(), and
// Timer_D_combineTDCCRToOutputPWM().
//
//*****************************************************************************
#define TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK                            TDCLKM_0
#define TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK                         TDCLKM_1
#define TIMER_D_CLOCKINGMODE_AUXILIARY_CLK                             TDCLKM_2

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initContinuousMode(), Timer_D_initUpMode(), and
// Timer_D_initUpDownMode().
//
//*****************************************************************************
#define TIMER_D_TDIE_INTERRUPT_ENABLE                                      TDIE
#define TIMER_D_TDIE_INTERRUPT_DISABLE                                     0x00

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initCaptureMode().
//
//*****************************************************************************
#define TIMER_D_CAPTURE_INTERRUPT_ENABLE                                   TDIE
#define TIMER_D_CAPTURE_INTERRUPT_DISABLE                                  0x00

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initUpMode(), and Timer_D_initUpDownMode().
//
//*****************************************************************************
#define TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE                                 CCIE
#define TIMER_D_CCIE_CCR0_INTERRUPT_DISABLE                                0x00

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initCaptureMode().
//
//*****************************************************************************
#define TIMER_D_SINGLE_CAPTURE_MODE                                        0x00
#define TIMER_D_DUAL_CAPTURE_MODE                                          0x01

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initCaptureMode().
//
//*****************************************************************************
#define TIMER_D_CAPTURE_INPUTSELECT_CCIxA                                CCIS_0
#define TIMER_D_CAPTURE_INPUTSELECT_CCIxB                                CCIS_1
#define TIMER_D_CAPTURE_INPUTSELECT_GND                                  CCIS_2
#define TIMER_D_CAPTURE_INPUTSELECT_Vcc                                  CCIS_3

//*****************************************************************************
//
// The following are values that can be passed to the compareOutputMode
// parameter for functions: Timer_D_setOutputMode(); the param parameter for
// functions: Timer_D_initCaptureMode(), Timer_D_initCompareMode(),
// Timer_D_outputPWM(), and Timer_D_combineTDCCRToOutputPWM().
//
//*****************************************************************************
#define TIMER_D_OUTPUTMODE_OUTBITVALUE                                 OUTMOD_0
#define TIMER_D_OUTPUTMODE_SET                                         OUTMOD_1
#define TIMER_D_OUTPUTMODE_TOGGLE_RESET                                OUTMOD_2
#define TIMER_D_OUTPUTMODE_SET_RESET                                   OUTMOD_3
#define TIMER_D_OUTPUTMODE_TOGGLE                                      OUTMOD_4
#define TIMER_D_OUTPUTMODE_RESET                                       OUTMOD_5
#define TIMER_D_OUTPUTMODE_TOGGLE_SET                                  OUTMOD_6
#define TIMER_D_OUTPUTMODE_RESET_SET                                   OUTMOD_7

//*****************************************************************************
//
// The following are values that can be passed to the compareRegister parameter
// for functions: Timer_D_setCompareValue(),
// Timer_D_initCompareLatchLoadEvent(), and Timer_D_setOutputMode(); the
// captureCompareRegister parameter for functions:
// Timer_D_enableCaptureCompareInterrupt(),
// Timer_D_disableCaptureCompareInterrupt(),
// Timer_D_getCaptureCompareInterruptStatus(),
// Timer_D_getSynchronizedCaptureCompareInput(),
// Timer_D_getOutputForOutputModeOutBitValue(),
// Timer_D_getCaptureCompareCount(), Timer_D_getCaptureCompareLatchCount(),
// Timer_D_getCaptureCompareInputSignal(),
// Timer_D_setOutputForOutputModeOutBitValue(), and
// Timer_D_clearCaptureCompareInterrupt(); the param parameter for functions:
// Timer_D_initCaptureMode(), Timer_D_outputPWM(), and
// Timer_D_initCompareMode().
//
//*****************************************************************************
#define TIMER_D_CAPTURECOMPARE_REGISTER_0                                  0x08
#define TIMER_D_CAPTURECOMPARE_REGISTER_1                                  0x0E
#define TIMER_D_CAPTURECOMPARE_REGISTER_2                                  0x14
#define TIMER_D_CAPTURECOMPARE_REGISTER_3                                  0x1A
#define TIMER_D_CAPTURECOMPARE_REGISTER_4                                  0x20
#define TIMER_D_CAPTURECOMPARE_REGISTER_5                                  0x28
#define TIMER_D_CAPTURECOMPARE_REGISTER_6                                  0x2E

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initCaptureMode().
//
//*****************************************************************************
#define TIMER_D_CAPTUREMODE_NO_CAPTURE                                     CM_0
#define TIMER_D_CAPTUREMODE_RISING_EDGE                                    CM_1
#define TIMER_D_CAPTUREMODE_FALLING_EDGE                                   CM_2
#define TIMER_D_CAPTUREMODE_RISING_AND_FALLING_EDGE                        CM_3

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initCaptureMode().
//
//*****************************************************************************
#define TIMER_D_CAPTURE_ASYNCHRONOUS                                       0x00
#define TIMER_D_CAPTURE_SYNCHRONOUS                                         SCS

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initCompareMode().
//
//*****************************************************************************
#define TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE                            CCIE
#define TIMER_D_CAPTURECOMPARE_INTERRUPT_DISABLE                           0x00

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: Timer_D_enableHighResInterrupt(),
// Timer_D_disableHighResInterrupt(), Timer_D_getHighResInterruptStatus(), and
// Timer_D_clearHighResInterrupt() as well as returned by the
// Timer_D_getHighResInterruptStatus() function.
//
//*****************************************************************************
#define TIMER_D_HIGH_RES_FREQUENCY_UNLOCK                             TDHUNLKIE
#define TIMER_D_HIGH_RES_FREQUENCY_LOCK                                 TDHLKIE
#define TIMER_D_HIGH_RES_FAIL_HIGH                                      TDHFHIE
#define TIMER_D_HIGH_RES_FAIL_LOW                                       TDHFLIE

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: Timer_D_getCaptureCompareInterruptStatus() as well as returned by
// the Timer_D_getCaptureCompareInterruptStatus() function.
//
//*****************************************************************************
#define TIMER_D_CAPTURE_OVERFLOW                                            COV
#define TIMER_D_CAPTURECOMPARE_INTERRUPT_FLAG                             CCIFG

//*****************************************************************************
//
// The following are values that can be passed to the synchronized parameter
// for functions: Timer_D_getSynchronizedCaptureCompareInput().
//
//*****************************************************************************
#define TIMER_D_READ_SYNCHRONIZED_CAPTURECOMPAREINPUT                      SCCI
#define TIMER_D_READ_CAPTURE_COMPARE_INPUT                                  CCI

//*****************************************************************************
//
// The following are values that can be passed to the outputModeOutBitValue
// parameter for functions: Timer_D_setOutputForOutputModeOutBitValue() as well
// as returned by the Timer_D_getOutputForOutputModeOutBitValue() function.
//
//*****************************************************************************
#define TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH                                 OUT
#define TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW                                 0x00

//*****************************************************************************
//
// The following are values that can be passed to the desiredHighResFrequency
// parameter for functions: Timer_D_initHighResGeneratorInFreeRunningMode().
//
//*****************************************************************************
#define TIMER_D_HIGHRES_64MHZ                                              0x00
#define TIMER_D_HIGHRES_128MHZ                                             0x01
#define TIMER_D_HIGHRES_200MHZ                                             0x02
#define TIMER_D_HIGHRES_256MHZ                                             0x03

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initHighResGeneratorInRegulatedMode().
//
//*****************************************************************************
#define TIMER_D_HIGHRES_CLK_DIVIDER_1                                   TDHD__1
#define TIMER_D_HIGHRES_CLK_DIVIDER_2                                   TDHD__2
#define TIMER_D_HIGHRES_CLK_DIVIDER_4                                   TDHD__4
#define TIMER_D_HIGHRES_CLK_DIVIDER_8                                   TDHD__8

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_initHighResGeneratorInRegulatedMode().
//
//*****************************************************************************
#define TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_8x                           TDHM_0
#define TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_16x                          TDHM_1

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Timer_D_combineTDCCRToOutputPWM().
//
//*****************************************************************************
#define TIMER_D_COMBINE_CCR1_CCR2                                             2
#define TIMER_D_COMBINE_CCR3_CCR4                                             4
#define TIMER_D_COMBINE_CCR5_CCR6                                             6

//*****************************************************************************
//
// The following are values that can be passed to the groupLatch parameter for
// functions: Timer_D_selectLatchingGroup().
//
//*****************************************************************************
#define TIMER_D_GROUP_NONE                                            TDCLGRP_0
#define TIMER_D_GROUP_CL12_CL23_CL56                                  TDCLGRP_1
#define TIMER_D_GROUP_CL123_CL456                                     TDCLGRP_2
#define TIMER_D_GROUP_ALL                                             TDCLGRP_3

//*****************************************************************************
//
// The following are values that can be passed to the counterLength parameter
// for functions: Timer_D_selectCounterLength().
//
//*****************************************************************************
#define TIMER_D_COUNTER_16BIT                                            CNTL_0
#define TIMER_D_COUNTER_12BIT                                            CNTL_1
#define TIMER_D_COUNTER_10BIT                                            CNTL_2
#define TIMER_D_COUNTER_8BIT                                             CNTL_3

//*****************************************************************************
//
// The following are values that can be passed to the compareLatchLoadEvent
// parameter for functions: Timer_D_initCompareLatchLoadEvent().
//
//*****************************************************************************
#define TIMER_D_LATCH_ON_WRITE_TO_TDxCCRn_COMPARE_REGISTER               CLLD_0
#define TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UP_OR_CONT_MODE        CLLD_1
#define TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UPDOWN_MODE            CLLD_2
#define TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_CURRENT_COMPARE_LATCH_VALUE CLLD_3

//*****************************************************************************
//
// The following are values that can be passed to the highResCoarseClockRange
// parameter for functions: Timer_D_selectHighResCoarseClockRange().
//
//*****************************************************************************
#define TIMER_D_HIGHRES_BELOW_15MHz                                        0x00
#define TIMER_D_HIGHRES_ABOVE_15MHz                                    TDHCLKCR

//*****************************************************************************
//
// The following are values that can be passed to the highResClockRange
// parameter for functions: Timer_D_selectHighResClockRange().
//
//*****************************************************************************
#define TIMER_D_CLOCK_RANGE0                                             0x0000
#define TIMER_D_CLOCK_RANGE1                                             0x2000
#define TIMER_D_CLOCK_RANGE2                                             0x4000

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the Timer_D_getSynchronizedCaptureCompareInput()
// function.
//
//*****************************************************************************
#define TIMER_D_CAPTURECOMPARE_INPUT_HIGH                                  0x01
#define TIMER_D_CAPTURECOMPARE_INPUT_LOW                                   0x00

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the Timer_D_getTimerInterruptStatus() function.
//
//*****************************************************************************
#define TIMER_D_INTERRUPT_NOT_PENDING                                      0x00
#define TIMER_D_INTERRUPT_PENDING                                          0x01

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the Timer_D_getCaptureCompareInputSignal() function.
//
//*****************************************************************************
#define TIMER_D_CAPTURECOMPARE_INPUT                                        CCI

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Starts Timer_D counter
//!
//! NOTE: This function assumes that the timer has been previously configured
//! using Timer_D_initContinuousMode, Timer_D_initUpMode or
//! Timer_D_initUpDownMode.
//!
//! \param baseAddress is the base address of the TIMER_DA module.
//! \param timerMode selects the mode of the timer
//!        Valid values are:
//!        - \b TIMER_D_STOP_MODE
//!        - \b TIMER_D_UP_MODE
//!        - \b TIMER_D_CONTINUOUS_MODE [Default]
//!        - \b TIMER_D_UPDOWN_MODE
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_startCounter(uint16_t baseAddress,
                                 uint16_t timerMode);

//*****************************************************************************
//
//! \brief Configures timer in continuous mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the Timer_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for continuous mode initialization.
//!
//! Modified bits of \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_initContinuousMode(uint16_t baseAddress,
                                       Timer_D_initContinuousModeParam *param);

//*****************************************************************************
//
//! \brief Configures timer in up mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the Timer_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for up mode initialization.
//!
//! Modified bits of \b TDxCCR0 register, bits of \b TDxCCTL0 register, bits of
//! \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_initUpMode(uint16_t baseAddress,
                               Timer_D_initUpModeParam *param);

//*****************************************************************************
//
//! \brief Configures timer in up down mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the Timer_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for up-down mode initialization.
//!
//! Modified bits of \b TDxCCR0 register, bits of \b TDxCCTL0 register, bits of
//! \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_initUpDownMode(uint16_t baseAddress,
                                   Timer_D_initUpDownModeParam *param);

//*****************************************************************************
//
//! \brief Initializes Capture Mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for capture mode initialization.
//!
//! Modified bits of \b TDxCCTLn register and bits of \b TDxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_initCaptureMode(uint16_t baseAddress,
                                    Timer_D_initCaptureModeParam *param);

//*****************************************************************************
//
//! \brief Initializes Compare Mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for compare mode initialization.
//!
//! Modified bits of \b TDxCCTLn register and bits of \b TDxCCRn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_initCompareMode(uint16_t baseAddress,
                                    Timer_D_initCompareModeParam *param);

//*****************************************************************************
//
//! \brief Enable timer interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_enableTimerInterrupt(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enable High Resolution interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask of interrupts to enable
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_enableHighResInterrupt(uint16_t baseAddress,
                                           uint16_t mask);

//*****************************************************************************
//
//! \brief Disable timer interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_disableTimerInterrupt(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disable High Resolution interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask of interrupts to disable
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_disableHighResInterrupt(uint16_t baseAddress,
                                            uint16_t mask);

//*****************************************************************************
//
//! \brief Get timer interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! \return One of the following:
//!         - \b TIMER_D_INTERRUPT_NOT_PENDING
//!         - \b TIMER_D_INTERRUPT_PENDING
//!         \n indicating the timer interrupt status
//
//*****************************************************************************
extern uint32_t Timer_D_getTimerInterruptStatus(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enable capture compare interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_enableCaptureCompareInterrupt(uint16_t baseAddress,
                                                  uint16_t captureCompareRegister);

//*****************************************************************************
//
//! \brief Disable capture compare interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_disableCaptureCompareInterrupt(uint16_t baseAddress,
                                                   uint16_t captureCompareRegister);

//*****************************************************************************
//
//! \brief Return capture compare interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param mask is the mask for the interrupt status
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_CAPTURE_OVERFLOW
//!        - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_FLAG
//!
//! \return Logical OR of any of the following:
//!         - \b TIMER_D_CAPTURE_OVERFLOW
//!         - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_FLAG
//!         \n indicating the status of the masked flags
//
//*****************************************************************************
extern uint32_t Timer_D_getCaptureCompareInterruptStatus(uint16_t baseAddress,
                                                         uint16_t captureCompareRegister,
                                                         uint16_t mask);

//*****************************************************************************
//
//! \brief Returns High Resolution interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask for the interrupt status
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return Logical OR of any of the following:
//!         - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!         - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!         - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!         - \b TIMER_D_HIGH_RES_FAIL_LOW
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint16_t Timer_D_getHighResInterruptStatus(uint16_t baseAddress,
                                                  uint16_t mask);

//*****************************************************************************
//
//! \brief Reset/Clear the timer clock divider, count direction, count
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_clear(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Clears High Resolution interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask for the interrupts to clear
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_clearHighResInterrupt(uint16_t baseAddress,
                                          uint16_t mask);

//*****************************************************************************
//
//! \brief Get synchronized capturecompare input
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param synchronized is to select type of capture compare input.
//!        Valid values are:
//!        - \b TIMER_D_READ_SYNCHRONIZED_CAPTURECOMPAREINPUT
//!        - \b TIMER_D_READ_CAPTURE_COMPARE_INPUT
//!
//! \return One of the following:
//!         - \b TIMER_D_CAPTURECOMPARE_INPUT_HIGH
//!         - \b TIMER_D_CAPTURECOMPARE_INPUT_LOW
//
//*****************************************************************************
extern uint8_t Timer_D_getSynchronizedCaptureCompareInput(uint16_t baseAddress,
                                                          uint16_t captureCompareRegister,
                                                          uint16_t synchronized);

//*****************************************************************************
//
//! \brief Get output bit for output mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return One of the following:
//!         - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH
//!         - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW
//
//*****************************************************************************
extern uint8_t Timer_D_getOutputForOutputModeOutBitValue(uint16_t baseAddress,
                                                         uint16_t captureCompareRegister);

//*****************************************************************************
//
//! \brief Get current capturecompare count
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return current count as uint16_t
//
//*****************************************************************************
extern uint16_t Timer_D_getCaptureCompareCount(uint16_t baseAddress,
                                               uint16_t captureCompareRegister);

//*****************************************************************************
//
//! \brief Get current capture compare latch register count
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return current count as uint16_t
//
//*****************************************************************************
extern uint16_t Timer_D_getCaptureCompareLatchCount(uint16_t baseAddress,
                                                    uint16_t captureCompareRegister);

//*****************************************************************************
//
//! \brief Get current capturecompare input signal
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return One of the following:
//!         - \b TIMER_D_CAPTURECOMPARE_INPUT
//!         - \b 0x00
//!         \n indicating the current input signal
//
//*****************************************************************************
extern uint8_t Timer_D_getCaptureCompareInputSignal(uint16_t baseAddress,
                                                    uint16_t captureCompareRegister);

//*****************************************************************************
//
//! \brief Set output bit for output mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param outputModeOutBitValue the value to be set for out bit
//!        Valid values are:
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW
//!
//! Modified bits of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_setOutputForOutputModeOutBitValue(uint16_t baseAddress,
                                                      uint16_t captureCompareRegister,
                                                      uint8_t outputModeOutBitValue);

//*****************************************************************************
//
//! \brief Generate a PWM with timer running in up mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for PWM configuration.
//!
//! Modified bits of \b TDxCCTLn register, bits of \b TDxCCR0 register, bits of
//! \b TDxCCTL0 register, bits of \b TDxCTL0 register and bits of \b TDxCTL1
//! register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_outputPWM(uint16_t baseAddress,
                              Timer_D_outputPWMParam *param);

//*****************************************************************************
//
//! \brief Stops the timer
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_stop(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets the value of the capture-compare register
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param compareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param compareValue is the count to be compared with in compare mode
//!
//! Modified bits of \b TDxCCRn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_setCompareValue(uint16_t baseAddress,
                                    uint16_t compareRegister,
                                    uint16_t compareValue);

//*****************************************************************************
//
//! \brief Clears the Timer TDIFG interrupt flag
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDIFG of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_clearTimerInterrupt(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Clears the capture-compare interrupt flag
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture-compare register being
//!        used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits are \b CCIFG of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_clearCaptureCompareInterrupt(uint16_t baseAddress,
                                                 uint16_t captureCompareRegister);

//*****************************************************************************
//
//! \brief Configures Timer_D in free running mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param desiredHighResFrequency selects the desired High Resolution
//!        frequency used.
//!        Valid values are:
//!        - \b TIMER_D_HIGHRES_64MHZ
//!        - \b TIMER_D_HIGHRES_128MHZ
//!        - \b TIMER_D_HIGHRES_200MHZ
//!        - \b TIMER_D_HIGHRES_256MHZ
//!
//! Modified bits of \b TDxHCTL1 register, bits of \b TDxHCTL0 register and
//! bits of \b TDxCTL1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern uint8_t Timer_D_initHighResGeneratorInFreeRunningMode(uint16_t baseAddress,
                                                             uint8_t desiredHighResFrequency);

//*****************************************************************************
//
//! \brief Configures Timer_D in Regulated mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for high resolution generator in
//!        regulated mode.
//!
//! Modified bits of \b TDxHCTL0 register, bits of \b TDxCTL0 register and bits
//! of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_initHighResGeneratorInRegulatedMode(uint16_t baseAddress,
                                                        Timer_D_initHighResGeneratorInRegulatedModeParam *param);

//*****************************************************************************
//
//! \brief Combine TDCCR to get PWM
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for PWM generation using two CCRs.
//!
//! Modified bits of \b TDxCCTLn register, bits of \b TDxCCR0 register, bits of
//! \b TDxCCTL0 register, bits of \b TDxCTL0 register and bits of \b TDxCTL1
//! register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_combineTDCCRToOutputPWM(uint16_t baseAddress,
                                            Timer_D_combineTDCCRToOutputPWMParam *param);

//*****************************************************************************
//
//! \brief Selects Timer_D Latching Group
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param groupLatch selects the group latch
//!        Valid values are:
//!        - \b TIMER_D_GROUP_NONE [Default]
//!        - \b TIMER_D_GROUP_CL12_CL23_CL56
//!        - \b TIMER_D_GROUP_CL123_CL456
//!        - \b TIMER_D_GROUP_ALL
//!
//! Modified bits are \b TDCLGRP of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_selectLatchingGroup(uint16_t baseAddress,
                                        uint16_t groupLatch);

//*****************************************************************************
//
//! \brief Selects Timer_D counter length
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param counterLength selects the value of counter length.
//!        Valid values are:
//!        - \b TIMER_D_COUNTER_16BIT [Default]
//!        - \b TIMER_D_COUNTER_12BIT
//!        - \b TIMER_D_COUNTER_10BIT
//!        - \b TIMER_D_COUNTER_8BIT
//!
//! Modified bits are \b CNTL of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_selectCounterLength(uint16_t baseAddress,
                                        uint16_t counterLength);

//*****************************************************************************
//
//! \brief Selects Compare Latch Load Event
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param compareRegister selects the compare register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param compareLatchLoadEvent selects the latch load event
//!        Valid values are:
//!        - \b TIMER_D_LATCH_ON_WRITE_TO_TDxCCRn_COMPARE_REGISTER [Default]
//!        - \b TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UP_OR_CONT_MODE
//!        - \b TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UPDOWN_MODE
//!        - \b
//!           TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_CURRENT_COMPARE_LATCH_VALUE
//!
//! Modified bits are \b CLLD of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_initCompareLatchLoadEvent(uint16_t baseAddress,
                                              uint16_t compareRegister,
                                              uint16_t compareLatchLoadEvent);

//*****************************************************************************
//
//! \brief Disable High Resolution fast wakeup
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHFW of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_disableHighResFastWakeup(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enable High Resolution fast wakeup
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHFW of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_enableHighResFastWakeup(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disable High Resolution Clock Enhanced Accuracy
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHEAEN of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_disableHighResClockEnhancedAccuracy(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enable High Resolution Clock Enhanced Accuracy
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHEAEN of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_enableHighResClockEnhancedAccuracy(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disable High Resolution Clock Enhanced Accuracy
//!
//! High-resolution generator is on if the Timer_D counter
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHRON of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_disableHighResGeneratorForceON(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enable High Resolution Clock Enhanced Accuracy
//!
//! High-resolution generator is on in all Timer_D MCx modes. The PMM remains
//! in high-current mode.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHRON of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_enableHighResGeneratorForceON(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Select High Resolution Coarse Clock Range
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param highResCoarseClockRange selects the High Resolution Coarse Clock
//!        Range
//!        Valid values are:
//!        - \b TIMER_D_HIGHRES_BELOW_15MHz [Default]
//!        - \b TIMER_D_HIGHRES_ABOVE_15MHz
//!
//! Modified bits are \b TDHCLKCR of \b TDxHCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_selectHighResCoarseClockRange(uint16_t baseAddress,
                                                  uint16_t highResCoarseClockRange);

//*****************************************************************************
//
//! \brief Select High Resolution Clock Range Selection
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param highResClockRange selects the High Resolution Clock Range. Refer to
//!        datasheet for frequency details
//!        Valid values are:
//!        - \b TIMER_D_CLOCK_RANGE0 [Default]
//!        - \b TIMER_D_CLOCK_RANGE1
//!        - \b TIMER_D_CLOCK_RANGE2
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_selectHighResClockRange(uint16_t baseAddress,
                                            uint16_t highResClockRange);

//*****************************************************************************
//
//! \brief Reads the current timer count value
//!
//! Reads the current count value of the timer. There is a majority vote system
//! in place to confirm an accurate value is returned. The Timer_D_THRESHOLD
//! #define in the corresponding header file can be modified so that the votes
//! must be closer together for a consensus to occur.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! \return Majority vote of timer count value
//
//*****************************************************************************
extern uint16_t Timer_D_getCounterValue(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets the output mode
//!
//! Sets the output mode for the timer even the timer is already running.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param compareRegister selects the compare register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param compareOutputMode specifies the output mode.
//!        Valid values are:
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
//!        - \b TIMER_D_OUTPUTMODE_SET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
//!        - \b TIMER_D_OUTPUTMODE_SET_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE
//!        - \b TIMER_D_OUTPUTMODE_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
//!        - \b TIMER_D_OUTPUTMODE_RESET_SET
//!
//! Modified bits are \b OUTMOD of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
extern void Timer_D_setOutputMode(uint16_t baseAddress,
                                  uint16_t compareRegister,
                                  uint16_t compareOutputMode);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_TIMER_D_H__
