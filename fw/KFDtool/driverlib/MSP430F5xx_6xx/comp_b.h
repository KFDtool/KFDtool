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
// comp_b.h - Driver for the COMP_B Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_COMP_B_H__
#define __MSP430WARE_COMP_B_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_COMPB__

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
//! \brief Used in the Comp_B_init() function as the param parameter.
//
//*****************************************************************************
typedef struct Comp_B_initParam {
    //! Selects the input to the positive terminal.
    //! \n Valid values are:
    //! - \b COMP_B_INPUT0 [Default]
    //! - \b COMP_B_INPUT1
    //! - \b COMP_B_INPUT2
    //! - \b COMP_B_INPUT3
    //! - \b COMP_B_INPUT4
    //! - \b COMP_B_INPUT5
    //! - \b COMP_B_INPUT6
    //! - \b COMP_B_INPUT7
    //! - \b COMP_B_INPUT8
    //! - \b COMP_B_INPUT9
    //! - \b COMP_B_INPUT10
    //! - \b COMP_B_INPUT11
    //! - \b COMP_B_INPUT12
    //! - \b COMP_B_INPUT13
    //! - \b COMP_B_INPUT14
    //! - \b COMP_B_INPUT15
    //! - \b COMP_B_VREF
    uint8_t positiveTerminalInput;
    //! Selects the input to the negative terminal.
    //! \n Valid values are:
    //! - \b COMP_B_INPUT0 [Default]
    //! - \b COMP_B_INPUT1
    //! - \b COMP_B_INPUT2
    //! - \b COMP_B_INPUT3
    //! - \b COMP_B_INPUT4
    //! - \b COMP_B_INPUT5
    //! - \b COMP_B_INPUT6
    //! - \b COMP_B_INPUT7
    //! - \b COMP_B_INPUT8
    //! - \b COMP_B_INPUT9
    //! - \b COMP_B_INPUT10
    //! - \b COMP_B_INPUT11
    //! - \b COMP_B_INPUT12
    //! - \b COMP_B_INPUT13
    //! - \b COMP_B_INPUT14
    //! - \b COMP_B_INPUT15
    //! - \b COMP_B_VREF
    uint8_t negativeTerminalInput;
    //! Selects the power mode at which the Comp_B module will operate at.
    //! \n Valid values are:
    //! - \b COMP_B_POWERMODE_HIGHSPEED [Default]
    //! - \b COMP_B_POWERMODE_NORMALMODE
    //! - \b COMP_B_POWERMODE_ULTRALOWPOWER
    uint16_t powerModeSelect;
    //! Controls the output filter delay state, which is either off or enabled
    //! with a specified delay level. This parameter is device specific and
    //! delay levels should be found in the device's datasheet.
    //! \n Valid values are:
    //! - \b COMP_B_FILTEROUTPUT_OFF [Default]
    //! - \b COMP_B_FILTEROUTPUT_DLYLVL1
    //! - \b COMP_B_FILTEROUTPUT_DLYLVL2
    //! - \b COMP_B_FILTEROUTPUT_DLYLVL3
    //! - \b COMP_B_FILTEROUTPUT_DLYLVL4
    uint8_t outputFilterEnableAndDelayLevel;
    //! Controls if the output will be inverted or not
    //! \n Valid values are:
    //! - \b COMP_B_NORMALOUTPUTPOLARITY [Default]
    //! - \b COMP_B_INVERTEDOUTPUTPOLARITY
    uint16_t invertedOutputPolarity;
} Comp_B_initParam;

//*****************************************************************************
//
//! \brief Used in the Comp_B_configureReferenceVoltage() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct Comp_B_configureReferenceVoltageParam {
    //! Decides the source and max amount of Voltage that can be used as a
    //! reference.
    //! \n Valid values are:
    //! - \b COMP_B_VREFBASE_VCC
    //! - \b COMP_B_VREFBASE1_5V
    //! - \b COMP_B_VREFBASE2_0V
    //! - \b COMP_B_VREFBASE2_5V
    uint16_t supplyVoltageReferenceBase;
    //! Is the numerator of the equation to generate the reference voltage for
    //! the lower limit reference voltage.
    uint16_t lowerLimitSupplyVoltageFractionOf32;
    //! Is the numerator of the equation to generate the reference voltage for
    //! the upper limit reference voltage.
    uint16_t upperLimitSupplyVoltageFractionOf32;
    //!  is the reference accuracy setting of the Comp_B. Clocked is for low
    //! power/low accuracy.
    //! \n Valid values are:
    //! - \b COMP_B_ACCURACY_STATIC
    //! - \b COMP_B_ACCURACY_CLOCKED
    uint16_t referenceAccuracy;
} Comp_B_configureReferenceVoltageParam;


