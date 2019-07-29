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
// dac12_a.h - Driver for the DAC12_A Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_DAC12_A_H__
#define __MSP430WARE_DAC12_A_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_DAC12_2__

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
//! \brief Used in the DAC12_A_init() function as the param parameter.
//
//*****************************************************************************
typedef struct DAC12_A_initParam {
    //! Decides which DAC12_A sub-module to configure.
    //! \n Valid values are:
    //! - \b DAC12_A_SUBMODULE_0
    //! - \b DAC12_A_SUBMODULE_1
    uint8_t submoduleSelect;
    //! Selects the output pin that the selected DAC12_A module will output to.
    //! \n Valid values are:
    //! - \b DAC12_A_OUTPUT_1 [Default]
    //! - \b DAC12_A_OUTPUT_2
    uint16_t outputSelect;
    //! Is the upper limit voltage that the data can be converted in to.
    //! \n Valid values are:
    //! - \b DAC12_A_VREF_INT [Default]
    //! - \b DAC12_A_VREF_AVCC
    //! - \b DAC12_A_VREF_EXT - For devices with CTSD16, use Ref module
    //!    Ref_enableReferenceVoltageOutput/Ref__disableReferenceVoltageOutput
    //!    to select VeREF(external reference signal) or VREFBG(internally
    //!    generated reference signal)
    uint16_t positiveReferenceVoltage;
    //! Is the multiplier of the Vout voltage.
    //! \n Valid values are:
    //! - \b DAC12_A_VREFx1 [Default]
    //! - \b DAC12_A_VREFx2
    //! - \b DAC12_A_VREFx3
    uint16_t outputVoltageMultiplier;
    //! Is the setting of the settling speed and current of the Vref+ and the
    //! Vout buffer.
    //! \n Valid values are:
    //! - \b DAC12_A_AMP_OFF_PINOUTHIGHZ [Default] - Initialize the DAC12_A
    //!    Module with settings, but do not turn it on.
    //! - \b DAC12_A_AMP_OFF_PINOUTLOW - Initialize the DAC12_A Module with
    //!    settings, and allow it to take control of the selected output pin to
    //!    pull it low (Note: this takes control away port mapping module).
    //! - \b DAC12_A_AMP_LOWIN_LOWOUT - Select a slow settling speed and
    //!    current for Vref+ input buffer and for Vout output buffer.
    //! - \b DAC12_A_AMP_LOWIN_MEDOUT - Select a slow settling speed and
    //!    current for Vref+ input buffer and a medium settling speed and
    //!    current for Vout output buffer.
    //! - \b DAC12_A_AMP_LOWIN_HIGHOUT - Select a slow settling speed and
    //!    current for Vref+ input buffer and a high settling speed and current
    //!    for Vout output buffer.
    //! - \b DAC12_A_AMP_MEDIN_MEDOUT - Select a medium settling speed and
    //!    current for Vref+ input buffer and for Vout output buffer.
    //! - \b DAC12_A_AMP_MEDIN_HIGHOUT - Select a medium settling speed and
    //!    current for Vref+ input buffer and a high settling speed and current
    //!    for Vout output buffer.
    //! - \b DAC12_A_AMP_HIGHIN_HIGHOUT - Select a high settling speed and
    //!    current for Vref+ input buffer and for Vout output buffer.
    uint8_t amplifierSetting;
    //! Selects the trigger that will start a conversion.
    //! \n Valid values are:
    //! - \b DAC12_A_TRIGGER_ENCBYPASS [Default] - Automatically converts data
    //!    as soon as it is written into the data buffer. (Note: Do not use
    //!    this selection if grouping DAC's).
    //! - \b DAC12_A_TRIGGER_ENC - Requires a call to enableConversions() to
    //!    allow a conversion, but starts a conversion as soon as data is
    //!    written to the data buffer (Note: with DAC12_A module's grouped,
    //!    data has to be set in BOTH DAC12_A data buffers to start a
    //!    conversion).
    //! - \b DAC12_A_TRIGGER_TA - Requires a call to enableConversions() to
    //!    allow a conversion, and a rising edge of Timer_A's Out1 (TA1) to
    //!    start a conversion.
    //! - \b DAC12_A_TRIGGER_TB - Requires a call to enableConversions() to
    //!    allow a conversion, and a rising edge of Timer_B's Out2 (TB2) to
    //!    start a conversion.
    uint16_t conversionTriggerSelect;
} DAC12_A_initParam;


