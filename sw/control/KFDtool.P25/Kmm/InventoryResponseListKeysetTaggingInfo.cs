using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class InventoryResponseListKeysetTaggingInfo : KmmBody
    {
        public List<KeysetItem> KeysetItems { get; private set; }

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
                return InventoryType.ListKeysetTaggingInfo;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public InventoryResponseListKeysetTaggingInfo()
        {
            KeysetItems = new List<KeysetItem>();
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
            else
            {
                int pos = 3;
                // Loop through each item
                for (int i = 0; i < NumberOfItems; i++)
                {
                    KeysetItem item = new KeysetItem();

                    /* keyset format */
                    string ksetType;
                    if ((contents[pos] & (1 << 7)) != 0) { ksetType = "KEK"; }
                    else { ksetType = "TEK"; }
                    item.KeysetType = ksetType;

                    // detect presense of 3 octet optional reserved field
                    bool reserved = (contents[pos] & (1 << 6)) != 0;

                    // detect presense of 5 octet optional datetime field
                    bool datetime = (contents[pos] & (1 << 5)) != 0;

                    int ksetNameSize = contents[pos] & 0x0F;

                    // iterate past the keyset format field
                    pos++;

                    // get the keyset id
                    item.KeysetId = contents[pos];
                    pos++;

                    // iterate past the deprecated reserved field
                    pos++;

                    if (reserved)
                    {
                        item.ReservedField |= contents[i + pos + 1] << 16;
                        item.ReservedField |= contents[i + pos + 2] << 8;
                        item.ReservedField |= contents[i + pos + 3];
                        pos += 3;
                    }

                    if (datetime)
                    {
                        int mon, day, year, hour, min, sec = new int();
                        //mmmm ddddd yyyyyyy
                        //0b 0000111 100001111 == 0x0F0F
                        mon = contents[i + pos + 1] >> 4;
                        day = (contents[i + pos + 1] & 0x0F) << 1;
                        day |= contents[i + pos + 2] >> 7;
                        year = contents[i + pos + 2] & 0x7F;
                        year += 2000;
                        //hhhhh mmmmmm ssssss 0000000
                        hour = contents[i + pos + 3] >> 3;
                        min = (contents[i + pos + 3] & 0x07) << 3;
                        min |= contents[i + pos + 4] >> 5;
                        sec = (contents[i + pos + 4] & 0x1F) << 1;
                        sec |= contents[i + pos + 5] >> 7;

                        item.ActivationDateTime = new System.DateTime(year, mon, day, hour, min, sec);

                        pos += 5;
                    }

                    byte[] keysetNameBytes = new byte[ksetNameSize];
                    Array.Copy(contents, pos, keysetNameBytes, 0, ksetNameSize);
                    item.KeysetName = System.Text.Encoding.ASCII.GetString(keysetNameBytes);
                    pos += ksetNameSize;

                    KeysetItems.Add(item);
                }

            }
        }
    }
}
