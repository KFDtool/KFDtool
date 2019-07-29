// KFDtool
// Copyright 2019 Daniel Dugger

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_app/usbConstructs.h"

#include "SerialProtocol.h"

#define SOM_EOM 0x61
#define SOM_EOM_PLACEHOLDER 0x62
#define ESC 0x63
#define ESC_PLACEHOLDER 0x64

void spConnect(void)
{
    USB_setup(TRUE, TRUE);
}

void spDisconnect(void)
{
    __disable_interrupt();

    /*
    USBKEYPID = 0x9628; // Unlock USB configuration registers
    USBCNF &= ~PUR_EN; // Set PUR pin to hi-Z, logically disconnect from host
    USBPWRCTL &= ~VBOFFIE; // Disable VUSBoff interrupt
    USBKEYPID = 0x9600; // Lock USB configuration register
    */

    USB_disconnect(); // PUR high, disable VBUS interrupt
    USB_disable(); // Disable USB module, disable PLL
}

uint16_t spRxData(uint8_t* outData)
{
    // TODO implement ring buffer, currently expecting all data to come in one transfer

    uint16_t inDataCount;
    uint8_t inData[128];

    inDataCount = cdcReceiveDataInBuffer(inData, sizeof(inData), CDC0_INTFNUM);

    // don't process partial frames
    if (inDataCount < 3 || inData[0] != SOM_EOM || inData[inDataCount - 1] != SOM_EOM)
    {
        return 0;
    }

    uint16_t inIndex;
    uint16_t outIndex;
    outIndex = 0;

    for (inIndex = 1; inIndex < inDataCount - 1; inIndex++) // skip SOM and EOM
    {
        if (inData[inIndex] == ESC)
        {
            inIndex++;

            if (inData[inIndex] == SOM_EOM_PLACEHOLDER)
            {
                outData[outIndex] = SOM_EOM;
            }
            else if (inData[inIndex] == ESC_PLACEHOLDER)
            {
                outData[outIndex] = ESC;
            }
        }
        else
        {
            outData[outIndex] = inData[inIndex];
        }

        outIndex++;
    }

    return outIndex;
}

uint16_t spFrameData(const uint8_t* inData,
                     uint16_t inLength,
                     uint8_t* outData)
{
    uint16_t escCharsNeeded = 0;
    uint16_t i;

    for (i = 0; i < inLength; i++)
    {
        if ((inData[i] == SOM_EOM) || (inData[i] == ESC))
        {
            escCharsNeeded++;
        }
    }

    uint16_t totalCharsNeeded = 1 + inLength + escCharsNeeded + 1;

    *(outData + 0) = SOM_EOM;

    uint16_t j;
    uint16_t k = 1;

    for (j = 0; j < inLength; j++)
    {
        if (inData[j] == SOM_EOM)
        {
            *(outData + k) = ESC;
            k++;
            *(outData + k) = SOM_EOM_PLACEHOLDER;
            k++;
        }
        else if (inData[j] == ESC)
        {
            *(outData + k) = ESC;
            k++;
            *(outData + k) = ESC_PLACEHOLDER;
            k++;
        }
        else
        {
            *(outData + k) = inData[j];
            k++;
        }
    }

    *(outData + (totalCharsNeeded - 1)) = SOM_EOM;

    return totalCharsNeeded;
}

void spTxDataBack(const uint8_t* inData,
                            uint16_t inLength)
{
    uint16_t outLength;
    uint8_t outData[128];

    outLength = spFrameData(inData, inLength, outData);

    cdcSendDataInBackground(outData, outLength, CDC0_INTFNUM, 1000);
}

void spTxDataWait(const uint8_t* inData,
                            uint16_t inLength)
{
    uint16_t outLength;
    uint8_t outData[128];

    outLength = spFrameData(inData, inLength, outData);

    cdcSendDataWaitTilDone(outData, outLength, CDC0_INTFNUM, 1000);
}
