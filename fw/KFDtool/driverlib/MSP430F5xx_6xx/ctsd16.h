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
// ctsd16.h - Driver for the CTSD16 Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_CTSD16_H__
#define __MSP430WARE_CTSD16_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_CTSD16__

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
//! \brief Used in the CTSD16_initConverterAdvanced() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct CTSD16_initConverterAdvancedParam {
    //! The converter to initialize.
    //! \n Valid values are:
    //! - \b CTSD16_CONVERTER_0
    //! - \b CTSD16_CONVERTER_1
    //! - \b CTSD16_CONVERTER_2
    //! - \b CTSD16_CONVERTER_3
    //! - \b CTSD16_CONVERTER_4
    //! - \b CTSD16_CONVERTER_5
    //! - \b CTSD16_CONVERTER_6
    uint8_t converter;
    //! Determines whether the converter will do continuous samples or a single
    //! sample
    //! \n Valid values are:
    //! - \b CTSD16_CONTINUOUS_MODE [Default]
    //! - \b CTSD16_SINGLE_MODE
    uint16_t conversionMode;
    //! \n Valid values are:
    //! - \b CTSD16_NOT_GROUPED
    //! - \b CTSD16_GROUPED [Default]
    uint8_t groupEnable;
    //! Selects oversampling ratio for the converter
    //! \n Valid values are:
    //! - \b CTSD16_INPUT_CH0
    //! - \b CTSD16_INPUT_CH1
    //! - \b CTSD16_INPUT_CH2
    //! - \b CTSD16_INPUT_CH3
    //! - \b CTSD16_INPUT_CH4
    //! - \b CTSD16_INPUT_CH5
    //! - \b CTSD16_INPUT_CH6
    //! - \b CTSD16_INPUT_CH7
    //! - \b CTSD16_INPUT_CH8
    //! - \b CTSD16_INPUT_CH9
    //! - \b CTSD16_INPUT_CH10
    //! - \b CTSD16_INPUT_CH11
    //! - \b CTSD16_INPUT_CH12
    //! - \b CTSD16_INPUT_CH13
    //! - \b CTSD16_INPUT_CH14
    //! - \b CTSD16_INPUT_CH15
    //! - \b CTSD16_INPUT_CH16
    //! - \b CTSD16_INPUT_CH17
    //! - \b CTSD16_INPUT_CH18
    uint8_t inputChannel;
    //! Selects how the data format of the results
    //! \n Valid values are:
    //! - \b CTSD16_DATA_FORMAT_BINARY [Default]
    //! - \b CTSD16_DATA_FORMAT_2COMPLEMENT
    uint8_t dataFormat;
    //! Determines the rail-to-rail input enabled or disabled.
    //! \n Valid values are:
    //! - \b CTSD16_RTR_INPUT_ENABLE
    //! - \b CTSD16_RTR_INPUT_DISABLE [Default]
    uint16_t railToRailInput;
    //! Selects the delay for the interrupt
    //! \n Valid values are:
    //! - \b CTSD16_FIRST_SAMPLE_INTERRUPT
    //! - \b CTSD16_FOURTH_SAMPLE_INTERRUPT [Default]
    uint16_t interruptDelay;
    //! Selects oversampling ratio for the converter
    //! \n Valid values are:
    //! - \b CTSD16_OVERSAMPLE_32
    //! - \b CTSD16_OVERSAMPLE_64
    //! - \b CTSD16_OVERSAMPLE_128
    //! - \b CTSD16_OVERSAMPLE_256 [Default]
    uint16_t oversampleRatio;
    //! Selects the gain for the converter
    //! \n Valid values are:
    //! - \b CTSD16_GAIN_1 [Default]
    //! - \b CTSD16_GAIN_2
    //! - \b CTSD16_GAIN_4
    //! - \b CTSD16_GAIN_8
    //! - \b CTSD16_GAIN_16
    uint8_t gain;
} CTSD16_initConverterAdvancedParam;