//*****************************************************************************
//
// The following are values that can be passed to the powerModeSelect parameter
// for functions: Comp_B_init(); the param parameter for functions:
// Comp_B_init().
//
//*****************************************************************************
#define COMP_B_POWERMODE_HIGHSPEED                                  (CBPWRMD_0)
#define COMP_B_POWERMODE_NORMALMODE                                 (CBPWRMD_1)
#define COMP_B_POWERMODE_ULTRALOWPOWER                              (CBPWRMD_2)

//*****************************************************************************
//
// The following are values that can be passed to the positiveTerminalInput
// parameter for functions: Comp_B_init(); the inputPort parameter for
// functions: Comp_B_disableInputBuffer(), and Comp_B_enableInputBuffer(); the
// param parameter for functions: Comp_B_init(), and Comp_B_init(); the
// negativeTerminalInput parameter for functions: Comp_B_init().
//
//*****************************************************************************
#define COMP_B_INPUT0                                               (CBIPSEL_0)
#define COMP_B_INPUT1                                               (CBIPSEL_1)
#define COMP_B_INPUT2                                               (CBIPSEL_2)
#define COMP_B_INPUT3                                               (CBIPSEL_3)
#define COMP_B_INPUT4                                               (CBIPSEL_4)
#define COMP_B_INPUT5                                               (CBIPSEL_5)
#define COMP_B_INPUT6                                               (CBIPSEL_6)
#define COMP_B_INPUT7                                               (CBIPSEL_7)
#define COMP_B_INPUT8                                               (CBIPSEL_8)
#define COMP_B_INPUT9                                               (CBIPSEL_9)
#define COMP_B_INPUT10                                             (CBIPSEL_10)
#define COMP_B_INPUT11                                             (CBIPSEL_11)
#define COMP_B_INPUT12                                             (CBIPSEL_12)
#define COMP_B_INPUT13                                             (CBIPSEL_13)
#define COMP_B_INPUT14                                             (CBIPSEL_14)
#define COMP_B_INPUT15                                             (CBIPSEL_15)
#define COMP_B_VREF                                                      (0x10)

//*****************************************************************************
//
// The following are values that can be passed to the
// outputFilterEnableAndDelayLevel parameter for functions: Comp_B_init(); the
// param parameter for functions: Comp_B_init().
//
//*****************************************************************************
#define COMP_B_FILTEROUTPUT_OFF                                            0x00
#define COMP_B_FILTEROUTPUT_DLYLVL1                            (CBF + CBFDLY_0)
#define COMP_B_FILTEROUTPUT_DLYLVL2                            (CBF + CBFDLY_1)
#define COMP_B_FILTEROUTPUT_DLYLVL3                            (CBF + CBFDLY_2)
#define COMP_B_FILTEROUTPUT_DLYLVL4                            (CBF + CBFDLY_3)

//*****************************************************************************
//
// The following are values that can be passed to the invertedOutputPolarity
// parameter for functions: Comp_B_init(); the param parameter for functions:
// Comp_B_init().
//
//*****************************************************************************
#define COMP_B_NORMALOUTPUTPOLARITY                               (!(CBOUTPOL))
#define COMP_B_INVERTEDOUTPUTPOLARITY                                (CBOUTPOL)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Comp_B_configureReferenceVoltage().
//
//*****************************************************************************
#define COMP_B_ACCURACY_STATIC                                      (!CBREFACC)
#define COMP_B_ACCURACY_CLOCKED                                      (CBREFACC)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: Comp_B_configureReferenceVoltage().
//
//*****************************************************************************
#define COMP_B_VREFBASE_VCC                                          (CBREFL_0)
#define COMP_B_VREFBASE1_5V                                          (CBREFL_1)
#define COMP_B_VREFBASE2_0V                                          (CBREFL_2)
#define COMP_B_VREFBASE2_5V                                          (CBREFL_3)

