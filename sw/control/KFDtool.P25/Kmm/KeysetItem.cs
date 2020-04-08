using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class KeysetItem
    {
        private int _keysetId;
        public string KeysetName { get; set; }
        public string KeysetType { get; set; }
        public DateTime ActivationDateTime { get; set; }
        private int _reservedField;

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

        public int ReservedField
        {
            get
            {
                return _reservedField;
            }
            set
            {
                if (value < 0 || value > 0xFFFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _reservedField = value;
            }
        }

        public byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
