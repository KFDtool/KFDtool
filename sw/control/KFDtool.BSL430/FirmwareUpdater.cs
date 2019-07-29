using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace KFDtool.BSL430
{
    /* 
     * THE BSL430 AND HIDLIBRARY PROJECTS NEED TO BE REPLACED
     * 
     * FOR SOME REASON ANYTHING BUT THE SHIPPING VERSION OF HIDLIBRARY
     * DOES NOT WORK, AND THERE ARE MODIFICATIONS TO HIDLIBRARY NOT IN
     * THE PROJECT MASTER ON GITHUB
     * 
     * THE HIDLIBRARY VS PROJECT IS TAKEN FROM THE DECOMPILED HIDLIBRARY
     * BIN IN THE ORIGINAL BSL430 PROJECT AS THERE IS ONLY A X86 BIN
     * 
     * IT "WORKS" BUT IT NEEDS TO BE REPLACED
     */

    /*
    *   The firmware updater expects a device to connect with VID/PID 0x2407/0x0200.
    *   Once this device is connected, the updater updates the firmware using custom code and
    *   HIDusb library (HIDInterface).
    */
    public class FirmwareUpdater
    {
        public const int DISCONNECTED = 0;
        public const int CONNECTED = 1;
        public const int SENDINGDATA = 2;
        public const int CANCELED = 3;
        public const int COMPLETE = 4;

        // Used for Firmware Upgrade only
        private HIDusb usbBSL = new HIDusb(0x2047, 0x0200);
        private BackgroundWorker firmwareUpgradeWorker;

        // Event Handlers
        public event EventHandler StatusChanged;
        public event EventHandler ConnectionChanged;
        public event EventHandler ProgressUpdated;

        // PROPERTIES
        private int status = DISCONNECTED;
        public int Status
        {
            get { return status; }
            set
            {
                if (status != value)
                {
                    status = value;
                    OnStatusChanged(new EventArgs());
                }
            }
        }
        public bool Connected
        {
            get
            {
                if (usbBSL != null && usbBSL.IsConnected)
                    return true;
                return false;
            }
        }


        private double percentage;
        public double Percentage
        {
            get
            {
                return percentage;
            }
            set
            {
                if (value < 0) percentage = 0.0;
                else if (value > 1.0) percentage = 1.0;
                else percentage = value;

                // Notify an update on the progress
                OnProgressUpdated(new EventArgs());
            }
        }

        public string FirmwareBinary { get; set; }

        public string RamBslBinary { get; set; }

        private bool AutoStartUpdate;

        public FirmwareUpdater(string firmwareStr, string ramBslStr, bool autoStartUpdate)
        {
            // Save the firmware string
            FirmwareBinary = firmwareStr;

            // Save the RAM BSL string
            RamBslBinary = ramBslStr;

            AutoStartUpdate = autoStartUpdate;

            // Status is initially DISCONNECTED
            Status = DISCONNECTED;

            // Attach a function to the worker's DoWork and other events
            firmwareUpgradeWorker = new BackgroundWorker();
            firmwareUpgradeWorker.DoWork += firmwareUpgradeWorker_DoWork;
            firmwareUpgradeWorker.RunWorkerCompleted += firmwareUpgradeWorker_RunWorkerCompleted;

            // Attach events to USB connection
            usbBSL.Connected += OnUSBConnected;
            usbBSL.Disconnected += OnUSBDisconnected;

            // The device is expected to be DISCONNECTED
            // But in case it is already connected, run the update already
            if (usbBSL.IsConnected)
            {
                // Generate event notifying status change
                Status = CONNECTED;

                if (autoStartUpdate)
                    startFirmwareUpdate();
            }
        }

        /// <summary>
        /// Reads a text file containing the code to be uploaded.
        /// Returns a new FirmwareUpdater instance for the given code.
        /// </summary>
        /// <param name="firmwarePath"></param>
        /// <param name="ramBslPath"></param>
        /// <returns></returns>
        public static FirmwareUpdater FromTextHexFile(string firmwarePath, string ramBslPath)
        {
            string fwStr = File.ReadAllText(firmwarePath);
            string bslStr = File.ReadAllText(ramBslPath);
            return new FirmwareUpdater(fwStr, bslStr, true);
        }

        // To be called at the end of the firmware update by the UI
        public void ResetConnectionStatus()
        {
            if (usbBSL.IsConnected)
                Status = CONNECTED;
            else
                Status = DISCONNECTED;
        }

        public void startFirmwareUpdate()
        {
            // The connection was detected.
            // De-register connection event. If the UI needs to re-attempt firmware update,
            // if will call a new instance of FirmwareUpdater.
            usbBSL.Connected -= OnUSBConnected;

            // Since this function can be called twice, verify that
            // the background worker is not alredy running. And then run it.
            if (!firmwareUpgradeWorker.IsBusy)
                firmwareUpgradeWorker.RunWorkerAsync();
        }

        // Firmware Update Background Work
        private void firmwareUpgradeWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            Percentage = 0;

            // Load and parse the binary source for RAM_BSL to be loaded in RAM, and the actual firmware code
            BinaryTextFileParser ram_bsl = new BinaryTextFileParser(RamBslBinary);
            BinaryTextFileParser firmware = new BinaryTextFileParser(FirmwareBinary);

            // append bor to firmware
            firmware.sections.Add(new CodeSection());
            firmware.sections.Last().StartAddress = 0x0120;
            firmware.sections.Last().AppendBinaryCode(new byte[] { 0xA5, 0x06 });

            // Clear the incomming messages from device
            while (usbBSL.readNext() != null) ;

            // 1. PASSWORD
            // Provide the empty password: will trigger a mass erase !!
            byte[] password = new byte[32];
            for (int i = 0; i < password.Length; i++) password[i] = 0xFF;
            byte[] cmd = BSLCoreCommandParser.RXPassword(password);

            int msg;
            do
            {
                usbBSL.write(cmd);

                while (usbBSL.PacketsIn.Count() == 0) Thread.Sleep(100);
                Thread.Sleep(200);

                // Read the incoming data
                msg = BSLCoreCommandParser.readMessage(usbBSL.readNext());

            } while (msg != BSLCoreCommandParser.MSG_SUCCESSFULL);

            // 2. LOAD RAM-BSL to RAM
            List<byte[]> cmds = BSLCoreCommandParser.listOfRxDataBlock(ram_bsl.sections, 62, true);
            foreach (byte[] command in cmds)
            {
                // The commands are RXDataBlockFast, with no response from the device
                usbBSL.write(command);
            }
            // Wait for all packets to be sent (not really necessary)
            while (usbBSL.PacketsOut.Count > 0) Thread.Sleep(100);

            // 3. START EXECUTING RAM BSL
            // At this point, USB connection may be lost and reconnected immediately after
            // TODO: test and check no wrong event is triggered
            usbBSL.Disconnected -= OnUSBDisconnected;

            cmd = BSLCoreCommandParser.LoadPC(0x2504);
            usbBSL.write(cmd);
            // Wait for all packets to be sent
            while (usbBSL.PacketsOut.Count > 0) Thread.Sleep(100);
            Thread.Sleep(500);

            // HID should reset automatically
            usbBSL.Close();
            usbBSL = new HIDusb(0x2047, 0x0200);
            while (!usbBSL.IsConnected) Thread.Sleep(100);

            // 4. UPLOAD NEW FIRMWARE
            Status = SENDINGDATA;

            usbBSL.Disconnected += OnUSBDisconnected;
            // Send a mass erase, in case the password was correct
            usbBSL.write(BSLCoreCommandParser.MassErase());
            while (usbBSL.PacketsOut.Count > 0) Thread.Sleep(20);

            // Send the firmware data
            int nbCmdsSent = 0;
            cmds = BSLCoreCommandParser.listOfRxDataBlock(firmware.sections, 62, false);
            foreach (byte[] command in cmds)
            {
                bool commandSucceeded;
                do
                {
                    commandSucceeded = true;

                    // The commands are RXDataBlock with response from the device
                    usbBSL.write(command);

                    // Estimate percentage of advance
                    Percentage = (double)nbCmdsSent / (double)cmds.Count;

                    // Expect a feedback after each write command. Long, but secure execution.
                    while (usbBSL.PacketsOut.Count > 0 && commandSucceeded)
                    {
                        if (usbBSL.PacketsOut[0] == null)
                        {
                            usbBSL.PacketsOut.RemoveAt(0);
                            commandSucceeded = false;
                        }
                        System.Threading.Thread.Sleep(1);
                    }


                    // Read the feedback
                    while (usbBSL.PacketsIn.Count > 0)
                    {
                        if (BSLCoreCommandParser.readMessage(usbBSL.PacketsIn[0]) != BSLCoreCommandParser.MSG_SUCCESSFULL)
                        {
                            commandSucceeded = false;
                        }
                        usbBSL.PacketsIn.RemoveAt(0);
                    }
                } while (!commandSucceeded);

                nbCmdsSent++;
            }

            Status = COMPLETE;
            usbBSL.Disconnected -= OnUSBDisconnected;

            // Program was uploaded. Need now to reset
            int resetAddress = firmware.ResetAddress();
            cmd = BSLCoreCommandParser.LoadPC(resetAddress);
            usbBSL.write(cmd);
            while (usbBSL.PacketsOut.Count > 0) Thread.Sleep(100);

            // TODO: manage the threads and event of the USB HID
            // Re-connecting generates issues
        }
        private void firmwareUpgradeWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            Percentage = 0.0;
        }

        // EXTERNAL EVENTS
        private void OnUSBConnected(object sender, EventArgs e)
        {
            // Generate event notifying status change
            Status = CONNECTED;
            OnConnectionChanged(new EventArgs());

            // Trigger the update on connection event
            if (AutoStartUpdate)
                startFirmwareUpdate();
        }
        private void OnUSBDisconnected(object sender, EventArgs e)
        {
            // If this event is fired, the device was disconnected BEFORE the end
            // of the firmware update. Fire an error.
            Status = CANCELED;
            OnConnectionChanged(new EventArgs());

            // De-register event. If the GUI wants to try the update again, it will
            // call a new instance of updater
            usbBSL.Disconnected -= OnUSBDisconnected;
        }

        // INTERNAL EVENTS
        private void OnStatusChanged(EventArgs e)
        {
            EventHandler handler = StatusChanged;
            if (handler != null)
                handler(this, e);
        }
        private void OnConnectionChanged(EventArgs e)
        {
            EventHandler handler = ConnectionChanged;
            if (handler != null)
                handler(this, e);
        }
        private void OnProgressUpdated(EventArgs e)
        {
            EventHandler handler = ProgressUpdated;
            if (handler != null)
                handler(this, e);
        }
    }
}
