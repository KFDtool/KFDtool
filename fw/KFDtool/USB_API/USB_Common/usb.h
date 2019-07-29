/* 
 * ======== usb.h ========
 */
#ifndef _USB_H_
#define _USB_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*----------------------------------------------------------------------------+
 | Constant Definition                                                         |
 +----------------------------------------------------------------------------*/
#if defined(__TI_COMPILER_VERSION__)  || defined(__GNUC__)
#define __no_init
#define __data16
#endif

/*----------------------------------------------------------------------------
 * The following macro names and function names are deprecated.  These were 
 * updated to new names to follow OneMCU naming convention.
 +---------------------------------------------------------------------------*/

#ifndef DEPRECATED
#define  kUSB_succeed                        USB_SUCCEED
#define  kUSB_generalError                   USB_GENERAL_ERROR
#define  kUSB_notEnabled                     USB_NOT_ENABLED
#define  kUSB_vbusPresent                    USB_VBUS_PRESENT
#define  kUSB_busActive                      USB_BUS_ACTIVE
#define  kUSB_ConnectNoVBUS                  USB_CONNECT_NO_VBUS
#define  kUSB_suspended                      USB_SUSPENDED
#define  kUSB_NotSuspended                   USB_NOT_SUSPENDED
#define  kUSB_Enumerated                     USB_ENUMERATED
#define  kUSB_purHigh                        USB_PUR_HIGH
#define  kUSB_clockFaultEvent                USB_CLOCK_FAULT_EVENT
#define  kUSB_VbusOnEvent                    USB_VBUS_ON_EVENT
#define  kUSB_VbusOffEvent                   USB_VBUS_OFF_EVENT
#define  kUSB_UsbResetEvent                  USB_RESET_EVENT
#define  kUSB_UsbSuspendEvent                USB_SUSPENDED_EVENT
#define  kUSB_UsbResumeEvent                 USB_RESUME_EVENT
#define  kUSB_dataReceivedEvent              USB_DATA_RECEIVED_EVENT
#define  kUSB_sendCompletedEvent             USB_SEND_COMPLETED_EVENT
#define  kUSB_receiveCompletedEvent          USB_RECEIVED_COMPLETED_EVENT
#define  kUSB_allUsbEvents                   USB_ALL_USB_EVENTS

#define   USB_connectionInfo                 USB_getConnectionInformation
#define   USB_connectionState  	             USB_getConnectionState
#define   USB_handleEnumCompleteEvent        USB_handleEnumerationCompleteEvent
#endif


#define FALSE   0
#define TRUE    1

#define USB_RETURN_DATA_LENGTH  8
#define SIZEOF_DEVICE_REQUEST   0x08

//Bit definitions for DEVICE_REQUEST.bmRequestType
//Bit 7:   Data direction
#define USB_REQ_TYPE_OUTPUT     0x00    //0 = Host sending data to device
#define USB_REQ_TYPE_INPUT      0x80    //1 = Device sending data to host

//Bit 6-5: Type
#define USB_REQ_TYPE_MASK       0x60    //Mask value for bits 6-5
#define USB_REQ_TYPE_STANDARD   0x00    //00 = Standard USB request
#define USB_REQ_TYPE_CLASS      0x20    //01 = Class specific
#define USB_REQ_TYPE_VENDOR     0x40    //10 = Vendor specific

//Bit 4-0: Recipient
#define USB_REQ_TYPE_RECIP_MASK 0x1F    //Mask value for bits 4-0
#define USB_REQ_TYPE_DEVICE     0x00    //00000 = Device
#define USB_REQ_TYPE_INTERFACE  0x01    //00001 = Interface
#define USB_REQ_TYPE_ENDPOINT   0x02    //00010 = Endpoint
#define USB_REQ_TYPE_OTHER      0x03    //00011 = Other

