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
// pmap.h - Driver for the PMAP Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_PMAP_H__
#define __MSP430WARE_PMAP_H__

#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PORT_MAPPING__

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

#include "inc/hw_memmap.h"
//*****************************************************************************
//
//! \brief Used in the PMAP_initPorts() function as the param parameter.
//
//*****************************************************************************
typedef struct PMAP_initPortsParam {
    //! Is the pointer to init Data
    const uint8_t* portMapping;
    //! Is the pointer start of first PMAP to initialize
    uint8_t* PxMAPy;
    //! Is the number of Ports to initialize
    uint8_t numberOfPorts;
    //! Is used to enable/disable reconfiguration
    //! \n Valid values are:
    //! - \b PMAP_ENABLE_RECONFIGURATION
    //! - \b PMAP_DISABLE_RECONFIGURATION [Default]
    uint8_t portMapReconfigure;
} PMAP_initPortsParam;


//*****************************************************************************
//
// The following are values that can be passed to the param parameter for
// functions: PMAP_initPorts().
//
//*****************************************************************************
#define PMAP_ENABLE_RECONFIGURATION                                   PMAPRECFG
#define PMAP_DISABLE_RECONFIGURATION                                       0x00

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief This function configures the MSP430 Port Mapper
//!
//! This function port maps a set of pins to a new set.
//!
//!
//! Modified bits of \b PMAPKETID register and bits of \b PMAPCTL register.
//!
//! \return None
//
//*****************************************************************************
extern void PMAP_initPorts(uint16_t baseAddress,
                           PMAP_initPortsParam *param);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
#endif // __MSP430WARE_PMAP_H__
