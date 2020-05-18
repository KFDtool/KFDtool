using KFDtool.P25.DeviceProtocol;
using KFDtool.P25.Kmm;
using KFDtool.P25.NetworkProtocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Net.Mail;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.DataLinkIndependent
{
    public class DataLinkIndependentProtocol : IDeviceProtocol
    {
        private static NLog.Logger Log = NLog.LogManager.GetCurrentClassLogger();

        private UdpProtocol Protocol;

        private bool MotVariant;

        public DataLinkIndependentProtocol(UdpProtocol udpProtocol, bool motVariant)
        {
            Protocol = udpProtocol;
            MotVariant = motVariant;
        }

        public void SendKeySignature()
        {
            // not needed
        }

        public void InitSession()
        {
            if (MotVariant)
            {
                Mfid90SendConnect();

                Mfid90SendBeginSession();
            }
            else
            {
                SendReadyRequest();
            }
        }

        public void EndSession()
        {
            if (MotVariant)
            {
                Mfid90SendTransferDone();

                Mfid90SendEndSession();

                Mfid90SendDisconnect();
            }
            else
            {
                SendTransferDone();

                SendEndSession();

                SendDisconnect();
            }
        }

        public void CheckTargetMrConnection()
        {
            if (MotVariant)
            {
                Mfid90SendConnect();

                Mfid90SendBeginSession();

                Mfid90SendTransferDone();

                Mfid90SendEndSession();

                Mfid90SendDisconnect();
            }
            else
            {
                SendReadyRequest();

                SendTransferDone();

                SendEndSession();

                SendDisconnect();
            }
        }

        public byte[] PerformKmmTransfer(byte[] toRadio)
        {
            Log.Debug("TX: {0}", BitConverter.ToString(toRadio));

            byte[] fromRadio = Protocol.TxRx(toRadio);

            Log.Debug("RX: {0}", BitConverter.ToString(fromRadio));

            return fromRadio;
        }

        private void SendReadyRequest()
        {
            SessionControl commandKmmBody = new SessionControl();
            commandKmmBody.SessionControlOpcode = SessionControl.ScOpcode.ReadyRequest;
            commandKmmBody.SourceDeviceType = SessionControl.ScSourceDeviceType.Kfd;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x00);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is SessionControl)
            {
                SessionControl kmm = responseKmmBody as SessionControl;

                if (kmm.SessionControlOpcode != SessionControl.ScOpcode.ReadyGeneralMode)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void Mfid90SendConnect()
        {
            Mfid90SessionControlVer1 commandKmmBody = new Mfid90SessionControlVer1();
            commandKmmBody.SessionControlOpcode = Mfid90SessionControlVer1.ScOpcode.Connect;
            commandKmmBody.SourceDeviceType = Mfid90SessionControlVer1.ScSourceDeviceType.Kfd;
            commandKmmBody.IsSessionTypeIncluded = false;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x90);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is Mfid90SessionControlVer1)
            {
                Mfid90SessionControlVer1 kmm = responseKmmBody as Mfid90SessionControlVer1;

                if (kmm.SessionControlOpcode != Mfid90SessionControlVer1.ScOpcode.ConnectAck)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void Mfid90SendBeginSession()
        {
            Mfid90SessionControlVer1 commandKmmBody = new Mfid90SessionControlVer1();
            commandKmmBody.SessionControlOpcode = Mfid90SessionControlVer1.ScOpcode.BeginSession;
            commandKmmBody.SourceDeviceType = Mfid90SessionControlVer1.ScSourceDeviceType.Kfd;
            commandKmmBody.IsSessionTypeIncluded = true;
            commandKmmBody.SessionType = Mfid90SessionControlVer1.ScSessionType.KeyFill;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x90);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is Mfid90SessionControlVer1)
            {
                Mfid90SessionControlVer1 kmm = responseKmmBody as Mfid90SessionControlVer1;

                if (kmm.SessionControlOpcode != Mfid90SessionControlVer1.ScOpcode.BeginSessionAck)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void SendTransferDone()
        {
            SessionControl commandKmmBody = new SessionControl();
            commandKmmBody.SessionControlOpcode = SessionControl.ScOpcode.TransferDone;
            commandKmmBody.SourceDeviceType = SessionControl.ScSourceDeviceType.Kfd;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x00);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is SessionControl)
            {
                SessionControl kmm = responseKmmBody as SessionControl;

                if (kmm.SessionControlOpcode != SessionControl.ScOpcode.TransferDone)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void Mfid90SendTransferDone()
        {
            Mfid90SessionControlVer1 commandKmmBody = new Mfid90SessionControlVer1();
            commandKmmBody.SessionControlOpcode = Mfid90SessionControlVer1.ScOpcode.TransferDone;
            commandKmmBody.SourceDeviceType = Mfid90SessionControlVer1.ScSourceDeviceType.Kfd;
            commandKmmBody.IsSessionTypeIncluded = false;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x90);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is Mfid90SessionControlVer1)
            {
                Mfid90SessionControlVer1 kmm = responseKmmBody as Mfid90SessionControlVer1;

                if (kmm.SessionControlOpcode != Mfid90SessionControlVer1.ScOpcode.TransferDone)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void SendEndSession()
        {
            SessionControl commandKmmBody = new SessionControl();
            commandKmmBody.SessionControlOpcode = SessionControl.ScOpcode.EndSession;
            commandKmmBody.SourceDeviceType = SessionControl.ScSourceDeviceType.Kfd;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x00);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is SessionControl)
            {
                SessionControl kmm = responseKmmBody as SessionControl;

                if (kmm.SessionControlOpcode != SessionControl.ScOpcode.EndSessionAck)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void Mfid90SendEndSession()
        {
            Mfid90SessionControlVer1 commandKmmBody = new Mfid90SessionControlVer1();
            commandKmmBody.SessionControlOpcode = Mfid90SessionControlVer1.ScOpcode.EndSession;
            commandKmmBody.SourceDeviceType = Mfid90SessionControlVer1.ScSourceDeviceType.Kfd;
            commandKmmBody.IsSessionTypeIncluded = true;
            commandKmmBody.SessionType = Mfid90SessionControlVer1.ScSessionType.KeyFill;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x90);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is Mfid90SessionControlVer1)
            {
                Mfid90SessionControlVer1 kmm = responseKmmBody as Mfid90SessionControlVer1;

                if (kmm.SessionControlOpcode != Mfid90SessionControlVer1.ScOpcode.EndSessionAck)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void SendDisconnect()
        {
            SessionControl commandKmmBody = new SessionControl();
            commandKmmBody.SessionControlOpcode = SessionControl.ScOpcode.Disconnect;
            commandKmmBody.SourceDeviceType = SessionControl.ScSourceDeviceType.Kfd;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x00);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is SessionControl)
            {
                SessionControl kmm = responseKmmBody as SessionControl;

                if (kmm.SessionControlOpcode != SessionControl.ScOpcode.DisconnectAck)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }

        private void Mfid90SendDisconnect()
        {
            Mfid90SessionControlVer1 commandKmmBody = new Mfid90SessionControlVer1();
            commandKmmBody.SessionControlOpcode = Mfid90SessionControlVer1.ScOpcode.Disconnect;
            commandKmmBody.SourceDeviceType = Mfid90SessionControlVer1.ScSourceDeviceType.Kfd;
            commandKmmBody.IsSessionTypeIncluded = false;

            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytesWithPreamble(0x90);

            byte[] fromRadio = PerformKmmTransfer(toRadio);

            KmmFrame responseKmmFrame = new KmmFrame(true, fromRadio);

            KmmBody responseKmmBody = responseKmmFrame.KmmBody;

            if (responseKmmBody is Mfid90SessionControlVer1)
            {
                Mfid90SessionControlVer1 kmm = responseKmmBody as Mfid90SessionControlVer1;

                if (kmm.SessionControlOpcode != Mfid90SessionControlVer1.ScOpcode.DisconnectAck)
                {
                    throw new Exception(string.Format("received unexpected session control opcode (0x{0:X2}) {1}", (byte)kmm.SessionControlOpcode, kmm.SessionControlOpcode.ToString()));
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }
        }
    }
}
