using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class InventoryCommandListActiveKeys : KmmBody
    {
        private int _inventoryMarker;

        private int _maxKeysRequested;

        public int InventoryMarker
        {
            get
            {
                return _inventoryMarker;
            }
            set
            {
                if (value < 0 || value > 0xFFFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _inventoryMarker = value;
            }
        }

        public int MaxKeysRequested
        {
            get
            {
                return _maxKeysRequested;
            }
            set
            {
                if (value < 0 || value > 0xFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _maxKeysRequested = value;
            }
        }

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
                return InventoryType.ListActiveKeys;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public InventoryCommandListActiveKeys()
        {
        }

        public override byte[] ToBytes()
        {
            byte[] contents = new byte[6];

            /* inventory type */
            contents[0] = (byte)InventoryType;

            /* inventory marker */
            contents[1] = (byte)((InventoryMarker >> 16) & 0xFF);
            contents[2] = (byte)((InventoryMarker >> 8) & 0xFF);
            contents[3] = (byte)(InventoryMarker & 0xFF);

            /* max number of keys requested */
            contents[4] = (byte)((MaxKeysRequested >> 8) & 0xFF);
            contents[5] = (byte)(MaxKeysRequested & 0xFF);

            return contents;
        }

        public override void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