//*****************************************************************************
//
// The following are values that can be passed to the converter parameter for
// functions: CTSD16_setConverterDataFormat(),
// CTSD16_startConverterConversion(), CTSD16_stopConverterConversion(),
// CTSD16_setInputChannel(), CTSD16_setInterruptDelay(),
// CTSD16_setOversampling(), CTSD16_setGain(), CTSD16_setRailToRailInput(),
// CTSD16_getResults(), CTSD16_getHighWordResults(), CTSD16_enableInterrupt(),
// CTSD16_disableInterrupt(), CTSD16_clearInterrupt(),
// CTSD16_getInterruptStatus(), and CTSD16_initConverter(); the param parameter
// for functions: CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_CONVERTER_0                                                    0
#define CTSD16_CONVERTER_1                                                    1
#define CTSD16_CONVERTER_2                                                    2
#define CTSD16_CONVERTER_3                                                    3
#define CTSD16_CONVERTER_4                                                    4
#define CTSD16_CONVERTER_5                                                    5
#define CTSD16_CONVERTER_6                                                    6

//*****************************************************************************
//
// The following are values that can be passed to the oversampleRatio parameter
// for functions: CTSD16_setOversampling(); the param parameter for functions:
// CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_OVERSAMPLE_32                                    (CTSD16OSR__32)
#define CTSD16_OVERSAMPLE_64                                    (CTSD16OSR__64)
#define CTSD16_OVERSAMPLE_128                                  (CTSD16OSR__128)
#define CTSD16_OVERSAMPLE_256                                  (CTSD16OSR__256)

//*****************************************************************************
//
// The following are values that can be passed to the inputChannel parameter
// for functions: CTSD16_initConverter(), and CTSD16_setInputChannel(); the
// param parameter for functions: CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_INPUT_CH0                                         (CTSD16INCH_0)
#define CTSD16_INPUT_CH1                                         (CTSD16INCH_1)
#define CTSD16_INPUT_CH2                                         (CTSD16INCH_2)
#define CTSD16_INPUT_CH3                                         (CTSD16INCH_3)
#define CTSD16_INPUT_CH4                                         (CTSD16INCH_4)
#define CTSD16_INPUT_CH5                                         (CTSD16INCH_5)
#define CTSD16_INPUT_CH6                                         (CTSD16INCH_6)
#define CTSD16_INPUT_CH7                                         (CTSD16INCH_7)
#define CTSD16_INPUT_CH8                                         (CTSD16INCH_8)
#define CTSD16_INPUT_CH9                                         (CTSD16INCH_9)
#define CTSD16_INPUT_CH10                                       (CTSD16INCH_10)
#define CTSD16_INPUT_CH11                                       (CTSD16INCH_11)
#define CTSD16_INPUT_CH12                                       (CTSD16INCH_12)
#define CTSD16_INPUT_CH13                                       (CTSD16INCH_13)
#define CTSD16_INPUT_CH14                                       (CTSD16INCH_14)
#define CTSD16_INPUT_CH15                                       (CTSD16INCH_15)
#define CTSD16_INPUT_CH16                                       (CTSD16INCH_16)
#define CTSD16_INPUT_CH17                                       (CTSD16INCH_17)
#define CTSD16_INPUT_CH18                                       (CTSD16INCH_18)

//*****************************************************************************
//
// The following are values that can be passed to the dataFormat parameter for
// functions: CTSD16_setConverterDataFormat(); the param parameter for
// functions: CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_DATA_FORMAT_BINARY                                        (0x00)
#define CTSD16_DATA_FORMAT_2COMPLEMENT                               (CTSD16DF)

//*****************************************************************************
//
// The following are values that can be passed to the gain parameter for
// functions: CTSD16_setGain(); the param parameter for functions:
// CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_GAIN_1                                            (CTSD16GAIN_1)
#define CTSD16_GAIN_2                                            (CTSD16GAIN_2)
#define CTSD16_GAIN_4                                            (CTSD16GAIN_4)
#define CTSD16_GAIN_8                                            (CTSD16GAIN_8)
#define CTSD16_GAIN_16                                          (CTSD16GAIN_16)

