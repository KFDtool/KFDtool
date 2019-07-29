using KFDtool.Adapter.Protocol.Adapter;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.ThreeWire
{
    public class ThreeWireProtocol
    {
        private const int TWI_TIMEOUT = 5000; // 5 second

        private const byte READY_REQ_OPCODE = 0xC0;
        private const byte READY_GENERAL_MODE_OPCODE = 0xD0;
        private const byte TRANSFER_DONE_OPCODE = 0xC1;
        private const byte KMM_OPCODE = 0xC2;
        private const byte DISCONNECT_ACK_OPCODE = 0x90;
        private const byte DISCONNECT_OPCODE = 0x92;

        private AdapterProtocol Protocol;

        public ThreeWireProtocol(AdapterProtocol ap)
        {
            Protocol = ap;
        }

        public void SendKeySignature()
        {
            Protocol.SendKeySignature();
        }

        public void InitSession()
        {
            // send ready req opcode
            List<byte> cmd = new List<byte>();
            cmd.Add(READY_REQ_OPCODE);
            //Console.WriteLine("kfd: ready req");
            //Console.WriteLine("kfd -> mr: 0x{0:X2}", temp);
            Protocol.SendData(cmd);

            // receive ready general mode opcode
            //Console.WriteLine("mr: ready general mode");
            byte rsp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            if (rsp != READY_GENERAL_MODE_OPCODE)
            {
                throw new Exception("mr: unexpected opcode");
            }
        }

        private List<byte> CreateKmmFrame(List<byte> kmm)
        {
            // create body
            List<byte> body = new List<byte>();

            body.Add(0x00); // control
            body.Add(0xFF); // destination RSI high byte
            body.Add(0xFF); // destination RSI mid byte
            body.Add(0xFF); // destination RSI low byte
            body.AddRange(kmm); // kmm

            // calculate crc
            byte[] crc = CRC16.CalculateCrc(body.ToArray());

            // create frame
            List<byte> frame = new List<byte>();

            int length = body.Count + 2; // control + dest rsi + kmm + crc

            frame.Add(KMM_OPCODE); // kmm opcode

            frame.Add((byte)((length >> 8) & 0xFF)); // length high byte
            frame.Add((byte)(length & 0xFF)); // length low byte

            frame.AddRange(body); // kmm body

            frame.Add(crc[0]); // crc high byte
            frame.Add(crc[1]); // crc low byte

            return frame;
        }

        private List<byte> ParseKmmFrame()
        {
            byte temp;

            // receive kmm opcode
            //Console.WriteLine("mr: kmm opcode");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            if (temp != KMM_OPCODE)
            {
                throw new Exception(string.Format("mr: unexpected opcode, expected: 0x{0:X2}, got: 0x{1:X2}", KMM_OPCODE, temp));
            }

            int length = 0;

            // receive length high byte
            //Console.WriteLine("mr: length high byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);

            length |= (temp & 0xFF) << 8;

            // receive length low byte
            //Console.WriteLine("mr: length low byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);

            length |= temp & 0xFF;

            //Console.WriteLine("length: {0}", length);

            List<byte> toCrc = new List<byte>();

            // receive control
            //Console.WriteLine("mr: control");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            toCrc.Add(temp);

            // receive dest rsi high byte
            //Console.WriteLine("mr: dest rsi high byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            toCrc.Add(temp);

            // receive dest rsi mid byte
            //Console.WriteLine("mr: dest rsi mid byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            toCrc.Add(temp);

            // receive dest rsi low byte
            //Console.WriteLine("mr: dest rsi low byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            toCrc.Add(temp);

            int bodyLength = length - 6;

            List<byte> kmm = new List<byte>();

            for (int i = 0; i < bodyLength; i++)
            {
                //Console.WriteLine("mr: kmm byte {0} of {1}", i + 1, bodyLength);
                temp = Protocol.GetByte(TWI_TIMEOUT);
                //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
                
                kmm.Add(temp);
            }

            toCrc.AddRange(kmm);

            byte[] crc = new byte[2];

            // receive crc high byte
            //Console.WriteLine("mr: crc high byte");
            crc[0] = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", crc[0]);

            // receive crc low byte
            //Console.WriteLine("mr: crc low byte");
            crc[1] = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", crc[1]);

            // calculate crc
            byte[] expectedCrc = CRC16.CalculateCrc(toCrc.ToArray());

            if (expectedCrc[0] != crc[0])
            {
                throw new Exception(string.Format("mr: crc high byte mismatch, expected: 0x{0:X2}, got: 0x{1:X2}", expectedCrc[0], crc[0]));
            }

            if (expectedCrc[1] != crc[1])
            {
                throw new Exception(string.Format("mr: crc low byte mismatch, expected: 0x{0:X2}, got: 0x{1:X2}", expectedCrc[1], crc[1]));
            }

            return kmm;
        }

        public void EndSession()
        {
            // send transfer done opcode
            List<byte> cmd1 = new List<byte>();
            cmd1.Add(TRANSFER_DONE_OPCODE);
            //Console.WriteLine("kfd: transfer done");
            //Console.WriteLine("kfd -> mr: 0x{0:X2}", temp);
            Protocol.SendData(cmd1);

            // receive transfer done opcode
            //Console.WriteLine("mr: transfer done");
            byte rsp1 = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            if (rsp1 != TRANSFER_DONE_OPCODE)
            {
                throw new Exception("mr: unexpected opcode");
            }

            // send disconnect opcode
            List<byte> cmd2 = new List<byte>();
            cmd2.Add(DISCONNECT_OPCODE);
            //Console.WriteLine("kfd: disconnect");
            //Console.WriteLine("kfd -> mr: 0x{0:X2}", temp);
            Protocol.SendData(cmd2);

            // receive disconnect ack opcode
            //Console.WriteLine("mr: disconnect ack");
            byte rsp2 = Protocol.GetByte(TWI_TIMEOUT);
            //Console.WriteLine("mr -> kfd: 0x{0:X2}", temp);
            if (rsp2 != DISCONNECT_ACK_OPCODE)
            {
                throw new Exception("mr: unexpected opcode");
            }
        }

        public byte[] PerformKmmTransfer(byte[] inKmm)
        {
            List<byte> frame = CreateKmmFrame(inKmm.ToList());
            //Console.WriteLine("kfd -> mr: {0}", BitConverter.ToString(frame.ToArray()));
            Protocol.SendData(frame);

            // receive kmm frame
            List<byte> outKmm = ParseKmmFrame();

            return outKmm.ToArray();
        }
    }
}
