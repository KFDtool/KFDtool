
//
//! \cond
//

/* 
 * ======== UsbMscReq.c ========
 */
#include <descriptors.h>

#ifdef _MSC_

#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_Common/usb.h"      //USB-specific Data Structures
#include "../USB_MSC_API/UsbMscScsi.h"
#include "../USB_MSC_API/UsbMscReq.h"
#include "../USB_MSC_API/UsbMsc.h"

extern __no_init tEDB __data16 tInputEndPointDescriptorBlock[];
extern __no_init tEDB __data16 tOutputEndPointDescriptorBlock[];
extern struct _MscState MscState;

/*----------------------------------------------------------------------------+
 | Functions                                                                   |
 +----------------------------------------------------------------------------*/
uint8_t USBMSC_reset (void)
{
    Msc_ResetStateMachine();
    Msc_ResetFlags();
    Msc_ResetStruct();
    MscState.isMSCConfigured = TRUE;

    MscState.bMscResetRequired = FALSE;
    tInputEndPointDescriptorBlock[stUsbHandle[MSC0_INTFNUM].edb_Index].bEPCNF
        &= ~(EPCNF_STALL | EPCNF_TOGGLE );
    tOutputEndPointDescriptorBlock[stUsbHandle[MSC0_INTFNUM].edb_Index].bEPCNF
        &= ~(EPCNF_STALL  | EPCNF_TOGGLE );
    usbSendZeroLengthPacketOnIEP0();    //status stage for control transfer

    return (FALSE);
}

//----------------------------------------------------------------------------
uint8_t Get_MaxLUN (void)
{
    uint8_t maxLunNumber = MSC_MAX_LUN_NUMBER - 1;

    wBytesRemainingOnIEP0 = 1;
    MscState.isMSCConfigured = TRUE;
    usbSendDataPacketOnEP0((uint8_t*)&maxLunNumber);

    return (FALSE);
}

#endif //_MSC_

//
//! \cond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
