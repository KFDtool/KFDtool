using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class ZeroizeResponse : KmmBody
    {
        public override MessageId MessageId
        {
            get
            {
                return MessageId.ZeroizeResponse;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public ZeroizeResponse()
        {
        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length != 0)
            {
                throw new ArgumentOutOfRangeException("contents", string.Format("length mismatch - expected 0, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }
        }
    }
}