//Values for DEVICE_REQUEST.bRequest
//Standard Device Requests
#define USB_REQ_GET_STATUS              0
#define USB_REQ_CLEAR_FEATURE           1
#define USB_REQ_SET_FEATURE             3
#define USB_REQ_SET_ADDRESS             5
#define USB_REQ_GET_DESCRIPTOR          6
#define USB_REQ_SET_DESCRIPTOR          7
#define USB_REQ_GET_CONFIGURATION       8
#define USB_REQ_SET_CONFIGURATION       9
#define USB_REQ_GET_INTERFACE           10
#define USB_REQ_SET_INTERFACE           11
#define USB_REQ_SYNCH_FRAME             12

//CDC CLASS Requests
#define USB_CDC_GET_LINE_CODING         0x21
#define USB_CDC_SET_LINE_CODING         0x20
#define USB_CDC_SET_CONTROL_LINE_STATE  0x22

//HID CLASS Requests
#define USB_HID_REQ                     0x81
#define USB_REQ_GET_REPORT              0x01
#define USB_REQ_GET_IDLE                0x02
#define USB_REQ_SET_REPORT              0x09
#define USB_REQ_SET_IDLE                0x0A
#define USB_REQ_SET_PROTOCOL            0x0B
#define USB_REQ_GET_PROTOCOL            0x03

//MSC CLASS Requests
#define USB_MSC_RESET_BULK              0xFF
#define USB_MSC_GET_MAX_LUN             0xFE

// PHDC CLASS Requests
#define USB_PHDC_GET_STATUS             0x00

//HID Values for HID Report Types (tSetup.bValueH)
#define USB_REQ_HID_INPUT               0x01
#define USB_REQ_HID_OUTPUT              0x02
#define USB_REQ_HID_FEATURE             0x03

#define USB_REQ_HID_BOOT_PROTOCOL       0x00
#define USB_REQ_HID_REPORT_PROTOCOL     0x01


//Descriptor Type Values
#define DESC_TYPE_DEVICE                1       //Device Descriptor (Type 1)
#define DESC_TYPE_CONFIG                2       //Configuration Descriptor (Type 2)
#define DESC_TYPE_STRING                3       //String Descriptor (Type 3)
#define DESC_TYPE_INTERFACE             4       //Interface Descriptor (Type 4)
#define DESC_TYPE_ENDPOINT              5       //Endpoint Descriptor (Type 5)
#define DESC_TYPE_DEVICE_QUALIFIER      6       //Endpoint Descriptor (Type 6)
#define DESC_TYPE_IAD                   0x0B
#define DESC_TYPE_HUB                   0x29    //Hub Descriptor (Type 6)
#define DESC_TYPE_HID                   0x21    //HID Descriptor
#define DESC_TYPE_REPORT                0x22    //Report Descriptor
#define DESC_TYPE_PHYSICAL              0x23    //Physical Descriptor

//Feature Selector Values
#define FEATURE_REMOTE_WAKEUP           1       //Remote wakeup (Type 1)
#define FEATURE_ENDPOINT_STALL          0       //Endpoint stall (Type 0)

//Device Status Values
#define DEVICE_STATUS_REMOTE_WAKEUP     0x02
#define DEVICE_STATUS_SELF_POWER        0x01

//Maximum descriptor size
#define MAX_DESC_SIZE                   256

//DEVICE_DESCRIPTOR structure
#define SIZEOF_DEVICE_DESCRIPTOR        0x12
#define OFFSET_DEVICE_DESCRIPTOR_VID_L  0x08
#define OFFSET_DEVICE_DESCRIPTOR_VID_H  0x09
#define OFFSET_DEVICE_DESCRIPTOR_PID_L  0x0A
#define OFFSET_DEVICE_DESCRIPTOR_PID_H  0x0B
#define OFFSET_CONFIG_DESCRIPTOR_POWER  0x07
#define OFFSET_CONFIG_DESCRIPTOR_CURT   0x08

//CONFIG_DESCRIPTOR structure
#define SIZEOF_CONFIG_DESCRIPTOR 0x09

//HID DESCRIPTOR structure
//#define SIZEOF_HID_DESCRIPTOR 0x09

//Bit definitions for CONFIG_DESCRIPTOR.bmAttributes
#define CFG_DESC_ATTR_SELF_POWERED  0x40    //Bit 6: If set, device is self powered
#define CFG_DESC_ATTR_BUS_POWERED   0x80    //Bit 7: If set, device is bus powered
#define CFG_DESC_ATTR_REMOTE_WAKE   0x20    //Bit 5: If set, device supports remote wakeup

