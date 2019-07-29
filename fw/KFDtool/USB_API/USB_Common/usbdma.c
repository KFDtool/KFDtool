
//
//! \cond
//

/* 
 * ======== dma.c ========
 */
#include <string.h>

#include "driverlib.h"

#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include <descriptors.h>
#include <string.h>

#ifdef __REGISTER_MODEL__
/* for IAR */
#   if __REGISTER_MODEL__ == __REGISTER_MODEL_REG20__
#       define __DMA_ACCESS_REG__ (void __data20 *)
#   else
#       define __DMA_ACCESS_REG__ (uint16_t)
#   endif
#else
/* for CCS */
#   define __DMA_ACCESS_REG__ (__SFR_FARPTR)(uint32_t)
#endif

//function pointers
void *(*USB_TX_memcpy)(void * dest, const void * source, size_t count);
void *(*USB_RX_memcpy)(void * dest, const void * source, size_t count);

void * memcpyDMA0 (void * dest, const void * source, size_t count);
void * memcpyDMA1 (void * dest, const void * source, size_t count);
void * memcpyDMA2 (void * dest, const void * source, size_t count);

//NOTE: this functin works only with data in the area <64k (small memory model)
void * memcpyV (void * dest, const void * source, size_t count)
{
    uint16_t i;
    volatile uint8_t bTmp;

    for (i = 0; i < count; i++)
    {
        bTmp = *((uint8_t*)source + i);
        *((uint8_t*)dest  + i) = bTmp;
    }
    return (dest);
}

void * memcpyDMA (void * dest, const void *  source, size_t count)
{
    if (count == 0){                                        //do nothing if zero bytes to transfer
        return (dest);
    }

    //DMA4 workaround - disable DMA transfers during read-modify-write CPU 
    //operations
    DMA_disableTransferDuringReadModifyWrite();
    DMA_setSrcAddress(USB_DMA_CHAN, (uint32_t)source, DMA_DIRECTION_INCREMENT);
    DMA_setDstAddress(USB_DMA_CHAN, (uint32_t)dest, DMA_DIRECTION_INCREMENT);
    //DMA4 workaround - re-enable DMA transfers during read-modify-write CPU 
    //operations
    DMA_enableTransferDuringReadModifyWrite();
    DMA_setTransferSize(USB_DMA_CHAN, count);
    DMA_enableTransfers(USB_DMA_CHAN);
    DMA_startTransfer(USB_DMA_CHAN);

    while (DMA_getInterruptStatus(USB_DMA_CHAN) == DMA_INT_INACTIVE);

    DMA_disableTransfers(USB_DMA_CHAN);
    return (dest);
}

//this function inits the DMA
void USB_initMemcpy (void)
{
    //set DMA parameters
    DMA_initParam dmaParams = {0};
	dmaParams.channelSelect = USB_DMA_CHAN;
	dmaParams.transferModeSelect = DMA_TRANSFER_BLOCK;
	dmaParams.transferSize = 0;
	dmaParams.triggerSourceSelect = DMA_TRIGGERSOURCE_0;
	dmaParams.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE;
	dmaParams.triggerTypeSelect = DMA_TRIGGER_HIGH;

    USB_TX_memcpy = memcpyV;
    USB_RX_memcpy = memcpyV;

    if (USB_DMA_CHAN != 0xFF) {
    	DMA_init(&dmaParams);
        USB_TX_memcpy = memcpyDMA;
        USB_RX_memcpy = memcpyDMA;
    }
}

//
//! \endcond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