//*****************************************************************************
//
// The following are values that can be passed to the positiveReferenceVoltage
// parameter for functions: DAC12_A_init(); the param parameter for functions:
// DAC12_A_init().
//
//*****************************************************************************
#define DAC12_A_VREF_INT                                          (DAC12SREF_0)
#define DAC12_A_VREF_AVCC                                         (DAC12SREF_1)
#define DAC12_A_VREF_EXT                                          (DAC12SREF_2)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: DAC12_A_init(); the amplifierSetting parameter for functions:
// DAC12_A_init(), and DAC12_A_setAmplifierSetting().
//
//*****************************************************************************
#define DAC12_A_AMP_OFF_PINOUTHIGHZ                                (DAC12AMP_0)
#define DAC12_A_AMP_OFF_PINOUTLOW                                  (DAC12AMP_1)
#define DAC12_A_AMP_LOWIN_LOWOUT                                   (DAC12AMP_2)
#define DAC12_A_AMP_LOWIN_MEDOUT                                   (DAC12AMP_3)
#define DAC12_A_AMP_LOWIN_HIGHOUT                                  (DAC12AMP_4)
#define DAC12_A_AMP_MEDIN_MEDOUT                                   (DAC12AMP_5)
#define DAC12_A_AMP_MEDIN_HIGHOUT                                  (DAC12AMP_6)
#define DAC12_A_AMP_HIGHIN_HIGHOUT                                 (DAC12AMP_7)

//*****************************************************************************
//
// The following are values that can be passed to the outputSelect parameter
// for functions: DAC12_A_init(); the param parameter for functions:
// DAC12_A_init().
//
//*****************************************************************************
#define DAC12_A_OUTPUT_1                                          (!(DAC12OPS))
#define DAC12_A_OUTPUT_2                                             (DAC12OPS)

//*****************************************************************************
//
// The following are values that can be passed to the submoduleSelect parameter
// for functions: DAC12_A_init(), DAC12_A_setAmplifierSetting(),
// DAC12_A_disable(), DAC12_A_enableInterrupt(), DAC12_A_disableInterrupt(),
// DAC12_A_getInterruptStatus(), DAC12_A_clearInterrupt(),
// DAC12_A_calibrateOutput(), DAC12_A_getCalibrationData(),
// DAC12_A_setCalibrationOffset(), DAC12_A_enableConversions(),
// DAC12_A_setData(), DAC12_A_disableConversions(), DAC12_A_setResolution(),
// DAC12_A_setInputDataFormat(), and
// DAC12_A_getDataBufferMemoryAddressForDMA(); the param parameter for
// functions: DAC12_A_init().
//
//*****************************************************************************
#define DAC12_A_SUBMODULE_0                                              (0x00)
#define DAC12_A_SUBMODULE_1                                              (0x10)

//*****************************************************************************
//
// The following are values that can be passed to the outputVoltageMultiplier
// parameter for functions: DAC12_A_init(); the param parameter for functions:
// DAC12_A_init().
//
//*****************************************************************************
#define DAC12_A_VREFx1                                                (DAC12IR)
#define DAC12_A_VREFx2                                                (DAC12OG)
#define DAC12_A_VREFx3                                                    (0x0)

//*****************************************************************************
//
// The following are values that can be passed to the conversionTriggerSelect
// parameter for functions: DAC12_A_init(); the param parameter for functions:
// DAC12_A_init().
//
//*****************************************************************************
#define DAC12_A_TRIGGER_ENCBYPASS                                 (DAC12LSEL_0)
#define DAC12_A_TRIGGER_ENC                                       (DAC12LSEL_1)
#define DAC12_A_TRIGGER_TA                                        (DAC12LSEL_2)
#define DAC12_A_TRIGGER_TB                                        (DAC12LSEL_3)

//*****************************************************************************
//
// The following are values that can be passed to the resolutionSelect
// parameter for functions: DAC12_A_setResolution().
//
//*****************************************************************************
#define DAC12_A_RESOLUTION_8BIT                                      (DAC12RES)
#define DAC12_A_RESOLUTION_12BIT                                  (!(DAC12RES))

