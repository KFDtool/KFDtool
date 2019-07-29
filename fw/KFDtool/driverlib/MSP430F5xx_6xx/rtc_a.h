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
// rtc_a.h - Driver for the RTC_A Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_RTC_A_H__
#define __MSP430WARE_RTC_A_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_RTC__

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
//! \brief Used in the RTC_A_configureCalendarAlarm() function as the param
//! parameter.
//
//*****************************************************************************
typedef struct RTC_A_configureCalendarAlarmParam {
    //! Is the alarm condition for the minutes.
    //! \n Valid values are:
    //! - \b RTC_A_ALARMCONDITION_OFF [Default]
    uint8_t minutesAlarm;
    //! Is the alarm condition for the hours.
    //! \n Valid values are:
    //! - \b RTC_A_ALARMCONDITION_OFF [Default]
    uint8_t hoursAlarm;
    //! Is the alarm condition for the day of week.
    //! \n Valid values are:
    //! - \b RTC_A_ALARMCONDITION_OFF [Default]
    uint8_t dayOfWeekAlarm;
    //! Is the alarm condition for the day of the month.
    //! \n Valid values are:
    //! - \b RTC_A_ALARMCONDITION_OFF [Default]
    uint8_t dayOfMonthAlarm;
} RTC_A_configureCalendarAlarmParam;

//*****************************************************************************
//
//! \brief Used in the RTC_A_initCalendar() function as the CalendarTime
//! parameter.
//
//*****************************************************************************
typedef struct Calendar {
    //! Seconds of minute between 0-59
    uint8_t Seconds;
    //! Minutes of hour between 0-59
    uint8_t Minutes;
    //! Hour of day between 0-23
    uint8_t Hours;
    //! Day of week between 0-6
    uint8_t DayOfWeek;
    //! Day of month between 1-31
    uint8_t DayOfMonth;
    //! Month between 1-12
    uint8_t Month;
    //! Year between 0-4095
    uint16_t Year;
} Calendar;


//*****************************************************************************
//
// The following are values that can be passed to the frequencySelect parameter
// for functions: RTC_A_setCalibrationFrequency().
//
//*****************************************************************************
#define RTC_A_CALIBRATIONFREQ_OFF                                   (RTCCALF_0)
#define RTC_A_CALIBRATIONFREQ_512HZ                                 (RTCCALF_1)
#define RTC_A_CALIBRATIONFREQ_256HZ                                 (RTCCALF_2)
#define RTC_A_CALIBRATIONFREQ_1HZ                                   (RTCCALF_3)

//*****************************************************************************
//
// The following are values that can be passed to the offsetDirection parameter
// for functions: RTC_A_setCalibrationData().
//
//*****************************************************************************
#define RTC_A_CALIBRATION_DOWN2PPM                                 (!(RTCCALS))
#define RTC_A_CALIBRATION_UP4PPM                                      (RTCCALS)

//*****************************************************************************
//
// The following are values that can be passed to the formatSelect parameter
// for functions: RTC_A_initCalendar().
//
//*****************************************************************************
#define RTC_A_FORMAT_BINARY                                         (!(RTCBCD))
#define RTC_A_FORMAT_BCD                                               (RTCBCD)

//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: RTC_A_configureCalendarAlarm(), RTC_A_configureCalendarAlarm(),
// RTC_A_configureCalendarAlarm(), and RTC_A_configureCalendarAlarm().
//
//*****************************************************************************
#define RTC_A_ALARMCONDITION_OFF                                         (0x80)

//*****************************************************************************
//
// The following are values that can be passed to the eventSelect parameter for
// functions: RTC_A_setCalendarEvent().
//
//*****************************************************************************
#define RTC_A_CALENDAREVENT_MINUTECHANGE                             (RTCTEV_0)
#define RTC_A_CALENDAREVENT_HOURCHANGE                               (RTCTEV_1)
#define RTC_A_CALENDAREVENT_NOON                                     (RTCTEV_2)
#define RTC_A_CALENDAREVENT_MIDNIGHT                                 (RTCTEV_3)

