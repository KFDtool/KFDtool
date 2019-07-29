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
// adc12_a.h - Driver for the ADC12_A Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_ADC12_A_H__
#define __MSP430WARE_ADC12_A_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_ADC12_PLUS__

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
//! \brief Used in the ADC12_A_configureMemory() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct ADC12_A_configureMemoryParam {
    //! Is the selected memory buffer to set the configuration for.
    //! \n Valid values are:
    //! - \b ADC12_A_MEMORY_0 [Default]
    //! - \b ADC12_A_MEMORY_1
    //! - \b ADC12_A_MEMORY_2
    //! - \b ADC12_A_MEMORY_3
    //! - \b ADC12_A_MEMORY_4
    //! - \b ADC12_A_MEMORY_5
    //! - \b ADC12_A_MEMORY_6
    //! - \b ADC12_A_MEMORY_7
    //! - \b ADC12_A_MEMORY_8
    //! - \b ADC12_A_MEMORY_9
    //! - \b ADC12_A_MEMORY_10
    //! - \b ADC12_A_MEMORY_11
    //! - \b ADC12_A_MEMORY_12
    //! - \b ADC12_A_MEMORY_13
    //! - \b ADC12_A_MEMORY_14
    //! - \b ADC12_A_MEMORY_15
    uint8_t memoryBufferControlIndex;
    //! Is the input that will store the converted data into the specified
    //! memory buffer.
    //! \n Valid values are:
    //! - \b ADC12_A_INPUT_A0 [Default]
    //! - \b ADC12_A_INPUT_A1
    //! - \b ADC12_A_INPUT_A2
    //! - \b ADC12_A_INPUT_A3
    //! - \b ADC12_A_INPUT_A4
    //! - \b ADC12_A_INPUT_A5
    //! - \b ADC12_A_INPUT_A6
    //! - \b ADC12_A_INPUT_A7
    //! - \b ADC12_A_INPUT_A8
    //! - \b ADC12_A_INPUT_A9
    //! - \b ADC12_A_INPUT_TEMPSENSOR
    //! - \b ADC12_A_INPUT_BATTERYMONITOR
    //! - \b ADC12_A_INPUT_A12
    //! - \b ADC12_A_INPUT_A13
    //! - \b ADC12_A_INPUT_A14
    //! - \b ADC12_A_INPUT_A15
    uint8_t inputSourceSelect;
    //! Is the reference voltage source to set as the upper limit for the
    //! conversion stored in the specified memory.
    //! \n Valid values are:
    //! - \b ADC12_A_VREFPOS_AVCC [Default]
    //! - \b ADC12_A_VREFPOS_EXT
    //! - \b ADC12_A_VREFPOS_INT
    uint8_t positiveRefVoltageSourceSelect;
    //! Is the reference voltage source to set as the lower limit for the
    //! conversion stored in the specified memory.
    //! \n Valid values are:
    //! - \b ADC12_A_VREFNEG_AVSS [Default]
    //! - \b ADC12_A_VREFNEG_EXT
    uint8_t negativeRefVoltageSourceSelect;
    //! Indicates that the specified memory buffer will be the end of the
    //! sequence if a sequenced conversion mode is selected
    //! \n Valid values are:
    //! - \b ADC12_A_NOTENDOFSEQUENCE [Default] - The specified memory buffer
    //!    will NOT be the end of the sequence OR a sequenced conversion mode
    //!    is not selected.
    //! - \b ADC12_A_ENDOFSEQUENCE - The specified memory buffer will be the
    //!    end of the sequence.
    uint8_t endOfSequence;
} ADC12_A_configureMemoryParam;


//*****************************************************************************
//
// The following are values that can be passed to the clockSourceSelect
// parameter for functions: ADC12_A_init().
//
//*****************************************************************************
#define ADC12_A_CLOCKSOURCE_ADC12OSC                              (ADC12SSEL_0)
#define ADC12_A_CLOCKSOURCE_ACLK                                  (ADC12SSEL_1)
#define ADC12_A_CLOCKSOURCE_MCLK                                  (ADC12SSEL_2)
#define ADC12_A_CLOCKSOURCE_SMCLK                                 (ADC12SSEL_3)

//*****************************************************************************
//
// The following are values that can be passed to the clockSourceDivider
// parameter for functions: ADC12_A_init().
//
//*****************************************************************************
#define ADC12_A_CLOCKDIVIDER_1                                     (ADC12DIV_0)
#define ADC12_A_CLOCKDIVIDER_2                                     (ADC12DIV_1)
#define ADC12_A_CLOCKDIVIDER_3                                     (ADC12DIV_2)
#define ADC12_A_CLOCKDIVIDER_4                                     (ADC12DIV_3)
#define ADC12_A_CLOCKDIVIDER_5                                     (ADC12DIV_4)
#define ADC12_A_CLOCKDIVIDER_6                                     (ADC12DIV_5)
#define ADC12_A_CLOCKDIVIDER_7                                     (ADC12DIV_6)
#define ADC12_A_CLOCKDIVIDER_8                                     (ADC12DIV_7)
#define ADC12_A_CLOCKDIVIDER_12                        (ADC12DIV_2 + ADC12PDIV)
#define ADC12_A_CLOCKDIVIDER_16                        (ADC12DIV_3 + ADC12PDIV)
#define ADC12_A_CLOCKDIVIDER_20                        (ADC12DIV_4 + ADC12PDIV)
#define ADC12_A_CLOCKDIVIDER_24                        (ADC12DIV_5 + ADC12PDIV)
#define ADC12_A_CLOCKDIVIDER_28                        (ADC12DIV_6 + ADC12PDIV)
#define ADC12_A_CLOCKDIVIDER_32                        (ADC12DIV_7 + ADC12PDIV)

