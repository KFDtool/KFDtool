/* 
 * ======== UsbHid.h ========
 */
#include <stdint.h>

#ifndef _UsbHid_H_
#define _UsbHid_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*----------------------------------------------------------------------------
 * The following function names and macro names are deprecated.  These were 
 * updated to new names to follow OneMCU naming convention.
 +---------------------------------------------------------------------------*/
#ifndef DEPRECATED
#define  kUSBHID_sendStarted          USBHID_SEND_STARTED
#define  kUSBHID_sendComplete         USBHID_SEND_COMPLETE
#define  kUSBHID_intfBusyError        USBHID_INTERFACE_BUSY_ERROR
#define  kUSBHID_receiveStarted       USBHID_RECEIVE_STARTED
#define  kUSBHID_receiveCompleted     USBHID_RECEIVE_COMPLETED
#define  kUSBHID_receiveInProgress    USBHID_RECEIVE_IN_PROGRESS
#define  kUSBHID_generalError         USBHID_GENERAL_ERROR
#define  kUSBHID_busNotAvailable      USBHID_BUS_NOT_AVAILABLE
#define  kUSBHID_waitingForSend       USBHID_WAITING_FOR_SEND
#define  kUSBHID_waitingForReceive    USBHID_WAITING_FOR_RECEIVE
#define  kUSBHID_dataWaiting          USBHID_DATA_WAITING
#define  kUSB_allHidEvents            USBHID_ALL_HID_EVENTS
#define  kUSBHID_noDataWaiting        USBHID_NO_DATA_WAITING

#define   USBHID_intfStatus           USBHID_getInterfaceStatus
#define   USBHID_bytesInUSBBuffer     USBHID_getBytesInUSBBuffer

#endif



#define USBHID_SEND_STARTED         0x01
#define USBHID_SEND_COMPLETE        0x02
#define USBHID_INTERFACE_BUSY_ERROR       0x03
#define USBHID_RECEIVE_STARTED      0x04
#define USBHID_RECEIVE_COMPLETED    0x05
#define USBHID_RECEIVE_IN_PROGRESS   0x06
#define USBHID_GENERAL_ERROR        0x07
#define USBHID_BUS_NOT_AVAILABLE     0x08
#define HID_BOOT_PROTOCOL       0x00
#define HID_REPORT_PROTOCOL     0x01
//returned by USBHID_rejectData() if no data pending
#define USBHID_NO_DATA_WAITING        1 
#define USBHID_WAITING_FOR_SEND      0x01
#define USBHID_WAITING_FOR_RECEIVE   0x02
#define USBHID_DATA_WAITING         0x04
#define USBHID_BUS_NOT_AVAILABLE     0x08
#define USBHID_ALL_HID_EVENTS           0xFF

#define USBHID_handleGetReport USBHID_handleEP0GetReport
#define USBHID_handleSetReport USBHID_handleEP0SetReport
#define USBHID_handleSetReportDataAvailable USBHID_handleEP0SetReportDataAvailable
#define USBHID_handleSetReportDataAvailable  USBHID_handleEP0SetReportDataAvailable


/*----------------------------------------------------------------------------
 * These functions can be used in application
 +----------------------------------------------------------------------------*/

/*
 * Sends a pre-built report reportData to the host.
 * Returns:  USBHID_SEND_COMPLETE
 *          USBHID_INTERFACE_BUSY_ERROR
 *          kUSBHID_busSuspended
 */
extern uint8_t USBHID_sendReport (const uint8_t * reportData, uint8_t intfNum);

/*
 * Receives report reportData from the host.
 * Return:     USBHID_RECEIVE_COMPLETED
 *          USBHID_GENERAL_ERROR
 *          kUSBHID_busSuspended
 */
uint8_t USBHID_receiveReport (uint8_t * reportData, uint8_t intfNum);

/*
 * Sends data over interface intfNum, of size size and starting at address data.
 * Returns:  USBHID_SEND_STARTED
 *          USBHID_SEND_COMPLETE
 *          USBHID_INTERFACE_BUSY_ERROR
 */
