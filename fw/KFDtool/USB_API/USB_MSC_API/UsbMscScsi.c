/** @file UsbMscScsi.c
 *  @brief Contains APIs related to MSC (Mass Storage) SCSI handling
 */
//
//! \cond
//

/* 
 * ======== UsbMscScsi.c ========
 */
/*----------------------------------------------------------------------------+
 | Includes                                                                    |
 +----------------------------------------------------------------------------*/
#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_Common/usb.h"
#include "../USB_MSC_API/UsbMscScsi.h"
#include "../USB_MSC_API/UsbMsc.h"
#include <descriptors.h>
#include <string.h>

#ifdef _MSC_

/*----------------------------------------------------------------------------+
 | Internal Definitions                                                        |
 +----------------------------------------------------------------------------*/
//Error codes
#define RESCODE_CURRENT_ERROR                    0x70

#define S_NO_SENSE                               0x00
#define S_NOT_READY                              0x02
#define S_MEDIUM_ERROR                           0x03
#define S_ILLEGAL_REQUEST                        0x05
#define S_UNITATTN                               0x06
#define S_WRITE_PROTECTED                        0x07
#define S_ABORTED_COMMAND                        0x0B

#define ASC_NOT_READY                            0x04
#define ASCQ_NOT_READY                           0x03

#define ASC_MEDIUM_NOT_PRESENT                   0x3A
#define ASCQ_MEDIUM_NOT_PRESENT                  0x00

#define ASC_INVALID_COMMAND_OP_CODE              0x20
#define ASCQ_INVALID_COMMAND_OP_CODE             0x00

#define ASC_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE   0x21
#define ASCQ_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE  0x00

#define ASC_INVALID_FIELD_IN_CDB                 0x24
#define ASCQ_INVALID_FIELD_IN_CDB                0x00

#define ASC_INVALID_PARAMETER_LIST               0x26
#define ASCQ_INVALID_PARAMETER_LIST              0x02

#define ASC_ABORTED_DATAPHASE_ERROR              0x4B
#define ASCQ_ABORTED_DATAPHASE_ERROR             0x00

#define ASC_ILLEGAL_REQUEST                      0x20
#define ASCQ_ILLEGAL_REQUEST                     0x00

#define ASC_UNITATTN_READY_NOTREADY              0x28
#define ASCQ_UNITATTN_READY_NOTREADY             0x00

#define ASC_WRITE_PROTECTED                      0X27
#define ASCQ_WRITE_PROTECTED                     0X00

#define ASC_WRITE_FAULT                          0x03
#define ASCQ_WRITE_FAULT                         0x00

#define ASC_UNRECOVERED_READ                     0x11
#define ASCQ_UNRECOVERED_READ                    0x00

#define DIRECTION_IN    0x80
#define DIRECTION_OUT   0x00

#define EP_MAX_PACKET_SIZE      0x40

void usbStallEndpoint (uint8_t);
uint8_t Scsi_Verify_CBW ();

extern struct config_struct USBMSC_config;

extern void *(*USB_TX_memcpy)(void * dest, const void * source, size_t count);
extern void *(*USB_RX_memcpy)(void * dest, const void * source, size_t count);

extern __no_init tEDB __data16 tInputEndPointDescriptorBlock[];
extern __no_init tEDB __data16 tOutputEndPointDescriptorBlock[];
/*----------------------------------------------------------------------------+
 | Global Variables                                                            |
 +----------------------------------------------------------------------------*/

struct _MscWriteControl MscWriteControl;
struct _MscReadControl MscReadControl;
struct _MscControl MscControl[MSC_MAX_LUN_NUMBER] = {0};

/* Structure internal to stack for maintaining LBA info,buffer address etc */
USBMSC_RWbuf_Info sRwbuf;

__no_init CBW McsCbw;
__no_init CSW McsCsw;

struct _MscState MscState;

/*----------------------------------------------------------------------------+
 | Initiliazing Command data                                                   |
 +----------------------------------------------------------------------------*/
uint8_t Scsi_Standard_Inquiry_Data[256];

REQUEST_SENSE_RESPONSE RequestSenseResponse;

struct _Scsi_Read_Capacity Scsi_Read_Capacity_10[MSC_MAX_LUN_NUMBER];

const struct _Report_Luns Report_Luns =  {{0x02,0x00,0x00,0x00},
                                          {0x00,0x00,0x00,0x00},
                                          {0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                           0x00}};

uint8_t Scsi_Mode_Sense_6[SCSI_MODE_SENSE_6_CMD_LEN] = {0x03,0,0,0 };              //No mode sense parameter

uint8_t Scsi_Mode_Sense_10[SCSI_MODE_SENSE_10_CMD_LEN] = {0,0x06,0,0,0,0,0,0 };    //No mode sense parameter

uint8_t Scsi_Read_Format_Capacity[SCSI_READ_FORMAT_CAPACITY_CMD_LEN] =
{0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00,0x03,0x00,0x02,0x00};

/*Default values initialized for SCSI Inquiry data */
const uint8_t bScsi_Standard_Inquiry_Data[SCSI_SCSI_INQUIRY_CMD_LEN] = {
#ifdef CDROM_SUPPORT
	0x05,                                                                       //Peripheral qualifier & peripheral device type
#else
	0x00,                                                                       //Peripheral qualifier & peripheral device type
#endif
	0x80,                                                                       //Removable medium
    0x02,                                                                       //Version of the standard (SPC-2)
    0x02,                                                                       //No NormACA, No HiSup, response data format=2
    0x1F,                                                                       //No extra parameters
    0x00,                                                                       //No flags
    0x00,                                                                       //0x80 => BQue => Basic Task Management supported
    0x00,                                                                       //No flags
    /* 'T','I',' ',' ',' ',' ',' ',' ',
     * 'M','a','s','s',' ','S','t','o','r','a','g','e', */
};

#ifdef CDROM_SUPPORT

/* SCSI TOC Record - Pg.459 of mmc6r02g.pdf */
const uint8_t Scsi_Read_TOC_PMA_ATIP_F1[Scsi_Read_TOC_PMA_ATIP_F1_LEN] = {
                0x00, 0x12,                  // Length
                0x01,                        // First Track
                0x01,                        // Last Track

                0x00,                        // Reserved
                0x14,                        // ADR/CTL
                0x01,                        // Track Number
                0x00,                        // Reserved
                0x00, 0x00, 0x02, 0x00,      // Track Address (TIME Form)
                0x00,0x00,0x00,0x00,		 // Padding
                0x00,0x00,0x00,0x00
}; 

const uint8_t Scsi_Read_TOC_PMA_ATIP_F2[Scsi_Read_TOC_PMA_ATIP_F2_LEN] = {
                0x00, 0x2E,                  // Length
                0x01,                        // First Track
                0x01,                        // Last Track
                0x01,                        // Reserved
                0x14,                        // ADR/CTL
                0x00,                        // Track Number
                0xA0,                        // Reserved
                0x00, 0x00, 0x00, 0x00,      // Track Address (TIME Form)
                0x01,0x00,0x00,0x01,		 // Padding/Descriptors
                0x14,0x00,0xA1,0x00,
                0x00,0x00,0x00,0x01,
                0x00,0x00,0x01,0x14,
                0x00,0xA2,0x00,0x00,
                0x00,0x00,0x1C,0x35,
                0x30,0x01,0x14,0x00,
                0x01,0x00,0x00,0x00,
                0x00,0x00,0x02,0x00
}; 

/* GET_CONFIGURATION Response Pg. 312 of mmc6r02g.pdf */
const uint8_t Scsi_Get_Configuration_Descriptor[SCSI_GET_CONFIGURATION_LEN] = {

                /* Feature Header */
                0x00,0x00,0x00,0x00         // Length
};

/* EVENT STATUS Response Pg. 316 of mmc6r02g.pdf */
const uint8_t Scsi_Event_Status_Descriptor[SCSI_EVENT_STATUS_LEN] = {

                /* Feature Header */
                0x00,0x06,                  // Event Descriptor Length
                0x04,                       // NEA/Reserved/Notification Class
                0x54,                       // Supported Event Classes
                0x02,                       // Reserved/Event Code
                0x02,
                0x00,0x00
};


/* READ_DISC_INFORMATION Response Pg. 374 of mmc6r02g.pdf  */
const uint8_t Scsi_Disc_Information_Descriptor[SCSI_READ_DISC_INFORMATION_LEN] = {

                0x00,0x00,                      // Disc Information Length
                0x00,                           // Disc Information Type, Non-Erasable, Last Session, Finalized
                0x00,                           // First Track on Disc
                0x00,                           // Number of Sessions
                0x00,                           // First Track Number in Last Session
                0x00,                           // Last Track Number in Last Session
                0x00,                           // BG/Barcode/ID Disable
                0x00,                           // Disc Type (CD-ROM)
                0x00,                           // Number of Sessions (MSB)
                0x00,                           // First Track Number in Last Session (MSB)
                0x00,                           // Last Track Number in Last Sessions (MSB)
                0x00,0x00,0x00,0x00,            // Disc ID
                0x00,0x00,0x00,0x00,            // Last Session Lead-In Address
                0x00,0x00,0x00,0x00,            // Last Lead-Out Start Address
                0x00,0x00,0x00,0x00,            // Bar Code (Not Supported)
                0x00,0x00,0x00,0x00,
                0x00,                           // Disc Application Code (Not Supported)
                0x00,0x00,0x00                   // No OPC Entries
};

void Scsi_Read_TocPmaAtip(uint8_t intfNum);
void Scsi_Get_Configuration(uint8_t intfNum);
void Scsi_Event_Status_Notification(uint8_t intfNum);
void Scsi_Read_Disc_Information(uint8_t intfNum);

#endif
/*----------------------------------------------------------------------------+
 | Functions                                                                   |
 +----------------------------------------------------------------------------*/
