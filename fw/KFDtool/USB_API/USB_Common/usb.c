/** @file usb.c
 *  @brief Contains APIs related to handling of Control Endpoint
 */
/* 
 * ======== usb.c ========
 */
/*----------------------------------------------------------------------------+
 | Include files                                                               |
 +----------------------------------------------------------------------------*/

//
//! \cond
//
#include <string.h>
#include "driverlib.h"

#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_Common/usb.h"      //USB-specific Data Structures
#include "../USB_CDC_API/UsbCdc.h"
#include "../USB_PHDC_API/UsbPHDC.h"
#include "../USB_HID_API/UsbHidReq.h"
#include "../USB_MSC_API/UsbMscScsi.h"
#include "../USB_Common/UsbIsr.h"
#include <descriptors.h>


/*----------------------------------------------------------------------------+
 | Internal Constant Definition                                               |
 +----------------------------------------------------------------------------*/
#define NO_MORE_DATA    0xFFFF
#define EPBCT_NAK       0x80
#define EPCNF_TOGLE     0x20

#define DIRECTION_IN    0x80
#define DIRECTION_OUT   0x00

#if defined(__TI_COMPILER_VERSION__)  || defined(__GNUC__)
#define __no_init
#define __data16
#endif

/*----------------------------------------------------------------------------+
 | Internal Variables                                                          |
 +----------------------------------------------------------------------------*/

static uint8_t bConfigurationNumber;   //Set to 1 when USB device has been
//configured, set to 0 when unconfigured

static uint8_t bInterfaceNumber;       //interface number

uint16_t wBytesRemainingOnIEP0;         //For endpoint zero transmitter only
                                    //Holds count of bytes remaining to be
                                    //transmitted by endpoint 0.  A value
                                    //of 0 means that a 0-length data packet
                                    //A value of 0xFFFF means that transfer
                                    //is complete.

uint16_t wBytesRemainingOnOEP0;         //For endpoint zero transmitter only
                                    //Holds count of bytes remaining to be
                                    //received by endpoint 0.  A value
                                    //of 0 means that a 0-length data packet
                                    //A value of 0xFFFF means that transfer
                                    //is complete.

static const uint8_t* pbIEP0Buffer;          //A buffer pointer to input end point 0
                                    //Data sent back to host is copied from
                                    //this pointed memory location

static uint8_t* pbOEP0Buffer;          //A buffer pointer to output end point 0
                                    //Data sent from host is copied to
                                    //this pointed memory location

static uint8_t bHostAskMoreDataThanAvailable = 0;

uint8_t abUsbRequestReturnData[USB_RETURN_DATA_LENGTH];
uint8_t abUsbRequestIncomingData[USB_RETURN_DATA_LENGTH];

__no_init uint8_t abramSerialStringDescriptor[34];

uint8_t bStatusAction;
uint8_t bFunctionSuspended = FALSE;    //TRUE if function is suspended
uint8_t bEnumerationStatus = 0;        //is 0 if not enumerated

static uint8_t bRemoteWakeup;

uint16_t wUsbEventMask;                 //used by USB_getEnabledEvents() and USB_setEnabledEvents()

#ifdef _MSC_
extern uint8_t USBMSC_reset (void);
void MscResetData ();
extern struct _MscState MscState;
#endif

#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES

extern const void *usbConfigurationDescriptors[];
extern const void *usbDeviceDescriptors[];
extern const uint8_t usbConfigurationsSizes[];
uint8_t activeInterfaceIndex = 0;

#endif

/*----------------------------------------------------------------------------+
 | Global Variables                                                            |
 +----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------+
 | Hardware Related Structure Definition                                       |
 +----------------------------------------------------------------------------*/

#ifdef __IAR_SYSTEMS_ICC__

#pragma location = 0x2380
__no_init tDEVICE_REQUEST __data16 tSetupPacket;

#pragma location = 0x0920
__no_init tEDB0 __data16 tEndPoint0DescriptorBlock;

#pragma location = 0x23C8
__no_init tEDB __data16 tInputEndPointDescriptorBlock[7];

#pragma location = 0x2388
__no_init tEDB __data16 tOutputEndPointDescriptorBlock[7];

#pragma location = 0x2378
__no_init uint8_t __data16 abIEP0Buffer[EP0_MAX_PACKET_SIZE];

#pragma location = 0x2370
__no_init uint8_t __data16 abOEP0Buffer[EP0_MAX_PACKET_SIZE];

#pragma location = OEP1_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp1[EP_MAX_PACKET_SIZE];

#pragma location = OEP1_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp1[EP_MAX_PACKET_SIZE];

#pragma location = IEP1_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp81[EP_MAX_PACKET_SIZE];

#pragma location = IEP1_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp81[EP_MAX_PACKET_SIZE];

#pragma location = OEP2_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp2[EP_MAX_PACKET_SIZE];

#pragma location = OEP2_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp2[EP_MAX_PACKET_SIZE];

#pragma location = IEP2_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp82[EP_MAX_PACKET_SIZE];

#pragma location = IEP2_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp82[EP_MAX_PACKET_SIZE];

#pragma location = OEP3_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp3[EP_MAX_PACKET_SIZE];

#pragma location = OEP3_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp3[EP_MAX_PACKET_SIZE];

#pragma location = IEP3_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp83[EP_MAX_PACKET_SIZE];

#pragma location = IEP3_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp83[EP_MAX_PACKET_SIZE];

#pragma location = OEP4_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp4[EP_MAX_PACKET_SIZE];

#pragma location = OEP4_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp4[EP_MAX_PACKET_SIZE];

#pragma location = IEP4_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp84[EP_MAX_PACKET_SIZE];

#pragma location = IEP4_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp84[EP_MAX_PACKET_SIZE];

#pragma location = OEP5_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp5[EP_MAX_PACKET_SIZE];

#pragma location = OEP5_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp5[EP_MAX_PACKET_SIZE];

#pragma location = IEP5_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp85[EP_MAX_PACKET_SIZE];

#pragma location = IEP5_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp85[EP_MAX_PACKET_SIZE];

#pragma location = OEP6_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp6[EP_MAX_PACKET_SIZE];

#pragma location = OEP6_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp6[EP_MAX_PACKET_SIZE];

#pragma location = IEP6_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp86[EP_MAX_PACKET_SIZE];

#pragma location = IEP6_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp86[EP_MAX_PACKET_SIZE];

#pragma location = OEP7_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp7[EP_MAX_PACKET_SIZE];

#pragma location = OEP7_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp7[EP_MAX_PACKET_SIZE];

#pragma location = IEP7_X_BUFFER_ADDRESS
__no_init uint8_t __data16 pbXBufferAddressEp87[EP_MAX_PACKET_SIZE];

#pragma location = IEP7_Y_BUFFER_ADDRESS
__no_init uint8_t __data16 pbYBufferAddressEp87[EP_MAX_PACKET_SIZE];



#endif

