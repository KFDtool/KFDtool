/* 
 * ======== UsbMscScsi.h ========
 */
#include <stdint.h>

#ifndef _UMSC_SCSI_H_
#define _UMSC_SCSI_H_

#include <descriptors.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*----------------------------------------------------------------------------
 * The following function names and macro names are deprecated.  These were 
 * updated to new names to follow OneMCU naming convention.
 +---------------------------------------------------------------------------*/
#ifndef DEPRECATED
#define  kUSBMSC_RWSuccess                USBMSC_RW_SUCCESS
#define  kUSBMSC_RWNotReady               USBMSC_RW_NOT_READY
#define  kUSBMSC_RWIllegalReq             USBMSC_RW_ILLEGAL_REQUEST 
#define  kUSBMSC_RWUnitAttn               USBMSC_RW_UNIT_ATTENTION
#define  kUSBMSC_RWLbaOutOfRange          USBMSC_RW_LBA_OUT_OF_RANGE
#define  kUSBMSC_RWMedNotPresent          USBMSC_RW_MEDIA_NOT_PRESENT
#define  kUSBMSC_RWDevWriteFault          USBMSC_RW_DEVICE_WRITE_FAULT
#define  kUSBMSC_RWUnrecoveredRead        USBMSC_RW_UNRECOVERED_READ
#define  kUSBMSC_RWWriteProtected         USBMSC_RW_WRITE_PROTECTED
#define  kUSBMSC_READ                     USBMSC_READ
#define  kUSBMSC_WRITE                    USBMSC_WRITE
#define  kUSBMSC_MEDIA_PRESENT            USBMSC_MEDIA_PRESENT
#define  kUSBMSC_MEDIA_NOT_PRESENT        USBMSC_MEDIA_NOT_PRESENT
#define  kUSBMSC_WRITE_PROTECTED          USBMSC_WRITE_PROTECTED

#define USBMSC_fetchInfoStruct            USBMSC_fetchInformationStructure
#endif


/*Macros for CBW, CSW signatures */
#define CBW_SIGNATURE 0x43425355u
#define CSW_SIGNATURE 0x53425355u

/*CBW, CSW length in bytes */
#define CBW_LENGTH   31
#define CSW_LENGTH   13

/*SCSI Commands - Mandatory only implemented */
#define SCSI_TEST_UNIT_READY            0x00
#define SCSI_REQUEST_SENSE          	0x03
#define SCSI_INQUIRY                	0x12
#define SCSI_MODE_SENSE_6           	0x1A
#define SCSI_MODE_SENSE_10          	0x5A
#define SCSI_READ_CAPACITY_10           0x25
#define SCSI_READ_10                	0x28
#define SCSI_WRITE_10               	0x2A
#define SCSI_READ_FORMAT_CAPACITIES     0x23
#define SCSI_MODE_SELECT_6              0x15
#define SCSI_MODE_SELECT_10             0x55
#define PREVENT_ALLW_MDM                0x1E
#define START_STOP_UNIT                 0x1B
#define SCSI_REPORT_LUNS                0xA0
#define SCSI_VERIFY                     0x2F

#define SCSI_READ_TOC_PMA_ATIP					0x43
#define Scsi_Read_TOC_PMA_ATIP_F1_LEN			20
#define Scsi_Read_TOC_PMA_ATIP_F2_LEN    		48
#define SCSI_GET_CONFIGURATION          		0x46
#define SCSI_GET_CONFIGURATION_LEN      		4
#define SCSI_EVENT_STATUS               		0x4A
#define SCSI_EVENT_STATUS_LEN           		8
#define SCSI_READ_DISC_INFORMATION     			0x51
#define SCSI_SET_CD_SPEED						0xBB
#define SCSI_READ_DISC_INFORMATION_LEN  		36

/*SCSI Status codes. Used in CSW response */
#define SCSI_PASSED           		0
#define SCSI_FAILED           		1
#define SCSI_PHASE_ERROR      		2
#define SCSI_READWRITE_FAIL       	2

#define USBMSC_RW_SUCCESS           0
#define USBMSC_RW_NOT_READY          1
#define USBMSC_RW_ILLEGAL_REQUEST        2
#define USBMSC_RW_UNIT_ATTENTION          3
#define USBMSC_RW_LBA_OUT_OF_RANGE     4
#define USBMSC_RW_MEDIA_NOT_PRESENT     5
#define USBMSC_RW_DEVICE_WRITE_FAULT     6
#define USBMSC_RW_UNRECOVERED_READ   7
#define USBMSC_RW_WRITE_PROTECTED    8


