using KFDtool.Adapter.Device;
using KFDtool.Gui.Dialog;
using KFDtool.P25.TransferConstructs;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace KFDtool.Gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        private AutoDetection AppDet;

        public MainWindow()
        {
            InitializeComponent();

            Logger.Info("starting");

            InitAppDet();

            // on load select the KFDtool type
            SwitchType(TypeTwiKfdtool);

            // on load select the P25 Keyload function
            SwitchScreen(NavigateP25Keyload);
        }

        void MainWindow_Closing(object sender, CancelEventArgs e)
        {
            if (Settings.InProgressScreen != string.Empty)
            {
                UpdateSelectionOnly(Settings.InProgressScreen);
                MessageBox.Show("Unable to exit - please stop the current operation", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                e.Cancel = true;
            }

            Logger.Info("stopping");

            // have to stop the WMI watcher or a RCW exception will be thrown
            StopAppDet();
        }

        private void UpdateTitle(string s)
        {
#if DEBUG
            this.Title = string.Format("KFDtool {0} DEBUG [{1}]", Settings.AssemblyInformationalVersion, s);
#else
            this.Title = string.Format("KFDtool {0} [{1}]", Settings.AssemblyInformationalVersion, s);
#endif
        }

        private void SwitchType(MenuItem mi)
        {
            foreach (MenuItem item in TypeMenu.Items)
            {
                item.IsChecked = false;
            }

            mi.IsChecked = true;

            if (mi.Name == "TypeTwiKfdtool")
            {
                DeviceMenu.Items.Clear();

                Settings.SelectedDevice.DeviceType = BaseDevice.DeviceTypeOptions.TwiKfdtool;

                StartAppDet();
            }
            else if (mi.Name == "TypeDliIp")
            {
                StopAppDet();

                DeviceMenu.Items.Clear();

                Settings.SelectedDevice.DeviceType = BaseDevice.DeviceTypeOptions.DliIp;

                MenuItem DliIpEdit = new MenuItem();
                DliIpEdit.Header = "_[Edit]";
                DliIpEdit.Click += DliIpEdit_MenuItem_Click;

                DeviceMenu.Items.Add(DliIpEdit);

                UpdateDeviceDliIp();
            }
        }

        private void UpdateDeviceDliIp()
        {
            lblSelectedDevice.Text = string.Format(
                "Selected Device - Type: DLI (IP) - Protocol: {0}, Hostname: {1}, Port: {2}, Variant: {3}",
                Settings.SelectedDevice.DliIpDevice.Protocol.ToString(),
                Settings.SelectedDevice.DliIpDevice.Hostname,
                Settings.SelectedDevice.DliIpDevice.Port.ToString(),
                Settings.SelectedDevice.DliIpDevice.Variant.ToString()
            );
        }

        private void Type_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItem mi = sender as MenuItem;

            SwitchType(mi);
        }

        private void DliIpEdit_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            DliIpDeviceEdit wnd = new DliIpDeviceEdit();
            wnd.Owner = this; // for centering in parent window
            wnd.ShowDialog();

            UpdateDeviceDliIp();
        }

        private void Navigate_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItem mi = sender as MenuItem;

            if (mi != null)
            {
                if (Settings.InProgressScreen != string.Empty)
                {
                    UpdateSelectionOnly(Settings.InProgressScreen);
                    MessageBox.Show("Unable to change screens - please stop the current operation", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
                else
                {
                    SwitchScreen(mi);
                }
            }
        }

        private void ClearAllSelections()
        {
            foreach (MenuItem item in P25KfdMenu.Items)
            {
                item.IsChecked = false;
            }

            foreach (MenuItem item in P25MrMenu.Items)
            {
                item.IsChecked = false;
            }

            foreach (MenuItem item in UtilityMenu.Items)
            {
                item.IsChecked = false;
            }
        }

        private void UpdateSelectionOnly(string item)
        {
            ClearAllSelections();

            if (item == "NavigateP25MrEmulator")
            {
                NavigateP25MrEmulator.IsChecked = true;
            }
            else
            {
                Logger.Fatal("unknown item passed to UpdateSelectionOnly - {0}", item);
                Application.Current.Shutdown(1);
            }
        }

        private void SwitchScreen(MenuItem mi)
        {
            ClearAllSelections();

            mi.IsChecked = true;

            if (mi.Name == "NavigateP25Keyload")
            {
                AppView.Content = new Control.P25Keyload();
                UpdateTitle("P25 KFD - Keyload");
            }
            else if (mi.Name == "NavigateP25KeyErase")
            {
                AppView.Content = new Control.P25KeyErase();
                UpdateTitle("P25 KFD - Key Erase");
            }
            else if (mi.Name == "NavigateP25EraseAllKeys")
            {
                AppView.Content = new Control.P25EraseAllKeys();
                UpdateTitle("P25 KFD - Erase All Keys");
            }
            else if (mi.Name == "NavigateP25ViewKeyInfo")
            {
                AppView.Content = new Control.P25ViewKeyInfo();
                UpdateTitle("P25 KFD - View Key Info");
            }
            else if (mi.Name == "NavigateP25ViewKeysetInfo")
            {
                AppView.Content = new Control.P25ViewKeysetInfo();
                UpdateTitle("P25 KFD - View Keyset Info");
            }
            else if (mi.Name == "NavigateP25ViewRsiConfig")
            {
                AppView.Content = new Control.P25ViewRsiConfig();
                UpdateTitle("P25 KFD - RSI Configuration");
            }
            else if (mi.Name == "NavigateP25KmfConfig")
            {
                AppView.Content = new Control.P25KmfConfig();
                UpdateTitle("P25 KFD - KMF Configuration");
            }
            else if (mi.Name == "NavigateP25MrEmulator")
            {
                AppView.Content = new Control.P25MrEmulator();
                UpdateTitle("P25 MR - Emulator");
            }
            else if (mi.Name == "NavigateUtilityFixDesKeyParity")
            {
                AppView.Content = new Control.UtilFixDesKeyParity();
                UpdateTitle("Utility - Fix DES Key Parity");
            }
            else if (mi.Name == "NavigateUtilityUpdateAdapterFirmware")
            {
                AppView.Content = new Control.UtilUpdateAdapterFw();
                UpdateTitle("Utility - Update Adapter Firmware");
            }
            else if (mi.Name == "NavigateUtilityInitializeAdapter")
            {
                AppView.Content = new Control.UtilInitAdapter();
                UpdateTitle("Utility - Initialize Adapter");
            }
            else if (mi.Name == "NavigateUtilityAdapterSelfTest")
            {
                AppView.Content = new Control.UtilAdapterSelfTest();
                UpdateTitle("Utility - Adapter Self Test");
            }
            else
            {
                Logger.Fatal("unknown item passed to SwitchScreen - {0}", mi.Name);
                Application.Current.Shutdown(1);
            }
        }

        private void InitAppDet()
        {
            AppDet = new AutoDetection();
            AppDet.DevicesChanged += CheckConnectedDevices;
        }

        private void StartAppDet()
        {
            AppDet.Start();
        }

        private void StopAppDet()
        {
            AppDet.Stop();
        }

        private void CheckConnectedDevices(object sender, EventArgs e)
        {
            Logger.Debug("device list updated");

            // needed to access UI elements from different thread
            this.Dispatcher.Invoke(() =>
            {
                bool first = true;

                List<string> ports = AppDet.Devices;

                // sort ports lowest to highest (COM6,COM12,COM26)
                ports.Sort();

                DeviceMenu.Items.Clear();

                // no devices detected
                if (ports.Count == 0)
                {
                    Settings.SelectedDevice.TwiKfdtoolDevice.ComPort = string.Empty;

                    lblSelectedDevice.Text = "Selected Device - Type: TWI (KFDtool) - None";

                    MenuItem item = new MenuItem();

                    item.Header = "No devices found";
                    item.IsCheckable = false;
                    item.IsEnabled = false;

                    DeviceMenu.Items.Add(item);
                }

                // one or more devices detected
                foreach (string port in ports)
                {
                    MenuItem item = new MenuItem();

                    item.Name = port;
                    item.Header = port;
                    item.IsCheckable = true;
                    item.Click += Device_MenuItem_Click;

                    DeviceMenu.Items.Add(item);

                    // there was a change in the device list, but the device that was previously selected is still connected
                    if (port == Settings.SelectedDevice.TwiKfdtoolDevice.ComPort)
                    {
                        SelectDevice(item);
                        first = false;
                    }

                    // select the lowest numbered device if there is no device currently selected
                    // or if the device that was previously selected was disconnected
                    if (first)
                    {
                        SelectDevice(item);
                        first = false;
                    }
                }
            });
        }

        private void Device_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItem mi = sender as MenuItem;

            SelectDevice(mi);
        }

        private void SelectDevice(MenuItem mi)
        {
            if (mi != null)
            {
                foreach (MenuItem item in DeviceMenu.Items)
                {
                    item.IsChecked = false;
                }

                mi.IsChecked = true;

                Settings.SelectedDevice.TwiKfdtoolDevice.ComPort = mi.Name;

                string apVerStr = string.Empty;

                try
                {
                    apVerStr = Interact.ReadAdapterProtocolVersion(Settings.SelectedDevice);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                Version apVersion = new Version(apVerStr);

                if (apVersion.Major != 1)
                {
                    MessageBox.Show(string.Format("Adapter protocol version not compatible ({0})", apVerStr), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                string fwVersion = string.Empty;
                string uniqueId = string.Empty;
                string model = string.Empty;
                string hwRev = string.Empty;
                string serialNum = string.Empty;

                try
                {
                    fwVersion = Interact.ReadFirmwareVersion(Settings.SelectedDevice);
                    uniqueId = Interact.ReadUniqueId(Settings.SelectedDevice);
                    model = Interact.ReadModel(Settings.SelectedDevice);
                    hwRev = Interact.ReadHardwareRevision(Settings.SelectedDevice);
                    serialNum = Interact.ReadSerialNumber(Settings.SelectedDevice);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                lblSelectedDevice.Text = string.Format(
                    "Selected Device - Type: TWI (KFDtool) - Port: {0}, Model: {1}, HW: {2}, Serial: {3}, UID: {4}, FW: {5}",
                    Settings.SelectedDevice.TwiKfdtoolDevice.ComPort,
                    model,
                    hwRev,
                    serialNum,
                    uniqueId,
                    fwVersion
                );
            }
        }

        private void Exit_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Manual_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Process.Start("KFDtool_Manual.pdf");
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void Website_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Process.Start("https://www.kfdtool.com");
        }

        private void About_MenuItem_Click(object sender, RoutedEventArgs e)
        {
#if DEBUG
            MessageBox.Show(
                string.Format(
                    "KFDtool Control Application{0}{0}Copyright 2019-2020 Daniel Dugger{0}{0}Version: {1} DEBUG",
                    Environment.NewLine,
                    Settings.AssemblyInformationalVersion
                ),
                "About",
                MessageBoxButton.OK
            );
#else
            MessageBox.Show(
                string.Format(
                    "KFDtool Control Application{0}{0}Copyright 2019-2020 Daniel Dugger{0}{0}Version: {1}",
                    Environment.NewLine,
                    Settings.AssemblyInformationalVersion
                ),
                "About",
                MessageBoxButton.OK
            );
#endif
        }
    }
}
