/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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
/* 
 * ======== usbEventHandling.c ========
 * Event-handling placeholder functions.
 * All functios are called in interrupt context.
 */

#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_API/USB_Common/defMSP430USB.h"
#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/usb.h"
//#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"

#ifdef _CDC_
#include "USB_API/USB_CDC_API/UsbCdc.h"
#endif

#ifdef _HID_
#include "USB_API/USB_HID_API/UsbHid.h"
#endif

#ifdef _MSC_
#include "USB_API/USB_MSC_API/UsbMsc.h"
#endif

#ifdef _PHDC_
#include "USB_API/USB_PHDC_API/UsbPHDC.h"
#endif

/*
 * If this function gets executed, it's a sign that the output of the USB PLL has failed.
 * returns TRUE to keep CPU awake
 */
BYTE USB_handleClockEvent ()
{
    //TO DO: You can place your code here

    return (FALSE);                              //return TRUE to wake the main loop (in the case the CPU slept before interrupt)
}

/*
 * If this function gets executed, it indicates that a valid voltage has just been applied to the VBUS pin.
 * returns TRUE to keep CPU awake
 */
BYTE USB_handleVbusOnEvent ()
{
    //TO DO: You can place your code here

    //We switch on USB and connect to the BUS
    if (USB_enable() == kUSB_succeed){
        USB_reset();
        USB_connect();                          //generate rising edge on DP -> the host enumerates our device as full speed device
    }
    return (FALSE);                              //return TRUE to wake the main loop (in the case the CPU slept before interrupt)
}

/*
 * If this function gets executed, it indicates that a valid voltage has just been removed from the VBUS pin.
 * returns TRUE to keep CPU awake
 */
BYTE USB_handleVbusOffEvent ()
{
    //TO DO: You can place your code here

    //XT2_Stop();

    return (FALSE);                              //return TRUE to wake the main loop (in the case the CPU slept before interrupt)
}

/*
 * If this function gets executed, it indicates that the USB host has issued a USB reset event to the device.
 * returns TRUE to keep CPU awake
 */
BYTE USB_handleResetEvent ()
{
    //TO DO: You can place your code here

    return (FALSE);                              //return TRUE to wake the main loop (in the case the CPU slept before interrupt)
}

/*
 * If this function gets executed, it indicates that the USB host has chosen to suspend this device after a period of active
 * operation.
 * returns TRUE to keep CPU awake
 */
BYTE USB_handleSuspendEvent ()
{
    //TO DO: You can place your code here

    return (FALSE);                              //return TRUE to wake the main loop (in the case the CPU slept before interrupt)
}

/*
 * If this function gets executed, it indicates that the USB host has chosen to resume this device after a period of suspended
 * operation.
 * returns TRUE to keep CPU awake
 */
BYTE USB_handleResumeEvent ()
{
    //TO DO: You can place your code here

    return (FALSE);                              //return TRUE to wake the main loop (in the case the CPU slept before interrupt)
}

/*
 * If this function gets executed, it indicates that the USB host has enumerated this device :
 * after host assigned the address to the device.
 * returns TRUE to keep CPU awake
 */
BYTE USB_handleEnumCompleteEvent ()
{
    //TO DO: You can place your code here

    return (FALSE);                              //return TRUE to wake the main loop (in the case the CPU slept before interrupt)
}

#ifdef _CDC_

/*
 * This event indicates that data has been received for interface intfNum, but no data receive operation is underway.
 * returns TRUE to keep CPU awake
 */
