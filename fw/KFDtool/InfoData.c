// KFDtool
// Copyright 2019 Daniel Dugger

#include "driverlib.h"
#include "InfoData.h"

#define INFOB_START (0x1900)
#define INFOB_LENGTH (128)

#define INFOC_START (0x1880)
#define INFOC_LENGTH (128)

uint16_t idWriteModelIdHwRev(uint8_t hwId, uint8_t hwRevMaj, uint8_t hwRevMin)
{
    uint8_t data[5];

    data[0] = 0x10;
    data[1] = hwId;
    data[2] = hwRevMaj;
    data[3] = hwRevMin;
    data[4] = 0x11;

    uint16_t status;

    do
    {
        FlashCtl_eraseSegment((uint8_t *)INFOB_START);
        status = FlashCtl_performEraseCheck((uint8_t *)INFOB_START, INFOB_LENGTH);
    } while (status == STATUS_FAIL);

    FlashCtl_write8(data, (uint8_t *)INFOB_START, sizeof(data));

    return 1;
}

uint16_t idWriteSerNum(uint8_t ser0, uint8_t ser1, uint8_t ser2, uint8_t ser3, uint8_t ser4, uint8_t ser5)
{
    uint8_t data[8];

    data[0] = 0x20;
    data[1] = ser0;
    data[2] = ser1;
    data[3] = ser2;
    data[4] = ser3;
    data[5] = ser4;
    data[6] = ser5;
    data[7] = 0x22;

    uint16_t status;

    do
    {
        FlashCtl_eraseSegment((uint8_t *)INFOC_START);
        status = FlashCtl_performEraseCheck((uint8_t *)INFOC_START, INFOC_LENGTH);
    } while (status == STATUS_FAIL);

    FlashCtl_write8(data, (uint8_t *)INFOC_START, sizeof(data));

    return 1;
}

uint16_t idReadModelId(uint8_t *hwId)
{
    uint16_t *flashPtr;

    flashPtr = (uint16_t *)INFOB_START;

    uint8_t header;
    uint8_t footer;

    header = *flashPtr;
    *hwId = (*flashPtr >> 8);
    *flashPtr++;
    *flashPtr++;
    footer = *flashPtr;

    if (header == 0x10 && footer == 0x11)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t idReadHwRev(uint8_t *hwRevMaj, uint8_t *hwRevMin)
{
    uint16_t *flashPtr;

    flashPtr = (uint16_t *)INFOB_START;

    uint8_t header;
    uint8_t footer;

    header = *flashPtr;
    *flashPtr++;
    *hwRevMaj = *flashPtr;
    *hwRevMin = (*flashPtr >> 8);
    *flashPtr++;
    footer = *flashPtr;

    if (header == 0x10 && footer == 0x11)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t idReadSerNum(uint8_t *ser0, uint8_t *ser1, uint8_t *ser2, uint8_t *ser3, uint8_t *ser4, uint8_t *ser5)
{
    uint16_t *flashPtr;

    flashPtr = (uint16_t *)INFOC_START;

    uint8_t header;
    uint8_t footer;

    header = *flashPtr;
    *ser0 = (*flashPtr >> 8);
    *flashPtr++;
    *ser1 = *flashPtr;
    *ser2 = (*flashPtr >> 8);
    *flashPtr++;
    *ser3 = *flashPtr;
    *ser4 = (*flashPtr >> 8);
    *flashPtr++;
    footer = (*flashPtr >> 8);
    *ser5 = *flashPtr;

    if (header == 0x20 && footer == 0x22)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