#if defined(__TI_COMPILER_VERSION__)  || defined(__GNUC__)
extern __no_init tDEVICE_REQUEST tSetupPacket;
extern __no_init tEDB0 tEndPoint0DescriptorBlock;
extern __no_init tEDB tInputEndPointDescriptorBlock[7];
extern __no_init tEDB tOutputEndPointDescriptorBlock[7];
extern __no_init uint8_t abIEP0Buffer[EP0_MAX_PACKET_SIZE];
extern __no_init uint8_t abOEP0Buffer[EP0_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp1[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp1[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp81[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp81[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp2[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp2[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp82[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp82[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp3[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp3[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp83[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp83[EP_MAX_PACKET_SIZE];

extern __no_init uint8_t pbXBufferAddressEp4[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp4[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp84[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp84[EP_MAX_PACKET_SIZE];

extern __no_init uint8_t pbXBufferAddressEp5[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp5[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbXBufferAddressEp85[EP_MAX_PACKET_SIZE];
extern __no_init uint8_t pbYBufferAddressEp85[EP_MAX_PACKET_SIZE];

#endif

void CdcResetData ();
void HidResetData ();
void PHDCResetData();

void USB_InitSerialStringDescriptor (void);
void USB_initMemcpy (void);
uint16_t USB_determineFreq(void);

/* Version string to embed in executable. May need to change for ELF compiler */
const char *VERSION = "__PRODUCT_VERSION__";
char *USB_getVersion(void)
{
	return ((char *)&VERSION);
}

//
//! \endcond
//

//*****************************************************************************
//
//! Initializes the USB Module.
//!
//! 
//! Initializes the USB module by configuring power and clocks, and configures
//! pins that are critical for USB. This should be called very soon after the 
//! beginning of program execution. 
//! 
//! Note that this does not enable the USB module (that is, does not set 
//! USB_EN bit). Rather, it prepares the USB module to detect the application of
//! power to VBUS, after which the application may choose to enable the module
//! and connect to USB. Calling this function is necessary to achieve expected 
//! LPM3 current consumption into DVCC.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_init (void)
{
    uint16_t bGIE  = __get_SR_register() & GIE;                                 //save interrupt status
    uint16_t MCLKFreq = USB_determineFreq();
    uint16_t DelayConstant_250us = ((MCLKFreq >> 6) + (MCLKFreq >> 7) + (MCLKFreq >> 9));
    volatile uint16_t i, j;

    char *(*fp)(void);

	/* force version string into executable */
    fp = &USB_getVersion;
    fp();

    //atomic operation - disable interrupts
    __disable_interrupt();                                                      //Disable global interrupts

    //configuration of USB module
    USBKEYPID   =     0x9628;                                                   //set KEY and PID to 0x9628 -> access to
																				//configuration registers enabled
    /* If USB device is self-powered, USB_SUPPORT_SELF_POWERED = 0xc0 */
#if (USB_SUPPORT_SELF_POWERED == 0xc0)
/*USB9 issue fixed in hardware for FG6626.  So no need to turn LDO off*/
#ifndef __MSP430FG6626__
        /* To fix USB9 enumeration issue */
        USBPWRCTL = 0;
#endif
#endif
	
    USBPHYCTL   =     PUSEL;                                                    //use DP and DM as USB terminals (not needed
                                                                                //because an external PHY is connected to port
                                                                                //9)
    if(USB_USE_INTERNAL_3V3LDO == TRUE)
        USBPWRCTL   =     VUSBEN + SLDOAON;                                     //enable 3.3v and 1.8v LDO (3.3 and 1.8V)
    else
        USBPWRCTL   =     SLDOEN + USBDETEN;                                    //enable 1.8v and VBUS voltage detection while internal 3.3v
                                                                                //LDO is turned off.

    for (j = 0; j < 20; j++) {
        for (i = 0; i < (DelayConstant_250us); i++) {//wait some time for LDOs (5ms delay)
            _NOP();
        }
    }

    USBPWRCTL   |=   VBONIE;                                  					//enable interrupt VBUSon
    USBKEYPID   =    0x9600;                                                    //access to configuration registers disabled

    //reset events mask
    wUsbEventMask = 0;

    //init Serial Number
#if (USB_STR_INDEX_SERNUM != 0)
    USB_InitSerialStringDescriptor();
#endif

    //init memcpy() function: DMA or non-DMA
    USB_initMemcpy();
#ifdef _MSC_
    MscResetCtrlLun();
#endif

    __bis_SR_register(bGIE);                                                    //restore interrupt status
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Initializes the USB Module. Also enables events and connects.
//!
//! 
//! Initializes the USB module by configuring power and clocks, and configures
//! pins that are critical for USB. This should be called very soon after the 
//! beginning of program execution. 
//!
//! If connectEnable is TRUE, then this API then enables the USB module, which 
//! includes activating the PLL and setting the USB_EN bit. AFter enabling the
//! USB module, this API will connect to the host if VBUS is present.
//!
//! If eventsEnable is set to TRUE then all USB events are enabled by this API.
//!
//! \param	connectEnable	If TRUE, Connect to host if VBUS is present by 
//!							pulling the D+ signal high using the PUR pin.
//! \param  eventsEnable	If TRUE, all USB events handlers are enabled
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_setup(uint8_t connectEnable, uint8_t eventsEnable)
{
	uint8_t status;

	status = USB_init();

	if (eventsEnable) {
		USB_setEnabledEvents(USB_ALL_USB_EVENTS);
	}
	if (connectEnable) {
		if (USB_getConnectionInformation() & USB_VBUS_PRESENT){
			if (USB_enable() == USB_SUCCEED){
				USB_reset();
				USB_connect();
			}
		}
	}

    return (status);
}

//
//! \cond
//

//----------------------------------------------------------------------------
//This function will be compiled only if
#if (USB_STR_INDEX_SERNUM != 0)
void USB_InitSerialStringDescriptor (void)
{
    uint8_t i,j,hexValue;
    uint8_t* pbSerNum;
    uint8_t bBytes;

    j = 1;                                                                      //we start with second byte, first byte (lenght)
                                                                                //will be filled later
    pbSerNum = 0;
    abramSerialStringDescriptor[j++] = DESC_TYPE_STRING;

    //TLV access Function Call
    TLV_getInfo(TLV_TAG_DIERECORD, 0,
        (uint8_t *)&bBytes, (uint16_t **)&pbSerNum);
    if (bBytes == 0){                                                           //no serial number available
        //use 00 as serial number = no serial number available
        abramSerialStringDescriptor[0] = 4;                                     //length
        abramSerialStringDescriptor[j++] = 0;                                   //no serial number available
        abramSerialStringDescriptor[j++] = 0;                                   //no serial number available
    } else {
        for (i = 0; (i < bBytes) && (i < 8); i++,pbSerNum++)
        {
            hexValue = (*pbSerNum & 0xF0) >> 4;
            if (hexValue < 10){
                abramSerialStringDescriptor[j++] = (hexValue + '0');
            } else {          abramSerialStringDescriptor[j++] = (hexValue + 55);}
            abramSerialStringDescriptor[j++] = 0x00;                            //needed for UNI-Code

            hexValue = (*pbSerNum & 0x0F);
            if (hexValue < 10){
                abramSerialStringDescriptor[j++] = (hexValue + '0');
            } else {          abramSerialStringDescriptor[j++] = (hexValue + 55);}
            abramSerialStringDescriptor[j++] = 0x00;                            //needed for UNI-Code
        }
        abramSerialStringDescriptor[0] = i * 4 + 2;                             //calculate the length
    }
}

#endif

//
//! \endcond
//

//*****************************************************************************
//
//! Enables the USB Module.
//!
//! Enables the USB module, which includes activating the PLL and setting the 
//! USB_EN bit. Power consumption increases as a result of this operation (see 
//! device datasheet for specifics). This call should only be made after an 
//! earlier call to USB_init(), and prior to any other call except than 
//! USB_setEnabledEvents(), or USB_getEnabledEvents(). It is usually called just
//! prior to attempting to connect with a host after a bus connection has 
//! already been detected.
//! 
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_enable ()
{
#ifdef USE_TIMER_FOR_RESUME
	USB_enable_crystal();
	return (USB_SUCCEED);
#else

    volatile uint16_t i, k;
    volatile uint16_t j = 0;
    uint16_t status;
    uint16_t MCLKFreq = USB_determineFreq();
    uint16_t DelayConstant_250us = ((MCLKFreq >> 6) + (MCLKFreq >> 7) + (MCLKFreq >> 9));

    if (!(USBPWRCTL & USBBGVBV)){                                               //check USB Bandgap and VBUS valid
        return (USB_GENERAL_ERROR);
    }

    if ((USBCNF & USB_EN) &&
        (USBPLLCTL & UPLLEN)){
        return (USB_SUCCEED);                                                  //exit if PLL is already enalbed
    }

#if defined (__MSP430F552x) || defined (__MSP430F550x)
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN2);
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3);
#elif defined (__MSP430F563x_F663x) || defined (__MSP430F565x_F665x) || defined (__MSP430FG6x2x)
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN2);
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN3);
#endif
    USBKEYPID = 0x9628;                                                         //set KEY and PID to 0x9628 -> access to
                                                                                //configuration registers enabled
    if(USB_XT2_BYPASS_MODE == FALSE){					//XT2 not in bypass mode
    	if (USB_XT_FREQ_VALUE >= 24) {
    		status = UCS_turnOnXT2WithTimeout(
    	   			XT2DRIVE_3, 50000);
    	}
    	else if(USB_XT_FREQ_VALUE >= 16) {
    		status = UCS_turnOnXT2WithTimeout(
    	  			XT2DRIVE_2, 50000);
    	}
    	else if(USB_XT_FREQ_VALUE >= 8) {
    		status = UCS_turnOnXT2WithTimeout(
    	   			XT2DRIVE_1, 50000);
    	}
    	else {
    		status = UCS_turnOnXT2WithTimeout(
    	 			XT2DRIVE_0, 50000);
    	}

    }
    else{												//XT2 in bypass mode
    	if (USB_XT_FREQ_VALUE >= 24) {
    		status = UCS_bypassXT2WithTimeout(
    			50000);
     	}
    	else if(USB_XT_FREQ_VALUE >= 16) {
    		status = UCS_bypassXT2WithTimeout(
    			 50000);
    	}
    	else if(USB_XT_FREQ_VALUE >= 8) {
    		status = UCS_bypassXT2WithTimeout(
    			50000);
    	}
    	else {
    		status = UCS_bypassXT2WithTimeout(
    			50000);
      	}
    }

	if (status == STATUS_FAIL) {
		return (USB_GENERAL_ERROR);
	}
	
    USBPLLDIVB = USB_XT_FREQ;                                                   //Settings desired frequency

    USBPLLCTL = UPFDEN + UPLLEN;                                    		    //Select XT2 as Ref / Select PLL for USB / Discrim.
                                                                                //on, enable PLL

    //Wait some time till PLL is settled
    do
    {
        USBPLLIR    =     0x0000;                                               //make sure no interrupts can occur on
                                                                                //PLL-module
        if ((((bFunctionSuspended == TRUE) || (bFunctionSuspended == FALSE)) && (USB_DISABLE_XT_SUSPEND == 1)) ||
        		((USB_DISABLE_XT_SUSPEND == 0) && (bFunctionSuspended == FALSE))){  //BUG NUMBER 4879
#ifdef __MSP430F6638
            //wait 1 ms till enable USB
            for(k = 0; k < 4; k++)
            {
                for (i = 0; i < (DelayConstant_250us); i++){
                    _NOP();
                }
            }
#else
            //wait 1/2 ms till enable USB
            for(k = 0; k < 2; k++)
            {
                for (i = 0; i < (DelayConstant_250us); i++){
                   _NOP();
                }
            }        
#endif
        }

        if (j++ > 10){
            USBKEYPID   =    0x9600;                                            //access to configuration registers disabled
            return (USB_GENERAL_ERROR);
        }
    } while (USBPLLIR != 0);

    USBCNF     |=    USB_EN;                                                    //enable USB module
    USBKEYPID   =    0x9600;                                                    //access to configuration registers disabled
    return (USB_SUCCEED);
#endif
}

#ifdef USE_TIMER_FOR_RESUME

//*****************************************************************************
//
//! First phase of enabling the USB Module when USE_TIMER_FOR_RESUME is defined
//!
//! This functions is only used by USB_resume to reduce the interrupt latency
//! of the resume interrupt.
//! This function starts the XT2 crystal and then calls an event handler
//! USB_handleCrystalStartedEvent() to allow the application to get control. The
//! application can use a timer or other peripheral to "wait" for the XT2
//! crystal to stabilize. See the crystal datasheet for typical wait times.
//! The application then informs the stack of XT2
//! stabilization by calling USB_enable_PLL().
//!
//! \return \b USB_SUCCEED or USB_GENERAL_ERROR
//
//*****************************************************************************

uint8_t USB_enable_crystal (void)
{
    volatile uint16_t i, k;
    volatile uint16_t j = 0;

    if (!(USBPWRCTL & USBBGVBV)){                                               //check USB Bandgap and VBUS valid
        return (USB_GENERAL_ERROR);
    }

    if ((USBCNF & USB_EN) &&
        (USBPLLCTL & UPLLEN)){
        return (USB_SUCCEED);                                                  //exit if PLL is already enalbed
    }

#if defined (__MSP430F552x) || defined (__MSP430F550x)
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN2);
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN3);
#elif defined (__MSP430F563x_F663x) || defined (__MSP430F565x_F665x) || defined (__MSP430FG6x2x)
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN2);
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN3);
#endif
 
    if(USB_XT2_BYPASS_MODE == FALSE){					//XT2 not in bypass mode
    	if (USB_XT_FREQ_VALUE >= 24) {
    		UCS_turnOnXT2WithTimeout(
    	   			XT2DRIVE_3, 50000);
    	}
    	else if(USB_XT_FREQ_VALUE >= 16) {
    		UCS_turnOnXT2WithTimeout(
    	  			XT2DRIVE_2, 50000);
    	}
    	else if(USB_XT_FREQ_VALUE >= 8) {
    		UCS_turnOnXT2WithTimeout(
    	   			XT2DRIVE_1, 50000);
    	}
    	else {
    		UCS_turnOnXT2WithTimeout(
    	 			XT2DRIVE_0, 50000);
    	}

    }
    else{												//XT2 in bypass mode
    	if (USB_XT_FREQ_VALUE >= 24) {
    		 UCS_bypassXT2WithTimeout(
    			50000);
     	}
    	else if(USB_XT_FREQ_VALUE >= 16) {
    		 UCS_bypassXT2WithTimeout(
    			 50000);
    	}
    	else if(USB_XT_FREQ_VALUE >= 8) {
    		 UCS_bypassXT2WithTimeout(
    			50000);
    	}
    	else {
    		 UCS_bypassXT2WithTimeout(
    			50000);
      	}
    }

    USB_handleCrystalStartedEvent();

    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Second phase of enabling the USB Module when USE_TIMER_FOR_RESUME is defined
//!
//! This functions is only used by USB_resume to reduce the interrupt latency
//! of the resume interrupt.
//! This function starts the PLL and then calls an event handler
//! USB_handlePLLStartedEvent() to allow the application to get control. The
//! application can use a timer or other peripheral to "wait" for the USB PLL
//! to stabilize. See the datasheet for typical PLL wait times.
//! The application then informs the stack of XT2
//! stabilization by calling USB_enable_final().
//!
//! \return \b USB_SUCCEED or USB_GENERAL_ERROR
//
//*****************************************************************************
void USB_enable_PLL(void)
{
    USBKEYPID = 0x9628;                                                         //set KEY and PID to 0x9628 -> access to
                                                                                //configuration registers enabled
    USBPLLDIVB = USB_XT_FREQ;                                                   //Settings desired frequency

    USBPLLCTL = UPFDEN + UPLLEN;                                               //Select XT2 as Ref / Select PLL for USB / Discrim.
                                                                                //on, enable PLL

    USB_handlePLLStartedEvent();
}

//*****************************************************************************
//
//! Final phase of enabling the USB Module when USE_TIMER_FOR_RESUME is defined
//!
//! This function is only used by USB_resume to reduce the interrupt latency
//! of the resume interrupt.
//! This function gets called by the application when thye USB PLL has stabilized
//! to allow the resume process to finish.
//!
//
//*****************************************************************************
void USB_enable_final(void)
{
    USBCNF     |=    USB_EN;                                                    //enable USB module
    USBKEYPID   =    0x9600;                                                    //access to configuration registers disabled
    USBIFG &= ~(RESRIFG | SUSRIFG);     //clear interrupt flags
    USBIE = SETUPIE | RSTRIE | SUSRIE;  //enable USB specific interrupts (setup, reset, suspend)

    bFunctionSuspended  = FALSE;
}

#endif

//*****************************************************************************
//
//! Disables the USB Module and PLL.
//!
//!
//! Disables the USB module and PLL. If USB is not enabled when this call is 
//! made, no error is returned - the call simply exits with success.
//! 
//! If a handleVbusOffEvent() occurs, or if USB_getConnectionState() begins 
//! returning ST_USB_DISCONNECTED, this function should be called (following a 
//! call to USB_disconnect()), in order to avoid unnecessary current draw.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_disable (void)
{
    USBKEYPID = 0x9628;                                                         //set KEY and PID to 0x9628 -> access to
                                                                                //configuration registers enabled
    USBCNF    = 0;                                                              //disable USB module
    USBPLLCTL &= ~UPLLEN;                                                       //disable PLL
    USBKEYPID = 0x9600;                                                         //access to configuration registers disabled
    bEnumerationStatus = 0x00;                                                  //device is not enumerated
    bFunctionSuspended = FALSE;                                                 //device is not suspended
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Enables/Disables the Various USB Events.
//!
//! \param events is the mask for what is to be enabled/disabled.
//!       - Valid values are:
//!        		- \b USB_CLOCK_FAULT_EVENT
//!        		- \b USB_VBUS_ON_EVENT
//!        		- \b USB_VBUS_OFF_EVENT
//!        		- \b USB_RESET_EVENT
//!        		- \b USB_SUSPENDED_EVENT
//!        		- \b USB_RESUME_EVENT
//!        		- \b USB_DATA_RECEIVED_EVENT
//!        		- \b USB_SEND_COMPLETED_EVENT
//!        		- \b USB_RECEIVED_COMPLETED_EVENT
//!        		- \b USB_ALL_USB_EVENTS
//!
//! Enables/disables various USB events. Within the events byte, all bits with
//! '1' values will be enabled, and all bits with '0' values will be disabled.
//! (There are no bit-wise operations). By default (that is, prior to any call 
//! to this function), all events are disabled.
//! 
//! The status of event enabling can be read with the USB_getEnabledEvents() 
//! function. This call can be made at any time after a call to USB_init().
//! 
//! USB_setEnabledEvents() can be thought of in a similar fashion to 
//! setting/clearing interrupt enable bits. The only benefit in keeping an event 
//! disabled is to save the unnecessary execution cycles incurred from running 
//! an "empty" event handler.
//! 
//! The mask constant \b USB_ALL_USB_EVENTS is used to enable/disable all events 
//! pertaining to core USB functions; in other words, it enables all those with 
//! a \b kUSB_ prefix. 
//! 
//! See Sec. 10 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for more information about
//! events.
//! 
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_setEnabledEvents (uint16_t events)
{
    wUsbEventMask = events;
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Returns Which Events are Enabled/Disabled.
//!
//! Returns which events are enabled and which are disabled. The definition of 
//! events is the same as for USB_enableEvents() above.
//! 
//! If the bit is set, the event is enabled. If cleared, the event is disabled. 
//! By default (that is, prior to calling USB_setEnabledEvents() ), all events 
//! are disabled. This call can be made at any time after a call to USB_init().
//! 
//! \return \b Events
//
//*****************************************************************************

uint16_t USB_getEnabledEvents ()
{
    return (wUsbEventMask);
}

//*****************************************************************************
//
//! Resets the USB Module and the Internal State of the API.
//!
//! Resets the USB module and also the internal state of the API. The interrupt 
//! register is cleared to make sure no interrupts are pending. If the device 
//! had been enumerated, the enumeration is now lost. All open send/receive 
//! operations are aborted. 
//! 
//! This function is most often called immediately before a call to 
//! USB_connect(). It should not be called prior to USB_enable().
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_reset ()
{
    int16_t i;

    USBKEYPID = 0x9628;                                                             //set KEY and PID to 0x9628 -> access to
                                                                                    //configuration registers enabled

    //reset should be on the bus after this!
    bEnumerationStatus = 0x00;                                                      //Device not enumerated yet
    bFunctionSuspended = FALSE;                                                     //Device is not in suspend mode

    bRemoteWakeup = DISABLE;

    bConfigurationNumber    = 0x00;                                                 //device unconfigured
    bInterfaceNumber        = 0x00;

    //FRSTE handling:
    //Clear FRSTE in the RESRIFG interrupt service routine before re-configuring USB control registers.
    //Set FRSTE at the beginning of SUSRIFG, SETUP, IEPIFG.EP0 and OEPIFG.EP0 interrupt service routines.
    USBCTL = 0;                                                                     //Function Reset Connection disable (FRSTE)

    wBytesRemainingOnIEP0   = NO_MORE_DATA;
    wBytesRemainingOnOEP0   = NO_MORE_DATA;
    bStatusAction           = STATUS_ACTION_NOTHING;

    //The address reset normally will be done automatically during bus function reset
    USBFUNADR   =     0x00;                                                         //reset address of USB device (unconfigured)

    /* Set settings for EP0 */
    //NAK both 0 endpoints and enable endpoint 0 interrupt
    tEndPoint0DescriptorBlock.bIEPBCNT = EPBCNT_NAK;
    tEndPoint0DescriptorBlock.bOEPBCNT = EPBCNT_NAK;
    tEndPoint0DescriptorBlock.bIEPCNFG = EPCNF_USBIE | EPCNF_UBME | EPCNF_STALL;    //8 byte data packet
    tEndPoint0DescriptorBlock.bOEPCNFG = EPCNF_USBIE | EPCNF_UBME | EPCNF_STALL;    //8 byte data packet

    USBOEPIE = USB_OUTEP_INT_EN;
    USBIEPIE = USB_INEP_INT_EN;

    //loop for initialization all of used enpoints
    for (i = 0;
         i < (CDC_NUM_INTERFACES + HID_NUM_INTERFACES + MSC_NUM_INTERFACES + PHDC_NUM_INTERFACES);
         i++)
    {
        uint8_t edbIndex = stUsbHandle[i].edb_Index;

        /* Set settings for IEPx */
        tInputEndPointDescriptorBlock[edbIndex].bEPCNF   = EPCNF_USBIE |
                                                           EPCNF_UBME |
                                                           EPCNF_DBUF;              //double buffering
        tInputEndPointDescriptorBlock[edbIndex].bEPBBAX  =
            (uint8_t)(((stUsbHandle[i].iep_X_Buffer -
                     START_OF_USB_BUFFER) >> 3) & 0x00ff);
        tInputEndPointDescriptorBlock[edbIndex].bEPBBAY  =
            (uint8_t)(((stUsbHandle[i].iep_Y_Buffer -
                     START_OF_USB_BUFFER) >> 3) & 0x00ff);
        tInputEndPointDescriptorBlock[edbIndex].bEPBCTX  = EPBCNT_NAK;
        tInputEndPointDescriptorBlock[edbIndex].bEPBCTY  = EPBCNT_NAK;
        tInputEndPointDescriptorBlock[edbIndex].bEPSIZXY = MAX_PACKET_SIZE;

        /* Set settings for OEPx */

        tOutputEndPointDescriptorBlock[edbIndex].bEPCNF   = EPCNF_USBIE |
                                                            EPCNF_UBME |
                                                            EPCNF_DBUF ;            //double buffering
        
        tOutputEndPointDescriptorBlock[edbIndex].bEPBBAX  =
            (uint8_t)(((stUsbHandle[i].oep_X_Buffer -
                     START_OF_USB_BUFFER) >> 3) & 0x00ff);
        tOutputEndPointDescriptorBlock[edbIndex].bEPBBAY  =
            (uint8_t)(((stUsbHandle[i].oep_Y_Buffer -
                     START_OF_USB_BUFFER) >> 3) & 0x00ff);
        tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX  = 0x00;
        tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY  = 0x00;
        tOutputEndPointDescriptorBlock[edbIndex].bEPSIZXY = MAX_PACKET_SIZE;

#       ifdef _CDC_
        /* Additional interrupt end point for CDC */
        if (stUsbHandle[i].dev_Class == CDC_CLASS){
            //The decriptor tool always generates the managemnet endpoint before the data endpoint
            tInputEndPointDescriptorBlock[edbIndex -
                                          1].bEPCNF   = EPCNF_USBIE |
                                                        EPCNF_UBME | EPCNF_DBUF;    //double buffering
            tInputEndPointDescriptorBlock[edbIndex -
                                          1].bEPBBAX  =
                (uint8_t)(((stUsbHandle[i].intepEP_X_Buffer -
                         START_OF_USB_BUFFER) >> 3) & 0x00ff);
            tInputEndPointDescriptorBlock[edbIndex -
                                          1].bEPBBAY  =
                (uint8_t)(((stUsbHandle[i].intepEP_Y_Buffer -
                         START_OF_USB_BUFFER) >> 3) & 0x00ff);
            tInputEndPointDescriptorBlock[edbIndex - 1].bEPBCTX  = EPBCNT_NAK;
            tInputEndPointDescriptorBlock[edbIndex - 1].bEPBCTY  = EPBCNT_NAK;
            tInputEndPointDescriptorBlock[edbIndex -
                                          1].bEPSIZXY = MAX_PACKET_SIZE;
        }
#       endif
    }

#   ifdef _HID_
    HidResetData();                                                                 //reset HID specific data structures
#   endif //_HID_

#   ifdef _MSC_
    MscState.isMSCConfigured = FALSE;
    USBMSC_reset();
    MscResetData();
#   endif

#   ifdef _CDC_
    CdcResetData();                                                                 //reset CDC specific data structures
#   endif //_CDC_

#   ifdef _PHDC_
        PHDCResetData();                     // reset CDC specific data structures
#   endif // _PHDC_

    USBCTL = FEN;                                                                   //enable function
    USBIFG = 0;                                                                     //make sure no interrupts are pending

    USBIE = SETUPIE | RSTRIE | SUSRIE;                                              //enable USB specific interrupts (setup, reset,
                                                                                    //suspend)
    USBKEYPID = 0x9600;                                                             //access to configuration registers disabled
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Makes USB Module Available to Host for Connection.
//!
//! Instructs the USB module to make itself available to the host for 
//! connection, by pulling the D+ signal high using the PUR pin. This call 
//! should only be made after a call to USB_enable().
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_connect ()
{
    USBKEYPID = 0x9628;                                                             //set KEY and PID to 0x9628 -> access to
                                                                                    //configuration registers enabled
    USBCNF |= PUR_EN;                                                               //generate rising edge on DP -> the host
                                                                                    //enumerates our device as full speed device
    USBPWRCTL |= VBOFFIE;                                                           //enable interrupt VUSBoff
    USBKEYPID = 0x9600;                                                             //access to configuration registers disabled

    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Forces a Disconnect From the USB Host.
//!
//! Forces a logical disconnect from the USB host by pulling the PUR pin low, 
//! removing the pullup on the D+ signal. The USB module and PLL remain enabled.
//! If the USB is not connected when this call is made, no error is returned -
//! the call simply exits with success after ensuring PUR is low.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USB_disconnect ()
{
    USBKEYPID = 0x9628;                                                             //set KEY and PID to 0x9628 -> access to
                                                                                    //configuration registers enabled
    USBCNF &= ~PUR_EN;                                                              //disconnect pull up resistor - logical
                                                                                    //disconnect from HOST
    USBPWRCTL &= ~VBOFFIE;                                                          //disable interrupt VUSBoff
    USBKEYPID = 0x9600;                                                             //access to configuration registers disabled
    bEnumerationStatus = 0;                                                         //not enumerated
    bFunctionSuspended = FALSE;                                                     //device is not suspended
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Remote Wakeup of USB Host.
//!
//! Prompts a remote wakeup of the USB host. The user must ensure that the USB 
//! descriptors had indicated remote wakeup capability (using the Descriptor 
//! Tool); otherwise the host will ignore the request.
//! 
//! If the function returns \b USB_GENERAL_ERROR, it means that the host did not 
//! grant the device the ability to perform a remote wakeup, when it enumerated 
//! the device.
//!
//! \return \b USB_SUCCEED, \b kUSBgeneralError or \b kUSB_notSuspended.
//
//*****************************************************************************

uint8_t USB_forceRemoteWakeup ()
{
    if (bFunctionSuspended == FALSE){                                               //device is not suspended
        return (USB_NOT_SUSPENDED);
    }
    if (bRemoteWakeup == ENABLE){
        volatile uint16_t i;
        USBCTL |= RWUP;                                                             //USB - Device Remote Wakeup Request - this bit
                                                                                    //is self-cleaned
        return (USB_SUCCEED);
    }
    return (USB_GENERAL_ERROR);
}

//*****************************************************************************
//
//! Gets Connection Info.
//! 
//! Returns low-level status information about the USB connection.
//! 
//! Because multiple flags can be returned, the possible values can be masked 
//! together - for example, \b USB_VBUS_PRESENT + \b USB_SUSPENDED.
//!
//! \return A single mask that is the all the statuses together and may
//! 		consist of the following:
//! 				- \b USB_PUR_HIGH
//! 				- \b USB_SUSPENDED
//! 				- \b USB_NOT_SUSPENDED
//! 				- \b USB_ENUMERATED
//! 				- \b USB_VBUS_PRESENT
//
//*****************************************************************************

uint8_t USB_getConnectionInformation ()
{
    uint8_t retVal = 0;

    if (USBPWRCTL & USBBGVBV){
        retVal |= USB_VBUS_PRESENT;
    }

    if (bEnumerationStatus == ENUMERATION_COMPLETE){
        retVal |= USB_ENUMERATED;
    }

    if (USBCNF & PUR_EN){
        retVal |= USB_PUR_HIGH;
    }

    if (bFunctionSuspended == TRUE){
        retVal |= USB_SUSPENDED;
    } else {
        retVal |= USB_NOT_SUSPENDED;
    }
    return (retVal);
}

//*****************************************************************************
//
//! Gets State of the USB Connection.
//!
//! Returns the state of the USB connection, according to the state diagram 
//! in Sec. 6 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC".
//! 
//! \return Any of the following: 
//! 			- \b ST_USB_DISCONNECTED
//! 			- \b ST_USB_CONNECTED_NO_ENUM
//! 			- \b ST_ENUM_IN_PROGRESS
//! 			- \b ST_ENUM_ACTIVE
//! 			- \b ST_ENUM_SUSPENDED
//! 			- \b ST_NOENUM_SUSPENDED,
//! 			- \b ST_ERROR.
//
//*****************************************************************************

uint8_t USB_getConnectionState ()
{
    //If no VBUS present
    if (!(USBPWRCTL & USBBGVBV)){
        return (ST_USB_DISCONNECTED);
    }

    //If VBUS present, but PUR is low
    if ((USBPWRCTL & USBBGVBV) && (!(USBCNF & PUR_EN))){
        return (ST_USB_CONNECTED_NO_ENUM);
    }

    //If VBUS present, PUR is high, and enumeration is complete, and not suspended
    if ((USBPWRCTL & USBBGVBV) && (USBCNF & PUR_EN)
        && (bEnumerationStatus == ENUMERATION_COMPLETE)
        && (!(bFunctionSuspended == TRUE))){
        return (ST_ENUM_ACTIVE);
    }

    //If VBUS present, PUR is high, and enumeration is NOT complete, and  suspended
    if ((USBPWRCTL & USBBGVBV) && (USBCNF & PUR_EN)
        && (!(bEnumerationStatus == ENUMERATION_COMPLETE))
        && (bFunctionSuspended == TRUE)){
        return (ST_NOENUM_SUSPENDED);
    }

    //If VBUS present, PUR is high, and enumeration is complete, and  suspended
    if ((USBPWRCTL & USBBGVBV) && (USBCNF & PUR_EN)
        && (bEnumerationStatus == ENUMERATION_COMPLETE)
        && (bFunctionSuspended == TRUE)){
        return (ST_ENUM_SUSPENDED);
    }

    //If VBUS present, PUR is high, but no enumeration yet
    if ((USBPWRCTL & USBBGVBV) && (USBCNF & PUR_EN)
        && (!(bEnumerationStatus == ENUMERATION_COMPLETE))){
        return (ST_ENUM_IN_PROGRESS);
    }

    return (ST_ERROR);
}

//
//! \cond
//

//----------------------------------------------------------------------------

uint8_t USB_suspend (void)
{
    bFunctionSuspended  = TRUE;
    USBKEYPID = 0x9628;                 //set KEY and PID to 0x9628 -> access to configuration registers enabled
    USBCTL |= FRSTE;                    //Function Reset Connection Enable
    USBIFG &= ~SUSRIFG;                 //clear interrupt flag

    USBPLLCTL &= ~UPLLEN;

    if (USB_DISABLE_XT_SUSPEND){
        UCS_turnOffXT2();         //disable XT2
    }

    USBIE = RESRIE;                     //disable USB specific interrupts (setup, suspend, reset), enable resume.
                                        //If the reset occured during device in suspend, the resume-interrupt will come, after -
                                        //reset interrupt
    USBKEYPID = 0x9600;                 //access to configuration registers disabled

    return (USB_SUCCEED);
}

//----------------------------------------------------------------------------

uint8_t USB_resume (void)
{
    USB_enable();                       //enable PLL

    USBIFG &= ~(RESRIFG | SUSRIFG);     //clear interrupt flags
    USBIE = SETUPIE | RSTRIE | SUSRIE;  //enable USB specific interrupts (setup, reset, suspend)

    bFunctionSuspended  = FALSE;
    return (USB_SUCCEED);
}

//----------------------------------------------------------------------------

void usbStallEndpoint0 (void)
{
    tEndPoint0DescriptorBlock.bIEPCNFG |= EPCNF_STALL;
    tEndPoint0DescriptorBlock.bOEPCNFG |= EPCNF_STALL;
}

//----------------------------------------------------------------------------

void usbClearOEP0ByteCount (void)
{
    tEndPoint0DescriptorBlock.bOEPBCNT = 0x00;
}

//----------------------------------------------------------------------------

void usbStallOEP0 (void)
{
    //in standard USB request, there is not control write request with data stage
    //control write, stall output endpoint 0
    //wLength should be 0 in all cases
    tEndPoint0DescriptorBlock.bOEPCNFG |= EPCNF_STALL;
}

//----------------------------------------------------------------------------

void usbSendNextPacketOnIEP0 (void)
{
    uint8_t bPacketSize,bIndex;

    //First check if there are bytes remaining to be transferred
    if (wBytesRemainingOnIEP0 != NO_MORE_DATA){
        if (wBytesRemainingOnIEP0 > EP0_PACKET_SIZE){
            //More bytes are remaining than will fit in one packet
            //there will be More IN Stage
            bPacketSize = EP0_PACKET_SIZE;
            wBytesRemainingOnIEP0 -= EP0_PACKET_SIZE;
            bStatusAction = STATUS_ACTION_DATA_IN;
        } else if (wBytesRemainingOnIEP0 < EP0_PACKET_SIZE){
            //The remaining data will fit in one packet.
            //This case will properly handle wBytesRemainingOnIEP0 == 0
            bPacketSize = (uint8_t)wBytesRemainingOnIEP0;
            wBytesRemainingOnIEP0 = NO_MORE_DATA;   //No more data need to be Txed
            bStatusAction = STATUS_ACTION_NOTHING;
        } else {
            bPacketSize = EP0_PACKET_SIZE;
            if (bHostAskMoreDataThanAvailable == TRUE){
                wBytesRemainingOnIEP0 = 0;
                bStatusAction = STATUS_ACTION_DATA_IN;
            } else {
                wBytesRemainingOnIEP0 = NO_MORE_DATA;
                bStatusAction = STATUS_ACTION_NOTHING;
            }
        }

        for (bIndex = 0; bIndex < bPacketSize; bIndex++)
        {
            abIEP0Buffer[bIndex] = *pbIEP0Buffer;
            pbIEP0Buffer++;
        }
        tEndPoint0DescriptorBlock.bIEPBCNT = bPacketSize;
    } else {
        bStatusAction = STATUS_ACTION_NOTHING;
    }
}

//----------------------------------------------------------------------------

void usbSendDataPacketOnEP0 (const uint8_t* pbBuffer)
{
    uint16_t wTemp;

    pbIEP0Buffer = pbBuffer;
    wTemp = tSetupPacket.wLength;

    //Limit transfer size to wLength if needed
    //this prevent USB device sending 'more than require' data back to host
    if (wBytesRemainingOnIEP0 >= wTemp){
        wBytesRemainingOnIEP0 = wTemp;
        bHostAskMoreDataThanAvailable = FALSE;
    } else {
        bHostAskMoreDataThanAvailable = TRUE;
    }
    usbSendNextPacketOnIEP0();
}

//----------------------------------------------------------------------------
void usbReceiveNextPacketOnOEP0 (void)
{
    uint8_t bIndex,bByte;

    bByte = tEndPoint0DescriptorBlock.bOEPBCNT & EPBCNT_BYTECNT_MASK;

    if (wBytesRemainingOnOEP0 >= (uint16_t)bByte){
        for (bIndex = 0; bIndex < bByte; bIndex++)
        {
            *pbOEP0Buffer = abOEP0Buffer[bIndex];
            pbOEP0Buffer++;
        }
        wBytesRemainingOnOEP0 -= (uint16_t)bByte;

        //clear the NAK bit for next packet
        if (wBytesRemainingOnOEP0 > 0){
            usbClearOEP0ByteCount();
            bStatusAction = STATUS_ACTION_DATA_OUT;
        } else {
            usbStallOEP0();
            bStatusAction = STATUS_ACTION_NOTHING;
        }
    } else {
        usbStallOEP0();
        bStatusAction = STATUS_ACTION_NOTHING;
    }
}

//----------------------------------------------------------------------------

void usbReceiveDataPacketOnEP0 (uint8_t* pbBuffer)
{
    pbOEP0Buffer = pbBuffer;

    wBytesRemainingOnOEP0 = tSetupPacket.wLength;
    bStatusAction = STATUS_ACTION_DATA_OUT;

    usbClearOEP0ByteCount();
}

//----------------------------------------------------------------------------

void usbSendZeroLengthPacketOnIEP0 (void)
{
    wBytesRemainingOnIEP0 = NO_MORE_DATA;
    bStatusAction = STATUS_ACTION_NOTHING;
    tEndPoint0DescriptorBlock.bIEPBCNT = 0x00;
}

//----------------------------------------------------------------------------

uint8_t usbClearEndpointFeature (void)
{
    uint8_t bEndpointNumber;

    //EP is from EP1 to EP7 while C language start from 0
    bEndpointNumber = (tSetupPacket.wIndex & EP_DESC_ADDR_EP_NUM);
    if (bEndpointNumber == 0x00){
        usbSendZeroLengthPacketOnIEP0();
    } else {
        bEndpointNumber--;
        if (bEndpointNumber < MAX_ENDPOINT_NUMBER){
            if ((tSetupPacket.wIndex & EP_DESC_ADDR_DIR_IN) ==
                EP_DESC_ADDR_DIR_IN){
#ifdef _MSC_
                if (!MscState.bMscResetRequired){
#endif
                tInputEndPointDescriptorBlock[bEndpointNumber].bEPCNF &=
                    ~(EPCNF_STALL | EPCNF_TOGGLE );
#ifdef _MSC_
            }
#endif
#               ifdef _MSC_
                if (stUsbHandle[MSC0_INTFNUM].edb_Index == bEndpointNumber){
                    MscReadControl.bCurrentBufferXY = 0;    //Set current buffer to X
                    MscState.bMcsCommandSupported = TRUE;
                }
#               endif
            } else {
#ifdef _MSC_
                if (!MscState.bMscResetRequired){
#endif
					tOutputEndPointDescriptorBlock[bEndpointNumber].bEPCNF &=
						~(EPCNF_STALL | EPCNF_TOGGLE );
#ifdef _MSC_						
					tOutputEndPointDescriptorBlock[bEndpointNumber].bEPBCTX = 0;
                    tOutputEndPointDescriptorBlock[bEndpointNumber].bEPBCTY = 0;
                    MscState.stallEndpoint = FALSE;
#endif					
#ifdef _MSC_
            }
#endif
#               ifdef _MSC_
                if (stUsbHandle[MSC0_INTFNUM].edb_Index == bEndpointNumber){
                    MscWriteControl.bCurrentBufferXY = 0;   //Set current buffer to X
                    MscState.bMcsCommandSupported = TRUE;
                }
#               endif
            }
            usbSendZeroLengthPacketOnIEP0();
        }
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetConfiguration (void)
{
    usbClearOEP0ByteCount();                                //for status stage
    wBytesRemainingOnIEP0 = 1;
    usbSendDataPacketOnEP0((uint8_t*)&bConfigurationNumber);

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetDeviceDescriptor (void)
{
    usbClearOEP0ByteCount();
    wBytesRemainingOnIEP0 = SIZEOF_DEVICE_DESCRIPTOR;
#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES
    usbSendDataPacketOnEP0((uint8_t*)usbDeviceDescriptors[activeInterfaceIndex]);
#else
    usbSendDataPacketOnEP0((uint8_t*)&abromDeviceDescriptor);
#endif
    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetConfigurationDescriptor (void)
{
    usbClearOEP0ByteCount();
#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES
    wBytesRemainingOnIEP0 = usbConfigurationsSizes[activeInterfaceIndex];
    usbSendDataPacketOnEP0((uint8_t*)usbConfigurationDescriptors[activeInterfaceIndex]);
#else
    wBytesRemainingOnIEP0 = sizeof(abromConfigurationDescriptorGroup);
    usbSendDataPacketOnEP0((uint8_t*)&abromConfigurationDescriptorGroup);
#endif	

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetStringDescriptor (void)
{
    uint16_t bIndex;
    uint8_t bVal = (uint8_t)tSetupPacket.wValue;

    usbClearOEP0ByteCount();    //for status stage
    if (bVal <= MAX_STRING_DESCRIPTOR_INDEX) {
#if (USB_STR_INDEX_SERNUM != 0)
        if (bVal == 0x03){
            wBytesRemainingOnIEP0 = abramSerialStringDescriptor[0];
            usbSendDataPacketOnEP0((uint8_t*)&abramSerialStringDescriptor);
        } else
#endif
        {
            bIndex = 0x00;
            while (bVal-- >  0x00) bIndex += abromStringDescriptor[bIndex];
            wBytesRemainingOnIEP0 = abromStringDescriptor[bIndex];
            usbSendDataPacketOnEP0((uint8_t*)&abromStringDescriptor[bIndex]);
        }
    }
    else {
        usbStallEndpoint0();
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetInterface (void)
{
    //not fully supported, return one byte, zero
    usbClearOEP0ByteCount();            //for status stage
    wBytesRemainingOnIEP0 = 0x02;
    abUsbRequestReturnData[0] = 0x00;   //changed to report alternative setting byte
    abUsbRequestReturnData[1] = bInterfaceNumber;
    usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetDeviceStatus (void)
{
#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES
	if ((((struct abromConfigurationDescriptorGroup *)
		usbConfigurationDescriptors[activeInterfaceIndex])->
		abromConfigurationDescriptorGenric.mattributes  &
		CFG_DESC_ATTR_SELF_POWERED) == CFG_DESC_ATTR_SELF_POWERED){
        abUsbRequestReturnData[0] = DEVICE_STATUS_SELF_POWER;
    }
#else
    if ((abromConfigurationDescriptorGroup.abromConfigurationDescriptorGenric.
         mattributes &
         CFG_DESC_ATTR_SELF_POWERED) == CFG_DESC_ATTR_SELF_POWERED){
        abUsbRequestReturnData[0] = DEVICE_STATUS_SELF_POWER;
    }
#endif	
    if (bRemoteWakeup == ENABLE){
        abUsbRequestReturnData[0] |= DEVICE_STATUS_REMOTE_WAKEUP;
    }
    usbClearOEP0ByteCount();            //for status stage

    //Return self power status and remote wakeup status
    wBytesRemainingOnIEP0 = 2;
    usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetInterfaceStatus (void)
{
    //check bIndexL for index number (not supported)
    usbClearOEP0ByteCount();            //for status stage

    //Return two zero bytes
    wBytesRemainingOnIEP0 = 2;
    abUsbRequestReturnData[0] = 0x00;   //changed to support multiple interfaces
    abUsbRequestReturnData[1] = bInterfaceNumber;
    usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbGetEndpointStatus (void)
{
    uint8_t bEndpointNumber;

    //Endpoint number is bIndexL
    bEndpointNumber = tSetupPacket.wIndex & EP_DESC_ADDR_EP_NUM;
    if (bEndpointNumber == 0x00){
        if ((tSetupPacket.wIndex & EP_DESC_ADDR_DIR_IN) ==
            EP_DESC_ADDR_DIR_IN){
            //input endpoint 0
            abUsbRequestReturnData[0] =
                (uint8_t)(tEndPoint0DescriptorBlock.bIEPCNFG & EPCNF_STALL);
        } else {
            //output endpoint 0
            abUsbRequestReturnData[0] =
                (uint8_t)(tEndPoint0DescriptorBlock.bOEPCNFG & EPCNF_STALL);
        }
        abUsbRequestReturnData[0] = abUsbRequestReturnData[0] >> 3; //STALL is on bit 3
        usbClearOEP0ByteCount();                                    //for status stage
        wBytesRemainingOnIEP0 = 0x02;
        usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);
    } else {
        bEndpointNumber--;
        //EP is from EP1 to EP7 while C language start from 0
        //Firmware should NOT response if specified endpoint is not supported. (charpter 8)
        if (bEndpointNumber < MAX_ENDPOINT_NUMBER){
            if (tSetupPacket.wIndex & EP_DESC_ADDR_DIR_IN){
                //input endpoint
                abUsbRequestReturnData[0] =
                    (uint8_t)(tInputEndPointDescriptorBlock[bEndpointNumber].
                           bEPCNF &
                           EPCNF_STALL);
            } else {
                //output endpoint
                abUsbRequestReturnData[0] =
                    (uint8_t)(tOutputEndPointDescriptorBlock[bEndpointNumber].
                           bEPCNF &
                           EPCNF_STALL);
            }
        }                                                           //no response if endpoint is not supported.
        abUsbRequestReturnData[0] = abUsbRequestReturnData[0] >> 3; //STALL is on bit 3
        usbClearOEP0ByteCount();
        wBytesRemainingOnIEP0 = 0x02;
        usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);
    }

    return (FALSE);
}

//----------------------------------------------------------------------------
uint8_t usbSetAddress (void)
{
    usbStallOEP0();                                                 //control write without data stage

    //bValueL contains device address
    if (tSetupPacket.wValue < 128){
        //hardware will update the address after status stage
        //therefore, firmware can set the address now.
        USBFUNADR = tSetupPacket.wValue;
        usbSendZeroLengthPacketOnIEP0();
    } else {
        usbStallEndpoint0();
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetConfiguration (void)
{
	uint8_t bWakeUp = FALSE;

    usbStallOEP0();                                                 //control write without data stage

    //configuration number is in bValueL
    //change the code if more than one configuration is supported
    bConfigurationNumber = tSetupPacket.wValue;
    usbSendZeroLengthPacketOnIEP0();

    if (bConfigurationNumber == 1){
        bEnumerationStatus = ENUMERATION_COMPLETE;                  //set device as enumerated
        //perform enumeration complete event:
        bWakeUp = USB_handleEnumerationCompleteEvent();
    } else {
        bEnumerationStatus = 0;                                     //device is not configured == config # is zero
    }

    return (bWakeUp);
}

//----------------------------------------------------------------------------

uint8_t usbClearDeviceFeature (void)
{
    //bValueL contains feature selector
    if (tSetupPacket.wValue == FEATURE_REMOTE_WAKEUP){
        bRemoteWakeup = DISABLE;
        usbSendZeroLengthPacketOnIEP0();
    } else {
        usbStallEndpoint0();
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetDeviceFeature (void)
{
    //bValueL contains feature selector
    if (tSetupPacket.wValue == FEATURE_REMOTE_WAKEUP){
        bRemoteWakeup = ENABLE;
        usbSendZeroLengthPacketOnIEP0();
    } else {
        usbStallEndpoint0();
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetEndpointFeature (void)
{
    uint8_t bEndpointNumber;

    //wValue contains feature selector
    //bIndexL contains endpoint number
    //Endpoint number is in low byte of wIndex
    if (tSetupPacket.wValue == FEATURE_ENDPOINT_STALL){
        bEndpointNumber = tSetupPacket.wIndex & EP_DESC_ADDR_EP_NUM;
        if (bEndpointNumber == 0x00){
            usbSendZeroLengthPacketOnIEP0();    //do nothing for endpoint 0
        } else {
            bEndpointNumber--;
            //Firmware should NOT response if specified endpoint is not supported. (charpter 8)
            if (bEndpointNumber < MAX_ENDPOINT_NUMBER){
                if (tSetupPacket.wIndex & EP_DESC_ADDR_DIR_IN){
                    //input endpoint
                    tInputEndPointDescriptorBlock[bEndpointNumber].bEPCNF |=
                        EPCNF_STALL;
                } else {
                    //output endpoint
                    tOutputEndPointDescriptorBlock[bEndpointNumber].bEPCNF |=
                        EPCNF_STALL;
                }
                usbSendZeroLengthPacketOnIEP0();
            } //no response if endpoint is not supported.
        }
    } else {
        usbStallEndpoint0();
    }

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetInterface (void)
{
    //bValueL contains alternative setting
    //bIndexL contains interface number
    //change code if more than one interface is supported
    usbStallOEP0();                         //control write without data stage
    bInterfaceNumber = tSetupPacket.wIndex;
#ifdef _MSC_
    tInputEndPointDescriptorBlock[stUsbHandle[MSC0_INTFNUM].edb_Index].bEPCNF
        &= ~(EPCNF_TOGGLE);
    tOutputEndPointDescriptorBlock[stUsbHandle[MSC0_INTFNUM].edb_Index].bEPCNF
        &= ~(EPCNF_TOGGLE);
    MscReadControl.bCurrentBufferXY = 0;    //Set current buffer to X
    MscWriteControl.bCurrentBufferXY = 0;   //Set current buffer to X
#endif
    usbSendZeroLengthPacketOnIEP0();

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbInvalidRequest (void)
{
    //check if setup overwrite is set
    //if set, do nothing since we might decode it wrong
    //setup packet buffer could be modified by hardware if another setup packet
    //was sent while we are deocding setup packet
    if ((USBIFG & STPOWIFG) == 0x00){
        usbStallEndpoint0();
    }

    return (FALSE);
}

typedef uint8_t (*tpF)(void);

uint8_t usbDecodeAndProcessUsbRequest (void)
{
    uint8_t bMask,bResult,bTemp;
    const uint8_t* pbUsbRequestList;
    uint8_t bWakeUp = FALSE;
    ptDEVICE_REQUEST ptSetupPacket = &tSetupPacket;
    uint8_t bRequestType,bRequest;
    tpF lAddrOfFunction;

    //point to beginning of the matrix
    pbUsbRequestList = (uint8_t*)&tUsbRequestList[0];

    while (1)
    {
        bRequestType = *pbUsbRequestList++;
        bRequest     = *pbUsbRequestList++;

        if (((bRequestType == 0xff) && (bRequest == 0xff)) ||
            (tSetupPacket.bmRequestType ==
             (USB_REQ_TYPE_INPUT | USB_REQ_TYPE_VENDOR |
              USB_REQ_TYPE_DEVICE)) ||
            (tSetupPacket.bmRequestType ==
             (USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_VENDOR |
              USB_REQ_TYPE_DEVICE))){
            pbUsbRequestList -= 2;
            break;
        }

        if ((bRequestType == tSetupPacket.bmRequestType) &&
            (bRequest == tSetupPacket.bRequest)){
            //compare the first two
            bResult = 0xc0;
            bMask   = 0x20;
            //first two bytes matched, compare the rest
            for (bTemp = 2; bTemp < 8; bTemp++)
            {
                if (*((uint8_t*)ptSetupPacket + bTemp) == *pbUsbRequestList){
                    bResult |= bMask;
                }
                pbUsbRequestList++;
                bMask = bMask >> 1;
            }
            //now we have the result
            if ((*pbUsbRequestList & bResult) == *pbUsbRequestList){
                pbUsbRequestList -= 8;
                break;
            } else {
                pbUsbRequestList += (sizeof(tDEVICE_REQUEST_COMPARE) - 8);
            }
        } else {
            pbUsbRequestList += (sizeof(tDEVICE_REQUEST_COMPARE) - 2);
        }
    }

    //if another setup packet comes before we have the chance to process current
    //setup request, we return here without processing the request
    //this check is not necessary but still kept here to reduce response(or simulation) time

    if ((USBIFG & STPOWIFG) != 0x00){
        return (bWakeUp);
    }

    //now we found the match and jump to the function accordingly.
    lAddrOfFunction =
        ((tDEVICE_REQUEST_COMPARE*)pbUsbRequestList)->pUsbFunction;

    //call function
    bWakeUp = (*lAddrOfFunction)();

    return (bWakeUp);
}

uint16_t usbDisableInEndpointInterrupt(uint8_t edbIndex)
{
	uint16_t state;
	state = USBIEPIE & (1 << (edbIndex + 1));
	USBIEPIE &= ~(1 << (edbIndex + 1));
	return (state);
}
void usbRestoreInEndpointInterrupt(uint16_t state)
{
	USBIEPIE |= state;
}
uint16_t usbDisableOutEndpointInterrupt(uint8_t edbIndex)
{
	uint16_t state;
	state = USBOEPIE & (1 << (edbIndex + 1));
	USBOEPIE &= ~(1 << (edbIndex + 1));
	return (state);
}
void usbRestoreOutEndpointInterrupt(uint16_t state)
{
	USBOEPIE |= state;
}
#ifdef NON_COMPOSITE_MULTIPLE_INTERFACES

uint8_t USB_switchInterface(uint8_t interfaceIndex)
{
	if (interfaceIndex < NONCOMP_NUM_USB_INTERFACES) {
		activeInterfaceIndex = interfaceIndex;
		return (TRUE);
	}
	else {
		return (FALSE);
	}
}

#endif

uint16_t USB_determineFreq(void){
    uint16_t freq;                  // calculated MCLK freq in kHz
    uint16_t currentFLLN;           // value of divider N taken from UCS registers
    uint8_t currentSELM;           // MCLK reference taken from UCS registers
    uint8_t currentFLLREFDIV;      // value of divider n taken from UCS registers
    uint16_t currentFLLD;           // value of prescalar D taken from UCS registers
    uint16_t FLLRefFreq;

    currentSELM = (UCSCTL4_L & SELM_7);   // get clock selection control register

    if(currentSELM<=4) // MCLK = DCO, DCOCLKDIV, XT1, VLO, or REFO.  The last three aren't supported by the API.
    {
        FLLRefFreq = 33;                    // The reference is usually 32.768 kHz.
        if((UCSCTL3_L & SELREF_7) >= 0x50){  // Unless it's XT2 frequency
            FLLRefFreq = USB_XT_FREQ_VALUE * 1000;
        }

        // determine factors N and n
        currentFLLN = UCSCTL2 & 0x03FF;          // get FLL multiplier register
        currentFLLN++;
        if(currentSELM == SELM_3)            // if MCLK is sourced by DCOCLK
        {
            // determine D
            currentFLLD = UCSCTL2 & FLLD_7;  // get FLLD register
            currentFLLD >>= 12;
            currentFLLN <<= currentFLLD;
        }

        currentFLLREFDIV = UCSCTL3_L & FLLREFDIV_7; // get FLL reference divider register
        if (currentFLLREFDIV == 0) {
            freq = currentFLLN * (FLLRefFreq / 1);
        }
        else if (currentFLLREFDIV == 1) {
            freq = currentFLLN * (FLLRefFreq / 2);
        }
        else if (currentFLLREFDIV == 2) {
            freq = currentFLLN * (FLLRefFreq / 4);
        }
        else if (currentFLLREFDIV == 3) {
            freq = currentFLLN * (FLLRefFreq / 8);
        }
        else if (currentFLLREFDIV == 4) {
            freq = currentFLLN * (FLLRefFreq / 12);
        }
        else if (currentFLLREFDIV == 5) {
            freq = currentFLLN * (FLLRefFreq / 16);
        }
    }
    else
    {
        freq = USB_XT_FREQ_VALUE * 1000;
    }
    return freq >> (UCSCTL5_L & DIVM_7);  // Divide by any divider present in DIVM
}


//
//! \endcond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
