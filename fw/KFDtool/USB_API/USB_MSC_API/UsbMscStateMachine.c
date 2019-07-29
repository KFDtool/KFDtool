/** @file UsbMscStateMachine.c
 *  @brief Contains APIs related to MSC task Management.
 */
//
//! \cond
//

/* 
 * ======== UsbMscStateMachine.c ========
 */
/*File includes */
#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_MSC_API/UsbMscScsi.h"
#include "../USB_MSC_API/UsbMsc.h"
#include "../USB_Common/usb.h"
#include <descriptors.h>
#include <string.h>

#ifdef _MSC_

/*Macros to indicate data direction */
#define DIRECTION_IN    0x80
#define DIRECTION_OUT   0x00

/*Buffer pointers passed by application */
extern __no_init tEDB __data16 tInputEndPointDescriptorBlock[];
extern struct _MscState MscState;

uint8_t Scsi_Verify_CBW ();

/*----------------------------------------------------------------------------+
 | Functions                                                                  |
 +----------------------------------------------------------------------------*/
void Msc_ResetStateMachine (void)
{
    MscState.bMscSendCsw = FALSE;
    MscState.Scsi_Residue = 0;
    MscState.Scsi_Status = SCSI_PASSED;             /*Variable to track command status */
    MscState.bMcsCommandSupported = TRUE;           /*Flag to indicate read/write command is recieved from host */
    MscState.bMscCbwReceived = 0;                   /*Flag to inidicate whether any CBW recieved from host*/
    MscState.bMscSendCsw = FALSE;
    MscState.isMSCConfigured = FALSE;
    MscState.bUnitAttention = FALSE;
    MscState.bMscCbwFailed = FALSE;
    MscState.bMscResetRequired = FALSE;
	MscState.stallEndpoint = FALSE;
	MscState.stallAtEndofTx = FALSE;
}

//----------------------------------------------------------------------------
/*This is the core function called by application to handle the MSC SCSI state
* machine */

//
//! \endcond
//

//*****************************************************************************
//
//! Checks to See if a SCSI Command has Been Received.
//!
//! Checks to see if a SCSI command has been received. If so, it handles it. If not, it returns
//! having taken no action.
//! The return values of this function are intended to be used with entry of low-power modes. If the
//! function returns \b USBMSC_OK_TO_SLEEP, then no further application action is required; that is,
//! either no SCSI command was received; one was received but immediately handled; or one was
//! received but the handling will be completed in the background by the API as it automatically
//! services USB interrupts.
//! If instead the function returns \b USBMSC_PROCESS_BUFFER, then the API is currently servicing a
//! SCSI READ or WRITE command, and the API requires the application to process a buffer. (See
//! Sec. 8.3.6 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a discussion of buffer
//! processing.)
//! Note that even if the function returns these values, the values could potentially be outdated by
//! the time the application evaluates them. For this reason, it's important to disable interrupts prior
//! to calling this function. See Sec. 8.3.5 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC"
//! for more information.
//!
//! \return \b USBMSC_OK_TO_SLEEP or \b USBMSC_PROCESS_BUFFER
//
//*****************************************************************************

uint8_t USBMSC_pollCommand ()
{
	uint16_t state;
    uint8_t edbIndex;
    uint8_t * pCT1;
    uint8_t * pCT2;

    edbIndex = stUsbHandle[MSC0_INTFNUM].edb_Index;
    pCT1 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTX;
    pCT2 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTY;

    //check if currently transmitting data..
    if (MscReadControl.bReadProcessing == TRUE){
    	state = usbDisableOutEndpointInterrupt(edbIndex);
        //atomic operation - disable interrupts
        if ((MscReadControl.dwBytesToSendLeft == 0) &&
            (MscReadControl.lbaCount == 0)){
            //data is no more processing - clear flags..
            MscReadControl.bReadProcessing = FALSE;
            usbRestoreOutEndpointInterrupt(state);
        } else {
            if (!(tInputEndPointDescriptorBlock[edbIndex].bEPCNF &
                  EPCNF_STALL)){                    //if it is not stalled - contiune communication
                USBIEPIFG |= 1 << (edbIndex + 1);   //trigger IN interrupt to finish data tranmition
            }
            usbRestoreOutEndpointInterrupt(state);
            return (USBMSC_PROCESS_BUFFER);
        }
    }

    if (MscState.isMSCConfigured == FALSE){
        return (USBMSC_OK_TO_SLEEP);
    }

    if (!MscState.bMscSendCsw){
        if (MscState.bMscCbwReceived){
            if (Scsi_Verify_CBW() == SUCCESS){
                //Successful reception of CBW
                //Parse the CBW opcode and invoke the right command handler function
                Scsi_Cmd_Parser(MSC0_INTFNUM);
                MscState.bMscSendCsw = TRUE;
            }
            MscState.bMscCbwReceived = FALSE;       //CBW is performed!
        } else {
            return (USBMSC_OK_TO_SLEEP);
        }
        //check if any of out pipes has pending data and trigger interrupt

        if ((MscWriteControl.pCT1 != NULL)   &&
            ((*MscWriteControl.pCT1 & EPBCNT_NAK ) ||
             (*MscWriteControl.pCT2 & EPBCNT_NAK ))){
            USBOEPIFG |= 1 << (edbIndex + 1);       //trigger OUT interrupt again
            return (USBMSC_PROCESS_BUFFER);            //do not asleep, as data is coming in
            //and follow up data perform will be required.
        }
    }

    if (MscState.bMscSendCsw){
        if (MscState.bMcsCommandSupported == TRUE){
            //watiting till transport is finished!
            if ((MscWriteControl.bWriteProcessing == FALSE) &&
                (MscReadControl.bReadProcessing == FALSE) &&
                (MscReadControl.lbaCount == 0)){
                //Send CSW
                if (MscState.stallAtEndofTx == TRUE) {
                	if ((*pCT1 & EPBCNT_NAK) && (*pCT2 & EPBCNT_NAK)) {
                		MscState.stallAtEndofTx = FALSE;
                		usbStallInEndpoint(MSC0_INTFNUM);
                	}
                }
                else if (SUCCESS == Scsi_Send_CSW(MSC0_INTFNUM)){
                    MscState.bMscSendCsw = FALSE;
                    return (USBMSC_OK_TO_SLEEP);
                }
            }		
            else {
            	MSCFromHostToBuffer();
            }
        }
    }

    return (USBMSC_PROCESS_BUFFER);                 //When MscState.bMcsCommandSupported = FALSE, bReadProcessing became true, and
                                                    //bWriteProcessing = true.
}

//
//! \cond
//

#endif //_MSC_

//
//! \endcond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
