using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.BSL430
{
    public class BSLCoreCommandParser
    {
        // PC --> MSP430 COMMANDS
        // Subset of CMDs in flash
        private const byte RXDATABLOCKFAST = 0x1B;
        private const byte RXPASSWORD = 0x11;
        private const byte LOADPC = 0x17;

        // Full set of RAM CMDs
        private const byte RXDATABLOCK = 0x10;
        private const byte ERASESGMENT = 0x12;
        private const byte UNLOCKANDLOCKINFO = 0x13;
        private const byte RESERVED = 0x14;
        private const byte MASSERASE = 0x15;
        private const byte CRCCHECK = 0x16;
        private const byte TXDATABLOCK = 0x18;
        private const byte TXBSLVERSION = 0x19;

        // Not implemented in MSP430 F5xx/6xx
        private const byte TXBUFFERSIZE = 0x1A;

        // MSP430 --> PC RESPONSES
        private const byte MULTIPLEBYTESRESONSE = 0x3A;
        private const byte SINGLEBYTERESONSE = 0x3B;

        public const byte MSG_NOTAMESSAGE = 0xFF;
        public const byte MSG_SUCCESSFULL = 0x00;
        public const byte MSG_FLASHWRITEFAILED = 0x01;
        public const byte MSG_FLASHFAILBIT = 0x02;
        public const byte MSG_VOLTAGECHANGE = 0x03;
        public const byte MSG_BSLLOCKED = 0x04;
        public const byte MSG_PASSWORDERROR = 0x05;
        public const byte MSG_WRITEFORBIDDEN = 0x06;
        public const byte MSG_UNKNOWNCMD = 0x07;
        public const byte MSG_PACKETTOOLONG = 0x08;


        /// <summary>
        /// RX Data Block
        /// The BSL core writes bytes D1 through Dn starting from the location 
        /// specified in the address fields.
        /// </summary>
        /// <param name="address"></param>
        /// <returns></returns>
        public static byte[] RXDataBlock(int address, byte[] data)
        {
            int length = data.Length + 4;
            byte[] coreCmd = new byte[length];

            byte[] add = BitConverter.GetBytes(address);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(add);

            coreCmd[0] = RXDATABLOCK;
            coreCmd[1] = add[3];
            coreCmd[2] = add[2];
            coreCmd[3] = add[1];

            // Copy data
            int i = 4;
            foreach (byte b in data)
            {
                coreCmd[i] = b;
                i++;
            }

            return coreCmd;
        }

        /// <summary>
        /// RX Data Block Fast
        //  This command is identical to RX Data Block, 
        //  except there is no reply indicating the data was correctly
        //  programmed. It is used primarily to speed up USB programming.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="data"></param>
        /// <returns></returns>
        public static byte[] RXDataBlockFast(int address, byte[] data)
        {
            byte[] cmd = RXDataBlock(address, data);
            cmd[0] = RXDATABLOCKFAST;

            return cmd;
        }

        /// <summary>
        /// RX Password
        /// The BSL core receives the password contained in the packet and unlocks the BSL protected
        /// commands if the password matches the top 16 words in the BSL interrupt vector table(located
        /// between addresses 0xFFE0 and 0xFFFF). When an incorrect password is given, a mass erase is
        /// initiated.This means all code flash is erased, but not Information Memory.
        /// Known bug: The password for the BSL is the bytes between addresses 0xFFF0 and 0xFFFF. 
        /// This means that this BSL version expects only 16 bytes for a password in the RX Password
        /// command.Sending 32 bytes returns an error.
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        public static byte[] RXPassword(byte[] password)
        {
            byte[] coreCmd = new byte[1 + password.Length];

            coreCmd[0] = RXPASSWORD;

            // Copy data
            int i = 1;
            foreach (byte b in password)
            {
                coreCmd[i] = b;
                i++;
            }

            return coreCmd;
        }

        /// <summary>
        /// Erase Segment
        /// The flash segment containing the given address is subjected to an erase.
        /// </summary>
        /// <param name="address"></param>
        /// <returns></returns>
        public static byte[] EraseSegment(int address)
        {
            byte[] coreCmd = new byte[4];

            byte[] add = BitConverter.GetBytes(address);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(add);

            coreCmd[0] = ERASESGMENT;
            coreCmd[1] = add[3];
            coreCmd[2] = add[2];
            coreCmd[3] = add[1];

            return coreCmd;
        }

        /// <summary>
        /// Unlock and Lock Info
        /// This command causes the INFO_A lock to toggle to either protect or lock the INFO_A segment.See
        /// the MSP430x5xx and MSP430x6xx Family User's Guide for more detail on this lock. This command
        /// must be sent before an erase segment command for INFO_A but is not required before a mass erase.
        /// </summary>
        /// <returns></returns>
        public static byte[] UnlockAndLockInfo()
        {
            byte[] coreCmd = new byte[1];

            coreCmd[0] = UNLOCKANDLOCKINFO;

            return coreCmd;
        }

        /// <summary>
        /// Mass Erase
        // All code Flash in the MSP430 is erased.This function does not erase Information Memory.
        /// </summary>
        /// <returns></returns>
        public static byte[] MassErase()
        {
            byte[] coreCmd = new byte[1];

            coreCmd[0] = MASSERASE;

            return coreCmd;
        }

        /// <summary>
        /// The MSP430 performs a 16-bit CRC check using the CCITT standard. The address given is the first
        /// byte of the CRC check.Two bytes are used for the length.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static byte[] CRCCheck(int address, int length)
        {
            byte[] coreCmd = new byte[6];

            byte[] add = BitConverter.GetBytes(address);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(add);

            byte[] len = BitConverter.GetBytes(length);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(len);

            coreCmd[0] = RXDATABLOCK;
            coreCmd[1] = add[3];
            coreCmd[2] = add[2];
            coreCmd[3] = add[1];
            coreCmd[4] = len[3];
            coreCmd[5] = len[2];

            return coreCmd;
        }

        /// <summary>
        /// Load PC
        /// Causes the BSL to begin execution at the given address using a CALLA instruction.As BSL code is
        /// immediately exited with this instruction, no core response can be expected.
        /// </summary>
        /// <param name="address"></param>
        /// <returns></returns>
        public static byte[] LoadPC(int address)
        {
            byte[] cmd = EraseSegment(address);
            cmd[0] = LOADPC;
            return cmd;
        }

        /// <summary>
        /// No Description
        /// </summary>
        /// <param name="address"></param>
        /// <param name="length"></param>
        /// <returns></returns>
        public static byte[] TXDataBlock(int address, int length)
        {
            byte[] cmd = CRCCheck(address, length);
            cmd[0] = TXDATABLOCK;
            return cmd;
        }

        /// <summary>
        /// TX BSL Version
        /// BSL transmits its version information(see Section 3.7.3 for more details).
        /// </summary>
        /// <returns></returns>
        public static byte[] TXBSLVersion()
        {
            byte[] coreCmd = new byte[1];

            coreCmd[0] = TXBSLVERSION;

            return coreCmd;
        }

        /// <summary>
        /// The BSL transmits a value that represents the number of bytes available in its data buffer for sending
        /// or receiving BSL core data packets.
        /// The TX Buffer Size command is currently not implemented in the BSL on MSP430 F5xx/6xx.
        /// </summary>
        /// <returns></returns>
        public static byte[] TXBufferSize()
        {
            byte[] coreCmd = new byte[1];

            coreCmd[0] = TXBUFFERSIZE;

            return coreCmd;
        }

        public static int readMessage(byte[] data)
        {
            if (data[0] != SINGLEBYTERESONSE)
                return MSG_NOTAMESSAGE;

            if (data[1] >= 0x00 && data[1] <= MSG_PACKETTOOLONG)
                return data[1];

            return MSG_NOTAMESSAGE;
        }

        /// <summary>
        /// Returns the list of RXDataBlockFast to be sent to the device.
        /// </summary>
        /// <param name="codeSections">List of sections of the code to be uploaded.</param>
        /// <param name="bufferSize">Nb of bytes for each cmd.</param>
        /// <returns>List of commands to be sent 1-by-1</returns>
        public static List<byte[]> listOfRxDataBlock(List<CodeSection> codeSections, int bufferSize,
                bool fastBlock)
        {
            int address;
            List<byte[]> commands = new List<byte[]>();
            foreach (CodeSection section in codeSections)
            {
                int i, j;

                address = section.StartAddress;
                for (i = 0; i < section.binaryCode.Length; i += bufferSize - 4)
                {
                    // Copy the content
                    byte[] data = new byte[bufferSize - 4];
                    for (j = 0; j < bufferSize - 4 && i + j < section.binaryCode.Length; j++)
                        data[j] = section.binaryCode[i + j];

                    // If last command is not full, fill with 0xFF
                    if (i + j == section.binaryCode.Length)
                        for (; j < bufferSize - 4; j++)
                            data[j] = 0xFF;

                    // Prepare the command
                    if (fastBlock)
                        commands.Add(RXDataBlockFast(address, data));
                    else
                        commands.Add(RXDataBlock(address, data));


                    // Update the address
                    address += bufferSize - 4;
                }
            }

            return commands;
        }
    }
}
