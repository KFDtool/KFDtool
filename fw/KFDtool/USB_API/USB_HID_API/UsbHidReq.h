/* 
 * ======== UsbHidReq.h ========
 */

#include <stdint.h>

#ifndef _UsbHidReq_H_
#define _UsbHidReq_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * Return Hid descriptor to host over control endpoint
 */
uint8_t usbGetHidDescriptor(void);
/**
 * Return HID report descriptor to host over control endpoint
 */
uint8_t usbGetReportDescriptor(void);
/**
 * Receive Set_Report from host over control endpoint
 */
uint8_t usbSetReport(void);
/**
 * Process Get_Report request from host over control endpoint
 */
uint8_t usbGetReport(void);
/**
 * Receive Set_Idle from host over control endpoint
 */
uint8_t usbSetIdle(void);
/**
 * Process Get_Idle request from host over control endpoint
 */
uint8_t usbGetIdle(void);
/**
 * Receive Set_Protocol from host over control endpoint
 */
uint8_t usbSetProtocol(void);
/**
 * Process Get_Protocol request from host over control endpoint
 */
uint8_t usbGetProtocol(void);


#ifdef __cplusplus
}
#endif
#endif  //_UsbHidReq_H_
