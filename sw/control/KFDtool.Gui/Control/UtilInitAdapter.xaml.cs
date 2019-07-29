using KFDtool.Adapter.Bundle;
using KFDtool.Adapter.Device;
using KFDtool.Adapter.Protocol.Adapter;
using KFDtool.P25.TransferConstructs;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
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
    class UpdateItem
    {
        public int PkgIndex { get; set; }

        public byte MdlId { get; set; } 

        public byte HwRevMaj { get; set; }

        public byte HwRevMin { get; set; }
    }

    /// <summary>
    /// Interaction logic for UtilInitAdapter.xaml
    /// </summary>
    public partial class UtilInitAdapter : UserControl
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        private const string NO_FILE_SELECTED = "No File Selected";

        private string UpdatePackage;

        private Dictionary<int,UpdateItem> UpdItm;

        public UtilInitAdapter()
        {
            InitializeComponent();

            cboAvailPkg.IsEnabled = false;
            btnInit.IsEnabled = false;

            UpdatePackage = string.Empty;
            lblPath.Content = NO_FILE_SELECTED;

            UpdItm = new Dictionary<int, UpdateItem>();
        }

        private void Select_Button_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Compressed Fw Pkg (*.cfp)|*.cfp|Uncompressed Fw Pkg (*.ufp)|*.ufp|All files (*.*)|*.*";

            if (openFileDialog.ShowDialog() == true)
            {
                UpdatePackage = openFileDialog.FileName;
                lblPath.Content = UpdatePackage;

                Logger.Debug("update package path: {0}", UpdatePackage);

                if (UpdatePackage.Equals(string.Empty))
                {
                    Logger.Error("no file selected");
                    MessageBox.Show("No file selected", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                // file existed when it was selected but now doesn't
                if (!File.Exists(UpdatePackage))
                {
                    Logger.Error("selected file does not exist");
                    MessageBox.Show("Selected file does not exist", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    UpdatePackage = string.Empty;
                    lblPath.Content = NO_FILE_SELECTED;
                    return;
                }

                // check if package is cfp or ufp
                string ext = System.IO.Path.GetExtension(UpdatePackage);

                Logger.Debug("update package extension: {0}", ext);

                List<Update> pkg = null;

                if (ext == ".cfp")
                {
                    Logger.Debug("opening compressed update package");

                    try
                    {
                        pkg = Firmware.OpenCompressedUpdatePackage(UpdatePackage);
                    }
                    catch (Exception ex)
                    {
                        Logger.Error("unable to parse file: {0}", ex.Message);
                        MessageBox.Show(string.Format("Unable to parse file: {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                }
                else if (ext == ".ufp")
                {
                    Logger.Debug("opening uncompressed update package");

                    try
                    {
                        pkg = Firmware.OpenUncompressedUpdatePackage(UpdatePackage);
                    }
                    catch (Exception ex)
                    {
                        Logger.Error("unable to parse file: {0}", ex.Message);
                        MessageBox.Show(string.Format("Unable to parse file: {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                }
                else
                {
                    Logger.Error("invalid file selected");
                    MessageBox.Show("Invalid file selected", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                Logger.Debug("items in update package: {0}", pkg.Count);

                if (pkg.Count < 1)
                {
                    Logger.Error("update package does not contain any updates");
                    MessageBox.Show("Update package does not contain any updates", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                cboAvailPkg.Items.Clear();
                UpdItm.Clear();

                int cboIndex = 0;
                int updIndex = 0;

                foreach (Update cdu in pkg)
                {
                    foreach (Model cdm in cdu.ProductModel)
                    {
                        foreach (string cdr in cdm.Revision)
                        {
                            UpdateItem temp = new UpdateItem();
                            temp.PkgIndex = updIndex;

                            if (cdm.Name == "KFD100")
                            {
                                temp.MdlId = 0x01;
                            }
                            else if (cdm.Name == "KFD200")
                            {
                                temp.MdlId = 0x02;
                            }
                            else
                            {
                                Logger.Error("unknown model: {0}", cdm.Name);
                                MessageBox.Show(string.Format("Unknown model: {0}", cdm.Name), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                                return;
                            }

                            try
                            {
                                Version ver = new Version(cdr);

                                if (ver.Major < 0 || ver.Major > 255)
                                {
                                    Logger.Error("invalid major version: {0}", ver.Major);
                                    MessageBox.Show(string.Format("Invalid major version: {0}", ver.Major), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                                    return;
                                }

                                temp.HwRevMaj = (byte)ver.Major;

                                if (ver.Minor < 0 || ver.Minor > 255)
                                {
                                    Logger.Error("invalid minor version: {0}", ver.Minor);
                                    MessageBox.Show(string.Format("Invalid minor version: {0}", ver.Minor), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                                    return;
                                }

                                temp.HwRevMin = (byte)ver.Minor;
                            }
                            catch (Exception ex)
                            {
                                Logger.Error("unable to parse hardware version: {0}", ex.Message);
                                MessageBox.Show(string.Format("Unable to parse hardware version: {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                                return;
                            }

                            UpdItm.Add(cboIndex, temp);
                            cboAvailPkg.Items.Add(string.Format("Model: {0}, Hardware Revision: {1}", cdm.Name, cdr));

                            cboIndex++;
                        }
                    }

                    updIndex++;
                }

                cboAvailPkg.SelectedIndex = 0;
                cboAvailPkg.IsEnabled = true;
                btnInit.IsEnabled = true;
            }
        }

        private void Initialize_Button_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show(string.Format("This function is only to be used to bootstrap blank hardware that you built yourself{0}{0}You should only use this function if you know what it does{0}{0}Continue?", Environment.NewLine), "Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.No)
            {
                return;
            }

            Logger.Debug("update package path: {0}", UpdatePackage);

            if (UpdatePackage.Equals(string.Empty))
            {
                Logger.Error("no file selected");
                MessageBox.Show("No file selected", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // file existed when it was selected but now doesn't
            if (!File.Exists(UpdatePackage))
            {
                Logger.Error("selected file does not exist");
                MessageBox.Show("Selected file does not exist", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                UpdatePackage = string.Empty;
                lblPath.Content = NO_FILE_SELECTED;
                return;
            }

            // check if package is cfp or ufp
            string ext = System.IO.Path.GetExtension(UpdatePackage);

            Logger.Debug("update package extension: {0}", ext);

            List<Update> pkg = null;

            if (ext == ".cfp")
            {
                Logger.Debug("opening compressed update package");

                try
                {
                    pkg = Firmware.OpenCompressedUpdatePackage(UpdatePackage);
                }
                catch (Exception ex)
                {
                    Logger.Error("unable to parse file: {0}", ex.Message);
                    MessageBox.Show(string.Format("Unable to parse file: {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }
            else if (ext == ".ufp")
            {
                Logger.Debug("opening uncompressed update package");

                try
                {
                    pkg = Firmware.OpenUncompressedUpdatePackage(UpdatePackage);
                }
                catch (Exception ex)
                {
                    Logger.Error("unable to parse file: {0}", ex.Message);
                    MessageBox.Show(string.Format("Unable to parse file: {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }
            else
            {
                Logger.Error("invalid file selected");
                MessageBox.Show("Invalid file selected", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            Logger.Debug("items in update package: {0}", pkg.Count);

            if (pkg.Count < 1)
            {
                Logger.Error("update package does not contain any updates");
                MessageBox.Show("Update package does not contain any updates", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            int selIndex = cboAvailPkg.SelectedIndex;

            if (selIndex < 0)
            {
                Logger.Error("invalid selected update: {0}", selIndex);
                MessageBox.Show(string.Format("Invalid selected update: {0}", selIndex), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            UpdateItem itm = UpdItm[selIndex];

            Update fw = pkg[itm.PkgIndex];

            if (fw != null)
            {
                Logger.Debug("found applicable firmware update in package");
            }
            else
            {
                Logger.Error("could not find applicable firmware update in package");
                MessageBox.Show("Could not find applicable firmware update in package", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            Logger.Debug("firmware audience: {0}", fw.Audience);

            // warn the user if the firmware audience is anything other than release
            if (fw.Audience != "RELEASE")
            {
                string message = string.Format(
                    "THIS FIRMWARE IS NOT FOR GENERAL USE{0}" +
                    "{0}" +
                    "IT MAY NOT WORK CORRECTLY OR AT ALL{0}" +
                    "{0}" +
                    "Audience: {1}{0}" +
                    "{0}" +
                    "DO NOT PROCEED UNLESS YOU HAVE BEEN{0}" +
                    "DIRECTED TO DO SO IN ORDER TO TROUBLESHOOT AN ISSUE{0}" +
                    "{0}" +
                    "Continue?",
                    Environment.NewLine, fw.Audience);

                if (MessageBox.Show(message, "Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.No)
                {
                    return;
                }
            }

            // check if any devices are in app mode
            int appDeviceCount = ManualDetection.DetectConnectedAppDevices().Count;

            Logger.Debug("app devices detected: {0}", appDeviceCount);

            // check if any devices are in bsl mode
            int bslDeviceCount = ManualDetection.DetectConnectedBslDevices();

            Logger.Debug("bsl devices detected: {0}", bslDeviceCount);

            // there should not be more than one device attached at the same time
            if (appDeviceCount > 0)
            {
                Logger.Error("one or more devices is connected in APP mode");
                MessageBox.Show("One or more devices is connected in APP mode", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if (bslDeviceCount > 1)
            {
                Logger.Error("more than one device is connected in bsl mode");
                MessageBox.Show("More than one device is connected in BSL mode", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if (bslDeviceCount == 1)
            {
                Logger.Debug("device is already in bsl mode");
            }
            else
            {
                Logger.Debug("no device is connected in bsl mode");
                MessageBox.Show("No device is connected in BSL mode", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (MessageBox.Show(string.Format("Make sure that you have selected the correct model and hardware revision - an incorrect choice may damage the device{0}{0}Continue?", Environment.NewLine), "Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.No)
            {
                return;
            }

            Logger.Debug("updating device");

            bool updateResult = false;

            try
            {
                FirmwareUpdate upd = new FirmwareUpdate();

                updateResult = upd.UpdateDevice(fw);
            }
            catch (Exception ex)
            {
                Logger.Error("failed to load firmware: {0}", ex.Message);
                MessageBox.Show(string.Format("Failed to load firmware: {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (updateResult)
            {
                Logger.Debug("update success");
            }
            else
            {
                Logger.Error("update failed");
                MessageBox.Show("Firmware update failed", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            Thread.Sleep(5000); // TODO do this better - added to fix catching the device as it first connects and is not ready yet

            List<string> connected = ManualDetection.DetectConnectedAppDevices();

            if (connected.Count > 1)
            {
                Logger.Error("more than one device detected");
                MessageBox.Show("More than one device detected", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if (connected.Count == 1)
            {
                Logger.Debug("one device found");
            }
            else
            {
                Logger.Error("no device detected");
                MessageBox.Show("No device detected", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            try
            {
                AdapterProtocol ap = new AdapterProtocol(connected[0]);

                ap.Open();

                ap.Clear();

                ap.WriteInfo(itm.MdlId, itm.HwRevMaj, itm.HwRevMin);

                ap.Close();
            }
            catch (Exception ex)
            {
                Logger.Error("failed to write model and hardware revision: {0}", ex.Message);
                MessageBox.Show(string.Format("Failed to write model and hardware revision: {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            MessageBox.Show("Firmware Updated Successfully", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
