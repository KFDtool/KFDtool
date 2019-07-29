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
// sd24_b.h - Driver for the SD24_B Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_SD24_B_H__
#define __MSP430WARE_SD24_B_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_SD24_B__

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
//! \brief Used in the SD24_B_init() function as the param parameter.
//
//*****************************************************************************
typedef struct SD24_B_initParam {
    //! Selects the clock that will be used as the SD24_B core
    //! \n Valid values are:
    //! - \b SD24_B_CLOCKSOURCE_MCLK [Default]
    //! - \b SD24_B_CLOCKSOURCE_SMCLK
    //! - \b SD24_B_CLOCKSOURCE_ACLK
    //! - \b SD24_B_CLOCKSOURCE_SD24CLK
    uint16_t clockSourceSelect;
    //! Selects the amount that the clock will be predivided
    //! \n Valid values are:
    //! - \b SD24_B_PRECLOCKDIVIDER_1 [Default]
    //! - \b SD24_B_PRECLOCKDIVIDER_2
    //! - \b SD24_B_PRECLOCKDIVIDER_4
    //! - \b SD24_B_PRECLOCKDIVIDER_8
    //! - \b SD24_B_PRECLOCKDIVIDER_16
    //! - \b SD24_B_PRECLOCKDIVIDER_32
    //! - \b SD24_B_PRECLOCKDIVIDER_64
    //! - \b SD24_B_PRECLOCKDIVIDER_128
    uint16_t clockPreDivider;
    //! Selects the amount that the clock will be divided.
    //! \n Valid values are:
    //! - \b SD24_B_CLOCKDIVIDER_1 [Default]
    //! - \b SD24_B_CLOCKDIVIDER_2
    //! - \b SD24_B_CLOCKDIVIDER_3
    //! - \b SD24_B_CLOCKDIVIDER_4
    //! - \b SD24_B_CLOCKDIVIDER_5
    //! - \b SD24_B_CLOCKDIVIDER_6
    //! - \b SD24_B_CLOCKDIVIDER_7
    //! - \b SD24_B_CLOCKDIVIDER_8
    //! - \b SD24_B_CLOCKDIVIDER_9
    //! - \b SD24_B_CLOCKDIVIDER_10
    //! - \b SD24_B_CLOCKDIVIDER_11
    //! - \b SD24_B_CLOCKDIVIDER_12
    //! - \b SD24_B_CLOCKDIVIDER_13
    //! - \b SD24_B_CLOCKDIVIDER_14
    //! - \b SD24_B_CLOCKDIVIDER_15
    //! - \b SD24_B_CLOCKDIVIDER_16
    //! - \b SD24_B_CLOCKDIVIDER_17
    //! - \b SD24_B_CLOCKDIVIDER_18
    //! - \b SD24_B_CLOCKDIVIDER_19
    //! - \b SD24_B_CLOCKDIVIDER_20
    //! - \b SD24_B_CLOCKDIVIDER_21
    //! - \b SD24_B_CLOCKDIVIDER_22
    //! - \b SD24_B_CLOCKDIVIDER_23
    //! - \b SD24_B_CLOCKDIVIDER_24
    //! - \b SD24_B_CLOCKDIVIDER_25
    //! - \b SD24_B_CLOCKDIVIDER_26
    //! - \b SD24_B_CLOCKDIVIDER_27
    //! - \b SD24_B_CLOCKDIVIDER_28
    //! - \b SD24_B_CLOCKDIVIDER_29
    //! - \b SD24_B_CLOCKDIVIDER_30
    //! - \b SD24_B_CLOCKDIVIDER_31
    //! - \b SD24_B_CLOCKDIVIDER_32
    uint16_t clockDivider;
    //! Selects the reference source for the SD24_B core
    //! \n Valid values are:
    //! - \b SD24_B_REF_EXTERNAL [Default]
    //! - \b SD24_B_REF_INTERNAL
    uint16_t referenceSelect;
} SD24_B_initParam;

