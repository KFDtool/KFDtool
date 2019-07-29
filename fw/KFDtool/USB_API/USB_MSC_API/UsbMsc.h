/* 
 * ======== UsbMsc.h ========
 */
#include <stdint.h>

#ifndef _USB_MSC_H_
#define _USB_MSC_H_

#include "UsbMscScsi.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*----------------------------------------------------------------------------
 * The following function names and macro names are deprecated.  These were 
 * updated to new names to follow OneMCU naming convention.
 +---------------------------------------------------------------------------*/
#ifndef DEPRECATED
#define kUSBMSC_idle                        USBMSC_IDLE
#define kUSBMSC_readInProgress              USBMSC_READ_IN_PROGRESS
#define kUSBMSC_writeInProgress             USBMSC_WRITE_IN_PROGRESS
#define kUSBMSC_cmdBeingProcessed           USBMSC_COMMAND_BEING_PROCESSED
#define kUSBMSC_okToSleep                   USBMSC_OK_TO_SLEEP
#define kUSBMSC_processBuffer               USBMSC_PROCESS_BUFFER

#define USBMSC_bufferProcessed              USBMSC_processBuffer
#define USBMSC_updateMediaInfo              USBMSC_updateMediaInformation
#define USBMSC_registerBufInfo              USBMSC_registerBufferInformation
#define USBMSC_poll                         USBMSC_pollCommand
#endif



/*Return values of getState() and USBMSC_pollCommand() API */
#define USBMSC_IDLE               0
#define USBMSC_READ_IN_PROGRESS     1
#define USBMSC_WRITE_IN_PROGRESS    2
#define USBMSC_COMMAND_BEING_PROCESSED  3
#define USBMSC_OK_TO_SLEEP          4
#define USBMSC_PROCESS_BUFFER      5


/*----------------------------------------------------------------------------+
 | Function Prototypes                                                         |
 +----------------------------------------------------------------------------*/

/*Function to handle the MSC SCSI state machine */
uint8_t USBMSC_pollCommand(void);

/* MSC functions */
int16_t MSCToHostFromBuffer ();
int16_t MSCFromHostToBuffer ();
uint8_t USBMSC_processBuffer(void);
uint8_t USBMSC_getState ();
uint8_t USBMSC_updateMediaInformation (uint8_t lun, struct USBMSC_mediaInfoStr *info);

uint8_t USBMSC_handleBufferEvent(void);
uint8_t USBMSC_registerBufferInformation ( uint8_t lun, uint8_t* RWbuf_x, uint8_t* RWbuf_y, uint16_t size);

#ifdef __cplusplus
}
#endif
#endif  //_USB_MSC_H_
