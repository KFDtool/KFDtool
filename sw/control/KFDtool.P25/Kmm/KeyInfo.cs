using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class KeyInfo
    {
        private int _keySetId;

        private int _sln;

        private int _algorithmId;

        private int _keyId;

        public int KeySetId
        {
            get
            {
                return _keySetId;
            }
            set
            {
                if (value < 0 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keySetId = value;
            }
        }

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

        public KeyInfo()
        {
        }

        public byte[] ToBytes()
        {
            byte[] contents = new byte[6];

            /* keyset id */
            contents[0] = (byte)KeySetId;

            /* sln */
            contents[1] = (byte)(SLN >> 8);
            contents[2] = (byte)SLN;

            /* algorithm id */
            contents[3] = (byte)AlgorithmId;

            /* key id */
            contents[4] = (byte)(KeyId >> 8);
            contents[5] = (byte)KeyId;

            return contents;
        }

        public void Parse(byte[] contents)
        {
            if (contents.Length != 6)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected 6, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* keyset id */
            KeySetId = contents[0];

            /* sln */
            SLN |= contents[1] << 8;
            SLN |= contents[2];

            /* algorithm id */
            AlgorithmId = contents[3];

            /* key id */
            KeyId |= contents[4] << 8;
            KeyId |= contents[5];
        }
    }
}