//*****************************************************************************
//
// The following are values that can be passed to the prescaleSelect parameter
// for functions: RTC_A_initCounterPrescale(), RTC_A_holdCounterPrescale(),
// RTC_A_startCounterPrescale(), RTC_A_definePrescaleEvent(),
// RTC_A_getPrescaleValue(), and RTC_A_setPrescaleValue().
//
//*****************************************************************************
#define RTC_A_PRESCALE_0                                                  (0x0)
#define RTC_A_PRESCALE_1                                                  (0x2)

//*****************************************************************************
//
// The following are values that can be passed to the clockSelect parameter for
// functions: RTC_A_initCounter().
//
//*****************************************************************************
#define RTC_A_CLOCKSELECT_ACLK                                      (RTCSSEL_0)
#define RTC_A_CLOCKSELECT_SMCLK                                     (RTCSSEL_1)
#define RTC_A_CLOCKSELECT_RT1PS                                     (RTCSSEL_2)

//*****************************************************************************
//
// The following are values that can be passed to the prescaleClockSelect
// parameter for functions: RTC_A_initCounterPrescale().
//
//*****************************************************************************
#define RTC_A_PSCLOCKSELECT_ACLK                                    (RT1SSEL_0)
#define RTC_A_PSCLOCKSELECT_SMCLK                                   (RT1SSEL_1)
#define RTC_A_PSCLOCKSELECT_RT0PS                                   (RT1SSEL_2)

//*****************************************************************************
//
// The following are values that can be passed to the counterSizeSelect
// parameter for functions: RTC_A_initCounter().
//
//*****************************************************************************
#define RTC_A_COUNTERSIZE_8BIT                                       (RTCTEV_0)
#define RTC_A_COUNTERSIZE_16BIT                                      (RTCTEV_1)
#define RTC_A_COUNTERSIZE_24BIT                                      (RTCTEV_2)
#define RTC_A_COUNTERSIZE_32BIT                                      (RTCTEV_3)

//*****************************************************************************
//
// The following are values that can be passed to the prescaleDivider parameter
// for functions: RTC_A_initCounterPrescale().
//
//*****************************************************************************
#define RTC_A_PSDIVIDER_2                                          (RT0PSDIV_0)
#define RTC_A_PSDIVIDER_4                                          (RT0PSDIV_1)
#define RTC_A_PSDIVIDER_8                                          (RT0PSDIV_2)
#define RTC_A_PSDIVIDER_16                                         (RT0PSDIV_3)
#define RTC_A_PSDIVIDER_32                                         (RT0PSDIV_4)
#define RTC_A_PSDIVIDER_64                                         (RT0PSDIV_5)
#define RTC_A_PSDIVIDER_128                                        (RT0PSDIV_6)
#define RTC_A_PSDIVIDER_256                                        (RT0PSDIV_7)

//*****************************************************************************
//
// The following are values that can be passed to the prescaleEventDivider
// parameter for functions: RTC_A_definePrescaleEvent().
//
//*****************************************************************************
#define RTC_A_PSEVENTDIVIDER_2                                        (RT0IP_0)
#define RTC_A_PSEVENTDIVIDER_4                                        (RT0IP_1)
#define RTC_A_PSEVENTDIVIDER_8                                        (RT0IP_2)
#define RTC_A_PSEVENTDIVIDER_16                                       (RT0IP_3)
#define RTC_A_PSEVENTDIVIDER_32                                       (RT0IP_4)
#define RTC_A_PSEVENTDIVIDER_64                                       (RT0IP_5)
#define RTC_A_PSEVENTDIVIDER_128                                      (RT0IP_6)
#define RTC_A_PSEVENTDIVIDER_256                                      (RT0IP_7)

