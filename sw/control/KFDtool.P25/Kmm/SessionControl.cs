using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class SessionControl : KmmBody
    {
        public enum ScOpcode : byte
        {
            ReadyRequest = 0x01,
            ReadyGeneralMode = 0x02,
            TransferDone = 0x03,
            EndSession = 0x04,
            EndSessionAck = 0x05,
            Disconnect = 0x06,
            DisconnectAck = 0x07
        }

        public enum ScSourceDeviceType : byte
        {
            Kfd = 0x01,
            Mr = 0x02
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

        public override byte[] ToBytes()
        {
            List<byte> contents = new List<byte>();

            contents.Add(0x00); // version

            contents.Add((byte)SessionControlOpcode);

            contents.Add((byte)SourceDeviceType);

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            if (contents[0] != 0x00)
            {
                throw new Exception("unsupported version");
            }

            if (contents.Length != 3)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected 3, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            SessionControlOpcode = (ScOpcode)contents[1];

            SourceDeviceType = (ScSourceDeviceType)contents[2];
        }
    }
}
