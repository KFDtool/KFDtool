/** @file UsbHid.c
 *  @brief Contains APIs related to HID (Human Interface Device) device class.
 */
//
//! \cond
//

/* 
 * ======== UsbHid.c ========
 */
#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_Common/usb.h"                  //USB-specific Data Structures
#include "UsbHid.h"
#include <descriptors.h>
#include <string.h>

#ifdef _HID_

//function pointers
extern void *(*USB_TX_memcpy)(void * dest, const void * source, size_t count);
extern void *(*USB_RX_memcpy)(void * dest, const void * source, size_t count);

//Local Macros
#define INTFNUM_OFFSET(X)   (X - HID0_INTFNUM)  //Get the HID offset

extern uint8_t const report_len_input[HID_NUM_INTERFACES];

static struct _HidWrite {
    uint16_t nHidBytesToSend;                       //holds counter of bytes to be sent
    uint16_t nHidBytesToSendLeft;                   //holds counter how many bytes is still to be sent
    const uint8_t* pHidBufferToSend;               //holds the buffer with data to be sent
    uint8_t bCurrentBufferXY;                      //indicates which buffer is to use next for for write into IN OUT endpoint
} HidWriteCtrl[HID_NUM_INTERFACES];

static struct _HidRead {
    uint8_t *pUserBuffer;                          //holds the current position of user's receiving buffer. If NULL- no receiving
                                                //operation started
    uint8_t *pCurrentEpPos;                        //current positon to read of received data from curent EP
    uint16_t nBytesToReceive;                       //holds how many bytes was requested by receiveData() to receive
    uint16_t nBytesToReceiveLeft;                   //holds how many bytes is still requested by receiveData() to receive
    uint8_t * pCT1;                                //holds current EPBCTxx register
    uint8_t * pCT2;                                //holds next EPBCTxx register
    uint8_t * pEP2;                                //holds addr of the next EP buffer
    uint8_t nBytesInEp;                            //how many received bytes still available in current EP
    uint8_t bCurrentBufferXY;                      //indicates which buffer is used by host to transmit data via OUT endpoint
} HidReadCtrl[HID_NUM_INTERFACES];

extern uint16_t wUsbEventMask;

uint8_t hidProtocol[HID_NUM_INTERFACES] = {0};
uint8_t hidIdleRate[HID_NUM_INTERFACES] = {0};

/*----------------------------------------------------------------------------+
 | Global Variables                                                            |
 +----------------------------------------------------------------------------*/

extern __no_init tEDB __data16 tInputEndPointDescriptorBlock[];
extern __no_init tEDB __data16 tOutputEndPointDescriptorBlock[];


void HidCopyUsbToBuff (uint8_t* pEP, uint8_t* pCT, uint8_t);

/*----------------------------------------------------------------------------+
 | Functions' implementatin                                                    |
 +----------------------------------------------------------------------------*/

//resets internal HID data structure
void HidResetData ()
{
    int16_t i;

    //indicates which buffer is used by host to transmit data via OUT endpoint3 - X buffer is first
    //HidReadCtrl[intfIndex].bCurrentBufferXY = X_BUFFER;

    memset(&HidReadCtrl, 0, sizeof(HidReadCtrl));
    memset(&HidWriteCtrl, 0, sizeof(HidWriteCtrl));
    for (i = 0; i < HID_NUM_INTERFACES; i++){
        hidProtocol[i] = USB_REQ_HID_REPORT_PROTOCOL;
    }
}

//
//! \endcond
//

//*****************************************************************************
//
//! Sends a Data Report to the Host.
//!
//! \param reportData is an array containing the report.
//! \param intfNum is which HID interface the data should be transmitted
//! 	over.
//!
//! Sends a pre-built report \b reportData to the host, on interface
//! \b intfNum. The report must be organized to reflect the format defined
//! by the report descriptor in descriptors.c.
//! 
//! When the function returns \b USBHID_SEND_COMPLETE, the data has been written to
//! the USB transmit buffers, and will be transferred to the host in the next
//! polling frame. If the function returns \b USBHID_BUS_NOT_AVAILABLE, then the bus
//! has either been disconnected or the device is suspended, allowing no reports
//! to be sent. If the function returns \b USBHID_INTERFACE_BUSY_ERROR, it means the USB
//! buffer for the interface has data in it, suggesting that the host has not
//! yet fetched the previously-loaded report.
//!
//! \return Any of the following:
//! 		- \b USBHID_SEND_COMPLETE
//! 		- \b USBHID_BUS_NOT_AVAILABLE
//! 		- \b USBHID_INTERFACE_BUSY_ERROR
//
//*****************************************************************************

