using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class Mfid90SessionControlVer1 : KmmBody
    {
        public enum ScOpcode : byte
        {
            Connect = 0x01,
            ConnectAck = 0x02,
            TransferDone = 0x03,
            EndSession = 0x04,
            EndSessionAck = 0x05,
            Disconnect = 0x06,
            DisconnectAck = 0x07,
            BeginSession = 0x08,
            BeginSessionAck = 0x09
        }

        public enum ScSourceDeviceType : byte
        {
            Kfd = 0x01,
            Mr = 0x02,
            Kmf = 0x03,
            Af = 0x04
        }

        public enum ScSessionType : byte
        {
            KeyFill = 0x01,
            BulkTransfer = 0x02,
            StoreAndForward = 0x03
        }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.SessionControl;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public ScOpcode SessionControlOpcode { get; set; }

        public ScSourceDeviceType SourceDeviceType { get; set; }

        public bool IsSessionTypeIncluded { get; set; }

        public ScSessionType SessionType { get; set; }

        public override byte[] ToBytes()
        {
            List<byte> contents = new List<byte>();

            contents.Add(0x01); // version

            contents.Add((byte)SessionControlOpcode);

            contents.Add((byte)SourceDeviceType);

            contents.Add((byte)(IsSessionTypeIncluded ? 1 : 0));

            if (IsSessionTypeIncluded)
            {
                contents.Add((byte)SessionType);
            }

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            if (contents[0] != 0x01)
            {
                throw new Exception("unsupported version");
            }

            if (contents.Length < 4)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected at least 4, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            SessionControlOpcode = (ScOpcode)contents[1];

            SourceDeviceType = (ScSourceDeviceType)contents[2];

            if (contents[3] == 0x00)
            {
                IsSessionTypeIncluded = false;
            }
            else if (contents[3] == 0x01)
            {
                IsSessionTypeIncluded = true;

                if (contents.Length < 5)
                {
                    throw new ArgumentOutOfRangeException(string.Format("length mismatch for session type - expected at least 5, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
                }

                SessionType = (ScSessionType)contents[4];
            }
            else
            {
                throw new Exception("invalid is session type included");
            }
        }
    }
}
