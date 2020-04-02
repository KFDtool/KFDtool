using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class ChangeRsiResponse : KmmBody
    {
        public int ChangeSequence { get; private set; }
        public int RsiOld { get; private set; }
        public int RsiNew { get; private set; }
        public int Status { get; private set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.ChangeRsiResponse;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public ChangeRsiResponse()
        {
        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length != 8)
            {
                throw new ArgumentOutOfRangeException("contents", string.Format("length mismatch - expected 8, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* change sequence/instruction */
            ChangeSequence = contents[0];

            /* old rsi */
            RsiOld |= contents[1] << 16;
            RsiOld |= contents[2] << 8;
            RsiOld |= contents[3];

            /* new rsi */
            RsiNew |= contents[4] << 16;
            RsiNew |= contents[5] << 8;
            RsiNew |= contents[6];

            /* status */
            Status |= contents[7];

        }
    }
}
