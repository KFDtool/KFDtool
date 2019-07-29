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
// aes.h - Driver for the AES Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_AES_H__
#define __MSP430WARE_AES_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_AES__

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
// The following are values that can be passed toThe following are values that
// can be returned by the AES_isBusy() function.
//
//*****************************************************************************
#define AES_BUSY                                                        AESBUSY
#define AES_NOT_BUSY                                                       0x00

//*****************************************************************************
//
// The following are values that can be passed toThe following are values that
// can be returned by the AES_getErrorFlagStatus() function.
//
//*****************************************************************************
#define AES_ERROR_OCCURRED                                             AESERRFG
#define AES_NO_ERROR                                                       0x00

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Loads a 128 bit cipher key to AES module.
//!
//! This function loads a 128 bit cipher key to AES module.
//!
//! \param baseAddress is the base address of the AES module.
//! \param CipherKey is a pointer to an uint8_t array with a length of 16 bytes
//!        that contains a 128 bit cipher key.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_setCipherKey(uint16_t baseAddress,
                                const uint8_t *CipherKey);

//*****************************************************************************
//
//! \brief Encrypts a block of data using the AES module.
//!
//! The cipher key that is used for encryption should be loaded in advance by
//! using function \b AES_setCipherKey()
//!
//! \param baseAddress is the base address of the AES module.
//! \param Data is a pointer to an uint8_t array with a length of 16 bytes that
//!        contains data to be encrypted.
//! \param encryptedData is a pointer to an uint8_t array with a length of 16
//!        bytes in that the encrypted data will be written.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_encryptData(uint16_t baseAddress,
                               const uint8_t *Data,
                               uint8_t *encryptedData);

//*****************************************************************************
//
//! \brief Decrypts a block of data using the AES module.
//!
//! This function requires a pre-generated decryption key. A key can be loaded
//! and pre-generated by using function \b AES_startSetDecipherKey() or \b
//! AES_setDecipherKey(). The decryption takes 167 MCLK.
//!
//! \param baseAddress is the base address of the AES module.
//! \param Data is a pointer to an uint8_t array with a length of 16 bytes that
//!        contains encrypted data to be decrypted.
//! \param decryptedData is a pointer to an uint8_t array with a length of 16
//!        bytes in that the decrypted data will be written.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_decryptData(uint16_t baseAddress,
                               const uint8_t *Data,
                               uint8_t *decryptedData);

//*****************************************************************************
//
//! \brief Sets the decipher key The API
//!
//! The API \b AES_startSetDecipherKey() or \b AES_setDecipherKey() must be
//! invoked before invoking \b AES_setDecipherKey().
//!
//! \param baseAddress is the base address of the AES module.
//! \param CipherKey is a pointer to an uint8_t array with a length of 16 bytes
//!        that contains the initial AES key.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_setDecipherKey(uint16_t baseAddress,
                                  const uint8_t *CipherKey);

//*****************************************************************************
//
//! \brief Clears the AES ready interrupt flag.
//!
//! This function clears the AES ready interrupt flag. This flag is
//! automatically cleared when AESADOUT is read, or when AESAKEY or AESADIN is
//! written. This function should be used when the flag needs to be reset and
//! it has not been automatically cleared by one of the previous actions.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! Modified bits are \b AESRDYIFG of \b AESACTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void AES_clearInterrupt(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Gets the AES ready interrupt flag status.
//!
//! This function checks the AES ready interrupt flag. This flag is
//! automatically cleared when AESADOUT is read, or when AESAKEY or AESADIN is
//! written. This function can be used to confirm that this has been done.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! \return uint32_t - AES_READY_INTERRUPT or 0x00.
//
//*****************************************************************************
extern uint32_t AES_getInterruptStatus(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Enables AES ready interrupt.
//!
//! Enables AES ready interrupt. This interrupt is reset by a PUC, but not
//! reset by AES_reset. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! Modified bits are \b AESRDYIE of \b AESACTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void AES_enableInterrupt(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Disables AES ready interrupt.
//!
//! Disables AES ready interrupt. This interrupt is reset by a PUC, but not
//! reset by AES_reset.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! Modified bits are \b AESRDYIE of \b AESACTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void AES_disableInterrupt(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Resets AES Module immediately.
//!
//! This function performs a software reset on the AES Module, note that this
//! does not affect the AES ready interrupt.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! Modified bits are \b AESSWRST of \b AESACTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void AES_reset(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Starts an encryption process on the AES module.
//!
//! This is the non-blocking equivalent of AES_encryptData(). The cipher key
//! that is used for decryption should be loaded in advance by using function
//! \b AES_setCipherKey(). It is recommended to use interrupt to check for
//! procedure completion then using AES_getDataOut() API to retrieve the
//! encrypted data.
//!
//! \param baseAddress is the base address of the AES module.
//! \param Data is a pointer to an uint8_t array with a length of 16 bytes that
//!        contains data to be encrypted.
//! \param encryptedData is a pointer to an uint8_t array with a length of 16
//!        bytes in that the encrypted data will be written.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_startEncryptData(uint16_t baseAddress,
                                    const uint8_t *Data,
                                    uint8_t *encryptedData);