//*****************************************************************************
//
// The following are values that can be passed to the interruptMask parameter
// for functions: RTC_A_enableInterrupt(), and RTC_A_disableInterrupt(); the
// interruptFlagMask parameter for functions: RTC_A_getInterruptStatus(), and
// RTC_A_clearInterrupt() as well as returned by the RTC_A_getInterruptStatus()
// function.
//
//*****************************************************************************
#define RTC_A_TIME_EVENT_INTERRUPT                                     RTCTEVIE
#define RTC_A_CLOCK_ALARM_INTERRUPT                                      RTCAIE
#define RTC_A_CLOCK_READ_READY_INTERRUPT                               RTCRDYIE
#define RTC_A_PRESCALE_TIMER0_INTERRUPT                                    0x02
#define RTC_A_PRESCALE_TIMER1_INTERRUPT                                    0x01

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Starts the RTC.
//!
//! This function clears the RTC main hold bit to allow the RTC to function.
//!
//! \param baseAddress is the base address of the RTC_A module.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_startClock(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Holds the RTC.
//!
//! This function sets the RTC main hold bit to disable RTC functionality.
//!
//! \param baseAddress is the base address of the RTC_A module.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_holdClock(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Allows and Sets the frequency output to RTCCLK pin for calibration
//! measurement.
//!
//! This function sets a frequency to measure at the RTCCLK output pin. After
//! testing the set frequency, the calibration could be set accordingly.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param frequencySelect is the frequency output to RTCCLK.
//!        Valid values are:
//!        - \b RTC_A_CALIBRATIONFREQ_OFF [Default] - turn off calibration
//!           output
//!        - \b RTC_A_CALIBRATIONFREQ_512HZ - output signal at 512Hz for
//!           calibration
//!        - \b RTC_A_CALIBRATIONFREQ_256HZ - output signal at 256Hz for
//!           calibration
//!        - \b RTC_A_CALIBRATIONFREQ_1HZ - output signal at 1Hz for
//!           calibration
//!        \n Modified bits are \b RTCCALF of \b RTCCTL3 register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_setCalibrationFrequency(uint16_t baseAddress,
                                          uint16_t frequencySelect);

//*****************************************************************************
//
//! \brief Sets the specified calibration for the RTC.
//!
//! This function sets the calibration offset to make the RTC as accurate as
//! possible. The offsetDirection can be either +4-ppm or -2-ppm, and the
//! offsetValue should be from 1-63 and is multiplied by the direction setting
//! (i.e. +4-ppm * 8 (offsetValue) = +32-ppm). Please note, when measuring the
//! frequency after setting the calibration, you will only see a change on the
//! 1Hz frequency.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param offsetDirection is the direction that the calibration offset will
//!        go.
//!        Valid values are:
//!        - \b RTC_A_CALIBRATION_DOWN2PPM - calibrate at steps of -2
//!        - \b RTC_A_CALIBRATION_UP4PPM - calibrate at steps of +4
//!        \n Modified bits are \b RTCCALS of \b RTCCTL2 register.
//! \param offsetValue is the value that the offset will be a factor of; a
//!        valid value is any integer from 1-63.
//!        \n Modified bits are \b RTCCAL of \b RTCCTL2 register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_setCalibrationData(uint16_t baseAddress,
                                     uint8_t offsetDirection,
                                     uint8_t offsetValue);

//*****************************************************************************
//
//! \brief Initializes the settings to operate the RTC in Counter mode.
//!
//! This function initializes the Counter mode of the RTC_A. Setting the clock
//! source and counter size will allow an interrupt from the RTCTEVIFG once an
//! overflow to the counter register occurs.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param clockSelect is the selected clock for the counter mode to use.
//!        Valid values are:
//!        - \b RTC_A_CLOCKSELECT_ACLK [Default]
//!        - \b RTC_A_CLOCKSELECT_SMCLK
//!        - \b RTC_A_CLOCKSELECT_RT1PS - use Prescaler 1 as source to RTC
//!        \n Modified bits are \b RTCSSEL of \b RTCCTL1 register.
//! \param counterSizeSelect is the size of the counter.
//!        Valid values are:
//!        - \b RTC_A_COUNTERSIZE_8BIT [Default]
//!        - \b RTC_A_COUNTERSIZE_16BIT
//!        - \b RTC_A_COUNTERSIZE_24BIT
//!        - \b RTC_A_COUNTERSIZE_32BIT
//!        \n Modified bits are \b RTCTEV of \b RTCCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_initCounter(uint16_t baseAddress,
                              uint16_t clockSelect,
                              uint16_t counterSizeSelect);

