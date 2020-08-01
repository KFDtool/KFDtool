using KFDtool.Container;
using KFDtool.P25.Constant;
using KFDtool.P25.Generator;
using KFDtool.P25.Validator;
using KFDtool.Shared;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;

namespace KFDtool.Gui.Dialog
{
    /// <summary>
    /// Interaction logic for ContainerEditKeyControl.xaml
    /// </summary>
    public partial class ContainerEditKeyControl : UserControl
    {
        private KeyItem LocalKey { get; set; }

        private bool IsKek { get; set; }

        public ContainerEditKeyControl(KeyItem keyItem)
        {
            InitializeComponent();

            LocalKey = keyItem;

            txtName.Text = keyItem.Name;

            if (keyItem.ActiveKeyset)
            {
                cbActiveKeyset.IsChecked = true;
            }
            else
            {
                cbActiveKeyset.IsChecked = false;

                txtKeysetIdDec.Text = keyItem.KeysetId.ToString();
                UpdateKeysetIdDec();
            }

            if (keyItem.KeyTypeAuto)
            {
                cboType.SelectedIndex = 0;
            }
            else if (keyItem.KeyTypeTek)
            {
                cboType.SelectedIndex = 1;
            }
            else if (keyItem.KeyTypeKek)
            {
                cboType.SelectedIndex = 2;
            }
            else
            {
                throw new Exception("invalid key type");
            }

            txtSlnDec.Text = keyItem.Sln.ToString();
            UpdateSlnDec();

            txtKeyIdDec.Text = keyItem.KeyId.ToString();
            UpdateKeyIdDec();

            if (keyItem.AlgorithmId == 0x84)
            {
                cboAlgo.SelectedIndex = 0;
            }
            else if (keyItem.AlgorithmId == 0x81)
            {
                cboAlgo.SelectedIndex = 1;
            }
            else if (keyItem.AlgorithmId == 0x9F)
            {
                cboAlgo.SelectedIndex = 2;
            }
            else if (keyItem.AlgorithmId == 0xAA)
            {
                cboAlgo.SelectedIndex = 3;
            }
            else
            {
                cboAlgo.SelectedIndex = 4;

                txtAlgoDec.Text = keyItem.AlgorithmId.ToString();

                UpdateAlgoDec();
            }

            cbHide.IsChecked = true;

            txtKeyHidden.Password = keyItem.Key;

        }

        private void UpdateKeysetIdDec()
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

        private void UpdateKeysetIdHex()
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

