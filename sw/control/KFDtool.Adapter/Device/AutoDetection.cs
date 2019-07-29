using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Adapter.Device
{
    public class AutoDetection
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        private ManagementEventWatcher Watcher;
        private bool FirstRun;

        public List<string> Devices { get; private set; }
        public event EventHandler DevicesChanged;

        public AutoDetection()
        {
            FirstRun = false;

            Devices = new List<string>();

            Watcher = new ManagementEventWatcher();
            WqlEventQuery query = new WqlEventQuery("SELECT * FROM Win32_DeviceChangeEvent");
            Watcher.EventArrived += new EventArrivedEventHandler(USBChangedEvent);
            Watcher.Query = query;
        }

        public void Start()
        {
            Logger.Debug("auto detection starting");

            FirstRun = true;

            UpdateDevices();
            Watcher.Start();

            Logger.Debug("auto detection started");
        }

        public void Stop()
        {
            Logger.Debug("auto detection stopping");

            Watcher.Stop();

            Logger.Debug("auto detection stopped");
        }

        private void USBChangedEvent(object sender, EventArrivedEventArgs e)
        {
            Logger.Debug("device added/removed");

            UpdateDevices();
        }

        private void UpdateDevices()
        {
            List<string> detDev = ManualDetection.DetectConnectedAppDevices();

            detDev.Sort();

            foreach (string dev in Devices)
            {
                Logger.Trace("current device: {0}", dev);
            }

            foreach (string dev in detDev)
            {
                Logger.Trace("new device: {0}", dev);
            }

            if (FirstRun || !detDev.SequenceEqual(Devices))
            {
                Logger.Debug("change in device list");

                FirstRun = false;

                Devices.Clear();
                Devices.AddRange(detDev);
                Devices.Sort();
                OnDevicesChanged(new EventArgs());
            }
        }

        private void OnDevicesChanged(EventArgs e)
        {
            EventHandler handler = DevicesChanged;
            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}
