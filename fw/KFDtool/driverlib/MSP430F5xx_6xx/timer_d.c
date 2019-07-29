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
// timer_d.c - Driver for the timer_d Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup timer_d_api timer_d
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_TxD7__
#include "timer_d.h"

#include <assert.h>

#include "tlv.h"
void Timer_D_startCounter ( uint16_t baseAddress,
    uint16_t timerMode
    )
{
    HWREG16(baseAddress + OFS_TDxCTL0) |= timerMode;
}

void Timer_D_initContinuousMode(uint16_t baseAddress,
    Timer_D_initContinuousModeParam *param)
{
    HWREG16(baseAddress +
        OFS_TDxCTL0) &= ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
                         TIMER_D_UPDOWN_MODE +
                         TIMER_D_DO_CLEAR +
                         TIMER_D_TDIE_INTERRUPT_ENABLE +
                         ID__8
                         );
    HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

    HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
    HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                          ((param->clockSourceDivider&0x7)<<8));

    HWREG16(baseAddress + OFS_TDxCTL0) |= (param->timerClear +
                                           param->timerInterruptEnable_TDIE +
                                           ((param->clockSourceDivider>>3)<<6));
}

void Timer_D_initUpMode(uint16_t baseAddress, Timer_D_initUpModeParam *param)
{
    HWREG16(baseAddress + OFS_TDxCTL0) &=
        ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
          TIMER_D_UPDOWN_MODE +
          TIMER_D_DO_CLEAR +
          TIMER_D_TDIE_INTERRUPT_ENABLE +
          ID__8
          );
    HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

    HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
    HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                           ((param->clockSourceDivider&0x7)<<8));

    HWREG16(baseAddress + OFS_TDxCTL0) |= (TIMER_D_STOP_MODE +
                                          param->timerClear +
                                          param->timerInterruptEnable_TDIE +
                                          ((param->clockSourceDivider>>3)<<6));

    if (TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE ==
        param->captureCompareInterruptEnable_CCR0_CCIE){
        HWREG16(baseAddress + OFS_TDxCCTL0)  |= TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;
    } else   {
        HWREG16(baseAddress + OFS_TDxCCTL0)  &= ~TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;
    }

    HWREG16(baseAddress + OFS_TDxCCR0) = param->timerPeriod;
}

void Timer_D_initUpDownMode(uint16_t baseAddress,
    Timer_D_initUpDownModeParam *param)
{
    HWREG16(baseAddress + OFS_TDxCTL0) &=
        ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
          TIMER_D_UPDOWN_MODE +
          TIMER_D_DO_CLEAR +
          TIMER_D_TDIE_INTERRUPT_ENABLE +
          ID__8
          );
    HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

    HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
    HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                           ((param->clockSourceDivider&0x7)<<8));

    HWREG16(baseAddress + OFS_TDxCTL0)  |= (TIMER_D_STOP_MODE +
                                          param->timerClear +
                                          param->timerInterruptEnable_TDIE +
                                          ((param->clockSourceDivider>>3)<<6));

    if (TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE ==
        param->captureCompareInterruptEnable_CCR0_CCIE){
        HWREG16(baseAddress + OFS_TDxCCTL0)  |= TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;
    } else   {
        HWREG16(baseAddress + OFS_TDxCCTL0)  &= ~TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;
    }

    HWREG16(baseAddress + OFS_TDxCCR0)  = param->timerPeriod;
}

void Timer_D_initCaptureMode(uint16_t baseAddress,
    Timer_D_initCaptureModeParam *param)
{

    HWREG16(baseAddress + param->captureRegister ) |= CAP;

    HWREG8(baseAddress + OFS_TDxCTL2) |=
            (param->channelCaptureMode << ((param->captureRegister - TIMER_D_CAPTURECOMPARE_REGISTER_0)/6));

    HWREG16(baseAddress + param->captureRegister) &=
        ~(TIMER_D_CAPTUREMODE_RISING_AND_FALLING_EDGE +
          TIMER_D_CAPTURE_INPUTSELECT_Vcc +
          TIMER_D_CAPTURE_SYNCHRONOUS +
          TIMER_D_DO_CLEAR +
          TIMER_D_TDIE_INTERRUPT_ENABLE +
          CM_3
          );

    HWREG16(baseAddress + param->captureRegister) |= (param->captureMode +
                                              param->captureInputSelect +
                                              param->synchronizeCaptureSource +
                                              param->captureInterruptEnable +
                                              param->captureOutputMode
                                              );
}

