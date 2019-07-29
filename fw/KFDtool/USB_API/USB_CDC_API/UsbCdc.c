/** @file UsbCdc.c
 *  @brief Contains APIs related to CDC (Virtual COMport) device class
 */


//
//! \cond
//

/*
 * ======== UsbCdc.c ========
 */
#include <descriptors.h>

#ifdef _CDC_


#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_Common/usb.h"                  //USB-specific Data Structures
#include "../USB_CDC_API/UsbCdc.h"

#include <string.h>

//Local Macros
#define INTFNUM_OFFSET(X)   (X - CDC0_INTFNUM)  //Get the CDC offset

static struct _CdcControl {
    uint32_t lBaudrate;
    uint8_t bDataBits;
    uint8_t bStopBits;
    uint8_t bParity;
} CdcControl[CDC_NUM_INTERFACES];

static struct _CdcWrite {
    uint16_t nCdcBytesToSend;                       //holds counter of bytes to be sent
    uint16_t nCdcBytesToSendLeft;                   //holds counter how many bytes is still to be sent
    const uint8_t* pUsbBufferToSend;               //holds the buffer with data to be sent
    uint8_t bCurrentBufferXY;                      //is 0 if current buffer to write data is X, or 1 if current buffer is Y
    uint8_t bZeroPacketSent;                       //= FALSE;
    uint8_t last_ByteSend;
} CdcWriteCtrl[CDC_NUM_INTERFACES];

static struct _CdcRead {
    uint8_t *pUserBuffer;                          //holds the current position of user's receiving buffer. If NULL- no receiving
                                                //operation started
    uint8_t *pCurrentEpPos;                        //current positon to read of received data from curent EP
    uint16_t nBytesToReceive;                       //holds how many bytes was requested by receiveData() to receive
    uint16_t nBytesToReceiveLeft;                   //holds how many bytes is still requested by receiveData() to receive
    uint8_t * pCT1;                                //holds current EPBCTxx register
    uint8_t * pCT2;                                //holds next EPBCTxx register
    uint8_t * pEP2;                                //holds addr of the next EP buffer
    uint8_t nBytesInEp;                            //how many received bytes still available in current EP
    uint8_t bCurrentBufferXY;                      //indicates which buffer is used by host to transmit data via OUT endpoint3
} CdcReadCtrl[CDC_NUM_INTERFACES];

extern uint16_t wUsbEventMask;

//function pointers
extern void *(*USB_TX_memcpy)(void * dest, const void * source, size_t count);
extern void *(*USB_RX_memcpy)(void * dest, const void * source, size_t count);


/*----------------------------------------------------------------------------+
 | Global Variables                                                            |
 +----------------------------------------------------------------------------*/

extern __no_init tEDB __data16 tInputEndPointDescriptorBlock[];
extern __no_init tEDB __data16 tOutputEndPointDescriptorBlock[];


void CdcResetData ()
{
    int16_t i;

    //indicates which buffer is used by host to transmit data via OUT endpoint3 - X buffer is first
    //CdcReadCtrl[intfIndex].bCurrentBufferXY = X_BUFFER;

    memset(&CdcWriteCtrl, 0, sizeof(CdcWriteCtrl));
    memset(&CdcReadCtrl, 0, sizeof(CdcReadCtrl));
    memset(&CdcControl, 0, sizeof(CdcControl));

    for (i = 0; i < CDC_NUM_INTERFACES; i++){
        CdcControl[i].bDataBits = 8;
    }
}

//
//! \endcond
//