//INTERFACE_DESCRIPTOR structure
#define SIZEOF_INTERFACE_DESCRIPTOR 0x09

//ENDPOINT_DESCRIPTOR structure
#define SIZEOF_ENDPOINT_DESCRIPTOR 0x07

//Bit definitions for EndpointDescriptor.EndpointAddr
#define EP_DESC_ADDR_EP_NUM     0x0F        //Bit 3-0: Endpoint number
#define EP_DESC_ADDR_DIR_IN     0x80        //Bit 7: Direction of endpoint, 1/0 = In/Out

//Bit definitions for EndpointDescriptor.EndpointFlags
#define EP_DESC_ATTR_TYPE_MASK  0x03        //Mask value for bits 1-0
#define EP_DESC_ATTR_TYPE_CONT  0x00        //Bit 1-0: 00 = Endpoint does control transfers
#define EP_DESC_ATTR_TYPE_ISOC  0x01        //Bit 1-0: 01 = Endpoint does isochronous transfers
#define EP_DESC_ATTR_TYPE_BULK  0x02        //Bit 1-0: 10 = Endpoint does bulk transfers
#define EP_DESC_ATTR_TYPE_INT   0x03        //Bit 1-0: 11 = Endpoint does interrupt transfers

//Definition to indicate valid/invalid data
#define DATA_VALID      1
#define DATA_INVALID    0

typedef enum {
    STATUS_ACTION_NOTHING,
    STATUS_ACTION_DATA_IN,
    STATUS_ACTION_DATA_OUT
} tSTATUS_ACTION_LIST;


typedef struct _tDEVICE_REQUEST {
    uint8_t bmRequestType;         //See bit definitions below
    uint8_t bRequest;              //See value definitions below
    uint16_t wValue;                //Meaning varies with request type
    uint16_t wIndex;                //Meaning varies with request type
    uint16_t wLength;               //Number of bytes of data to transfer
} tDEVICE_REQUEST, *ptDEVICE_REQUEST;

extern __no_init tDEVICE_REQUEST __data16 tSetupPacket;
extern __no_init uint8_t __data16 abIEP0Buffer[];
extern __no_init uint8_t __data16 abOEP0Buffer[];
extern __no_init uint8_t __data16 pbXBufferAddressEp1[];
extern __no_init uint8_t __data16 pbYBufferAddressEp1[];
extern __no_init uint8_t __data16 pbXBufferAddressEp81[];
extern __no_init uint8_t __data16 pbYBufferAddressEp81[];
extern __no_init uint8_t __data16 pbXBufferAddressEp2[];
extern __no_init uint8_t __data16 pbYBufferAddressEp2[];
extern __no_init uint8_t __data16 pbXBufferAddressEp82[];
extern __no_init uint8_t __data16 pbYBufferAddressEp82[];

extern __no_init uint8_t __data16 pbXBufferAddressEp3[];
extern __no_init uint8_t __data16 pbYBufferAddressEp3[];
extern __no_init uint8_t __data16 pbXBufferAddressEp83[];
extern __no_init uint8_t __data16 pbYBufferAddressEp83[];

extern __no_init uint8_t __data16 pbXBufferAddressEp4[];
extern __no_init uint8_t __data16 pbYBufferAddressEp4[];
extern __no_init uint8_t __data16 pbXBufferAddressEp84[];
extern __no_init uint8_t __data16 pbYBufferAddressEp84[];

extern __no_init uint8_t __data16 pbXBufferAddressEp5[];
extern __no_init uint8_t __data16 pbYBufferAddressEp5[];
extern __no_init uint8_t __data16 pbXBufferAddressEp85[];
extern __no_init uint8_t __data16 pbYBufferAddressEp85[];


extern __no_init uint8_t __data16 pbXBufferAddressEp6[];
extern __no_init uint8_t __data16 pbYBufferAddressEp6[];
extern __no_init uint8_t __data16 pbXBufferAddressEp86[];
extern __no_init uint8_t __data16 pbYBufferAddressEp86[];

