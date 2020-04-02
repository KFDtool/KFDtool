using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.TransferConstructs
{
    public class RspRsiInfo
    {
        private int _rsi;
        private int _mn;
        private int _status;

        public int RSI
        {
            get
            {
                return _rsi;
            }
            set
            {
                if (value < 0x00 || value > 0xFFFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _rsi = value;
            }
        }

        public int MN
        {
            get
            {
                return _mn;
            }
            set
            {
                if (value < 0x00 || value > 0xFFFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _mn = value;
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
                if (value < 0x00 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _status = value;
            }
        }

        public RspRsiInfo()
        {
        }
    }
}
