using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HidLibrary
{
    public class HidEnumerator : IHidEnumerator
    {
        public bool IsConnected(string devicePath)
        {
            return HidDevices.IsConnected(devicePath);
        }

        public IHidDevice GetDevice(string devicePath)
        {
            return HidDevices.GetDevice(devicePath);
        }

        public IEnumerable<IHidDevice> Enumerate()
        {
            return from d in HidDevices.Enumerate()
                   select (d);
        }

        public IEnumerable<IHidDevice> Enumerate(string devicePath)
        {
            return from d in HidDevices.Enumerate(devicePath)
                   select (d);
        }

        public IEnumerable<IHidDevice> Enumerate(int vendorId, params int[] productIds)
        {
            return from d in HidDevices.Enumerate(vendorId, productIds)
                   select (d);
        }

        public IEnumerable<IHidDevice> Enumerate(int vendorId)
        {
            return from d in HidDevices.Enumerate(vendorId)
                   select (d);
        }
    }

}
