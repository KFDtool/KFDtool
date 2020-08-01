using KFDtool.Adapter.Protocol.Adapter;
using KFDtool.P25.ThreeWire;
using KFDtool.P25.TransferConstructs;
using System;
using System.Collections.Generic;
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

namespace KFDtool.Gui.Control
{
    /// <summary>
    /// Interaction logic for P25MrEmulator.xaml
    /// </summary>
    public partial class P25MrEmulator : UserControl
    {
        private static NLog.Logger Log = NLog.LogManager.GetCurrentClassLogger();

        AdapterProtocol ap;

        ThreeWireProtocol twp;

        public P25MrEmulator()
        {
            InitializeComponent();

            StopEmulation(); // set up button states
        }

        private void StartEmulation()
        {
            Settings.ScreenInProgress = true;

            StartButton.IsEnabled = false;
            StopButton.IsEnabled = true;

            TextArea.Text = string.Empty;
        }

        private void StopEmulation()
        {
            Settings.ScreenInProgress = false;

            StartButton.IsEnabled = true;
            StopButton.IsEnabled = false;
        }

        private void ErrorEmulation(string message)
        {
            this.Dispatcher.Invoke(() =>
            {
                MessageBox.Show(string.Format("Error -- {0}", message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                StopEmulation();
            });
        }

        private void Start_Button_Click(object sender, RoutedEventArgs e)
        {
            if (Settings.SelectedDevice.DeviceType != BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                ErrorEmulation(string.Format("The device type {0} does not support MR emulation", Settings.SelectedDevice.DeviceType.ToString()));
                return;
            }

            if (Settings.SelectedDevice.TwiKfdtoolDevice.ComPort == string.Empty)
            {
                ErrorEmulation("No device selected");
                return;
            }

            StartEmulation();

            Task.Run(() =>
            {
                ap = null;

                try
                {
                    ap = new AdapterProtocol(Settings.SelectedDevice.TwiKfdtoolDevice.ComPort);

                    ap.Open();

                    ap.Clear();

                    twp = new ThreeWireProtocol(ap);

                    twp.StatusChanged += OnProgressUpdated;

                    twp.MrRunProducer();
                }
                catch (Exception ex)
                {
                    ErrorEmulation(ex.Message);
                }
                finally
                {
                    try
                    {
                        if (ap != null)
                        {
                            ap.Close();
                        }
                    }
                    catch (System.IO.IOException ex)
                    {
                        Log.Warn("could not close serial port: {0}", ex.Message);
                    }
                }
            });
        }

        private void OnProgressUpdated(object sender, EventArgs e)
        {
            this.Dispatcher.Invoke(() =>
            {
                TextArea.Text = twp.Status;
                TextArea.ScrollToEnd();
            });
        }

        private void Stop_Button_Click(object sender, RoutedEventArgs e)
        {
            if (ap != null)
            {
                ap.Cancel();
            }

            if (ap != null)
            {
                ap.Close();
            }

            StopEmulation();
        }
    }
}