//*****************************************************************************
//
//! Begins a Send Operation to the USB Host.
//!
//! \param *data is an array of data to be sent.
//! \param size is the number of bytes to be sent, starting from address
//! 	\b data.
//! \param intfNum selects which data should be transmitted over.
//!
//! Initiates sending of a user buffer over CDC interface \b intfNum, of size
//! \b size and starting at address \b data. If \b size is larger than the
//! packet size, the function handles all packetization and buffer management.
//! \b size has no inherent upper limit (beyond being a 16-bit value).
//!
//! In most cases where a send operation is successfully started, the function
//! will return \b USBCDC_SEND_STARTED. A send operation is said to be underway. At
//! some point, either before or after the function returns, the send operation
//! will complete, barring any events that would preclude it. (Even if the
//! operation completes before the function returns, the return code will still
//! be \b USBCDC_SEND_STARTED.)
//! If the bus is not connected when the function is called, the function
//! returns \b USBCDC_BUS_NOT_AVAILABLE, and no operation is begun. If \b size is 0,
//! the function returns \b USBCDC_GENERAL_ERROR. If a previous send operation is
//! already underway for this data interface, the function returns with
//! \b USBCDC_INTERFACE_BUSY_ERROR.
//!
//! USB includes low-level mechanisms that ensure valid transmission of data.
//!
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a detailed discussion of
//! send operations.
//!
//! \return Any of the following:
//! 			- \b USBCDC_SEND_STARTED: a send operation was successfully
//! 				started
//! 			- \b USBCDC_INTERFACE_BUSY_ERROR: a previous send operation is
//! 				underway
//! 			- \b USBCDC_BUS_NOT_AVAILABLE: the bus is either suspended or
//! 				disconnected
//! 			- \b USBCDC_GENERAL_ERROR: \b size was zero, or other error
//
//*****************************************************************************

uint8_t USBCDC_sendData (const uint8_t* data, uint16_t size, uint8_t intfNum)
{
    uint8_t edbIndex;
    uint16_t state;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if (size == 0){
        return (USBCDC_GENERAL_ERROR);
    }

    state = usbDisableInEndpointInterrupt(edbIndex);

    //do not access USB memory if suspended (PLL uce BUS_ERROR
    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //data can not be read because of USB suspended
    	usbRestoreInEndpointInterrupt(state);                                            //restore interrupt status
        return (USBCDC_BUS_NOT_AVAILABLE);
    }

    if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft != 0){
        //the USB still sends previous data, we have to wait
    	usbRestoreInEndpointInterrupt(state);                                           //restore interrupt status
        return (USBCDC_INTERFACE_BUSY_ERROR);
    }

    //This function generate the USB interrupt. The data will be sent out from interrupt

    CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSend = size;
    CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft = size;
    CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].pUsbBufferToSend = data;

    //trigger Endpoint Interrupt - to start send operation
    USBIEPIFG |= 1 << (edbIndex + 1);                                       //IEPIFGx;

    usbRestoreInEndpointInterrupt(state);

    return (USBCDC_SEND_STARTED);
}

//
//! \cond
//

#define EP_MAX_PACKET_SIZE_CDC      0x40

