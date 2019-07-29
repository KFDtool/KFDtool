using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HidLibrary
{
    public class HidReport
    {
        private byte _reportId;

        private byte[] _data = new byte[0];

        private readonly HidDeviceData.ReadStatus _status;

        public bool Exists
        {
            get;
            private set;
        }

        public HidDeviceData.ReadStatus ReadStatus => _status;

        public byte ReportId
        {
            get
            {
                return _reportId;
            }
            set
            {
                _reportId = value;
                Exists = true;
            }
        }

        public byte[] Data
        {
            get
            {
                return _data;
            }
            set
            {
                _data = value;
                Exists = true;
            }
        }

        public HidReport(int reportSize)
        {
            Array.Resize(ref _data, reportSize - 1);
        }

        public HidReport(int reportSize, HidDeviceData deviceData)
        {
            _status = deviceData.Status;
            Array.Resize(ref _data, reportSize - 1);
            if (deviceData.Data != null)
            {
                if (deviceData.Data.Length != 0)
                {
                    _reportId = deviceData.Data[0];
                    Exists = true;
                    if (deviceData.Data.Length > 1)
                    {
                        int length = reportSize - 1;
                        if (deviceData.Data.Length < reportSize - 1)
                        {
                            length = deviceData.Data.Length;
                        }
                        Array.Copy(deviceData.Data, 1, _data, 0, length);
                    }
                }
                else
                {
                    Exists = false;
                }
            }
            else
            {
                Exists = false;
            }
        }

        public byte[] GetBytes()
        {
            byte[] array = null;
            Array.Resize(ref array, _data.Length + 1);
            array[0] = _reportId;
            Array.Copy(_data, 0, array, 1, _data.Length);
            return array;
        }
    }
}
