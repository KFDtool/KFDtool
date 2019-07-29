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
// adc10_a.h - Driver for the ADC10_A Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_ADC10_A_H__
#define __MSP430WARE_ADC10_A_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_ADC10_A__

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
// The following are values that can be passed to the clockSourceSelect
// parameter for functions: ADC10_A_init().
//
//*****************************************************************************
#define ADC10_A_CLOCKSOURCE_ADC10OSC                              (ADC10SSEL_0)
#define ADC10_A_CLOCKSOURCE_ACLK                                  (ADC10SSEL_1)
#define ADC10_A_CLOCKSOURCE_MCLK                                  (ADC10SSEL_2)
#define ADC10_A_CLOCKSOURCE_SMCLK                                 (ADC10SSEL_3)

//*****************************************************************************
//
// The following are values that can be passed to the clockSourceDivider
// parameter for functions: ADC10_A_init().
//
//*****************************************************************************
#define ADC10_A_CLOCKDIVIDER_1                       (ADC10DIV_0 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_2                       (ADC10DIV_1 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_3                       (ADC10DIV_2 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_4                       (ADC10DIV_3 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_5                       (ADC10DIV_4 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_6                       (ADC10DIV_5 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_7                       (ADC10DIV_6 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_8                       (ADC10DIV_7 + ADC10PDIV_0)
#define ADC10_A_CLOCKDIVIDER_12                      (ADC10DIV_2 + ADC10PDIV_1)
#define ADC10_A_CLOCKDIVIDER_16                      (ADC10DIV_3 + ADC10PDIV_1)
#define ADC10_A_CLOCKDIVIDER_20                      (ADC10DIV_4 + ADC10PDIV_1)
#define ADC10_A_CLOCKDIVIDER_24                      (ADC10DIV_5 + ADC10PDIV_1)
#define ADC10_A_CLOCKDIVIDER_28                      (ADC10DIV_6 + ADC10PDIV_1)
#define ADC10_A_CLOCKDIVIDER_32                      (ADC10DIV_7 + ADC10PDIV_1)
#define ADC10_A_CLOCKDIVIDER_64                      (ADC10DIV_0 + ADC10PDIV_2)
#define ADC10_A_CLOCKDIVIDER_128                     (ADC10DIV_1 + ADC10PDIV_2)
#define ADC10_A_CLOCKDIVIDER_192                     (ADC10DIV_2 + ADC10PDIV_2)
#define ADC10_A_CLOCKDIVIDER_256                     (ADC10DIV_3 + ADC10PDIV_2)
#define ADC10_A_CLOCKDIVIDER_320                     (ADC10DIV_4 + ADC10PDIV_2)
#define ADC10_A_CLOCKDIVIDER_384                     (ADC10DIV_5 + ADC10PDIV_2)
#define ADC10_A_CLOCKDIVIDER_448                     (ADC10DIV_6 + ADC10PDIV_2)
#define ADC10_A_CLOCKDIVIDER_512                     (ADC10DIV_7 + ADC10PDIV_2)

//*****************************************************************************
//
// The following are values that can be passed to the
// sampleHoldSignalSourceSelect parameter for functions: ADC10_A_init().
//
//*****************************************************************************
#define ADC10_A_SAMPLEHOLDSOURCE_SC                                (ADC10SHS_0)
#define ADC10_A_SAMPLEHOLDSOURCE_1                                 (ADC10SHS_1)
#define ADC10_A_SAMPLEHOLDSOURCE_2                                 (ADC10SHS_2)
#define ADC10_A_SAMPLEHOLDSOURCE_3                                 (ADC10SHS_3)

//*****************************************************************************
//
// The following are values that can be passed to the multipleSamplesEnabled
// parameter for functions: ADC10_A_setupSamplingTimer().
//
//*****************************************************************************
#define ADC10_A_MULTIPLESAMPLESDISABLE                            (!(ADC10MSC))
#define ADC10_A_MULTIPLESAMPLESENABLE                                (ADC10MSC)

