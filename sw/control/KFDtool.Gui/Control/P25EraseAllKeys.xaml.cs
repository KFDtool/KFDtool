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
    /// Interaction logic for P25EraseAllKeys.xaml
    /// </summary>
    public partial class P25EraseAllKeys : UserControl
    {
        public P25EraseAllKeys()
        {
            InitializeComponent();
        }

        private void Erase_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Interact.EraseAllKeys(Settings.SelectedDevice);
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("Error -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBox.Show("All Keys Erased Successfully", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