//this function is used only by USB interrupt
int16_t CdcToHostFromBuffer (uint8_t intfNum)
{
    uint8_t byte_count, nTmp2;
    uint8_t * pEP1;
    uint8_t * pEP2;
    uint8_t * pCT1;
    uint8_t * pCT2;
    uint8_t bWakeUp = FALSE;                                                   //TRUE for wake up after interrupt
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft == 0){    //do we have somtething to send?
        if (!CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bZeroPacketSent){        //zero packet was not yet sent
            CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bZeroPacketSent = TRUE;

            if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].last_ByteSend ==
                EP_MAX_PACKET_SIZE_CDC){
                if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY ==
                    X_BUFFER){
                    if (tInputEndPointDescriptorBlock[edbIndex].bEPBCTX &
                        EPBCNT_NAK){
                        tInputEndPointDescriptorBlock[edbIndex].bEPBCTX = 0;
                        CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY
                            = Y_BUFFER;                                     //switch buffer
                    }
                } else {
                    if (tInputEndPointDescriptorBlock[edbIndex].bEPBCTY &
                        EPBCNT_NAK){
                        tInputEndPointDescriptorBlock[edbIndex].bEPBCTY = 0;
                        CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY
                            = X_BUFFER;                                     //switch buffer
                    }
                }
            }

            CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSend = 0;      //nothing to send

            //call event callback function
            if (wUsbEventMask & USB_SEND_COMPLETED_EVENT){
                bWakeUp = USBCDC_handleSendCompleted(intfNum);
            }
        } //if (!bSentZeroPacket)

        return (bWakeUp);
    }

    CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bZeroPacketSent = FALSE;          //zero packet will be not sent: we have data

    if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY == X_BUFFER){
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
    byte_count =
        (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft >
         EP_MAX_PACKET_SIZE_CDC) ? EP_MAX_PACKET_SIZE_CDC : CdcWriteCtrl[
            INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft;
    nTmp2 = *pCT1;

    if (nTmp2 & EPBCNT_NAK){
        USB_TX_memcpy(pEP1, CdcWriteCtrl[INTFNUM_OFFSET(
                                             intfNum)].pUsbBufferToSend,
            byte_count);                                                            //copy data into IEP3 X or Y buffer
        *pCT1 = byte_count;                                                         //Set counter for usb In-Transaction
        CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
            (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY + 1) & 0x01;    //switch buffer
        CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft -= byte_count;
        CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].pUsbBufferToSend += byte_count;       //move buffer pointer
        CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].last_ByteSend = byte_count;

        //try to send data over second buffer
        nTmp2 = *pCT2;
        if ((CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft > 0) &&      //do we have more data to send?
            (nTmp2 & EPBCNT_NAK)){                                                  //if the second buffer is free?
            //how many byte we can send over one endpoint buffer
            byte_count =
                (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft >
                 EP_MAX_PACKET_SIZE_CDC) ? EP_MAX_PACKET_SIZE_CDC :
                CdcWriteCtrl[
                    INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft;

            USB_TX_memcpy(pEP2, CdcWriteCtrl[INTFNUM_OFFSET(
                                                 intfNum)].pUsbBufferToSend,
                byte_count);                                                        //copy data into IEP3 X or Y buffer
            *pCT2 = byte_count;                                                     //Set counter for usb In-Transaction
            CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
                (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY +
                 1) & 0x01;                                                         //switch buffer
            CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft -=
                byte_count;
            CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].pUsbBufferToSend +=
                byte_count;                                                         //move buffer pointer
            CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].last_ByteSend = byte_count;
        }
    }
    return (bWakeUp);
}

//
//! \endcond
//

