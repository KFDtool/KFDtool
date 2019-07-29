/* 
 * ======== UsbCdc.h ========
 */

#ifndef _UsbCdc_H_
#define _UsbCdc_H_

#ifdef __cplusplus
extern "C"
{
#endif


/*----------------------------------------------------------------------------
 * The following function names and macro names are deprecated.  These were 
 * updated to new names to follow OneMCU naming convention.
 +---------------------------------------------------------------------------*/
#ifndef DEPRECATED
#define  kUSBCDC_sendStarted            USBCDC_SEND_STARTED
#define  kUSBCDC_sendComplete           USBCDC_SEND_COMPLETE
#define  kUSBCDC_intfBusyError          USBCDC_INTERFACE_BUSY_ERROR
#define  kUSBCDC_receiveStarted         USBCDC_RECEIVE_STARTED
#define  kUSBCDC_receiveCompleted       USBCDC_RECEIVE_COMPLETED
#define  kUSBCDC_receiveInProgress      USBCDC_RECEIVE_IN_PROGRESS
#define  kUSBCDC_generalError           USBCDC_GENERAL_ERROR
#define  kUSBCDC_busNotAvailable        USBCDC_BUS_NOT_AVAILABLE
#define  kUSBCDC_waitingForSend         USBCDC_WAITING_FOR_SEND
#define  kUSBCDC_waitingForReceive      USBCDC_WAITING_FOR_RECEIVE
#define  kUSBCDC_dataWaiting            USBCDC_DATA_WAITING
#define  kUSB_allCdcEvents              USBCDC_ALL_CDC_EVENTS
#define  kUSBCDC_noDataWaiting          USBCDC_NO_DATA_WAITING

#define USBCDC_intfStatus               USBCDC_getInterfaceStatus
#define USBCDC_bytesInUSBBuffer         USBCDC_getBytesInUSBBuffer
#endif


#define USBCDC_SEND_STARTED         0x01
#define USBCDC_SEND_COMPLETE        0x02
#define USBCDC_INTERFACE_BUSY_ERROR       0x03
#define USBCDC_RECEIVE_STARTED      0x04
#define USBCDC_RECEIVE_COMPLETED    0x05
#define USBCDC_RECEIVE_IN_PROGRESS   0x06
#define USBCDC_GENERAL_ERROR        0x07
#define USBCDC_BUS_NOT_AVAILABLE     0x08
//returned by USBCDC_rejectData() if no data pending
#define USBCDC_NO_DATA_WAITING        0X01
#define USBCDC_WAITING_FOR_SEND      0x01
#define USBCDC_WAITING_FOR_RECEIVE   0x02
#define USBCDC_DATA_WAITING         0x04
#define USBCDC_BUS_NOT_AVAILABLE     0x08
#define USBCDC_ALL_CDC_EVENTS           0xFF


/*----------------------------------------------------------------------------
 * These functions can be used in application
 +----------------------------------------------------------------------------*/

/*
 * Sends data over interface intfNum, of size size and starting at address data.
 * Returns:  USBCDC_SEND_STARTED
 *          USBCDC_SEND_COMPLETE
 *          USBCDC_INTERFACE_BUSY_ERROR
 */
uint8_t USBCDC_sendData (const uint8_t* data, uint16_t size, uint8_t intfNum);

/*
 * Receives data over interface intfNum, of size size, into memory starting at address data.
 */
uint8_t USBCDC_receiveData (uint8_t* data, uint16_t size, uint8_t intfNum);

/*
 * Aborts an active receive operation on interface intfNum.
 * size: the number of bytes that were received and transferred
 * to the data location established for this receive operation.
 */
uint8_t USBCDC_abortReceive (uint16_t* size, uint8_t intfNum);

/*
 * This function rejects payload data that has been received from the host.
 */
uint8_t USBCDC_rejectData (uint8_t intfNum);

/*
 * Aborts an active send operation on interface intfNum.  Returns the number of bytes that were sent prior to the abort, in size.
 */
uint8_t USBCDC_abortSend (uint16_t* size, uint8_t intfNum);

/*
 * This function indicates the status of the interface intfNum.
 * If a send operation is active for this interface,
 * the function also returns the number of bytes that have been transmitted to the host.
 * If a receiver operation is active for this interface, the function also returns
 * the number of bytes that have been received from the host and are waiting at the assigned address.
 *
 * returns USBCDC_WAITING_FOR_SEND (indicates that a call to USBCDC_SendData()
 * has been made, for which data transfer has not been completed)
 *
 * returns USBCDC_WAITING_FOR_RECEIVE (indicates that a receive operation
 * has been initiated, but not all data has yet been received)
 *
 * returns USBCDC_DATA_WAITING (indicates that data has been received
 * from the host, waiting in the USB receive buffers)
 */
uint8_t USBCDC_getInterfaceStatus (uint8_t intfNum, uint16_t* bytesSent, uint16_t* bytesReceived);

/*
 * Returns how many bytes are in the buffer are received and ready to be read.
 */
uint8_t USBCDC_getBytesInUSBBuffer (uint8_t intfNum);

/*----------------------------------------------------------------------------
 * Event-Handling routines
 +----------------------------------------------------------------------------*/

/*
 * This event indicates that data has been received for interface intfNum, but no data receive operation is underway.
 * returns TRUE to keep CPU awake
 */
uint8_t USBCDC_handleDataReceived (uint8_t intfNum);

/*
 * This event indicates that a send operation on interface intfNum has just been completed.
 * returns TRUE to keep CPU awake
 */
uint8_t USBCDC_handleSendCompleted (uint8_t intfNum);

/*
 * This event indicates that a receive operation on interface intfNum has just been completed.
 * returns TRUE to keep CPU awake
 */
uint8_t USBCDC_handleReceiveCompleted (uint8_t intfNum);

/*
 * Toggle state variable for CTS in USB Stack
 */
void USBCDC_setCTS(uint8_t state);

/*
 * This event indicates that a SetLineCoding request was received from the host and new values
 * for line coding paramters are available.
 *
 */
uint8_t USBCDC_handleSetLineCoding (uint8_t intfNum, uint32_t lBaudrate);

/*
 * This event indicates that a SetControlLineState request was received from the host. 
 * Basically new RTS and DTR states have been sent. Bit 0 of lineState is DTR and Bit 1 is RTS.
 *
 */
uint8_t USBCDC_handleSetControlLineState (uint8_t intfNum, uint8_t lineState);

/*----------------------------------------------------------------------------
 * These functions is to be used ONLY by USB stack, and not by application
 +----------------------------------------------------------------------------*/

/**
 * Send a packet with the settings of the second uart back to the usb host
 */
uint8_t usbGetLineCoding(void);

/**
 * Prepare EP0 to receive a packet with the settings for the second uart
 */
uint8_t usbSetLineCoding(void);

/**
 * Function set or reset RTS
 */
uint8_t usbSetControlLineState(void);

/**
 * Readout the settings (send from usb host) for the second uart
 */
uint8_t Handler_SetLineCoding(void);

#ifdef __cplusplus
}
#endif
#endif  //_UsbCdc_H_
