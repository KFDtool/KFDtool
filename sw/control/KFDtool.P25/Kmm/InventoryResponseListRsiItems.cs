using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class InventoryResponseListRsiItems : KmmBody
    {
        public List<RsiItem> RsiItems { get; private set; }

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
                return InventoryType.ListRsiItems;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public InventoryResponseListRsiItems()
        {
            RsiItems = new List<RsiItem>();
        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length < 2)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected at least 2, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* inventory type */
            if (contents[0] != (byte)InventoryType)
            {
                throw new Exception("inventory type mismatch");
            }

            /* number of items */
            int NumberOfItems = 0;
            NumberOfItems |= contents[1] << 8;
            NumberOfItems |= contents[2];

            /* items */
            if ((NumberOfItems == 0) && (contents.Length == 3))
            {
                return;
            }
            else if (((NumberOfItems * 5) % (contents.Length - 3)) == 0)
            {
                for (int i = 0; i < NumberOfItems; i++)
                {
                    byte[] info = new byte[5];
                    Array.Copy(contents, 3 + (i * 5), info, 0, 5);
                    RsiItem info2 = new RsiItem();
                    info2.Parse(info);
                    RsiItems.Add(info2);
                }
            }
            else
            {
                throw new Exception("number of items field and length mismatch");
            }
        }
    }
}