uint8_t USBHID_sendReport (const uint8_t * reportData, uint8_t intfNum)
{
    uint8_t byte_count;
    uint8_t * pEP1;
    uint8_t * pCT1;

    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        return (USBHID_BUS_NOT_AVAILABLE);
    }

    if (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY == X_BUFFER){
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].iep_X_Buffer;
        pCT1 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTX;
    } else {
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].iep_Y_Buffer;
        pCT1 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTY;
    }

    byte_count = report_len_input[INTFNUM_OFFSET(intfNum)];

    if (*pCT1 & EPBCNT_NAK){                                                        //if this EP is empty
        USB_TX_memcpy(pEP1, reportData, byte_count);                                //copy data into IEP X or Y buffer
        *pCT1 = byte_count;                                                         //Set counter for usb In-Transaction
        HidWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
            (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY + 1) & 0x01;    //switch buffer
        return (USBHID_SEND_COMPLETE);
    }
    return (USBHID_INTERFACE_BUSY_ERROR);
}

//*****************************************************************************
//
//! Receives a Report from the Host into \b reportData.
//!
//! \param reportData is an array containing the report.
//! \param intfNum is the HID interface over which the data is to be
//! 	received.
//!
//! Receives a report from the host into \b reportData, on interface
//! \b intfNum. It is expected that the host will organize the report in the
//! format defined by the report descriptor in descriptors.c.
//! 
//! When the function returns \b USBHID_RECEIVE_COMPLETED, the data has been
//! successfully copied from the USB receive buffers into \b reportData. If the
//! function returns \b USBHID_BUS_NOT_AVAILABLE, then the bus has either been
//! disconnected or the device is suspended, allowing no reports to be sent. If
//! the function returns \b USBHID_GENERAL_ERROR, it means the call failed for
//! unspecified reasons.
//! 
//! The call is intended to be called only when it is known that a report is in
//! the USB buffer. This means it is best called in response to the API calling
//! USBHID_handleDataReceived(), which indicates that a report has been received
//! for interface \b intfNum.
//! 
//! \return Any of the following:
//! 		- \b USBHID_RECEIVE_COMPLETED
//! 		- \b USBHID_BUS_NOT_AVAILABLE
//! 		- \b USBHID_INTERFACE_BUSY_ERROR
//
//*****************************************************************************

uint8_t USBHID_receiveReport (uint8_t * reportData, uint8_t intfNum)
{
    uint8_t ret = USBHID_GENERAL_ERROR;
    uint8_t nTmp1 = 0;

    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        return (USBHID_BUS_NOT_AVAILABLE);
    }

    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY == X_BUFFER){ //this is current buffer
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX & EPBCNT_NAK){ //this buffer has a valid data packet
            //this is the active EP buffer
            //pEP1
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;

            //second EP buffer
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
            nTmp1 = 1;                                                      //indicate that data is available
        }
    } else {                                                                //Y_BUFFER
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY & EPBCNT_NAK){
            //this is the active EP buffer
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;

            //second EP buffer
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
            nTmp1 = 1;                                                      //indicate that data is available
        }
    }

    if (nTmp1){
        //how many byte we can get from one endpoint buffer
        nTmp1 = *HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;

        if (nTmp1 & EPBCNT_NAK){
            nTmp1 = nTmp1 & 0x7f;                                           //clear NAK bit
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1;        //holds how many valid bytes in the EP buffer

            USB_RX_memcpy(reportData, HidReadCtrl[INTFNUM_OFFSET(
                                                      intfNum)].pCurrentEpPos,
                nTmp1);
            //memcpy(reportData, HidReadCtrl.pEP1, nTmp1);
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
                (HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY +
                 1) & 0x01;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = 0;
            *HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 = 0;                 //clear NAK, EP ready to receive data

            ret = USBHID_RECEIVE_COMPLETED;
        }
    }
    return (ret);
}

