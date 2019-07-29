using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class InventoryResponseListActiveKeys : KmmBody
    {
        public int InventoryMarker { get; private set; }

        public int NumberOfItems { get; private set; }

        public List<KeyInfo> Keys { get; private set; }

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
                return InventoryType.ListActiveKeys;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public InventoryResponseListActiveKeys()
        {
            Keys = new List<KeyInfo>();
        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length < 6)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected at least 6, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* inventory type */
            if (contents[0] != (byte)InventoryType)
            {
                throw new Exception("inventory type mismatch");
            }

            /* inventory marker */
            InventoryMarker |= contents[1] << 16;
            InventoryMarker |= contents[2] << 8;
            InventoryMarker |= contents[3];

            /* number of items */
            NumberOfItems |= contents[4] << 8;
            NumberOfItems |= contents[5];

            /* items */
            if ((NumberOfItems == 0) && (contents.Length == 6))
            {
                return;
            }
            else if (((NumberOfItems * 6) % (contents.Length - 6)) == 0)
            {
                for (int i = 0; i < NumberOfItems; i++)
                {
                    byte[] info = new byte[6];
                    Array.Copy(contents, 6 + (i * 6), info, 0, 6);
                    KeyInfo info2 = new KeyInfo();
                    info2.Parse(info);
                    Keys.Add(info2);
                }
            }
            else
            {
                throw new Exception("number of items field and length mismatch");
            }
        }
    }
}
