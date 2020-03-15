using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public enum OperationStatus : byte
    {
        CommandWasPerformed = 0x00,
        CommandWasNotPerformed = 0x01,
        ItemDoesNotExist = 0x02,
        InvalidMessageId = 0x03,
        InvalidMac = 0x04,
        OutOfMemory = 0x05,
        CouldNotDecryptTheMessage = 0x06,
        InvalidMessageNumber = 0x07,
        InvalidKeyId = 0x08,
        InvalidAlgorithmId = 0x09,
        InvalidMfid = 0x0A,
        ModuleFailure = 0x0B,
        MiAllZeros = 0x0C,
        Keyfail = 0x0D,
        Unknown = 0xFF
    }

    public static class OperationStatusExtensions
    {
        public static string ToStatusString(OperationStatus status)
        {
            switch (status)
            {
                case OperationStatus.CommandWasPerformed:
                    return "Command was performed";
                case OperationStatus.CommandWasNotPerformed:
                    return "Command not performed";
                case OperationStatus.ItemDoesNotExist:
                    return "Item does not exist";
                case OperationStatus.InvalidMessageId:
                    return "Invalid Message ID";
                case OperationStatus.InvalidMac:
                    return "Invalid MAC";
                case OperationStatus.OutOfMemory:
                    return "Out of Memory";
                case OperationStatus.CouldNotDecryptTheMessage:
                    return "Could not decrypt the message";
                case OperationStatus.InvalidMessageNumber:
                    return "Invalid Message Number";
                case OperationStatus.InvalidKeyId:
                    return "Invalid Key ID";
                case OperationStatus.InvalidAlgorithmId:
                    return "Invalid Algorithm ID";
                case OperationStatus.InvalidMfid:
                    return "Invalid MFID";
                case OperationStatus.ModuleFailure:
                    return "Module Failure";
                case OperationStatus.MiAllZeros:
                    return "MI all zeros";
                case OperationStatus.Keyfail:
                    return "Keyfail";
                case OperationStatus.Unknown:
                    return "Unknown";
                default:
                    return "Reserved";
            }
        }

        public static string ToReasonString(OperationStatus status)
        {
            switch (status)
            {
                case OperationStatus.CommandWasPerformed:
                    return "Command was executed successfully";
                case OperationStatus.CommandWasNotPerformed:
                    return "Command could not be performed due to an unspecified reason";
                case OperationStatus.ItemDoesNotExist:
                    return "Key / Keyset needed to perform the operation does not exist";
                case OperationStatus.InvalidMessageId:
                    return "Message ID is invalid/unsupported";
                case OperationStatus.InvalidMac:
                    return "MAC is invalid";
                case OperationStatus.OutOfMemory:
                    return "Memory unavailable to process the command / message";
                case OperationStatus.CouldNotDecryptTheMessage:
                    return "KEK does not exist";
                case OperationStatus.InvalidMessageNumber:
                    return "Message Number is invalid";
                case OperationStatus.InvalidKeyId:
                    return "Key ID is invalid or not present";
                case OperationStatus.InvalidAlgorithmId:
                    return "ALGID is invalid or not present";
                case OperationStatus.InvalidMfid:
                    return "MFID is invalid";
                case OperationStatus.ModuleFailure:
                    return "Encryption Hardware failure";
                case OperationStatus.MiAllZeros:
                    return "Received MI was all zeros";
                case OperationStatus.Keyfail:
                    return "Key identified by ALGID/Key ID is erased";
                case OperationStatus.Unknown:
                    return "Unknown";
                default:
                    return "Reserved";
            }
        }
    }
}
