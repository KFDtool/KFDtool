using KFDtool.Adapter.Protocol.Adapter;
using KFDtool.P25.Constant;
using KFDtool.P25.DeviceProtocol;
using KFDtool.P25.Kmm;
using KFDtool.Shared;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.ThreeWire
{
    public class ThreeWireProtocol : IDeviceProtocol
    {
        private static NLog.Logger Log = NLog.LogManager.GetCurrentClassLogger();

        private const int TIMEOUT_NONE = 0; // no timeout
        private const int TIMEOUT_STD = 5000; // 5 second timeout

        private const byte OPCODE_READY_REQ = 0xC0;
        private const byte OPCODE_READY_GENERAL_MODE = 0xD0;
        private const byte OPCODE_TRANSFER_DONE = 0xC1;
        private const byte OPCODE_KMM = 0xC2;
        private const byte OPCODE_DISCONNECT_ACK = 0x90;
        private const byte OPCODE_DISCONNECT = 0x92;

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
            cmd.Add(OPCODE_READY_REQ);
            Log.Debug("kfd: ready req");
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(cmd));
            Protocol.SendData(cmd);

            // receive ready general mode opcode
            Log.Debug("mr: ready general mode");
            byte rsp = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(rsp));
            if (rsp != OPCODE_READY_GENERAL_MODE)
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

            frame.Add(OPCODE_KMM); // kmm opcode

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

            int length = 0;

            // receive length high byte
            Log.Debug("mr: length high byte");
            temp = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: 0x{0:X2}", temp);

            length |= (temp & 0xFF) << 8;

            // receive length low byte
            Log.Debug("mr: length low byte");
            temp = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));

            length |= temp & 0xFF;

            Log.Debug("length: {0}", length);

            List<byte> toCrc = new List<byte>();

            // receive control
            Log.Debug("mr: control");
            temp = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            // receive dest rsi high byte
            Log.Debug("mr: dest rsi high byte");
            temp = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            // receive dest rsi mid byte
            Log.Debug("mr: dest rsi mid byte");
            temp = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            // receive dest rsi low byte
            Log.Debug("mr: dest rsi low byte");
            temp = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(temp));
            toCrc.Add(temp);

            int bodyLength = length - 6;

            List<byte> kmm = new List<byte>();

            for (int i = 0; i < bodyLength; i++)
            {
                Log.Debug("mr: kmm byte {0} of {1}", i + 1, bodyLength);
                temp = Protocol.GetByte(TIMEOUT_STD);
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
            crc[0] = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(crc[0]));

            // receive crc low byte
            Log.Debug("mr: crc low byte");
            crc[1] = Protocol.GetByte(TIMEOUT_STD);
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
            cmd1.Add(OPCODE_TRANSFER_DONE);
            Log.Debug("kfd: transfer done");
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(cmd1));
            Protocol.SendData(cmd1);

            // receive transfer done opcode
            Log.Debug("mr: transfer done");
            byte rsp1 = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(rsp1));
            if (rsp1 != OPCODE_TRANSFER_DONE)
            {
                throw new Exception("mr: unexpected opcode");
            }

            // send disconnect opcode
            List<byte> cmd2 = new List<byte>();
            cmd2.Add(OPCODE_DISCONNECT);
            Log.Debug("kfd: disconnect");
            Log.Debug("kfd -> mr: {0}", Utility.DataFormat(cmd2));
            Protocol.SendData(cmd2);

            // receive disconnect ack opcode
            Log.Debug("mr: disconnect ack");
            byte rsp2 = Protocol.GetByte(TIMEOUT_STD);
            Log.Debug("mr -> kfd: {0}", Utility.DataFormat(rsp2));
            if (rsp2 != OPCODE_DISCONNECT_ACK)
            {
                throw new Exception("mr: unexpected opcode");
            }
        }

        private void SendKmm(byte[] inKmm)
        {
            if (inKmm.Length > 512)
            {
                throw new Exception("kmm exceeds max size");
            }

            List<byte> txFrame = CreateKmmFrame(inKmm.ToList());
            Log.Debug("out: {0}", Utility.DataFormat(txFrame));
            Protocol.SendData(txFrame);
        }

        public byte[] PerformKmmTransfer(byte[] inKmm)
        {
            Log.Debug("KFD -> MR KMM FRAME: {0}", BitConverter.ToString(inKmm));

            // send kmm frame
            SendKmm(inKmm);

            byte rx;

            // receive kmm opcode
            try
            {
                Log.Debug("in: waiting for kmm opcode");
                rx = Protocol.GetByte(TIMEOUT_STD);
                Log.Trace("in: {0}", Utility.DataFormat(rx));
            }
            catch (Exception)
            {
                string msg = string.Format("in: timed out waiting for kmm opcode");
                Log.Warn(msg);
                throw new Exception(msg);
            }

            if (rx == OPCODE_KMM)
            {
                Log.Debug("in: got kmm opcode");
            }
            else
            {
                string msg = string.Format("in: unexpected kmm opcode, expected ({0}) got ({1})", Utility.DataFormat(OPCODE_KMM), Utility.DataFormat(rx));
                Log.Warn(msg);
                throw new Exception(msg);
            }

            // receive kmm frame
            byte[] rxFrame = ParseKmmFrame().ToArray();

            Log.Debug("MR -> KFD KMM FRAME: {0}", BitConverter.ToString(rxFrame));

            return rxFrame;
        }

        public event EventHandler StatusChanged;

        private string _status;

        public string Status
        {
            get { return _status; }
            set
            {
                if (_status != value)
                {
                    _status = value;
                    OnStatusChanged(new EventArgs());
                }
            }
        }

        private void OnStatusChanged(EventArgs e)
        {
            EventHandler handler = StatusChanged;
            if (handler != null)
                handler(this, e);
        }

        public void MrRunProducer()
        {
            try
            {
                while (true)
                {
                    byte rx;

                    /* RX: KEY SIGNATURE */

                    // currently there is no rx key signature function in the adapter
                    // however, the key signature will appear as a 0x00 byte

                    // the 5 second timeout should prevent most sync issues, however
                    // a rx key signature function should be added to the adapter
                    // to make this more robust and correct

                    Log.Debug("in: waiting for key signature");
                    rx = Protocol.GetByte(TIMEOUT_NONE);
                    Log.Trace("in: {0}", Utility.DataFormat(rx));

                    byte sig = 0x00; // key signature

                    if (rx == sig)
                    {
                        Log.Debug("in: got key signature");
                    }
                    else
                    {
                        string msg = string.Format("in: unexpected key signature opcode, expected ({0}) got ({1})", Utility.DataFormat(sig), Utility.DataFormat(rx));
                        Log.Warn(msg);
                        continue;
                    }

                    /* RX: READY REQUEST */

                    try
                    {
                        Log.Debug("in: waiting for ready request opcode");
                        rx = Protocol.GetByte(TIMEOUT_STD);
                        Log.Trace("in: {0}", Utility.DataFormat(rx));
                    }
                    catch (Exception)
                    {
                        string msg = string.Format("in: timed out waiting for ready request opcode");
                        Log.Warn(msg);
                        continue;
                    }

                    if (rx == OPCODE_READY_REQ)
                    {
                        Log.Debug("in: got ready request opcode");
                    }
                    else
                    {
                        string msg = string.Format("in: unexpected ready request opcode, expected ({0}) got ({1})", Utility.DataFormat(OPCODE_READY_REQ), Utility.DataFormat(rx));
                        Log.Warn(msg);
                        continue;
                    }

                    /* TX: READY GENERAL MODE */

                    Log.Debug("out: ready general mode opcode");
                    Log.Trace("out: {0}", Utility.DataFormat(OPCODE_READY_GENERAL_MODE));
                    Protocol.SendByte(OPCODE_READY_GENERAL_MODE);

                    while (true)
                    {
                        /* RX: FRAME TYPE */

                        try
                        {
                            Log.Debug("in: waiting for frame type opcode");
                            rx = Protocol.GetByte(TIMEOUT_STD);
                            Log.Trace("in: {0}", Utility.DataFormat(rx));
                        }
                        catch (Exception)
                        {
                            string msg = string.Format("in: timed out waiting for frame type opcode");
                            Log.Warn(msg);
                            break;
                        }

                        if (rx == OPCODE_KMM)
                        {
                            Log.Debug("in: got kmm opcode");

                            List<byte> rxFrame;

                            try
                            {
                                rxFrame = ParseKmmFrame();
                            }
                            catch (Exception ex)
                            {
                                Log.Warn(ex.Message);
                                break;
                            }

                            Log.Debug("kmm frame in: {0}", BitConverter.ToString(rxFrame.ToArray()));

                            KmmFrame kfdKmmFrame = null;

                            try
                            {
                                kfdKmmFrame = new KmmFrame(false, rxFrame.ToArray());
                            }
                            catch (Exception ex)
                            {
                                Log.Warn(ex.Message);

                                byte[] message = rxFrame.ToArray();

                                if (message.Length != 0)
                                {
                                    Log.Warn("unexpected message id: {0} (dec), {0:X} (hex)", message[0]);

                                    NegativeAcknowledgment kmm = new NegativeAcknowledgment();

                                    kmm.AcknowledgedMessageId = (MessageId)message[0];
                                    kmm.Status = OperationStatus.InvalidMessageId;

                                    KmmFrame frame = new KmmFrame(kmm);

                                    SendKmm(frame.ToBytes());
                                }

                                continue;
                            }

                            KmmBody kfdKmmBody = kfdKmmFrame.KmmBody;

                            if (kfdKmmBody is InventoryCommandListActiveKsetIds)
                            {
                                InventoryResponseListActiveKsetIds mrKmm = new InventoryResponseListActiveKsetIds();

                                // do not return any keysets, to match factory Motorola SU behavior

                                KmmFrame commandKmmFrame = new KmmFrame(mrKmm);

                                SendKmm(commandKmmFrame.ToBytes());
                            }
                            else if (kfdKmmBody is InventoryCommandListRsiItems)
                            {
                                InventoryResponseListRsiItems mrKmm = new InventoryResponseListRsiItems();

                                RsiItem item = new RsiItem();

                                // set RSI and message number to match factory Motorola SU behavior

                                item.RSI = 0x000001;
                                item.MessageNumber = 0x0000;

                                mrKmm.RsiItems.Add(item);

                                KmmFrame commandKmmFrame = new KmmFrame(mrKmm);

                                SendKmm(commandKmmFrame.ToBytes());
                            }
                            else if (kfdKmmBody is ModifyKeyCommand)
                            {
                                ModifyKeyCommand cmdKmm = kfdKmmBody as ModifyKeyCommand;

                                Log.Debug("keyset id: {0} (dec), {0:X} (hex)", cmdKmm.KeysetId);
                                Log.Debug("algorithm id: {0} (dec), {0:X} (hex)", cmdKmm.AlgorithmId);

                                RekeyAcknowledgment rspKmm = new RekeyAcknowledgment();

                                rspKmm.MessageIdAcknowledged = MessageId.ModifyKeyCommand;
                                rspKmm.NumberOfItems = cmdKmm.KeyItems.Count;

                                for (int i = 0; i < cmdKmm.KeyItems.Count; i++)
                                {
                                    KeyItem item = cmdKmm.KeyItems[i];

                                    Log.Debug("* key item {0} *", i);
                                    Log.Debug("erase: {0}", item.Erase);
                                    Log.Debug("sln: {0} (dec), {0:X} (hex)", item.SLN);
                                    Log.Debug("key id: {0} (dec), {0:X} (hex)", item.KeyId);
                                    Log.Debug("key: {0}", BitConverter.ToString(item.Key));

                                    string algName = string.Empty;

                                    if (Enum.IsDefined(typeof(AlgorithmId), (byte)cmdKmm.AlgorithmId))
                                    {
                                        algName = ((AlgorithmId)cmdKmm.AlgorithmId).ToString();
                                    }
                                    else
                                    {
                                        algName = "UNKNOWN";
                                    }

                                    Status +=
                                        string.Format("Keyset ID: {0} (dec), {0:X} (hex)", cmdKmm.KeysetId) + Environment.NewLine +
                                        string.Format("SLN/CKR: {0} (dec), {0:X} (hex)", item.SLN) + Environment.NewLine +
                                        string.Format("Key ID: {0} (dec), {0:X} (hex)", item.KeyId) + Environment.NewLine +
                                        string.Format("Algorithm: {0} (dec), {0:X} (hex), {1}", cmdKmm.AlgorithmId, algName) + Environment.NewLine +
                                        string.Format("Key: {0}", BitConverter.ToString(item.Key).Replace("-", string.Empty)) + Environment.NewLine +
                                        "--" + Environment.NewLine;

                                    KeyStatus status = new KeyStatus();

                                    status.AlgorithmId = cmdKmm.AlgorithmId;
                                    status.KeyId = item.KeyId;
                                    status.Status = 0x00; // command was performed

                                    rspKmm.Keys.Add(status);
                                }

                                KmmFrame cmdKmmFrame = new KmmFrame(rspKmm);

                                SendKmm(cmdKmmFrame.ToBytes());
                            }
                        }
                        else if (rx == OPCODE_TRANSFER_DONE)
                        {
                            Log.Debug("in: got transfer done opcode");

                            /* TX: TRANSFER DONE */

                            Log.Debug("out: transfer done opcode");
                            Log.Trace("out: {0}", Utility.DataFormat(OPCODE_TRANSFER_DONE));
                            Protocol.SendByte(OPCODE_TRANSFER_DONE);

                            /* RX: DISCONNECT */

                            try
                            {
                                Log.Debug("in: waiting for disconnect opcode");
                                rx = Protocol.GetByte(TIMEOUT_STD);
                                Log.Trace("in: {0}", Utility.DataFormat(rx));
                            }
                            catch (Exception)
                            {
                                string msg = string.Format("in: timed out waiting for disconnect opcode");
                                Log.Warn(msg);
                                break;
                            }

                            if (rx == OPCODE_DISCONNECT)
                            {
                                Log.Debug("in: got disconnect opcode");
                            }
                            else
                            {
                                string msg = string.Format("in: unexpected disconnect opcode, expected ({0}) got ({1})", Utility.DataFormat(OPCODE_DISCONNECT), Utility.DataFormat(rx));
                                Log.Warn(msg);
                                break;
                            }

                            /* TX: DISCONNECT ACKNOWLEDGE */

                            Log.Debug("out: disconnect acknowledge opcode");
                            Log.Trace("out: {0}", Utility.DataFormat(OPCODE_DISCONNECT_ACK));
                            Protocol.SendByte(OPCODE_DISCONNECT_ACK);
                            break;
                        }
                        else
                        {
                            string msg = string.Format("in: unexpected frame type opcode ({0})", Utility.DataFormat(rx));
                            Log.Warn(msg);
                            break;
                        }
                    }
                }
            }
            catch (OperationCanceledException)
            {
                Log.Debug("operation cancelled");
                return;
            }
            catch (Exception ex)
            {
                Log.Warn("error in mr emulation: {0}", ex.Message);
                throw;
            }
        }
    }
}