/* Macros to indicate READ or WRITE operation */
#define USBMSC_READ 1
#define USBMSC_WRITE 2

#define USBMSC_MEDIA_PRESENT 0x81
#define USBMSC_MEDIA_NOT_PRESENT 0x82

#define USBMSC_WRITE_PROTECTED 0x00

/* Defines for MSC SCSI State-Machine */
#define MSC_READY                   0x00
#define MSC_COMMAND_TRANSPORT       0x01
#define MSC_DATA_IN                 0x02
#define MSC_DATA_OUT                0x03
#define MSC_STATUS_TRANSPORT        0x04
#define MSC_DATA                    0x05
#define MSC_WAIT4RESET              0x06

/*Lengths of SCSI commands(in bytes) */
#define SCSI_SCSI_INQUIRY_CMD_LEN            36
#define SCSI_READ_CAPACITY_CMD_LEN           8
#define SCSI_MODE_SENSE_6_CMD_LEN            4
#define SCSI_MODE_SENSE_10_CMD_LEN           8
#define SCSI_REQ_SENSE_CMD_LEN               18
#define SCSI_READ_FORMAT_CAPACITY_CMD_LEN    12
#define SCSI_REPORT_LUNS_CMD_LEN             16

/*----------------------------------------------------------------------------+
 | Type defines and structures                                                 |
 +----------------------------------------------------------------------------*/
/*CBW Structure */
typedef struct {
    uint32_t dCBWSignature;
    uint32_t dCBWTag;
    uint32_t dCBWDataTransferLength;
    uint8_t bmCBWFlags;
    uint8_t bCBWLUN;
    uint8_t bCBWCBLength;
    uint8_t CBWCB[16];
} CBW, *pCBW;

/*CSW structure */
typedef struct {
    uint32_t dCSWSignature;
    uint32_t dCSWTag;
    uint32_t dCSWDataResidue;
    uint8_t bCSWStatus;
} CSW, *pCSW;

/*Request Response union(Required for Request sense command) */
typedef struct {
    uint8_t ResponseCode : 7;
    uint8_t VALID : 1;
    uint8_t Obsolete;
    uint8_t SenseKey : 4;
    uint8_t Resv : 1;
    uint8_t ILI : 1;
    uint8_t EOM : 1;
    uint8_t FILEMARK : 1;
    uint8_t Information[4];
    uint8_t AddSenseLen;
    uint8_t CmdSpecificInfo[4];
    uint8_t ASC;
    uint8_t ASCQ;
    uint8_t FRUC;
    uint8_t SenseKeySpecific[3];
    uint8_t padding[14];   /* padding to cover case where host requests 24 bytes of sense data */
} REQUEST_SENSE_RESPONSE;

/*Read capacity union(Required for READ CAPACITY command)*/
typedef struct {
    uint32_t Last_LBA;
    uint8_t Resv;
    uint8_t Size_LBA[3];
} SCSI_READ_CAPACITY;

/*Structure internal to stack for holding LBA,buffer addr etc information*/
typedef struct {
    //uint8_t	intfNum;
    uint8_t 	lun;
    uint8_t 	operation;
    uint32_t 	lba;
    uint8_t 	lbCount;
    uint8_t    *bufferAddr;
    uint8_t 	returnCode;
    uint8_t 	XorY;
    uint8_t	xBufFull;
    uint16_t	xWordCnt;
    uint8_t	yBufFull;
    uint16_t	yWordCnt;
    uint8_t	bufferProcessed;
    uint8_t	firstFlag;
    uint32_t	xlba;
    uint8_t	xlbaCount;
    uint32_t	ylba;
    uint8_t	ylbaCount;

}USBMSC_RWbuf_Info;

/*Media info structure */
struct USBMSC_mediaInfoStr {
    uint32_t lastBlockLba;
    uint32_t bytesPerBlock;
    uint8_t mediaPresent;
    uint8_t mediaChanged;
    uint8_t writeProtected;
};

/*Lun entry Structures */
struct _LUN_entry_struct {
    uint8_t number;
    uint8_t PDT;
    uint8_t removable;
    char t10VID[8];
    char t10PID[16];
    char t10rev[4];
};

struct config_struct {
    struct _LUN_entry_struct LUN[MSC_MAX_LUN_NUMBER];
};

struct _Report_Luns {
    uint8_t LunListLength[4];
    uint8_t Reserved[4];
    uint8_t LunList1[8];
};

