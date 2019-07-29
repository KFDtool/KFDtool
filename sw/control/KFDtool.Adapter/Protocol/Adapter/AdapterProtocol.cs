using KFDtool.Adapter.Protocol.Serial;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Adapter.Protocol.Adapter
{
    public class AdapterProtocol
    {
        private const int AP_TIMEOUT = 1000; // 1 second

        /* COMMAND OPCODES */
        private const byte CMD_READ = 0x11;
        private const byte CMD_WRITE_INFO = 0x12;
        private const byte CMD_ENTER_BSL_MODE = 0x13;
        private const byte CMD_RESET = 0x14;
        private const byte CMD_SELF_TEST = 0x15;
        private const byte CMD_SEND_KEY_SIG = 0x16;
        private const byte CMD_SEND_BYTE = 0x17;

        /* RESPONSE OPCODES */
        private const byte RSP_ERROR = 0x20;
        private const byte RSP_READ = 0x21;
        private const byte RSP_WRITE_INFO = 0x22;
        private const byte RSP_ENTER_BSL_MODE = 0x23;
        private const byte RSP_RESET = 0x24;
        private const byte RSP_SELF_TEST = 0x25;
        private const byte RSP_SEND_KEY_SIG = 0x26;
        private const byte RSP_SEND_BYTE = 0x27;

        /* BROADCAST OPCODES */
        private const byte BCST_RECEIVE_BYTE = 0x31;

        /* READ OPCODES */
        private const byte READ_AP_VER = 0x01;
        private const byte READ_FW_VER = 0x02;
        private const byte READ_UNIQUE_ID = 0x03;
        private const byte READ_MODEL_ID = 0x04;
        private const byte READ_HW_REV = 0x05;
        private const byte READ_SER_NUM = 0x06;

        /* WRITE OPCODES */
        private const byte WRITE_MDL_REV = 0x01;
        private const byte WRITE_SER = 0x02;

        /* ERROR OPCODES */
        private const byte ERR_OTHER = 0x00;
        private const byte ERR_INVALID_CMD_LENGTH = 0x01;
        private const byte ERR_INVALID_CMD_OPCODE = 0x02;
        private const byte ERR_INVALID_READ_OPCODE = 0x03;
        private const byte ERR_READ_FAILED = 0x04;
        private const byte ERR_INVALID_WRITE_OPCODE = 0x05;
        private const byte ERR_WRITE_FAILED = 0x06;

        private SerialProtocol Lower;

        public AdapterProtocol(string portName)
        {
            Lower = new SerialProtocol(portName);
        }

        public void Open()
        {
            Lower.Open();
        }

        public void Close()
        {
            Lower.Close();
        }

        public void Clear()
        {
            Lower.Clear();
        }

        public byte[] ReadAdapterProtocolVersion()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: READ AP VERSION
            * 
            * [0] CMD_READ
            * [1] READ_AP_VER
            */

            cmd.Add(CMD_READ);
            cmd.Add(READ_AP_VER);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: READ AP VERSION
            * 
            * [0] RSP_READ
            * [1] READ_AP_VER
            * [2] api major version
            * [3] api minor version
            * [4] api patch version
            */

            if (rsp.Count == 5)
            {
                if (rsp[0] == RSP_READ)
                {
                    if (rsp[1] == READ_AP_VER)
                    {
                        byte[] ver = new byte[3];
                        ver[0] = rsp[2];
                        ver[1] = rsp[3];
                        ver[2] = rsp[4];
                        return ver;
                    }
                    else
                    {
                        throw new Exception("invalid read opcode");
                    }
                }
                else
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public byte[] ReadFirmwareVersion()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: READ FW VERSION
            * 
            * [0] CMD_READ
            * [1] READ_FW_VER
            */

            cmd.Add(CMD_READ);
            cmd.Add(READ_FW_VER);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: READ FW VERSION
            * 
            * [0] RSP_READ
            * [1] READ_FW_VER
            * [2] fw major version
            * [3] fw minor version
            * [4] fw patch version
            */

            if (rsp.Count == 5)
            {
                if (rsp[0] == RSP_READ)
                {
                    if (rsp[1] == READ_FW_VER)
                    {
                        byte[] ver = new byte[3];
                        ver[0] = rsp[2];
                        ver[1] = rsp[3];
                        ver[2] = rsp[4];
                        return ver;
                    }
                    else
                    {
                        throw new Exception("invalid read opcode");
                    }
                }
                else
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public byte[] ReadUniqueId()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: READ UNIQUE ID
            * 
            * [0] CMD_READ
            * [1] READ_UNIQUE_ID
            */

            cmd.Add(CMD_READ);
            cmd.Add(READ_UNIQUE_ID);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: READ UNIQUE ID
            * 
            * [0] RSP_READ
            * [1] READ_UNIQUE_ID
            * [2] unique id length
            * [3 to 3 + unique id length] raw unique id if unique id length > 0
            */

            if (rsp.Count >= 3)
            {
                if (rsp[0] == RSP_READ)
                {
                    if (rsp[1] == READ_UNIQUE_ID)
                    {
                        if (rsp[2] == 0) // no unique id
                        {
                            return new byte[0];
                        }
                        else if (rsp[2] == (rsp.Count - 3))
                        {
                            int len = rsp[2];
                            byte[] num = new byte[len];
                            Array.Copy(rsp.ToArray(), 3, num, 0, len);
                            return num;
                        }
                        else
                        {
                            throw new Exception("message length field and message length mismatch");
                        }
                    }
                    else
                    {
                        throw new Exception("invalid read opcode");
                    }
                }
                else
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public byte ReadModelId()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: READ MODEL ID
            * 
            * [0] CMD_READ
            * [1] READ_MODEL_ID
            */

            cmd.Add(CMD_READ);
            cmd.Add(READ_MODEL_ID);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: READ MODEL ID
            * 
            * [0] RSP_READ
            * [1] READ_MODEL_ID
            * [2] model id
            */

            if (rsp.Count == 3)
            {
                if (rsp[0] == RSP_READ)
                {
                    if (rsp[1] == READ_MODEL_ID)
                    {
                        return rsp[2];
                    }
                    else
                    {
                        throw new Exception("invalid read opcode");
                    }
                }
                else
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public byte[] ReadHardwareRevision()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: READ HARDWARE REVISION
            * 
            * [0] CMD_READ
            * [1] READ_HW_REV
            */

            cmd.Add(CMD_READ);
            cmd.Add(READ_HW_REV);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: READ HARDWARE REVISION
            * 
            * [0] RSP_READ
            * [1] READ_HW_REV
            * [2] hw rev major version
            * [3] hw rev minor version
            */

            if (rsp.Count == 4)
            {
                if (rsp[0] == RSP_READ)
                {
                    if (rsp[1] == READ_HW_REV)
                    {
                        byte[] ver = new byte[2];
                        ver[0] = rsp[2];
                        ver[1] = rsp[3];
                        return ver;
                    }
                    else
                    {
                        throw new Exception("invalid read opcode");
                    }
                }
                else
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public byte[] ReadSerialNumber()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: READ SERIAL NUMBER
            * 
            * [0] CMD_READ
            * [1] READ_SER_NUM
            */

            cmd.Add(CMD_READ);
            cmd.Add(READ_SER_NUM);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: READ SERIAL NUMBER
            * 
            * [0] RSP_READ
            * [1] READ_SER_NUM
            * [2] serial number length
            * [3 to 3 + serial number length] serial number if serial number length > 0
            */

            if (rsp.Count >= 3)
            {
                if (rsp[0] == RSP_READ)
                {
                    if (rsp[1] == READ_SER_NUM)
                    {
                        if (rsp[2] == 0) // no serial number
                        {
                            return new byte[0];
                        }
                        else if (rsp[2] == (rsp.Count - 3))
                        {
                            int len = rsp[2];
                            byte[] num = new byte[len];
                            Array.Copy(rsp.ToArray(), 3, num, 0, len);

                            // TODO validate ascii characters to only accept 0-9, A-Z

                            return num;
                        }
                        else
                        {
                            throw new Exception("message length field and message length mismatch");
                        }
                    }
                    else
                    {
                        throw new Exception("invalid read opcode");
                    }
                }
                else
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public void WriteInfo(byte mdlId, byte hwRevMaj, byte hwRevMin)
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: WRITE INFO
            * 
            * [0] CMD_WRITE_INFO
            * [1] WRITE_MDL_REV
            * [2] model id
            * [3] hardware revision major version
            * [4] hardware revision minor version
            */

            cmd.Add(CMD_WRITE_INFO);
            cmd.Add(WRITE_MDL_REV);
            cmd.Add(mdlId);
            cmd.Add(hwRevMaj);
            cmd.Add(hwRevMin);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: WRITE INFO
            * 
            * [0] RSP_WRITE_INFO
            */

            if (rsp.Count == 1)
            {
                if (rsp[0] != RSP_WRITE_INFO)
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public void EnterBslMode()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: ENTER BSL MODE
            * 
            * [0] CMD_ENTER_BSL_MODE
            */

            cmd.Add(CMD_ENTER_BSL_MODE);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: ENTER BSL MODE
            * 
            * [0] RSP_ENTER_BSL_MODE
            */

            if (rsp.Count == 1)
            {
                if (rsp[0] != RSP_ENTER_BSL_MODE)
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public void Reset()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: RESET
            * 
            * [0] CMD_RESET
            */

            cmd.Add(CMD_RESET);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: RESET
            * 
            * [0] RSP_RESET
            */

            if (rsp.Count == 1)
            {
                if (rsp[0] != RSP_RESET)
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public byte SelfTest()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: SELF TEST
            * 
            * [0] CMD_SELF_TEST
            */

            cmd.Add(CMD_SELF_TEST);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: SELF TEST
            * 
            * [0] RSP_SELF_TEST
            * [1] self test result
            */

            if (rsp.Count == 2)
            {
                if (rsp[0] == RSP_SELF_TEST)
                {
                    return rsp[1];
                }
                else
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public void SendKeySignature()
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: SEND KEY SIGNATURE
            * 
            * [0] CMD_SEND_KEY_SIG
            * [1] reserved (set to 0x00)
            */

            cmd.Add(CMD_SEND_KEY_SIG);
            cmd.Add(0x00);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: SEND KEY SIGNATURE
            * 
            * [0] RSP_SEND_KEY_SIG
            */

            if (rsp.Count == 1)
            {
                if (rsp[0] != RSP_SEND_KEY_SIG)
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public void SendByte(byte data)
        {
            List<byte> cmd = new List<byte>();

            /*
            * CMD: SEND BYTE
            * 
            * [0] CMD_SEND_BYTE
            * [1] reserved (set to 0x00)
            * [2] byte to send
            */

            cmd.Add(CMD_SEND_BYTE);
            cmd.Add(0x00);
            cmd.Add(data);

            Lower.Send(cmd);

            List<byte> rsp = Lower.Read(AP_TIMEOUT);

            /*
            * RSP: SEND BYTE
            * 
            * [0] RSP_SEND_BYTE
            */

            if (rsp.Count == 1)
            {
                if (rsp[0] != RSP_SEND_BYTE)
                {
                    throw new Exception("invalid response opcode");
                }
            }
            else
            {
                throw new Exception("invalid response length");
            }
        }

        public void SendData(List<byte> data)
        {
            foreach (byte b in data)
            {
                SendByte(b);
            }
        }

        public byte GetByte(int timeout)
        {
            List<byte> rsp = Lower.Read(timeout);

            /*
            * BCST: RECEIVE BYTE
            * 
            * [0] BCST_RECEIVE_BYTE
            * [1] reserved (set to 0x00)
            * [2] byte received
            */

            if (rsp.Count == 3)
            {
                if (rsp[0] == BCST_RECEIVE_BYTE)
                {
                    return rsp[2];
                }
                else
                {
                    throw new Exception("invalid broadcast opcode");
                }
            }
            else
            {
                throw new Exception("invalid broadcast length");
            }
        }

        public void Cancel()
        {
            Lower.Cancel();
        }
    }
}