//*****************************************************************************
//
//! Aborts an Active Send Operation.
//!
//! \param size is the number of bytes that were sent prior to the abort action.
//! \param intfNum is the data interface for which the send should be aborted.
//!
//! Aborts an active send operation on data interface \b intfNum. Returns the
//! number of bytes that were sent prior to the abort, in \b size.
//!
//! An application may choose to call this function if sending failed, due to
//! factors such as:
//! 		- a surprise removal of the bus
//! 		- a USB suspend event
//! 		- any send operation that extends longer than desired (perhaps due
//! 			to no open COM port on the host.)
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USBCDC_abortSend (uint16_t* size, uint8_t intfNum)
{
    uint8_t edbIndex;
    uint16_t state;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    state = usbDisableInEndpointInterrupt(edbIndex);                                                         //disable interrupts - atomic operation

    *size =
        (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSend -
         CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft);
    CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSend = 0;
    CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft = 0;

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
void CopyUsbToBuff (uint8_t* pEP, uint8_t* pCT, uint8_t intfNum)
{
    uint8_t nCount;

    //how many byte we can get from one endpoint buffer
    nCount =
        (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft >
         CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp) ? CdcReadCtrl[
            INTFNUM_OFFSET(intfNum)].nBytesInEp : CdcReadCtrl[INTFNUM_OFFSET(
                                                                  intfNum)].
        nBytesToReceiveLeft;

    USB_RX_memcpy(CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer, pEP, nCount);   //copy data from OEP3 X or Y buffer
    CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft -= nCount;
    CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer += nCount;                     //move buffer pointer
    //to read rest of data next time from this place

    if (nCount == CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp){                 //all bytes are copied from receive buffer?
        //switch current buffer
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
            (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY + 1) & 0x01;

        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = 0;

        //clear NAK, EP ready to receive data
        *pCT = 0x00;
    } else {
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp -= nCount;
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos = pEP + nCount;
    }
}

//
//! \endcond
//

//*****************************************************************************
//
//! Begins a Receive Operation from the USB Host.
//!
//! \param *data is an array to contain the data received.
//! \param size is the number of bytes to be received.
//! \param intfNum is which data interface to receive from.
//!
//! Receives \b size bytes over CDC interface \b intfNum into memory starting at
//! address \b data. \b size has no inherent upper limit (beyond being a 16-bit
//! value).
//!
//! The function may return with \b USBCDC_RECEIVE_STARTED, indicating that a
//! receive operation is underway. The operation completes when \b size bytes
//! are received. The application should ensure that the data memory buffer be
//! available during the whole of the receive operation.
//!
//! The function may also return with \b USBCDC_RECEIVE_COMPLETED. This means that
//! the receive operation was complete by the time the function returned.
//!
//! If the bus is not connected when the function is called, the function
//! returns \b USBCDC_BUS_NOT_AVAILABLE, and no operation is begun. If \b size is 0,
//! the function returns \b USBCDC_GENERAL_ERROR. If a previous receive operation
//! is already underway for this data interface, the function returns
//! \b USBCDC_INTERFACE_BUSY_ERROR.
//!
//! USB includes low-level mechanisms that ensure valid transmission of data.
//!
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a detailed discussion of
//! receive operations.
//!
//! \return Any of the following:
//! 		- \b USBCDC_RECEIVE_STARTED: A receive operation has been
//! 			succesfully started.
//! 		- \b USBCDC_RECEIVE_COMPLETED: The receive operation is already
//! 			completed.
//! 		- \b USBCDC_INTERFACE_BUSY_ERROR: a previous receive operation is
//! 			underway.
//! 		- \b USBCDC_BUS_NOT_AVAILABLE: the bus is either suspended or
//! 			disconnected.
//! 		- \b USBCDC_GENERAL_ERROR: \b size was zero, or other error.
//
//*****************************************************************************

uint8_t USBCDC_receiveData (uint8_t* data, uint16_t size, uint8_t intfNum)
{
    uint8_t nTmp1;
    uint8_t edbIndex;
    uint16_t state;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if ((size == 0) ||                                                      //read size is 0
        (data == NULL)){
        return (USBCDC_GENERAL_ERROR);
    }

    state = usbDisableOutEndpointInterrupt(edbIndex);
    //atomic operation - disable interrupts

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //data can not be read because of USB suspended
    	usbRestoreOutEndpointInterrupt(state);
        return (USBCDC_BUS_NOT_AVAILABLE);
    }

    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer != NULL){          //receive process already started
    	usbRestoreOutEndpointInterrupt(state);
        return (USBCDC_INTERFACE_BUSY_ERROR);
    }

    CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceive = size;            //bytes to receive
    CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft = size;        //left bytes to receive
    CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = data;                //set user receive buffer

    //read rest of data from buffer, if any
    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > 0){
        //copy data from pEP-endpoint into User's buffer
        CopyUsbToBuff(CdcReadCtrl[INTFNUM_OFFSET(
                                      intfNum)].pCurrentEpPos,
            CdcReadCtrl[INTFNUM_OFFSET(
                            intfNum)
            ].pCT1, intfNum);

        if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){ //the Receive opereation is completed
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;        //no more receiving pending
            if (wUsbEventMask & USB_RECEIVED_COMPLETED_EVENT){
                USBCDC_handleReceiveCompleted(intfNum);                     //call event handler in interrupt context
            }
            usbRestoreOutEndpointInterrupt(state);
            return (USBCDC_RECEIVE_COMPLETED);                              //receive completed
        }

        //check other EP buffer for data - exchange pCT1 with pCT2
        if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 ==
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX){
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
        } else {
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
        }

        nTmp1 = *CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;
        //try read data from second buffer
        if (nTmp1 & EPBCNT_NAK){                                            //if the second buffer has received data?
            nTmp1 = nTmp1 & 0x7f;                                           //clear NAK bit
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1;        //holds how many valid bytes in the EP buffer
            CopyUsbToBuff(CdcReadCtrl[INTFNUM_OFFSET(
                                          intfNum)].pCurrentEpPos,
                CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1, intfNum);
        }

        if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){ //the Receive opereation is completed
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;        //no more receiving pending
            if (wUsbEventMask & USB_RECEIVED_COMPLETED_EVENT){
                USBCDC_handleReceiveCompleted(intfNum);                     //call event handler in interrupt context
            }
            usbRestoreOutEndpointInterrupt(state);
            return (USBCDC_RECEIVE_COMPLETED);                              //receive completed
        }
    } //read rest of data from buffer, if any

    //read 'fresh' data, if available
    nTmp1 = 0;
    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY == X_BUFFER){ //this is current buffer
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX & EPBCNT_NAK){ //this buffer has a valid data packet
            //this is the active EP buffer
            //pEP1
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;

            //second EP buffer
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
            nTmp1 = 1;                                                      //indicate that data is available
        }
    } else {                                                                //Y_BUFFER
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY & EPBCNT_NAK){
            //this is the active EP buffer
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCurrentEpPos =
                (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;

            //second EP buffer
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
                (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
            nTmp1 = 1;                                                      //indicate that data is available
        }
    }

    if (nTmp1){
        //how many byte we can get from one endpoint buffer
        nTmp1 = *CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;
        while (nTmp1 == 0)
        {
            nTmp1 = *CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;
        }

        if (nTmp1 & EPBCNT_NAK){
            nTmp1 = nTmp1 & 0x7f;                                           //clear NAK bit
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1;        //holds how many valid bytes in the EP buffer

            CopyUsbToBuff(CdcReadCtrl[INTFNUM_OFFSET(
                                          intfNum)].pCurrentEpPos,
                CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1, intfNum);

            nTmp1 = *CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
            //try read data from second buffer
            if ((CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft >
                 0) &&                                                      //do we have more data to send?
                (nTmp1 & EPBCNT_NAK)){                                      //if the second buffer has received data?
                nTmp1 = nTmp1 & 0x7f;                                       //clear NAK bit
                CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1;    //holds how many valid bytes in the EP buffer
                CopyUsbToBuff(CdcReadCtrl[INTFNUM_OFFSET(
                                              intfNum)].pEP2,
                    CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2, intfNum);
                CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                    CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
            }
        }
    }

    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){     //the Receive opereation is completed
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;            //no more receiving pending
        if (wUsbEventMask & USB_RECEIVED_COMPLETED_EVENT){
            USBCDC_handleReceiveCompleted(intfNum);                         //call event handler in interrupt context
        }
        usbRestoreOutEndpointInterrupt(state);
        return (USBCDC_RECEIVE_COMPLETED);
    }

    //interrupts enable
    usbRestoreOutEndpointInterrupt(state);
    return (USBCDC_RECEIVE_STARTED);
}