void Timer_D_initCompareMode(uint16_t baseAddress,
    Timer_D_initCompareModeParam *param)
{
    HWREG16(baseAddress + param->compareRegister ) &= ~CAP;

    HWREG16(baseAddress + param->compareRegister) &=
        ~(TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE +
          TIMER_D_OUTPUTMODE_RESET_SET
          );

    HWREG16(baseAddress + param->compareRegister) |= (param->compareInterruptEnable +
                                               param->compareOutputMode
                                               );

    HWREG16(baseAddress + param->compareRegister + 2) = param->compareValue;
}

void Timer_D_enableTimerInterrupt (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_TDxCTL0) &=  ~TDIFG;
    HWREG8(baseAddress + OFS_TDxCTL0) |= TDIE;
}

void Timer_D_enableHighResInterrupt (uint16_t baseAddress,
                uint16_t mask)
{
    HWREG16(baseAddress + OFS_TDxHINT) &=  ~(mask >> 8);
    HWREG16(baseAddress + OFS_TDxHINT) |= mask;
}

void Timer_D_disableTimerInterrupt (uint16_t baseAddress)
{
    HWREG8(baseAddress + OFS_TDxCTL0) &= ~TDIE;
}

void Timer_D_disableHighResInterrupt (uint16_t baseAddress,
                        uint16_t mask)
{
    HWREG16(baseAddress + OFS_TDxHINT) &= ~mask;
}

uint32_t Timer_D_getTimerInterruptStatus (uint16_t baseAddress)
{
    return (HWREG8(baseAddress + OFS_TDxCTL0) & TDIFG);
}

void Timer_D_enableCaptureCompareInterrupt (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    HWREG8(baseAddress + captureCompareRegister) &= ~CCIFG;
    HWREG16(baseAddress + captureCompareRegister) |= CCIE;
}

void Timer_D_disableCaptureCompareInterrupt (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    HWREG16(baseAddress + captureCompareRegister) &= ~CCIE;
}

uint32_t Timer_D_getCaptureCompareInterruptStatus (uint16_t baseAddress,
        uint16_t captureCompareRegister,
        uint16_t mask
        )
{
    return ( HWREG16(baseAddress + captureCompareRegister) & mask );
}

uint16_t Timer_D_getHighResInterruptStatus (uint16_t baseAddress,
    uint16_t mask)
{
    mask = (mask >> 8);
    return ( (HWREG16(baseAddress + OFS_TDxHINT) & mask) << 8 );
}

void Timer_D_clear (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxCTL0) |= TDCLR;
}

void Timer_D_clearHighResInterrupt (uint16_t baseAddress,
    uint16_t mask)
{
    mask = (mask >> 8);
    HWREG16(baseAddress + OFS_TDxHINT) &= ~mask;
}

uint8_t Timer_D_getSynchronizedCaptureCompareInput
    (uint16_t baseAddress,
    uint16_t captureCompareRegister,
    uint16_t synchronized
    )
{
    if (HWREG16(baseAddress + captureCompareRegister) & synchronized){
        return ( TIMER_D_CAPTURECOMPARE_INPUT_HIGH) ;
    } else   {
        return ( TIMER_D_CAPTURECOMPARE_INPUT_LOW) ;
    }
}

uint8_t Timer_D_getOutputForOutputModeOutBitValue
    (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    if (HWREG16(baseAddress + captureCompareRegister) & OUT){
        return ( TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH) ;
    } else   {
        return ( TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW) ;
    }
}

uint16_t Timer_D_getCaptureCompareCount
    (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    return  (HWREG16(baseAddress + captureCompareRegister + 2));
}

uint16_t Timer_D_getCaptureCompareLatchCount
    (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    return  (HWREG16(baseAddress + captureCompareRegister + 4));
}

uint8_t Timer_D_getCaptureCompareInputSignal
    (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    return  ((HWREG8(baseAddress + captureCompareRegister) & CCI));
}

void Timer_D_setOutputForOutputModeOutBitValue
    (uint16_t baseAddress,
    uint16_t captureCompareRegister,
    uint8_t outputModeOutBitValue
    )
{
    HWREG16(baseAddress + captureCompareRegister) &= ~OUT;
    HWREG16(baseAddress + captureCompareRegister) |= outputModeOutBitValue;
}

