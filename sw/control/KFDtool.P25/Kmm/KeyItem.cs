using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class KeyItem
    {
        private int _sln;

        private int _keyId;

        private byte[] _key;

        public int SLN
        {
            get
            {
                return _sln;
            }
            set
            {
                if (value < 0 || value > 0xFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _sln = value;
            }
        }

        public int KeyId
        {
            get
            {
                return _keyId;
            }
            set
            {
                if (value < 0 || value > 0xFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keyId = value;
            }
        }

        public byte[] Key
        {
            get
            {
                return _key;
            }
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException();
                }

                _key = value;
            }
        }

        public bool Erase { get; set; }

        public KeyItem()
        {
            Erase = false;
        }

        public byte[] ToBytes()
        {
            byte[] contents = new byte[5 + Key.Length];

            /* key format */
            if (Erase)
            {
                contents[0] = 0x20;
            }
            else
            {
                contents[0] = 0x00;
            }

            /* sln */
            contents[1] = (byte)(SLN >> 8);
            contents[2] = (byte)SLN;

            /* key id */
            contents[3] = (byte)(KeyId >> 8);
            contents[4] = (byte)KeyId;

            /* key */
            Array.Copy(Key, 0, contents, 5, Key.Length);

            return contents;
        }

        public void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