//
//! \cond
//

//this function is used only by USB interrupt.
//It fills user receiving buffer with received data
int16_t CdcToBufferFromHost (uint8_t intfNum)
{
    uint8_t * pEP1;
    uint8_t nTmp1;
    uint8_t bWakeUp = FALSE;                                                   //per default we do not wake up after interrupt

    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){     //do we have somtething to receive?
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;            //no more receiving pending
        return (bWakeUp);
    }

    //No data to receive...
    if (!((tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX |
           tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY)
          & 0x80)){
        return (bWakeUp);
    }

    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY == X_BUFFER){ //X is current buffer
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;

        //second EP buffer
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
            (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
    } else {
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;

        //second EP buffer
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pEP2 =
            (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 =
            &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
    }

    //how many byte we can get from one endpoint buffer
    nTmp1 = *CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1;

    if (nTmp1 & EPBCNT_NAK){
        nTmp1 = nTmp1 & 0x7f;                                                   //clear NAK bit
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1;                //holds how many valid bytes in the EP buffer

        CopyUsbToBuff(pEP1, CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1, intfNum);

        nTmp1 = *CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
        //try read data from second buffer
        if ((CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft > 0) &&   //do we have more data to send?
            (nTmp1 & EPBCNT_NAK)){                                              //if the second buffer has received data?
            nTmp1 = nTmp1 & 0x7f;                                               //clear NAK bit
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = nTmp1;            //holds how many valid bytes in the EP buffer
            CopyUsbToBuff(CdcReadCtrl[INTFNUM_OFFSET(
                                          intfNum)].pEP2,
                CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2, intfNum);
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT1 =
                CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2;
        }
    }

    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft == 0){         //the Receive opereation is completed
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;                //no more receiving pending
        if (wUsbEventMask & USB_RECEIVED_COMPLETED_EVENT){
            bWakeUp |= USBCDC_handleReceiveCompleted(intfNum);
        }

        if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp){                   //Is not read data still available in the EP?
            if (wUsbEventMask & USB_DATA_RECEIVED_EVENT){
                bWakeUp |= USBCDC_handleDataReceived(intfNum);
            }
        }
    }
    return (bWakeUp);
}

