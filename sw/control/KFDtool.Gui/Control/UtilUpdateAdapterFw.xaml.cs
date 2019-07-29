using KFDtool.Adapter.Bundle;
using KFDtool.Adapter.Device;
using KFDtool.P25.TransferConstructs;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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
    /// Interaction logic for UtilUpdateAdapterFw.xaml
    /// </summary>
    public partial class UtilUpdateAdapterFw : UserControl
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        private const string NO_FILE_SELECTED = "No File Selected";

        private string UpdatePackage;

        public UtilUpdateAdapterFw()
        {
            InitializeComponent();

            UpdatePackage = string.Empty;
            lblPath.Content = NO_FILE_SELECTED;
        }

        private void Select_Button_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Compressed Fw Pkg (*.cfp)|*.cfp|Uncompressed Fw Pkg (*.ufp)|*.ufp|All files (*.*)|*.*";

            if (openFileDialog.ShowDialog() == true)
            {
                UpdatePackage = openFileDialog.FileName;
                lblPath.Content = UpdatePackage;
            }
        }

        private void Update_Button_Click(object sender, RoutedEventArgs e)
        {
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

            string curVerStr = string.Empty;

            try
            {
                curVerStr = Interact.ReadFirmwareVersion(Settings.Port);
                Logger.Debug("adapter version: {0}", curVerStr);
            }
            catch (Exception ex)
            {
                Logger.Error("error reading firmware version from adapter -- {0}", ex.Message);
                MessageBox.Show(string.Format("Error reading firmware version from adapter -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            string curModel = string.Empty;

            try
            {
                curModel = Interact.ReadModel(Settings.Port);
                Logger.Debug("adapter model: {0}", curModel);
            }
            catch (Exception ex)
            {
                Logger.Error("error reading model from adapter -- {0}", ex.Message);
                MessageBox.Show(string.Format("Error reading model from adapter -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            string curHwRev = string.Empty;

            try
            {
                curHwRev = Interact.ReadHardwareRevision(Settings.Port);
                Logger.Debug("adapter hardware revision: {0}", curHwRev);
            }
            catch (Exception ex)
            {
                Logger.Error("error reading hardware revision from adapter -- {0}", ex.Message);
                MessageBox.Show(string.Format("Error reading hardware revision from adapter -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            Version curVersion = new Version(curVerStr);

            // select the correct firmware update item to install based on hardware

            Update fw = null;

            foreach (Update cdu in pkg)
            {
                Logger.Trace("update");

                foreach (Model cdm in cdu.ProductModel)
                {
                    Logger.Trace("model: {0}", cdm.Name);

                    if (cdm.Name == curModel)
                    {
                        foreach (string cdr in cdm.Revision)
                        {
                            Logger.Trace("revision: {0}", cdr);

                            if (cdr == curHwRev)
                            {
                                fw = cdu;
                            }
                        }
                    }
                }
            }

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

            Version updVersion = new Version(fw.AppVersion);

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

            // adapter already has current firmware
            if (updVersion.Equals(curVersion))
            {
                Logger.Error("the version of the firmware on the device is the same as the version that you are attempting to load");
                MessageBox.Show("The version of the firmware on the device is the same as the version that you are attempting to load", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // do not allow downgrades
            if (updVersion.CompareTo(curVersion) < 0)
            {
                Logger.Error("the version of the firmware on the device is newer than the version that you are attempting to load");
                MessageBox.Show("The version of the firmware on the device is newer than the version that you are attempting to load", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // check if any devices are in app mode
            int appDeviceCount = ManualDetection.DetectConnectedAppDevices().Count;

            Logger.Debug("app devices detected: {0}", appDeviceCount);

            // check if any devices are in bsl mode
            int bslDeviceCount = ManualDetection.DetectConnectedBslDevices();

            Logger.Debug("bsl devices detected: {0}", bslDeviceCount);

            // there should not be more than one device attached at the same time
            if (appDeviceCount > 1)
            {
                Logger.Error("more than one device is connected in app mode");
                MessageBox.Show("More than one device is connected in APP mode", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if (bslDeviceCount > 1)
            {
                Logger.Error("more than one device is connected in bsl mode");
                MessageBox.Show("More than one device is connected in BSL mode", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if (appDeviceCount == 1 && bslDeviceCount == 1)
            {
                Logger.Error("both a device in app mode and a device in bsl mode are connected");
                MessageBox.Show("Both a device in APP mode and a device in BSL mode are connected", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            else if (appDeviceCount == 1 && bslDeviceCount == 0)
            {
                Logger.Debug("requesting device to enter bsl mode");

                try
                {
                    Interact.EnterBslMode(Settings.Port);
                }
                catch (Exception ex)
                {
                    Logger.Error("error entering bsl mode -- {0}", ex.Message);
                    MessageBox.Show(string.Format("Error entering BSL mode -- {0}", ex.Message), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                Logger.Debug("device intends to enter bsl mode");
            }
            else if (appDeviceCount == 0 && bslDeviceCount == 1)
            {
                Logger.Debug("device is already in bsl mode");
            }
            else
            {
                Logger.Debug("no devices are connected in either app or bsl mode");
                MessageBox.Show("No devices are connected in either APP or BSL mode", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
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
                MessageBox.Show("Firmware Updated Successfully", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
            {
                Logger.Error("update failed");
                MessageBox.Show("Firmware update failed", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