//*****************************************************************************
//
// The following are values that can be passed to the interruptDelay parameter
// for functions: CTSD16_setInterruptDelay(); the param parameter for
// functions: CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_FIRST_SAMPLE_INTERRUPT                            (CTSD16INTDLY)
#define CTSD16_FOURTH_SAMPLE_INTERRUPT                                   (0x00)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_NOT_GROUPED                                               (0x00)
#define CTSD16_GROUPED                                              (CTSD16GRP)

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: CTSD16_enableInterrupt(), CTSD16_disableInterrupt(),
// CTSD16_clearInterrupt(), and CTSD16_getInterruptStatus() as well as returned
// by the CTSD16_getInterruptStatus() function.
//
//*****************************************************************************
#define CTSD16_CONVERTER_INTERRUPT                                  (CTSD16IE0)
#define CTSD16_CONVERTER_OVERFLOW_INTERRUPT                       (CTSD16OVIE0)

//*****************************************************************************
//
// The following are values that can be passed to the chargePumpBurstRequest
// parameter for functions: CTSD16_init().
//
//*****************************************************************************
#define CTSD16_RTR_INPUT_CHARGEPUMP_BURST_REQUEST_ENABLE       (CTSD16RRIBURST)
#define CTSD16_RTR_INPUT_CHARGEPUMP_BURST_REQUEST_DISABLE                 (0x0)

//*****************************************************************************
//
// The following are values that can be passed to the referenceSelect parameter
// for functions: CTSD16_init().
//
//*****************************************************************************
#define CTSD16_REF_EXTERNAL                                               (0x0)
#define CTSD16_REF_INTERNAL                                        (CTSD16REFS)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: CTSD16_initConverterAdvanced(); the conversionMode parameter for
// functions: CTSD16_initConverter().
//
//*****************************************************************************
#define CTSD16_CONTINUOUS_MODE                                            (0x0)
#define CTSD16_SINGLE_MODE                                         (CTSD16SNGL)

//*****************************************************************************
//
// The following are values that can be passed to the railToRailInput parameter
// for functions: CTSD16_setRailToRailInput(); the param parameter for
// functions: CTSD16_initConverterAdvanced().
//
//*****************************************************************************
#define CTSD16_RTR_INPUT_ENABLE                                     (CTSD16RRI)
#define CTSD16_RTR_INPUT_DISABLE                                          (0x0)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the CTSD16_isRailToRailInputReady() function.
//
//*****************************************************************************
#define CTSD16_RTR_INPUT_READY                                   (CTSD16RRIRDY)
#define CTSD16_RTR_INPUT_NOT_READY                                        (0x0)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the CTSD16 Module
//!
//! This function initializes the CTSD16 module sigma-delta analog-to-digital
//! conversions. Specifically the function sets up the clock source for the
//! CTSD16 core to use for conversions. Upon completion of the initialization
//! the CTSD16 interrupt registers will be reset and the given parameters will
//! be set. The converter configuration settings are independent of this
//! function. The default clock frequency for CTSD16 is: f_CTSD16 = 1.024MHz
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param chargePumpBurstRequest sets the rail-to-rail input charge pump burst
//!        mode request enabled or disabled.
//!        Valid values are:
//!        - \b CTSD16_RTR_INPUT_CHARGEPUMP_BURST_REQUEST_ENABLE
//!        - \b CTSD16_RTR_INPUT_CHARGEPUMP_BURST_REQUEST_DISABLE [Default]
//!        \n Modified bits are \b CTSD16RRIBURST of \b CTSD16CTL register.
//! \param referenceSelect selects the reference source for the CTSD16 core.
//!        Valid values are:
//!        - \b CTSD16_REF_EXTERNAL [Default]
//!        - \b CTSD16_REF_INTERNAL
//!        \n Modified bits are \b CTSD16REFS of \b CTSD16CTL register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_init(uint16_t baseAddress,
                        uint16_t chargePumpBurstRequest,
                        uint8_t referenceSelect);