//*****************************************************************************
//
// The following are values that can be passed to the
// sampleHoldSignalSourceSelect parameter for functions: ADC12_A_init().
//
//*****************************************************************************
#define ADC12_A_SAMPLEHOLDSOURCE_SC                                (ADC12SHS_0)
#define ADC12_A_SAMPLEHOLDSOURCE_1                                 (ADC12SHS_1)
#define ADC12_A_SAMPLEHOLDSOURCE_2                                 (ADC12SHS_2)
#define ADC12_A_SAMPLEHOLDSOURCE_3                                 (ADC12SHS_3)

//*****************************************************************************
//
// The following are values that can be passed to the clockCycleHoldCountLowMem
// parameter for functions: ADC12_A_setupSamplingTimer(); the
// clockCycleHoldCountHighMem parameter for functions:
// ADC12_A_setupSamplingTimer().
//
//*****************************************************************************
#define ADC12_A_CYCLEHOLD_4_CYCLES                                (ADC12SHT0_0)
#define ADC12_A_CYCLEHOLD_8_CYCLES                                (ADC12SHT0_1)
#define ADC12_A_CYCLEHOLD_16_CYCLES                               (ADC12SHT0_2)
#define ADC12_A_CYCLEHOLD_32_CYCLES                               (ADC12SHT0_3)
#define ADC12_A_CYCLEHOLD_64_CYCLES                               (ADC12SHT0_4)
#define ADC12_A_CYCLEHOLD_96_CYCLES                               (ADC12SHT0_5)
#define ADC12_A_CYCLEHOLD_128_CYCLES                              (ADC12SHT0_6)
#define ADC12_A_CYCLEHOLD_192_CYCLES                              (ADC12SHT0_7)
#define ADC12_A_CYCLEHOLD_256_CYCLES                              (ADC12SHT0_8)
#define ADC12_A_CYCLEHOLD_384_CYCLES                              (ADC12SHT0_9)
#define ADC12_A_CYCLEHOLD_512_CYCLES                             (ADC12SHT0_10)
#define ADC12_A_CYCLEHOLD_768_CYCLES                             (ADC12SHT0_11)
#define ADC12_A_CYCLEHOLD_1024_CYCLES                            (ADC12SHT0_12)

//*****************************************************************************
//
// The following are values that can be passed to the multipleSamplesEnabled
// parameter for functions: ADC12_A_setupSamplingTimer().
//
//*****************************************************************************
#define ADC12_A_MULTIPLESAMPLESDISABLE                            (!(ADC12MSC))
#define ADC12_A_MULTIPLESAMPLESENABLE                                (ADC12MSC)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: ADC12_A_configureMemory().
//
//*****************************************************************************
#define ADC12_A_VREFPOS_AVCC                       (!(ADC12SREF0 + ADC12SREF1))
#define ADC12_A_VREFPOS_EXT                                        (ADC12SREF1)
#define ADC12_A_VREFPOS_INT                                        (ADC12SREF0)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: ADC12_A_configureMemory().
//
//*****************************************************************************
#define ADC12_A_VREFNEG_AVSS                                    (!(ADC12SREF2))
#define ADC12_A_VREFNEG_EXT                                        (ADC12SREF2)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: ADC12_A_configureMemory().
//
//*****************************************************************************
#define ADC12_A_NOTENDOFSEQUENCE                                  (!(ADC12EOS))
#define ADC12_A_ENDOFSEQUENCE                                        (ADC12EOS)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: ADC12_A_configureMemory().
//
//*****************************************************************************
#define ADC12_A_INPUT_A0                                          (ADC12INCH_0)
#define ADC12_A_INPUT_A1                                          (ADC12INCH_1)
#define ADC12_A_INPUT_A2                                          (ADC12INCH_2)
#define ADC12_A_INPUT_A3                                          (ADC12INCH_3)
#define ADC12_A_INPUT_A4                                          (ADC12INCH_4)
#define ADC12_A_INPUT_A5                                          (ADC12INCH_5)
#define ADC12_A_INPUT_A6                                          (ADC12INCH_6)
#define ADC12_A_INPUT_A7                                          (ADC12INCH_7)
#define ADC12_A_INPUT_A8                                          (ADC12INCH_8)
#define ADC12_A_INPUT_A9                                          (ADC12INCH_9)
#define ADC12_A_INPUT_TEMPSENSOR                                 (ADC12INCH_10)
#define ADC12_A_INPUT_BATTERYMONITOR                             (ADC12INCH_11)
#define ADC12_A_INPUT_A12                                        (ADC12INCH_12)
#define ADC12_A_INPUT_A13                                        (ADC12INCH_13)
#define ADC12_A_INPUT_A14                                        (ADC12INCH_14)
#define ADC12_A_INPUT_A15                                        (ADC12INCH_15)