//*****************************************************************************
//
// The following are values that can be passed to the inputJustification
// parameter for functions: DAC12_A_setInputDataFormat().
//
//*****************************************************************************
#define DAC12_A_JUSTIFICATION_RIGHT                               (!(DAC12DFJ))
#define DAC12_A_JUSTIFICATION_LEFT                                   (DAC12DFJ)

//*****************************************************************************
//
// The following are values that can be passed to the inputSign parameter for
// functions: DAC12_A_setInputDataFormat().
//
//*****************************************************************************
#define DAC12_A_UNSIGNED_BINARY                                    (!(DAC12DF))
#define DAC12_A_SIGNED_2SCOMPLEMENT                                   (DAC12DF)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the DAC12_A_getInterruptStatus() function.
//
//*****************************************************************************
#define DAC12_A_INT_ACTIVE                                           (DAC12IFG)
#define DAC12_A_INT_INACTIVE                                             (0x00)

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the DAC12_A module with the specified settings.
//!
//! This function initializes the DAC12_A module with the specified settings.
//! Upon successful completion of the initialization of this module the control
//! registers and interrupts of this module are all reset, and the specified
//! variables will be set. Please note, that if conversions are enabled with
//! the enableConversions() function, then disableConversions() must be called
//! before re-initializing the DAC12_A module with this function.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param param is the pointer to struct for initialization.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process.
//
//*****************************************************************************
extern bool DAC12_A_init(uint16_t baseAddress,
                         DAC12_A_initParam *param);

//*****************************************************************************
//
//! \brief Sets the amplifier settings for the Vref+ and Vout buffers.
//!
//! This function sets the amplifier settings of the DAC12_A module for the
//! Vref+ and Vout buffers without re-initializing the DAC12_A module. This can
//! be used to disable the control of the pin by the DAC12_A module.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//! \param amplifierSetting is the setting of the settling speed and current of
//!        the Vref+ and the Vout buffer.
//!        Valid values are:
//!        - \b DAC12_A_AMP_OFF_PINOUTHIGHZ [Default] - Initialize the DAC12_A
//!           Module with settings, but do not turn it on.
//!        - \b DAC12_A_AMP_OFF_PINOUTLOW - Initialize the DAC12_A Module with
//!           settings, and allow it to take control of the selected output pin
//!           to pull it low (Note: this takes control away port mapping
//!           module).
//!        - \b DAC12_A_AMP_LOWIN_LOWOUT - Select a slow settling speed and
//!           current for Vref+ input buffer and for Vout output buffer.
//!        - \b DAC12_A_AMP_LOWIN_MEDOUT - Select a slow settling speed and
//!           current for Vref+ input buffer and a medium settling speed and
//!           current for Vout output buffer.
//!        - \b DAC12_A_AMP_LOWIN_HIGHOUT - Select a slow settling speed and
//!           current for Vref+ input buffer and a high settling speed and
//!           current for Vout output buffer.
//!        - \b DAC12_A_AMP_MEDIN_MEDOUT - Select a medium settling speed and
//!           current for Vref+ input buffer and for Vout output buffer.
//!        - \b DAC12_A_AMP_MEDIN_HIGHOUT - Select a medium settling speed and
//!           current for Vref+ input buffer and a high settling speed and
//!           current for Vout output buffer.
//!        - \b DAC12_A_AMP_HIGHIN_HIGHOUT - Select a high settling speed and
//!           current for Vref+ input buffer and for Vout output buffer.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_setAmplifierSetting(uint16_t baseAddress,
                                        uint8_t submoduleSelect,
                                        uint8_t amplifierSetting);

//*****************************************************************************
//
//! \brief Clears the amplifier settings to disable the DAC12_A module.
//!
//! This function clears the amplifier settings for the selected DAC12_A module
//! to disable the DAC12_A module.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! Modified bits are \b DAC12AMP_7 of \b DAC12_xCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_disable(uint16_t baseAddress,
                            uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Enables grouping of two DAC12_A modules in a dual DAC12_A system.
