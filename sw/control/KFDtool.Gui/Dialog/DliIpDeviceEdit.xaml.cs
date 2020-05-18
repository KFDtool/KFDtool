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
using System.Windows.Shapes;

namespace KFDtool.Gui.Dialog
{
    /// <summary>
    /// Interaction logic for DliIpDeviceEdit.xaml
    /// </summary>
    public partial class DliIpDeviceEdit : Window
    {
        public DliIpDeviceEdit()
        {
            InitializeComponent();

            // protocol

            if (Settings.SelectedDevice.DliIpDevice.Protocol == DliIpDevice.ProtocolOptions.UDP)
            {
                PcbProtocol.SelectedItem = PcbiProtocolUdp;
            }
            else
            {
                throw new Exception("unknown DliIpProtocol setting");
            }

            // hostname

            TbHostname.Text = Settings.SelectedDevice.DliIpDevice.Hostname;

            // port

            TbPort.Text = Settings.SelectedDevice.DliIpDevice.Port.ToString();

            // variant

            if (Settings.SelectedDevice.DliIpDevice.Variant == DliIpDevice.VariantOptions.Standard)
            {
                PcbVariant.SelectedItem = PcbiVariantStandard;
            }
            else if (Settings.SelectedDevice.DliIpDevice.Variant == DliIpDevice.VariantOptions.Motorola)
            {
                PcbVariant.SelectedItem = PcbiVariantMotorola;
            }
            else
            {
                throw new Exception("unknown DliIpVariant setting");
            }
        }

        private void Save_Button_Click(object sender, RoutedEventArgs e)
        {
            // protocol

            if (PcbProtocol.SelectedItem == PcbiProtocolUdp)
            {
                Settings.SelectedDevice.DliIpDevice.Protocol = DliIpDevice.ProtocolOptions.UDP;
            }
            else
            {
                throw new Exception("unknown PcbProtocol selection");
            }

            // hostname

            Settings.SelectedDevice.DliIpDevice.Hostname = TbHostname.Text;

            // port

            int port;

            if (int.TryParse(TbPort.Text, out port))
            {
                if (port >= 0 && port <= 65535)
                {
                    Settings.SelectedDevice.DliIpDevice.Port = port;
                }
                else
                {
                    MessageBox.Show("Valid port range is 0-65535", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }
            else
            {
                MessageBox.Show("Could not parse port", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // variant

            if (PcbVariant.SelectedItem == PcbiVariantStandard)
            {
                Settings.SelectedDevice.DliIpDevice.Variant = DliIpDevice.VariantOptions.Standard;
            }
            else if (PcbVariant.SelectedItem == PcbiVariantMotorola)
            {
                Settings.SelectedDevice.DliIpDevice.Variant = DliIpDevice.VariantOptions.Motorola;
            }
            else
            {
                throw new Exception("unknown PcbVariant selection");
            }

            this.Close();
        }

        private void Cancel_Button_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