//*****************************************************************************
//
//! Initiates Sending of a User Buffer Over HID Interface.
//!
//! \param data is an array of data to be sent.
//! \param size is the number of bytes to be sent, starting from address
//! 	\b data.
//! \param intfNum is which data interface the \b data should be transmitted
//! 	over.
//!
//! Initiates sending of a user buffer over HID interface \b intfNum, of size
//! \b size and starting at address \b data. If \b size is larger than the
//! packet size, the function handles all packetization and buffer management.
//! \b size has no inherent upper limit (beyond being a 16-bit value).
//! 
//! In most cases where a send operation is successfully started, the function
//! will return \b USBHID_SEND_STARTED. A send operation is said to be underway. At
//! some point, either before or after the function returns, the send operation
//! will complete, barring any events that would preclude it. (Even if the
//! operation completes before the function returns, the return code will still
//! be \b USBHID_SEND_STARTED.)
//! 
//! If the bus is not connected when the function is called, the function
//! returns \b USBHID_BUS_NOT_AVAILABLE, and no operation is begun. If \b size is 0,
//! the function returns \b USBHID_GENERAL_ERROR. If a previous send operation is
//! already underway for this data interface, the function returns with
//! \b USBHID_INTERFACE_BUSY_ERROR.
//! 
//! USB includes low-level mechanisms that ensure valid transmission of data.
//! 
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a detailed discussion of
//! send operations.
//! 
//! \return Any of the following:
//! 		- \b USBHID_SEND_STARTED: a send operation was successfully
//! 			started.
//! 		- \b USBHID_INTERFACE_BUSY_ERROR: a previous send operation is
//! 			underway.
//! 		- \b USBHID_BUS_NOT_AVAILABLE: the bus is either suspended or
//! 			disconnected.
//! 		- \b USBHID_GENERAL_ERROR: \b size was zero, or other
//! 			errorkUSBHID_receiveCompleted.
//
//*****************************************************************************

uint8_t USBHID_sendData (const uint8_t* data, uint16_t size, uint8_t intfNum)
{
	uint16_t state;
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if (size == 0){
        return (USBHID_GENERAL_ERROR);
    }

    state = usbDisableInEndpointInterrupt(edbIndex);

    //atomic operation - disable interrupts

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //data can not be read because of USB suspended
    	usbRestoreInEndpointInterrupt(state);
        return (USBHID_BUS_NOT_AVAILABLE);
    }

    if (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft != 0){
        //the USB still sends previous data, we have to wait
    	usbRestoreInEndpointInterrupt(state);
        return (USBHID_INTERFACE_BUSY_ERROR);
    }

    //This function generate the USB interrupt. The data will be sent out from interrupt

    HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSend = size;
    HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft = size;
    HidWriteCtrl[INTFNUM_OFFSET(intfNum)].pHidBufferToSend = data;

    //trigger Endpoint Interrupt - to start send operation
    USBIEPIFG |= 1 << (edbIndex + 1);                                       //IEPIFGx;

    usbRestoreInEndpointInterrupt(state);

    return (USBHID_SEND_STARTED);
}

//
//! \cond
//