        private void KeysetIdDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKeysetIdDec.IsFocused)
            {
                UpdateKeysetIdDec();
            }
        }

        private void KeysetIdHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKeysetIdHex.IsFocused)
            {
                UpdateKeysetIdHex();
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

        private void UpdateSlnDec()
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

        private void UpdateSlnHex()
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

        private void SlnDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtSlnDec.IsFocused)
            {
                UpdateSlnDec();
            }
        }

        private void SlnHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtSlnHex.IsFocused)
            {
                UpdateSlnHex();
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

        private void UpdateKeyIdDec()
        {
            int num;

            if (int.TryParse(txtKeyIdDec.Text, out num))
            {
                txtKeyIdHex.Text = string.Format("{0:X}", num);
            }
            else
            {
                txtKeyIdHex.Text = string.Empty;
            }
        }

        private void UpdateKeyIdHex()
        {
            int num;

            if (int.TryParse(txtKeyIdHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
            {
                txtKeyIdDec.Text = num.ToString();
            }
            else
            {
                txtKeyIdDec.Text = string.Empty;
            }
        }

        private void KeyIdDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKeyIdDec.IsFocused)
            {
                UpdateKeyIdDec();
            }
        }

        private void KeyIdHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKeyIdHex.IsFocused)
            {
                UpdateKeyIdHex();
            }
        }

        private void UpdateAlgoDec()
        {
            int num;

            if (int.TryParse(txtAlgoDec.Text, out num))
            {
                txtAlgoHex.Text = string.Format("{0:X}", num);
            }
            else
            {
                txtAlgoHex.Text = string.Empty;
            }
        }

        private void UpdateAlgoHex()
        {
            int num;

            if (int.TryParse(txtAlgoHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
            {
                txtAlgoDec.Text = num.ToString();
            }
            else
            {
                txtAlgoDec.Text = string.Empty;
            }
        }

        private void AlgoDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtAlgoDec.IsFocused)
            {
                UpdateAlgoDec();
            }
        }

        private void AlgoHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtAlgoHex.IsFocused)
            {
                UpdateAlgoHex();
            }
        }

        private void OnAlgoChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cboAlgo.SelectedItem != null)
            {
                string name = ((ComboBoxItem)cboAlgo.SelectedItem).Name as string;

                if (name == "AES256")
                {
                    txtAlgoHex.Text = "84";
                    UpdateAlgoHex();
                    txtAlgoDec.IsEnabled = false;
                    txtAlgoHex.IsEnabled = false;
                }
                else if (name == "DESOFB")
                {
                    txtAlgoHex.Text = "81";
                    UpdateAlgoHex();
                    txtAlgoDec.IsEnabled = false;
                    txtAlgoHex.IsEnabled = false;
                }
                else if (name == "DESXL")
                {
                    txtAlgoHex.Text = "9F";
                    UpdateAlgoHex();
                    txtAlgoDec.IsEnabled = false;
                    txtAlgoHex.IsEnabled = false;
                }
                else if (name == "ADP")
                {
                    txtAlgoHex.Text = "AA";
                    UpdateAlgoHex();
                    txtAlgoDec.IsEnabled = false;
                    txtAlgoHex.IsEnabled = false;
                }
                else
                {
                    txtAlgoDec.Text = string.Empty;
                    txtAlgoHex.Text = string.Empty;
                    txtAlgoDec.IsEnabled = true;
                    txtAlgoHex.IsEnabled = true;
                }
            }
        }

        private void Generate_Button_Click(object sender, RoutedEventArgs e)
        {
            int algId = 0;

            try
            {
                algId = Convert.ToInt32(txtAlgoHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing Algorithm ID", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (!FieldValidator.IsValidAlgorithmId(algId))
            {
                MessageBox.Show("Algorithm ID invalid - valid range 0 to 255 (dec), 0x00 to 0xFF (hex)", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            List<byte> key = new List<byte>();

            if (algId == (byte)AlgorithmId.AES256)
            {
                key = KeyGenerator.GenerateVarKey(32);
            }
            else if (algId == (byte)AlgorithmId.DESOFB || algId == (byte)AlgorithmId.DESXL)
            {
                key = KeyGenerator.GenerateSingleDesKey();
            }
            else if (algId == (byte)AlgorithmId.ADP)
            {
                key = KeyGenerator.GenerateVarKey(5);
            }
            else
            {
                MessageBox.Show(string.Format("No key generator exists for algorithm ID 0x{0:X2}", algId), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            SetKey(BitConverter.ToString(key.ToArray()).Replace("-", string.Empty));
        }

        private void OnHideChecked(object sender, RoutedEventArgs e)
        {
            txtKeyHidden.Password = txtKeyVisible.Text;
            txtKeyVisible.Text = string.Empty;
            txtKeyVisible.Visibility = Visibility.Hidden;
            txtKeyHidden.Visibility = Visibility.Visible;
        }

        private void OnHideUnchecked(object sender, RoutedEventArgs e)
        {
            txtKeyVisible.Text = txtKeyHidden.Password;
            txtKeyHidden.Password = null;
            txtKeyVisible.Visibility = Visibility.Visible;
            txtKeyHidden.Visibility = Visibility.Hidden;
        }

        private string GetKey()
        {
            if (cbHide.IsChecked == true)
            {
                return txtKeyHidden.Password;
            }
            else
            {
                return txtKeyVisible.Text;
            }
        }

        private void SetKey(string str)
        {
            if (cbHide.IsChecked == true)
            {
                txtKeyHidden.Password = str;
            }
            else
            {
                txtKeyVisible.Text = str;
            }
        }

        private void Save_Button_Click(object sender, RoutedEventArgs e)
        {
            int keysetId;
            int sln;
            int keyId;
            int algId;
            List<byte> key;

            bool useActiveKeyset = cbActiveKeyset.IsChecked == true;

            if (useActiveKeyset)
            {
                keysetId = 1; // to pass validation, will not get used
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

            try
            {
                keyId = Convert.ToInt32(txtKeyIdHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing Key ID", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                algId = Convert.ToInt32(txtAlgoHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing Algorithm ID", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                key = Utility.ByteStringToByteList(GetKey());
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing Key", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            Tuple<ValidateResult, string> validateResult = FieldValidator.KeyloadValidate(keysetId, sln, IsKek, keyId, algId, key);

            if (validateResult.Item1 == ValidateResult.Warning)
            {
                if (MessageBox.Show(string.Format("{1}{0}{0}Continue?", Environment.NewLine, validateResult.Item2), "Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.No)
                {
                    return;
                }
            }
            else if (validateResult.Item1 == ValidateResult.Error)
            {
                MessageBox.Show(validateResult.Item2, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (txtName.Text.Length == 0)
            {
                MessageBox.Show("Key name required", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (txtName.Text != LocalKey.Name)
            {
                foreach (KeyItem keyItem in Settings.ContainerInner.Keys)
                {
                    if (txtName.Text == keyItem.Name)
                    {
                        MessageBox.Show("Key name must be unique", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                }
            }

            LocalKey.Name = txtName.Text;
            LocalKey.ActiveKeyset = useActiveKeyset;
            LocalKey.KeysetId = keysetId;
            LocalKey.Sln = sln;

            if (cboType.SelectedIndex == 0)
            {
                LocalKey.KeyTypeAuto = true;
                LocalKey.KeyTypeTek = false;
                LocalKey.KeyTypeKek = false;
            }
            else if (cboType.SelectedIndex == 1)
            {
                LocalKey.KeyTypeAuto = false;
                LocalKey.KeyTypeTek = true;
                LocalKey.KeyTypeKek = false;
            }
            else if (cboType.SelectedIndex == 2)
            {
                LocalKey.KeyTypeAuto = false;
                LocalKey.KeyTypeTek = false;
                LocalKey.KeyTypeKek = true;
            }
            else
            {
                throw new Exception("invalid key type");
            }

            LocalKey.KeyId = keyId;
            LocalKey.AlgorithmId = algId;
            LocalKey.Key = BitConverter.ToString(key.ToArray()).Replace("-", string.Empty);
        }
    }
}
