using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.TransferConstructs
{
    public class RspKeyInfo
    {
        private int _keysetId;

        private int _sln;

        private int _algorithmId;

        private int _keyId;

        public int KeysetId
        {
            get
            {
                return _keysetId;
            }
            set
            {
                if (value < 0x00 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keysetId = value;
            }
        }

        public int Sln
        {
            get
            {
                return _sln;
            }
            set
            {
                if (value < 0x0000 || value > 0xFFFF)
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
                if (value < 0x00 || value > 0xFF)
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
                if (value < 0x0000 || value > 0xFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keyId = value;
            }
        }

        public RspKeyInfo()
        {
        }
    }
}