//*****************************************************************************
//
//! \brief Initializes the settings to operate the RTC in calendar mode
//!
//! This function initializes the Calendar mode of the RTC module. To prevent
//! potential erroneous alarm conditions from occurring, the alarm should be
//! disabled by clearing the RTCAIE, RTCAIFG and AE bits with APIs:
//! RTC_A_disableInterrupt(), RTC_A_clearInterrupt() and
//! RTC_A_configureCalendarAlarm() before calendar initialization.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param CalendarTime is the pointer to the structure containing the values
//!        for the Calendar to be initialized to. Valid values should be of
//!        type pointer to Calendar and should contain the following members
//!        and corresponding values: \n\b Seconds between 0-59 \n\b Minutes
//!        between 0-59 \n\b Hours between 0-23 \n\b DayOfWeek between 0-6 \n\b
//!        DayOfMonth between 1-31 \n\b Month between 1-12 \n\b Year between
//!        0-4095 \n\b NOTE: Values beyond the ones specified may result in
//!        erratic behavior.
//! \param formatSelect is the format for the Calendar registers to use.
//!        Valid values are:
//!        - \b RTC_A_FORMAT_BINARY [Default]
//!        - \b RTC_A_FORMAT_BCD
//!        \n Modified bits are \b RTCBCD of \b RTCCTL1 register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_initCalendar(uint16_t baseAddress,
                               Calendar *CalendarTime,
                               uint16_t formatSelect);

//*****************************************************************************
//
//! \brief Returns the Calendar Time stored in the Calendar registers of the
//! RTC.
//!
//! This function returns the current Calendar time in the form of a Calendar
//! structure. The RTCRDY polling is used in this function to prevent reading
//! invalid time.
//!
//! \param baseAddress is the base address of the RTC_A module.
//!
//! \return A Calendar structure containing the current time.
//
//*****************************************************************************
extern Calendar RTC_A_getCalendarTime(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets and Enables the desired Calendar Alarm settings.
//!
//! This function sets a Calendar interrupt condition to assert the RTCAIFG
//! interrupt flag. The condition is a logical and of all of the parameters.
//! For example if the minutes and hours alarm is set, then the interrupt will
//! only assert when the minutes AND the hours change to the specified setting.
//! Use the RTC_A_ALARM_OFF for any alarm settings that should not be apart of
//! the alarm condition.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param param is the pointer to struct for calendar alarm configuration.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_configureCalendarAlarm(uint16_t baseAddress,
                                         RTC_A_configureCalendarAlarmParam *param);

//*****************************************************************************
//
//! \brief Sets a single specified Calendar interrupt condition
//!
//! This function sets a specified event to assert the RTCTEVIFG interrupt.
//! This interrupt is independent from the Calendar alarm interrupt.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param eventSelect is the condition selected.
//!        Valid values are:
//!        - \b RTC_A_CALENDAREVENT_MINUTECHANGE - assert interrupt on every
//!           minute
//!        - \b RTC_A_CALENDAREVENT_HOURCHANGE - assert interrupt on every hour
//!        - \b RTC_A_CALENDAREVENT_NOON - assert interrupt when hour is 12
//!        - \b RTC_A_CALENDAREVENT_MIDNIGHT - assert interrupt when hour is 0
//!        \n Modified bits are \b RTCTEV of \b RTCCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_setCalendarEvent(uint16_t baseAddress,
                                   uint16_t eventSelect);