void Reset_RequestSenseResponse (void)
{
    int16_t i;

    RequestSenseResponse.ResponseCode = RESCODE_CURRENT_ERROR;
    RequestSenseResponse.VALID = 0;                                             //no data in the information field
    RequestSenseResponse.Obsolete = 0x00;
    RequestSenseResponse.SenseKey = S_NO_SENSE;
    RequestSenseResponse.ILI = 0;
    RequestSenseResponse.EOM = 0;
    RequestSenseResponse.FILEMARK = 0;
    RequestSenseResponse.Information[0] = 0x00;
    RequestSenseResponse.Information[1] = 0x00;
    RequestSenseResponse.Information[2] = 0x00;
    RequestSenseResponse.Information[3] = 0x00;
    RequestSenseResponse.AddSenseLen = 0x0a;
    RequestSenseResponse.CmdSpecificInfo[0] = 0x00;
    RequestSenseResponse.CmdSpecificInfo[1] = 0x00;
    RequestSenseResponse.CmdSpecificInfo[2] = 0x00;
    RequestSenseResponse.CmdSpecificInfo[3] = 0x00;
    RequestSenseResponse.ASC = 0x00;
    RequestSenseResponse.ASCQ = 0x00;
    RequestSenseResponse.FRUC = 0x00;
    RequestSenseResponse.SenseKeySpecific[0] = 0x00;
    RequestSenseResponse.SenseKeySpecific[1] = 0x00;
    RequestSenseResponse.SenseKeySpecific[2] = 0x00;
    for (i = 0; i < 14; i++){
        RequestSenseResponse.padding[i] = 0x00;
    }
}

//----------------------------------------------------------------------------

uint8_t Check_CBW (uint8_t intfNum,uint8_t Dir_Dev_Exp, uint32_t Bytes_Dev_Exp)
{
    if (McsCbw.CBWCB[0] == SCSI_INQUIRY || McsCbw.CBWCB[0] ==
        SCSI_REQUEST_SENSE){
        return (SUCCESS);
    }

    if (Dir_Dev_Exp == McsCbw.bmCBWFlags){                  //all is right. Host is sending direction as expected by device
        if (McsCbw.dCBWDataTransferLength < Bytes_Dev_Exp){ //Host expect less data to send or receive then device
            MscState.Scsi_Status = SCSI_PHASE_ERROR;
            MscState.Scsi_Residue = 0 ;
            if (McsCbw.bmCBWFlags == DIRECTION_IN){
                usbStallInEndpoint(intfNum);
            } else {
                usbStallOutEndpoint(intfNum);
            }
        } else if ((McsCbw.dCBWDataTransferLength > Bytes_Dev_Exp) &&
                   (McsCbw.CBWCB[0] != SCSI_MODE_SENSE_6) &&
                   (McsCbw.CBWCB[0] != SCSI_MODE_SENSE_10) &&
                   (McsCbw.CBWCB[0] != SCSI_READ_TOC_PMA_ATIP)){
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = McsCbw.dCBWDataTransferLength -
                                    Bytes_Dev_Exp;
            if (McsCbw.bmCBWFlags == DIRECTION_IN){
                usbStallInEndpoint(intfNum);
            } else {
                usbStallOutEndpoint(intfNum);
            }
        } else {
            return ( SUCCESS) ;
        }
    } else {    //Direction mismatch
        MscState.Scsi_Residue = McsCbw.dCBWDataTransferLength;
        MscState.Scsi_Status = SCSI_FAILED;
        if (McsCbw.bmCBWFlags == DIRECTION_IN){
            usbStallInEndpoint(intfNum);
        } else if ((McsCbw.bmCBWFlags == DIRECTION_OUT) &&
                   (McsCbw.CBWCB[0] == SCSI_READ_10)){
            usbStallOutEndpoint(intfNum);
        }
    }

    //Indicates a generic failure. Read/write failure/sense data is handled separately
    if (MscState.Scsi_Status != SCSI_READWRITE_FAIL){
        RequestSenseResponse.ResponseCode = RESCODE_CURRENT_ERROR;
        RequestSenseResponse.VALID = 1;
        RequestSenseResponse.AddSenseLen = 0xA0;
        RequestSenseResponse.SenseKey = S_ILLEGAL_REQUEST;
        RequestSenseResponse.ASC = ASC_INVALID_PARAMETER_LIST;
        RequestSenseResponse.ASCQ = ASCQ_INVALID_PARAMETER_LIST;
    }

    return (FAILURE);
}

//----------------------------------------------------------------------------
uint8_t Scsi_Verify_CBW ()
{
    /*(5.2.3) Devices must consider the CBW meaningful if no reserved bits
     * are set, the LUN number indicates a LUN supported by the device,
     * bCBWCBLength is in the range of 1 through 16, and the length and
     * content of the CBWCB field are appropriate to the SubClass.
     */
    if ((MscState.bMscResetRequired || McsCbw.dCBWSignature !=
         CBW_SIGNATURE) ||              //Check for correct CBW signature
        ((McsCbw.bmCBWFlags != DIRECTION_IN && McsCbw.bmCBWFlags !=
          DIRECTION_OUT) ||
         (McsCbw.bCBWLUN & 0xF0) ||     //Upper bits have to be zero
         (McsCbw.bCBWCBLength > 16))){  //maximum length is 16
        MscState.bMscResetRequired = TRUE;
        usbStallEndpoint(MSC0_INTFNUM);
        usbClearOEPByteCount(MSC0_INTFNUM);
        MscState.Scsi_Status = SCSI_FAILED;
        MscState.Scsi_Residue = 0;
        return (FAILURE);
    }
    MscState.Scsi_Status = SCSI_PASSED;
    return (SUCCESS);
}

//----------------------------------------------------------------------------
uint8_t Scsi_Send_CSW (uint8_t intfNum)
{
    uint8_t retval = 0;

    //Populate the CSW to be sent
    McsCsw.dCSWSignature = CSW_SIGNATURE;
    McsCsw.dCSWTag = McsCbw.dCBWTag;
    McsCsw.bCSWStatus = MscState.Scsi_Status;
    McsCsw.dCSWDataResidue = MscState.Scsi_Residue;
    retval = MscSendData((uint8_t*)&McsCsw, CSW_LENGTH);   //Sending CSW
    MscState.Scsi_Status = SCSI_PASSED;
    return (retval);
}

//----------------------------------------------------------------------------

void Scsi_Inquiry (uint8_t intfNum)
{
    //int16_t index;

    //clear the inquiry array
    memset(Scsi_Standard_Inquiry_Data, 256, 0);
    //copy the inquiry data from flash to RAM

    memcpy(Scsi_Standard_Inquiry_Data,
        bScsi_Standard_Inquiry_Data,
        SCSI_SCSI_INQUIRY_CMD_LEN);



    //get the values from USB_Config
    Scsi_Standard_Inquiry_Data[1] = USBMSC_config.LUN[McsCbw.bCBWLUN].removable;
    memcpy(&Scsi_Standard_Inquiry_Data[8],
        USBMSC_config.LUN[McsCbw.bCBWLUN].t10VID,
        8);
    memcpy(&Scsi_Standard_Inquiry_Data[16],
        USBMSC_config.LUN[McsCbw.bCBWLUN].t10PID,
        16);
    memcpy(&Scsi_Standard_Inquiry_Data[32],
        USBMSC_config.LUN[McsCbw.bCBWLUN].t10rev,
        4);

    if (McsCbw.dCBWDataTransferLength < SCSI_SCSI_INQUIRY_CMD_LEN){
        if (McsCbw.dCBWDataTransferLength == 0){
            MscState.Scsi_Residue = 0;
            return;
        }
        if (SUCCESS ==
            MscSendData((uint8_t*)Scsi_Standard_Inquiry_Data,
                McsCbw.dCBWDataTransferLength)){
            MscState.Scsi_Residue = 0;
        } else {
            MscState.Scsi_Status = SCSI_FAILED;
        }
    } else if (McsCbw.dCBWDataTransferLength > SCSI_SCSI_INQUIRY_CMD_LEN){
        Reset_RequestSenseResponse();

        RequestSenseResponse.ResponseCode = RESCODE_CURRENT_ERROR;
        RequestSenseResponse.VALID = 1;
        RequestSenseResponse.SenseKey = S_ILLEGAL_REQUEST;
        RequestSenseResponse.ASC = ASC_INVALID_FIELD_IN_CDB;
        RequestSenseResponse.ASCQ = ASCQ_INVALID_FIELD_IN_CDB;
        usbStallInEndpoint(intfNum);
        MscState.Scsi_Status = SCSI_FAILED;
    } else {
        if (SUCCESS ==
            MscSendData((uint8_t*)Scsi_Standard_Inquiry_Data,
                SCSI_SCSI_INQUIRY_CMD_LEN)){
            MscState.Scsi_Residue = 0;
        } else {
            MscState.Scsi_Status = SCSI_FAILED;
        }
    }
}

//----------------------------------------------------------------------------

void Scsi_Read_Capacity10 (uint8_t intfNum)
{
    if (FAILURE == Check_CBW(intfNum,DIRECTION_IN,SCSI_READ_CAPACITY_CMD_LEN)){
        return;
    }
	MscState.Scsi_Residue = 0;
    if (SUCCESS !=
        MscSendData( (uint8_t*)&Scsi_Read_Capacity_10[McsCbw.bCBWLUN],
            SCSI_READ_CAPACITY_CMD_LEN)){
        MscState.Scsi_Status = SCSI_FAILED;
    }
}


//----------------------------------------------------------------------------

void Scsi_Read10 (uint8_t intfNum)
{
    uint16_t wLBA_len;
    uint16_t state;
    uint8_t edbIndex;
    uint32_t dLBA;

    /* Get first LBA: convert 4 bytes into uint32_t */
    dLBA = McsCbw.CBWCB[2];
    dLBA <<= 8;
    dLBA += McsCbw.CBWCB[3];
    dLBA <<= 8;
    dLBA += McsCbw.CBWCB[4];
    dLBA <<= 8;
    dLBA += McsCbw.CBWCB[5];

    /* Get number of requested logical blocks */
    wLBA_len = McsCbw.CBWCB[7];
    wLBA_len <<= 8;
    wLBA_len += McsCbw.CBWCB[8];

    if (FAILURE ==
        Check_CBW( intfNum, DIRECTION_IN, ((uint32_t)wLBA_len) *
            MscControl[McsCbw.bCBWLUN].lbaSize)){
        return;
    }

    edbIndex = stUsbHandle[intfNum].edb_Index;
    state = usbDisableOutEndpointInterrupt(edbIndex);

    //Populating stack internal structure required for READ/WRITE
    MscReadControl.lba = dLBA;              //the first LBA number.
    MscReadControl.lbaCount = wLBA_len;     //how many LBAs to read.
    MscReadControl.XorY = 0;

    sRwbuf.bufferAddr = MscControl[McsCbw.bCBWLUN].xBufferAddr;
    sRwbuf.lun = McsCbw.bCBWLUN;

    //set LBA count
    sRwbuf.lbCount = wLBA_len >
        MscControl[McsCbw.bCBWLUN].lbaBufCapacity ? 
		MscControl[McsCbw.bCBWLUN].lbaBufCapacity : wLBA_len;
    sRwbuf.operation = USBMSC_READ;
    sRwbuf.lba = dLBA;
    sRwbuf.returnCode = USBMSC_RW_SUCCESS;
    sRwbuf.XorY = 0;
    sRwbuf.xBufFull = 0;
    sRwbuf.xWordCnt = 0;
    sRwbuf.yBufFull = 0;
    sRwbuf.yWordCnt = 0;
    sRwbuf.bufferProcessed = 0;
    sRwbuf.firstFlag = 0;
    //buffer is prepared, let user's Application fill data.
    USBMSC_handleBufferEvent();

    usbRestoreOutEndpointInterrupt(state);
}

