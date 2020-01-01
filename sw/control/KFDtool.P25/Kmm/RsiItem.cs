using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class RsiItem
    {
        private uint _rsi;

        private int _messageNumber;

        public uint RSI
        {
            get
            {
                return _rsi;
            }
            set
            {
                if (value < 0 || value > 0xFFFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _rsi = value;
            }
        }

        public int MessageNumber
        {
            get
            {
                return _messageNumber;
            }
            set
            {
                if (value < 0 || value > 0xFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _messageNumber = value;
            }
        }

        public RsiItem()
        {
        }

        public byte[] ToBytes()
        {
            byte[] contents = new byte[5];

            /* rsi */
            contents[0] = (byte)(MessageNumber >> 16);
            contents[1] = (byte)(MessageNumber >> 8);
            contents[2] = (byte)MessageNumber;

            /* message number */
            contents[3] = (byte)(MessageNumber >> 8);
            contents[4] = (byte)MessageNumber;

            return contents;
        }

        public void Parse(byte[] contents)
        {
            if (contents.Length != 5)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected 5, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* rsi */
            RSI |= (uint)(contents[0] << 16);
            RSI |= (uint)(contents[1] << 8);
            RSI |= contents[2];

            /* message number */
            MessageNumber |= contents[3] << 8;
            MessageNumber |= contents[4];
        }
    }
}
