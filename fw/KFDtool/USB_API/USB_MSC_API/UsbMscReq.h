/* 
 * ======== UsbMscReq.h ========
 */
#ifndef _USB_MSC_REQ_H_
#define _USB_MSC_REQ_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* MSC Class defined Request.Reset State-Machine and makes endpoints ready again*/
uint8_t USBMSC_reset(void);

/* MSC Class defined Request.Tells the host the number of supported logical units*/
uint8_t Get_MaxLUN(void);

#ifdef __cplusplus
}
#endif
#endif  //_USB_MSC_REQ_H_

