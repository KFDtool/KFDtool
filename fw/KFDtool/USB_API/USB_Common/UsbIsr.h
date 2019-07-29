/* 
 * ======== UsbIsr.h ========
 */
#include <stdint.h>

#ifndef _ISR_H_
#define _ISR_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Handle incoming setup packet.
 * returns TRUE to keep CPU awake
 */
uint8_t SetupPacketInterruptHandler(void);

/**
 * Handle VBuss on signal.
 */
void PWRVBUSonHandler(void);

/**
 * Handle VBuss off signal.
 */
void PWRVBUSoffHandler(void);

/**
 * Handle In-requests from control pipe.
 */
void IEP0InterruptHandler(void);

/**
 * Handle Out-requests from control pipe.
 */
uint8_t OEP0InterruptHandler(void);

/*----------------------------------------------------------------------------+
 | End of header file                                                          |
 +----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* _ISR_H_ */

/*------------------------ Nothing Below This Line --------------------------*/
