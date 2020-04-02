using KFDtool.P25.Constant;
using KFDtool.P25.Generator;
using KFDtool.P25.TransferConstructs;
using KFDtool.P25.Validator;
using KFDtool.Shared;
using System;
using System.Collections.Generic;
using System.Globalization;
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

namespace KFDtool.Gui.Control
{
    /// <summary>
    /// Interaction logic for P25ViewKeyInfo.xaml
    /// </summary>
    public partial class P25ViewRsiConfig : UserControl
    {
        public P25ViewRsiConfig()
        {
            InitializeComponent();
        }

        private void View_RsiItems_Click(object sender, RoutedEventArgs e)
        {
            ViewRsiItems();
        }

        void cbRsiAction_Changed(object sender, SelectionChangedEventArgs e)
        {
            if (cbRsiAction.SelectedIndex == 0)
            {
                btnUpdateRsi.IsEnabled = false;
            }
            else
            {
                btnUpdateRsi.IsEnabled = true;
            }
        }

        private void Update_RSI_Click(object sender, RoutedEventArgs e)
        {
            int rsiOld = 0;
            int rsiNew = 0;
            int mn = 0;
            RspRsiInfo result = new RspRsiInfo();

            try
            {
                rsiOld = Convert.ToInt32(txtRsiOldHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing old RSI", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                rsiNew = Convert.ToInt32(txtRsiNewHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing new RSI", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                mn = Convert.ToInt32(txtMnpHex.Text, 16);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing Message Number", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Ensure valid RSI value
            if ((rsiOld < 0) || (rsiOld > 16777214) || (rsiOld == 9999999))
            {
                MessageBox.Show("Invalid old RSI - must be between 1 and 9,999,998 or 10,000,000 to 16,777,214", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if ((rsiNew < 0) || (rsiNew > 16777214) || (rsiNew == 9999999))
            {
                MessageBox.Show("Invalid new RSI - must be between 1 and 9,999,998 or 10,000,000 to 16,777,214", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Ensure valid MN value
            if ((mn > 65535) || (mn < 0))
            {
                MessageBox.Show("Invalid Message Number - must be between 0 and 65,535", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Ensure individual-individual and group-group
            if ((rsiOld < 9999999) && (rsiNew > 9999999))
            {
                MessageBox.Show("Individual RSI cannot be changed to a Group RSI", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if ((rsiOld > 9999999) && (rsiNew < 9999999))
            {
                MessageBox.Show("Group RSI cannot be changed to an Individual RSI", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                result = Interact.ChangeRsi(Settings.Port, rsiOld, rsiNew, mn);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (result.Status == 0)
            {
                ViewRsiItems();
                MessageBox.Show("RSI changed successfully", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
            {
                MessageBox.Show("Error, status: " + result.Status, "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        private void ViewRsiItems()
        {
            dgRsiItems.ItemsSource = null; // clear table

            List<RspRsiInfo> items = null;

            try
            {
                items = Interact.ViewRsiItems(Settings.Port);
                //Console.WriteLine(items.Count + " result");
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (items != null)
            {
                dgRsiItems.ItemsSource = items;

                dgRsiItems.Items.SortDescriptions.Add(new SortDescription("RSI", ListSortDirection.Ascending));
                dgRsiItems.Items.SortDescriptions.Add(new SortDescription("MN", ListSortDirection.Ascending));

                //MessageBox.Show(string.Format("{0} RSI(s) returned", items.Count), "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }

            txtRsiOldDec.Text = string.Empty;
            txtRsiOldHex.Text = string.Empty;
            txtRsiNewDec.Text = string.Empty;
            txtRsiNewHex.Text = string.Empty;
            txtMnpDec.Text = string.Empty;
            txtMnpHex.Text = string.Empty;
        }

        private void RsiOldDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtRsiOldDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtRsiOldDec.Text, out num))
                {
                    txtRsiOldHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtRsiOldHex.Text = string.Empty;
                }
            }
        }

        private void RsiOldHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtRsiOldHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtRsiOldHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtRsiOldDec.Text = num.ToString();
                }
                else
                {
                    txtRsiOldDec.Text = string.Empty;
                }
            }
        }

        private void RsiNewDec_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtRsiNewDec.IsFocused)
            {
                int num;

                if (int.TryParse(txtRsiNewDec.Text, out num))
                {
                    txtRsiNewHex.Text = string.Format("{0:X}", num);
                }
                else
                {
                    txtRsiNewHex.Text = string.Empty;
                }
            }
        }

        private void RsiNewHex_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (txtRsiNewHex.IsFocused)
            {
                int num;

                if (int.TryParse(txtRsiNewHex.Text, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out num))
                {
                    txtRsiNewDec.Text = num.ToString();
                }
                else
                {
                    txtRsiNewDec.Text = string.Empty;
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
            }
        }

        private void cbNewClick(object sender, RoutedEventArgs e)
        {
            btnUpdateRsi.IsEnabled = true;
            txtRsiOldDec.Text = "0";
            txtRsiOldHex.Text = "0";
            txtRsiNewDec.Text = "0000000";
            txtRsiNewHex.Text = "0000000";
            txtMnpDec.Text = "0000";
            txtMnpHex.Text = "0000";
            txtRsiOldDec.IsEnabled = false;
            txtRsiOldHex.IsEnabled = false;
            txtRsiNewDec.IsEnabled = true;
            txtRsiNewHex.IsEnabled = true;
            txtMnpDec.IsEnabled = true;
            txtMnpHex.IsEnabled = true;
        }

        private void cbChangeClick(object sender, RoutedEventArgs e)
        {
            btnUpdateRsi.IsEnabled = true;
            txtRsiOldDec.Text = "Choose from table";
            txtRsiOldHex.Text = "";
            txtRsiNewDec.Text = "0000000";
            txtRsiNewHex.Text = "0000000";
            txtMnpDec.Text = "0000";
            txtMnpHex.Text = "0000";
            txtRsiOldDec.IsEnabled = false;
            txtRsiOldHex.IsEnabled = false;
            txtRsiNewDec.IsEnabled = true;
            txtRsiNewHex.IsEnabled = true;
            txtMnpDec.IsEnabled = true;
            txtMnpHex.IsEnabled = true;
        }

        private void cbDeleteClick(object sender, RoutedEventArgs e)
        {
            btnUpdateRsi.IsEnabled = true;
            txtRsiOldDec.Text = "Choose from table";
            txtRsiOldHex.Text = "";
            txtRsiNewDec.Text = "0000000";
            txtRsiNewHex.Text = "0000000";
            txtMnpDec.Text = "0000";
            txtMnpHex.Text = "0000";
            txtRsiOldDec.IsEnabled = false;
            txtRsiOldHex.IsEnabled = false;
            txtRsiNewDec.IsEnabled = false;
            txtRsiNewHex.IsEnabled = false;
            txtMnpDec.IsEnabled = false;
            txtMnpHex.IsEnabled = false;
        }

        private void dgRsiItems_RowClicked(object sender, MouseButtonEventArgs e)
        {
            if (sender != null)
            {
                DataGrid grid = sender as DataGrid;
                if (grid != null && grid.SelectedItems != null && grid.SelectedItems.Count == 1)
                {
                    if (cbRsiAction.SelectedIndex == 1) { return; }
                    DataGridRow dgr = grid.ItemContainerGenerator.ContainerFromItem(grid.SelectedItem) as DataGridRow;
                    RspRsiInfo rsiItem = (RspRsiInfo)dgRsiItems.SelectedItem;
                    txtRsiOldDec.Text = rsiItem.RSI.ToString();
                    txtRsiOldHex.Text = rsiItem.RSI.ToString("X");
                    txtMnpDec.Text = rsiItem.MN.ToString();
                    txtMnpHex.Text = rsiItem.MN.ToString("X");
                }
            }
        }
    }
}
