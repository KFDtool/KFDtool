using System;
using System.Collections;
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

        public bool KEK { get; set; }

        public bool Erase { get; set; }

        public KeyItem()
        {
            KEK = false;
            Erase = false;
        }

        public byte[] ToBytes()
        {
            byte[] contents = new byte[5 + Key.Length];

            /* key format */
            BitArray keyFormat = new BitArray(8, false);
            keyFormat.Set(7, KEK);
            keyFormat.Set(5, Erase);
            keyFormat.CopyTo(contents, 0);

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

        public void Parse(byte[] contents, int keyLength)
        {
            if (contents.Length < 5)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected at least 5, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            int expectedConetentsLength =  5 + keyLength;

            if (contents.Length != expectedConetentsLength)
            {
                throw new ArgumentOutOfRangeException(string.Format("key and key length mismatch - expected {0}, got {1} - {2}", expectedConetentsLength, contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* key format */
            KEK = (contents[0] & 0x80) == 1;
            Erase = (contents[0] & 0x20) == 1;

            /* sln */
            SLN |= contents[1] << 8;
            SLN |= contents[2];

            /* key id */
            KeyId |= contents[3] << 8;
            KeyId |= contents[4];

            /* key */
            Key = new byte[keyLength];
            Array.Copy(contents, 5, Key, 0, keyLength);
        }
    }
}