//helper for USB interrupt handler
int16_t CdcIsReceiveInProgress (uint8_t intfNum)
{
    return (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer != NULL);
}

//
//! \endcond
//

//*****************************************************************************
//
//! Aborts an Active Receive Operation.
//!
//! \param *size is the number of bytes that were received and are waiting
//! 		at the assigned address.
//! \param intfNum is the data interface for which the send should be
//! 		aborted.
//!
//! Aborts an active receive operation on CDC interface \b intfNum. Returns the
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

uint8_t USBCDC_abortReceive (uint16_t* size, uint8_t intfNum)
{
    //interrupts disable
	uint8_t edbIndex;
	uint16_t state;

    edbIndex = stUsbHandle[intfNum].edb_Index;
    state = usbDisableOutEndpointInterrupt(edbIndex);

    *size = 0;                                                              //set received bytes count to 0

    //is receive operation underway?
    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer){
        //how many bytes are already received?
        *size = CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceive -
                CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft;

        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = 0;
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer = NULL;
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceiveLeft = 0;
    }

    //restore interrupt status
    usbRestoreOutEndpointInterrupt(state);
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Rejects the Data Received from the Host.
//!
//! This function rejects data that has been received from the host, for
//! interface inftNum, that does not have an active receive operation underway.
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

