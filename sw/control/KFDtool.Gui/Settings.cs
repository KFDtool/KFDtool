using KFDtool.P25.TransferConstructs;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Gui
{
    class Settings
    {
        public static string AssemblyVersion { get; private set; }

        public static string AssemblyInformationalVersion { get; private set; }

        public static string InProgressScreen { get; set; }

        public static BaseDevice SelectedDevice { get; set; }

        static Settings()
        {
            AssemblyVersion = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            AssemblyInformationalVersion = FileVersionInfo.GetVersionInfo(Assembly.GetExecutingAssembly().Location).ProductVersion;
            InProgressScreen = string.Empty;

            SelectedDevice = new BaseDevice();

            SelectedDevice.TwiKfdtoolDevice = new TwiKfdtoolDevice();
            SelectedDevice.TwiKfdtoolDevice.ComPort = string.Empty;

            SelectedDevice.DliIpDevice = new DliIpDevice();
            SelectedDevice.DliIpDevice.Protocol = DliIpDevice.ProtocolOptions.UDP;
            SelectedDevice.DliIpDevice.Hostname = "192.168.128.1";
            SelectedDevice.DliIpDevice.Port = 49644;
            SelectedDevice.DliIpDevice.Variant = DliIpDevice.VariantOptions.Motorola;

            SelectedDevice.DeviceType = BaseDevice.DeviceTypeOptions.TwiKfdtool;
        }
    }
}
