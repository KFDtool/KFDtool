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
            List<byte> contents = new List<byte>();

            /* inventory type */
            contents.Add((byte)InventoryType);

            /* number of items */
            contents.Add((byte)((RsiItems.Count >> 8) & 0xFF));
            contents.Add((byte)(RsiItems.Count & 0xFF));

            /* items */
            foreach (RsiItem item in RsiItems)
            {
                contents.AddRange(item.ToBytes());
            }

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