//*****************************************************************************
//
//! \brief Used in the SD24_B_initConverter() function as the param parameter.
//
//*****************************************************************************
typedef struct SD24_B_initConverterParam {
    //! Selects the converter that will be configured. Check datasheet for
    //! available converters on device.
    //! \n Valid values are:
    //! - \b SD24_B_CONVERTER_0
    //! - \b SD24_B_CONVERTER_1
    //! - \b SD24_B_CONVERTER_2
    //! - \b SD24_B_CONVERTER_3
    //! - \b SD24_B_CONVERTER_4
    //! - \b SD24_B_CONVERTER_5
    //! - \b SD24_B_CONVERTER_6
    //! - \b SD24_B_CONVERTER_7
    uint8_t converter;
    //! Selects how the data will be aligned in result
    //! \n Valid values are:
    //! - \b SD24_B_ALIGN_RIGHT [Default]
    //! - \b SD24_B_ALIGN_LEFT
    uint8_t alignment;
    //! Selects what will trigger the start of the converter
    //! \n Valid values are:
    //! - \b SD24_B_CONVERSION_SELECT_SD24SC [Default]
    //! - \b SD24_B_CONVERSION_SELECT_EXT1
    //! - \b SD24_B_CONVERSION_SELECT_EXT2
    //! - \b SD24_B_CONVERSION_SELECT_EXT3
    //! - \b SD24_B_CONVERSION_SELECT_GROUP0
    //! - \b SD24_B_CONVERSION_SELECT_GROUP1
    //! - \b SD24_B_CONVERSION_SELECT_GROUP2
    //! - \b SD24_B_CONVERSION_SELECT_GROUP3
    uint8_t startSelect;
    //! Determines whether the converter will do continuous samples or a single
    //! sample
    //! \n Valid values are:
    //! - \b SD24_B_CONTINUOUS_MODE [Default]
    //! - \b SD24_B_SINGLE_MODE
    uint8_t conversionMode;
} SD24_B_initConverterParam;

//*****************************************************************************
//
//! \brief Used in the SD24_B_initConverterAdvanced() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct SD24_B_initConverterAdvancedParam {
    //! Selects the converter that will be configured. Check datasheet for
    //! available converters on device.
    //! \n Valid values are:
    //! - \b SD24_B_CONVERTER_0
    //! - \b SD24_B_CONVERTER_1
    //! - \b SD24_B_CONVERTER_2
    //! - \b SD24_B_CONVERTER_3
    //! - \b SD24_B_CONVERTER_4
    //! - \b SD24_B_CONVERTER_5
    //! - \b SD24_B_CONVERTER_6
    //! - \b SD24_B_CONVERTER_7
    uint8_t converter;
    //! Selects how the data will be aligned in result
    //! \n Valid values are:
    //! - \b SD24_B_ALIGN_RIGHT [Default]
    //! - \b SD24_B_ALIGN_LEFT
    uint8_t alignment;
    //! Selects what will trigger the start of the converter
    //! \n Valid values are:
    //! - \b SD24_B_CONVERSION_SELECT_SD24SC [Default]
    //! - \b SD24_B_CONVERSION_SELECT_EXT1
    //! - \b SD24_B_CONVERSION_SELECT_EXT2
    //! - \b SD24_B_CONVERSION_SELECT_EXT3
    //! - \b SD24_B_CONVERSION_SELECT_GROUP0
    //! - \b SD24_B_CONVERSION_SELECT_GROUP1
    //! - \b SD24_B_CONVERSION_SELECT_GROUP2
    //! - \b SD24_B_CONVERSION_SELECT_GROUP3
    uint8_t startSelect;
    //! Determines whether the converter will do continuous samples or a single
    //! sample
    //! \n Valid values are:
    //! - \b SD24_B_CONTINUOUS_MODE [Default]
    //! - \b SD24_B_SINGLE_MODE
    uint8_t conversionMode;
    //! Selects how the data format of the results
    //! \n Valid values are:
    //! - \b SD24_B_DATA_FORMAT_BINARY [Default]
    //! - \b SD24_B_DATA_FORMAT_2COMPLEMENT
    uint8_t dataFormat;
    //! Selects the delay for the interrupt
    //! \n Valid values are:
    //! - \b SD24_B_FOURTH_SAMPLE_INTERRUPT [Default]
    //! - \b SD24_B_THIRD_SAMPLE_INTERRUPT
    //! - \b SD24_B_SECOND_SAMPLE_INTERRUPT
    //! - \b SD24_B_FIRST_SAMPLE_INTERRUPT
    uint8_t sampleDelay;
    //! Selects oversampling ratio for the converter
    //! \n Valid values are:
    //! - \b SD24_B_OVERSAMPLE_32
    //! - \b SD24_B_OVERSAMPLE_64
    //! - \b SD24_B_OVERSAMPLE_128
    //! - \b SD24_B_OVERSAMPLE_256
    //! - \b SD24_B_OVERSAMPLE_512
    //! - \b SD24_B_OVERSAMPLE_1024
    uint16_t oversampleRatio;
    //! Selects the gain for the converter
    //! \n Valid values are:
    //! - \b SD24_B_GAIN_1 [Default]
    //! - \b SD24_B_GAIN_2
    //! - \b SD24_B_GAIN_4
    //! - \b SD24_B_GAIN_8
    //! - \b SD24_B_GAIN_16
    //! - \b SD24_B_GAIN_32
    //! - \b SD24_B_GAIN_64
    //! - \b SD24_B_GAIN_128
    uint8_t gain;
} SD24_B_initConverterAdvancedParam;