//*****************************************************************************
//
// The following are values that can be passed to the interruptMask parameter
// for functions: Comp_B_enableInterrupt(), and Comp_B_disableInterrupt().
//
//*****************************************************************************
#define COMP_B_OUTPUT_INT                                                  CBIE
#define COMP_B_OUTPUTINVERTED_INT                                         CBIIE

//*****************************************************************************
//
// The following are values that can be passed to the interruptFlagMask
// parameter for functions: Comp_B_clearInterrupt(), and
// Comp_B_getInterruptStatus() as well as returned by the
// Comp_B_getInterruptStatus() function.
//
//*****************************************************************************
#define COMP_B_OUTPUT_FLAG                                                CBIFG
#define COMP_B_OUTPUTINVERTED_FLAG                                       CBIIFG

//*****************************************************************************
//
// The following are values that can be passed to the edgeDirection parameter
// for functions: Comp_B_setInterruptEdgeDirection().
//
//*****************************************************************************
#define COMP_B_RISINGEDGE                                            (!(CBIES))
#define COMP_B_FALLINGEDGE                                              (CBIES)

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the Comp_B_outputValue() function.
//
//*****************************************************************************
#define COMP_B_LOW                                                        (0x0)
#define COMP_B_HIGH                                                     (CBOUT)

//*****************************************************************************
//
// The following are values that can be passed to the selectType parameter for
// functions: Comp_B_selectReferenceVoltage().
//
//*****************************************************************************
#define COMP_B_VREF_AUTO_SELECT                                          0x0000
#define COMP_B_VREF_MANUAL_SELECT                                        CBMRVS

//*****************************************************************************
//
// The following are values that can be passed to the selectVRef parameter for
// functions: Comp_B_selectReferenceVoltage().
//
//*****************************************************************************
#define COMP_B_SELECT_VREF0                                              0x0000
#define COMP_B_SELECT_VREF1                                              CBMRVL

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Initializes the Comp_B Module.
//!
//! Upon successful initialization of the Comp_B module, this function will
//! have reset all necessary register bits and set the given options in the
//! registers. To actually use the Comp_B module, the Comp_B_enable() function
//! must be explicitly called before use. If a Reference Voltage is set to a
//! terminal, the Voltage should be set using the Comp_B_setReferenceVoltage()
//! function.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param param is the pointer to struct for initialization.
//!
//! \return STATUS_SUCCESS or STATUS_FAILURE of the initialization process.
//
//*****************************************************************************
extern bool Comp_B_init(uint16_t baseAddress,
                        Comp_B_initParam *param);

//*****************************************************************************
//
//! \brief Generates a Reference Voltage to the terminal selected during
//! initialization.
//!
//! Use this function to generate a voltage to serve as a reference to the
//! terminal selected at initialization. The voltage is determined by the
//! equation: Vbase * (Numerator / 32). If the upper and lower limit voltage
//! numerators are equal, then a static reference is defined, whereas they are
//! different then a hysteresis effect is generated.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param param is the pointer to struct for reference voltage configuration.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_configureReferenceVoltage(uint16_t baseAddress,
                                             Comp_B_configureReferenceVoltageParam *param);

//*****************************************************************************
//
//! \brief Enables selected Comp_B interrupt sources.
//!
//! Enables the indicated Comp_B interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param interruptMask is the bit mask of the interrupt sources to be
//!        enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_B_OUTPUT_INT - Output interrupt
//!        - \b COMP_B_OUTPUTINVERTED_INT - Output interrupt inverted polarity
//!        \n Modified bits of \b CBINT register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_enableInterrupt(uint16_t baseAddress,
                                   uint16_t interruptMask);

//*****************************************************************************
//
//! \brief Disables selected Comp_B interrupt sources.
//!
//! Disables the indicated Comp_B interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param interruptMask is the bit mask of the interrupt sources to be
//!        disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_B_OUTPUT_INT - Output interrupt
//!        - \b COMP_B_OUTPUTINVERTED_INT - Output interrupt inverted polarity
//!        \n Modified bits of \b CBINT register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_disableInterrupt(uint16_t baseAddress,
                                    uint16_t interruptMask);