extern __no_init uint8_t __data16 pbXBufferAddressEp7[];
extern __no_init uint8_t __data16 pbYBufferAddressEp7[];
extern __no_init uint8_t __data16 pbXBufferAddressEp87[];
extern __no_init uint8_t __data16 pbYBufferAddressEp87[];

extern uint16_t wBytesRemainingOnIEP0;
extern uint16_t wBytesRemainingOnOEP0;
extern uint8_t abUsbRequestReturnData[];
extern uint8_t abUsbRequestIncomingData[];
extern uint8_t bEnumerationStatus;
extern uint8_t bFunctionSuspended;

//Function return values
#define USB_SUCCEED         0x00
#define USB_GENERAL_ERROR   0x01
#define USB_NOT_ENABLED     0x02
//#define kUSB_VbusNotPresent 0x03

//return values USB_getConnectionInformation(), USB_connect()
#define USB_VBUS_PRESENT     0x01
#define USB_BUS_ACTIVE       0x02    //frame sync packets are being received
#define USB_CONNECT_NO_VBUS  0x04
#define USB_SUSPENDED        0x08
#define USB_NOT_SUSPENDED    0x10
#define USB_ENUMERATED       0x20
#define USB_PUR_HIGH         0x40

//Parameters for function USB_setEnabledEvents()
#define USB_CLOCK_FAULT_EVENT         0x0001
#define USB_VBUS_ON_EVENT             0x0002
#define USB_VBUS_OFF_EVENT            0x0004
#define USB_RESET_EVENT               0x0008
#define USB_SUSPENDED_EVENT           0x0010
#define USB_RESUME_EVENT              0x0020
#define USB_DATA_RECEIVED_EVENT       0x0040
#define USB_SEND_COMPLETED_EVENT      0x0080
#define USB_RECEIVED_COMPLETED_EVENT  0x0100
#define USB_ALL_USB_EVENTS            0x01FF

//USB connection states
#define ST_USB_DISCONNECTED         0x80
#define ST_USB_CONNECTED_NO_ENUM    0x81
#define ST_ENUM_IN_PROGRESS         0x82
#define ST_ENUM_ACTIVE              0x83
#define ST_ENUM_SUSPENDED           0x84
//#define ST_FAILED_ENUM              0x85
#define ST_ERROR                    0x86
#define ST_NOENUM_SUSPENDED         0x87

#define ST_PHYS_DISCONNECTED            ST_USB_DISCONNECTED
#define ST_PHYS_CONNECTED_NOENUM        ST_USB_CONNECTED_NO_ENUM
#define ST_PHYS_CONNECTED_NOENUM_SUSP   ST_NOENUM_SUSPENDED

#define USB_CLOCKFAULT_EVENTMASK        USB_CLOCK_FAULT_EVENT
#define USB_VBUSON_EVENTMASK            USB_VBUS_ON_EVENT
#define USB_VBUSOFF_EVENTMASK           USB_VBUS_OFF_EVENT
#define USB_USBRESET_EVENTMASK          USB_RESET_EVENT
#define USB_USBSUSPEND_EVENTMASK        USB_SUSPENDED_EVENT
#define USB_USBRESUME_EVENTMASK         USB_RESUME_EVENT
#define USB_DATARECEIVED_EVENTMASK      USB_DATA_RECEIVED_EVENT
#define USB_SENDCOMPLETED_EVENTMASK     USB_SEND_COMPLETED_EVENT
#define USB_RECEIVECOMPLETED_EVENTMASK  USB_RECEIVED_COMPLETED_EVENT
#define USB_ALL_EVENTMASK           	USB_ALL_USB_EVENTS

#define SUCCESS 0
#define FAILURE 1