//*****************************************************************************
//
// The following are values that can be passed to the startingMemoryBufferIndex
// parameter for functions: ADC12_A_startConversion(); the memoryIndex
// parameter for functions: ADC12_A_getMemoryAddressForDMA(); the
// memoryBufferIndex parameter for functions: ADC12_A_getResults(); the param
// parameter for functions: ADC12_A_configureMemory().
//
//*****************************************************************************
#define ADC12_A_MEMORY_0                                                  (0x0)
#define ADC12_A_MEMORY_1                                                  (0x1)
#define ADC12_A_MEMORY_2                                                  (0x2)
#define ADC12_A_MEMORY_3                                                  (0x3)
#define ADC12_A_MEMORY_4                                                  (0x4)
#define ADC12_A_MEMORY_5                                                  (0x5)
#define ADC12_A_MEMORY_6                                                  (0x6)
#define ADC12_A_MEMORY_7                                                  (0x7)
#define ADC12_A_MEMORY_8                                                  (0x8)
#define ADC12_A_MEMORY_9                                                  (0x9)
#define ADC12_A_MEMORY_10                                                 (0xA)
#define ADC12_A_MEMORY_11                                                 (0xB)
#define ADC12_A_MEMORY_12                                                 (0xC)
#define ADC12_A_MEMORY_13                                                 (0xD)
#define ADC12_A_MEMORY_14                                                 (0xE)
#define ADC12_A_MEMORY_15                                                 (0xF)

//*****************************************************************************
//
// The following are values that can be passed to the memoryInterruptFlagMask
// parameter for functions: ADC12_A_clearInterrupt(), and
// ADC12_A_getInterruptStatus().
//
//*****************************************************************************
#define ADC12_A_IFG0                                                (ADC12IFG0)
#define ADC12_A_IFG1                                                (ADC12IFG1)
#define ADC12_A_IFG2                                                (ADC12IFG2)
#define ADC12_A_IFG3                                                (ADC12IFG3)
#define ADC12_A_IFG4                                                (ADC12IFG4)
#define ADC12_A_IFG5                                                (ADC12IFG5)
#define ADC12_A_IFG6                                                (ADC12IFG6)
#define ADC12_A_IFG7                                                (ADC12IFG7)
#define ADC12_A_IFG8                                                (ADC12IFG8)
#define ADC12_A_IFG9                                                (ADC12IFG9)
#define ADC12_A_IFG10                                              (ADC12IFG10)
#define ADC12_A_IFG11                                              (ADC12IFG11)
#define ADC12_A_IFG12                                              (ADC12IFG12)
#define ADC12_A_IFG13                                              (ADC12IFG13)
#define ADC12_A_IFG14                                              (ADC12IFG14)
#define ADC12_A_IFG15                                              (ADC12IFG15)

//*****************************************************************************
//
// The following are values that can be passed to the
// conversionSequenceModeSelect parameter for functions:
// ADC12_A_startConversion().
//
//*****************************************************************************
#define ADC12_A_SINGLECHANNEL                                   (ADC12CONSEQ_0)
#define ADC12_A_SEQOFCHANNELS                                   (ADC12CONSEQ_1)
#define ADC12_A_REPEATED_SINGLECHANNEL                          (ADC12CONSEQ_2)
#define ADC12_A_REPEATED_SEQOFCHANNELS                          (ADC12CONSEQ_3)

//*****************************************************************************
//
// The following are values that can be passed to the preempt parameter for
// functions: ADC12_A_disableConversions().
//
//*****************************************************************************
#define ADC12_A_COMPLETECONVERSION                                        false
#define ADC12_A_PREEMPTCONVERSION                                          true

//*****************************************************************************
//
// The following are values that can be passed to the resolutionSelect
// parameter for functions: ADC12_A_setResolution().
//
//*****************************************************************************
#define ADC12_A_RESOLUTION_8BIT                                    (ADC12RES_0)
#define ADC12_A_RESOLUTION_10BIT                                   (ADC12RES_1)
#define ADC12_A_RESOLUTION_12BIT                                   (ADC12RES_2)

//*****************************************************************************
//
// The following are values that can be passed to the invertedSignal parameter
// for functions: ADC12_A_setSampleHoldSignalInversion().
//
//*****************************************************************************
#define ADC12_A_NONINVERTEDSIGNAL                                (!(ADC12ISSH))
#define ADC12_A_INVERTEDSIGNAL                                      (ADC12ISSH)

//*****************************************************************************
//
// The following are values that can be passed to the readBackFormat parameter
// for functions: ADC12_A_setDataReadBackFormat().
//
//*****************************************************************************
#define ADC12_A_UNSIGNED_BINARY                                    (!(ADC12DF))
#define ADC12_A_SIGNED_2SCOMPLEMENT                                   (ADC12DF)

