using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class InventoryCommandListKmfRsi : KmmBody
    {
        public override MessageId MessageId
        {
            get
            {
                return MessageId.InventoryCommand;
            }
        }

        public InventoryType InventoryType
        {
            get
            {
                return InventoryType.ListKmfRsi;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public InventoryCommandListKmfRsi()
        {
        }

        public override byte[] ToBytes()
        {
            byte[] contents = new byte[1];

            /* inventory type */
            contents[0] = (byte)InventoryType;

            return contents;
        }

        public override void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