//*****************************************************************************
//
// The following are values that can be passed to the clockCycleHoldCount
// parameter for functions: ADC10_A_setupSamplingTimer().
//
//*****************************************************************************
#define ADC10_A_CYCLEHOLD_4_CYCLES                                 (ADC10SHT_0)
#define ADC10_A_CYCLEHOLD_8_CYCLES                                 (ADC10SHT_1)
#define ADC10_A_CYCLEHOLD_16_CYCLES                                (ADC10SHT_2)
#define ADC10_A_CYCLEHOLD_32_CYCLES                                (ADC10SHT_3)
#define ADC10_A_CYCLEHOLD_64_CYCLES                                (ADC10SHT_4)
#define ADC10_A_CYCLEHOLD_96_CYCLES                                (ADC10SHT_5)
#define ADC10_A_CYCLEHOLD_128_CYCLES                               (ADC10SHT_6)
#define ADC10_A_CYCLEHOLD_192_CYCLES                               (ADC10SHT_7)
#define ADC10_A_CYCLEHOLD_256_CYCLES                               (ADC10SHT_8)
#define ADC10_A_CYCLEHOLD_384_CYCLES                               (ADC10SHT_9)
#define ADC10_A_CYCLEHOLD_512_CYCLES                              (ADC10SHT_10)
#define ADC10_A_CYCLEHOLD_768_CYCLES                              (ADC10SHT_11)
#define ADC10_A_CYCLEHOLD_1024_CYCLES                             (ADC10SHT_12)

//*****************************************************************************
//
// The following are values that can be passed to the
// positiveRefVoltageSourceSelect parameter for functions:
// ADC10_A_configureMemory().
//
//*****************************************************************************
#define ADC10_A_VREFPOS_AVCC                       (!(ADC10SREF0 + ADC10SREF1))
#define ADC10_A_VREFPOS_EXT                                        (ADC10SREF1)
#define ADC10_A_VREFPOS_INT                                        (ADC10SREF0)

//*****************************************************************************
//
// The following are values that can be passed to the inputSourceSelect
// parameter for functions: ADC10_A_configureMemory().
//
//*****************************************************************************
#define ADC10_A_INPUT_A0                                          (ADC10INCH_0)
#define ADC10_A_INPUT_A1                                          (ADC10INCH_1)
#define ADC10_A_INPUT_A2                                          (ADC10INCH_2)
#define ADC10_A_INPUT_A3                                          (ADC10INCH_3)
#define ADC10_A_INPUT_A4                                          (ADC10INCH_4)
#define ADC10_A_INPUT_A5                                          (ADC10INCH_5)
#define ADC10_A_INPUT_A6                                          (ADC10INCH_6)
#define ADC10_A_INPUT_A7                                          (ADC10INCH_7)
#define ADC10_A_INPUT_A8                                          (ADC10INCH_8)
#define ADC10_A_INPUT_A9                                          (ADC10INCH_9)
#define ADC10_A_INPUT_TEMPSENSOR                                 (ADC10INCH_10)
#define ADC10_A_INPUT_BATTERYMONITOR                             (ADC10INCH_11)
#define ADC10_A_INPUT_A12                                        (ADC10INCH_12)
#define ADC10_A_INPUT_A13                                        (ADC10INCH_13)
#define ADC10_A_INPUT_A14                                        (ADC10INCH_14)
#define ADC10_A_INPUT_A15                                        (ADC10INCH_15)

//*****************************************************************************
//
// The following are values that can be passed to the
// negativeRefVoltageSourceSelect parameter for functions:
// ADC10_A_configureMemory().
//
//*****************************************************************************
#define ADC10_A_VREFNEG_AVSS                                    (!(ADC10SREF2))
#define ADC10_A_VREFNEG_EXT                                        (ADC10SREF2)

//*****************************************************************************
//
// The following are values that can be passed to the interruptMask parameter
// for functions: ADC10_A_enableInterrupt(), and ADC10_A_disableInterrupt().
//
//*****************************************************************************
#define ADC10_A_TIMEOVERFLOW_INT                                   (ADC10TOVIE)
#define ADC10_A_OVERFLOW_INT                                        (ADC10OVIE)
#define ADC10_A_ABOVETHRESHOLD_INT                                  (ADC10HIIE)
#define ADC10_A_BELOWTHRESHOLD_INT                                  (ADC10LOIE)
#define ADC10_A_INSIDEWINDOW_INT                                    (ADC10INIE)
#define ADC10_A_COMPLETED_INT                                        (ADC10IE0)

