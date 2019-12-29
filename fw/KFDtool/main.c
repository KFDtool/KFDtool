// KFDtool
// Copyright 2019 Daniel Dugger

// MSP430 Driver Library Version 2.91.11.01 2019-02-20
// MSP430 USB Stack Version ???

#include "driverlib.h"

#include "hal.h"
#include "TwiProtocol.h"
#include "SerialProtocol.h"
#include "InfoData.h"

/* FIRMWARE VERSION */
#define VER_FW_MAJOR 0x01
#define VER_FW_MINOR 0x03
#define VER_FW_PATCH 0x00

/* ADAPTER PROTOCOL VERSION */
#define VER_AP_MAJOR 0x01
#define VER_AP_MINOR 0x00
#define VER_AP_PATCH 0x00

/* COMMAND OPCODES */
#define CMD_READ 0x11
#define CMD_WRITE_INFO 0x12
#define CMD_ENTER_BSL_MODE 0x13
#define CMD_RESET 0x14
#define CMD_SELF_TEST 0x15
#define CMD_SEND_KEY_SIG 0x16
#define CMD_SEND_BYTE 0x17

/* RESPONSE OPCODES */
#define RSP_ERROR 0x20
#define RSP_READ 0x21
#define RSP_WRITE_INFO 0x22
#define RSP_ENTER_BSL_MODE 0x23
#define RSP_RESET 0x24
#define RSP_SELF_TEST 0x25
#define RSP_SEND_KEY_SIG 0x26
#define RSP_SEND_BYTE 0x27

/* BROADCAST OPCODES */
#define BCST_RECEIVE_BYTE 0x31

/* READ OPCODES */
#define READ_AP_VER 0x01
#define READ_FW_VER 0x02
#define READ_UNIQUE_ID 0x03
#define READ_MODEL_ID 0x04
#define READ_HW_REV 0x05
#define READ_SER_NUM 0x06

/* WRITE OPCODES */
#define WRITE_MDL_REV 0x01
#define WRITE_SER 0x02

/* ERROR OPCODES */
#define ERR_OTHER 0x00
#define ERR_INVALID_CMD_LENGTH 0x01
#define ERR_INVALID_CMD_OPCODE 0x02
#define ERR_INVALID_READ_OPCODE 0x03
#define ERR_READ_FAILED 0x04
#define ERR_INVALID_WRITE_OPCODE 0x05
#define ERR_WRITE_FAILED 0x06

uint16_t cmdCount;
uint8_t cmdData[128];
uint16_t rxReady;
uint8_t rxTemp;

