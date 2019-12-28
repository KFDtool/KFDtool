using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    /* TIA 102.AACA-A 10.2.19 and TIA 102.AACD-A 3.9.2.17 */
    public class ModifyKeyCommand : KmmBody
    {
        private int _keysetId;

        private int _algorithmId;

        public int KeysetId
        {
            get
            {
                return _keysetId;
            }
            set
            {
                if (value < 0 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keysetId = value;
            }
        }

        public int AlgorithmId
        {
            get
            {
                return _algorithmId;
            }
            set
            {
                if (value < 0 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _algorithmId = value;
            }
        }

        public List<KeyItem> KeyItems { get; set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.ModifyKeyCommand;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public ModifyKeyCommand()
        {
            KeyItems = new List<KeyItem>();
        }

        public override byte[] ToBytes()
        {
            List<byte> keys = new List<byte>();

            foreach (KeyItem key in KeyItems)
            {
                keys.AddRange(key.ToBytes());
            }

            List<byte> contents = new List<byte>();

            /* decryption instruction format */
            contents.Add(0x00);

            /* extended decryption instruction format */
            contents.Add(0x00);

            /* algorithm id */
            contents.Add(0x80);

            /* key id */
            contents.Add(0x00);
            contents.Add(0x00);

            /* keyset id */
            contents.Add((byte)KeysetId);

            /* algorithm id */
            contents.Add((byte)AlgorithmId);

            /* key length */
            contents.Add((byte)KeyItems[0].Key.Length);

            /* number of keys */
            contents.Add((byte)KeyItems.Count);

            /* keys */
            contents.AddRange(keys);

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length < 9)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected at least 9, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* keyset id */
            KeysetId = contents[5];

            /* algorithm id */
            AlgorithmId = contents[6];

            /* key length */
            int keyLength = contents[7];

            /* number of keys */
            int keyCount = contents[8];

            /* keys */
            if ((keyCount == 0) && (contents.Length == 9))
            {
                return;
            }
            else if (((keyCount * (5 + keyLength)) % (contents.Length - 9)) == 0)
            {
                for (int i = 0; i < keyCount; i++)
                {
                    byte[] item = new byte[5 + keyLength];
                    Array.Copy(contents, 9 + (i * (5 + keyLength)), item, 0, 5 + keyLength);
                    KeyItem item2 = new KeyItem();
                    item2.Parse(item, keyLength);
                    KeyItems.Add(item2);
                }
            }
            else
            {
                throw new Exception("number of keys field and length mismatch");
            }
        }
    }
}
