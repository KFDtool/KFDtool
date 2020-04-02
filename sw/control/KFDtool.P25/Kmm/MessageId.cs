using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public enum MessageId : byte
    {
        Null = 0x00,
        //CapabilitiesCommand = 0x01,
        //CapabilitiesResponse = 0x02,
        ChangeRsiCommand = 0x03,
        ChangeRsiResponse = 0x04,
        ChangeoverCommand = 0x05,
        ChangeoverResponse = 0x06,
        //DelayedAcknowledgement = 0x07,
        //DeleteKeyCommand = 0x08,
        //DeleteKeyResponse = 0x09,
        //DeleteKeysetCommand = 0x0A,
        //DeleteKeysetResponse = 0x0B,
        //Hello = 0x0C,
        InventoryCommand = 0x0D,
        InventoryResponse = 0x0E,
        //KeyAssignmentCommand = 0x0F,
        //KeyAssignmentResponse = 0x10,
        //Reserved11 = 0x11,
        //Reserved12 = 0x12,
        ModifyKeyCommand = 0x13,
        //ModifyKeysetAttributesCommand = 0x14,
        //ModifyKeysetAttributesResponse = 0x15,
        NegativeAcknowledgment = 0x16,
        //NoService = 0x17,
        //Reserved18 = 0x18,
        //Reserved19 = 0x19,
        //Reserved1A = 0x1A,
        //Reserved1B = 0x1B,
        //Reserved1C = 0x1C,
        RekeyAcknowledgment = 0x1D,
        //RekeyCommand = 0x1E,
        //SetDateTime = 0x1F,
        //WarmStartCommand = 0x20,
        ZeroizeCommand = 0x21,
        ZeroizeResponse = 0x22,
        //DeregistrationCommand = 0x23,
        //DeregistrationResponse = 0x24,
        //RegistrationCommand = 0x25,
        //RegistrationResponse = 0x26,
        //UnableToDecryptResponse = 0x27,
        //LoadAuthKeyCommand = 0x28,
        //LoadAuthKeyResponse = 0x29,
        //DeleteAuthKeyCommand = 0x2A,
        //DeleteAuthKeyResponse = 0x2B,
        //UnknownMotorolaCommand = 0xA0,
        //UnknownMotorolaResponse = 0xA1,
        LoadConfigResponse = 0xFC,
        LoadConfigCommand = 0xFD
    }
}