//*****************************************************************************
//
// The following are values that can be passed to the interruptFlagMask
// parameter for functions: ADC10_A_clearInterrupt(), and
// ADC10_A_getInterruptStatus().
//
//*****************************************************************************
#define ADC10_A_TIMEOVERFLOW_INTFLAG                              (ADC10TOVIFG)
#define ADC10_A_OVERFLOW_INTFLAG                                   (ADC10OVIFG)
#define ADC10_A_ABOVETHRESHOLD_INTFLAG                             (ADC10HIIFG)
#define ADC10_A_BELOWTHRESHOLD_INTFLAG                             (ADC10LOIFG)
#define ADC10_A_INSIDEWINDOW_INTFLAG                               (ADC10INIFG)
#define ADC10_A_COMPLETED_INTFLAG                                   (ADC10IFG0)

//*****************************************************************************
//
// The following are values that can be passed to the
// conversionSequenceModeSelect parameter for functions:
// ADC10_A_startConversion().
//
//*****************************************************************************
#define ADC10_A_SINGLECHANNEL                                   (ADC10CONSEQ_0)
#define ADC10_A_SEQOFCHANNELS                                   (ADC10CONSEQ_1)
#define ADC10_A_REPEATED_SINGLECHANNEL                          (ADC10CONSEQ_2)
#define ADC10_A_REPEATED_SEQOFCHANNELS                          (ADC10CONSEQ_3)

//*****************************************************************************
//
// The following are values that can be passed to the preempt parameter for
// functions: ADC10_A_disableConversions().
//
//*****************************************************************************
#define ADC10_A_COMPLETECONVERSION                                        false
#define ADC10_A_PREEMPTCONVERSION                                          true

//*****************************************************************************
//
// The following are values that can be passed to the resolutionSelect
// parameter for functions: ADC10_A_setResolution().
//
//*****************************************************************************
#define ADC10_A_RESOLUTION_8BIT                                   (!(ADC10RES))
#define ADC10_A_RESOLUTION_10BIT                                     (ADC10RES)

//*****************************************************************************
//
// The following are values that can be passed to the invertedSignal parameter
// for functions: ADC10_A_setSampleHoldSignalInversion().
//
//*****************************************************************************
#define ADC10_A_NONINVERTEDSIGNAL                                (!(ADC10ISSH))
#define ADC10_A_INVERTEDSIGNAL                                      (ADC10ISSH)

//*****************************************************************************
//
// The following are values that can be passed to the readBackFormat parameter
// for functions: ADC10_A_setDataReadBackFormat().
//
//*****************************************************************************
#define ADC10_A_UNSIGNED_BINARY                                    (!(ADC10DF))
#define ADC10_A_SIGNED_2SCOMPLEMENT                                   (ADC10DF)

