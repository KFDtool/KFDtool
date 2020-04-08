using KFDtool.P25.TransferConstructs;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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
    /// Interaction logic for P25ViewKeysetInfo.xaml
    /// </summary>
    public partial class P25ViewKeysetInfo : UserControl
    {
        public P25ViewKeysetInfo()
        {
            InitializeComponent();
        }

        private void ksIdOldDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKsIdOldDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtKsIdOldDec.Text, out num))
                {
                    txtKsIdOldHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtKsIdOldHex.Text = string.Empty;
                }
            }
        }

        private void ksIdOldHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKsIdOldHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtKsIdOldHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtKsIdOldDec.Text = num.ToString();
                }
                else
                {
                    txtKsIdOldDec.Text = string.Empty;
                }
            }
        }

        private void ksIdNewDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKsIdNewDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtKsIdNewDec.Text, out num))
                {
                    txtKsIdNewHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtKsIdNewHex.Text = string.Empty;
                }

                //UpdateType();
            }
        }

        private void ksIdNewHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtKsIdNewHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtKsIdNewHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtKsIdNewDec.Text = num.ToString();
                }
                else
                {
                    txtKsIdNewDec.Text = string.Empty;
                }

                //UpdateType();
            }
        }

        private void View_KeysetInfo_Click(object sender, RoutedEventArgs e)
        {
            KeysetItems.ItemsSource = null; // clear table

            List<RspKeysetInfo> keyset = null;

            try
            {
                keyset = Interact.ViewKeysetTaggingInfo(Settings.Port);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (keyset != null)
            {
                KeysetItems.ItemsSource = keyset;

                KeysetItems.Items.SortDescriptions.Add(new SortDescription("KeysetId", ListSortDirection.Ascending));

                MessageBox.Show(string.Format("{0} keyset(s) returned", keyset.Count), "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        private void Changeover_Click(object sender, RoutedEventArgs e)
        {
            RspChangeoverInfo changeoverResult = new RspChangeoverInfo();
            try
            {
                //Interact.ActivateKeyset(Settings.Port, keysetSuperseded, keysetActivated);
                changeoverResult = Interact.ActivateKeyset(Settings.Port, int.Parse(txtKsIdOldDec.Text), int.Parse(txtKsIdNewDec.Text));
                MessageBox.Show("Keyset " + changeoverResult.KeysetIdActivated + " activated, Keyset " + changeoverResult.KeysetIdSuperseded + " superseded", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
        }
    }
}