uint8_t USBCDC_rejectData (uint8_t intfNum)
{
    uint8_t edbIndex;
    uint16_t state;

    edbIndex = stUsbHandle[intfNum].edb_Index;
    state = usbDisableOutEndpointInterrupt(edbIndex);

    //atomic operation - disable interrupts

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if (bFunctionSuspended){
    	usbRestoreOutEndpointInterrupt(state);
        return (USBCDC_BUS_NOT_AVAILABLE);
    }

    //Is receive operation underway?
    //- do not flush buffers if any operation still active.
    if (!CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer){
        uint8_t tmp1 = tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX &
                    EPBCNT_NAK;
        uint8_t tmp2 = tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY &
                    EPBCNT_NAK;

        if (tmp1 ^ tmp2){                                                   //switch current buffer if any and only ONE of buffers
                                                                            //is full
            //switch current buffer
            CdcReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY =
                (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].bCurrentBufferXY +
                 1) & 0x01;
        }

        tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX = 0;               //flush buffer X
        tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY = 0;               //flush buffer Y
        CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp = 0;                //indicates that no more data available in the EP
    }

    usbRestoreOutEndpointInterrupt(state);
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Indicates the Status of the CDC Interface.
//!
//! \param intfNum is the interface number for which status is being retrieved.
//! \param bytesSent If a send operation is underway, the number of bytes that
//! 	send have been transferred to the host is returned in this location. If
//! 	no operation is underway, this returns zero.
//! \param bytesReceived If a receive operation is underway, the number of bytes
//! 	that have been transferred to the assigned memory location is returned
//! 	in this location. If no receive operation is underway, this returns
//! 	zero.
//!
//! Indicates the status of the CDC interface \b intfNum. If a send operation is
//! active for this interface, the function also returns the number of bytes
//! that have been transmitted to the host. If a receive operation is active for
//! this interface, the function also returns the number of bytes that have been
//! received from the host and are waiting at the assigned address.
//!
//! Because multiple flags can be returned, the possible values can be masked
//! together - for example, \b USBCDC_WAITING_FOR_SEND + \b USBCDC_DATA_WAITING.
//!
//! \return Any combination of the following:
//! 		- \b USBCDC_WAITING_FOR_SEND: Indicates that a send operation is
//! 			open ont his interface
//! 		- \b USBCDC_WAITING_FOR_RECEIVE: Indicates that a receive operation
//! 			is open on this interface
//! 		- \b USBCDC_DATA_WAITING: Indicates that data has been received
//! 			from the host for this interface, waiting in the USB receive
//! 			buffers, lacking an open receive operation to accept it.
//! 		- \b USBCDC_BUS_NOT_AVAILABLE: Indicates that the bus is either
//! 			suspended or disconnected. Any operations that had previously
//! 			been underway are now aborted.
//
//*****************************************************************************

uint8_t USBCDC_getInterfaceStatus (uint8_t intfNum, uint16_t* bytesSent, uint16_t* bytesReceived)
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
    if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft != 0){
        ret |= USBCDC_WAITING_FOR_SEND;
        *bytesSent = CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSend -
                     CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].nCdcBytesToSendLeft;
    }

    //Is receive operation underway?
    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer != NULL){
        ret |= USBCDC_WAITING_FOR_RECEIVE;
        *bytesReceived = CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesToReceive -
                         CdcReadCtrl[INTFNUM_OFFSET(intfNum)].
                         nBytesToReceiveLeft;
    } else {                                                                //receive operation not started
        //do not access USB memory if suspended (PLL off).
        //It may produce BUS_ERROR
        if (!bFunctionSuspended){
            if ((tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX &
                 EPBCNT_NAK)  |                                             //any of buffers has a valid data packet
                (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY &
                 EPBCNT_NAK)){
                ret |= USBCDC_DATA_WAITING;
            }
        }
    }

    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //if suspended or not enumerated - report no other tasks pending
        ret = USBCDC_BUS_NOT_AVAILABLE;
    }

    //restore interrupt status
    usbRestoreInEndpointInterrupt(stateIn);
    usbRestoreOutEndpointInterrupt(stateOut);

    __no_operation();
    return (ret);
}

//*****************************************************************************
//
//! Gives the Number of Bytes in the USB Endpoint Buffer.
//!
//! \param intfNum is the data interface whose buffer is to be checked.
//!
//! Returns the number of bytes waiting in the USB endpoint buffer for
//! \b intfNum. A non-zero value generally means that no receive operation is
//! open by which these bytes can be copied to a user buffer. If the value is
//! non-zero, the application should either open a receive operation so that the
//! data can be moved out of the endpoint buffer, or the data should be rejected
//! (USBCDC_rejectData()).
//!
//! \return The number of bytes waiting in this buffer.
//
//*****************************************************************************