//*****************************************************************************
//
// The following are values that can be passed to the samplingRateSelect
// parameter for functions: ADC10_A_setReferenceBufferSamplingRate().
//
//*****************************************************************************
#define ADC10_A_MAXSAMPLINGRATE_200KSPS                            (!(ADC10SR))
#define ADC10_A_MAXSAMPLINGRATE_50KSPS                                (ADC10SR)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the ADC10_A_isBusy() function.
//
//*****************************************************************************
#define ADC10_A_BUSY                                                  ADC10BUSY
#define ADC10_A_NOTBUSY                                                    0x00

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the ADC10_A Module.
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
//! \param baseAddress is the base address of the ADC10_A module.
//! \param sampleHoldSignalSourceSelect is the signal that will trigger a
//!        sample-and-hold for an input signal to be converted. This parameter
//!        is device specific and sources should be found in the device's
//!        datasheet
//!        Valid values are:
//!        - \b ADC10_A_SAMPLEHOLDSOURCE_SC
//!        - \b ADC10_A_SAMPLEHOLDSOURCE_1
//!        - \b ADC10_A_SAMPLEHOLDSOURCE_2
//!        - \b ADC10_A_SAMPLEHOLDSOURCE_3
//!        \n Modified bits are \b ADC10SHSx of \b ADC10CTL1 register.
//! \param clockSourceSelect selects the clock that will be used by the ADC10_A
//!        core and the sampling timer if a sampling pulse mode is enabled.
//!        Valid values are:
//!        - \b ADC10_A_CLOCKSOURCE_ADC10OSC [Default] - MODOSC 5 MHz
//!           oscillator from the UCS
//!        - \b ADC10_A_CLOCKSOURCE_ACLK - The Auxiliary Clock
//!        - \b ADC10_A_CLOCKSOURCE_MCLK - The Master Clock
//!        - \b ADC10_A_CLOCKSOURCE_SMCLK - The Sub-Master Clock
//!        \n Modified bits are \b ADC10SSELx of \b ADC10CTL1 register.
//! \param clockSourceDivider selects the amount that the clock will be
//!        divided.
//!        Valid values are:
//!        - \b ADC10_A_CLOCKDIVIDER_1 [Default]
//!        - \b ADC10_A_CLOCKDIVIDER_2
//!        - \b ADC10_A_CLOCKDIVIDER_3
//!        - \b ADC10_A_CLOCKDIVIDER_4
//!        - \b ADC10_A_CLOCKDIVIDER_5
//!        - \b ADC10_A_CLOCKDIVIDER_6
//!        - \b ADC10_A_CLOCKDIVIDER_7
//!        - \b ADC10_A_CLOCKDIVIDER_8
//!        - \b ADC10_A_CLOCKDIVIDER_12
//!        - \b ADC10_A_CLOCKDIVIDER_16
//!        - \b ADC10_A_CLOCKDIVIDER_20
//!        - \b ADC10_A_CLOCKDIVIDER_24
//!        - \b ADC10_A_CLOCKDIVIDER_28
//!        - \b ADC10_A_CLOCKDIVIDER_32
//!        - \b ADC10_A_CLOCKDIVIDER_64
//!        - \b ADC10_A_CLOCKDIVIDER_128
//!        - \b ADC10_A_CLOCKDIVIDER_192
//!        - \b ADC10_A_CLOCKDIVIDER_256
//!        - \b ADC10_A_CLOCKDIVIDER_320
//!        - \b ADC10_A_CLOCKDIVIDER_384
//!        - \b ADC10_A_CLOCKDIVIDER_448
//!        - \b ADC10_A_CLOCKDIVIDER_512
//!        \n Modified bits are \b ADC10DIVx of \b ADC10CTL1 register; bits \b
//!        ADC10PDIVx of \b ADC10CTL2 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process.
//
//*****************************************************************************
extern bool ADC10_A_init(uint16_t baseAddress,
                         uint16_t sampleHoldSignalSourceSelect,
                         uint8_t clockSourceSelect,
                         uint16_t clockSourceDivider);

//*****************************************************************************
//
//! \brief Enables the ADC10_A block.
//!
//! This will enable operation of the ADC10_A block.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! Modified bits are \b ADC10ON of \b ADC10CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_enable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the ADC10_A block.
//!
//! This will disable operation of the ADC10_A block.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! Modified bits are \b ADC10ON of \b ADC10CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_disable(uint16_t baseAddress);

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
//! \param baseAddress is the base address of the ADC10_A module.
//! \param clockCycleHoldCount sets the amount of clock cycles to sample-and-
//!        hold for the memory buffer.
//!        Valid values are:
//!        - \b ADC10_A_CYCLEHOLD_4_CYCLES [Default]
//!        - \b ADC10_A_CYCLEHOLD_8_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_16_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_32_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_64_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_96_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_128_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_192_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_256_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_384_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_512_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_768_CYCLES
//!        - \b ADC10_A_CYCLEHOLD_1024_CYCLES
//!        \n Modified bits are \b ADC10SHTx of \b ADC10CTL0 register.
//! \param multipleSamplesEnabled allows multiple conversions to start without
//!        a trigger signal from the sample/hold signal
//!        Valid values are:
//!        - \b ADC10_A_MULTIPLESAMPLESDISABLE - a timer trigger will be needed
//!           to start every ADC conversion.
//!        - \b ADC10_A_MULTIPLESAMPLESENABLE - during a sequenced and/or
//!           repeated conversion mode, after the first conversion, no
//!           sample/hold signal is necessary to start subsequent samples.
//!        \n Modified bits are \b ADC10MSC of \b ADC10CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_setupSamplingTimer(uint16_t baseAddress,
                                       uint16_t clockCycleHoldCount,
                                       uint16_t multipleSamplesEnabled);