//*****************************************************************************
//
// The following are values that can be passed to the samplingRateSelect
// parameter for functions: ADC12_A_setReferenceBufferSamplingRate().
//
//*****************************************************************************
#define ADC12_A_MAXSAMPLINGRATE_200KSPS                            (!(ADC12SR))
#define ADC12_A_MAXSAMPLINGRATE_50KSPS                                (ADC12SR)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the ADC12_A_isBusy() function.
//
//*****************************************************************************
#define ADC12_A_NOTBUSY                                                    0x00
#define ADC12_A_BUSY                                                  ADC12BUSY

//*****************************************************************************
//
// The following are values that can be passed to the interruptMask parameter
// for functions: ADC12_A_enableInterrupt(), and ADC12_A_disableInterrupt().
//
//*****************************************************************************
#define ADC12_A_IE0                                                  (ADC12IE0)
#define ADC12_A_IE1                                                  (ADC12IE1)
#define ADC12_A_IE2                                                  (ADC12IE2)
#define ADC12_A_IE3                                                  (ADC12IE3)
#define ADC12_A_IE4                                                  (ADC12IE4)
#define ADC12_A_IE5                                                  (ADC12IE5)
#define ADC12_A_IE6                                                  (ADC12IE6)
#define ADC12_A_IE7                                                  (ADC12IE7)
#define ADC12_A_IE8                                                  (ADC12IE8)
#define ADC12_A_IE9                                                  (ADC12IE9)
#define ADC12_A_IE10                                                (ADC12IE10)
#define ADC12_A_IE11                                                (ADC12IE11)
#define ADC12_A_IE12                                                (ADC12IE12)
#define ADC12_A_IE13                                                (ADC12IE13)
#define ADC12_A_IE14                                                (ADC12IE14)
#define ADC12_A_IE15                                                (ADC12IE15)
#define ADC12_A_OVERFLOW_IE                         ((uint32_t)ADC12OVIE << 16)
#define ADC12_A_CONVERSION_TIME_OVERFLOW_IE        ((uint32_t)ADC12TOVIE << 16)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the ADC12_A Module.
//!
//! This function initializes the ADC module to allow for analog-to-digital
//! conversions. Specifically this function sets up the sample-and-hold signal
//! and clock sources for the ADC core to use for conversions. Upon successful
//! completion of the initialization all of the ADC control registers will be
//! reset, excluding the memory controls and reference module bits, the given
//! parameters will be set, and the ADC core will be turned on (Note, that the
//! ADC core only draws power during conversions and remains off when not
//! converting).Note that sample/hold signal sources are device dependent. Note
//! that if re-initializing the ADC after starting a conversion with the
//! startConversion() function, the disableConversion() must be called BEFORE
//! this function can be called.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param sampleHoldSignalSourceSelect is the signal that will trigger a
//!        sample-and-hold for an input signal to be converted. This parameter
//!        is device specific and sources should be found in the device's
//!        datasheet.
//!        Valid values are:
//!        - \b ADC12_A_SAMPLEHOLDSOURCE_SC [Default]
//!        - \b ADC12_A_SAMPLEHOLDSOURCE_1
//!        - \b ADC12_A_SAMPLEHOLDSOURCE_2
//!        - \b ADC12_A_SAMPLEHOLDSOURCE_3 - This parameter is device specific
//!           and sources should be found in the device's datasheet.
//!        \n Modified bits are \b ADC12SHSx of \b ADC12CTL1 register.
//! \param clockSourceSelect selects the clock that will be used by the ADC12_A
//!        core, and the sampling timer if a sampling pulse mode is enabled.
//!        Valid values are:
//!        - \b ADC12_A_CLOCKSOURCE_ADC12OSC [Default] - MODOSC 5 MHz
//!           oscillator from the UCS
//!        - \b ADC12_A_CLOCKSOURCE_ACLK - The Auxiliary Clock
//!        - \b ADC12_A_CLOCKSOURCE_MCLK - The Master Clock
//!        - \b ADC12_A_CLOCKSOURCE_SMCLK - The Sub-Master Clock
//!        \n Modified bits are \b ADC12SSELx of \b ADC12CTL1 register.
//! \param clockSourceDivider selects the amount that the clock will be
//!        divided.
//!        Valid values are:
//!        - \b ADC12_A_CLOCKDIVIDER_1 [Default]
//!        - \b ADC12_A_CLOCKDIVIDER_2
//!        - \b ADC12_A_CLOCKDIVIDER_3
//!        - \b ADC12_A_CLOCKDIVIDER_4
//!        - \b ADC12_A_CLOCKDIVIDER_5
//!        - \b ADC12_A_CLOCKDIVIDER_6
//!        - \b ADC12_A_CLOCKDIVIDER_7
//!        - \b ADC12_A_CLOCKDIVIDER_8
//!        - \b ADC12_A_CLOCKDIVIDER_12
//!        - \b ADC12_A_CLOCKDIVIDER_16
//!        - \b ADC12_A_CLOCKDIVIDER_20
//!        - \b ADC12_A_CLOCKDIVIDER_24
//!        - \b ADC12_A_CLOCKDIVIDER_28
//!        - \b ADC12_A_CLOCKDIVIDER_32
//!        \n Modified bits are \b ADC12PDIV of \b ADC12CTL2 register; bits \b
//!        ADC12DIVx of \b ADC12CTL1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process.
//
//*****************************************************************************
extern bool ADC12_A_init(uint16_t baseAddress,
                         uint16_t sampleHoldSignalSourceSelect,
                         uint8_t clockSourceSelect,
                         uint16_t clockSourceDivider);

