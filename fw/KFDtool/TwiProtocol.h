// KFDtool
// Copyright 2019 Daniel Dugger

#ifndef TWIPROTOCOL_H_
#define TWIPROTOCOL_H_

#include "stdint.h"

void twiInit(void);

uint8_t twiSelfTest(void);

uint16_t twiReceiveByte(uint8_t *c);

void twiSendKeySig(void);

void twiSendPhyByte(uint8_t byteToSend);

#endif /* TWIPROTOCOL_H_ */
