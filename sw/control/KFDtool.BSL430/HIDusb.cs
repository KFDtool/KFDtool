using HidLibrary;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace KFDtool.BSL430
{
    public class HIDusb : INotifyPropertyChanged
    {
        public const string NOTIFY_CONNECTION_STATE = "notifyConnectionState";

        private Object ReadLock = new Object();
        private Object WriteLock = new Object();

        public bool IsConnected
        {
            get
            {
                if (_selectedDevice == null)
                    return false;
                if (!_selectedDevice.IsOpen || !_selectedDevice.IsConnected)
                    return false;

                return true;
            }
        }

        private int vid, pid;
        private HidFastReadDevice _selectedDevice = null;

        public delegate void ReadHandlerDelegate(HidReport report);
        public delegate void WriteHandlerDelegate(HidReport report);

        // List of packets to be sent through HID
        private List<byte[]> packetsOut = new List<byte[]>();
        public List<byte[]> PacketsOut
        {
            get
            {
                lock (WriteLock)
                {
                    return packetsOut;
                }
            }
        }
        private List<byte[]> packetsIn = new List<byte[]>();
        public List<byte[]> PacketsIn
        {
            get
            {
                lock (ReadLock)
                {
                    return packetsIn;
                }
            }
        }
        private bool sendingData = false;

        private int writeFailuresInARow = 0;

        // Worker handling the polling for USB device for connection
        private BackgroundWorker connectionWorker = new BackgroundWorker();

        [DllImport("hid.dll", SetLastError = true)]
        protected static extern Boolean HidD_GetNumInputBuffers(int HidDeviceObject, ref int NumberBuffers);
        [DllImport("hid.dll", SetLastError = true)]
        protected static extern Boolean HidD_SetNumInputBuffers(IntPtr HidDeviceObject, Int32 NumberBuffers);


        public HIDusb(int vid, int pid)
        {
            this.vid = vid;
            this.pid = pid;

            // Need a polling worker to handle connection if device is not already connected
            connectionWorker.WorkerSupportsCancellation = true;
            connectionWorker.DoWork += connectionWorker_DoWork;

            autoReconnectToHidDevice();

            // If not successfully connected, start the worker for polling every 500 ms
            if (_selectedDevice == null || !_selectedDevice.IsConnected)
                connectionWorker.RunWorkerAsync();
        }

        public void Close()
        {
            // Cancel the connection worker if active
            connectionWorker.CancelAsync();

            // De-register the events
            _selectedDevice.Inserted -= Device_Inserted;
            _selectedDevice.Removed -= Device_Removed;

            // Nullify the selected device
            _selectedDevice.CloseDevice();
            _selectedDevice = null;
        }

        private void ResetConnection()
        {
            this.Close();

            autoReconnectToHidDevice();

            // If not successfully connected, start the worker for polling every 500 ms
            if (_selectedDevice == null || !_selectedDevice.IsConnected)
                connectionWorker.RunWorkerAsync();
        }

        private void autoReconnectToHidDevice()
        {
            // No need to reconnect if device already connected
            if (_selectedDevice != null && _selectedDevice.IsConnected)
                return;

            // Try to connect to the device, if any
            _selectedDevice = HidDevices.EnumerateFastRead(vid, pid).FirstOrDefault();
            if (_selectedDevice != null)
            {
                _selectedDevice.OpenDevice();
                HidD_SetNumInputBuffers(_selectedDevice.ReadHandle, 512);

                _selectedDevice.Inserted += Device_Inserted;
                _selectedDevice.Removed += Device_Removed;

                _selectedDevice.MonitorDeviceEvents = true;
            }
        }

        // Detecting when a USB device is connected is very difficult in Windows and C#
        // If, when the constructor is called, the device is not detected, this worker 
        // is to be called and run in the background an infinite loop until the device is
        // connected. This the worker ends. It is called only once, since the device is automatically
        // detected after a disconnection/reconnection of the USB cable.
        private void connectionWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while ((_selectedDevice == null || !_selectedDevice.IsConnected) &&
                    !connectionWorker.CancellationPending)
            {
                // Attempt connection again
                autoReconnectToHidDevice();

                // Try again in 500 ms
                Thread.Sleep(500);
            }

            // Up to this point, the device was either connected or the thread was cancelled
        }

        /// <summary>
        /// Divides the buffer into chunks of 62 bytes and continuously send them. First byte in, first out.
        /// </summary>
        /// <param name="data">Data to be sent</param>
        public void write(byte[] data)
        {
            if (data.Length == 0) return;

            lock (WriteLock)
            {
                // Count the number of packets to be sent
                int nbNewPackets = data.Length / 62;
                if (data.Length % 62 != 0) nbNewPackets++;

                // Create the packets
                for (int i = 0; i < nbNewPackets - 1 || (data.Length % 62 == 0 && i < nbNewPackets); i++)
                {
                    byte[] buffer = new byte[64];
                    buffer[0] = 0x3f;   // See Progammer's Guide: MSP430 USB API Stack for CDC/HID/MSC v2.0, page 39, section 7.3
                    buffer[1] = 62;
                    for (int j = 0; j < 62; j++)
                        buffer[j + 2] = data[j];
                    packetsOut.Add(buffer);
                }

                if (data.Length % 62 != 0)
                {
                    int len = data.Length % 62;

                    byte[] buffer = new byte[len + 2];
                    buffer[0] = 0x3f;   // See Progammer's Guide: MSP430 USB API Stack for CDC/HID/MSC v2.0, page 39, section 7.3
                    buffer[1] = (byte)len;
                    for (int j = 0; j < len; j++)
                        buffer[j + 2] = data[j];

                    packetsOut.Add(buffer);
                }

                // If there is only 1 packet, then no data is being sent
                // Therefore, initiate the sending of the packet
                if (!sendingData)
                {
                    sendingData = true;
                    _selectedDevice.Write(packetsOut.First(), OnWriteReport);
                }
            }
        }

        /// <summary>
        /// Returns the first incomming packet in memory and remove it from the list.
        /// Thread safe unsing Lock.
        /// Returns null if no data left.
        /// </summary>
        /// <returns></returns>
        public byte[] readNext()
        {
            lock (ReadLock)
            {
                if (packetsIn.Count == 0)
                    return null;

                byte[] buf = packetsIn.First();
                packetsIn.RemoveAt(0);
                return buf;
            }
        }

        // Fired when received data - or device disconnected
        private void OnReadReport(HidReport report)
        {
            lock (ReadLock)
            {
                // process your data here
                byte len = report.Data[0];

                if (len > 0)
                {
                    byte[] buf = new byte[len];
                    for (int i = 0; i < len; i++)
                        buf[i] = report.Data[i + 1];

                    packetsIn.Add(buf);

                    OnNewIncommingData(new EventArgs());
                }

                // we need to start listening again for more data
                if (_selectedDevice != null && _selectedDevice.IsConnected)
                    try { _selectedDevice.ReadReport(OnReadReport); }
                    catch { }
            }
        }

        // Fired after data was sent to HID device
        private void OnWriteReport(bool success)
        {
            lock (WriteLock)
            {
                if (packetsOut.Count > 0)
                {
                    // Remove the data afterwards, once it is properly sent
                    if (success)
                    {
                        packetsOut.RemoveAt(0);
                        writeFailuresInARow = 0;
                    }
                    else
                        writeFailuresInARow++;

                    // Sometimes, even if the device is connected and open, HIDLibrary fails to write reports.
                    // To solve this, the device must be reset. This is a long call (500 ms).
                    if (writeFailuresInARow > 3)
                    {
                        writeFailuresInARow = 0;
                        this.ResetConnection();
                        Thread.Sleep(500);
                    }

                    // If more data to send, send it
                    if (packetsOut.Count > 0)
                        _selectedDevice.Write(packetsOut.First(), OnWriteReport);
                    else
                        sendingData = false;
                }
            }
        }

        private void Device_Inserted()
        {
            // Start reading - when incoming data, calls the callback
            _selectedDevice.ReadReport(OnReadReport);
        }
        private void Device_Removed()
        {
            // The device was removed. However, there is no need to reset _selectedDevice
            // as it will trigger Device_Inserted() as soon as it is inserted once again.
            sendingData = false;
        }


        // INTERNAL EVENTS
        private void OnConnected(EventArgs e)
        {
            EventHandler handler = Connected;
            if (handler != null)
                handler(this, e);
        }
        private void OnDisconnected(EventArgs e)
        {
            EventHandler handler = Disconnected;
            if (handler != null)
                handler(this, e);
        }
        private void OnNewIncommingData(EventArgs e)
        {
            EventHandler handler = NewIncommingData;
            if (handler != null)
                handler(this, e);
        }

        // Events to notify the subscribers (Main Application) that the Battery value changed
        private void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
                handler(this, e);
        }
        private void OnPropertyChanged(string propertyName)
        {
            OnPropertyChanged(new PropertyChangedEventArgs(propertyName));
        }

        /// <summary>
        /// Event for the App to register to in order to be notified when one of the 
        /// accessible parameters is updated: battery value, received ACK, received message
        /// or connection status.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;
        public event EventHandler Connected;
        public event EventHandler Disconnected;
        public event EventHandler NewIncommingData;
    }
}