//*****************************************************************************
//
//! \brief Configure CTSD16 converter
//!
//! This function initializes a converter of the CTSD16 module. Upon completion
//! the converter will be ready for a conversion and can be started with the
//! CTSD16_startConverterConversion(). Additional configuration such as data
//! format can be configured in CTSD16_setDataFormat().
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter the converter to initialize.
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param conversionMode determines whether the converter will do continuous
//!        samples or a single sample.
//!        Valid values are:
//!        - \b CTSD16_CONTINUOUS_MODE [Default]
//!        - \b CTSD16_SINGLE_MODE
//!        \n Modified bits are \b CTSD16SNGL of \b CTSD16CTL register.
//! \param inputChannel selects oversampling ratio for the converter
//!        Valid values are:
//!        - \b CTSD16_INPUT_CH0
//!        - \b CTSD16_INPUT_CH1
//!        - \b CTSD16_INPUT_CH2
//!        - \b CTSD16_INPUT_CH3
//!        - \b CTSD16_INPUT_CH4
//!        - \b CTSD16_INPUT_CH5
//!        - \b CTSD16_INPUT_CH6
//!        - \b CTSD16_INPUT_CH7
//!        - \b CTSD16_INPUT_CH8
//!        - \b CTSD16_INPUT_CH9
//!        - \b CTSD16_INPUT_CH10
//!        - \b CTSD16_INPUT_CH11
//!        - \b CTSD16_INPUT_CH12
//!        - \b CTSD16_INPUT_CH13
//!        - \b CTSD16_INPUT_CH14
//!        - \b CTSD16_INPUT_CH15
//!        - \b CTSD16_INPUT_CH16
//!        - \b CTSD16_INPUT_CH17
//!        - \b CTSD16_INPUT_CH18
//!        \n Modified bits are \b CTSD16INCHx of \b CTSD16INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_initConverter(uint16_t baseAddress,
                                 uint8_t converter,
                                 uint16_t conversionMode,
                                 uint8_t inputChannel);

//*****************************************************************************
//
//! \brief Configure CTSD16 converter - Advanced Configure
//!
//! This function initializes a converter of the CTSD16 module with more
//! configuration parameters. Upon completion the converter will be ready for a
//! conversion and can be started with the CTSD16_startConverterConversion().
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param param for initilization
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_initConverterAdvanced(uint16_t baseAddress,
                                         CTSD16_initConverterAdvancedParam *param);

//*****************************************************************************
//
//! \brief Start Conversion for Converter
//!
//! This function starts a single converter.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be started
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//!        \n Modified bits are \b CTSD16SC of \b CTSD16CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_startConverterConversion(uint16_t baseAddress,
                                            uint8_t converter);

//*****************************************************************************
//
//! \brief Stop Conversion for Converter
//!
//! This function stops a single converter.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be stopped
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//!        \n Modified bits are \b CTSD16SC of \b CTSD16CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_stopConverterConversion(uint16_t baseAddress,
                                           uint8_t converter);

//*****************************************************************************
//
//! \brief Set CTSD16 converter data format
//!
//! This function sets the converter format so that the resulting data can be
//! viewed in either binary or 2's complement.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be configured. Check check
//!        datasheet for available converters on device.
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param dataFormat selects how the data format of the results
//!        Valid values are:
//!        - \b CTSD16_DATA_FORMAT_BINARY [Default]
//!        - \b CTSD16_DATA_FORMAT_2COMPLEMENT
//!        \n Modified bits are \b CTSD16DFx of \b CTSD16CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_setConverterDataFormat(uint16_t baseAddress,
                                          uint8_t converter,
                                          uint8_t dataFormat);

