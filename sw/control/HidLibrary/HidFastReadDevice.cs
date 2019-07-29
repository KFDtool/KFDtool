using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HidLibrary
{
    public class HidFastReadDevice : HidDevice
    {
        internal HidFastReadDevice(string devicePath, string description = null)
            : base(devicePath, description)
        {
        }

        public HidDeviceData FastRead()
        {
            return FastRead(0);
        }

        public HidDeviceData FastRead(int timeout)
        {
            try
            {
                return ReadData(timeout);
            }
            catch
            {
                return new HidDeviceData(HidDeviceData.ReadStatus.ReadError);
            }
        }

        public void FastRead(ReadCallback callback)
        {
            ReadDelegate readDelegate = FastRead;
            HidAsyncState @object = new HidAsyncState(readDelegate, callback);
            readDelegate.BeginInvoke(HidDevice.EndRead, @object);
        }

        public void FastReadReport(ReadReportCallback callback)
        {
            ReadReportDelegate readReportDelegate = FastReadReport;
            HidAsyncState @object = new HidAsyncState(readReportDelegate, callback);
            readReportDelegate.BeginInvoke(HidDevice.EndReadReport, @object);
        }

        public HidReport FastReadReport(int timeout)
        {
            return new HidReport(base.Capabilities.InputReportByteLength, FastRead(timeout));
        }

        public HidReport FastReadReport()
        {
            return FastReadReport(0);
        }
    }
}
