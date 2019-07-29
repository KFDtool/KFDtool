
//
//! \cond
//

/* 
 * ======== UsbHidReq.c ========
 */
#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_Common/usb.h"                              //USB-specific Data Structures
#include "UsbHid.h"
#include "UsbHidReq.h"
#include <descriptors.h>

#ifdef _HID_

void usbClearOEP0ByteCount (void);
void usbSendDataPacketOnEP0 (const uint8_t* pbBuffer);
void usbReceiveDataPacketOnEP0 (uint8_t* pbBuffer);

extern const uint16_t report_desc_size[HID_NUM_INTERFACES];
extern const uint8_t* report_desc[HID_NUM_INTERFACES];         //KLQ
extern uint8_t hidProtocol[];
extern uint8_t hidIdleRate[];
extern uint16_t wUsbHidEventMask;

#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES
extern const struct abromConfigurationDescriptorGroupHID abromConfigurationDescriptorGroupHID;
#endif

//Local Macros
#define INTERFACE_OFFSET(X)   (X - HID0_REPORT_INTERFACE)   //Get the HID offset

uint8_t usbGetHidDescriptor (void)
{
    usbClearOEP0ByteCount();
    wBytesRemainingOnIEP0 = 9;
#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES
    usbSendDataPacketOnEP0((uint8_t*)&abromConfigurationDescriptorGroupHID.stHid[
            INTERFACE_OFFSET(tSetupPacket.wIndex)].blength_hid_descriptor);
#else
    usbSendDataPacketOnEP0((uint8_t*)&abromConfigurationDescriptorGroup.stHid[
            INTERFACE_OFFSET(tSetupPacket.wIndex)].blength_hid_descriptor);
#endif
    return (FALSE);
}

uint8_t usbGetReportDescriptor (void)
{
    wBytesRemainingOnIEP0 =
        report_desc_size[INTERFACE_OFFSET(tSetupPacket.wIndex)];
    usbSendDataPacketOnEP0(report_desc[INTERFACE_OFFSET(tSetupPacket.wIndex)]);

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetReport (void)
{
    uint8_t *buffer;

    //tSetupPacket.wValue = USB_REQ_HID_FEATURE or USB_REQ_HID_INPUT
    buffer = USBHID_handleEP0SetReport(tSetupPacket.wValue >> 8, tSetupPacket.wValue,
        tSetupPacket.wLength,
        tSetupPacket.wIndex);

    //What if buffer is NULL?
    if (buffer == 0){
        usbReceiveDataPacketOnEP0((uint8_t*)&abUsbRequestIncomingData);
    } else {
        usbReceiveDataPacketOnEP0((uint8_t*)buffer);   //receive data over EP0 from Host
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetReport (void)
{
    uint8_t *buffer;

    //tSetupPacket.wValue = USB_REQ_HID_FEATURE or USB_REQ_HID_INPUT
    buffer = USBHID_handleEP0GetReport(tSetupPacket.wValue >> 8, tSetupPacket.wValue,
        tSetupPacket.wLength,
        tSetupPacket.wIndex);
    if (buffer != 0){
        usbSendDataPacketOnEP0((uint8_t*)buffer);
    }

    return (FALSE);
}

uint8_t usbSetProtocol (void)
{
    uint8_t bWakeUp = FALSE;

    hidProtocol[INTERFACE_OFFSET(tSetupPacket.wIndex)] =
        (uint8_t)tSetupPacket.wValue;
    //tSetupPacket.wValue = USB_REQ_HID_BOOT_PROTOCOL or USB_REQ_HID_REPORT_PROTOCOL
    bWakeUp = USBHID_handleBootProtocol((uint8_t)tSetupPacket.wValue,
        tSetupPacket.wIndex);
    usbSendZeroLengthPacketOnIEP0();

    return (bWakeUp);
}

//----------------------------------------------------------------------------

uint8_t usbGetProtocol (void)
{
    usbSendDataPacketOnEP0(&hidProtocol[INTERFACE_OFFSET(tSetupPacket.wIndex)]);

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetIdle (void)
{
    if (hidProtocol[INTERFACE_OFFSET(tSetupPacket.wIndex)] ==
        USB_REQ_HID_BOOT_PROTOCOL){
        hidIdleRate[INTERFACE_OFFSET(tSetupPacket.wIndex)] =
            tSetupPacket.wValue >> 8;
        usbSendZeroLengthPacketOnIEP0();
    } else {
        usbInvalidRequest();
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetIdle (void)
{
    if (hidProtocol[INTERFACE_OFFSET(tSetupPacket.wIndex)] ==
        USB_REQ_HID_BOOT_PROTOCOL){
        usbSendDataPacketOnEP0(&hidIdleRate[INTERFACE_OFFSET(tSetupPacket.
                                                wIndex)]);
    } else {
        usbInvalidRequest();
    }

    return (FALSE);
}


//----------------------------------------------------------------------------

#endif  //_HID_

//
//! \endcond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