//*****************************************************************************
//
// The following are values that can be passed to the clockSourceSelect
// parameter for functions: SD24_B_init(); the param parameter for functions:
// SD24_B_init().
//
//*****************************************************************************
#define SD24_B_CLOCKSOURCE_MCLK                                (SD24SSEL__MCLK)
#define SD24_B_CLOCKSOURCE_SMCLK                              (SD24SSEL__SMCLK)
#define SD24_B_CLOCKSOURCE_ACLK                                (SD24SSEL__ACLK)
#define SD24_B_CLOCKSOURCE_SD24CLK                          (SD24SSEL__SD24CLK)

//*****************************************************************************
//
// The following are values that can be passed to the referenceSelect parameter
// for functions: SD24_B_init(); the param parameter for functions:
// SD24_B_init().
//
//*****************************************************************************
#define SD24_B_REF_EXTERNAL                                              (0x00)
#define SD24_B_REF_INTERNAL                                          (SD24REFS)

//*****************************************************************************
//
// The following are values that can be passed to the clockPreDivider parameter
// for functions: SD24_B_init(); the param parameter for functions:
// SD24_B_init().
//
//*****************************************************************************
#define SD24_B_PRECLOCKDIVIDER_1                                   (SD24PDIV_0)
#define SD24_B_PRECLOCKDIVIDER_2                                   (SD24PDIV_1)
#define SD24_B_PRECLOCKDIVIDER_4                                   (SD24PDIV_2)
#define SD24_B_PRECLOCKDIVIDER_8                                   (SD24PDIV_3)
#define SD24_B_PRECLOCKDIVIDER_16                                  (SD24PDIV_4)
#define SD24_B_PRECLOCKDIVIDER_32                                  (SD24PDIV_5)
#define SD24_B_PRECLOCKDIVIDER_64                                  (SD24PDIV_6)
#define SD24_B_PRECLOCKDIVIDER_128                                 (SD24PDIV_7)

