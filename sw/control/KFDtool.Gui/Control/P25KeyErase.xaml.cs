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
        private bool IsKek { get; set; }

        public P25KeyErase()
        {
            InitializeComponent();

            IsKek = false;

            cbActiveKeyset.IsChecked = true; // check here to trigger the cb/txt logic on load
            cboType.SelectedIndex = 0; // set to the first item here to trigger the cbo/lbl logic on load
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

                UpdateType();
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

                UpdateType();
            }
        }

        private void UpdateType()
        {
            if (cboType.SelectedItem != null)
            {
                string name = ((ComboBoxItem)cboType.SelectedItem).Name as string;

                if (name == "AUTO")
                {
                    int num;

                    if (int.TryParse(txtSlnHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                    {
                        if (num >= 0 && num <= 61439)
                        {
                            lblType.Content = "TEK";
                            IsKek = false;
                        }
                        else if (num >= 61440 && num <= 65535)
                        {
                            lblType.Content = "KEK";
                            IsKek = true;
                        }
                        else
                        {
                            lblType.Content = "Auto";
                        }
                    }
                    else
                    {
                        lblType.Content = "Auto";
                    }
                }
                else if (name == "TEK")
                {
                    lblType.Content = "TEK";
                    IsKek = false;
                }
                else if (name == "KEK")
                {
                    lblType.Content = "KEK";
                    IsKek = true;
                }
                else
                {
                    // error
                }
            }
        }

        private void OnTypeChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateType();
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
                MessageBox.Show("SLN invalid - valid range 0 to 65535 (dec), 0x0000 to 0xFFFF (hex)", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            List<CmdKeyItem> keys = new List<CmdKeyItem>();

            CmdKeyItem keyItem = new CmdKeyItem();

            keyItem.UseActiveKeyset = useActiveKeyset;
            keyItem.KeysetId = keysetId;
            keyItem.Sln = sln;
            keyItem.IsKek = IsKek;

            keys.Add(keyItem);

            try
            {
                Interact.EraseKey(Settings.SelectedDevice, keys);
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