//this function is used only by USB interrupt
int16_t HidToHostFromBuffer (uint8_t intfNum)
{
    uint8_t byte_count, nTmp2;
    uint8_t * pEP1;
    uint8_t * pEP2;
    uint8_t * pCT1;
    uint8_t * pCT2;
    uint8_t bWakeUp = FALSE;                                                   //per default we do not wake up after interrupt

    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft == 0){    //do we have somtething to send?
        HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSend = 0;

        //call event callback function
        if (wUsbEventMask & USB_SEND_COMPLETED_EVENT){
            bWakeUp = USBHID_handleSendCompleted(intfNum);
        }
        return (bWakeUp);
    }

    if (!(tInputEndPointDescriptorBlock[edbIndex].bEPCNF & EPCNF_TOGGLE)){
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].iep_X_Buffer;
        pCT1 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTX;

        //second EP buffer
        pEP2 = (uint8_t*)stUsbHandle[intfNum].iep_Y_Buffer;
        pCT2 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTY;
    } else {
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].iep_Y_Buffer;
        pCT1 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTY;

        //second EP buffer
        pEP2 = (uint8_t*)stUsbHandle[intfNum].iep_X_Buffer;
        pCT2 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTX;
    }

    //how many byte we can send over one endpoint buffer
    //2 bytes a reserved: [0] - HID Report Descriptor, [1] - count of valid bytes
    byte_count =
        (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft >
         EP_MAX_PACKET_SIZE -
         2) ? EP_MAX_PACKET_SIZE -
        2 : HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft;
    nTmp2 = *pCT1;

    if (nTmp2 & EPBCNT_NAK){
        USB_TX_memcpy(pEP1 + 2, HidWriteCtrl[INTFNUM_OFFSET(
                                                 intfNum)].pHidBufferToSend,
            byte_count);                                                        //copy data into IEP3 X or Y buffer
        pEP1[0] = 0x3F;                                                         //set HID report descriptor: 0x3F
        pEP1[1] = byte_count;                                                   //set HID report descriptor

        //64 bytes will be send: we use only one HID report descriptor
        *pCT1 = 0x40;                                                           //Set counter for usb In-Transaction

        HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft -= byte_count;
        HidWriteCtrl[INTFNUM_OFFSET(intfNum)].pHidBufferToSend += byte_count;   //move buffer pointer

        //try to send data over second buffer
        nTmp2 = *pCT2;
        if ((HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft > 0) &&  //do we have more data to send?
            (nTmp2 & EPBCNT_NAK)){                                              //if the second buffer is free?
            //how many byte we can send over one endpoint buffer
            byte_count =
                (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft >
                 EP_MAX_PACKET_SIZE -
                 2) ? EP_MAX_PACKET_SIZE -
                2 : HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft;

            USB_TX_memcpy(pEP2 + 2, HidWriteCtrl[INTFNUM_OFFSET(
                                                     intfNum)].pHidBufferToSend,
                byte_count);                                                    //copy data into IEP3 X or Y buffer
            pEP2[0] = 0x3F;                                                     //set HID report descriptor: 0x3F
            pEP2[1] = byte_count;                                               //set byte count of valid data

            //64 bytes will be send: we use only one HID report descriptor
            *pCT2 = 0x40;                                                       //Set counter for usb In-Transaction

            HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft -=
                byte_count;
            HidWriteCtrl[INTFNUM_OFFSET(intfNum)].pHidBufferToSend +=
                byte_count;                                                     //move buffer pointer
        }
    }
    return (bWakeUp);
}

//
//! \endcond
//

//*****************************************************************************
//
//! Aborts an Active Send Operation on Data Interface.
//!
//! \param size is the number of bytes that were sent prior to the aboert
//! 	action.
//! \param intfNum is the data interface for which the send should be
//! 	aborted.
//!
//! Aborts an active send operation on data interface \b intfNum. Returns
//! the number of bytes that were sent prior to the abort, in \b size.
//! 
//! An application may choose to call this function if sending failed, due to
//! factors such as:
//! - a surprise removal of the bus
//! - a USB suspend event
//! - any send operation that extends longer than desired//! 
//! 
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USBHID_abortSend (uint16_t* size, uint8_t intfNum)
{
    uint8_t edbIndex;
    uint16_t state;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    state = usbDisableInEndpointInterrupt(edbIndex);

    *size =
        (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSend -
         HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft);
    HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSend = 0;
    HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft = 0;

    usbRestoreInEndpointInterrupt(state);
    return (USB_SUCCEED);
}

//
//! \cond
//

//This function copies data from OUT endpoint into user's buffer
//Arguments:
//pEP - pointer to EP to copy from
//pCT - pointer to pCT control reg
//
void HidCopyUsbToBuff (uint8_t* pEP, uint8_t* pCT,uint8_t intfNum)
{
    uint8_t nCount;

    //how many byte we can get from one endpoint buffer
    nCount =
        (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft >
         HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp) ? HidReadCtrl[
            INTFNUM_OFFSET(intfNum)].nBytesInEp : HidReadCtrl[INTFNUM_OFFSET(
                                                                  intfNum)].
        nBytesToReceiveLeft;

    USB_RX_memcpy(HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer, pEP, nCount);   //copy data from OEPx X or Y buffer
    HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft -= nCount;
    HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer += nCount;                     //move buffer pointer
    //to read rest of data next time from this place

    if (nCount == HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp){                 //all bytes are copied from receive buffer?
        //switch current buffer
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
            (HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY + 1) & 0x01;

        HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = 0;

        //clear NAK, EP ready to receive data
        *pCT = 0;
    } else {
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp -= nCount;
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos = pEP + nCount;
    }
}

//
//! \endcond
//