//*****************************************************************************
//
//! \brief Returns the value of the Counter register.
//!
//! This function returns the value of the counter register for the RTC_A
//! module. It will return the 32-bit value no matter the size set during
//! initialization. The RTC should be held before trying to use this function.
//!
//! \param baseAddress is the base address of the RTC_A module.
//!
//! \return The raw value of the full 32-bit Counter Register.
//
//*****************************************************************************
extern uint32_t RTC_A_getCounterValue(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Sets the value of the Counter register
//!
//! This function sets the counter register of the RTC_A module.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param counterValue is the value to set the Counter register to; a valid
//!        value may be any 32-bit integer.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_setCounterValue(uint16_t baseAddress,
                                  uint32_t counterValue);

//*****************************************************************************
//
//! \brief Initializes the Prescaler for Counter mode.
//!
//! This function initializes the selected prescaler for the counter mode in
//! the RTC_A module. If the RTC is initialized in Calendar mode, then these
//! are automatically initialized. The Prescalers can be used to divide a clock
//! source additionally before it gets to the main RTC clock.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param prescaleSelect is the prescaler to initialize.
//!        Valid values are:
//!        - \b RTC_A_PRESCALE_0
//!        - \b RTC_A_PRESCALE_1
//! \param prescaleClockSelect is the clock to drive the selected prescaler.
//!        Valid values are:
//!        - \b RTC_A_PSCLOCKSELECT_ACLK
//!        - \b RTC_A_PSCLOCKSELECT_SMCLK
//!        - \b RTC_A_PSCLOCKSELECT_RT0PS - use Prescaler 0 as source to
//!           Prescaler 1 (May only be used if prescaleSelect is
//!           RTC_A_PRESCALE_1)
//!        \n Modified bits are \b RTxSSEL of \b RTCPSxCTL register.
//! \param prescaleDivider is the divider for the selected clock source.
//!        Valid values are:
//!        - \b RTC_A_PSDIVIDER_2 [Default]
//!        - \b RTC_A_PSDIVIDER_4
//!        - \b RTC_A_PSDIVIDER_8
//!        - \b RTC_A_PSDIVIDER_16
//!        - \b RTC_A_PSDIVIDER_32
//!        - \b RTC_A_PSDIVIDER_64
//!        - \b RTC_A_PSDIVIDER_128
//!        - \b RTC_A_PSDIVIDER_256
//!        \n Modified bits are \b RTxPSDIV of \b RTCPSxCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_initCounterPrescale(uint16_t baseAddress,
                                      uint8_t prescaleSelect,
                                      uint16_t prescaleClockSelect,
                                      uint16_t prescaleDivider);

//*****************************************************************************
//
//! \brief Holds the selected Prescaler.
//!
//! This function holds the prescale counter from continuing. This will only
//! work in counter mode, in Calendar mode, the RTC_A_holdClock() must be used.
//! In counter mode, if using both prescalers in conjunction with the main RTC
//! counter, then stopping RT0PS will stop RT1PS, but stopping RT1PS will not
//! stop RT0PS.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param prescaleSelect is the prescaler to hold.
//!        Valid values are:
//!        - \b RTC_A_PRESCALE_0
//!        - \b RTC_A_PRESCALE_1
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_holdCounterPrescale(uint16_t baseAddress,
                                      uint8_t prescaleSelect);

//*****************************************************************************
//
//! \brief Starts the selected Prescaler.
//!
//! This function starts the selected prescale counter. This function will only
//! work if the RTC is in counter mode.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param prescaleSelect is the prescaler to start.
//!        Valid values are:
//!        - \b RTC_A_PRESCALE_0
//!        - \b RTC_A_PRESCALE_1
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_startCounterPrescale(uint16_t baseAddress,
                                       uint8_t prescaleSelect);