//*****************************************************************************
//
//! \brief Clears Comp_B interrupt flags.
//!
//! The Comp_B interrupt source is cleared, so that it no longer asserts. The
//! highest interrupt flag is automatically cleared when an interrupt vector
//! generator is used.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param interruptFlagMask is a bit mask of the interrupt sources to be
//!        cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_B_OUTPUT_FLAG - Output interrupt
//!        - \b COMP_B_OUTPUTINVERTED_FLAG - Output interrupt inverted polarity
//!        \n Modified bits of \b CBINT register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_clearInterrupt(uint16_t baseAddress,
                                  uint16_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Gets the current Comp_B interrupt status.
//!
//! This returns the interrupt status for the Comp_B module based on which flag
//! is passed.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param interruptFlagMask is the masked interrupt flag status to be
//!        returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b COMP_B_OUTPUT_FLAG - Output interrupt
//!        - \b COMP_B_OUTPUTINVERTED_FLAG - Output interrupt inverted polarity
//!
//! \return Logical OR of any of the following:
//!         - \b COMP_B_OUTPUT_FLAG Output interrupt
//!         - \b COMP_B_OUTPUTINVERTED_FLAG Output interrupt inverted polarity
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint8_t Comp_B_getInterruptStatus(uint16_t baseAddress,
                                         uint16_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Explicitly sets the edge direction that would trigger an interrupt.
//!
//! This function will set which direction the output will have to go, whether
//! rising or falling, to generate an interrupt based on a non-inverted
//! interrupt.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param edgeDirection determines which direction the edge would have to go
//!        to generate an interrupt based on the non-inverted interrupt flag.
//!        Valid values are:
//!        - \b COMP_B_RISINGEDGE [Default] - sets the bit to generate an
//!           interrupt when the output of the Comp_B falls from LOW to HIGH if
//!           the normal interrupt bit is set(and HIGH to LOW if the inverted
//!           interrupt enable bit is set).
//!        - \b COMP_B_FALLINGEDGE - sets the bit to generate an interrupt when
//!           the output of the Comp_B rises from HIGH to LOW if the normal
//!           interrupt bit is set(and LOW to HIGH if the inverted interrupt
//!           enable bit is set).
//!        \n Modified bits are \b CBIES of \b CBCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_setInterruptEdgeDirection(uint16_t baseAddress,
                                             uint16_t edgeDirection);

//*****************************************************************************
//
//! \brief Toggles the edge direction that would trigger an interrupt.
//!
//! This function will toggle which direction the output will have to go,
//! whether rising or falling, to generate an interrupt based on a non-inverted
//! interrupt. If the direction was rising, it is now falling, if it was
//! falling, it is now rising.
//!
//! \param baseAddress is the base address of the COMP_B module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_toggleInterruptEdgeDirection(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Turns on the Comp_B module.
//!
//! This function sets the bit that enables the operation of the Comp_B module.
//!
//! \param baseAddress is the base address of the COMP_B module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_enable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Turns off the Comp_B module.
//!
//! This function clears the CBON bit disabling the operation of the Comp_B
//! module, saving from excess power consumption.
//!
//! \param baseAddress is the base address of the COMP_B module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_disable(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Shorts the two input pins chosen during initialization.
//!
//! This function sets the bit that shorts the devices attached to the input
//! pins chosen from the initialization of the Comp_B.
//!
//! \param baseAddress is the base address of the COMP_B module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_shortInputs(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the short of the two input pins chosen during
//! initialization.
//!
//! This function clears the bit that shorts the devices attached to the input
//! pins chosen from the initialization of the Comp_B.
//!
//! \param baseAddress is the base address of the COMP_B module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_unshortInputs(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables the input buffer of the selected input port to effectively
//! allow for analog signals.
//!
//! This function sets the bit to disable the buffer for the specified input
//! port to allow for analog signals from any of the Comp_B input pins. This
//! bit is automatically set when the input is initialized to be used with the
//! Comp_B module. This function should be used whenever an analog input is
//! connected to one of these pins to prevent parasitic voltage from causing
//! unexpected results.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param inputPort is the port in which the input buffer will be disabled.
//!        Valid values are:
//!        - \b COMP_B_INPUT0 [Default]
//!        - \b COMP_B_INPUT1
//!        - \b COMP_B_INPUT2
//!        - \b COMP_B_INPUT3
//!        - \b COMP_B_INPUT4
//!        - \b COMP_B_INPUT5
//!        - \b COMP_B_INPUT6
//!        - \b COMP_B_INPUT7
//!        - \b COMP_B_INPUT8
//!        - \b COMP_B_INPUT9
//!        - \b COMP_B_INPUT10
//!        - \b COMP_B_INPUT11
//!        - \b COMP_B_INPUT12
//!        - \b COMP_B_INPUT13
//!        - \b COMP_B_INPUT14
//!        - \b COMP_B_INPUT15
//!        - \b COMP_B_VREF
//!        \n Modified bits are \b CBPDx of \b CBCTL3 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_disableInputBuffer(uint16_t baseAddress,
                                      uint8_t inputPort);

