using KFDtool.Container;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;

namespace KFDtool.Gui.Dialog
{
    /// <summary>
    /// Interaction logic for ContainerEditGroupControl.xaml
    /// </summary>
    public partial class ContainerEditGroupControl : UserControl
    {
        private Container.GroupItem LocalGroup { get; set; }

        private List<int> Keys;

        private Dictionary<int, string> Available;

        private Dictionary<int, string> Selected;

        public ContainerEditGroupControl(Container.GroupItem groupItem)
        {
            InitializeComponent();

            LocalGroup = groupItem;

            Keys = new List<int>();
            Keys.AddRange(groupItem.Keys);

            Available = new Dictionary<int, string>();

            Selected = new Dictionary<int, string>();

            txtName.Text = groupItem.Name;

            lbAvailable.ItemsSource = Available;

            lbSelected.ItemsSource = Selected;

            UpdateColumns();
        }

        private void UpdateColumns()
        {
            Available.Clear();

            foreach (KeyItem keyItem in Settings.ContainerInner.Keys)
            {
                Available.Add(keyItem.Id, keyItem.Name);
            }

            Selected.Clear();

            foreach (int key in Keys)
            {
                Selected.Add(key, Available[key]);
            }

            foreach (KeyValuePair<int, string> selected in Selected)
            {
                Available.Remove(selected.Key);
            }

            lbAvailable.Items.Refresh();

            lbSelected.Items.Refresh();
        }

        private void Add_Button_Click(object sender, RoutedEventArgs e)
        {
            if (lbAvailable.SelectedItem != null)
            {
                int key = ((KeyValuePair<int, string>)lbAvailable.SelectedItem).Key;

                Keys.Add(key);

                UpdateColumns();
            }
        }

        private void Remove_Button_Click(object sender, RoutedEventArgs e)
        {
            if (lbSelected.SelectedItem != null)
            {
                int key = ((KeyValuePair<int, string>)lbSelected.SelectedItem).Key;

                Keys.Remove(key);

                UpdateColumns();
            }
        }

        private void Save_Button_Click(object sender, RoutedEventArgs e)
        {
            if (txtName.Text.Length == 0)
            {
                MessageBox.Show("Group name required", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (txtName.Text != LocalGroup.Name)
            {
                foreach (Container.GroupItem groupItem in Settings.ContainerInner.Groups)
                {
                    if (txtName.Text == groupItem.Name)
                    {
                        MessageBox.Show("Group name must be unique", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                }
            }

            LocalGroup.Name = txtName.Text;
            LocalGroup.Keys.Clear();
            LocalGroup.Keys.AddRange(Keys);
        }
    }
}