//*****************************************************************************
//
//! \brief Sets up an interrupt condition for the selected Prescaler.
//!
//! This function sets the condition for an interrupt to assert based on the
//! individual prescalers.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param prescaleSelect is the prescaler to define an interrupt for.
//!        Valid values are:
//!        - \b RTC_A_PRESCALE_0
//!        - \b RTC_A_PRESCALE_1
//! \param prescaleEventDivider is a divider to specify when an interrupt can
//!        occur based on the clock source of the selected prescaler. (Does not
//!        affect timer of the selected prescaler).
//!        Valid values are:
//!        - \b RTC_A_PSEVENTDIVIDER_2 [Default]
//!        - \b RTC_A_PSEVENTDIVIDER_4
//!        - \b RTC_A_PSEVENTDIVIDER_8
//!        - \b RTC_A_PSEVENTDIVIDER_16
//!        - \b RTC_A_PSEVENTDIVIDER_32
//!        - \b RTC_A_PSEVENTDIVIDER_64
//!        - \b RTC_A_PSEVENTDIVIDER_128
//!        - \b RTC_A_PSEVENTDIVIDER_256
//!        \n Modified bits are \b RTxIP of \b RTCPSxCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_definePrescaleEvent(uint16_t baseAddress,
                                      uint8_t prescaleSelect,
                                      uint8_t prescaleEventDivider);

//*****************************************************************************
//
//! \brief Returns the selected prescaler value.
//!
//! This function returns the value of the selected prescale counter register.
//! Note that the counter value should be held by calling RTC_A_holdClock()
//! before calling this API.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param prescaleSelect is the prescaler to obtain the value of.
//!        Valid values are:
//!        - \b RTC_A_PRESCALE_0
//!        - \b RTC_A_PRESCALE_1
//!
//! \return The value of the specified prescaler count register
//
//*****************************************************************************
extern uint8_t RTC_A_getPrescaleValue(uint16_t baseAddress,
                                      uint8_t prescaleSelect);

//*****************************************************************************
//
//! \brief Sets the selected prescaler value.
//!
//! This function sets the prescale counter value. Before setting the prescale
//! counter, it should be held by calling RTC_A_holdClock().
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param prescaleSelect is the prescaler to set the value for.
//!        Valid values are:
//!        - \b RTC_A_PRESCALE_0
//!        - \b RTC_A_PRESCALE_1
//! \param prescaleCounterValue is the specified value to set the prescaler to.
//!        Valid values are any integer between 0-255
//!        \n Modified bits are \b RTxPS of \b RTxPS register.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_setPrescaleValue(uint16_t baseAddress,
                                   uint8_t prescaleSelect,
                                   uint8_t prescaleCounterValue);

//*****************************************************************************
//
//! \brief Enables selected RTC interrupt sources.
//!
//! This function enables the selected RTC interrupt source.  Only the sources
//! that are enabled can be reflected to the processor interrupt; disabled
//! sources have no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param interruptMask is a bit mask of the interrupts to enable.
//!        Mask value is the logical OR of any of the following:
//!        - \b RTC_A_TIME_EVENT_INTERRUPT - asserts when counter overflows in
//!           counter mode or when Calendar event condition defined by
//!           defineCalendarEvent() is met.
//!        - \b RTC_A_CLOCK_ALARM_INTERRUPT - asserts when alarm condition in
//!           Calendar mode is met.
//!        - \b RTC_A_CLOCK_READ_READY_INTERRUPT - asserts when Calendar
//!           registers are settled.
//!        - \b RTC_A_PRESCALE_TIMER0_INTERRUPT - asserts when Prescaler 0
//!           event condition is met.
//!        - \b RTC_A_PRESCALE_TIMER1_INTERRUPT - asserts when Prescaler 1
//!           event condition is met.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_enableInterrupt(uint16_t baseAddress,
                                  uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Disables selected RTC interrupt sources.