//*****************************************************************************
//
// The following are values that can be passed to the clockDivider parameter
// for functions: SD24_B_init(); the param parameter for functions:
// SD24_B_init().
//
//*****************************************************************************
#define SD24_B_CLOCKDIVIDER_1                                            (0x00)
#define SD24_B_CLOCKDIVIDER_2                                        (SD24DIV0)
#define SD24_B_CLOCKDIVIDER_3                                        (SD24DIV1)
#define SD24_B_CLOCKDIVIDER_4                             (SD24DIV1 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_5                                        (SD24DIV2)
#define SD24_B_CLOCKDIVIDER_6                             (SD24DIV2 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_7                             (SD24DIV2 | SD24DIV1)
#define SD24_B_CLOCKDIVIDER_8                  (SD24DIV2 | SD24DIV1 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_9                                        (SD24DIV3)
#define SD24_B_CLOCKDIVIDER_10                            (SD24DIV3 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_11                            (SD24DIV3 | SD24DIV1)
#define SD24_B_CLOCKDIVIDER_12                 (SD24DIV3 | SD24DIV1 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_13                            (SD24DIV3 | SD24DIV2)
#define SD24_B_CLOCKDIVIDER_14                 (SD24DIV3 | SD24DIV2 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_15                 (SD24DIV3 | SD24DIV2 | SD24DIV1)
#define SD24_B_CLOCKDIVIDER_16      (SD24DIV3 | SD24DIV2 | SD24DIV1 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_17                                       (SD24DIV4)
#define SD24_B_CLOCKDIVIDER_18                            (SD24DIV4 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_19                            (SD24DIV4 | SD24DIV1)
#define SD24_B_CLOCKDIVIDER_20                 (SD24DIV4 | SD24DIV1 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_21                            (SD24DIV4 | SD24DIV2)
#define SD24_B_CLOCKDIVIDER_22                 (SD24DIV4 | SD24DIV2 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_23                 (SD24DIV4 | SD24DIV2 | SD24DIV1)
#define SD24_B_CLOCKDIVIDER_24      (SD24DIV4 | SD24DIV2 | SD24DIV1 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_25                            (SD24DIV4 | SD24DIV3)
#define SD24_B_CLOCKDIVIDER_26                 (SD24DIV4 | SD24DIV3 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_27                 (SD24DIV4 | SD24DIV3 | SD24DIV1)
#define SD24_B_CLOCKDIVIDER_28      (SD24DIV4 | SD24DIV3 | SD24DIV1 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_29                 (SD24DIV4 | SD24DIV3 | SD24DIV2)
#define SD24_B_CLOCKDIVIDER_30      (SD24DIV4 | SD24DIV3 | SD24DIV2 | SD24DIV0)
#define SD24_B_CLOCKDIVIDER_31      (SD24DIV4 | SD24DIV3 | SD24DIV2 | SD24DIV1)
#define SD24_B_CLOCKDIVIDER_32                                                \
                         (SD24DIV4 | SD24DIV3 | SD24DIV2 | SD24DIV1 | SD24DIV0)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: SD24_B_initConverter(), and SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_CONTINUOUS_MODE                                           (0x00)
#define SD24_B_SINGLE_MODE                                         (SD24SNGL_H)

//*****************************************************************************
//
// The following are values that can be passed to the converter parameter for
// functions: SD24_B_setConverterDataFormat(),
// SD24_B_startConverterConversion(), SD24_B_stopConverterConversion(),
// SD24_B_setInterruptDelay(), SD24_B_setConversionDelay(),
// SD24_B_setOversampling(), SD24_B_setGain(), SD24_B_getResults(),
// SD24_B_getHighWordResults(), SD24_B_enableInterrupt(),
// SD24_B_disableInterrupt(), SD24_B_clearInterrupt(), and
// SD24_B_getInterruptStatus(); the param parameter for functions:
// SD24_B_initConverter(), and SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_CONVERTER_0                                                    0
#define SD24_B_CONVERTER_1                                                    1
#define SD24_B_CONVERTER_2                                                    2
#define SD24_B_CONVERTER_3                                                    3
#define SD24_B_CONVERTER_4                                                    4
#define SD24_B_CONVERTER_5                                                    5
#define SD24_B_CONVERTER_6                                                    6
#define SD24_B_CONVERTER_7                                                    7

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: SD24_B_initConverter(), and SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_ALIGN_RIGHT                                               (0x00)
#define SD24_B_ALIGN_LEFT                                            (SD24ALGN)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: SD24_B_initConverter(), and SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_CONVERSION_SELECT_SD24SC                       (SD24SCS__SD24SC)
#define SD24_B_CONVERSION_SELECT_EXT1                           (SD24SCS__EXT1)
#define SD24_B_CONVERSION_SELECT_EXT2                           (SD24SCS__EXT2)
#define SD24_B_CONVERSION_SELECT_EXT3                           (SD24SCS__EXT3)
#define SD24_B_CONVERSION_SELECT_GROUP0                       (SD24SCS__GROUP0)
#define SD24_B_CONVERSION_SELECT_GROUP1                       (SD24SCS__GROUP1)
#define SD24_B_CONVERSION_SELECT_GROUP2                       (SD24SCS__GROUP2)
#define SD24_B_CONVERSION_SELECT_GROUP3                       (SD24SCS__GROUP3)