//*****************************************************************************
//
//! \brief Disables Sampling Timer Pulse Mode.
//!
//! Disables the Sampling Timer Pulse Mode. Note that if a conversion has been
//! started with the startConversion() function, then a call to
//! disableConversions() is required before this function may be called.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_disableSamplingTimer(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Configures the controls of the selected memory buffer.
//!
//! Maps an input signal conversion into the memory buffer, as well as the
//! positive and negative reference voltages for each conversion being stored
//! into the memory buffer. If the internal reference is used for the positive
//! reference voltage, the internal REF module has to control the voltage
//! level. Note that if a conversion has been started with the
//! startConversion() function, then a call to disableConversions() is required
//! before this function may be called. If conversion is not disabled, this
//! function does nothing.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param inputSourceSelect is the input that will store the converted data
//!        into the specified memory buffer.
//!        Valid values are:
//!        - \b ADC10_A_INPUT_A0 [Default]
//!        - \b ADC10_A_INPUT_A1
//!        - \b ADC10_A_INPUT_A2
//!        - \b ADC10_A_INPUT_A3
//!        - \b ADC10_A_INPUT_A4
//!        - \b ADC10_A_INPUT_A5
//!        - \b ADC10_A_INPUT_A6
//!        - \b ADC10_A_INPUT_A7
//!        - \b ADC10_A_INPUT_A8
//!        - \b ADC10_A_INPUT_A9
//!        - \b ADC10_A_INPUT_TEMPSENSOR
//!        - \b ADC10_A_INPUT_BATTERYMONITOR
//!        - \b ADC10_A_INPUT_A12
//!        - \b ADC10_A_INPUT_A13
//!        - \b ADC10_A_INPUT_A14
//!        - \b ADC10_A_INPUT_A15
//!        \n Modified bits are \b ADC10INCHx of \b ADC10MCTL0 register.
//! \param positiveRefVoltageSourceSelect is the reference voltage source to
//!        set as the upper limit for the conversion that is to be stored in
//!        the specified memory buffer.
//!        Valid values are:
//!        - \b ADC10_A_VREFPOS_AVCC [Default]
//!        - \b ADC10_A_VREFPOS_EXT
//!        - \b ADC10_A_VREFPOS_INT
//!        \n Modified bits are \b ADC10SREF of \b ADC10MCTL0 register.
//! \param negativeRefVoltageSourceSelect is the reference voltage source to
//!        set as the lower limit for the conversion that is to be stored in
//!        the specified memory buffer.
//!        Valid values are:
//!        - \b ADC10_A_VREFNEG_AVSS
//!        - \b ADC10_A_VREFNEG_EXT
//!        \n Modified bits are \b ADC10SREF of \b ADC10CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_configureMemory(uint16_t baseAddress,
                                    uint8_t inputSourceSelect,
                                    uint8_t positiveRefVoltageSourceSelect,
                                    uint8_t negativeRefVoltageSourceSelect);

//*****************************************************************************
//
//! \brief Enables selected ADC10_A interrupt sources.
//!
//! Enables the indicated ADC10_A interrupt sources. Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param interruptMask is the bit mask of the memory buffer interrupt sources
//!        to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC10_A_TIMEOVERFLOW_INT - Interrupts when a new conversion is
//!           starting before the previous one has finished
//!        - \b ADC10_A_OVERFLOW_INT - Interrupts when a new conversion is
//!           about to overwrite the previous one
//!        - \b ADC10_A_ABOVETHRESHOLD_INT - Interrupts when the input signal
//!           has gone above the high threshold of the window comparator
//!        - \b ADC10_A_BELOWTHRESHOLD_INT - Interrupts when the input signal
//!           has gone below the low threshold of the low window comparator
//!        - \b ADC10_A_INSIDEWINDOW_INT - Interrupts when the input signal is
//!           in between the high and low thresholds of the window comparator
//!        - \b ADC10_A_COMPLETED_INT - Interrupt for new conversion data in
//!           the memory buffer
//!
//! Modified bits of \b ADC10IE register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_enableInterrupt(uint16_t baseAddress,
                                    uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Disables selected ADC10_A interrupt sources.