typedef struct _tDEVICE_REQUEST_COMPARE {
    uint8_t bmRequestType;         //See bit definitions below
    uint8_t bRequest;              //See value definitions below
    uint8_t bValueL;               //Meaning varies with request type
    uint8_t bValueH;               //Meaning varies with request type
    uint8_t bIndexL;               //Meaning varies with request type
    uint8_t bIndexH;               //Meaning varies with request type
    uint8_t bLengthL;              //Number of bytes of data to transfer (LSByte)
    uint8_t bLengthH;              //Number of bytes of data to transfer (MSByte)
    uint8_t bCompareMask;          //MSB is bRequest, if set 1, bRequest should be matched
    uint8_t (*pUsbFunction)(void); //function pointer
} tDEVICE_REQUEST_COMPARE, *ptDEVICE_REQUEST_COMPARE;

void usbStallInEndpoint(uint8_t);
void usbStallOutEndpoint(uint8_t);
void usbStallEndpoint(uint8_t);
void usbClearOEPByteCount(uint8_t);

/*----------------------------------------------------------------------------
 * These functions can be used in application
 +----------------------------------------------------------------------------*/

/*
 * MSP430 USB Module Management functions
 */

/**
 * Init the USB HW interface.
 */
uint8_t USB_init(void);

/**
 * Init the USB HW interface, enable events and connect
 */
uint8_t USB_setup(uint8_t connectEnable, uint8_t eventsEnable);

/**
 * Init and start the USB PLL.
 */
uint8_t USB_enable ();

#ifdef USE_TIMER_FOR_RESUME
/**
 * First phase of enable in the case where a timer is used to stabilize crystal and PLL
 */
uint8_t USB_enable_crystal (void);

/**
 * Second phase of enable in the case where a timer is used to stabilize crystal and PLL
 */
void USB_enable_PLL(void);

/**
 * Final phase of enable in the case where a timer is used to stabilize crystal and PLL
 */
void USB_enable_final(void);

#endif
/**
 * Disables the USB module and PLL.
 */
uint8_t USB_disable(void);

/*
 * Enables/disables various USB events.
 */
uint8_t USB_setEnabledEvents (uint16_t events);

/*
 * Returns which events are enabled and which are disabled.
 */
uint16_t USB_getEnabledEvents ();

/*
 * Instruct USB module to make itself available to the PC for connection, by pulling PUR high.
 */
uint8_t USB_connect ();

/*
 * Force a disconnect from the PC by pulling PUR low.
 */
uint8_t USB_disconnect ();

/**
 * Reset USB-SIE and global variables.
 */
uint8_t USB_reset ();

/**
 * Suspend USB.
 */
uint8_t USB_suspend(void);

/**
 * Resume USB.
 */
uint8_t USB_resume(void);

/*
 * Force a remote wakeup of the USB host.
 *     This method can be generated only if device supports
 *     remote wake-up feature in some of its configurations.
 *     The method wakes-up the USB bus only if wake-up feature is enabled by the host.
 */
uint8_t USB_forceRemoteWakeup ();

/*
 * Returns the status of the USB connection.
 */
uint8_t USB_getConnectionInformation ();

/*
 * Returns the state of the USB connection.
 */
uint8_t USB_getConnectionState ();

#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES
/*
 * Switch to a different USB configuration. Used only for non-composite devices with multiple configuratons.
 */
uint8_t USB_switchInterface(uint8_t interfaceIndex);

#endif

/*
 * Event-Handling routines
 */

/*
 * If this function gets executed, it's a sign that the output of the USB PLL has failed.
 * returns TRUE to keep CPU awake
 */
uint8_t USB_handleClockEvent ();

/*
 * If this function gets executed, it indicates that a valid voltage has just been applied to the VBUS pin.
 * returns TRUE to keep CPU awake
 */
uint8_t USB_handleVbusOnEvent ();

/*
 * If this function gets executed, it indicates that a valid voltage has just been removed from the VBUS pin.
 * returns TRUE to keep CPU awake
 */
uint8_t USB_handleVbusOffEvent ();

/*
 * If this function gets executed, it indicates that the USB host has issued a USB reset event to the device.
 * returns TRUE to keep CPU awake
 */
uint8_t USB_handleResetEvent ();

/*
 * If this function gets executed, it indicates that the USB host has chosen to suspend this device after a period of active
 * operation.
 * returns TRUE to keep CPU awake
 */
uint8_t USB_handleSuspendEvent ();