uint8_t USBCDC_getBytesInUSBBuffer (uint8_t intfNum)
{
    uint8_t bTmp1 = 0;
    uint16_t state;
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;

    state = usbDisableOutEndpointInterrupt(edbIndex);
    //atomic operation - disable interrupts

    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
    	usbRestoreOutEndpointInterrupt(state);
        //if suspended or not enumerated - report 0 bytes available
        return (0);
    }

    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp > 0){               //If a RX operation is underway, part of data may
                                                                            //was read of the OEP buffer
        bTmp1 = CdcReadCtrl[INTFNUM_OFFSET(intfNum)].nBytesInEp;
        if (*CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 & EPBCNT_NAK){       //the next buffer has a valid data packet
            bTmp1 += *CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pCT2 & 0x7F;
        }
    } else {
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX & EPBCNT_NAK){ //this buffer has a valid data packet
            bTmp1 = tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX & 0x7F;
        }
        if (tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY & EPBCNT_NAK){ //this buffer has a valid data packet
            bTmp1 += tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY & 0x7F;
        }
    }

    usbRestoreOutEndpointInterrupt(state);
    return (bTmp1);
}

//
//! \cond
//

//----------------------------------------------------------------------------
//Line Coding Structure
//dwDTERate     | 4 | Data terminal rate, in bits per second
//bCharFormat   | 1 | Stop bits, 0 = 1 Stop bit, 1 = 1,5 Stop bits, 2 = 2 Stop bits
//bParityType   | 1 | Parity, 0 = None, 1 = Odd, 2 = Even, 3= Mark, 4 = Space
//bDataBits     | 1 | Data bits (5,6,7,8,16)
//----------------------------------------------------------------------------
uint8_t usbGetLineCoding (void)
{
    uint8_t infIndex;

    if(tSetupPacket.wIndex % 2)
    {
        infIndex = (tSetupPacket.wIndex-1) / 2;
    }
    else
    {
        infIndex = (tSetupPacket.wIndex) / 2;
    }

    abUsbRequestReturnData[6] =
        CdcControl[infIndex].bDataBits;          //Data bits = 8
    abUsbRequestReturnData[5] =
        CdcControl[infIndex].bParity;            //No Parity
    abUsbRequestReturnData[4] =
        CdcControl[infIndex].bStopBits;          //Stop bits = 1

    abUsbRequestReturnData[3] =
        CdcControl[infIndex].lBaudrate >> 24;
    abUsbRequestReturnData[2] =
        CdcControl[infIndex].lBaudrate >> 16;
    abUsbRequestReturnData[1] =
        CdcControl[infIndex].lBaudrate >> 8;
    abUsbRequestReturnData[0] =
        CdcControl[infIndex].lBaudrate;

    wBytesRemainingOnIEP0 = 0x07;                                           //amount of data to be send over EP0 to host
    usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);              //send data to host

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetLineCoding (void)
{
    usbReceiveDataPacketOnEP0((uint8_t*)&abUsbRequestIncomingData);            //receive data over EP0 from Host

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetControlLineState (void)
{
	USBCDC_handleSetControlLineState((uint8_t)tSetupPacket.wIndex,
            (uint8_t)tSetupPacket.wValue);
    usbSendZeroLengthPacketOnIEP0();                                        //Send ZLP for status stage

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t Handler_SetLineCoding (void)
{
    uint8_t bWakeUp;
    volatile uint8_t infIndex;

    if(tSetupPacket.wIndex % 2)
    {
        infIndex = (tSetupPacket.wIndex-1) / 2;
    }
    else
    {
        infIndex = (tSetupPacket.wIndex) / 2;
    }

    //Baudrate Settings

    CdcControl[infIndex].lBaudrate =
        (uint32_t)abUsbRequestIncomingData[3] << 24 |
        (uint32_t)abUsbRequestIncomingData[2] << 16 |
        (uint32_t)
        abUsbRequestIncomingData[1] << 8 | abUsbRequestIncomingData[0];
    bWakeUp =
        USBCDC_handleSetLineCoding(tSetupPacket.wIndex,
            CdcControl[infIndex].lBaudrate);

    return (bWakeUp);
}

#endif  //ifdef _CDC_

//
//! \endcond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