//*****************************************************************************
//
//! Receives \b size Bytes Over HID Interface.
//!
//! \param data is an array to contain the data received.
//! \param size is the number of bytes to be received.
//! \param intfNum is which data interface to receive from.
//!
//! Receives \b size bytes over HID interface \b intfNum into memory starting at
//! address \b data. \b size has no inherent upper limit (beyond being a 16-bit
//! value).
//! 
//! The function may return with \b USBHID_RECEIVE_STARTED, indicating that a
//! receive operation is underway. The operation completes when \b size bytes
//! are received. The application should ensure that the data memory buffer be
//! available during the whole of the receive operation.
//! 
//! The function may also return with \b USBHID_RECEIVE_COMPLETED. This means that
//! the receive operation was complete by the time the function returned.
//! 
//! If the bus is not connected when the function is called, the function
//! returns \b USBHID_BUS_NOT_AVAILABLE, and no operation is begun. If \b size is 0,
//! the function returns \b USBHID_GENERAL_ERROR. If a previous receive operation
//! is already underway for this data interface, the function returns
//! \b USBHID_INTERFACE_BUSY_ERROR.
//! 
//! USB includes low-level mechanisms that ensure valid transmission of data.
//! 
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a detailed discussion of
//! receive operations.
//!
//! \return Any of the following:
//! 		- \b USBHID_RECEIVE_STARTED: A receive operation has been
//! 			successfully started.
//! 		- \b USBHID_RECEIVE_COMPLETED: The receive operation is already 
//! 			completed.
//! 		- \b USBHID_INTERFACE_BUSY_ERROR: a previous receive operation is
//! 			underway.
//! 		- \b kUSBHID_ busNotAvailable: the bus is either suspended or
//! 			disconnected.
//! 		- \b USBHID_GENERAL_ERROR: size was zero, or other error.
//
//*****************************************************************************

uint8_t USBHID_receiveData (uint8_t* data, uint16_t size, uint8_t intfNum)
{
    uint8_t nTmp1;
    uint16_t state;
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if ((size == 0) ||                                                      //read size is 0
        (data == NULL)){
        return (USBHID_GENERAL_ERROR);
    }

    state = usbDisableOutEndpointInterrupt(edbIndex);

    //atomic operation - disable interrupts

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
    	usbRestoreOutEndpointInterrupt(state);
        return (USBHID_BUS_NOT_AVAILABLE);
    }

    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer != NULL){          //receive process already started
    	usbRestoreOutEndpointInterrupt(state);
        return (USBHID_RECEIVE_IN_PROGRESS);
    }

    HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceive = size;            //bytes to receive
    HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft = size;        //left bytes to receive
    HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = data;                //set user receive buffer

    //read rest of data from buffer, if any
    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > 0){
        //copy data from pEP-endpoint into User's buffer
        HidCopyUsbToBuff(HidReadCtrl[INTFNUM_OFFSET(
                                         intfNum)].pCurrentEpPos,
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1,intfNum);

        if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){ //the Receive opereation is completed
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;        //no more receiving pending
            USBHID_handleReceiveCompleted(intfNum);                         //call event handler in interrupt context
            usbRestoreOutEndpointInterrupt(state);
            return (USBHID_RECEIVE_COMPLETED);                              //receive completed
        }

        //check other EP buffer for data - exchange pCT1 with pCT2
        if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 ==
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX){
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
        } else {
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
        }
        nTmp1 = *HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;
        //try read data from second buffer
        if (nTmp1 & EPBCNT_NAK){                                            //if the second buffer has received data?
            nTmp1 = nTmp1 & 0x7f;                                           //clear NAK bit
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp =
                *(HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos + 1);  //holds how many valid bytes in the EP buffer
            if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > nTmp1 - 2){
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1 - 2;
            }
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos += 2;        //here starts user data
            HidCopyUsbToBuff(HidReadCtrl[INTFNUM_OFFSET(
                                             intfNum)].pCurrentEpPos,
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1,intfNum);
        }

        if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){ //the Receive opereation is completed
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;        //no more receiving pending
            if (wUsbEventMask & USB_RECEIVED_COMPLETED_EVENT){
                USBHID_handleReceiveCompleted(intfNum);                     //call event handler in interrupt context
            }
            usbRestoreOutEndpointInterrupt(state);
            return (USBHID_RECEIVE_COMPLETED);                              //receive completed
        }
    } //read rest of data from buffer, if any

    //read 'fresh' data, if available
    nTmp1 = 0;
    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY == X_BUFFER){ //this is current buffer
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX & EPBCNT_NAK){ //this buffer has a valid data packet
            //this is the active EP buffer
            //pEP1
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;

            //second EP buffer
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
            nTmp1 = 1;                                                      //indicate that data is available
        }
    } else {                                                                //Y_BUFFER
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY & EPBCNT_NAK){
            //this is the active EP buffer
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;

            //second EP buffer
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
            nTmp1 = 1;                                                      //indicate that data is available
        }
    }

    if (nTmp1){
        //how many byte we can get from one endpoint buffer
        nTmp1 = *HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;

        if (nTmp1 & EPBCNT_NAK){
            nTmp1 = nTmp1 & 0x7f;                                           //clear NAK bit
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp =
                *(HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos + 1);  //holds how many valid bytes in the EP buffer
            if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > nTmp1 - 2){
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1 - 2;
            }
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos += 2;        //here starts user data
            HidCopyUsbToBuff(HidReadCtrl[INTFNUM_OFFSET(
                                             intfNum)].pCurrentEpPos,
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1,intfNum);

            nTmp1 = *HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
            //try read data from second buffer
            if ((HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft >
                 0) &&                                                      //do we have more data to receive?
                (nTmp1 & EPBCNT_NAK)){                                      //if the second buffer has received data?
                nTmp1 = nTmp1 & 0x7f;                                       //clear NAK bit
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp =
                    *(HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 + 1);       //holds how many valid bytes in the EP buffer
                if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > nTmp1 -
                    2){
                    HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1 - 2;
                }
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 += 2;             //here starts user data
                HidCopyUsbToBuff(HidReadCtrl[INTFNUM_OFFSET(
                                                 intfNum)].pEP2,
                    HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2,intfNum);
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                    HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
            }
        }
    }

    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){     //the Receive opereation is completed
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;            //no more receiving pending
        if (wUsbEventMask & USB_RECEIVED_COMPLETED_EVENT){
            USBHID_handleReceiveCompleted(intfNum);                         //call event handler in interrupt context
        }
        usbRestoreOutEndpointInterrupt(state);
        return (USBHID_RECEIVE_COMPLETED);
    }

    //interrupts enable
    usbRestoreOutEndpointInterrupt(state);
    return (USBHID_RECEIVE_STARTED);
}