void Timer_D_outputPWM(uint16_t baseAddress, Timer_D_outputPWMParam *param)
{
    HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

    HWREG16(baseAddress + OFS_TDxCTL0)  &=
            ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
               TIMER_D_UPDOWN_MODE + TIMER_D_DO_CLEAR +
               TIMER_D_TDIE_INTERRUPT_ENABLE +
               ID__8
               );

    HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
    HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                           ((param->clockSourceDivider&0x7)<<8));

    HWREG16(baseAddress + OFS_TDxCTL0) |= (TIMER_D_UP_MODE +
                                           TIMER_D_DO_CLEAR +
                                           ((param->clockSourceDivider>>3)<<6));

    HWREG16(baseAddress + OFS_TDxCCR0)  = param->timerPeriod;

    HWREG16(baseAddress + OFS_TDxCCTL0)  &=
        ~(TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE +
          TIMER_D_OUTPUTMODE_RESET_SET
          );
    HWREG16(baseAddress + param->compareRegister) |= param->compareOutputMode;

    HWREG16(baseAddress + param->compareRegister + 2) = param->dutyCycle;
}

void Timer_D_stop ( uint16_t baseAddress )
{
    HWREG16(baseAddress + OFS_TDxCTL0)  &= ~MC_3;
}

void Timer_D_setCompareValue (  uint16_t baseAddress,
    uint16_t compareRegister,
    uint16_t compareValue
    )
{
    HWREG16(baseAddress + compareRegister + 0x02) = compareValue;
}

void Timer_D_clearTimerInterrupt (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxCTL0) &= ~TDIFG;
}

void Timer_D_clearCaptureCompareInterrupt (uint16_t baseAddress,
    uint16_t captureCompareRegister
    )
{
    HWREG16(baseAddress + captureCompareRegister)  &= ~CCIFG;
}

uint8_t Timer_D_initHighResGeneratorInFreeRunningMode
    (uint16_t baseAddress,
    uint8_t desiredHighResFrequency
    )
{
    struct s_TLV_Timer_D_Cal_Data * pTD0CAL;
    uint8_t TD0CAL_bytes;

    // Read the TimerD TLV Data
    TLV_getInfo(TLV_TAG_TIMER_D_CAL,
              0,
              &TD0CAL_bytes,
              (uint16_t **)&pTD0CAL
              );

    if(0x00 == TD0CAL_bytes)
    {
        // No TimerD free running cal data found
        return STATUS_FAIL;
    }

    HWREG16(baseAddress + OFS_TDxHCTL1) = TDHCLKTRIM6;
    HWREG16(baseAddress + OFS_TDxCTL1) = 0x00;
    HWREG16(baseAddress + OFS_TDxHCTL0) = 0x00;

    switch( desiredHighResFrequency )
    {
        case TIMER_D_HIGHRES_64MHZ:
            HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_64;
            break;

        case TIMER_D_HIGHRES_128MHZ:
            HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_128;
            break;

        case TIMER_D_HIGHRES_200MHZ:
            HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_200;
            break;

        case TIMER_D_HIGHRES_256MHZ:
            HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_256;
            break;
    }

    // Select Hi-res local clock
    HWREG16(baseAddress + OFS_TDxCTL1) |= TDCLKM_1;

    // CALEN=0 => free running mode; enable Hi-res mode
    if(TIMER_D_HIGHRES_256MHZ == desiredHighResFrequency)
        HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHM_1;

    HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHEN;

    return STATUS_SUCCESS;

}

void Timer_D_initHighResGeneratorInRegulatedMode(uint16_t baseAddress,
    Timer_D_initHighResGeneratorInRegulatedModeParam *param)
{
    HWREG16(baseAddress + OFS_TDxCTL0) &= ~(TDSSEL_3 + TDHD_3 + TDCLR + ID__8);
    HWREG16(baseAddress + OFS_TDxCTL1) &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

    HWREG16(baseAddress + OFS_TDxCTL0) |= (param->clockSource +
                                           ((param->clockSourceDivider>>3)<<6));
    HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                           ((param->clockSourceDivider&0x7)<<8));

    // Select Hi-res local clock
    // Calibration and Hi-res mode enable
    HWREG16(baseAddress + OFS_TDxCTL1) |= TDCLKM_1;
    // Select Hi-res local clock
    HWREG16(baseAddress + OFS_TDxHCTL0) =  TDHREGEN + TDHEN ;
    HWREG16(baseAddress + OFS_TDxHCTL0) |= param->highResClockMultiplyFactor +
                    param->highResClockDivider;
}

