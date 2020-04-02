using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    /* TIA 102.AACA-A 10.2.19 and TIA 102.AACD-A 3.9.2.17 */
    public class LoadConfigCommand : KmmBody
    {
        private int _kmfRsi;

        private int _mnp;

        public int KmfRsi
        {
            get
            {
                return _kmfRsi;
            }
            set
            {
                if (value < 0 || value > 0xFFFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _kmfRsi = value;
            }
        }

        public int MessageNumberPeriod
        {
            get
            {
                return _mnp;
            }
            set
            {
                if (value < 0 || value > 0xFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _mnp = value;
            }
        }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.LoadConfigCommand;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public LoadConfigCommand()
        {

        }

        public override byte[] ToBytes()
        {
            //List<byte> contents = new List<byte>();
            byte[] contents = new byte[5];

            /* kmf rsi */
            contents[0] = (byte)(KmfRsi >> 16);
            contents[1] = (byte)(KmfRsi >> 8);
            contents[2] = (byte)(KmfRsi);

            /* message number period */
            contents[3] = (byte)(MessageNumberPeriod >> 8);
            contents[4] = (byte)(MessageNumberPeriod);

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length != 5)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected 5, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* kmf rsi */
            KmfRsi |= contents[0] << 16;
            KmfRsi |= contents[1] << 8;
            KmfRsi |= contents[2];

            /* message number period */
            MessageNumberPeriod |= contents[3] << 8;
            MessageNumberPeriod |= contents[4];
        }
    }
}