//
//! \cond
//

//this function is used only by USB interrupt.
//It fills user receiving buffer with received data
int16_t HidToBufferFromHost (uint8_t intfNum)
{
    uint8_t * pEP1;
    uint8_t nTmp1;
    uint8_t bWakeUp = FALSE;                                                   //per default we do not wake up after interrupt

    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){     //do we have somtething to receive?
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;            //no more receiving pending
        return (bWakeUp);
    }

    //No data to receive...
    if (!((tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX |
           tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY)
          & 0x80)){
        return (bWakeUp);
    }

    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY == X_BUFFER){ //X is current buffer
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;

        //second EP buffer
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
            (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
    } else {
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;

        //second EP buffer
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
            (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
    }

    //how many byte we can get from one endpoint buffer
    nTmp1 = *HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;

    if (nTmp1 & EPBCNT_NAK){
        nTmp1 = nTmp1 & 0x7f;                                                   //clear NAK bit
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = *(pEP1 + 1);          //holds how many valid bytes in the EP buffer
        if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > nTmp1 - 2){
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1 - 2;
        }
        pEP1 += 2;                                                              //here starts user data
        HidCopyUsbToBuff(pEP1, HidReadCtrl[INTFNUM_OFFSET(
                                               intfNum)].pCT1,intfNum);

        nTmp1 = *HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
        //try read data from second buffer
        if ((HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft > 0) &&   //do we have more data to send?
            (nTmp1 & EPBCNT_NAK)){                                              //if the second buffer has received data?
            nTmp1 = nTmp1 & 0x7f;                                               //clear NAK bit
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = *(pEP1 + 1);      //holds how many valid bytes in the EP buffer
            if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > nTmp1 - 2){
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1 - 2;
            }
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 += 2;                     //here starts user data
            HidCopyUsbToBuff(HidReadCtrl[INTFNUM_OFFSET(
                                             intfNum)].pEP2,
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2,intfNum);
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
        }
    }

    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){         //the Receive opereation is completed
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;                //no more receiving pending
        if (wUsbEventMask & USB_RECEIVED_COMPLETED_EVENT){
            bWakeUp = USBHID_handleReceiveCompleted(intfNum);
        }

        if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp){                   //Is not read data still available in the EP?
            if (wUsbEventMask & USB_DATA_RECEIVED_EVENT){
                bWakeUp = USBHID_handleDataReceived(intfNum);
            }
        }
    }
    return (bWakeUp);
}

//helper for USB interrupt handler
int16_t HidIsReceiveInProgress (uint8_t intfNum)
{
    return (HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer != NULL);
}