BYTE USBCDC_handleDataReceived (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (TRUE);                              //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that a send operation on interface intfNum has just been completed.
 * returns TRUE to keep CPU awake
 */
BYTE USBCDC_handleSendCompleted (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (FALSE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that a receive operation on interface intfNum has just been completed.
 */
BYTE USBCDC_handleReceiveCompleted (BYTE intfNum)
{
    return (TRUE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that new line coding params have been received from the host
 */
BYTE USBCDC_handleSetLineCoding (BYTE intfNum, ULONG lBaudrate)
{
    //TO DO: You can place your code here

    return (FALSE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that new line state has been received from the host
 */
BYTE USBCDC_handleSetControlLineState (BYTE intfNum, BYTE lineState)
{
	return FALSE;
}

#endif //_CDC_

#ifdef _HID_
/*
 * This event indicates that data has been received for interface intfNum, but no data receive operation is underway.
 * returns TRUE to keep CPU awake
 */
BYTE USBHID_handleDataReceived (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (TRUE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that a send operation on interface intfNum has just been completed.
 * returns TRUE to keep CPU awake
 */
BYTE USBHID_handleSendCompleted (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (FALSE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that a receive operation on interface intfNum has just been completed.
 */
BYTE USBHID_handleReceiveCompleted (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (FALSE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that a request to switch to or from Boot protocol
 * was received from the host
 */
BYTE USBHID_handleBootProtocol (BYTE protocol, BYTE intfnum)
{
    return (FALSE);
}

/*
 * This event indicates that a Set_Report request was received from the host
 * The application needs to supply a buffer to retrieve the data that will be sent
 * as part of this request. This handler is passed the reportTypereportId(lower byte), the length of data
 * phase as well as the interface number.
 */
BYTE *USBHID_handleSetReport (BYTE reportType, BYTE reportId,
    WORD dataLength,
    BYTE intfnum)
{
	switch (reportType) {
		case USB_REQ_HID_INPUT:
			//Return pointer to input Report Buffer
			return (0);
		case USB_REQ_HID_OUTPUT:
			//Return pointer to output Report Buffer
			return (0);

		case USB_REQ_HID_FEATURE:
			//Return pointer to feature Report Buffer
			return (0);

		default:
			return (0);
	}
}

/*
 * This event indicates that data as part of Set_Report request was received from the host
 * Tha application can return TRUE to wake up the CPU. If the application supplied a buffer
 * as part of USBHID_handleSetReport, then this buffer will contain the Set Report data.
 */
BYTE USBHID_handleSetReportDataAvailable (BYTE intfnum)
{
	//Process received data based on currentReportType
    return (FALSE);
}

/*
 * This event indicates that a Get_Report request was received from the host
 * The application can supply a buffer of data that will be sent to the host.
 * This handler is passed the reportType, reportId, the requested length as well
 * as the interface number.
 */
BYTE *USBHID_handleGetReport (BYTE reportType, BYTE reportId,
    WORD requestedLength,
    BYTE intfnum)
{
	//report data should be ready in buffers for Get Report.
	switch (reportType) {
		case USB_REQ_HID_INPUT:
			//Return pointer to input Report Buffer
			return (0);
		case USB_REQ_HID_OUTPUT:
			//Return pointer to OUTput Report Buffer
			return (0);
		case USB_REQ_HID_FEATURE:
			//Return pointer to FEATURE Report Buffer
			return (0);
		default:
			return (0);
	}
}

#endif //_HID_

#ifdef _MSC_
BYTE USBMSC_handleBufferEvent (VOID)
{
    return (FALSE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

#endif //_MSC_


#ifdef _PHDC_

/*
 * This event indicates that data has been received for interface intfNum, but no data receive operation is underway.
 * returns TRUE to keep CPU awake
 */
BYTE USBPHDC_handleDataReceived (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (FALSE);                              //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that a send operation on interface intfNum has just been completed.
 * returns TRUE to keep CPU awake
 */
BYTE USBPHDC_handleSendCompleted (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (FALSE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

/*
 * This event indicates that a receive operation on interface intfNum has just been completed.
 */
BYTE USBPHDC_handleReceiveCompleted (BYTE intfNum)
{
    //TO DO: You can place your code here

    return (FALSE);                             //return FALSE to go asleep after interrupt (in the case the CPU slept before
                                                //interrupt)
}

#endif //_PHDC_

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