//*****************************************************************************
//
// The following are values that can be passed to the oversampleRatio parameter
// for functions: SD24_B_setOversampling(); the param parameter for functions:
// SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_OVERSAMPLE_32                                          (OSR__32)
#define SD24_B_OVERSAMPLE_64                                          (OSR__64)
#define SD24_B_OVERSAMPLE_128                                        (OSR__128)
#define SD24_B_OVERSAMPLE_256                                        (OSR__256)
#define SD24_B_OVERSAMPLE_512                                        (OSR__512)
#define SD24_B_OVERSAMPLE_1024                                      (OSR__1024)

//*****************************************************************************
//
// The following are values that can be passed to the dataFormat parameter for
// functions: SD24_B_setConverterDataFormat(); the param parameter for
// functions: SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_DATA_FORMAT_BINARY                                    (SD24DF_0)
#define SD24_B_DATA_FORMAT_2COMPLEMENT                               (SD24DF_1)

//*****************************************************************************
//
// The following are values that can be passed to the gain parameter for
// functions: SD24_B_setGain(); the param parameter for functions:
// SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_GAIN_1                                              (SD24GAIN_1)
#define SD24_B_GAIN_2                                              (SD24GAIN_2)
#define SD24_B_GAIN_4                                              (SD24GAIN_4)
#define SD24_B_GAIN_8                                              (SD24GAIN_8)
#define SD24_B_GAIN_16                                            (SD24GAIN_16)
#define SD24_B_GAIN_32                                            (SD24GAIN_32)
#define SD24_B_GAIN_64                                            (SD24GAIN_64)
#define SD24_B_GAIN_128                                          (SD24GAIN_128)

//*****************************************************************************
//
// The following are values that can be passed to the sampleDelay parameter for
// functions: SD24_B_setInterruptDelay(); the param parameter for functions:
// SD24_B_initConverterAdvanced().
//
//*****************************************************************************
#define SD24_B_FOURTH_SAMPLE_INTERRUPT                           (SD24INTDLY_0)
#define SD24_B_THIRD_SAMPLE_INTERRUPT                            (SD24INTDLY_1)
#define SD24_B_SECOND_SAMPLE_INTERRUPT                           (SD24INTDLY_2)
#define SD24_B_FIRST_SAMPLE_INTERRUPT                            (SD24INTDLY_3)

//*****************************************************************************
//
// The following are values that can be passed to the group parameter for
// functions: SD24_B_startGroupConversion(), and SD24_B_stopGroupConversion().
//
//*****************************************************************************
#define SD24_B_GROUP0                                                         0
#define SD24_B_GROUP1                                                         1
#define SD24_B_GROUP2                                                         2
#define SD24_B_GROUP3                                                         3

//*****************************************************************************
//
// The following are values that can be passed to the interruptFlag parameter
// for functions: SD24_B_configureDMATrigger().
//
//*****************************************************************************
#define SD24_B_DMA_TRIGGER_IFG0                                     (SD24DMA_0)
#define SD24_B_DMA_TRIGGER_IFG1                                     (SD24DMA_1)
#define SD24_B_DMA_TRIGGER_IFG2                                     (SD24DMA_2)
#define SD24_B_DMA_TRIGGER_IFG3                                     (SD24DMA_3)
#define SD24_B_DMA_TRIGGER_IFG4                                     (SD24DMA_4)
#define SD24_B_DMA_TRIGGER_IFG5                                     (SD24DMA_5)
#define SD24_B_DMA_TRIGGER_IFG6                                     (SD24DMA_6)
#define SD24_B_DMA_TRIGGER_IFG7                                     (SD24DMA_7)
#define SD24_B_DMA_TRIGGER_TRGIFG                                   (SD24DMA_8)

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: SD24_B_enableInterrupt(), SD24_B_disableInterrupt(),
// SD24_B_clearInterrupt(), and SD24_B_getInterruptStatus() as well as returned
// by the SD24_B_getInterruptStatus() function.
//
//*****************************************************************************
#define SD24_B_CONVERTER_INTERRUPT                                      SD24IE0
#define SD24_B_CONVERTER_OVERFLOW_INTERRUPT                           SD24OVIE0

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the SD24_B Module
//!
//! This function initializes the SD24_B module sigma-delta analog-to-digital
//! conversions. Specifically the function sets up the clock source for the
//! SD24_B core to use for conversions. Upon completion of the initialization
//! the SD24_B interrupt registers will be reset and the given parameters will
//! be set. The converter configuration settings are independent of this
//! function. The values you choose for the clock divider and predivider are
//! used to determine the effective clock frequency. The formula used is:
//! f_sd24 = f_clk /(divider * predivider)
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param param is the pointer to struct for initialization.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_init(uint16_t baseAddress,
                        SD24_B_initParam *param);