//----------------------------------------------------------------------------

void Scsi_Write10 (uint8_t intfNum)
{
    uint16_t wLBA_len;
    uint8_t edbIndex;
    uint16_t state;
    /* Get first LBA: convert 4 bytes into uint32_t */
    uint32_t dLBA = McsCbw.CBWCB[2];

    dLBA <<= 8;
    dLBA += McsCbw.CBWCB[3];
    dLBA <<= 8;
    dLBA += McsCbw.CBWCB[4];
    dLBA <<= 8;
    dLBA += McsCbw.CBWCB[5];

    /* Get number of requested logical blocks */
    wLBA_len = McsCbw.CBWCB[7];
    wLBA_len <<= 8;
    wLBA_len += McsCbw.CBWCB[8];

    if (FAILURE ==
        Check_CBW(intfNum,DIRECTION_OUT,((uint32_t)wLBA_len) *
            MscControl[McsCbw.bCBWLUN].lbaSize)){
        return;
    }

    edbIndex = stUsbHandle[intfNum].edb_Index;
    state = usbDisableInEndpointInterrupt(edbIndex);

    //calculate the whole size to receive (Host to MSP430)
    MscWriteControl.dwBytesToReceiveLeft = (uint32_t)wLBA_len *
        MscControl[McsCbw.bCBWLUN].lbaSize;
    MscWriteControl.pUserBuffer = MscControl[McsCbw.bCBWLUN].xBufferAddr;
    MscWriteControl.wFreeBytesLeft =
        MscControl[McsCbw.bCBWLUN].wMscUserBufferSize;

    /*Populating stack internal structure required for READ/WRITE */

    MscWriteControl.bWriteProcessing = TRUE;    //indicate that we are in WRITE phase
    MscWriteControl.lba = dLBA;
    MscWriteControl.wCurrentByte = 0;           //reset internal variable
    MscWriteControl.lbaCount = 0;               //reset internal variable
    MscWriteControl.XorY = 0;

    sRwbuf.lun = McsCbw.bCBWLUN;
    sRwbuf.operation = 0;
    sRwbuf.lba = 0;
    sRwbuf.lbCount = 0;
    sRwbuf.bufferAddr = MscControl[McsCbw.bCBWLUN].xBufferAddr;
    sRwbuf.returnCode = 0;
    sRwbuf.XorY = 0;
    sRwbuf.xBufFull = 0;
    sRwbuf.yBufFull = 0;
    sRwbuf.bufferProcessed = 0;
    sRwbuf.firstFlag = 0;
    sRwbuf.xlba = 0;
    sRwbuf.xlbaCount = 0;
    sRwbuf.ylba = 0;
    sRwbuf.ylbaCount = 0;

    usbRestoreInEndpointInterrupt(state);
}

//----------------------------------------------------------------------------

void Scsi_Mode_Sense6 (uint8_t intfNum)
{
    if (FAILURE == Check_CBW(intfNum,DIRECTION_IN,SCSI_MODE_SENSE_6_CMD_LEN)){
        return;
    }
    /* Fix for SDOCM00077834 - Set WP bit. WP bit is BIT7 in byte 3 */
    Scsi_Mode_Sense_6[2] |= (MscControl[McsCbw.bCBWLUN].bWriteProtected << 0x7);

	MscState.Scsi_Residue =  McsCbw.dCBWDataTransferLength -
			SCSI_MODE_SENSE_6_CMD_LEN;
	if (MscState.Scsi_Residue) {
	    MscState.stallAtEndofTx = TRUE;
	}
    if (SUCCESS !=
        MscSendData((uint8_t*)Scsi_Mode_Sense_6, SCSI_MODE_SENSE_6_CMD_LEN)){
        MscState.Scsi_Status = SCSI_FAILED;
    }
}

//----------------------------------------------------------------------------

void Scsi_Mode_Sense10 (uint8_t intfNum)
{
    if (FAILURE == Check_CBW(intfNum,DIRECTION_IN,SCSI_MODE_SENSE_10_CMD_LEN)){
        return;
    }
    /* Fix for SDOCM00077834 - Set WP bit. WP bit is BIT7 in byte 3 */
    Scsi_Mode_Sense_10[4] |= (MscControl[McsCbw.bCBWLUN].bWriteProtected << 0x7);
	MscState.Scsi_Residue = McsCbw.dCBWDataTransferLength -
			SCSI_MODE_SENSE_10_CMD_LEN;
	if (MscState.Scsi_Residue) {
	    MscState.stallAtEndofTx = TRUE;
	}
    if (SUCCESS !=
        MscSendData((uint8_t*)Scsi_Mode_Sense_10, SCSI_MODE_SENSE_10_CMD_LEN)){
        MscState.Scsi_Status = SCSI_FAILED;
    }
}

//----------------------------------------------------------------------------

void Scsi_Request_Sense (uint8_t intfNum)
{
    if (FAILURE == Check_CBW(intfNum,DIRECTION_IN,SCSI_REQ_SENSE_CMD_LEN)){
        return;
    }

    //If there is attention needed, setup the request sense response. The
    //bUnitAttention flag is set in USBMSC_updateMediaInformation() when the volume
    //is removed or inserted. Note that the response is different for the
    //removed and inserted case.
    if (MscState.bUnitAttention == TRUE){
        //Check if the volume was removed.
        if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
            USBMSC_MEDIA_NOT_PRESENT){
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_NOT_READY;
            RequestSenseResponse.ASC = ASC_MEDIUM_NOT_PRESENT;
            RequestSenseResponse.ASCQ = ASCQ_MEDIUM_NOT_PRESENT;
        }
        //Otherwise it was inserted.
        else {
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_UNITATTN;
            RequestSenseResponse.ASC = ASC_UNITATTN_READY_NOTREADY;
            RequestSenseResponse.ASCQ = ASCQ_UNITATTN_READY_NOTREADY;
        }
    }

    if (McsCbw.dCBWDataTransferLength < SCSI_REQ_SENSE_CMD_LEN){
        if (SUCCESS ==
            MscSendData((uint8_t*)&RequestSenseResponse,
                McsCbw.dCBWDataTransferLength)){
            MscState.Scsi_Residue = 0;
        } else {
            MscState.Scsi_Status = SCSI_FAILED;
        }
    } else if (McsCbw.dCBWDataTransferLength > SCSI_REQ_SENSE_CMD_LEN){
        RequestSenseResponse.AddSenseLen +=
            (McsCbw.dCBWDataTransferLength -  SCSI_REQ_SENSE_CMD_LEN);
        if (SUCCESS ==
            MscSendData((uint8_t*)&RequestSenseResponse,
                McsCbw.dCBWDataTransferLength)){
            MscState.Scsi_Residue = 0;
        } else {
            MscState.Scsi_Status = SCSI_FAILED;
        }
    } else {
        if (SUCCESS ==
            MscSendData((uint8_t*)&RequestSenseResponse,SCSI_REQ_SENSE_CMD_LEN)){
            MscState.Scsi_Residue = 0;
        } else {
            MscState.Scsi_Status = SCSI_FAILED;
        }
    }

    //Clear the bUnitAttention flag after the response was properly sent via
    //MscSendData().
    if (MscState.bUnitAttention == TRUE){
        MscState.bUnitAttention = FALSE;
    }
}

//----------------------------------------------------------------------------

void Scsi_Test_Unit_Ready (uint8_t intfNum)
{
    if (SUCCESS != Check_CBW(intfNum,DIRECTION_OUT,0)){
        MscState.Scsi_Status = SCSI_FAILED;
    }

    Reset_RequestSenseResponse();
}

//----------------------------------------------------------------------------

void Scsi_Unknown_Request (uint8_t intfNum)
{
    Reset_RequestSenseResponse();

    RequestSenseResponse.ResponseCode = RESCODE_CURRENT_ERROR;
    RequestSenseResponse.VALID = 1;
    RequestSenseResponse.AddSenseLen = 0xA0;
    RequestSenseResponse.SenseKey = S_ILLEGAL_REQUEST;
    RequestSenseResponse.ASC = ASC_INVALID_COMMAND_OP_CODE;
    RequestSenseResponse.ASCQ = ASCQ_INVALID_COMMAND_OP_CODE;
    MscState.Scsi_Residue = 0;
    MscState.Scsi_Status = SCSI_FAILED;

    if (McsCbw.dCBWDataTransferLength && (McsCbw.bmCBWFlags == DIRECTION_IN)){
        MscState.bMcsCommandSupported = FALSE;
        usbStallInEndpoint(intfNum);
    }
    if (McsCbw.dCBWDataTransferLength && (McsCbw.bmCBWFlags == DIRECTION_OUT)){
        MscState.bMcsCommandSupported = FALSE;
        usbStallOutEndpoint(intfNum);
    }
}

//----------------------------------------------------------------------------

void Scsi_Report_Luns (uint8_t intfNum)
{
    if (FAILURE ==
        Check_CBW( intfNum, DIRECTION_IN, SCSI_REPORT_LUNS_CMD_LEN)){
        return;
    }
    if (SUCCESS !=
        MscSendData( (uint8_t*)&Report_Luns, SCSI_REPORT_LUNS_CMD_LEN)){
        MscState.Scsi_Status = SCSI_FAILED;
    }
}

//----------------------------------------------------------------------------

