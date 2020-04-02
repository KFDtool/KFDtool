using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.TransferConstructs
{
    public class RspChangeoverInfo
    {
        private int _keysetIdSuperseded;
        private int _keysetIdActivated;

        public int KeysetIdSuperseded
        {
            get
            {
                return _keysetIdSuperseded;
            }
            set
            {
                if (value < 0x00 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keysetIdSuperseded = value;
            }
        }

        public int KeysetIdActivated
        {
            get
            {
                return _keysetIdActivated;
            }
            set
            {
                if (value < 0x00 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keysetIdActivated = value;
            }
        }

        public RspChangeoverInfo()
        {
        }
    }
}