uint8_t USBHID_sendData (const uint8_t* data, uint16_t size, uint8_t intfNum);

/*
 * Receives data over interface intfNum, of size size, into memory starting at address data.
 */
uint8_t USBHID_receiveData (uint8_t* data, uint16_t size, uint8_t intfNum);

/*
 * Aborts an active receive operation on interface intfNum.
 * size: the number of bytes that were received and transferred
 * to the data location established for this receive operation.
 */
uint8_t USBHID_abortReceive (uint16_t* size, uint8_t intfNum);

/*
 * This function rejects payload data that has been received from the host.
 */
uint8_t USBHID_rejectData (uint8_t intfNum);

/*
 * Aborts an active send operation on interface intfNum.  Returns the number of bytes that were sent prior to the abort, in size.
 */
uint8_t USBHID_abortSend (uint16_t* size, uint8_t intfNum);

/*
 * This function indicates the status of the interface intfNum.
 * If a send operation is active for this interface,
 * the function also returns the number of bytes that have been transmitted to the host.
 * If a receiver operation is active for this interface, the function also returns
 * the number of bytes that have been received from the host and are waiting at the assigned address.
 *
 * returns USBHID_WAITING_FOR_SEND (indicates that a call to USBHID_SendData()
 * has been made, for which data transfer has not been completed)
 *
 * returns USBHID_WAITING_FOR_RECEIVE (indicates that a receive operation
 * has been initiated, but not all data has yet been received)
 *
 * returns USBHID_DATA_WAITING (indicates that data has been received
 * from the host, waiting in the USB receive buffers)
 */
uint8_t USBHID_getInterfaceStatus (uint8_t intfNum, uint16_t* bytesSent, uint16_t* bytesReceived);

/*
 * Returns how many bytes are in the buffer are received and ready to be read.
 */
uint8_t USBHID_getBytesInUSBBuffer (uint8_t intfNum);

/*----------------------------------------------------------------------------
 * Event-Handling routines
 +----------------------------------------------------------------------------*/

/*
 * This event indicates that data has been received for port port, but no data receive operation is underway.
 * returns TRUE to keep CPU awake
 */
uint8_t USBHID_handleDataReceived (uint8_t intfNum);

/*
 * This event indicates that a send operation on port port has just been completed.
 * returns TRUE to keep CPU awake
 */
uint8_t USBHID_handleSendCompleted (uint8_t intfNum);

/*
 * This event indicates that a receive operation on port port has just been completed.
 * returns TRUE to keep CPU awake
 */
uint8_t USBHID_handleReceiveCompleted (uint8_t intfNum);

/*
 * This event indicates that a Set_Protocol request was received from the host
 * The application may maintain separate reports for boot and report protocols.
 * The protocol field is either HID_BOOT_PROTOCOL or
 * HID_REPORT_PROTOCOL
 */
uint8_t USBHID_handleBootProtocol (uint8_t protocol, uint8_t intfnum);

/*
 * This event indicates that a Set_Report request was received from the host
 * The application needs to supply a buffer to retrieve the report data that will be sent
 * as part of this request. This handler is passed the reportType, reportId, the length of data
 * phase as well as the interface number.
 */
uint8_t *USBHID_handleEP0SetReport (uint8_t reportType, uint8_t reportId,
    uint16_t requestedLength,
    uint8_t intfnum);
/*
 * This event indicates that data as part of Set_Report request was received from the host
 * Tha application can return TRUE to wake up the CPU. If the application supplied a buffer
 * as part of USBHID_handleEP0SetReport, then this buffer will contain the Set Report data.
 */
uint8_t USBHID_handleEP0SetReportDataAvailable (uint8_t intfnum);
/*
 * This event indicates that a Get_Report request was received from the host
 * The application can supply a buffer of data that will be sent to the host.
 * This handler is passed the reportType, reportId, the requested length as
 * well as the interface number.
 */
uint8_t *USBHID_handleEP0GetReport (uint8_t reportType, uint8_t reportId,
    uint16_t requestedLength,
    uint8_t intfnum);

#ifdef __cplusplus
}
#endif
#endif  //_UsbHid_H_