//*****************************************************************************
//
//! \brief Enables the ADC12_A block.
//!
//! This will enable operation of the ADC12_A block.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//!
//! Modified bits are \b ADC12ON of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_enable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the ADC12_A block.
//!
//! This will disable operation of the ADC12_A block.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//!
//! Modified bits are \b ADC12ON of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_disable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets up and enables the Sampling Timer Pulse Mode.
//!
//! This function sets up the sampling timer pulse mode which allows the
//! sample/hold signal to trigger a sampling timer to sample-and-hold an input
//! signal for a specified number of clock cycles without having to hold the
//! sample/hold signal for the entire period of sampling. Note that if a
//! conversion has been started with the startConversion() function, then a
//! call to disableConversions() is required before this function may be
//! called.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param clockCycleHoldCountLowMem sets the amount of clock cycles to sample-
//!        and-hold for the higher memory buffers 0-7.
//!        Valid values are:
//!        - \b ADC12_A_CYCLEHOLD_4_CYCLES [Default]
//!        - \b ADC12_A_CYCLEHOLD_8_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_16_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_32_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_64_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_96_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_128_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_192_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_256_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_384_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_512_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_768_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_1024_CYCLES
//!        \n Modified bits are \b ADC12SHT0x of \b ADC12CTL0 register.
//! \param clockCycleHoldCountHighMem sets the amount of clock cycles to
//!        sample-and-hold for the higher memory buffers 8-15.
//!        Valid values are:
//!        - \b ADC12_A_CYCLEHOLD_4_CYCLES [Default]
//!        - \b ADC12_A_CYCLEHOLD_8_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_16_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_32_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_64_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_96_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_128_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_192_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_256_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_384_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_512_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_768_CYCLES
//!        - \b ADC12_A_CYCLEHOLD_1024_CYCLES
//!        \n Modified bits are \b ADC12SHT1x of \b ADC12CTL0 register.
//! \param multipleSamplesEnabled allows multiple conversions to start without
//!        a trigger signal from the sample/hold signal
//!        Valid values are:
//!        - \b ADC12_A_MULTIPLESAMPLESDISABLE [Default] - a timer trigger will
//!           be needed to start every ADC conversion.
//!        - \b ADC12_A_MULTIPLESAMPLESENABLE - during a sequenced and/or
//!           repeated conversion mode, after the first conversion, no
//!           sample/hold signal is necessary to start subsequent sample/hold
//!           and convert processes.
//!        \n Modified bits are \b ADC12MSC of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_setupSamplingTimer(uint16_t baseAddress,
                                       uint16_t clockCycleHoldCountLowMem,
                                       uint16_t clockCycleHoldCountHighMem,
                                       uint16_t multipleSamplesEnabled);

//*****************************************************************************
//
//! \brief Disables Sampling Timer Pulse Mode.
//!
//! Disables the Sampling Timer Pulse Mode. Note that if a conversion has been
//! started with the startConversion() function, then a call to
//! disableConversions() is required before this function may be called.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//!
//! Modified bits are \b ADC12SHP of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_disableSamplingTimer(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Configures the controls of the selected memory buffer.
//!
//! Maps an input signal conversion into the selected memory buffer, as well as
//! the positive and negative reference voltages for each conversion being
//! stored into this memory buffer. If the internal reference is used for the
//! positive reference voltage, the internal REF module must be used to control
//! the voltage level. Note that if a conversion has been started with the
//! startConversion() function, then a call to disableConversions() is required
//! before this function may be called. If conversion is not disabled, this
//! function does nothing.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param param is the pointer to struct for memory configuration.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_configureMemory(uint16_t baseAddress,
                                    ADC12_A_configureMemoryParam *param);

//*****************************************************************************
//
//! \brief Enables selected ADC12_A interrupt sources.
//!
//! Enables the indicated ADC12_A interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt, disabled sources have
//! no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param interruptMask
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC12_A_IE0
//!        - \b ADC12_A_IE1
//!        - \b ADC12_A_IE2
//!        - \b ADC12_A_IE3
//!        - \b ADC12_A_IE4
//!        - \b ADC12_A_IE5
//!        - \b ADC12_A_IE6
//!        - \b ADC12_A_IE7
//!        - \b ADC12_A_IE8
//!        - \b ADC12_A_IE9
//!        - \b ADC12_A_IE10
//!        - \b ADC12_A_IE11
//!        - \b ADC12_A_IE12
//!        - \b ADC12_A_IE13
//!        - \b ADC12_A_IE14
//!        - \b ADC12_A_IE15
//!        - \b ADC12_A_OVERFLOW_IE
//!        - \b ADC12_A_CONVERSION_TIME_OVERFLOW_IE
//!
//! Modified bits of \b ADC12IE register and bits of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_enableInterrupt(uint16_t baseAddress,
                                    uint32_t interruptMask);