//*****************************************************************************
//
//! \brief Set the input channel for specified converter
//!
//! This function configures the input channel. For MSP430F5xx_6xx devices,
//! users can choose either analog input or internal temperature input.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param inputChannel selects oversampling ratio for the converter
//!        Valid values are:
//!        - \b CTSD16_INPUT_CH0
//!        - \b CTSD16_INPUT_CH1
//!        - \b CTSD16_INPUT_CH2
//!        - \b CTSD16_INPUT_CH3
//!        - \b CTSD16_INPUT_CH4
//!        - \b CTSD16_INPUT_CH5
//!        - \b CTSD16_INPUT_CH6
//!        - \b CTSD16_INPUT_CH7
//!        - \b CTSD16_INPUT_CH8
//!        - \b CTSD16_INPUT_CH9
//!        - \b CTSD16_INPUT_CH10
//!        - \b CTSD16_INPUT_CH11
//!        - \b CTSD16_INPUT_CH12
//!        - \b CTSD16_INPUT_CH13
//!        - \b CTSD16_INPUT_CH14
//!        - \b CTSD16_INPUT_CH15
//!        - \b CTSD16_INPUT_CH16
//!        - \b CTSD16_INPUT_CH17
//!        - \b CTSD16_INPUT_CH18
//!        \n Modified bits are \b CTSD16INCHx of \b CTSD16INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_setInputChannel(uint16_t baseAddress,
                                   uint8_t converter,
                                   uint8_t inputChannel);

//*****************************************************************************
//
//! \brief Enable/Disable rail-to-rail input for specified converter
//!
//! This function enable/disables rail-to-rail input for specified converter.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param railToRailInput sets rail-to-rail input enabled/disabled.
//!        Valid values are:
//!        - \b CTSD16_RTR_INPUT_ENABLE
//!        - \b CTSD16_RTR_INPUT_DISABLE [Default]
//!        \n Modified bits are \b CTSD16RRI of \b CTSD16INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_setRailToRailInput(uint16_t baseAddress,
                                      uint8_t converter,
                                      uint16_t railToRailInput);

//*****************************************************************************
//
//! \brief Configures the delay for an interrupt to trigger
//!
//! This function configures the delay for the first interrupt service request
//! for the corresponding converter. This feature delays the interrupt request
//! for a completed conversion by up to four conversion cycles allowing the
//! digital filter to settle prior to generating an interrupt request.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be stopped
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param interruptDelay selects the delay for the interrupt
//!        Valid values are:
//!        - \b CTSD16_FIRST_SAMPLE_INTERRUPT
//!        - \b CTSD16_FOURTH_SAMPLE_INTERRUPT [Default]
//!        \n Modified bits are \b CTSD16INTDLYx of \b CTSD16INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_setInterruptDelay(uint16_t baseAddress,
                                     uint8_t converter,
                                     uint16_t interruptDelay);

//*****************************************************************************
//
//! \brief Configures the oversampling ratio for a converter
//!
//! This function configures the oversampling ratio for a given converter.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param oversampleRatio selects oversampling ratio for the converter
//!        Valid values are:
//!        - \b CTSD16_OVERSAMPLE_32
//!        - \b CTSD16_OVERSAMPLE_64
//!        - \b CTSD16_OVERSAMPLE_128
//!        - \b CTSD16_OVERSAMPLE_256 [Default]
//!        \n Modified bits are \b CTSD16OSRx of \b CTSD16CCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_setOversampling(uint16_t baseAddress,
                                   uint8_t converter,
                                   uint16_t oversampleRatio);

//*****************************************************************************
//
//! \brief Configures the gain for the converter
//!
//! This function configures the gain for a single converter.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param gain selects the gain for the converter
//!        Valid values are:
//!        - \b CTSD16_GAIN_1 [Default]
//!        - \b CTSD16_GAIN_2
//!        - \b CTSD16_GAIN_4
//!        - \b CTSD16_GAIN_8
//!        - \b CTSD16_GAIN_16
//!        \n Modified bits are \b CTSD16GAINx of \b CTSD16INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_setGain(uint16_t baseAddress,
                           uint8_t converter,
                           uint8_t gain);

