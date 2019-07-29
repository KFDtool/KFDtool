using KFDtool.Adapter.Device;
using KFDtool.P25.TransferConstructs;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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

            AppDet = new AutoDetection();
            AppDet.DevicesChanged += CheckConnectedDevices;
            AppDet.Start();

            // on load select the P25 Keyload function
            NavigateP25Keyload.IsChecked = true;
            AppView.Content = new Control.P25Keyload();
            UpdateTitle("P25 - Keyload");
        }

        void MainWindow_Closing(object sender, CancelEventArgs e)
        {
            Logger.Info("stopping");

            // have to stop the WMI watcher or a RCW exception will be thrown
            AppDet.Stop();
        }

        private void UpdateTitle(string s)
        {
            this.Title = string.Format("KFDtool {0} [{1}]", Settings.AssemblyInformationalVersion, s);
        }

        private void Navigate_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            MenuItem mi = sender as MenuItem;

            if (mi != null)
            {
                foreach (MenuItem item in P25Menu.Items)
                {
                    item.IsChecked = false;
                }

                foreach (MenuItem item in UtilityMenu.Items)
                {
                    item.IsChecked = false;
                }

                mi.IsChecked = true;

                if (mi.Name == "NavigateP25Keyload")
                {
                    AppView.Content = new Control.P25Keyload();
                    UpdateTitle("P25 - Keyload");
                }
                else if (mi.Name == "NavigateP25KeyErase")
                {
                    AppView.Content = new Control.P25KeyErase();
                    UpdateTitle("P25 - Key Erase");
                }
                else if (mi.Name == "NavigateP25EraseAllKeys")
                {
                    AppView.Content = new Control.P25EraseAllKeys();
                    UpdateTitle("P25 - Erase All Keys");
                }
                else if (mi.Name == "NavigateP25ViewKeyInfo")
                {
                    AppView.Content = new Control.P25ViewKeyInfo();
                    UpdateTitle("P25 - View Key Info");
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
            }
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
                    Settings.Port = string.Empty;

                    lblSelectedDevice.Text = "Selected Device: None";

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
                    if (port == Settings.Port)
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

                string verStr = string.Empty;

                try
                {
                    verStr = Interact.ReadAdapterProtocolVersion(mi.Name);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                if (verStr == "1.0.0")
                {
                    mi.IsChecked = true;

                    Settings.Port = mi.Name;

                    lblSelectedDevice.Text = string.Format("Selected Device: {0}", Settings.Port);
                }
                else
                {
                    MessageBox.Show("Adapter protocol version not compatible", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void Exit_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void About_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show(string.Format("KFDtool Control Application{0}{0}Copyright 2019 Daniel Dugger{0}{0}Version: {1}", Environment.NewLine, Settings.AssemblyInformationalVersion), "About", MessageBoxButton.OK);
        }

        private void Website_MenuItem_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://www.kfdtool.com");
        }
    }
}
