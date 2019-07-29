/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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
 /*
 * BCUart.h
 *
 *  Created on: Aug 14, 2013
 *      Author: a0199742
 */

#ifndef BCUART_H_
#define BCUART_H_

#include "stdint.h"


/*****************************************************************************
 *** SET THESE CONSTANTS, TO CONFIGURE THE BACKCHANNEL UART LIBRARY **********

1) Set the SMCLK speed and desired baudrate.

   The baudrate is determined by the UCA1BR0, UCA1BR1, and UCA1MCTL registers.
   UCA1MCTL has three relevant fields:  UCA1BRF, UCA1BRS, and UCOS16.  The
   settings below are for:
   SMCLK = 8MHz
   baudrate = 28.8kbps

   If you change the SMCLK speed, or want a different baudrate, you need to
   change these values.  An easy way to determine them is the calculator at
   the MSP430 wiki page:

http://processors.wiki.ti.com/index.php/USCI_UART_Baud_Rate_Gen_Mode_Selection

   (If the link is somehow broken, the wiki page name is "USCI UART Baud Rate
   Gen Mode Selection", or try web-searching "msp430 usci calculator".
   Ultimately, the final reference is the UCS chapter of the F5xx Family User's
   Guide.) */

#define UCA1_OS   1    // 1 = oversampling mode, 0 = low-freq mode
#define UCA1_BR0  17   // Value of UCA1BR0 register
#define UCA1_BR1  0    // Value of UCA1BR1 register
#define UCA1_BRS  0    // Value of UCBRS field in UCA1MCTL register
#define UCA1_BRF  6    // Value of UCBRF field in UCA1MCTL register


// There is no hardware RTS/CTS handshaking in this example.  Your code must
// remain responsive to incoming UART data, to avoid overruns.


/*

2) If needed, re-configure the library's characteristics:

The size of the UART receive buffer.  Set smaller if RAM is in short supply.
Set larger if larger data chunks are to be received, or if the application
can't process incoming data very often   */
#define BC_RXBUF_SIZE  (128)

/* The threshold within bcUartRcvBuf at which main() will be awakened.  Must
be less than BC_RXBUF_SIZE.  A value of '1' will alert main() whenever
even a single byte is received.  If no wake is desired, set to
BC_RXBUF_SIZE+1     */
#define BC_RX_WAKE_THRESH  (1)

// ****************************************************************************


void bcUartInit(void);
void bcUartSend(uint8_t* buf, uint8_t len);
uint16_t bcUartReceiveBytesInBuffer(uint8_t* buf);

#endif /* BCUART_H_ */
