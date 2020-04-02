using KFDtool.P25.Constant;
using KFDtool.P25.Generator;
using KFDtool.P25.TransferConstructs;
using KFDtool.P25.Kmm;
using KFDtool.P25.Validator;
using KFDtool.Shared;
using System;
using System.Collections.Generic;
using System.Globalization;
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
    /// Interaction logic for P25KmfConfig.xaml
    /// </summary>
    public partial class P25KmfConfig : UserControl
    {
        public P25KmfConfig()
        {
            InitializeComponent();
        }

        private void KmfRsiDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKmfRsiDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtKmfRsiDec.Text, out num))
                {
                    txtKmfRsiHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtKmfRsiHex.Text = string.Empty;
                }
            }
        }

        private void KmfRsiHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKmfRsiHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtKmfRsiHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtKmfRsiDec.Text = num.ToString();
                }
                else
                {
                    txtKmfRsiDec.Text = string.Empty;
                }
            }
        }

        private void MnpDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtMnpDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtMnpDec.Text, out num))
                {
                    txtMnpHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtMnpHex.Text = string.Empty;
                }

                //UpdateType();
            }
        }

        private void MnpHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtMnpHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtMnpHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtMnpDec.Text = num.ToString();
                }
                else
                {
                    txtMnpDec.Text = string.Empty;
                }

                //UpdateType();
            }
        }

        private void View_MNP_Click(object sender, RoutedEventArgs e)
        {
            int mnp = -1;
            try
            {
                mnp = Interact.ViewMnp(Settings.Port);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBox.Show("Message Number Period: " + mnp, "Information", MessageBoxButton.OK, MessageBoxImage.Information);

        }

        private void View_KmfRsi_Click(object sender, RoutedEventArgs e)
        {
            // First get KMF RSI
            try
            {
                int rsi = new int();
                rsi = Interact.ViewKmfRsi(Settings.Port);
                //MessageBox.Show("KMF RSI: " + result + " (0x" + string.Format("{0:X}", result) + ")", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
                txtKmfRsiDec.Text = rsi.ToString();
                txtKmfRsiHex.Text = string.Format("{0:X}", rsi);

                // Next get MNP for KMF
                int mnp = -1;
                try
                {
                    mnp = Interact.ViewMnp(Settings.Port);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                txtMnpDec.Text = mnp.ToString();
                txtMnpHex.Text = string.Format("{0:X}", mnp);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }


        }

        private void Load_Config_Click(object sender, RoutedEventArgs e)
        {
            int kmfRsi = 0;
            int mnp = 0;

            try
            {
                kmfRsi = Convert.ToInt32(txtKmfRsiHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing KMF RSI", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                mnp = Convert.ToInt32(txtMnpHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing MNP", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Ensure valid KMF RSI value
            if ((kmfRsi > 9999999) || (kmfRsi < 1))
            {
                MessageBox.Show("Invalid KMF RSI - must be between 1 and 9,999,999", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Ensure valid MNP value
            if ((mnp > 65535) || (mnp < 0))
            {
                MessageBox.Show("Invalid MNP - must be between 0 and 65,535", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                RspRsiInfo temp = new RspRsiInfo();
                temp = Interact.LoadConfig(Settings.Port, kmfRsi, mnp);
                MessageBox.Show("Config Loaded Successfully - RSI: " + temp.RSI + ", Message Number: " + temp.MN + ", Status: " + temp.Status, "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            //MessageBox.Show("Config Loaded Successfully", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }
        
    }
}
