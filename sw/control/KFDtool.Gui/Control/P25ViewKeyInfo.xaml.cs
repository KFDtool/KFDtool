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

namespace KFDtool.Gui.Control
{
    /// <summary>
    /// Interaction logic for P25ViewKeyInfo.xaml
    /// </summary>
    public partial class P25ViewKeyInfo : UserControl
    {
        public P25ViewKeyInfo()
        {
            InitializeComponent();
        }

        private void Update_Click(object sender, RoutedEventArgs e)
        {
            KeyItems.ItemsSource = null; // clear table

            List<RspKeyInfo> keys = null;

            try
            {
                keys = Interact.ViewKeyInfo(Settings.Port);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (keys != null)
            {
                KeyItems.ItemsSource = keys;

                KeyItems.Items.SortDescriptions.Add(new SortDescription("KeysetId", ListSortDirection.Ascending));
                KeyItems.Items.SortDescriptions.Add(new SortDescription("Sln", ListSortDirection.Ascending));

                MessageBox.Show(string.Format("{0} key(s) returned", keys.Count), "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }
    }
}
