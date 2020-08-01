using KFDtool.Container;
using KFDtool.P25.TransferConstructs;
using System.Diagnostics;
using System.Reflection;

namespace KFDtool.Gui
{
    class Settings
    {
        public static string AssemblyVersion { get; private set; }

        public static string AssemblyInformationalVersion { get; private set; }

        public static string ScreenCurrent { get; set; }

        public static bool ScreenInProgress { get; set; }

        public static bool ContainerOpen { get; set; }

        public static bool ContainerSaved { get; set; }

        public static string ContainerPath { get; set; }

        public static byte[] ContainerKey { get; set; }

        public static OuterContainer ContainerOuter { get; set; }

        public static InnerContainer ContainerInner { get; set; }

        public static BaseDevice SelectedDevice { get; set; }

        static Settings()
        {
            AssemblyVersion = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            AssemblyInformationalVersion = FileVersionInfo.GetVersionInfo(Assembly.GetExecutingAssembly().Location).ProductVersion;
            ScreenCurrent = string.Empty;
            ScreenInProgress = false;
            ContainerOpen = false;
            ContainerSaved = false;
            ContainerPath = string.Empty;
            ContainerKey = null;
            ContainerInner = null;
            ContainerOuter = null;

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