//*****************************************************************************
//
//! \brief Enables the input buffer of the selected input port to allow for
//! digital signals.
//!
//! This function clears the bit to enable the buffer for the specified input
//! port to allow for digital signals from any of the Comp_B input pins. This
//! should not be reset if there is an analog signal connected to the specified
//! input pin to prevent from unexpected results.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param inputPort is the port in which the input buffer will be enabled.
//!        Valid values are:
//!        - \b COMP_B_INPUT0 [Default]
//!        - \b COMP_B_INPUT1
//!        - \b COMP_B_INPUT2
//!        - \b COMP_B_INPUT3
//!        - \b COMP_B_INPUT4
//!        - \b COMP_B_INPUT5
//!        - \b COMP_B_INPUT6
//!        - \b COMP_B_INPUT7
//!        - \b COMP_B_INPUT8
//!        - \b COMP_B_INPUT9
//!        - \b COMP_B_INPUT10
//!        - \b COMP_B_INPUT11
//!        - \b COMP_B_INPUT12
//!        - \b COMP_B_INPUT13
//!        - \b COMP_B_INPUT14
//!        - \b COMP_B_INPUT15
//!        - \b COMP_B_VREF
//!        \n Modified bits are \b CBPDx of \b CBCTL3 register.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_enableInputBuffer(uint16_t baseAddress,
                                     uint8_t inputPort);

//*****************************************************************************
//
//! \brief Toggles the bit that swaps which terminals the inputs go to, while
//! also inverting the output of the Comp_B.
//!
//! This function toggles the bit that controls which input goes to which
//! terminal. After initialization, this bit is set to 0, after toggling it
//! once the inputs are routed to the opposite terminal and the output is
//! inverted.
//!
//! \param baseAddress is the base address of the COMP_B module.
//!
//! \return None
//
//*****************************************************************************
extern void Comp_B_swapIO(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Returns the output value of the Comp_B module.
//!
//! Returns the output value of the Comp_B module.
//!
//! \param baseAddress is the base address of the COMP_B module.
//!
//! \return One of the following:
//!         - \b COMP_B_LOW
//!         - \b COMP_B_HIGH
//!         \n indicating the output value of the Comp_B module
//
//*****************************************************************************
extern uint16_t Comp_B_outputValue(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Modifies how comparator output selects between VREF0 or VREF1.
//!
//! Only applicable in certain Comp_B reference sources. Consult
//! Comp_B_configureReferenceVoltage for details. If COMP_B_VREF_AUTO_SELECT,
//! then comparator output state chooses between VREF0 and VREF1. If
//! COMP_B_VREF_MANUAL_SELECT, then selectVRef param chooses.
//!
//! \param baseAddress is the base address of the COMP_B module.
//! \param selectType determines whether VREF instance is chosen automatically
//!        or manually
//!        Valid values are:
//!        - \b COMP_B_VREF_AUTO_SELECT [Default] - VREF instance is chosen by
//!           comparator output state.
//!        - \b COMP_B_VREF_MANUAL_SELECT - VREF instance is chosen by user
//!           (CBCTL1. CBMRVL bit)
//!        \n Modified bits are \b CBMRVS of \b CBCTL1 register.
//! \param selectVRef selects VREF0 or VREF1. Only applicable if VREF instance
//!        is set up to be chosen manually
//!        Valid values are:
//!        - \b COMP_B_SELECT_VREF0 [Default]
//!        - \b COMP_B_SELECT_VREF1
//!        \n Modified bits are \b CBMRVL of \b CBCTL1 register.
//!
//
//*****************************************************************************
extern void Comp_B_selectReferenceVoltage(uint16_t baseAddress,
                                          uint16_t selectType,
                                          uint16_t selectVRef);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_COMP_B_H__
