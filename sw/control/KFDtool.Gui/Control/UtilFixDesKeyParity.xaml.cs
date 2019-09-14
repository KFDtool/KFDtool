using KFDtool.P25.Generator;
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
    /// Interaction logic for UtilFixDesKeyParity.xaml
    /// </summary>
    public partial class UtilFixDesKeyParity : UserControl
    {
        public UtilFixDesKeyParity()
        {
            InitializeComponent();
        }

        private void Fix_Button_Click(object sender, RoutedEventArgs e)
        {
            if (txtDesKeyIn.Text.Length != 16)
            {
                MessageBox.Show("Invalid key length", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            List<byte> key = new List<byte>();

            try
            {
                key = Utility.ByteStringToByteList(txtDesKeyIn.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("Error Parsing Key", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            byte[] outArrKey = null;

            try
            {
                outArrKey = KeyGenerator.FixupKeyParity(key.ToArray());
            }
            catch (Exception)
            {
                MessageBox.Show("Error Fixing Key", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            txtDesKeyOut.Text = BitConverter.ToString(outArrKey).Replace("-", string.Empty);
        }
    }
}