/*
 * If this function gets executed, it indicates that the USB host has chosen to resume this device after a period of suspended
 * operation.
 * returns TRUE to keep CPU awake
 */
uint8_t USB_handleResumeEvent ();

/*
 * If this function gets executed, it indicates that the USB host has enumerated this device :
 * after host assigned the address to the device.
 * returns TRUE to keep CPU awake
 */
uint8_t USB_handleEnumerationCompleteEvent ();

#ifdef USE_TIMER_FOR_RESUME
/*
 * When this function gets executed, it indicates that a USB_resume is in progress and the USB
 * stack requires the application to use a timer to wait until the XT2 crystal has
 * stabilized. See crystal specific datasheet for delay times. When the crystal has 
 * stabilized the application needs to call the function USB_enable_PLL() to allow
 * resume to continue.
 */
void USB_handleCrystalStartedEvent(void);

/*
 * When this function gets executed, it indicates that a USB_resume is in progress and the USB
 * stack requires the application to use a timer to wait until the USB PLL has
 * stabilized. See device specific datasheet for PLL delay times. When the PLL has 
 * stabilized the application needs to call the function USB_enable_final() to allow resume
 * to complete.
 */
void USB_handlePLLStartedEvent(void);

#endif

/**
 * Send stall handshake for in- and out-endpoint0 (control pipe)
 */
void usbStallEndpoint0(void);

/**
 * Clear byte counter for endpoint0 (control pipe)
 */
void usbClearOEP0ByteCount(void);

/**
 * Send stall handshake for out-endpoint0 (control pipe)
 */
void usbStallOEP0(void);

/**
 * Send further data over control pipe if needed.
 *     Function is called from control-in IRQ. Do not call from user application
 */
void usbSendNextPacketOnIEP0(void);

/**
 * Send data over control pipe to host.
 *     Number of bytes to transmit should be set with
 *     global varible "wBytesRemainingOnIEP0" before function is called.
 */
void usbSendDataPacketOnEP0 (const uint8_t* pbBuffer);

/**
 * Receive further data from control pipe if needed.
 *     Function is called from control-out IRQ. Do not call from user application
 */
void usbReceiveNextPacketOnOEP0(void);

/**
 * Receive data from control pipe.
 *     Number of bytes to receive should be set with
 *     global varible "wBytesRemainingOnOEP0" before function is called.
 */
void usbReceiveDataPacketOnEP0 (uint8_t* pbBuffer);

/**
 * Send zero length packet on control pipe.
 */
void usbSendZeroLengthPacketOnIEP0(void);

/*Send data to host.*/
uint8_t MscSendData (const uint8_t* data, uint16_t size);

/**
 * Decode incoming usb setup packet and call corresponding function
 *     usbDecodeAndProcessUsbRequest is called from IRQ. Do not call from user application
 */
uint8_t usbDecodeAndProcessUsbRequest(void);
uint8_t usbClearEndpointFeature(void);
uint8_t usbGetConfiguration(void);
uint8_t usbGetDeviceDescriptor(void);
uint8_t usbGetConfigurationDescriptor(void);
uint8_t usbGetStringDescriptor(void);
uint8_t usbGetInterface(void);
uint8_t usbGetDeviceStatus(void);
uint8_t usbGetEndpointStatus(void);
uint8_t usbGetInterfaceStatus(void);
uint8_t usbSetAddress(void);
uint8_t usbSetConfiguration(void);
uint8_t usbClearDeviceFeature(void);
uint8_t usbSetDeviceFeature(void);
uint8_t usbSetEndpointFeature(void);
uint8_t usbSetInterface(void);
uint8_t usbInvalidRequest(void);
uint16_t usbDisableInEndpointInterrupt(uint8_t edbIndex);
void usbRestoreInEndpointInterrupt(uint16_t state);
uint16_t usbDisableOutEndpointInterrupt(uint8_t edbIndex);
void usbRestoreOutEndpointInterrupt(uint16_t state);

#define ENUMERATION_COMPLETE 0x01

/*----------------------------------------------------------------------------+
 | End of header file                                                          |
 +----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif  /*
         * _USB_H
         *------------------------ Nothing Below This Line --------------------------
         */