//*****************************************************************************
//
//! \brief Disables selected ADC12_A interrupt sources.
//!
//! Disables the indicated ADC12_A interrupt sources.  Only the sources that
//! are enabled can be reflected to the processor interrupt, disabled sources
//! have no effect on the processor.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param interruptMask
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC12_A_IE0
//!        - \b ADC12_A_IE1
//!        - \b ADC12_A_IE2
//!        - \b ADC12_A_IE3
//!        - \b ADC12_A_IE4
//!        - \b ADC12_A_IE5
//!        - \b ADC12_A_IE6
//!        - \b ADC12_A_IE7
//!        - \b ADC12_A_IE8
//!        - \b ADC12_A_IE9
//!        - \b ADC12_A_IE10
//!        - \b ADC12_A_IE11
//!        - \b ADC12_A_IE12
//!        - \b ADC12_A_IE13
//!        - \b ADC12_A_IE14
//!        - \b ADC12_A_IE15
//!        - \b ADC12_A_OVERFLOW_IE
//!        - \b ADC12_A_CONVERSION_TIME_OVERFLOW_IE
//!
//! Modified bits of \b ADC12IE register and bits of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_disableInterrupt(uint16_t baseAddress,
                                     uint32_t interruptMask);

//*****************************************************************************
//
//! \brief Clears ADC12_A selected interrupt flags.
//!
//! The selected ADC12_A interrupt flags are cleared, so that it no longer
//! asserts. The memory buffer interrupt flags are only cleared when the memory
//! buffer is accessed. Note that the overflow interrupts do not have an
//! interrupt flag to clear; they must be accessed directly from the interrupt
//! vector.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param memoryInterruptFlagMask is a bit mask of the interrupt flags to be
//!        cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC12_A_IFG0
//!        - \b ADC12_A_IFG1
//!        - \b ADC12_A_IFG2
//!        - \b ADC12_A_IFG3
//!        - \b ADC12_A_IFG4
//!        - \b ADC12_A_IFG5
//!        - \b ADC12_A_IFG6
//!        - \b ADC12_A_IFG7
//!        - \b ADC12_A_IFG8
//!        - \b ADC12_A_IFG9
//!        - \b ADC12_A_IFG10
//!        - \b ADC12_A_IFG11
//!        - \b ADC12_A_IFG12
//!        - \b ADC12_A_IFG13
//!        - \b ADC12_A_IFG14
//!        - \b ADC12_A_IFG15
//!
//! Modified bits of \b ADC12IFG register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_clearInterrupt(uint16_t baseAddress,
                                   uint16_t memoryInterruptFlagMask);

//*****************************************************************************
//
//! \brief Returns the status of the selected memory interrupt flags.
//!
//! Returns the status of the selected memory interrupt flags. Note that the
//! overflow interrupts do not have an interrupt flag to clear; they must be
//! accessed directly from the interrupt vector.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param memoryInterruptFlagMask is a bit mask of the interrupt flags status
//!        to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC12_A_IFG0
//!        - \b ADC12_A_IFG1
//!        - \b ADC12_A_IFG2
//!        - \b ADC12_A_IFG3
//!        - \b ADC12_A_IFG4
//!        - \b ADC12_A_IFG5
//!        - \b ADC12_A_IFG6
//!        - \b ADC12_A_IFG7
//!        - \b ADC12_A_IFG8
//!        - \b ADC12_A_IFG9
//!        - \b ADC12_A_IFG10
//!        - \b ADC12_A_IFG11
//!        - \b ADC12_A_IFG12
//!        - \b ADC12_A_IFG13
//!        - \b ADC12_A_IFG14
//!        - \b ADC12_A_IFG15
//!
//! \return The current interrupt flag status for the corresponding mask.
//
//*****************************************************************************
extern uint16_t ADC12_A_getInterruptStatus(uint16_t baseAddress,
                                           uint16_t memoryInterruptFlagMask);

