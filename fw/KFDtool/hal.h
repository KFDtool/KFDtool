// KFDtool
// Copyright 2019 Daniel Dugger

#ifndef HAL_H_
#define HAL_H_

#define FCPU 24000000 // also change in USB_config/descriptors.h

void halInit(void);

void halDelayUs(uint16_t us);

void halDelayMs(uint16_t ms);

void halEnterBsl(void);

void halReset(void);

void halLed1On(void);

void halLed1Off(void);

void halLed1Toggle(void);

void halGpio1High(void);

void halGpio1Low(void);

void halGpio1Toggle(void);

void halGpio2High(void);

void halGpio2Low(void);

void halGpio2Toggle(void);

void halKfdTxBusy(void);

void halKfdTxIdle(void);

void halSenTxConn(void);

void halSenTxDisc(void);

#endif /* HAL_H_ */
