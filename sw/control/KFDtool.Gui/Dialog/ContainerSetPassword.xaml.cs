using System.Windows;

namespace KFDtool.Gui.Dialog
{
    /// <summary>
    /// Interaction logic for ContainerSetPassword.xaml
    /// </summary>
    public partial class ContainerSetPassword : Window
    {
        public bool PasswordSet { get; set; }

        public string PasswordText { get; set; }

        public ContainerSetPassword()
        {
            InitializeComponent();

            PasswordSet = false;
            PasswordText = string.Empty;

            txtPassword.Focus(); // focus first password field on load
        }

        private void Set_Password_Click(object sender, RoutedEventArgs e)
        {
            if (txtPassword.Password != txtPasswordConfirm.Password)
            {
                MessageBox.Show("Passwords do not match", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (txtPassword.Password.Length == 0)
            {
                MessageBox.Show("Password is required", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (txtPassword.Password.Length < 16)
            {
                MessageBoxResult res = MessageBox.Show("This password is weak (under 16 characters in length) - use anyways?", "Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning, MessageBoxResult.No);

                if (res == MessageBoxResult.No)
                {
                    return;
                }
            }

            PasswordSet = true;
            PasswordText = txtPassword.Password;

            Close();
        }
    }
}