uint8_t Scsi_Cmd_Parser (uint8_t intfNum)
{
    uint8_t ret = USBMSC_COMMAND_BEING_PROCESSED;

    //MscState.Scsi_Status = SCSI_FAILED;
    MscState.Scsi_Residue = McsCbw.dCBWDataTransferLength;

    //fails the commands during UNIT ATTENTION
    if ((MscState.bUnitAttention) && (McsCbw.CBWCB[0] != SCSI_INQUIRY) &&
        (McsCbw.CBWCB[0] != SCSI_REQUEST_SENSE)){
        MscState.Scsi_Status = SCSI_FAILED;
        return (USB_GENERAL_ERROR);
    }

    if (!McsCbw.bCBWCBLength){
        return (USB_GENERAL_ERROR);
    }

    switch (McsCbw.CBWCB[0])                                        //SCSI Operation code
    {
        case SCSI_READ_10:
            if (MscControl[McsCbw.bCBWLUN].xBufferAddr == NULL){    //Check for null address.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseNotReady();
                MscState.Scsi_Status = SCSI_FAILED;
                usbStallInEndpoint(intfNum);
                break;
            }

            if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                USBMSC_MEDIA_NOT_PRESENT){                         //Check for media present. Do this for any command that accesses
                                                                    //media.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseMediaNotPresent();
                usbStallInEndpoint(intfNum);
                break;
            }
            Scsi_Read10(intfNum);
            break;

        case SCSI_WRITE_10:
            if (MscControl[McsCbw.bCBWLUN].xBufferAddr == NULL){    //Check for null address.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseNotReady();
                MscState.Scsi_Status = SCSI_FAILED;
                break;
            }

            if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                USBMSC_MEDIA_NOT_PRESENT){                         //Check for media present. Do this for any command that accesses
                                                                    //media.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseMediaNotPresent();
                usbStallOutEndpoint(intfNum);
                break;
            }

            if (MscControl[McsCbw.bCBWLUN].bWriteProtected){        //Do this only for WRITE
                ret = USB_GENERAL_ERROR;
                                                                    //Set REQUEST SENSE with "write protected"
                Reset_RequestSenseResponse();
                RequestSenseResponse.VALID = 1;
                RequestSenseResponse.SenseKey = S_WRITE_PROTECTED;
                RequestSenseResponse.ASC = ASC_WRITE_PROTECTED;
                RequestSenseResponse.ASCQ = ASCQ_WRITE_PROTECTED;
                MscWriteControl.bWriteProcessing = FALSE;
                                                                    //Send CSW with error status
                MscState.Scsi_Residue = 1;
                MscState.Scsi_Status = SCSI_FAILED;
                usbStallOutEndpoint(intfNum);
                break;
            }

            Scsi_Write10(intfNum);
            break;

        case START_STOP_UNIT:
        case PREVENT_ALLW_MDM:
        case SCSI_MODE_SELECT_10:
        case SCSI_MODE_SELECT_6:
        case SCSI_TEST_UNIT_READY:
            if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                USBMSC_MEDIA_NOT_PRESENT){                         //Check for media present. Do this for any command that accesses
                                                                    //media.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseMediaNotPresent();
                break;
            }
            Scsi_Test_Unit_Ready(intfNum);
            break;
            
        case SCSI_SET_CD_SPEED:
        	break;
        case SCSI_INQUIRY:
            Scsi_Inquiry(intfNum);
            break;

        case SCSI_MODE_SENSE_6:
            Scsi_Mode_Sense6(intfNum);
            break;

        case SCSI_MODE_SENSE_10:
            Scsi_Mode_Sense10(intfNum);
            break;

        case SCSI_READ_CAPACITY_10:
            if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                USBMSC_MEDIA_NOT_PRESENT){             //Check for media present. Do this for any command that accesses media.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseMediaNotPresent();
                usbStallInEndpoint(intfNum);
                break;
            }
            Scsi_Read_Capacity10(intfNum);
            break;

        case SCSI_REQUEST_SENSE:
            Scsi_Request_Sense(intfNum);
            break;

        case SCSI_REPORT_LUNS:
            if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                USBMSC_MEDIA_NOT_PRESENT){             //Check for media present. Do this for any command that accesses media.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseMediaNotPresent();
                if (McsCbw.bmCBWFlags == DIRECTION_IN){
                    usbStallInEndpoint(intfNum);
                } else {
                    usbStallOutEndpoint(intfNum);
                }
                break;
            }
            Scsi_Report_Luns(intfNum);
            break;
        case SCSI_VERIFY:
                                                        /* Fix for SDOCM00078183 */
                                                        /* NOTE: we are assuming that BYTCHK=0 and PASSing the command. */
            break;
#ifdef CDROM_SUPPORT

        case SCSI_READ_TOC_PMA_ATIP:
            if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                USBMSC_MEDIA_NOT_PRESENT){             //Check for media present. Do this for any command that accesses media.
                ret = USB_GENERAL_ERROR;
                SET_RequestsenseMediaNotPresent();
                usbStallInEndpoint(intfNum);
                break;
            }
            Scsi_Read_TocPmaAtip(intfNum);
            break;

        case SCSI_GET_CONFIGURATION:
                if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                    USBMSC_MEDIA_NOT_PRESENT){             //Check for media present. Do this for any command that accesses media.
                        ret = USB_GENERAL_ERROR;
                        SET_RequestsenseMediaNotPresent();
                        usbStallInEndpoint(intfNum);
                        break;
                }
                Scsi_Get_Configuration(intfNum);
                break;
        case SCSI_EVENT_STATUS:
                if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                    USBMSC_MEDIA_NOT_PRESENT){             //Check for media present. Do this for any command that accesses media.
                        ret = USB_GENERAL_ERROR;
                        SET_RequestsenseMediaNotPresent();
                        usbStallInEndpoint(intfNum);
                        break;
                }
                Scsi_Event_Status_Notification(intfNum);
                break;

        case SCSI_READ_DISC_INFORMATION:

                if (MscControl[McsCbw.bCBWLUN].bMediaPresent ==
                    USBMSC_MEDIA_NOT_PRESENT){             //Check for media present. Do this for any command that accesses media.
                        ret = USB_GENERAL_ERROR;
                        SET_RequestsenseMediaNotPresent();
                        usbStallInEndpoint(intfNum);
                        break;
                }

                Scsi_Read_Disc_Information(intfNum);
                break;
#endif
        default:
            ret = USB_GENERAL_ERROR;
            Scsi_Unknown_Request(intfNum);
            break;
    }
    return (ret);
}

//-------------------------------------------------------------------------------------------------------
/* This function is called only from ISR(only on Input endpoint interrupt to transfer data to host)
 * This function actually performs the data transfer to host Over USB */
int16_t MSCToHostFromBuffer ()
{
    //Check if there are any pending LBAs to process
    uint8_t * pEP1;
    uint8_t * pEP2;
    uint8_t * pCT1;
    uint8_t * pCT2;
    uint8_t bWakeUp = FALSE;                               //per default we do not wake up after interrupt
    uint8_t edbIndex;
    uint8_t bCount;

    if (MscControl[sRwbuf.lun].yBufferAddr == NULL) {
	    //Check if there are any pending data to send
	    if (MscReadControl.dwBytesToSendLeft == 0){
	        //no more data to send - clear ready busy status
	        MscReadControl.bReadProcessing = FALSE;

	        //check if more LBA to send out pending...
	        if (MscReadControl.lbaCount > 0){
	            sRwbuf.lba = MscReadControl.lba;            //update current lba
	            sRwbuf.lbCount = MscControl[sRwbuf.lun].lbaBufCapacity >
	                             MscReadControl.lbaCount ?
	                             MscReadControl.lbaCount : MscControl[sRwbuf.lun].
	                             lbaBufCapacity;            //update LBA count
	            sRwbuf.operation = USBMSC_READ;            //start data READ phase
	            sRwbuf.returnCode = USBMSC_RW_SUCCESS;
	            sRwbuf.bufferAddr = MscControl[sRwbuf.lun].xBufferAddr;
	            sRwbuf.XorY = 0;                        //only one buffer is active
	            //buffer is prepared, let user's Application fill data.
	            USBMSC_handleBufferEvent();
	        }
	        return (TRUE);                                  //data sent out - wake up!
    	}
	}
	else {
		if ((MscReadControl.lbaCount > 0) && (sRwbuf.bufferProcessed == 1)) {
	    	if ((sRwbuf.XorY == 0) && (sRwbuf.yBufFull == 0)){
	    		sRwbuf.bufferProcessed = 0;
	    		sRwbuf.XorY = 1;
	    		sRwbuf.bufferAddr = MscControl[sRwbuf.lun].yBufferAddr;
	            sRwbuf.lba = MscReadControl.lba;            //update current lba
	            sRwbuf.lbCount = MscControl[sRwbuf.lun].lbaBufCapacity >
	                             MscReadControl.lbaCount ?
	                             MscReadControl.lbaCount : MscControl[sRwbuf.lun].
	                             lbaBufCapacity;            //update LBA count
	            sRwbuf.operation = USBMSC_READ;            //start data READ phase
	            sRwbuf.returnCode = USBMSC_RW_SUCCESS;
	            //buffer is prepared, let user's Application fill data.
	            USBMSC_handleBufferEvent();
	    	}
	    	else if ((sRwbuf.XorY == 1) && (sRwbuf.xBufFull == 0)){
	    		sRwbuf.bufferProcessed = 0;
	    		sRwbuf.XorY = 0;
	    		sRwbuf.bufferAddr = MscControl[sRwbuf.lun].xBufferAddr;
	            sRwbuf.lba = MscReadControl.lba;            //update current lba
	            sRwbuf.lbCount = MscControl[sRwbuf.lun].lbaBufCapacity >
	                             MscReadControl.lbaCount ?
	                             MscReadControl.lbaCount : MscControl[sRwbuf.lun].
	                             lbaBufCapacity;            //update LBA count
	            sRwbuf.operation = USBMSC_READ;            //start data READ phase
	            sRwbuf.returnCode = USBMSC_RW_SUCCESS;
	            //buffer is prepared, let user's Application fill data.
	            USBMSC_handleBufferEvent();
	    	}
	    }

	    //Check if there are any pending data to send
	    if (MscReadControl.dwBytesToSendLeft == 0){
	        //no more data to send - clear ready busy status
	        MscReadControl.bReadProcessing = FALSE;

			if (MscReadControl.XorY == 0) {
				sRwbuf.xBufFull = 0;
				sRwbuf.xWordCnt = 0;
				if (sRwbuf.yBufFull) {
					MscSendData(MscControl[sRwbuf.lun].yBufferAddr, sRwbuf.yWordCnt);
					MscReadControl.XorY = 1;
				}
			}
			else {
				sRwbuf.yBufFull = 0;
				sRwbuf.yWordCnt = 0;
				if (sRwbuf.xBufFull) {
					MscSendData(MscControl[sRwbuf.lun].xBufferAddr, sRwbuf.xWordCnt);
					MscReadControl.XorY = 0;
				}
			}

	        return (TRUE);                                  //data sent out - wake up!
	    }
	}
	

    edbIndex = stUsbHandle[MSC0_INTFNUM].edb_Index;

    //check if the endpoint is stalled = do not send data.
    if (tInputEndPointDescriptorBlock[edbIndex].bEPCNF & EPCNF_STALL){
        return (TRUE);
    }

    //send one chunk of 64 bytes
    //check what is current buffer: X or Y
    if (MscReadControl.bCurrentBufferXY == X_BUFFER){   //X is current buffer
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].iep_X_Buffer;
        pCT1 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTX;

        //second EP buffer
        pEP2 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].iep_Y_Buffer;
        pCT2 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTY;
    } else {
        //this is the active EP buffer
        pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].iep_Y_Buffer;
        pCT1 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTY;

        //second EP buffer
        pEP2 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].iep_X_Buffer;
        pCT2 = &tInputEndPointDescriptorBlock[edbIndex].bEPBCTX;
    }

    //how many byte we can send over one endpoint buffer
    bCount =
        (MscReadControl.dwBytesToSendLeft >
         EP_MAX_PACKET_SIZE) ? EP_MAX_PACKET_SIZE : MscReadControl.
        dwBytesToSendLeft;

    if (*pCT1 & EPBCNT_NAK){
        USB_TX_memcpy(pEP1, MscReadControl.pUserBuffer, bCount);    //copy data into IEPx X or Y buffer
        *pCT1 = bCount;                                             //Set counter for usb In-Transaction
        MscReadControl.bCurrentBufferXY =
            (MscReadControl.bCurrentBufferXY + 1) & 0x01;           //switch buffer
        MscReadControl.dwBytesToSendLeft -= bCount;
        MscReadControl.pUserBuffer += bCount;                       //move buffer pointer

        //try to send data over second buffer
        if ((MscReadControl.dwBytesToSendLeft > 0) &&               //do we have more data to send?
            (*pCT2 & EPBCNT_NAK)){                                  //if the second buffer is free?
            //how many byte we can send over one endpoint buffer
            bCount =
                (MscReadControl.dwBytesToSendLeft >
                 EP_MAX_PACKET_SIZE) ? EP_MAX_PACKET_SIZE : MscReadControl.
                dwBytesToSendLeft;
            //copy data into IEPx X or Y buffer
            USB_TX_memcpy(pEP2, MscReadControl.pUserBuffer, bCount);
            //Set counter for usb In-Transaction
            *pCT2 = bCount;
            //switch buffer
            MscReadControl.bCurrentBufferXY =
                (MscReadControl.bCurrentBufferXY + 1) & 0x01;
            MscReadControl.dwBytesToSendLeft -= bCount;
            //move buffer pointer
            MscReadControl.pUserBuffer += bCount;
        }
    } //if(*pCT1 & EPBCNT_NAK)
    return (bWakeUp);
}