//!
//! Disables the indicated ADC10_A interrupt sources. Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param interruptMask is the bit mask of the memory buffer interrupt sources
//!        to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC10_A_TIMEOVERFLOW_INT - Interrupts when a new conversion is
//!           starting before the previous one has finished
//!        - \b ADC10_A_OVERFLOW_INT - Interrupts when a new conversion is
//!           about to overwrite the previous one
//!        - \b ADC10_A_ABOVETHRESHOLD_INT - Interrupts when the input signal
//!           has gone above the high threshold of the window comparator
//!        - \b ADC10_A_BELOWTHRESHOLD_INT - Interrupts when the input signal
//!           has gone below the low threshold of the low window comparator
//!        - \b ADC10_A_INSIDEWINDOW_INT - Interrupts when the input signal is
//!           in between the high and low thresholds of the window comparator
//!        - \b ADC10_A_COMPLETED_INT - Interrupt for new conversion data in
//!           the memory buffer
//!
//! Modified bits of \b ADC10IE register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_disableInterrupt(uint16_t baseAddress,
                                     uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Clears ADC10_A selected interrupt flags.
//!
//! The selected ADC10_A interrupt flags are cleared, so that it no longer
//! asserts. The memory buffer interrupt flags are only cleared when the memory
//! buffer is accessed.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param interruptFlagMask is a bit mask of the interrupt flags to be
//!        cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC10_A_TIMEOVERFLOW_INTFLAG - Interrupts flag when a new
//!           conversion is starting before the previous one has finished
//!        - \b ADC10_A_OVERFLOW_INTFLAG - Interrupts flag when a new
//!           conversion is about to overwrite the previous one
//!        - \b ADC10_A_ABOVETHRESHOLD_INTFLAG - Interrupts flag when the input
//!           signal has gone above the high threshold of the window comparator
//!        - \b ADC10_A_BELOWTHRESHOLD_INTFLAG - Interrupts flag when the input
//!           signal has gone below the low threshold of the low window
//!           comparator
//!        - \b ADC10_A_INSIDEWINDOW_INTFLAG - Interrupts flag when the input
//!           signal is in between the high and low thresholds of the window
//!           comparator
//!        - \b ADC10_A_COMPLETED_INTFLAG - Interrupt flag for new conversion
//!           data in the memory buffer
//!
//! Modified bits of \b ADC10IFG register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_clearInterrupt(uint16_t baseAddress,
                                   uint8_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Returns the status of the selected memory interrupt flags.
//!
//! Returns the status of the selected interrupt flags.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param interruptFlagMask is a bit mask of the interrupt flags status to be
//!        returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b ADC10_A_TIMEOVERFLOW_INTFLAG - Interrupts flag when a new
//!           conversion is starting before the previous one has finished
//!        - \b ADC10_A_OVERFLOW_INTFLAG - Interrupts flag when a new
//!           conversion is about to overwrite the previous one
//!        - \b ADC10_A_ABOVETHRESHOLD_INTFLAG - Interrupts flag when the input
//!           signal has gone above the high threshold of the window comparator
//!        - \b ADC10_A_BELOWTHRESHOLD_INTFLAG - Interrupts flag when the input
//!           signal has gone below the low threshold of the low window
//!           comparator
//!        - \b ADC10_A_INSIDEWINDOW_INTFLAG - Interrupts flag when the input
//!           signal is in between the high and low thresholds of the window
//!           comparator
//!        - \b ADC10_A_COMPLETED_INTFLAG - Interrupt flag for new conversion
//!           data in the memory buffer
//!
//! \return The current interrupt flag status for the corresponding mask.
//
//*****************************************************************************
extern uint16_t ADC10_A_getInterruptStatus(uint16_t baseAddress,
                                           uint8_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Enables/Starts an Analog-to-Digital Conversion.
//!
//! This function enables/starts the conversion process of the ADC. If the
//! sample/hold signal source chosen during initialization was ADC10OSC, then
//! the conversion is started immediately, otherwise the chosen sample/hold
//! signal source starts the conversion by a rising edge of the signal. Keep in
//! mind when selecting conversion modes, that for sequenced and/or repeated
//! modes, to keep the sample/hold-and-convert process continuing without a
//! trigger from the sample/hold signal source, the multiple samples must be
//! enabled using the ADC10_A_setupSamplingTimer() function. Also note that
//! when a sequence conversion mode is selected, the first input channel is the
//! one mapped to the memory buffer, the next input channel selected for
//! conversion is one less than the input channel just converted (i.e. A1 comes
//! after A2), until A0 is reached, and if in repeating mode, then the next
//! input channel will again be the one mapped to the memory buffer. Note that
//! after this function is called, the ADC10_A_stopConversions() has to be
//! called to re-initialize the ADC, reconfigure a memory buffer control,
//! enable/disable the sampling timer, or to change the internal reference
//! voltage.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param conversionSequenceModeSelect determines the ADC operating mode.
//!        Valid values are:
//!        - \b ADC10_A_SINGLECHANNEL [Default] - one-time conversion of a
//!           single channel into a single memory buffer
//!        - \b ADC10_A_SEQOFCHANNELS - one time conversion of multiple
//!           channels into the specified starting memory buffer and each
//!           subsequent memory buffer up until the conversion is stored in a
//!           memory buffer dedicated as the end-of-sequence by the memory's
//!           control register
//!        - \b ADC10_A_REPEATED_SINGLECHANNEL - repeated conversions of one
//!           channel into a single memory buffer
//!        - \b ADC10_A_REPEATED_SEQOFCHANNELS - repeated conversions of
//!           multiple channels into the specified starting memory buffer and
//!           each subsequent memory buffer up until the conversion is stored
//!           in a memory buffer dedicated as the end-of-sequence by the
//!           memory's control register
//!        \n Modified bits are \b ADC10CONSEQx of \b ADC10CTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_startConversion(uint16_t baseAddress,
                                    uint8_t conversionSequenceModeSelect);

