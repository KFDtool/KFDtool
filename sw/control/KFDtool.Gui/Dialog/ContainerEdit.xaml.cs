using KFDtool.Container;
using KFDtool.P25.Generator;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;

namespace KFDtool.Gui.Dialog
{
    /// <summary>
    /// Interaction logic for ContainerEdit.xaml
    /// </summary>
    public partial class ContainerEdit : Window
    {
        private string OriginalContainer;

        public ContainerEdit()
        {
            InitializeComponent();

            OriginalContainer = ContainerUtilities.SerializeInnerContainer(Settings.ContainerInner).OuterXml;

            keysListView.ItemsSource = Settings.ContainerInner.Keys;

            keysListView.SelectionChanged += KeysListView_SelectionChanged;

            groupsListView.ItemsSource = Settings.ContainerInner.Groups;

            groupsListView.SelectionChanged += GroupsListView_SelectionChanged;
        }

        private void Tab_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.Source is TabControl)
            {
                keysListView.SelectedItem = null;

                groupsListView.SelectedItem = null;
            }
        }

        private void KeysListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (keysListView.SelectedItem != null)
            {
                ContainerEditKeyControl keyEdit = new ContainerEditKeyControl((KeyItem)keysListView.SelectedItem);

                ItemView.Content = keyEdit;
            }
            else
            {
                ItemView.Content = null;
            }
        }

        private void GroupsListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (groupsListView.SelectedItem != null)
            {
                ContainerEditGroupControl keyEdit = new ContainerEditGroupControl((Container.GroupItem)groupsListView.SelectedItem);

                ItemView.Content = keyEdit;
            }
            else
            {
                ItemView.Content = null;
            }
        }

        void ContainerEdit_Closing(object sender, CancelEventArgs e)
        {
            string currentContainer = ContainerUtilities.SerializeInnerContainer(Settings.ContainerInner).OuterXml;

            if (OriginalContainer != currentContainer)
            {
                Settings.ContainerSaved = false;
            }
        }

        private void New_Click(object sender, RoutedEventArgs e)
        {
            if (containerTabControl.SelectedItem == keysTabItem)
            {
                KeyItem key = new KeyItem();
                key.Id = Settings.ContainerInner.NextKeyNumber;
                key.Name = string.Format("Key {0}", Settings.ContainerInner.NextKeyNumber);
                Settings.ContainerInner.NextKeyNumber++;
                key.ActiveKeyset = true;
                key.KeysetId = 1;
                key.Sln = 1;
                key.KeyTypeAuto = true;
                key.KeyTypeTek = false;
                key.KeyTypeKek = false;
                key.KeyId = 1;
                key.AlgorithmId = 0x84;
                key.Key = BitConverter.ToString(KeyGenerator.GenerateVarKey(32).ToArray()).Replace("-", string.Empty);
                Settings.ContainerInner.Keys.Add(key);
            }
            else if (containerTabControl.SelectedItem == groupsTabItem)
            {
                Container.GroupItem group = new Container.GroupItem();
                group.Id = Settings.ContainerInner.NextGroupNumber;
                group.Name = string.Format("Group {0}", Settings.ContainerInner.NextGroupNumber);
                Settings.ContainerInner.NextGroupNumber++;
                group.Keys = new List<int>();
                Settings.ContainerInner.Groups.Add(group);
            }
        }

        private void Up_Click(object sender, RoutedEventArgs e)
        {
            if (containerTabControl.SelectedItem == keysTabItem)
            {
                if (keysListView.SelectedItem != null)
                {
                    int index = keysListView.SelectedIndex;

                    if (index - 1 >= 0)
                    {
                        Settings.ContainerInner.Keys.Move(index, index - 1);
                    }
                }
            }
            else if (containerTabControl.SelectedItem == groupsTabItem)
            {
                if (groupsListView.SelectedItem != null)
                {
                    int index = groupsListView.SelectedIndex;

                    if (index - 1 >= 0)
                    {
                        Settings.ContainerInner.Groups.Move(index, index - 1);
                    }
                }
            }
        }

        private void Down_Click(object sender, RoutedEventArgs e)
        {
            if (containerTabControl.SelectedItem == keysTabItem)
            {
                if (keysListView.SelectedItem != null)
                {
                    int index = keysListView.SelectedIndex;

                    if (index + 1 < Settings.ContainerInner.Keys.Count)
                    {
                        Settings.ContainerInner.Keys.Move(index, index + 1);
                    }
                }
            }
            else if (containerTabControl.SelectedItem == groupsTabItem)
            {
                if (groupsListView.SelectedItem != null)
                {
                    int index = groupsListView.SelectedIndex;

                    if (index + 1 < Settings.ContainerInner.Groups.Count)
                    {
                        Settings.ContainerInner.Groups.Move(index, index + 1);
                    }
                }
            }
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            if (containerTabControl.SelectedItem == keysTabItem)
            {
                if (keysListView.SelectedItem != null)
                {
                    int index = keysListView.SelectedIndex;

                    int id = Settings.ContainerInner.Keys[index].Id;

                    // remove key reference from groups
                    foreach (Container.GroupItem groupItem in Settings.ContainerInner.Groups)
                    {
                        if (groupItem.Keys.Contains(id))
                        {
                            groupItem.Keys.Remove(id);
                        }
                    }

                    // remove key item
                    Settings.ContainerInner.Keys.RemoveAt(index);
                }
            }
            else if (containerTabControl.SelectedItem == groupsTabItem)
            {
                if (groupsListView.SelectedItem != null)
                {
                    int index = groupsListView.SelectedIndex;
                    Settings.ContainerInner.Groups.RemoveAt(index);
                }
            }
        }
    }
}
