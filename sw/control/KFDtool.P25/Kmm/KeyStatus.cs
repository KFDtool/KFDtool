using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    /* TIA 102.AACA-A 10.2.26 */
    public class KeyStatus
    {
        private int _algorithmId;

        private int _keyId;

        private int _status;

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

        public int Status
        {
            get
            {
                return _status;
            }
            set
            {
                if (value < 0 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _status = value;
            }
        }

        public KeyStatus()
        {
        }

        public byte[] ToBytes()
        {
            byte[] contents = new byte[4];

            /* algorithm id */
            contents[0] = (byte)AlgorithmId;

            /* key id */
            contents[1] = (byte)(KeyId >> 8);
            contents[2] = (byte)KeyId;

            /* keyset id */
            contents[3] = (byte)Status;

            return contents;
        }

        public void Parse(byte[] contents)
        {
            if (contents.Length != 4)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected 4, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* algorithm id */
            AlgorithmId = contents[0];

            /* key id */
            KeyId |= contents[1] << 8;
            KeyId |= contents[2];

            /* status */
            Status = contents[3];
        }
    }
}