//!
//! This function enables grouping two DAC12_A modules in a dual DAC12_A
//! system. Both DAC12_A modules will work in sync, converting data at the same
//! time. To convert data, the same trigger should be set for both DAC12_A
//! modules during initialization (which should not be
//! DAC12_A_TRIGGER_ENCBYPASS), the enableConversions() function needs to be
//! called with both DAC12_A modules, and data needs to be set for both DAC12_A
//! modules separately.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//!
//! Modified bits are \b DAC12GRP of \b DAC12_xCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_enableGrouping(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables grouping of two DAC12_A modules in a dual DAC12_A system.
//!
//! This function disables grouping of two DAC12_A modules in a dual DAC12_A
//! system.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_disableGrouping(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables the DAC12_A module interrupt source.
//!
//! This function to enable the DAC12_A module interrupt, which throws an
//! interrupt when the data buffer is available for new data to be set. Only
//! the sources that are enabled can be reflected to the processor interrupt;
//! disabled sources have no effect on the processor. Note that an interrupt is
//! not thrown when DAC12_A_TRIGGER_ENCBYPASS has been set for the parameter
//! conversionTriggerSelect in initialization. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_enableInterrupt(uint16_t baseAddress,
                                    uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Disables the DAC12_A module interrupt source.
//!
//! Enables the DAC12_A module interrupt source. Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_disableInterrupt(uint16_t baseAddress,
                                     uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Returns the status of the DAC12_A module interrupt flag.
//!
//! This function returns the status of the DAC12_A module interrupt flag. Note
//! that an interrupt is not thrown when DAC12_A_TRIGGER_ENCBYPASS has been set
//! for the conversionTriggerSelect parameter in initialization.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! \return One of the following:
//!         - \b DAC12_A_INT_ACTIVE
//!         - \b DAC12_A_INT_INACTIVE
//!         \n indicating the status for the selected DAC12_A module
//
//*****************************************************************************
extern uint16_t DAC12_A_getInterruptStatus(uint16_t baseAddress,
                                           uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Clears the DAC12_A module interrupt flag.
//!
//! The DAC12_A module interrupt flag is cleared, so that it no longer asserts.
//! Note that an interrupt is not thrown when DAC12_A_TRIGGER_ENCBYPASS has
//! been set for the parameter conversionTriggerSelect in initialization.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! Modified bits are \b DAC12IFG of \b DAC12_xCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_clearInterrupt(uint16_t baseAddress,
                                   uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Calibrates the output offset.
//!
//! This function disables the calibration lock, starts the calibration, whats
//! for the calibration to complete, and then re-locks the calibration lock.
//! Please note, this function should be called after initializing the dac12
//! module, and before using it.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! Modified bits are \b DAC12CALON of \b DAC12_xCTL0 register; bits \b DAC12PW
//! of \b DAC12_xCALCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_calibrateOutput(uint16_t baseAddress,
                                    uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Returns the calibrated offset of the output buffer.
//!
//! This function returns the calibrated offset of the output buffer. The
//! output buffer offset is used to obtain accurate results from the output
//! pin. This function should only be used while the calibration lock is
//! enabled. Only the lower byte of the word of the register is returned, and
//! the value is between -128 and +127.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! \return The calibrated offset of the output buffer.
//
//*****************************************************************************
extern uint16_t DAC12_A_getCalibrationData(uint16_t baseAddress,
                                           uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Returns the calibrated offset of the output buffer.
//!
//! This function is used to manually set the calibration offset value. The
//! calibration is automatically unlocked and re-locked to be able to allow for
//! the offset value to be set.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//! \param calibrationOffsetValue calibration offset value
//!
//! Modified bits are \b DAC12LOCK of \b DAC12_xCALDAT register; bits \b
//! DAC12PW of \b DAC12_xCTL0 register; bits \b DAC12PW of \b DAC12_xCALCTL
//! register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_setCalibrationOffset(uint16_t baseAddress,
                                         uint8_t submoduleSelect,
                                         uint16_t calibrationOffsetValue);

//*****************************************************************************
//
//! \brief Enables triggers to start conversions.
//!
//! This function is used to allow triggers to start a conversion. Note that
//! this function does not need to be used if DAC12_A_TRIGGER_ENCBYPASS was set
//! for the conversionTriggerSelect parameter during initialization. If DAC
//! grouping is enabled, this has to be called for both DAC's.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! Modified bits are \b DAC12ENC of \b DAC12_xCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_enableConversions(uint16_t baseAddress,
                                      uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Sets the given data into the buffer to be converted.
//!
//! This function is used to set the given data into the data buffer of the
//! DAC12_A module. The data given should be in the format set (12-bit
//! Unsigned, Right-justified by default). Note if DAC12_A_TRIGGER_ENCBYPASS
//! was set for the conversionTriggerSelect during initialization then using
//! this function will set the data and automatically trigger a conversion. If
//! any other trigger was set during initialization, then the
//! DAC12_A_enableConversions() function needs to be called before a conversion
//! can be started. If grouping DAC's and DAC12_A_TRIGGER_ENC was set during
//! initialization, then both data buffers must be set before a conversion will
//! be started.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//! \param data is the data to be set into the DAC12_A data buffer to be
//!        converted.
//!        \n Modified bits are \b DAC12_DATA of \b DAC12_xDAT register.
//!
//! Modified bits of \b DAC12_xDAT register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_setData(uint16_t baseAddress,
                            uint8_t submoduleSelect,
                            uint16_t data);

