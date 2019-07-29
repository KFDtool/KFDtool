using KFDtool.P25.TransferConstructs;
using KFDtool.P25.Validator;
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
    /// Interaction logic for P25KeyErase.xaml
    /// </summary>
    public partial class P25KeyErase : UserControl
    {
        public P25KeyErase()
        {
            InitializeComponent();

            cbActiveKeyset.IsChecked = true; // check here to trigger the cb/txt logic on load
        }

        private void OnActiveKeysetChecked(object sender, RoutedEventArgs e)
        {
            txtKeysetIdDec.Text = string.Empty;
            txtKeysetIdHex.Text = string.Empty;
            txtKeysetIdDec.IsEnabled = false;
            txtKeysetIdHex.IsEnabled = false;
        }

        private void OnActiveKeysetUnchecked(object sender, RoutedEventArgs e)
        {
            txtKeysetIdDec.IsEnabled = true;
            txtKeysetIdHex.IsEnabled = true;
        }

        private void KeysetIdDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKeysetIdDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtKeysetIdDec.Text, out num))
                {
                    txtKeysetIdHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtKeysetIdHex.Text = string.Empty;
                }
            }
        }

        private void KeysetIdHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKeysetIdHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtKeysetIdHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtKeysetIdDec.Text = num.ToString();
                }
                else
                {
                    txtKeysetIdDec.Text = string.Empty;
                }
            }
        }

        private void SlnDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtSlnDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtSlnDec.Text, out num))
                {
                    txtSlnHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtSlnHex.Text = string.Empty;
                }
            }
        }

        private void SlnHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtSlnHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtSlnHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtSlnDec.Text = num.ToString();
                }
                else
                {
                    txtSlnDec.Text = string.Empty;
                }
            }
        }

        private void Erase_Button_Click(object sender, RoutedEventArgs e)
        {
            int keysetId = 0;
            int sln = 0;

            bool useActiveKeyset = cbActiveKeyset.IsChecked == true;

            if (useActiveKeyset)
            {
                keysetId = 2; // to pass validation, will not get used
            }
            else
            {
                try
                {
                    keysetId = Convert.ToInt32(txtKeysetIdHex.Text, 16);
                }
                catch (Exception)
                {
                    MessageBox.Show("Error Parsing Keyset ID", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }

            try
            {
                sln = Convert.ToInt32(txtSlnHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing SLN", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            bool keysetValidateResult = FieldValidator.IsValidKeysetId(keysetId);

            if (!keysetValidateResult)
            {
                MessageBox.Show("Keyset ID invalid - valid range 1 to 255 (dec), 0x01 to 0xFF (hex)", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            bool slnValidateResult = FieldValidator.IsValidSln(sln);

            if (!slnValidateResult)
            {
                MessageBox.Show("SLN invalid - valid range 1 to 65553 (dec), 0x0001 to 0x1999 (hex)", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                Interact.EraseKey(Settings.Port, useActiveKeyset, keysetId, sln);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBox.Show("Key Erased Successfully", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