//*****************************************************************************
//
//! \brief Decrypts a block of data using the AES module.
//!
//! This is the non-blocking equivalent of AES_decryptData(). This function
//! requires a pre-generated decryption key. A key can be loaded and pre-
//! generated by using function \b AES_setDecipherKey() or \b
//! AES_startSetDecipherKey(). The decryption takes 167 MCLK. It is recommended
//! to use interrupt to check for procedure completion then using
//! AES_getDataOut() API to retrieve the decrypted data.
//!
//! \param baseAddress is the base address of the AES module.
//! \param Data is a pointer to an uint8_t array with a length of 16 bytes that
//!        contains encrypted data to be decrypted.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_startDecryptData(uint16_t baseAddress,
                                    const uint8_t *Data);

//*****************************************************************************
//
//! \brief Loads the decipher key.
//!
//! This is the non-blocking equivalent of AES_setDecipherKey(). The API \b
//! AES_startSetDecipherKey() or \b AES_setDecipherKey() must be invoked before
//! invoking \b AES_startSetDecipherKey().
//!
//! \param baseAddress is the base address of the AES module.
//! \param CipherKey is a pointer to an uint8_t array with a length of 16 bytes
//!        that contains the initial AES key.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_startSetDecipherKey(uint16_t baseAddress,
                                       const uint8_t *CipherKey);

//*****************************************************************************
//
//! \brief Reads back the output data from AES module.
//!
//! This function is meant to use after an encryption or decryption process
//! that was started and finished by initiating an interrupt by use of the \b
//! AES_startEncryptData() or \b AES_startDecryptData() functions.
//!
//! \param baseAddress is the base address of the AES module.
//! \param OutputData is a pointer to an uint8_t array with a length of 16
//!        bytes in which the output data of the AES module is available. If
//!        AES module is busy returns NULL.
//!
//! \return STATUS_SUCCESS if AES is not busy, STATUS_FAIL if it is busy
//
//*****************************************************************************
extern uint8_t AES_getDataOut(uint16_t baseAddress,
                              uint8_t *OutputData);

//*****************************************************************************
//
//! \brief Gets the AES module busy status.
//!
//! Gets the AES module busy status. If a key or data are written while the AES
//! module is busy, an error flag will be thrown.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! \return One of the following:
//!         - \b AES_BUSY
//!         - \b AES_NOT_BUSY
//!         \n indicating if encryption/decryption/key generation is taking
//!         place
//
//*****************************************************************************
extern uint8_t AES_isBusy(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Clears the AES error flag.
//!
//! Clears the AES error flag that results from a key or data being written
//! while the AES module is busy. Modified bit is AESERRFG of AESACTL0
//! register.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! Modified bits are \b AESERRFG of \b AESACTL0 register.
//!
//! \return None
//
//*****************************************************************************
extern void AES_clearErrorFlag(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief Gets the AES error flag status.
//!
//! Checks the AES error flag that results from a key or data being written
//! while the AES module is busy. If the flag is set, it needs to be cleared
//! using AES_clearErrorFlag.
//!
//! \param baseAddress is the base address of the AES module.
//!
//! \return One of the following:
//!         - \b AES_ERROR_OCCURRED
//!         - \b AES_NO_ERROR
//!         \n indicating if AESAKEY or AESADIN were written while an AES
//!         operation was in progress
//
//*****************************************************************************
extern uint32_t AES_getErrorFlagStatus(uint16_t baseAddress);

//*****************************************************************************
//
//! \brief DEPRECATED Starts an decryption process on the AES module.
//!
//! This is the non-blocking equivalent of AES_decryptDataUsingEncryptionKey().
//! This function can be used to decrypt data by using the same key as used for
//! a previous performed encryption. The decryption takes 214 MCLK.
//!
//! \param baseAddress is the base address of the AES module.
//! \param Data is a pointer to an uint8_t array with a length of 16 bytes that
//!        contains encrypted data to be decrypted.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_startDecryptDataUsingEncryptionKey(uint16_t baseAddress,
                                                      const uint8_t *Data);

//*****************************************************************************
//
//! \brief DEPRECATED Decrypts a block of data using the AES module.
//!
//! This function can be used to decrypt data by using the same key as used for
//! a previous performed encryption. The decryption takes 214 MCLK.
//!
//! \param baseAddress is the base address of the AES module.
//! \param Data is a pointer to an uint8_t array with a length of 16 bytes that
//!        contains encrypted data to be decrypted.
//! \param decryptedData is a pointer to an uint8_t array with a length of 16
//!        bytes in that the decrypted data will be written.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
extern uint8_t AES_decryptDataUsingEncryptionKey(uint16_t baseAddress,
                                                 const uint8_t *Data,
                                                 uint8_t *decryptedData);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_AES_H__