void Timer_D_combineTDCCRToOutputPWM(uint16_t baseAddress,
    Timer_D_combineTDCCRToOutputPWMParam *param)
{
    HWREG16(baseAddress + OFS_TDxCCTL2) &= ~OUTMOD_7;
    HWREG16(baseAddress + OFS_TDxCCTL2)  |= param->compareOutputMode;

    HWREG16(baseAddress + OFS_TDxCCR0)  = param->timerPeriod;

    HWREG16(baseAddress + OFS_TDxCCR1 + (0x05 *
            (param->combineCCRRegistersCombination - TIMER_D_COMBINE_CCR1_CCR2))) = param->dutyCycle1;
    HWREG16(baseAddress + OFS_TDxCCR2 + (0x05 *
            (param->combineCCRRegistersCombination - TIMER_D_COMBINE_CCR1_CCR2))) = param->dutyCycle2;

    HWREG16(baseAddress + OFS_TDxCTL0)  &= ~ID__8;
    HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

    HWREG16(baseAddress + OFS_TDxCTL0) |= (param->clockSource +
                                           ((param->clockSourceDivider>>3)<<6));
    HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                           ((param->clockSourceDivider&0x7)<<8));
    HWREG16(baseAddress + OFS_TDxCTL1) |=
            (TD2CMB << (param->combineCCRRegistersCombination - TIMER_D_COMBINE_CCR1_CCR2));
}

void Timer_D_selectLatchingGroup(uint16_t  baseAddress,
        uint16_t groupLatch)
{

    HWREG16(baseAddress + OFS_TDxCTL0) &= ~TDCLGRP_3;
    HWREG16(baseAddress + OFS_TDxCTL0) |= groupLatch;
}

void Timer_D_selectCounterLength (uint16_t  baseAddress,
        uint16_t counterLength
        )
{

    HWREG16(baseAddress + OFS_TDxCTL0) &= ~CNTL_3;
    HWREG16(baseAddress + OFS_TDxCTL0) |= counterLength;
}

void Timer_D_initCompareLatchLoadEvent(uint16_t  baseAddress,
        uint16_t  compareRegister,
        uint16_t  compareLatchLoadEvent
        )
{

    HWREG16(baseAddress + compareRegister)  &= ~CLLD_3;
    HWREG16(baseAddress + compareRegister)  |= compareLatchLoadEvent;
}

void Timer_D_disableHighResFastWakeup (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxHCTL0) &= ~TDHFW;
}

void Timer_D_enableHighResFastWakeup (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHFW;
}

void Timer_D_disableHighResClockEnhancedAccuracy (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxHCTL0) &= ~TDHEAEN;
}

void Timer_D_enableHighResClockEnhancedAccuracy (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHEAEN;
}

void Timer_D_disableHighResGeneratorForceON (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxHCTL0) &= ~TDHRON;
}

void Timer_D_enableHighResGeneratorForceON (uint16_t baseAddress)
{
    HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHRON;
}

void Timer_D_selectHighResCoarseClockRange (uint16_t baseAddress,
        uint16_t highResCoarseClockRange
        )
{
    HWREG16(baseAddress + OFS_TDxHCTL1) &= ~TDHCLKCR;
    HWREG16(baseAddress + OFS_TDxHCTL1) |= highResCoarseClockRange;
}

void Timer_D_selectHighResClockRange (uint16_t baseAddress,
        uint16_t highResClockRange
        )
{
    HWREG16(baseAddress + OFS_TDxHCTL1) &= ~TDHCLKCR;
    HWREG16(baseAddress + OFS_TDxHCTL1) |= highResClockRange;
}

uint16_t Timer_D_getCounterValue (uint16_t baseAddress)
{
    uint16_t voteOne, voteTwo, res;

    voteTwo = HWREG16(baseAddress + OFS_TDxR);

    do
    {
        voteOne = voteTwo;
        voteTwo = HWREG16(baseAddress + OFS_TDxR);

        if(voteTwo > voteOne) {
            res = voteTwo - voteOne;
        } else if(voteOne > voteTwo) {
            res = voteOne - voteTwo;
        } else{
            res = 0;
        }

    } while ( res > TIMER_D_THRESHOLD);

    return voteTwo;
}

void Timer_D_setOutputMode(uint16_t baseAddress,
                             uint16_t compareRegister,
                             uint16_t compareOutputMode)
{
    uint16_t temp = HWREG16(baseAddress + compareRegister);
    HWREG16(baseAddress + compareRegister) = temp & ~(OUTMOD_7) | compareOutputMode;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for timer_d_api
//! @}
//
//*****************************************************************************