//*****************************************************************************
//
//! \brief Returns the results for a converter
//!
//! This function gets the results from the CTSD16MEMx register for upper
//! 16-bit and lower 16-bit results, and concatenates them to form a long. The
//! actual result is a maximum 24 bits.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter who's results will be returned
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//!
//! \return Result of conversion
//
//*****************************************************************************
extern uint32_t CTSD16_getResults(uint16_t baseAddress,
                                  uint8_t converter);

//*****************************************************************************
//
//! \brief Returns the high word results for a converter
//!
//! This function gets the upper 16-bit result from the CTSD16MEMx register and
//! returns it.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter selects the converter who's results will be returned
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//!
//! \return High word result of conversion
//
//*****************************************************************************
extern uint16_t CTSD16_getHighWordResults(uint16_t baseAddress,
                                          uint8_t converter);

//*****************************************************************************
//
//! \brief Returns the rail-to-rail input ready status
//!
//! This function returns the rail-to-rail input ready status.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//!
//! \return ready status
//!         Return Logical OR of any of the following:
//!         - \b CTSD16_RTR_INPUT_READY
//!         - \b CTSD16_RTR_INPUT_NOT_READY
//!         \n Returns the rail-to-rail input ready status.
//
//*****************************************************************************
extern uint16_t CTSD16_isRailToRailInputReady(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables interrupts for the CTSD16 Module
//!
//! This function enables interrupts for the CTSD16 module. Does not clear
//! interrupt flags.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param mask is the bit mask of the converter interrupt sources to be
//!        enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b CTSD16_CONVERTER_INTERRUPT
//!        - \b CTSD16_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b CTSD16OVIEx of \b CTSD16IE register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_enableInterrupt(uint16_t baseAddress,
                                   uint8_t converter,
                                   uint16_t mask);

//*****************************************************************************
//
//! \brief Disables interrupts for the CTSD16 Module
//!
//! This function disables interrupts for the CTSD16 module.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param mask is the bit mask of the converter interrupt sources to be
//!        disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b CTSD16_CONVERTER_INTERRUPT
//!        - \b CTSD16_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b CTSD16OVIEx of \b CTSD16IE register.
//!
//! Modified bits of \b SD24BIE register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_disableInterrupt(uint16_t baseAddress,
                                    uint8_t converter,
                                    uint16_t mask);

//*****************************************************************************
//
//! \brief Clears interrupts for the CTSD16 Module
//!
//! This function clears interrupt flags for the CTSD16 module.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param mask is the bit mask of the converter interrupt sources to clear.
//!        Mask value is the logical OR of any of the following:
//!        - \b CTSD16_CONVERTER_INTERRUPT
//!        - \b CTSD16_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b CTSD16OVIFGx of \b CTSD16IFG register.
//!
//! \return None
//
//*****************************************************************************
extern void CTSD16_clearInterrupt(uint16_t baseAddress,
                                  uint8_t converter,
                                  uint16_t mask);

//*****************************************************************************
//
//! \brief Returns the interrupt status for the CTSD16 Module
//!
//! This function returns interrupt flag statuses for the CTSD16 module.
//!
//! \param baseAddress is the base address of the CTSD16 module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b CTSD16_CONVERTER_0
//!        - \b CTSD16_CONVERTER_1
//!        - \b CTSD16_CONVERTER_2
//!        - \b CTSD16_CONVERTER_3
//!        - \b CTSD16_CONVERTER_4
//!        - \b CTSD16_CONVERTER_5
//!        - \b CTSD16_CONVERTER_6
//! \param mask is the bit mask of the converter interrupt sources to return.
//!        Mask value is the logical OR of any of the following:
//!        - \b CTSD16_CONVERTER_INTERRUPT
//!        - \b CTSD16_CONVERTER_OVERFLOW_INTERRUPT
//!
//! \return Logical OR of any of the following:
//!         - \b CTSD16_CONVERTER_INTERRUPT
//!         - \b CTSD16_CONVERTER_OVERFLOW_INTERRUPT
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint16_t CTSD16_getInterruptStatus(uint16_t baseAddress,
                                          uint8_t converter,
                                          uint16_t mask);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_CTSD16_H__
