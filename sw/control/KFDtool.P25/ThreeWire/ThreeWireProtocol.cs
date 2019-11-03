using KFDtool.Adapter.Protocol.Adapter;
using KFDtool.Shared;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.ThreeWire
{
    public class ThreeWireProtocol
    {
        private static NLog.Logger Log = NLog.LogManager.GetCurrentClassLogger();

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
            Log.Debug("kfd: key signature");
            Protocol.SendKeySignature();
        }

        public void InitSession()
        {
            // send ready req opcode
            List<byte> cmd = new List<byte>();
            cmd.Add(READY_REQ_OPCODE);
            Log.Debug("kfd: ready req");
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(cmd));
            Protocol.SendData(cmd);

            // receive ready general mode opcode
            Log.Debug("mr: ready general mode");
            byte rsp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(rsp));
            if (rsp != READY_GENERAL_MODE_OPCODE)
            {
                throw new Exception("mr: unexpected opcode");
            }
        }

        public void CheckTargetMrConnection()
        {
            SendKeySignature();
            InitSession();
            EndSession();
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
            Log.Debug("mr: kmm opcode");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            if (temp != KMM_OPCODE)
            {
                throw new Exception(string.Format("mr: unexpected opcode, expected: 0x{0:X2}, got: 0x{1:X2}", KMM_OPCODE, temp));
            }

            int length = 0;

            // receive length high byte
            Log.Debug("mr: length high byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: 0x{0:X2}", temp);

            length |= (temp & 0xFF) << 8;

            // receive length low byte
            Log.Debug("mr: length low byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));

            length |= temp & 0xFF;

            Log.Debug("length: {0}", length);

            List<byte> toCrc = new List<byte>();

            // receive control
            Log.Debug("mr: control");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            // receive dest rsi high byte
            Log.Debug("mr: dest rsi high byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            // receive dest rsi mid byte
            Log.Debug("mr: dest rsi mid byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            // receive dest rsi low byte
            Log.Debug("mr: dest rsi low byte");
            temp = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            int bodyLength = length - 6;

            List<byte> kmm = new List<byte>();

            for (int i = 0; i < bodyLength; i++)
            {
                Log.Debug("mr: kmm byte {0} of {1}", i + 1, bodyLength);
                temp = Protocol.GetByte(TWI_TIMEOUT);
                Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));

                kmm.Add(temp);
            }

            toCrc.AddRange(kmm);

            // calculate crc
            byte[] expectedCrc = CRC16.CalculateCrc(toCrc.ToArray());

            Log.Debug("expected crc - high: 0x{0:X2}, low: 0x{1:X2}", expectedCrc[0], expectedCrc[1]);

            byte[] crc = new byte[2];

            // receive crc high byte
            Log.Debug("mr: crc high byte");
            crc[0] = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(crc[0]));

            // receive crc low byte
            Log.Debug("mr: crc low byte");
            crc[1] = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(crc[1]));

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
            Log.Debug("kfd: transfer done");
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(cmd1));
            Protocol.SendData(cmd1);

            // receive transfer done opcode
            Log.Debug("mr: transfer done");
            byte rsp1 = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(rsp1));
            if (rsp1 != TRANSFER_DONE_OPCODE)
            {
                throw new Exception("mr: unexpected opcode");
            }

            // send disconnect opcode
            List<byte> cmd2 = new List<byte>();
            cmd2.Add(DISCONNECT_OPCODE);
            Log.Debug("kfd: disconnect");
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(cmd2));
            Protocol.SendData(cmd2);

            // receive disconnect ack opcode
            Log.Debug("mr: disconnect ack");
            byte rsp2 = Protocol.GetByte(TWI_TIMEOUT);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(rsp2));
            if (rsp2 != DISCONNECT_ACK_OPCODE)
            {
                throw new Exception("mr: unexpected opcode");
            }
        }

        public byte[] PerformKmmTransfer(byte[] inKmm)
        {
            List<byte> txFrame = CreateKmmFrame(inKmm.ToList());
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(txFrame));
            Protocol.SendData(txFrame);

            List<byte> rxFrame = ParseKmmFrame();

            return rxFrame.ToArray();
        }
    }
}
