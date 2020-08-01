using System.Windows;

namespace KFDtool.Gui.Dialog
{
    /// <summary>
    /// Interaction logic for ContainerEnterPassword.xaml
    /// </summary>
    public partial class ContainerEnterPassword : Window
    {
        public bool PasswordSet { get; set; }

        public string PasswordText { get; set; }

        public ContainerEnterPassword()
        {
            InitializeComponent();

            PasswordSet = false;
            PasswordText = string.Empty;

            txtPassword.Focus(); // focus first password field on load
        }

        private void Submit_Click(object sender, RoutedEventArgs e)
        {
            if (txtPassword.Password.Length == 0)
            {
                MessageBox.Show("Password is required", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            PasswordSet = true;
            PasswordText = txtPassword.Password;

            Close();
        }
    }
}