//*****************************************************************************
//
//! \brief Configure SD24_B converter
//!
//! This function initializes a converter of the SD24_B module. Upon completion
//! the converter will be ready for a conversion and can be started with the
//! SD24_B_startGroupConversion() or SD24_B_startConverterConversion()
//! depending on the startSelect parameter. Additional configuration such as
//! data format can be configured in SD24_B_setConverterDataFormat().
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param param is the pointer to struct for converter configuration.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_initConverter(uint16_t baseAddress,
                                 SD24_B_initConverterParam *param);

//*****************************************************************************
//
//! \brief Configure SD24_B converter - Advanced Configure
//!
//! This function initializes a converter of the SD24_B module. Upon completion
//! the converter will be ready for a conversion and can be started with the
//! SD24_B_startGroupConversion() or SD24_B_startConverterConversion()
//! depending on the startSelect parameter.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param param is the pointer to struct for converter advanced configuration.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_initConverterAdvanced(uint16_t baseAddress,
                                         SD24_B_initConverterAdvancedParam *param);

//*****************************************************************************
//
//! \brief Set SD24_B converter data format
//!
//! This function sets the converter format so that the resulting data can be
//! viewed in either binary or 2's complement.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter that will be configured. Check
//!        datasheet for available converters on device.
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param dataFormat selects how the data format of the results
//!        Valid values are:
//!        - \b SD24_B_DATA_FORMAT_BINARY [Default]
//!        - \b SD24_B_DATA_FORMAT_2COMPLEMENT
//!        \n Modified bits are \b SD24DFx of \b SD24BCCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_setConverterDataFormat(uint16_t baseAddress,
                                          uint8_t converter,
                                          uint8_t dataFormat);

//*****************************************************************************
//
//! \brief Start Conversion Group
//!
//! This function starts all the converters that are associated with a group.
//! To set a converter to a group use the SD24_B_configureConverter() function.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param group selects the group that will be started
//!        Valid values are:
//!        - \b SD24_B_GROUP0
//!        - \b SD24_B_GROUP1
//!        - \b SD24_B_GROUP2
//!        - \b SD24_B_GROUP3
//!        \n Modified bits are \b SD24DGRPxSC of \b SD24BCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_startGroupConversion(uint16_t baseAddress,
                                        uint8_t group);

//*****************************************************************************
//
//! \brief Stop Conversion Group
//!
//! This function stops all the converters that are associated with a group. To
//! set a converter to a group use the SD24_B_configureConverter() function.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param group selects the group that will be stopped
//!        Valid values are:
//!        - \b SD24_B_GROUP0
//!        - \b SD24_B_GROUP1
//!        - \b SD24_B_GROUP2
//!        - \b SD24_B_GROUP3
//!        \n Modified bits are \b SD24DGRPxSC of \b SD24BCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_stopGroupConversion(uint16_t baseAddress,
                                       uint8_t group);

//*****************************************************************************
//
//! \brief Start Conversion for Converter
//!
//! This function starts a single converter.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter that will be started
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//!        \n Modified bits are \b SD24SC of \b SD24BCCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_startConverterConversion(uint16_t baseAddress,
                                            uint8_t converter);

