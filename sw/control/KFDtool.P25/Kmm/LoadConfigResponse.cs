using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class LoadConfigResponse : KmmBody
    {
        public int RSI { get; private set; }
        public int MN { get; private set; }
        //public RsiInfo RsiInfo { get; set; }
        public int Status { get; private set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.LoadConfigResponse;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public LoadConfigResponse()
        {
        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length != 6)
            {
                throw new ArgumentOutOfRangeException("contents", string.Format("length mismatch - expected 6, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* kmf rsi */
            RSI |= contents[0] << 16;
            RSI |= contents[1] << 8;
            RSI |= contents[2];

            /* message number */
            MN |= contents[3] << 8;
            MN |= contents[4];

            /* status */
            Status |= contents[5];
        }
    }
}
