using KFDtool.P25.TransferConstructs;
using KFDtool.Shared;
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
    /// Interaction logic for UtilAdapterSelfTest.xaml
    /// </summary>
    public partial class UtilAdapterSelfTest : UserControl
    {
        public UtilAdapterSelfTest()
        {
            InitializeComponent();
        }

        private void ST_Button_Click(object sender, RoutedEventArgs e)
        {
            if (Settings.SelectedDevice.DeviceType != BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                MessageBox.Show("Please select the KFDtool device type to perform this function", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBoxResult messageBoxResult = MessageBox.Show(string.Format("Radio must be disconnected{0}{0}Continue?", Environment.NewLine), "Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning);

            if (messageBoxResult == MessageBoxResult.No)
            {
                return;
            }

            string result = string.Empty;

            try
            {
                result = Interact.SelfTest(Settings.SelectedDevice);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (result == string.Empty)
            {
                MessageBox.Show("Self Test Passed", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
            {
                MessageBox.Show(string.Format("Error -- {0}", result), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void BSL_Button_Click(object sender, RoutedEventArgs e)
        {
            if (Settings.SelectedDevice.DeviceType != BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                MessageBox.Show("Please select the KFDtool device type to perform this function", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBoxResult messageBoxResult = MessageBox.Show(string.Format("Adapter will be put in BSL mode{0}{0}Continue?", Environment.NewLine), "Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning);

            if (messageBoxResult == MessageBoxResult.No)
            {
                return;
            }

            try
            {
                Interact.EnterBslMode(Settings.SelectedDevice);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBox.Show("Switching to BSL mode", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Info_Button_Click(object sender, RoutedEventArgs e)
        {
            if (Settings.SelectedDevice.DeviceType != BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                MessageBox.Show("Please select the KFDtool device type to perform this function", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            string apVersion = string.Empty;
            string fwVersion = string.Empty;
            string uniqueId = string.Empty;
            string model = string.Empty;
            string hwRev = string.Empty;
            string serialNum = string.Empty;

            try
            {
                apVersion = Interact.ReadAdapterProtocolVersion(Settings.SelectedDevice);
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

            MessageBox.Show(string.Format("Adapter Protocol: {1}{0}Firmware Version: {2}{0}Unique ID: {3}{0}Model: {4}{0}Hardware Revision: {5}{0}Serial Number: {6}", Environment.NewLine, apVersion, fwVersion, uniqueId, model, hwRev, serialNum), "Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Detect_Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Interact.CheckTargetMrConnection(Settings.SelectedDevice);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBox.Show("Detected P25 MR", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