//*****************************************************************************
//
//! \brief Enables/Starts an Analog-to-Digital Conversion.
//!
//! This function enables/starts the conversion process of the ADC. If the
//! sample/hold signal source chosen during initialization was ADC12OSC, then
//! the conversion is started immediately, otherwise the chosen sample/hold
//! signal source starts the conversion by a rising edge of the signal. Keep in
//! mind when selecting conversion modes, that for sequenced and/or repeated
//! modes, to keep the sample/hold-and-convert process continuing without a
//! trigger from the sample/hold signal source, the multiple samples must be
//! enabled using the ADC12_A_setupSamplingTimer() function. Note that after
//! this function is called, the ADC12_A_disableConversions() has to be called
//! to re-initialize the ADC, reconfigure a memory buffer control,
//! enable/disable the sampling timer, or to change the internal reference
//! voltage.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param startingMemoryBufferIndex is the memory buffer that will hold the
//!        first or only conversion.
//!        Valid values are:
//!        - \b ADC12_A_MEMORY_0 [Default]
//!        - \b ADC12_A_MEMORY_1
//!        - \b ADC12_A_MEMORY_2
//!        - \b ADC12_A_MEMORY_3
//!        - \b ADC12_A_MEMORY_4
//!        - \b ADC12_A_MEMORY_5
//!        - \b ADC12_A_MEMORY_6
//!        - \b ADC12_A_MEMORY_7
//!        - \b ADC12_A_MEMORY_8
//!        - \b ADC12_A_MEMORY_9
//!        - \b ADC12_A_MEMORY_10
//!        - \b ADC12_A_MEMORY_11
//!        - \b ADC12_A_MEMORY_12
//!        - \b ADC12_A_MEMORY_13
//!        - \b ADC12_A_MEMORY_14
//!        - \b ADC12_A_MEMORY_15
//!        \n Modified bits are \b ADC12STARTADDx of \b ADC12CTL1 register.
//! \param conversionSequenceModeSelect determines the ADC operating mode.
//!        Valid values are:
//!        - \b ADC12_A_SINGLECHANNEL [Default] - one-time conversion of a
//!           single channel into a single memory buffer.
//!        - \b ADC12_A_SEQOFCHANNELS - one time conversion of multiple
//!           channels into the specified starting memory buffer and each
//!           subsequent memory buffer up until the conversion is stored in a
//!           memory buffer dedicated as the end-of-sequence by the memory's
//!           control register.
//!        - \b ADC12_A_REPEATED_SINGLECHANNEL - repeated conversions of one
//!           channel into a single memory buffer.
//!        - \b ADC12_A_REPEATED_SEQOFCHANNELS - repeated conversions of
//!           multiple channels into the specified starting memory buffer and
//!           each subsequent memory buffer up until the conversion is stored
//!           in a memory buffer dedicated as the end-of-sequence by the
//!           memory's control register.
//!        \n Modified bits are \b ADC12CONSEQx of \b ADC12CTL1 register.
//!
//! Modified bits of \b ADC12CTL1 register and bits of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_startConversion(uint16_t baseAddress,
                                    uint16_t startingMemoryBufferIndex,
                                    uint8_t conversionSequenceModeSelect);

//*****************************************************************************
//
//! \brief Disables the ADC from converting any more signals.
//!
//! Disables the ADC from converting any more signals. If there is a conversion
//! in progress, this function can stop it immediately if the preempt parameter
//! is set as TRUE, by changing the conversion mode to single-channel, single-
//! conversion and disabling conversions. If the conversion mode is set as
//! single-channel, single-conversion and this function is called without
//! preemption, then the ADC core conversion status is polled until the
//! conversion is complete before disabling conversions to prevent
//! unpredictable data. If the ADC12_A_startConversion() has been called, then
//! this function has to be called to re-initialize the ADC, reconfigure a
//! memory buffer control, enable/disable the sampling pulse mode, or change
//! the internal reference voltage.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param preempt specifies if the current conversion should be pre-empted
//!        before the end of the conversion.
//!        Valid values are:
//!        - \b ADC12_A_COMPLETECONVERSION - Allows the ADC12_A to end the
//!           current conversion before disabling conversions.
//!        - \b ADC12_A_PREEMPTCONVERSION - Stops the ADC12_A immediately, with
//!           unpredictable results of the current conversion.
//!
//! Modified bits of \b ADC12CTL1 register and bits of \b ADC12CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_disableConversions(uint16_t baseAddress,
                                       bool preempt);

//*****************************************************************************
//
//! \brief A Signed Integer of the contents of the specified memory buffer
//!
//! Returns the raw contents of the specified memory buffer. The format of the
//! content depends on the read-back format of the data: if the data is in
//! signed 2's complement format then the contents in the memory buffer will be
//! left-justified with the least-significant bits as 0's, whereas if the data
//! is in unsigned format then the contents in the memory buffer will be right-
//! justified with the most-significant bits as 0's.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param memoryBufferIndex is the specified Memory Buffer to read.
//!        Valid values are:
//!        - \b ADC12_A_MEMORY_0 [Default]
//!        - \b ADC12_A_MEMORY_1
//!        - \b ADC12_A_MEMORY_2
//!        - \b ADC12_A_MEMORY_3
//!        - \b ADC12_A_MEMORY_4
//!        - \b ADC12_A_MEMORY_5
//!        - \b ADC12_A_MEMORY_6
//!        - \b ADC12_A_MEMORY_7
//!        - \b ADC12_A_MEMORY_8
//!        - \b ADC12_A_MEMORY_9
//!        - \b ADC12_A_MEMORY_10
//!        - \b ADC12_A_MEMORY_11
//!        - \b ADC12_A_MEMORY_12
//!        - \b ADC12_A_MEMORY_13
//!        - \b ADC12_A_MEMORY_14
//!        - \b ADC12_A_MEMORY_15
//!
//! \return A signed integer of the contents of the specified memory buffer
//
//*****************************************************************************
extern uint16_t ADC12_A_getResults(uint16_t baseAddress,
                                   uint8_t memoryBufferIndex);