//------------------------------------------------------------------------------------------------------

//This function used to initialize the sending process.
//Use this by functiosn for send CSW or send LBA
//To use only by STACK itself, not by application
//Returns: SUCCESS or FAILURE
uint8_t MscSendData (const uint8_t* data, uint16_t size)
{
    uint8_t edbIndex;
    uint16_t state;

    edbIndex = stUsbHandle[MSC0_INTFNUM].edb_Index;

    if (size == 0){
        return (FAILURE);
    }

    state = usbDisableInEndpointInterrupt(edbIndex);
    //atomic operation - disable interrupts

    //do not access USB memory if suspended (PLL off). It may produce BUS_ERROR
    if ((bFunctionSuspended) ||
        (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //data can not be read because of USB suspended
    	usbRestoreInEndpointInterrupt(state);
        return (FAILURE);
    }

    if ((MscReadControl.dwBytesToSendLeft != 0) ||  //data was not sent out
        (MscReadControl.bReadProcessing == TRUE)){  //still processing previous data
        //the USB still sends previous data, we have to wait
    	usbRestoreInEndpointInterrupt(state);
        return (FAILURE);
    }

    //This function generate the USB interrupt. The data will be sent out from interrupt

    MscReadControl.bReadProcessing = TRUE;          //set reading busy status.
    MscReadControl.dwBytesToSendLeft = size;
    MscReadControl.pUserBuffer = (uint8_t*)data;

    //trigger Endpoint Interrupt - to start send operation
    USBIEPIFG |= 1 << (edbIndex + 1);               //IEPIFGx;

    usbRestoreInEndpointInterrupt(state);

    return (SUCCESS);
}

//This function copies data from OUT endpoint into user's buffer
//This function to call only from MSCFromHostToBuffer()
//Arguments:
//pEP - pointer to EP to copy from
//pCT - pointer to EP control reg
//
void MscCopyUsbToBuff (uint8_t* pEP, uint8_t* pCT)
{
    uint8_t nCount;

    nCount = *pCT & (~EPBCNT_NAK);

    //how many bytes we can receive to avoid overflow
    nCount =
        (nCount >
         MscWriteControl.dwBytesToReceiveLeft) ? MscWriteControl.
        dwBytesToReceiveLeft :
        nCount;

    USB_RX_memcpy(MscWriteControl.pUserBuffer, pEP, nCount);    //copy data from OEPx X or Y buffer
    MscWriteControl.dwBytesToReceiveLeft -= nCount;
    MscWriteControl.pUserBuffer += nCount;                      //move buffer pointer
                                                                //to read rest of data next time from this place
    MscWriteControl.wFreeBytesLeft -= nCount;                   //update counter

    MscWriteControl.wCurrentByte += nCount;
    if (MscWriteControl.wCurrentByte >= MscControl[sRwbuf.lun].lbaSize){
        MscWriteControl.wCurrentByte = 0;
        MscWriteControl.lbaCount++;
    }

    //switch current buffer
    MscWriteControl.bCurrentBufferXY =
        (MscWriteControl.bCurrentBufferXY + 1) & 0x01;

    //clear NAK, EP ready to receive data
    *pCT = 0x00;
}

//------------------------------------------------------------------------------------------------------
/* This function is called only from ISR(only on Output endpoint interrupt, to recv data from host)
 * This function actually recieves the data from host Over USB */
int16_t MSCFromHostToBuffer ()
{
    uint8_t * pEP1;
    uint8_t nTmp1;
    uint8_t bWakeUp = FALSE;                                   //per default we do not wake up after interrupt
    uint8_t edbIndex;

    edbIndex = stUsbHandle[MSC0_INTFNUM].edb_Index;

	if (MscState.stallEndpoint == TRUE) {
    	tOutputEndPointDescriptorBlock[edbIndex].bEPCNF |= EPCNF_STALL;
    	return TRUE;
    }
	
    if (MscState.bMscCbwReceived == TRUE){
        //previous CBW is not performed, so exit interrupt hendler
        //and trigger it again later
        return (TRUE);                                      //true for wake up!
    }

    if (!MscWriteControl.bWriteProcessing){                 //receiving CBW
        //CBW will be received here....
        //check what is current buffer: X or Y
        if (MscWriteControl.bCurrentBufferXY == X_BUFFER){  //X is current buffer
            //this is the active EP buffer
            pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_X_Buffer;
            MscWriteControl.pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
            MscWriteControl.pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
        } else {
            //this is the active EP buffer
            pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_Y_Buffer;
            MscWriteControl.pCT1 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
            MscWriteControl.pCT2 =
                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
        }

        //how many byte we can get from one endpoint buffer
        nTmp1 = *MscWriteControl.pCT1;

        if (nTmp1 & EPBCNT_NAK){
            uint8_t nCount;

            //switch current buffer
            MscWriteControl.bCurrentBufferXY =
                (MscWriteControl.bCurrentBufferXY + 1) & 0x01;

            nTmp1 = nTmp1 & 0x7f;                           //clear NAK bit
            nCount = (nTmp1 > sizeof(McsCbw)) ? sizeof(McsCbw) : nTmp1;
            USB_RX_memcpy(&McsCbw, pEP1, nCount);           //copy data from OEPx X or Y buffer

            //clear NAK, EP ready to receive data
            *MscWriteControl.pCT1 = 0x00;

            //set flag and check the CBW from the usbmsc_poll
            MscState.bMscCbwReceived = TRUE;

            //second 64b buffer will be not read out here because the CBW is <64 bytes
        }

        bWakeUp = TRUE;                                     //wake up to perform CBW
        return (bWakeUp);
    }

    //if we are here - LBAs will be received
	if (MscControl[sRwbuf.lun].yBufferAddr == NULL) {

	    /*Check if there are any pending LBAs to process */
	    if (MscWriteControl.dwBytesToReceiveLeft > 0){
	        //read one chunk of 64 bytes

	        //check what is current buffer: X or Y
	        if (MscWriteControl.bCurrentBufferXY == X_BUFFER){  //X is current buffer
	            //this is the active EP buffer
	            pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_X_Buffer;
	            MscWriteControl.pCT1 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;

	            //second EP buffer
	            MscWriteControl.pEP2 =
	                (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_Y_Buffer;
	            MscWriteControl.pCT2 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
	        } else {
	            //this is the active EP buffer
	            pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_Y_Buffer;
	            MscWriteControl.pCT1 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;

	            //second EP buffer
	            MscWriteControl.pEP2 =
	                (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_X_Buffer;
	            MscWriteControl.pCT2 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
	        }

	        //how many byte we can get from one endpoint buffer
	        nTmp1 = *MscWriteControl.pCT1;

	        if ((nTmp1 & EPBCNT_NAK) &&
	            (MscWriteControl.wFreeBytesLeft >= 64)){
	            //copy data from Endpoint
	            MscCopyUsbToBuff(pEP1, MscWriteControl.pCT1);

	            nTmp1 = *MscWriteControl.pCT2;

	            //try read data from second buffer
	            if ((MscWriteControl.dwBytesToReceiveLeft > 0) &&   //do we have more data to send?
	                (MscWriteControl.wFreeBytesLeft >= 64) &&
	                (nTmp1 & EPBCNT_NAK)){                          //if the second buffer has received data?
	                //copy data from Endpoint
	                MscCopyUsbToBuff(MscWriteControl.pEP2, MscWriteControl.pCT2);
	                //MscWriteControl.pCT1 = MscWriteControl.pCT2;
	            }

	            if ((MscWriteControl.wFreeBytesLeft == 0) ||        //user's buffer is full, give it to User
	                (MscWriteControl.dwBytesToReceiveLeft == 0)){   //or no bytes to read left - give it to User
	                sRwbuf.operation = USBMSC_WRITE;
	                sRwbuf.lba = MscWriteControl.lba;               //copy lba number
	                MscWriteControl.lba += MscWriteControl.lbaCount;
	                sRwbuf.lbCount = MscWriteControl.lbaCount;      //copy lba count
	                MscWriteControl.wCurrentByte = 0;
	                MscWriteControl.lbaCount = 0;

		            //call event handler, we are ready with data
		            bWakeUp = USBMSC_handleBufferEvent();
	            } //if (wFreeBytesLeft == 0)
	        }
	    } //if (MscWriteControl.dwBytesToReceiveLeft > 0)
	    else {
	        //perform error handling here, if required.
	        bWakeUp = TRUE;
	    }
	}
	else {
	    //if we are here - LBAs will be received
	    if (sRwbuf.bufferProcessed == 1) {
	    	if (sRwbuf.XorY == 0) {
	    		sRwbuf.xBufFull = 0;
	    		if (sRwbuf.yBufFull) {
	    			sRwbuf.bufferProcessed = 0;
	    			sRwbuf.XorY = 1;
	    			sRwbuf.bufferAddr = MscControl[McsCbw.bCBWLUN].yBufferAddr;
	            	sRwbuf.operation = USBMSC_WRITE;
	            	sRwbuf.lba = sRwbuf.ylba;               //copy lba number
	            	sRwbuf.lbCount = sRwbuf.ylbaCount;      //copy lba count

	            	//call event handler, we are ready with data
	            	bWakeUp = USBMSC_handleBufferEvent();
	    		}
	    	}
	    	else {
	    		sRwbuf.yBufFull = 0;
	    		if (sRwbuf.xBufFull) {
	    			sRwbuf.bufferProcessed = 0;
	    			sRwbuf.XorY = 0;
	    			sRwbuf.bufferAddr = MscControl[McsCbw.bCBWLUN].xBufferAddr;
	            	sRwbuf.operation = USBMSC_WRITE;
	            	sRwbuf.lba = sRwbuf.xlba;               //copy lba number
	            	sRwbuf.lbCount = sRwbuf.xlbaCount;      //copy lba count

	            	//call event handler, we are ready with data
	            	bWakeUp = USBMSC_handleBufferEvent();
	    		}
	    	}
	    }

	    /*Check if there are any pending LBAs to process */
	    if (MscWriteControl.dwBytesToReceiveLeft > 0){
	        //read one chunk of 64 bytes

	    	if (MscWriteControl.wFreeBytesLeft == 0) {
				if (MscWriteControl.XorY == 0) {
					if (sRwbuf.yBufFull == 0) {
						MscWriteControl.lba += MscWriteControl.lbaCount;
						MscWriteControl.wCurrentByte = 0;
						MscWriteControl.lbaCount = 0;
						MscWriteControl.pUserBuffer = MscControl[sRwbuf.lun].yBufferAddr;
						MscWriteControl.XorY = 1;
						MscWriteControl.wFreeBytesLeft =
							MscControl[sRwbuf.lun].wMscUserBufferSize;
					}
				}
				else {
					if (sRwbuf.xBufFull == 0) {
						MscWriteControl.lba += MscWriteControl.lbaCount;
						MscWriteControl.wCurrentByte = 0;
						MscWriteControl.lbaCount = 0;
						MscWriteControl.pUserBuffer = MscControl[sRwbuf.lun].xBufferAddr;
						MscWriteControl.XorY = 0;
						MscWriteControl.wFreeBytesLeft =
							MscControl[sRwbuf.lun].wMscUserBufferSize;
					}
				}
	    	}


	        //check what is current buffer: X or Y
	        if (MscWriteControl.bCurrentBufferXY == X_BUFFER){  //X is current buffer
	            //this is the active EP buffer
	            pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_X_Buffer;
	            MscWriteControl.pCT1 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;

	            //second EP buffer
	            MscWriteControl.pEP2 =
	                (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_Y_Buffer;
	            MscWriteControl.pCT2 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;
	        } else {
	            //this is the active EP buffer
	            pEP1 = (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_Y_Buffer;
	            MscWriteControl.pCT1 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTY;

	            //second EP buffer
	            MscWriteControl.pEP2 =
	                (uint8_t*)stUsbHandle[MSC0_INTFNUM].oep_X_Buffer;
	            MscWriteControl.pCT2 =
	                &tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX;
	        }

	        //how many byte we can get from one endpoint buffer
	        nTmp1 = *MscWriteControl.pCT1;

	        if ((nTmp1 & EPBCNT_NAK) &&
	            (MscWriteControl.wFreeBytesLeft >= 64)){
	            //copy data from Endpoint
	            MscCopyUsbToBuff(pEP1, MscWriteControl.pCT1);

	            nTmp1 = *MscWriteControl.pCT2;

	            //try read data from second buffer
	            if ((MscWriteControl.dwBytesToReceiveLeft > 0) &&   //do we have more data to send?
	                (MscWriteControl.wFreeBytesLeft >= 64) &&
	                (nTmp1 & EPBCNT_NAK)){                          //if the second buffer has received data?
	                //copy data from Endpoint
	                MscCopyUsbToBuff(MscWriteControl.pEP2, MscWriteControl.pCT2);
	                //MscWriteControl.pCT1 = MscWriteControl.pCT2;
	            }

	            if ((MscWriteControl.wFreeBytesLeft == 0) ||        //user's buffer is full, give it to User
	                (MscWriteControl.dwBytesToReceiveLeft == 0)){   //or no bytes to read left - give it to User

	                if (sRwbuf.firstFlag == 0) {
	                	sRwbuf.firstFlag = 1;
	                	sRwbuf.operation = USBMSC_WRITE;
	                	sRwbuf.lba = MscWriteControl.lba;               //copy lba number
	                	sRwbuf.lbCount = MscWriteControl.lbaCount;      //copy lba count

	                	//call event handler, we are ready with data
	                	bWakeUp = USBMSC_handleBufferEvent();
	                }
	            	if (MscWriteControl.XorY == 0) {
	                	sRwbuf.xBufFull = 1;
	                	sRwbuf.xlba = MscWriteControl.lba;
	                	sRwbuf.xlbaCount = MscWriteControl.lbaCount;
	                }
	                else {
	                	sRwbuf.yBufFull = 1;
	                	sRwbuf.ylba = MscWriteControl.lba;
	                	sRwbuf.ylbaCount = MscWriteControl.lbaCount;
	                }
	            	return (TRUE);
	            } //if (wFreeBytesLeft == 0)
	        }
	    } //if (MscWriteControl.dwBytesToReceiveLeft > 0)
	    else {
	    	if (sRwbuf.xBufFull ==0 && sRwbuf.yBufFull == 0) {
	    		MscWriteControl.pUserBuffer = NULL;         //no more receiving pending
	    		MscWriteControl.bWriteProcessing = FALSE;   //ready to receive next CBW
	    	}
	        bWakeUp = TRUE;
	    }
	}
    return (bWakeUp);
}

//
//! \endcond
//

//*****************************************************************************
//
//! This function should be called by the application after it has processed a buffer request.
//!
//! \param USBMSC_Rwbuf_Info*RWBufInfo Pass the value received from USBMSC_fetchInformationStructure().
//!
//! This function should be called by the application after it has processed a buffer request. It
//! indicates to the API that the application has fulfilled the request.
//! Prior to calling this function, the application needs to write a return code to rwInfo.returnCode.
//! This code should reflect the result of the operation. The value may come from the file system
//! software, depending on the application. See Sec. 8.3.6 of
//! \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for a list of valid return codes.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USBMSC_processBuffer ()
{
	uint16_t stateIn, stateOut;
    uint8_t edbIndex;

	edbIndex = stUsbHandle[MSC0_INTFNUM].edb_Index;
    stateIn = usbDisableInEndpointInterrupt(edbIndex);
    stateOut = usbDisableOutEndpointInterrupt(edbIndex);

	if (MscControl[sRwbuf.lun].yBufferAddr == NULL) {
	    /*
	     * Fix for SDOCM00078384
	     * Reset bWriteProcessing after last buffer is processed by the application
	     */
	    if (sRwbuf.operation == USBMSC_WRITE &&
	        MscWriteControl.dwBytesToReceiveLeft == 0){ //the Receive opereation (MSC_WRITE) is completed
	        MscWriteControl.pUserBuffer = NULL;         //no more receiving pending
	        MscWriteControl.bWriteProcessing = FALSE;   //ready to receive next CBW
	    }

	    if (sRwbuf.operation == USBMSC_WRITE && sRwbuf.returnCode ==
	        USBMSC_RW_SUCCESS){
	        //initialize user buffer.
	        MscWriteControl.pUserBuffer = MscControl[sRwbuf.lun].xBufferAddr;
	        MscWriteControl.wFreeBytesLeft =
	            MscControl[sRwbuf.lun].wMscUserBufferSize;
	    	sRwbuf.operation = NULL;                    //no operation pending...
	    	//read out next portion of data if available.
	    	MSCFromHostToBuffer();
	    } else if (sRwbuf.operation == USBMSC_READ && sRwbuf.returnCode ==
	               USBMSC_RW_SUCCESS){
	        uint16_t wCnt = sRwbuf.lbCount * MscControl[sRwbuf.lun].lbaSize;

	        //trigger sending LBA(s)
	        MscSendData(sRwbuf.bufferAddr, wCnt);

	        if (sRwbuf.lbCount >= MscReadControl.lbaCount){
	            //all bytes sent, reset structure
	            MscReadControl.lbaCount = 0;
	        } else {
	            //update read structure
	            MscReadControl.lbaCount -= sRwbuf.lbCount;
	            MscReadControl.lba += sRwbuf.lbCount;

	        }
	    	sRwbuf.operation = NULL;                                        //no operation pending...
	    }
	}
	else {
	    if (sRwbuf.operation == USBMSC_WRITE && sRwbuf.returnCode ==
	        USBMSC_RW_SUCCESS){
	        //initialize user buffer.
	    	sRwbuf.bufferProcessed = 1;
	    	if (sRwbuf.XorY == 0) {
	    		sRwbuf.xBufFull = 0;
	    	}
	    	else {
	    		sRwbuf.yBufFull = 0;
	    	}
	        sRwbuf.operation = NULL;                    //no operation pending...
	        //read out next portion of data if available.
	        MSCFromHostToBuffer();
	    } else if (sRwbuf.operation == USBMSC_READ && sRwbuf.returnCode ==
	               USBMSC_RW_SUCCESS){
	        uint16_t wCnt = sRwbuf.lbCount * MscControl[sRwbuf.lun].lbaSize;

	        sRwbuf.bufferProcessed = 1;

	        if (sRwbuf.XorY == 0) {
	        	sRwbuf.xBufFull = 1;
	        	sRwbuf.xWordCnt = wCnt;
	        }
	        else {
	        	sRwbuf.yBufFull = 1;
	        	sRwbuf.yWordCnt = wCnt;
	        }

	        if (sRwbuf.firstFlag == 0) {
	        	//trigger sending LBA(s)
	        	sRwbuf.firstFlag = 1;
	        	MscSendData(sRwbuf.bufferAddr, wCnt);
	        }
	        else {
	            edbIndex = stUsbHandle[MSC0_INTFNUM].edb_Index;
	            //trigger Endpoint Interrupt - to start send operation
	            USBIEPIFG |= 1 << (edbIndex + 1);               //IEPIFGx;
	        }

	        if (sRwbuf.lbCount >= MscReadControl.lbaCount){
	            //all bytes sent, reset structure
	            MscReadControl.lbaCount = 0;
	            sRwbuf.operation = NULL;
	        } else {
	            //update read structure
	            MscReadControl.lbaCount -= sRwbuf.lbCount;
	            MscReadControl.lba += sRwbuf.lbCount;
	        }
	        sRwbuf.operation = NULL;                                        //no operation pending...
	    }
	}

    switch (sRwbuf.returnCode)
    {
        case USBMSC_RW_SUCCESS:
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            break;
        //Set RequestSenseResponse if necessary?  Maybe initialized values OK?

        case USBMSC_RW_NOT_READY:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 1;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_NOT_READY;
            RequestSenseResponse.ASC = ASC_NOT_READY;
            RequestSenseResponse.ASCQ = ASCQ_NOT_READY;
            break;

        case USBMSC_RW_ILLEGAL_REQUEST:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_ILLEGAL_REQUEST;
            RequestSenseResponse.ASC = ASC_ILLEGAL_REQUEST;
            RequestSenseResponse.ASCQ = ASCQ_ILLEGAL_REQUEST;
            break;

        case USBMSC_RW_UNIT_ATTENTION:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_UNITATTN;
            RequestSenseResponse.ASC = ASC_UNITATTN_READY_NOTREADY;
            RequestSenseResponse.ASCQ = ASCQ_UNITATTN_READY_NOTREADY;
            break;

        case USBMSC_RW_LBA_OUT_OF_RANGE:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_ILLEGAL_REQUEST;
            RequestSenseResponse.ASC = ASC_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
            RequestSenseResponse.ASCQ = ASCQ_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
            break;

        case USBMSC_RW_MEDIA_NOT_PRESENT:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_NOT_READY;
            RequestSenseResponse.ASC = ASC_MEDIUM_NOT_PRESENT;
            RequestSenseResponse.ASCQ = ASCQ_MEDIUM_NOT_PRESENT;
            break;

        case USBMSC_RW_DEVICE_WRITE_FAULT:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_MEDIUM_ERROR;
            RequestSenseResponse.ASC = ASC_WRITE_FAULT;
            RequestSenseResponse.ASCQ = ASCQ_WRITE_FAULT;
            break;

        case USBMSC_RW_UNRECOVERED_READ:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey = S_MEDIUM_ERROR;
            RequestSenseResponse.ASC = ASC_UNRECOVERED_READ;
            RequestSenseResponse.ASCQ = ASCQ_UNRECOVERED_READ;
            break;

        case USBMSC_RW_WRITE_PROTECTED:
            MscState.Scsi_Status = SCSI_FAILED;
            MscState.Scsi_Residue = 0;
            Reset_RequestSenseResponse();
            RequestSenseResponse.VALID = 1;
            RequestSenseResponse.SenseKey =  S_WRITE_PROTECTED;
            RequestSenseResponse.ASC = ASC_WRITE_PROTECTED;
            RequestSenseResponse.ASCQ = ASCQ_WRITE_PROTECTED;
            break;
                                                        //case breakouts for all the codes
    }

    if (sRwbuf.returnCode != USBMSC_RW_SUCCESS){
        sRwbuf.operation = NULL;                        //no operation pending...
        if (McsCbw.bmCBWFlags == DIRECTION_IN){
            usbStallInEndpoint(MSC0_INTFNUM);
            MscReadControl.bReadProcessing = FALSE;     //ready to receive next CBW
            MscReadControl.pUserBuffer = NULL;          //no more receiving pending
            MscReadControl.lbaCount = 0;
        } else {
            //we need to stall only if not all af data was transfered
            if (MscWriteControl.dwBytesToReceiveLeft > 0){
                usbStallOutEndpoint(MSC0_INTFNUM);
            }
            MscWriteControl.bWriteProcessing = FALSE;   //ready to receive next CBW
            MscWriteControl.pUserBuffer = NULL;         //no more receiving pending
            *MscWriteControl.pCT1 = 0x00;               //clear NAK, EP ready to receive next data
            *MscWriteControl.pCT2 = 0x00;               //clear NAK, EP ready to receive next data
        }
    }

    usbRestoreInEndpointInterrupt(stateIn);
    usbRestoreOutEndpointInterrupt(stateOut);
    return (USB_SUCCEED);
}

//
//! \cond
//

//-------------------------------------------------------------------------------------------
void Msc_ResetFlags ()
{
    MscState.bMscCbwReceived = FALSE;
}

//-------------------------------------------------------------------------------------------
void Msc_ResetStruct ()
{
    memset(&sRwbuf,0,sizeof(USBMSC_RWbuf_Info));
    memset(&McsCsw,0,sizeof(CSW));

    MscReadControl.pUserBuffer = NULL;
    MscReadControl.dwBytesToSendLeft = 0;
    MscReadControl.bReadProcessing = FALSE;

    MscWriteControl.bWriteProcessing = FALSE;
    MscWriteControl.pUserBuffer = NULL;
    MscWriteControl.dwBytesToReceiveLeft = 0;   //holds how many bytes is still requested by WRITE operation (Host to MSP430)
    //we do not reset the bCurrentBufferXY, becuase the buffer doesnt changed if he MSC reseted.
    //The bCurrentBufferXY should be reseted in USB_Reset()

    Reset_RequestSenseResponse();
}

//-------------------------------------------------------------------------------------------
void MscResetData ()
{
    Msc_ResetStruct();

    memset(&MscWriteControl, 0, sizeof(MscWriteControl));
    memset(&MscReadControl, 0, sizeof(MscReadControl));
}

//-------------------------------------------------------------------------------------------
void MscResetCtrlLun ()
{
    int16_t i;

    for (i = 0; i < MSC_MAX_LUN_NUMBER; i++)
    {
        MscControl[i].bMediaPresent = 0x80;
        MscControl[i].bWriteProtected = FALSE;
    }
}

//-------------------------------------------------------------------------------------------
/* This function can be called by application to get the current status of stack operation */
uint8_t USBMSC_getState ()
{
    uint8_t state;

    if (sRwbuf.operation == 0 && MscState.bMscSendCsw == FALSE){
        state = USBMSC_IDLE;
    } else if (sRwbuf.operation == USBMSC_READ && sRwbuf.lbCount > 0){
        state =  USBMSC_READ_IN_PROGRESS;
    } else if (sRwbuf.operation == USBMSC_WRITE && sRwbuf.lbCount > 0){
        state =  USBMSC_WRITE_IN_PROGRESS;
    } else if (sRwbuf.operation == 0 && MscState.bMscSendCsw == TRUE){
        state =  USBMSC_COMMAND_BEING_PROCESSED;
    }
    return (state);
}

//
//! \endcond
//

//*****************************************************************************
//
//! Informs the API of the Current State of the Media on LUN \b lun.
//!
//! \param lun is the logical unit (LUN) on which the operation is taking place. Zero-based. (This version of the API
//! 	only supports a single LUN.)
//! \param info is a structure that communicates the most recent information about the medium.
//!
//! Informs the API of the current state of the media on LUN \b lun. It does this using an instance \b info
//! of the API-defined structure USBMSC_mediaInfoStr. The API uses the information in the most
//! recent call to this function in automatically handling certain requests from the host.
//! In LUNs that are marked as not removable in USBMSC_CONFIG, this function should be called
//! once at the beginning of execution, prior to attachment to the USB host. It then no longer needs
//! to be called.
//! 
//! In LUNS that are marked as removable, the media information is dynamic. The function should
//! still be called at the beginning of execution to indicate the initial state of the media, and then it
//! should also be called every time the media changes.
//! 
//! See Sec. 8.3.4 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for more about informing
//! the API of media changes.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USBMSC_updateMediaInformation ( uint8_t lun,  struct USBMSC_mediaInfoStr *info)
{
    uint8_t state;

    Scsi_Read_Capacity_10[lun].lLba[0] = (uint8_t)(info->lastBlockLba >> 24);
    Scsi_Read_Capacity_10[lun].lLba[1] = (uint8_t)(info->lastBlockLba >> 16);
    Scsi_Read_Capacity_10[lun].lLba[2] = (uint8_t)(info->lastBlockLba >> 8);
    Scsi_Read_Capacity_10[lun].lLba[3] = (uint8_t)(info->lastBlockLba);

    Scsi_Read_Capacity_10[lun].bLength[0] = (uint8_t)(info->bytesPerBlock >> 24);
    Scsi_Read_Capacity_10[lun].bLength[1] = (uint8_t)(info->bytesPerBlock >> 16);
    Scsi_Read_Capacity_10[lun].bLength[2] = (uint8_t)(info->bytesPerBlock >> 8);
    Scsi_Read_Capacity_10[lun].bLength[3] = (uint8_t)(info->bytesPerBlock);

    MscControl[lun].lbaSize = (uint16_t)Scsi_Read_Capacity_10[lun].bLength[2] <<
                              8 | Scsi_Read_Capacity_10[lun].bLength[3];

    //If the LUN was reported as not removable, then leave mediaPresent/mediaChanged as
    //their initialized defaults.
    if (USBMSC_config.LUN[lun].removable){
        if (((MscControl[lun].bMediaPresent == USBMSC_MEDIA_NOT_PRESENT)) &&
            (info->mediaPresent == USBMSC_MEDIA_PRESENT)){             //If media was inserted...
            //Set Unit Attention flag. This flag is used in Scsi_Request_Sense().
            MscState.bUnitAttention = TRUE;
            MscState.Scsi_Status = SCSI_FAILED;
        }

        if ((MscControl[lun].bMediaPresent == USBMSC_MEDIA_PRESENT &&
             ((info->mediaPresent == USBMSC_MEDIA_NOT_PRESENT))) ||    //If media was removed...
            ((info->mediaPresent == USBMSC_MEDIA_PRESENT) &&
             (info->mediaChanged))){                                    //Or if media still present, but has changed...
            //Set Unit Attention flag. This flag is used in Scsi_Request_Sense().
            MscState.bUnitAttention = TRUE;
            MscState.Scsi_Status = SCSI_FAILED;
            state = USBMSC_getState();

            if (state ==  USBMSC_READ_IN_PROGRESS || state ==
                USBMSC_WRITE_IN_PROGRESS){
                if (McsCbw.bmCBWFlags == DIRECTION_IN){
                    usbStallInEndpoint(MSC0_INTFNUM);
                } else {
                    usbStallOutEndpoint(MSC0_INTFNUM);
                }

                Msc_ResetStateMachine();
                Msc_ResetFlags();
                Msc_ResetStruct();
                MscState.isMSCConfigured = TRUE;

                Scsi_Send_CSW(MSC0_INTFNUM);
            }
        }
        MscControl[lun].bMediaPresent = info->mediaPresent;
    }

    MscControl[lun].bWriteProtected = info->writeProtected;
    return (USB_SUCCEED);
}

//*****************************************************************************
//
//! Gives the API a Buffer to Use for READ/WRITE Data Transfer.
//!
//! \param lun is the Lun number.
//! \param *RWbuf_x is the address of an X-buffer. If null, then both buffers are de-activated.
//! \param *RWbuf_y is the address of an Y-buffer. (Double-buffering is not supported in this version of the API.)
//! \param size is the size, in bytes, of the buffers.
//!
//! Gives the API a buffer to use for READ/WRITE data transfer. \b size indicates the size of the
//! buffer, in bytes.
//! 
//! \b NOTE: Currently, only single-buffering is supported, so \b RWbuf_y should be set to null.
//! If the application intends to allocate the buffer statically, then this function needs only to be
//! called once, prior to any READ/WRITE commands being received from the host. Most likely this
//! would happen during the application's initialization functions.
//! 
//! \b NOTE: This API has to be called after the call to USBMSC_updateMediaInformation() at the beginning
//! of execution.
//! 
//! However, this function optionally enables dynamic buffer management. That is, it can activate
//! and de-activate the buffer, by alternately assigning a null and valid address in \b RWbuf_x. This is
//! useful because the buffer uses a significant portion of the RAM resources (typically 512 bytes).
//! This memory is not needed when USB is not attached or suspended.
//! 
//! If doing this, it's important that the application re-activate the buffer when USB becomes active
//! again, by issuing another call to the function, this time using valid buffer information. If the API
//! needs the buffer and doesn't have it, it will begin failing READ/WRITE commands from the host.
//! The re-activation can take place within USB_handleVbusOffEvent().
//! 
//! \b size must be a multiple of a block size - for FAT, a block size is typically 512 bytes. Thus
//! values of 512, 1024, 1536, etc. are valid. Non-multiples are not valid.
//! 
//! The function returns \b USB_SUCCEED every time. It is up to the application to ensure that the
//! buffers are valid.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************

uint8_t USBMSC_registerBufferInformation (uint8_t lun, uint8_t *RWbuf_x, uint8_t *RWbuf_y, uint16_t size)
{
    MscControl[lun].wMscUserBufferSize = 0;
    MscControl[lun].xBufferAddr = NULL;
    MscControl[lun].yBufferAddr = NULL; //this version supports only X buffer.

    //check if arguments are valid
    if ((size < MscControl[lun].lbaSize) ||
        (RWbuf_x == NULL)){             //Need at least one buffer
        return (USB_GENERAL_ERROR);
    }

    MscControl[lun].wMscUserBufferSize = size;
    MscControl[lun].lbaBufCapacity = MscControl[lun].wMscUserBufferSize /
                                     MscControl[lun].lbaSize;
    MscControl[lun].xBufferAddr = RWbuf_x;
    MscControl[lun].yBufferAddr = RWbuf_y;
    return (USB_SUCCEED);
}

//
//! \cond
//

//-------------------------------------------------------------------------------------------
void SET_RequestsenseNotReady ()
{
    //Set REQUEST SENSE with "not ready"
    Reset_RequestSenseResponse();
    RequestSenseResponse.VALID = 1;
    RequestSenseResponse.SenseKey = S_NOT_READY;
    RequestSenseResponse.ASC = ASC_NOT_READY;
    RequestSenseResponse.ASCQ = ASCQ_NOT_READY;
    //Send CSW with error status
    MscState.Scsi_Status = SCSI_FAILED;
}

//-------------------------------------------------------------------------------------------
void SET_RequestsenseMediaNotPresent ()
{
    //Set REQUEST SENSE with "not ready"
    Reset_RequestSenseResponse();
    RequestSenseResponse.VALID = 1;
    RequestSenseResponse.SenseKey = S_NOT_READY;
    RequestSenseResponse.ASC = ASC_MEDIUM_NOT_PRESENT;
    RequestSenseResponse.ASCQ = ASCQ_MEDIUM_NOT_PRESENT;
    //Send CSW with error status
    MscState.Scsi_Status = SCSI_FAILED;
}

//-------------------------------------------------------------------------------------------
void usbClearOEPByteCount (uint8_t intfNum)
{
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;
    tOutputEndPointDescriptorBlock[edbIndex].bEPBCTX = 0;
}

//-------------------------------------------------------------------------------------------
void usbStallEndpoint (uint8_t intfNum)
{
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;
    tOutputEndPointDescriptorBlock[edbIndex].bEPCNF |= EPCNF_STALL;
    tInputEndPointDescriptorBlock[edbIndex].bEPCNF |= EPCNF_STALL;
}

//-------------------------------------------------------------------------------------------
void usbStallInEndpoint (uint8_t intfNum)
{
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;
    tInputEndPointDescriptorBlock[edbIndex].bEPCNF |= EPCNF_STALL;
}

//-------------------------------------------------------------------------------------------
void usbStallOutEndpoint (uint8_t intfNum)
{
    uint8_t edbIndex;

    edbIndex = stUsbHandle[intfNum].edb_Index;
    tOutputEndPointDescriptorBlock[edbIndex].bEPCNF |= EPCNF_STALL;
	MscState.stallEndpoint = TRUE;
}

//
//! \endcond
//

//*****************************************************************************
//
//! Returns a pointer to the \b USBMSC_Rwbuf_Info structure instance maintained within the API.
//!
//! Returns a pointer to the \b USBMSC_Rwbuf_Info structure instance maintained within the API.
//! See Sec. 8.3.6 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for information on using
//! this structure.
//! This function should be called prior to USB enumeration; that is, prior to calling USB_connect().
//!
//! \return A pointer to an application-allocated instance of \b USBMSC_RWBuf_Info,
//! which will be used to exchange information related to buffer requests from
//! the API to the application.
//
//*****************************************************************************

USBMSC_RWbuf_Info* USBMSC_fetchInformationStructure (void)
{
    return (&sRwbuf);
}

//
//! \cond
//

#ifdef CDROM_SUPPORT
//----------------------------------------------------------------------------

void Scsi_Read_TocPmaAtip(uint8_t intfNum)
{
    if(McsCbw.CBWCB[2] & 0x01) {

	if (SUCCESS !=
			MscSendData( (uint8_t*)&Scsi_Read_TOC_PMA_ATIP_F1[McsCbw.bCBWLUN],
					Scsi_Read_TOC_PMA_ATIP_F1_LEN)){
			MscState.Scsi_Status = SCSI_FAILED;
		}
    } else {

		if (SUCCESS !=
			MscSendData( (uint8_t*)&Scsi_Read_TOC_PMA_ATIP_F2[McsCbw.bCBWLUN],
					Scsi_Read_TOC_PMA_ATIP_F2_LEN)){
			MscState.Scsi_Status = SCSI_FAILED;
		}
    }
}

void Scsi_Get_Configuration(uint8_t intfNum) {

		if (FAILURE == Check_CBW(intfNum,DIRECTION_IN,SCSI_GET_CONFIGURATION_LEN)){
		  return;
		}

        if (SUCCESS !=
            MscSendData( (uint8_t*)&Scsi_Get_Configuration_Descriptor[McsCbw.bCBWLUN],
                    SCSI_GET_CONFIGURATION_LEN)){
            MscState.Scsi_Status = SCSI_FAILED;
        }
}

void Scsi_Event_Status_Notification(uint8_t intfNum) {

       if (FAILURE == Check_CBW(intfNum,DIRECTION_IN,SCSI_EVENT_STATUS_LEN)){
           return;
        }
        if (SUCCESS !=
            MscSendData( (uint8_t*)&Scsi_Event_Status_Descriptor[McsCbw.bCBWLUN],
                    SCSI_EVENT_STATUS_LEN)){
            MscState.Scsi_Status = SCSI_FAILED;
        }
}

void Scsi_Read_Disc_Information(uint8_t intfNum) {

                    
	if (FAILURE == Check_CBW(intfNum,DIRECTION_IN,SCSI_READ_DISC_INFORMATION_LEN)){
			return;
		 }
        if (SUCCESS !=
            MscSendData( (uint8_t*)&Scsi_Disc_Information_Descriptor[McsCbw.bCBWLUN],
                    SCSI_READ_DISC_INFORMATION_LEN)){
            MscState.Scsi_Status = SCSI_FAILED;
        }
}

#endif

#endif  //_MSC_

//
//! \endcond
//

/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