//
//! \endcond
//

//*****************************************************************************
//
//! Aborts an Active Recieve Operation on HID Interface.
//!
//! \param size is the number of bytes that were received and are waiting at the
//! 	assigned address.
//! \param intfNum is the data interface for which the receive should be
//! 	aborted.
//!
//! Aborts an active receive operation on HID interface \b intfNum. Returns the
//! number of bytes that were received and transferred to the data location
//! established for this receive operation. The data moved to the buffer up to
//! that time remains valid.
//! 
//! An application may choose to call this function if it decides it no longer
//! wants to receive data from the USB host. It should be noted that if a
//! continuous stream of data is being received from the host, aborting the
//! operation is akin to pressing a "pause" button; the host will be NAK'ed
//! until another receive operation is opened.
//! 
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a detailed discussion of
//! receive operations.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USBHID_abortReceive (uint16_t* size, uint8_t intfNum)
{
    uint16_t state;
    uint8_t edbIndex;
	
    edbIndex = stUsbHandle[intfNum].edb_Index;
    state = usbDisableOutEndpointInterrupt(edbIndex);

    *size = 0;                                                                  //set received bytes count to 0

    //is receive operation underway?
    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer){
        //how many bytes are already received?
        *size = HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceive -
                HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft;

        HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = 0;
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft = 0;
    }

    //restore interrupt status
    usbRestoreOutEndpointInterrupt(state);
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Rejects Data Received from the Host.
//!
//! This function rejects data that has been received from the host, for
//! interface \b intfNum, that does not have an active receive operation underway.
//! It resides in the USB endpoint buffer and blocks further data until a
//! receive operation is opened, or until rejected. When this function is
//! called, the buffer for this interface is purged, and the data lost. This
//! frees the USB path to resume communication.
//! 
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a detailed discussion of
//! receive operations.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USBHID_rejectData (uint8_t intfNum)
{
	uint16_t state;
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    state = usbDisableOutEndpointInterrupt(edbIndex);

    //interrupts disable

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if (bFunctionSuspended){
    	usbRestoreOutEndpointInterrupt(state);
        return (USBHID_BUS_NOT_AVAILABLE);
    }

    //Is receive operation underway?
    //- do not flush buffers if any operation still active.
    if (!HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer){
        uint8_t tmp1 = tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX &
                    EPBCNT_NAK;
        uint8_t tmp2 = tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY &
                    EPBCNT_NAK;

        if (tmp1 ^ tmp2){                                                       //switch current buffer if any and only ONE of the
                                                                                //buffers is full
            //switch current buffer
            HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
                (HidReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY +
                 1) & 0x01;
        }

        tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX = 0;                   //flush buffer X
        tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY = 0;                   //flush buffer Y
        HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = 0;                    //indicates that no more data available in the EP
    }

    usbRestoreOutEndpointInterrupt(state);
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Indicates the Status of the HID Interface.
//!
//! \param intfNum is the interface number for which the status is being
//! 	retrieved.
//! \param bytesSent If a send operation is underway, the number of bytes
//! 	that have been transferred to the host is returned in this location. If 
//! 	no send operation is underway, this returns zero.
//! \param bytesReceived If a receive operation is underway, the number of
//! 	bytes that have been transferred to the assigned memory location is
//! 	returned in this location. If no receive operation is underway, this
//! 	returns zero.
//!
//! Indicates the status of the HID interface \b intfNum. If a send operation is
//! active for this interface, the function also returns the number of bytes
//! that have been transmitted to the host. If a receive operation is active for
//! this interface, the function also returns the number of bytes that have been
//! received from the host and are waiting at the assigned address.
//! 
//! Because multiple flags can be returned, the possible values can be masked
//! together - for example, \b USBHID_WAITING_FOR_SEND + \b USBHID_DATA_WAITING.
//!
//! \return Any combination of the following:
//! 		- \b USBHID_WAITING_FOR_SEND: Indicates that a send operation is open
//! 			on this interface.
//! 		- \b USBHID_WAITING_FOR_RECEIVE: Indicates that a receive operation
//! 			is open on this interface.
//! 		- \b USBHID_DATA_WAITING: Indicates that data has been received from
//! 			the host for this interface, waiting in the USB receive buffers,
//! 			lacking an open receive operation to accept it.
//! 		- \b USBHID_BUS_NOT_AVAILABLE: Indicates that the bus is either
//! 			suspended or disconnected. Any operations that had previously
//! 			been underway are now aborted.
//
//*****************************************************************************

uint8_t USBHID_getInterfaceStatus (uint8_t intfNum, uint16_t* bytesSent, uint16_t* bytesReceived)
{
    uint8_t ret = 0;
    uint16_t stateIn, stateOut;
    uint8_t edbIndex;

    *bytesSent = 0;
    *bytesReceived = 0;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    stateIn = usbDisableInEndpointInterrupt(edbIndex);
    stateOut = usbDisableOutEndpointInterrupt(edbIndex);

    //Is send operation underway?
    if (HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft != 0){
        ret |= USBHID_WAITING_FOR_SEND;
        *bytesSent = HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSend -
                     HidWriteCtrl[INTFNUM_OFFSET(intfNum)].nHidBytesToSendLeft;
    }

    //Is receive operation underway?
    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer != NULL){
        ret |= USBHID_WAITING_FOR_RECEIVE;
        *bytesReceived = HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceive -
                         HidReadCtrl[INTFNUM_OFFSET(intfNum)].
                         nBytesToReceiveLeft;
    } else {                                                                    //not receive operation started
        //do not access USB memory if suspended (PLL off).
        //It may produce BUS_ERROR
        if (!bFunctionSuspended){
            if ((tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX &
                 EPBCNT_NAK)  |                                                 //any of buffers has a valid data packet
                (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY &
                 EPBCNT_NAK)){
                ret |= USBHID_DATA_WAITING;
            }
        }
    }

    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //if suspended or not enumerated  - report no other tasks pending
        ret = USBHID_BUS_NOT_AVAILABLE;
    }

    //restore interrupt status
    usbRestoreInEndpointInterrupt(stateIn);
    usbRestoreOutEndpointInterrupt(stateOut);

    return (ret);
}

