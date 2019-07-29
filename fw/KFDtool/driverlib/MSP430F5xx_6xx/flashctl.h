/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// flashctl.h - Driver for the FLASHCTL Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_FLASHCTL_H__
#define __MSP430WARE_FLASHCTL_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_FLASH__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// The following are values that can be passed to the mask parameter for
// functions: FlashCtl_getStatus() as well as returned by the
// FlashCtl_getStatus() function.
//
//*****************************************************************************
#define FLASHCTL_READY_FOR_NEXT_WRITE                                      WAIT
#define FLASHCTL_ACCESS_VIOLATION_INTERRUPT_FLAG                        ACCVIFG
#define FLASHCTL_PASSWORD_WRITTEN_INCORRECTLY                              KEYV
#define FLASHCTL_BUSY                                                      BUSY

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Erase a single segment of the flash memory.
//!
//! For devices like MSP430i204x, if the specified segment is the information
//! flash segment, the FLASH_unlockInfo API must be called prior to calling
//! this API.
//!
//! \param flash_ptr is the pointer into the flash segment to be erased
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_eraseSegment(uint8_t *flash_ptr);

//*****************************************************************************
//
//! \brief Erase a single bank of the flash memory.
//!
//! This function erases a single bank of the flash memory.  This API will
//! erase the entire flash if device contains only one flash bank.
//!
//! \param flash_ptr is a pointer into the bank to be erased
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_eraseBank(uint8_t *flash_ptr);

//*****************************************************************************
//
//! \brief Erase all flash memory.
//!
//! This function erases all the flash memory banks. For devices like
//! MSP430i204x, this API erases main memory and information flash memory if
//! the FLASH_unlockInfo API was previously executed (otherwise the information
//! flash is not erased). Also note that erasing information flash memory in
//! the MSP430i204x impacts the TLV calibration constants located at the
//! information memory.
//!
//! \param flash_ptr is a pointer into the bank to be erased
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_performMassErase(uint8_t *flash_ptr);

//*****************************************************************************
//
//! \brief Erase check of the flash memory
//!
//! This function checks bytes in flash memory to make sure that they are in an
//! erased state (are set to 0xFF).
//!
//! \param flash_ptr is the pointer to the starting location of the erase check
//! \param numberOfBytes is the number of bytes to be checked
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
extern bool FlashCtl_performEraseCheck(uint8_t *flash_ptr,
                                       uint16_t numberOfBytes);

//*****************************************************************************
//
//! \brief Write data into the flash memory in byte format, pass by reference
//!
//! This function writes a byte array of size count into flash memory. Assumes
//! the flash memory is already erased and unlocked. FlashCtl_eraseSegment can
//! be used to erase a segment.
//!
//! \param data_ptr is the pointer to the data to be written
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_write8(uint8_t *data_ptr,
                            uint8_t *flash_ptr,
                            uint16_t count);

//*****************************************************************************
//
//! \brief Write data into the flash memory in 16-bit word format, pass by
//! reference
//!
//! This function writes a 16-bit word array of size count into flash memory.
//! Assumes the flash memory is already erased and unlocked.
//! FlashCtl_eraseSegment can be used to erase a segment.
//!
//! \param data_ptr is the pointer to the data to be written
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_write16(uint16_t *data_ptr,
                             uint16_t *flash_ptr,
                             uint16_t count);

//*****************************************************************************
//
//! \brief Write data into the flash memory in 32-bit word format, pass by
//! reference
//!
//! This function writes a 32-bit array of size count into flash memory.
//! Assumes the flash memory is already erased and unlocked.
//! FlashCtl_eraseSegment can be used to erase a segment.
//!
//! \param data_ptr is the pointer to the data to be written
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_write32(uint32_t *data_ptr,
                             uint32_t *flash_ptr,
                             uint16_t count);

//*****************************************************************************
//
//! \brief Write data into the flash memory in 32-bit word format, pass by
//! value
//!
//! This function writes a 32-bit data value into flash memory, count times.
//! Assumes the flash memory is already erased and unlocked.
//! FlashCtl_eraseSegment can be used to erase a segment.
//!
//! \param value value to fill memory with
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_fillMemory32(uint32_t value,
                                  uint32_t *flash_ptr,
                                  uint16_t count);

//*****************************************************************************
//
//! \brief Check FlashCtl status to see if it is currently busy erasing or
//! programming
//!
//! This function checks the status register to determine if the flash memory
//! is ready for writing.
//!
//! \param mask FLASHCTL status to read
//!        Mask value is the logical OR of any of the following:
//!        - \b FLASHCTL_READY_FOR_NEXT_WRITE
//!        - \b FLASHCTL_ACCESS_VIOLATION_INTERRUPT_FLAG
//!        - \b FLASHCTL_PASSWORD_WRITTEN_INCORRECTLY
//!        - \b FLASHCTL_BUSY
//!
//! \return Logical OR of any of the following:
//!         - \b FLASHCTL_READY_FOR_NEXT_WRITE
//!         - \b FLASHCTL_ACCESS_VIOLATION_INTERRUPT_FLAG
//!         - \b FLASHCTL_PASSWORD_WRITTEN_INCORRECTLY
//!         - \b FLASHCTL_BUSY
//!         \n indicating the status of the FlashCtl
//
//*****************************************************************************
extern uint8_t FlashCtl_getStatus(uint8_t mask);

//*****************************************************************************
//
//! \brief Locks the information flash memory segment A
//!
//! This function is typically called after an erase or write operation on the
//! information flash segment is performed by any of the other API functions in
//! order to re-lock the information flash segment.
//!
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_lockInfoA(void);

//*****************************************************************************
//
//! \brief Unlocks the information flash memory segment A
//!
//! This function must be called before an erase or write operation on the
//! information flash segment is performed by any of the other API functions.
//!
//!
//! \return None
//
//*****************************************************************************
extern void FlashCtl_unlockInfoA(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_FLASHCTL_H__
