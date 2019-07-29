using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace HidLibrary
{
    internal class HidDeviceEventMonitor
    {
        public delegate void InsertedEventHandler();

        public delegate void RemovedEventHandler();

        private readonly HidDevice _device;

        private bool _wasConnected;

        public event InsertedEventHandler Inserted;

        public event RemovedEventHandler Removed;

        public HidDeviceEventMonitor(HidDevice device)
        {
            _device = device;
        }

        public void Init()
        {
            Action action = DeviceEventMonitor;
            action.BeginInvoke(DisposeDeviceEventMonitor, action);
        }

        private void DeviceEventMonitor()
        {
            bool isConnected = _device.IsConnected;
            if (isConnected != _wasConnected)
            {
                if (isConnected && this.Inserted != null)
                {
                    this.Inserted();
                }
                else if (!isConnected && this.Removed != null)
                {
                    this.Removed();
                }
                _wasConnected = isConnected;
            }
            Thread.Sleep(500);
            if (_device.MonitorDeviceEvents)
            {
                Init();
            }
        }

        private static void DisposeDeviceEventMonitor(IAsyncResult ar)
        {
            ((Action)ar.AsyncState).EndInvoke(ar);
        }
    }
}
