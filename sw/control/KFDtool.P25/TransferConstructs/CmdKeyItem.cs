using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.TransferConstructs
{
    public class CmdKeyItem
    {
        private int _keysetId;

        private int _sln;

        private int _algorithmId;

        private int _keyId;

        private List<byte> _key;

        public bool UseActiveKeyset { get; set; }

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

        public bool IsKek { get; set; }

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

        public List<byte> Key
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

        public CmdKeyItem()
        {
            Key = new List<byte>();
        }

        public CmdKeyItem(bool useActiveKeyset, int keysetId, int sln, bool isKek, int keyId, int algorithmId, List<byte> key)
        {
            UseActiveKeyset = useActiveKeyset;
            KeysetId = keysetId;
            Sln = sln;
            IsKek = isKek;
            KeyId = keyId;
            AlgorithmId = algorithmId;
            Key = key;
        }

        public override string ToString()
        {
            return string.Format("UseActiveKeyset: {0}, KeysetId: {1}, Sln: {2}, IsKek: {3}, KeyId: {4}, AlgorithmId: {5}, Key: {6}", UseActiveKeyset, KeysetId, Sln, IsKek, KeyId, AlgorithmId, BitConverter.ToString(Key.ToArray()));
        }
    }
}