//*****************************************************************************
//
//! \brief Stop Conversion for Converter
//!
//! This function stops a single converter.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter that will be stopped
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//!        \n Modified bits are \b SD24SC of \b SD24BCCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_stopConverterConversion(uint16_t baseAddress,
                                           uint8_t converter);

//*****************************************************************************
//
//! \brief Configures the converter that triggers a DMA transfer
//!
//! This function chooses which interrupt will trigger a DMA transfer.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param interruptFlag selects the converter interrupt that triggers a DMA
//!        transfer.
//!        Valid values are:
//!        - \b SD24_B_DMA_TRIGGER_IFG0
//!        - \b SD24_B_DMA_TRIGGER_IFG1
//!        - \b SD24_B_DMA_TRIGGER_IFG2
//!        - \b SD24_B_DMA_TRIGGER_IFG3
//!        - \b SD24_B_DMA_TRIGGER_IFG4
//!        - \b SD24_B_DMA_TRIGGER_IFG5
//!        - \b SD24_B_DMA_TRIGGER_IFG6
//!        - \b SD24_B_DMA_TRIGGER_IFG7
//!        - \b SD24_B_DMA_TRIGGER_TRGIFG
//!        \n Modified bits are \b SD24DMAx of \b SD24BCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_configureDMATrigger(uint16_t baseAddress,
                                       uint16_t interruptFlag);

//*****************************************************************************
//
//! \brief Configures the delay for an interrupt to trigger
//!
//! This function configures the delay for the first interrupt service request
//! for the corresponding converter. This feature delays the interrupt request
//! for a completed conversion by up to four conversion cycles allowing the
//! digital filter to settle prior to generating an interrupt request.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter that will be stopped
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param sampleDelay selects the delay for the interrupt
//!        Valid values are:
//!        - \b SD24_B_FOURTH_SAMPLE_INTERRUPT [Default]
//!        - \b SD24_B_THIRD_SAMPLE_INTERRUPT
//!        - \b SD24_B_SECOND_SAMPLE_INTERRUPT
//!        - \b SD24_B_FIRST_SAMPLE_INTERRUPT
//!        \n Modified bits are \b SD24INTDLYx of \b SD24INCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_setInterruptDelay(uint16_t baseAddress,
                                     uint8_t converter,
                                     uint8_t sampleDelay);

//*****************************************************************************
//
//! \brief Configures the delay for the conversion start
//!
//! This function configures the delay for the specified converter start.
//! Please note the delay should be written before conversion or after
//! corresponding conversion is completed. If no delay at start of conversion
//! is desired, a previously written non-zero value must be changed to zero
//! before starting the conversion.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter that will be delayed
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param cycleDelay is the clock cycles to delay ranging from 0 to 1023.
//!        \n Modified bits are \b SD24PREx of \b SD24BPREx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_setConversionDelay(uint16_t baseAddress,
                                      uint8_t converter,
                                      uint16_t cycleDelay);

//*****************************************************************************
//
//! \brief Configures the oversampling ratio for a converter
//!
//! This function configures the oversampling ratio for a given converter.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param oversampleRatio selects oversampling ratio for the converter
//!        Valid values are:
//!        - \b SD24_B_OVERSAMPLE_32
//!        - \b SD24_B_OVERSAMPLE_64
//!        - \b SD24_B_OVERSAMPLE_128
//!        - \b SD24_B_OVERSAMPLE_256
//!        - \b SD24_B_OVERSAMPLE_512
//!        - \b SD24_B_OVERSAMPLE_1024
//!        \n Modified bits are \b SD24OSRx of \b SD24BOSRx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_setOversampling(uint16_t baseAddress,
                                   uint8_t converter,
                                   uint16_t oversampleRatio);