//!
//! This function disables the selected RTC interrupt source. Only the sources
//! that are enabled can be reflected to the processor interrupt; disabled
//! sources have no effect on the processor.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param interruptMask is a bit mask of the interrupts to disable.
//!        Mask value is the logical OR of any of the following:
//!        - \b RTC_A_TIME_EVENT_INTERRUPT - asserts when counter overflows in
//!           counter mode or when Calendar event condition defined by
//!           defineCalendarEvent() is met.
//!        - \b RTC_A_CLOCK_ALARM_INTERRUPT - asserts when alarm condition in
//!           Calendar mode is met.
//!        - \b RTC_A_CLOCK_READ_READY_INTERRUPT - asserts when Calendar
//!           registers are settled.
//!        - \b RTC_A_PRESCALE_TIMER0_INTERRUPT - asserts when Prescaler 0
//!           event condition is met.
//!        - \b RTC_A_PRESCALE_TIMER1_INTERRUPT - asserts when Prescaler 1
//!           event condition is met.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_disableInterrupt(uint16_t baseAddress,
                                   uint8_t interruptMask);

//*****************************************************************************
//
//! \brief Returns the status of the selected interrupts flags.
//!
//! This function returns the status of the interrupt flag for the selected
//! channel.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param interruptFlagMask is a bit mask of the interrupt flags to return the
//!        status of.
//!        Mask value is the logical OR of any of the following:
//!        - \b RTC_A_TIME_EVENT_INTERRUPT - asserts when counter overflows in
//!           counter mode or when Calendar event condition defined by
//!           defineCalendarEvent() is met.
//!        - \b RTC_A_CLOCK_ALARM_INTERRUPT - asserts when alarm condition in
//!           Calendar mode is met.
//!        - \b RTC_A_CLOCK_READ_READY_INTERRUPT - asserts when Calendar
//!           registers are settled.
//!        - \b RTC_A_PRESCALE_TIMER0_INTERRUPT - asserts when Prescaler 0
//!           event condition is met.
//!        - \b RTC_A_PRESCALE_TIMER1_INTERRUPT - asserts when Prescaler 1
//!           event condition is met.
//!
//! \return Logical OR of any of the following:
//!         - \b RTC_A_TIME_EVENT_INTERRUPT asserts when counter overflows in
//!         counter mode or when Calendar event condition defined by
//!         defineCalendarEvent() is met.
//!         - \b RTC_A_CLOCK_ALARM_INTERRUPT asserts when alarm condition in
//!         Calendar mode is met.
//!         - \b RTC_A_CLOCK_READ_READY_INTERRUPT asserts when Calendar
//!         registers are settled.
//!         - \b RTC_A_PRESCALE_TIMER0_INTERRUPT asserts when Prescaler 0 event
//!         condition is met.
//!         - \b RTC_A_PRESCALE_TIMER1_INTERRUPT asserts when Prescaler 1 event
//!         condition is met.
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
extern uint8_t RTC_A_getInterruptStatus(uint16_t baseAddress,
                                        uint8_t interruptFlagMask);

//*****************************************************************************
//
//! \brief Clears selected RTC interrupt flags.
//!
//! This function clears the RTC interrupt flag is cleared, so that it no
//! longer asserts.
//!
//! \param baseAddress is the base address of the RTC_A module.
//! \param interruptFlagMask is a bit mask of the interrupt flags to be
//!        cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b RTC_A_TIME_EVENT_INTERRUPT - asserts when counter overflows in
//!           counter mode or when Calendar event condition defined by
//!           defineCalendarEvent() is met.
//!        - \b RTC_A_CLOCK_ALARM_INTERRUPT - asserts when alarm condition in
//!           Calendar mode is met.
//!        - \b RTC_A_CLOCK_READ_READY_INTERRUPT - asserts when Calendar
//!           registers are settled.
//!        - \b RTC_A_PRESCALE_TIMER0_INTERRUPT - asserts when Prescaler 0
//!           event condition is met.
//!        - \b RTC_A_PRESCALE_TIMER1_INTERRUPT - asserts when Prescaler 1
//!           event condition is met.
//!
//! \return None
//
//*****************************************************************************
extern void RTC_A_clearInterrupt(uint16_t baseAddress,
                                 uint8_t interruptFlagMask);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_RTC_A_H__