void main(void)
{
    WDT_A_hold(WDT_A_BASE);

    halInit();
    spConnect();

    __enable_interrupt();

    twiInit();

    halLed1On();

    while (1)
    {
        cmdCount = spRxData(cmdData);

        if (cmdCount > 0)
        {
            if (cmdData[0] == CMD_READ) // read
            {
                if (cmdCount == 2)
                {
                    if (cmdData[1] == READ_AP_VER) // read adapter protocol version
                    {
                        uint8_t rspData[5];

                        rspData[0] = RSP_READ;
                        rspData[1] = READ_AP_VER;
                        rspData[2] = VER_AP_MAJOR;
                        rspData[3] = VER_AP_MINOR;
                        rspData[4] = VER_AP_PATCH;

                        spTxDataWait(rspData, sizeof(rspData));
                    }
                    else if (cmdData[1] == READ_FW_VER) // read firmware version
                    {
                        uint8_t rspData[5];

                        rspData[0] = RSP_READ;
                        rspData[1] = READ_FW_VER;
                        rspData[2] = VER_FW_MAJOR;
                        rspData[3] = VER_FW_MINOR;
                        rspData[4] = VER_FW_PATCH;

                        spTxDataWait(rspData, sizeof(rspData));
                    }
                    else if (cmdData[1] == READ_UNIQUE_ID) // read unique id
                    {
                        uint8_t* serNum;
                        uint8_t serLen;

                        serNum = 0;

                        TLV_getInfo(TLV_TAG_DIERECORD, 0, (uint8_t *)&serLen, (uint16_t **)&serNum);

                        if (serLen == 10)
                        {
                            uint8_t rspData[12];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_UNIQUE_ID;
                            rspData[2] = 0x09; // id length
                            rspData[3] = 0x10; // id source (msp430 die record)
                            rspData[4] = *serNum; // id[0]
                            serNum++;
                            rspData[5] = *serNum; // id[1]
                            serNum++;
                            rspData[6] = *serNum; // id[2]
                            serNum++;
                            rspData[7] = *serNum; // id[3]
                            serNum++;
                            rspData[8] = *serNum; // id[4]
                            serNum++;
                            rspData[9] = *serNum; // id[5]
                            serNum++;
                            rspData[10] = *serNum; // id[6]
                            serNum++;
                            rspData[11] = *serNum; // id[7]

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                        else // no unique id available
                        {
                            uint8_t rspData[3];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_UNIQUE_ID;
                            rspData[2] = 0x00; // id length

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                    }
                    else if (cmdData[1] == READ_MODEL_ID) // read model id
                    {
                        uint16_t status;
                        uint8_t hwId;

                        status = idReadModelId(&hwId);

                        if (status) // check if available
                        {
                            uint8_t rspData[3];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_MODEL_ID;
                            rspData[2] = hwId;

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                        else // no model id available
                        {
                            uint8_t rspData[3];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_MODEL_ID;
                            rspData[2] = 0x00;

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                    }
                    else if (cmdData[1] == READ_HW_REV) // read hardware revision
                    {
                        uint16_t status;
                        uint8_t hwRevMaj;
                        uint8_t hwRevMin;

                        status = idReadHwRev(&hwRevMaj, &hwRevMin);

                        if (status == 1) // check if available
                        {
                            uint8_t rspData[4];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_HW_REV;
                            rspData[2] = hwRevMaj;
                            rspData[3] = hwRevMin;

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                        else // no hardware revision available
                        {
                            uint8_t rspData[4];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_HW_REV;
                            rspData[2] = 0x00;
                            rspData[3] = 0x00;

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                    }
                    else if (cmdData[1] == READ_SER_NUM) // read serial number
                    {
                        uint16_t status;
                        uint8_t ser0;
                        uint8_t ser1;
                        uint8_t ser2;
                        uint8_t ser3;
                        uint8_t ser4;
                        uint8_t ser5;

                        status = idReadSerNum(&ser0, &ser1, &ser2, &ser3, &ser4, &ser5);

                        if (status == 1) // check if available
                        {
                            uint8_t rspData[9];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_SER_NUM;
                            rspData[2] = 0x06; // serial length
                            rspData[3] = ser0;
                            rspData[4] = ser1;
                            rspData[5] = ser2;
                            rspData[6] = ser3;
                            rspData[7] = ser4;
                            rspData[8] = ser5;

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                        else // no serial number available
                        {
                            uint8_t rspData[3];

                            rspData[0] = RSP_READ;
                            rspData[1] = READ_SER_NUM;
                            rspData[2] = 0x00; // serial length

                            spTxDataWait(rspData, sizeof(rspData));
                        }

                    }
                    else // invalid read opcode
                    {
                        uint8_t rspData[2];

                        rspData[0] = RSP_ERROR;
                        rspData[1] = ERR_INVALID_READ_OPCODE;

                        spTxDataWait(rspData, sizeof(rspData));
                    }
                }
                else // invalid command length
                {
                    uint8_t rspData[2];

                    rspData[0] = RSP_ERROR;
                    rspData[1] = ERR_INVALID_CMD_LENGTH;

                    spTxDataWait(rspData, sizeof(rspData));
                }
            }
            else if (cmdData[0] == CMD_WRITE_INFO) // write info
            {
                if (cmdCount > 1)
                {
                    if (cmdData[1] == WRITE_MDL_REV) // write model id and hardware revision
                    {
                        if (cmdCount == 5)
                        {
                            uint16_t result;

                            result = idWriteModelIdHwRev(cmdData[2], cmdData[3], cmdData[4]);

                            if (result == 1)
                            {
                                uint8_t rspData[1];

                                rspData[0] = RSP_WRITE_INFO;

                                spTxDataWait(rspData, sizeof(rspData));
                            }
                            else // write failed
                            {
                                uint8_t rspData[2];

                                rspData[0] = RSP_ERROR;
                                rspData[1] = ERR_WRITE_FAILED;

                                spTxDataWait(rspData, sizeof(rspData));
                            }
                        }
                        else // invalid command length
                        {
                            uint8_t rspData[2];

                            rspData[0] = RSP_ERROR;
                            rspData[1] = ERR_INVALID_CMD_LENGTH;

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                    }
                    else if (cmdData[1] == WRITE_SER) // write serial number
                    {
                        if (cmdCount == 8)
                        {
                            uint16_t result;

                            result = idWriteSerNum(cmdData[2], cmdData[3], cmdData[4], cmdData[5], cmdData[6], cmdData[7]);

                            if (result == 1)
                            {
                                uint8_t rspData[1];

                                rspData[0] = RSP_WRITE_INFO;

                                spTxDataWait(rspData, sizeof(rspData));
                            }
                            else // write failed
                            {
                                uint8_t rspData[2];

                                rspData[0] = RSP_ERROR;
                                rspData[1] = ERR_WRITE_FAILED;

                                spTxDataWait(rspData, sizeof(rspData));
                            }
                        }
                        else // invalid command length
                        {
                            uint8_t rspData[2];

                            rspData[0] = RSP_ERROR;
                            rspData[1] = ERR_INVALID_CMD_LENGTH;

                            spTxDataWait(rspData, sizeof(rspData));
                        }
                    }
                    else // invalid write opcode
                    {
                        uint8_t rspData[2];

                        rspData[0] = RSP_ERROR;
                        rspData[1] = ERR_INVALID_WRITE_OPCODE;

                        spTxDataWait(rspData, sizeof(rspData));
                    }
                }
                else // invalid command length
                {
                    uint8_t rspData[2];

                    rspData[0] = RSP_ERROR;
                    rspData[1] = ERR_INVALID_CMD_LENGTH;

                    spTxDataWait(rspData, sizeof(rspData));
                }
            }
            else if (cmdData[0] == CMD_ENTER_BSL_MODE) // enter bsl mode
            {
                if (cmdCount == 1)
                {
                    uint8_t rspData[1];

                    rspData[0] = RSP_ENTER_BSL_MODE;

                    spTxDataWait(rspData, sizeof(rspData));

                    halDelayMs(1000); // wait 1 second

                    spDisconnect(); // disconnect usb

                    halDelayMs(3000); // wait 3 seconds

                    halLed1Off();

                    halEnterBsl();
                }
                else // invalid command length
                {
                    uint8_t rspData[2];

                    rspData[0] = RSP_ERROR;
                    rspData[1] = ERR_INVALID_CMD_LENGTH;

                    spTxDataWait(rspData, sizeof(rspData));
                }
            }
            else if (cmdData[0] == CMD_RESET) // reset
            {
                if (cmdCount == 1)
                {
                    uint8_t rspData[1];

                    rspData[0] = RSP_RESET;

                    spTxDataWait(rspData, sizeof(rspData));

                    halDelayMs(1000); // wait 1 second

                    spDisconnect(); // disconnect usb

                    halDelayMs(3000); // wait 3 seconds

                    halLed1Off();

                    halReset();
                }
                else // invalid command length
                {
                    uint8_t rspData[2];

                    rspData[0] = RSP_ERROR;
                    rspData[1] = ERR_INVALID_CMD_LENGTH;

                    spTxDataWait(rspData, sizeof(rspData));
                }
            }
            else if (cmdData[0] == CMD_SELF_TEST) // self test
            {
                if (cmdCount == 1)
                {
                    uint8_t rspData[2];

                    uint8_t result;
                    result = twiSelfTest();

                    rspData[0] = RSP_SELF_TEST;
                    rspData[1] = result;

                    spTxDataWait(rspData, sizeof(rspData));
                }
                else // invalid command length
                {
                    uint8_t rspData[2];

                    rspData[0] = RSP_ERROR;
                    rspData[1] = ERR_INVALID_CMD_LENGTH;

                    spTxDataWait(rspData, sizeof(rspData));
                }
            }
            else if (cmdData[0] == CMD_SEND_KEY_SIG) // send key signature
            {
                if (cmdCount == 2)
                {
                    twiSendKeySig();

                    uint8_t rspData[1];

                    rspData[0] = RSP_SEND_KEY_SIG;

                    spTxDataWait(rspData, sizeof(rspData));
                }
                else // invalid command length
                {
                    uint8_t rspData[2];

                    rspData[0] = RSP_ERROR;
                    rspData[1] = ERR_INVALID_CMD_LENGTH;

                    spTxDataWait(rspData, sizeof(rspData));
                }
            }
            else if (cmdData[0] == CMD_SEND_BYTE) // send byte
            {
                if (cmdCount == 3)
                {
                    twiSendPhyByte(cmdData[2]);

                    uint8_t rspData[1];

                    rspData[0] = RSP_SEND_BYTE;

                    spTxDataWait(rspData, sizeof(rspData));
                }
                else // invalid command length
                {
                    uint8_t rspData[2];

                    rspData[0] = RSP_ERROR;
                    rspData[1] = ERR_INVALID_CMD_LENGTH;

                    spTxDataWait(rspData, sizeof(rspData));
                }
            }
            else // invalid command opcode
            {
                uint8_t rspData[2];

                rspData[0] = RSP_ERROR;
                rspData[1] = ERR_INVALID_CMD_OPCODE;

                spTxDataWait(rspData, sizeof(rspData));
            }
        }

        rxReady = twiReceiveByte(&rxTemp);

        if (rxReady == 1)
        {
            uint8_t bcstData[3];

            bcstData[0] = BCST_RECEIVE_BYTE;
            bcstData[1] = 0x00; // reserved (set to 0x00)
            bcstData[2] = rxTemp;

            spTxDataBack(bcstData, sizeof(bcstData));
        }
    }
}
