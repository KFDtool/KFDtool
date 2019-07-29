using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class InventoryResponseListActiveKsetIds : KmmBody
    {
        public int NumberOfItems { get; private set; }

        public List<byte> KsetIds { get; private set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.InventoryResponse;
            }
        }

        public InventoryType InventoryType
        {
            get
            {
                return InventoryType.ListActiveKsetIds;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public InventoryResponseListActiveKsetIds()
        {
            KsetIds = new List<byte>();
        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length < 3)
            {
                throw new Exception(string.Format("length mismatch - expected at least 3, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* inventory type */
            if (contents[0] != (byte)InventoryType)
            {
                throw new Exception("inventory type mismatch");
            }

            /* number of items */
            NumberOfItems |= contents[1] << 8;
            NumberOfItems |= contents[2];

            /* items */
            if ((NumberOfItems == 0) && (contents.Length == 3))
            {
                return;
            }
            else if (NumberOfItems == (contents.Length - 3))
            {
                for (int i = 0; i < NumberOfItems; i++)
                {
                    KsetIds.Add(contents[3 + i]);
                }
            }
            else
            {
                throw new Exception("number of items field and length mismatch");
            }
        }
    }
}