struct _Scsi_Read_Capacity {
    uint8_t lLba[4];               //Last logical block address
    uint8_t bLength[4];            //Block length, in this case 0x200 = 512 bytes for each Logical Block
};

//structure for controlling WRITE phase (HOST to MSP430)
struct _MscWriteControl {
    uint32_t dwBytesToReceiveLeft; //holds how many bytes is still requested by WRITE operation:
    //Host to MSP430.
    uint16_t wFreeBytesLeft;        //free bytes left in UserBuffer
    uint32_t lba;                  //holds the current LBA number. This is the first LBA in the UserBuffer
    uint8_t *pUserBuffer;          //holds the current position of user's receiving buffer.
                                //If NULL- no receiving operation started
    uint16_t wCurrentByte;          //how many bytes in current LBA are received
    uint16_t lbaCount;              //how many LBA we have received in current User Buffer
    uint8_t * pCT1;                //holds current EPBCTxx register
    uint8_t * pCT2;                //holds next EPBCTxx register
    uint8_t * pEP2;                //holds addr of the next EP buffer
    uint8_t bCurrentBufferXY;      //indicates which buffer is used by host to transmit data via OUT
    uint8_t bWriteProcessing;      //indicated if the current state is DATA WRITE phase or CBW receiwing
    uint8_t XorY;
};

//structure for controlling READ phase (MSP430 to HOST)
struct _MscReadControl {
    uint32_t dwBytesToSendLeft;    //holds how many bytes is still requested by WRITE operation (Host to MSP430)
    uint8_t *pUserBuffer;          //holds the current position of user's receiving buffer.
                                //If NULL- no receiving operation started
    uint32_t lba;                  //holds the current LBA number. This is the first LBA in the UserBuffer.
    uint8_t * pCT1;                //holds current EPBCTxx register
    uint8_t * pCT2;                //holds next EPBCTxx register
    uint8_t * pEP2;                //holds addr of the next EP buffer
    uint16_t lbaCount;              //how many LBA we have to send to Host
    uint8_t bCurrentBufferXY;      //indicates which buffer is used by host to transmit data via OUT
    uint8_t bReadProcessing;       //indicated if the current state is DATA READ phase or CSW sending
                                //initiated by McsDataSend()
    uint8_t XorY;
};

//structure for common control of MSC stack
struct _MscControl {
    uint16_t wMscUserBufferSize;
    uint16_t lbaSize;               //limitid to uint16_t, but could be increased if required.
    uint8_t lbaBufCapacity;        //how many LBAs (max) contains UserBuffer for read/write operation (>=1)
    uint8_t *xBufferAddr;
    uint8_t *yBufferAddr;
    uint8_t bMediaPresent;
    uint8_t bWriteProtected;
};

struct _MscState {
    volatile uint32_t Scsi_Residue;
    volatile uint8_t Scsi_Status;  /*Variable to track command status */
    int16_t bMcsCommandSupported;  /*Flag to indicate read/write command is recieved from host */
    int16_t bMscCbwReceived;       /*Flag to inidicate whether any CBW recieved from host*/
    int16_t bMscSendCsw;
    int16_t isMSCConfigured;
    uint8_t bUnitAttention;
    uint8_t bMscCbwFailed;
    uint8_t bMscResetRequired;
	uint8_t stallEndpoint;
	uint8_t stallAtEndofTx;
};

extern struct _MscWriteControl MscWriteControl;
extern struct _MscReadControl MscReadControl;
extern struct _MscControl MscControl[];

/*----------------------------------------------------------------------------+
 | Extern Variables                                                            |
 +----------------------------------------------------------------------------*/

extern CBW cbw;
extern CSW csw;
extern REQUEST_SENSE_RESPONSE RequestSenseResponse;

/*----------------------------------------------------------------------------+
 | Function Prototypes                                                         |
 +----------------------------------------------------------------------------*/

/*SCSI Wrapper functions */
uint8_t Scsi_Cmd_Parser (uint8_t opcode);
uint8_t Scsi_Send_CSW (uint8_t intfNum);

/*Function to reset MSC SCSI state machine */
void Msc_ResetStateMachine(void);
void Msc_ResetFlags(void);
void Msc_ResetStruct(void);
void SET_RequestsenseNotReady(void);
void SET_RequestsenseMediaNotPresent(void);
void MscResetCtrlLun(void);

USBMSC_RWbuf_Info* USBMSC_fetchInformationStructure(void);
#ifdef __cplusplus
}
#endif
#endif  //_MSC_SCSI_H_