//*****************************************************************************
//
//! Returns the Number of Bytes Waiting in the USB Endpoint Buffer.
//!
//! \param intfNum is the data interface whose buffer is to be checked.
//!
//! Returns the number of bytes waiting in the USB endpoint buffer for
//! \b intfNum. A non-zero value generally means that no receive operation is
//! open by which these bytes can be copied to a user buffer. If the value is
//! non-zero, the application should either open a receive operation so that the
//! data can be moved out of the endpoint buffer, or the data should be rejected
//! (USBHID_rejectData()).
//!
//! \return The number of bytes waiting in this buffer.
//
//*****************************************************************************

uint8_t USBHID_getBytesInUSBBuffer (uint8_t intfNum)
{
    uint8_t bTmp1 = 0;
    uint8_t bTmp2;

    uint8_t edbIndex;
    uint16_t state;


    edbIndex = stUsbHandle[intfNum].edb_Index;

    //interrupts disable
    state = usbDisableOutEndpointInterrupt(edbIndex);

    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //if suspended or not enumerated - report 0 bytes available
    	usbRestoreOutEndpointInterrupt(state);
        return (0);
    }

    if (HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > 0){                   //If a RX operation is underway, part of data may
                                                                                //was read of the OEP buffer
        bTmp1 = HidReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp;
        if (*HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 & EPBCNT_NAK){           //the next buffer has a valid data packet
            bTmp2 = *(HidReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 + 1);           //holds how many valid bytes in the EP buffer
            if (bTmp2 >
                (*HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 & 0x7F) - 2){       //check if all data received correctly
                bTmp1 +=
                    (*HidReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 & 0x7F) - 2;
            } else {
                bTmp1 += bTmp2;
            }
        }
    } else {
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX & EPBCNT_NAK){     //this buffer has a valid data packet
            bTmp2 = tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX & 0x7F;
            bTmp1 = *((uint8_t*)stUsbHandle[intfNum].oep_X_Buffer + 1);
            if (bTmp2 - 2 < bTmp1){                                             //check if the count (second byte) is valid
                bTmp1 = bTmp2 - 2;
            }
        }
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY & EPBCNT_NAK){     //this buffer has a valid data packet
            bTmp2 = tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY & 0x7F;
            if (bTmp2 - 2 > *((uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer + 1)){   //check if the count (second byte) is valid
                bTmp1 += *((uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer + 1);
            } else {
                bTmp1 += bTmp2 - 2;
            }
        }
    }

    //interrupts enable
    usbRestoreOutEndpointInterrupt(state);
    return (bTmp1);
}

//
//! \cond
//

#endif //ifdef _HID_

//
//! \endcond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