//*****************************************************************************
//
//! \brief Use to change the resolution of the converted data.
//!
//! This function can be used to change the resolution of the converted data
//! from the default of 12-bits.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param resolutionSelect determines the resolution of the converted data.
//!        Valid values are:
//!        - \b ADC12_A_RESOLUTION_8BIT
//!        - \b ADC12_A_RESOLUTION_10BIT
//!        - \b ADC12_A_RESOLUTION_12BIT [Default]
//!        \n Modified bits are \b ADC12RESx of \b ADC12CTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_setResolution(uint16_t baseAddress,
                                  uint8_t resolutionSelect);

//*****************************************************************************
//
//! \brief Use to invert or un-invert the sample/hold signal.
//!
//! This function can be used to invert or un-invert the sample/hold signal.
//! Note that if a conversion has been started with the startConversion()
//! function, then a call to disableConversions() is required before this
//! function may be called.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param invertedSignal set if the sample/hold signal should be inverted
//!        Valid values are:
//!        - \b ADC12_A_NONINVERTEDSIGNAL [Default] - a sample-and-hold of an
//!           input signal for conversion will be started on a rising edge of
//!           the sample/hold signal.
//!        - \b ADC12_A_INVERTEDSIGNAL - a sample-and-hold of an input signal
//!           for conversion will be started on a falling edge of the
//!           sample/hold signal.
//!        \n Modified bits are \b ADC12ISSH of \b ADC12CTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_setSampleHoldSignalInversion(uint16_t baseAddress,
                                                 uint16_t invertedSignal);

//*****************************************************************************
//
//! \brief Use to set the read-back format of the converted data.
//!
//! Sets the format of the converted data: how it will be stored into the
//! memory buffer, and how it should be read back. The format can be set as
//! right-justified (default), which indicates that the number will be
//! unsigned, or left-justified, which indicates that the number will be signed
//! in 2's complement format. This change affects all memory buffers for
//! subsequent conversions.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param readBackFormat is the specified format to store the conversions in
//!        the memory buffer.
//!        Valid values are:
//!        - \b ADC12_A_UNSIGNED_BINARY [Default]
//!        - \b ADC12_A_SIGNED_2SCOMPLEMENT
//!        \n Modified bits are \b ADC12DF of \b ADC12CTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_setDataReadBackFormat(uint16_t baseAddress,
                                          uint8_t readBackFormat);

//*****************************************************************************
//
//! \brief Enables the reference buffer's burst ability.
//!
//! Enables the reference buffer's burst ability, allowing the reference buffer
//! to turn off while the ADC is not converting, and automatically turning on
//! when the ADC needs the generated reference voltage for a conversion.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_enableReferenceBurst(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the reference buffer's burst ability.
//!
//! Disables the reference buffer's burst ability, forcing the reference buffer
//! to remain on continuously.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_disableReferenceBurst(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Use to set the reference buffer's sampling rate.
//!
//! Sets the reference buffer's sampling rate to the selected sampling rate.
//! The default sampling rate is maximum of 200-ksps, and can be reduced to a
//! maximum of 50-ksps to conserve power.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param samplingRateSelect is the specified maximum sampling rate.
//!        Valid values are:
//!        - \b ADC12_A_MAXSAMPLINGRATE_200KSPS [Default]
//!        - \b ADC12_A_MAXSAMPLINGRATE_50KSPS
//!        \n Modified bits are \b ADC12SR of \b ADC12CTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC12_A_setReferenceBufferSamplingRate(uint16_t baseAddress,
                                                   uint8_t samplingRateSelect);

//*****************************************************************************
//
//! \brief Returns the address of the specified memory buffer for the DMA
//! module.
//!
//! Returns the address of the specified memory buffer. This can be used in
//! conjunction with the DMA to store the converted data directly to memory.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//! \param memoryIndex is the memory buffer to return the address of.
//!        Valid values are:
//!        - \b ADC12_A_MEMORY_0 [Default]
//!        - \b ADC12_A_MEMORY_1
//!        - \b ADC12_A_MEMORY_2
//!        - \b ADC12_A_MEMORY_3
//!        - \b ADC12_A_MEMORY_4
//!        - \b ADC12_A_MEMORY_5
//!        - \b ADC12_A_MEMORY_6
//!        - \b ADC12_A_MEMORY_7
//!        - \b ADC12_A_MEMORY_8
//!        - \b ADC12_A_MEMORY_9
//!        - \b ADC12_A_MEMORY_10
//!        - \b ADC12_A_MEMORY_11
//!        - \b ADC12_A_MEMORY_12
//!        - \b ADC12_A_MEMORY_13
//!        - \b ADC12_A_MEMORY_14
//!        - \b ADC12_A_MEMORY_15
//!
//! \return address of the specified memory buffer
//
//*****************************************************************************
extern uint32_t ADC12_A_getMemoryAddressForDMA(uint16_t baseAddress,
                                               uint8_t memoryIndex);

//*****************************************************************************
//
//! \brief Returns the busy status of the ADC12_A core.
//!
//! Returns the status of the ADC core if there is a conversion currently
//! taking place.
//!
//! \param baseAddress is the base address of the ADC12_A module.
//!
//! \return One of the following:
//!         - \b ADC12_A_NOTBUSY
//!         - \b ADC12_A_BUSY
//!         \n indicating if a conversion is taking place
//
//*****************************************************************************
extern uint16_t ADC12_A_isBusy(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_ADC12_A_H__