//*****************************************************************************
//
//! \brief Configures the gain for the converter
//!
//! This function configures the gain for a single converter.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter that will be configured
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param gain selects the gain for the converter
//!        Valid values are:
//!        - \b SD24_B_GAIN_1 [Default]
//!        - \b SD24_B_GAIN_2
//!        - \b SD24_B_GAIN_4
//!        - \b SD24_B_GAIN_8
//!        - \b SD24_B_GAIN_16
//!        - \b SD24_B_GAIN_32
//!        - \b SD24_B_GAIN_64
//!        - \b SD24_B_GAIN_128
//!        \n Modified bits are \b SD24GAINx of \b SD24BINCTLx register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_setGain(uint16_t baseAddress,
                           uint8_t converter,
                           uint8_t gain);

//*****************************************************************************
//
//! \brief Returns the results for a converter
//!
//! This function gets the results from the SD24BMEMLx and SD24MEMHx registers
//! and concatenates them to form a long. The actual result is a maximum 24
//! bits.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter who's results will be returned
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//!
//! \return Result of conversion
//
//*****************************************************************************
extern uint32_t SD24_B_getResults(uint16_t baseAddress,
                                  uint8_t converter);

//*****************************************************************************
//
//! \brief Returns the high word results for a converter
//!
//! This function gets the results from the SD24MEMHx register and returns it.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter selects the converter who's results will be returned
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//!
//! \return Result of conversion
//
//*****************************************************************************
extern uint16_t SD24_B_getHighWordResults(uint16_t baseAddress,
                                          uint8_t converter);

//*****************************************************************************
//
//! \brief Enables interrupts for the SD24_B Module
//!
//! This function enables interrupts for the SD24_B module. Does not clear
//! interrupt flags.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param mask is the bit mask of the converter interrupt sources to be
//!        enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_B_CONVERTER_INTERRUPT
//!        - \b SD24_B_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b SD24OVIEx of \b SD24BIE register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_enableInterrupt(uint16_t baseAddress,
                                   uint8_t converter,
                                   uint16_t mask);

//*****************************************************************************
//
//! \brief Disables interrupts for the SD24_B Module
//!
//! This function disables interrupts for the SD24_B module.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param mask is the bit mask of the converter interrupt sources to be
//!        disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_B_CONVERTER_INTERRUPT
//!        - \b SD24_B_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b SD24OVIEx of \b SD24BIE register.
//!
//! Modified bits of \b SD24BIE register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_disableInterrupt(uint16_t baseAddress,
                                    uint8_t converter,
                                    uint16_t mask);

//*****************************************************************************
//
//! \brief Clears interrupts for the SD24_B Module
//!
//! This function clears interrupt flags for the SD24_B module.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param mask is the bit mask of the converter interrupt sources to clear.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_B_CONVERTER_INTERRUPT
//!        - \b SD24_B_CONVERTER_OVERFLOW_INTERRUPT
//!        \n Modified bits are \b SD24OVIFGx of \b SD24BIFG register.
//!
//! \return None
//
//*****************************************************************************
extern void SD24_B_clearInterrupt(uint16_t baseAddress,
                                  uint8_t converter,
                                  uint16_t mask);

//*****************************************************************************
//
//! \brief Returns the interrupt status for the SD24_B Module
//!
//! This function returns interrupt flag statuses for the SD24_B module.
//!
//! \param baseAddress is the base address of the SD24_B module.
//! \param converter is the selected converter.
//!        Valid values are:
//!        - \b SD24_B_CONVERTER_0
//!        - \b SD24_B_CONVERTER_1
//!        - \b SD24_B_CONVERTER_2
//!        - \b SD24_B_CONVERTER_3
//!        - \b SD24_B_CONVERTER_4
//!        - \b SD24_B_CONVERTER_5
//!        - \b SD24_B_CONVERTER_6
//!        - \b SD24_B_CONVERTER_7
//! \param mask is the bit mask of the converter interrupt sources to return.
//!        Mask value is the logical OR of any of the following:
//!        - \b SD24_B_CONVERTER_INTERRUPT
//!        - \b SD24_B_CONVERTER_OVERFLOW_INTERRUPT
//!
//! \return Logical OR of any of the following:
//!         - \b SD24_B_CONVERTER_INTERRUPT
//!         - \b SD24_B_CONVERTER_OVERFLOW_INTERRUPT
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint16_t SD24_B_getInterruptStatus(uint16_t baseAddress,
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
#endif // __MSP430WARE_SD24_B_H__