//*****************************************************************************
//
//! \brief Disables the ADC from converting any more signals.
//!
//! Disables the ADC from converting any more signals. If there is a conversion
//! in progress, this function can stop it immediately if the preempt parameter
//! is set as ADC10_A_PREEMPTCONVERSION, by changing the conversion mode to
//! single-channel, single-conversion and disabling conversions. If the
//! conversion mode is set as single-channel, single-conversion and this
//! function is called without preemption, then the ADC core conversion status
//! is polled until the conversion is complete before disabling conversions to
//! prevent unpredictable data.  If the ADC10_A_startConversion() has been
//! called, then this function has to be called to re-initialize the ADC,
//! reconfigure a memory buffer control, enable/disable the sampling pulse
//! mode, or change the internal reference voltage.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param preempt specifies if the current conversion should be pre-empted
//!        before the end of the conversion
//!        Valid values are:
//!        - \b ADC10_A_COMPLETECONVERSION - Allows the ADC10_A to end the
//!           current conversion before disabling conversions.
//!        - \b ADC10_A_PREEMPTCONVERSION - Stops the ADC10_A immediately, with
//!           unpredictable results of the current conversion. Cannot be used
//!           with repeated conversion.
//!
//! Modified bits of \b ADC10CTL1 register and bits of \b ADC10CTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_disableConversions(uint16_t baseAddress,
                                       bool preempt);

//*****************************************************************************
//
//! \brief Returns the raw contents of the specified memory buffer.
//!
//! Returns the raw contents of the specified memory buffer. The format of the
//! content depends on the read-back format of the data: if the data is in
//! signed 2's complement format then the contents in the memory buffer will be
//! left-justified with the least-significant bits as 0's, whereas if the data
//! is in unsigned format then the contents in the memory buffer will be right-
//! justified with the most-significant bits as 0's.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! \return A Signed Integer of the contents of the specified memory buffer.
//
//*****************************************************************************
extern int16_t ADC10_A_getResults(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Use to change the resolution of the converted data.
//!
//! This function can be used to change the resolution of the converted data
//! from the default of 12-bits.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param resolutionSelect determines the resolution of the converted data.
//!        Valid values are:
//!        - \b ADC10_A_RESOLUTION_8BIT
//!        - \b ADC10_A_RESOLUTION_10BIT [Default]
//!        \n Modified bits are \b ADC10RES of \b ADC10CTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_setResolution(uint16_t baseAddress,
                                  uint8_t resolutionSelect);

