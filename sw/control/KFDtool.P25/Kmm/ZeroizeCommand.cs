using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class ZeroizeCommand : KmmBody
    {
        public override MessageId MessageId
        {
            get
            {
                return MessageId.ZeroizeCommand;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public ZeroizeCommand()
        {
        }

        public override byte[] ToBytes()
        {
            byte[] contents = new byte[0];

            return contents;
        }

        public override void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
