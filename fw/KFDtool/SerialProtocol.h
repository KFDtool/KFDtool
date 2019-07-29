// KFDtool
// Copyright 2019 Daniel Dugger

#ifndef SERIALPROTOCOL_H_
#define SERIALPROTOCOL_H_

#include "stdint.h"

void spConnect(void);

void spDisconnect(void);

uint16_t spRxData(uint8_t* outData);

uint16_t spFrameData(const uint8_t* inData,
                     uint16_t inLength,
                     uint8_t* outData);

void spTxDataBack(const uint8_t* inData,
                  uint16_t inLength);

void spTxDataWait(const uint8_t* inData,
                  uint16_t inLength);

#endif /* SERIALPROTOCOL_H_ */
