using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class ChangeRsiCommand : KmmBody
    {
        private int _changeSequence;
        private int _rsiOld;
        private int _rsiNew;
        private int _messageNumber;

        public int RsiOld
        {
            get
            {
                return _rsiOld;
            }
            set
            {
                if (value < 0 || value > 0xFFFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _rsiOld = value;
            }
        }

        public int RsiNew
        {
            get
            {
                return _rsiNew;
            }
            set
            {
                if (value < 0 || value > 0xFFFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _rsiNew = value;
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

        public int ChangeSequence
        {
            get
            {
                return _changeSequence;
            }
            set
            {
                if (value < 0 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _changeSequence = value;
            }
        }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.ChangeRsiCommand;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public ChangeRsiCommand()
        {
        }

        public override byte[] ToBytes()
        {
            //List<byte> contents = new List<byte>();
            byte[] contents = new byte[9];

            /* change sequence/instruction */
            contents[0] = 0x01;

            /* old rsi */
            contents[1] = (byte)(RsiOld >> 16);
            contents[2] = (byte)(RsiOld >> 8);
            contents[3] = (byte)(RsiOld);

            /* new rsi */
            contents[4] = (byte)(RsiNew >> 16);
            contents[5] = (byte)(RsiNew >> 8);
            contents[6] = (byte)(RsiNew);

            /* message number */
            contents[7] = (byte)(MessageNumber >> 8);
            contents[8] = (byte)(MessageNumber);

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