//*****************************************************************************
//
//! \brief Disables triggers to start conversions.
//!
//! This function is used to disallow triggers to start a conversion. Note that
//! this function does not have any affect if DAC12_A_TRIGGER_ENCBYPASS was set
//! for the conversionTriggerSelect parameter during initialization.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! Modified bits are \b DAC12ENC of \b DAC12_xCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_disableConversions(uint16_t baseAddress,
                                       uint8_t submoduleSelect);

//*****************************************************************************
//
//! \brief Sets the resolution to be used by the DAC12_A module.
//!
//! This function sets the resolution of the data to be converted.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//! \param resolutionSelect is the resolution to use for conversions.
//!        Valid values are:
//!        - \b DAC12_A_RESOLUTION_8BIT
//!        - \b DAC12_A_RESOLUTION_12BIT [Default]
//!        \n Modified bits are \b DAC12RES of \b DAC12_xCTL0 register.
//!
//! Modified bits are \b DAC12ENC and \b DAC12RES of \b DAC12_xCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_setResolution(uint16_t baseAddress,
                                  uint8_t submoduleSelect,
                                  uint16_t resolutionSelect);

//*****************************************************************************
//
//! \brief Sets the input data format for the DAC12_A module.
//!
//! This function sets the input format for the binary data to be converted.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//! \param inputJustification is the justification of the data to be converted.
//!        Valid values are:
//!        - \b DAC12_A_JUSTIFICATION_RIGHT [Default]
//!        - \b DAC12_A_JUSTIFICATION_LEFT
//!        \n Modified bits are \b DAC12DFJ of \b DAC12_xCTL1 register.
//! \param inputSign is the sign of the data to be converted.
//!        Valid values are:
//!        - \b DAC12_A_UNSIGNED_BINARY [Default]
//!        - \b DAC12_A_SIGNED_2SCOMPLEMENT
//!        \n Modified bits are \b DAC12DF of \b DAC12_xCTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void DAC12_A_setInputDataFormat(uint16_t baseAddress,
                                       uint8_t submoduleSelect,
                                       uint8_t inputJustification,
                                       uint8_t inputSign);

//*****************************************************************************
//
//! \brief Returns the address of the specified DAC12_A data buffer for the DMA
//! module.
//!
//! Returns the address of the specified memory buffer. This can be used in
//! conjunction with the DMA to obtain the data directly from memory.
//!
//! \param baseAddress is the base address of the DAC12_A module.
//! \param submoduleSelect decides which DAC12_A sub-module to configure.
//!        Valid values are:
//!        - \b DAC12_A_SUBMODULE_0
//!        - \b DAC12_A_SUBMODULE_1
//!
//! \return The address of the specified memory buffer
//
//*****************************************************************************
extern uint32_t DAC12_A_getDataBufferMemoryAddressForDMA(uint16_t baseAddress,
                                                         uint8_t submoduleSelect);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_DAC12_A_H__