//*****************************************************************************
//
//! \brief Use to invert or un-invert the sample/hold signal
//!
//! This function can be used to invert or un-invert the sample/hold signal.
//! Note that if a conversion has been started with the startConversion()
//! function, then a call to disableConversions() is required before this
//! function may be called.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param invertedSignal set if the sample/hold signal should be inverted
//!        Valid values are:
//!        - \b ADC10_A_NONINVERTEDSIGNAL [Default] - a sample-and-hold of an
//!           input signal for conversion will be started on a rising edge of
//!           the sample/hold signal.
//!        - \b ADC10_A_INVERTEDSIGNAL - a sample-and-hold of an input signal
//!           for conversion will be started on a falling edge of the
//!           sample/hold signal.
//!        \n Modified bits are \b ADC10ISSH of \b ADC10CTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_setSampleHoldSignalInversion(uint16_t baseAddress,
                                                 uint16_t invertedSignal);

//*****************************************************************************
//
//! \brief Use to set the read-back format of the converted data
//!
//! Sets the format of the converted data: how it will be stored into the
//! memory buffer, and how it should be read back. The format can be set as
//! right-justified (default), which indicates that the number will be
//! unsigned, or left-justified, which indicates that the number will be signed
//! in 2's complement format. This change affects all memory buffers for
//! subsequent conversions.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param readBackFormat is the specified format to store the conversions in
//!        the memory buffer.
//!        Valid values are:
//!        - \b ADC10_A_UNSIGNED_BINARY [Default]
//!        - \b ADC10_A_SIGNED_2SCOMPLEMENT
//!        \n Modified bits are \b ADC10DF of \b ADC10CTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_setDataReadBackFormat(uint16_t baseAddress,
                                          uint16_t readBackFormat);

//*****************************************************************************
//
//! \brief Enables the reference buffer's burst ability.
//!
//! Enables the reference buffer's burst ability, allowing the reference buffer
//! to turn off while the ADC is not converting, and automatically turning on
//! when the ADC needs the generated reference voltage for a conversion.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_enableReferenceBurst(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the reference buffer's burst ability.
//!
//! Disables the reference buffer's burst ability, forcing the reference buffer
//! to remain on continuously.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_disableReferenceBurst(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Use to set the reference buffer's sampling rate.
//!
//! Sets the reference buffer's sampling rate to the selected sampling rate.
//! The default sampling rate is maximum of 200-ksps, and can be reduced to a
//! maximum of 50-ksps to conserve power.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param samplingRateSelect is the specified maximum sampling rate.
//!        Valid values are:
//!        - \b ADC10_A_MAXSAMPLINGRATE_200KSPS [Default]
//!        - \b ADC10_A_MAXSAMPLINGRATE_50KSPS
//!        \n Modified bits are \b ADC10SR of \b ADC10CTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_setReferenceBufferSamplingRate(uint16_t baseAddress,
                                                   uint16_t samplingRateSelect);

//*****************************************************************************
//
//! \brief Sets the high and low threshold for the window comparator feature.
//!
//! Sets the high and low threshold for the window comparator feature. Use the
//! ADC10HIIE, ADC10INIE, ADC10LOIE interrupts to utilize this feature.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//! \param highThreshold is the upper bound that could trip an interrupt for
//!        the window comparator.
//! \param lowThreshold is the lower bound that could trip on interrupt for the
//!        window comparator.
//!
//! \return None
//
//*****************************************************************************
extern void ADC10_A_setWindowComp(uint16_t baseAddress,
                                  uint16_t highThreshold,
                                  uint16_t lowThreshold);

//*****************************************************************************
//
//! \brief Returns the address of the memory buffer for the DMA module.
//!
//! Returns the address of the memory buffer. This can be used in conjunction
//! with the DMA to store the converted data directly to memory.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! \return The memory address of the memory buffer
//
//*****************************************************************************
extern uint32_t ADC10_A_getMemoryAddressForDMA(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the busy status of the ADC10_A core.
//!
//! Returns the status of the ADC core if there is a conversion currently
//! taking place.
//!
//! \param baseAddress is the base address of the ADC10_A module.
//!
//! \return One of the following:
//!         - \b ADC10_A_BUSY
//!         - \b ADC10_A_NOTBUSY
//!         \n indicating if there is a conversion currently taking place
//
//*****************************************************************************
extern uint16_t ADC10_A_isBusy(uint16_t baseAddress);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_ADC10_A_H__
