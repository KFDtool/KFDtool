using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace HidLibrary
{
    public class HidDevice : IHidDevice, IDisposable
    {
        protected delegate HidDeviceData ReadDelegate();

        protected delegate HidReport ReadReportDelegate();

        private delegate bool WriteDelegate(byte[] data);

        private delegate bool WriteReportDelegate(HidReport report);

        private readonly string _description;

        private readonly string _devicePath;

        private readonly HidDeviceAttributes _deviceAttributes;

        private readonly HidDeviceCapabilities _deviceCapabilities;

        private DeviceMode _deviceReadMode = DeviceMode.NonOverlapped;

        private DeviceMode _deviceWriteMode = DeviceMode.NonOverlapped;

        private readonly HidDeviceEventMonitor _deviceEventMonitor;

        private bool _monitorDeviceEvents;

        public IntPtr ReadHandle
        {
            get;
            private set;
        }

        public IntPtr WriteHandle
        {
            get;
            private set;
        }

        public bool IsOpen
        {
            get;
            private set;
        }

        public bool IsConnected => HidDevices.IsConnected(_devicePath);

        public string Description => _description;

        public HidDeviceCapabilities Capabilities => _deviceCapabilities;

        public HidDeviceAttributes Attributes => _deviceAttributes;

        public string DevicePath => _devicePath;

        public bool MonitorDeviceEvents
        {
            get
            {
                return _monitorDeviceEvents;
            }
            set
            {
                if (value & !_monitorDeviceEvents)
                {
                    _deviceEventMonitor.Init();
                }
                _monitorDeviceEvents = value;
            }
        }

        public event InsertedEventHandler Inserted;

        public event RemovedEventHandler Removed;

        internal HidDevice(string devicePath, string description = null)
        {
            _deviceEventMonitor = new HidDeviceEventMonitor(this);
            _deviceEventMonitor.Inserted += DeviceEventMonitorInserted;
            _deviceEventMonitor.Removed += DeviceEventMonitorRemoved;
            _devicePath = devicePath;
            _description = description;
            try
            {
                IntPtr intPtr = OpenDeviceIO(_devicePath, 0u);
                _deviceAttributes = GetDeviceAttributes(intPtr);
                _deviceCapabilities = GetDeviceCapabilities(intPtr);
                CloseDeviceIO(intPtr);
            }
            catch (Exception innerException)
            {
                throw new Exception($"Error querying HID device '{devicePath}'.", innerException);
            }
        }

        public override string ToString()
        {
            return $"VendorID={_deviceAttributes.VendorHexId}, ProductID={_deviceAttributes.ProductHexId}, Version={_deviceAttributes.Version}, DevicePath={_devicePath}";
        }

        public void OpenDevice()
        {
            OpenDevice(DeviceMode.NonOverlapped, DeviceMode.NonOverlapped);
        }

        public void OpenDevice(DeviceMode readMode, DeviceMode writeMode)
        {
            if (!IsOpen)
            {
                _deviceReadMode = readMode;
                _deviceWriteMode = writeMode;
                try
                {
                    ReadHandle = OpenDeviceIO(_devicePath, readMode, 2147483648u);
                    WriteHandle = OpenDeviceIO(_devicePath, writeMode, 1073741824u);
                }
                catch (Exception innerException)
                {
                    IsOpen = false;
                    throw new Exception("Error opening HID device.", innerException);
                }
                IntPtr intPtr = ReadHandle;
                bool num = intPtr.ToInt32() != -1;
                intPtr = WriteHandle;
                IsOpen = (num & (intPtr.ToInt32() != -1));
            }
        }

        public void CloseDevice()
        {
            if (IsOpen)
            {
                CloseDeviceIO(ReadHandle);
                CloseDeviceIO(WriteHandle);
                IsOpen = false;
            }
        }

        public HidDeviceData Read()
        {
            return Read(0);
        }

        public void Read(ReadCallback callback)
        {
            ReadDelegate readDelegate = Read;
            HidAsyncState @object = new HidAsyncState(readDelegate, callback);
            readDelegate.BeginInvoke(EndRead, @object);
        }

        public HidDeviceData Read(int timeout)
        {
            if (IsConnected)
            {
                if (!IsOpen)
                {
                    OpenDevice();
                }
                try
                {
                    return ReadData(timeout);
                }
                catch
                {
                    return new HidDeviceData(HidDeviceData.ReadStatus.ReadError);
                }
            }
            return new HidDeviceData(HidDeviceData.ReadStatus.NotConnected);
        }

        public void ReadReport(ReadReportCallback callback)
        {
            ReadReportDelegate readReportDelegate = ReadReport;
            HidAsyncState @object = new HidAsyncState(readReportDelegate, callback);
            readReportDelegate.BeginInvoke(EndReadReport, @object);
        }

        public HidReport ReadReport(int timeout)
        {
            return new HidReport(Capabilities.InputReportByteLength, Read(timeout));
        }

        public HidReport ReadReport()
        {
            return ReadReport(0);
        }

        public bool ReadFeatureData(out byte[] data, byte reportId = 0)
        {
            if (_deviceCapabilities.FeatureReportByteLength > 0)
            {
                data = new byte[_deviceCapabilities.FeatureReportByteLength];
                byte[] array = CreateFeatureOutputBuffer();
                array[0] = reportId;
                IntPtr intPtr = IntPtr.Zero;
                bool flag = false;
                try
                {
                    intPtr = OpenDeviceIO(_devicePath, 0u);
                    flag = NativeMethods.HidD_GetFeature(intPtr, array, array.Length);
                    if (flag)
                    {
                        Array.Copy(array, 0, data, 0, Math.Min(data.Length, _deviceCapabilities.FeatureReportByteLength));
                    }
                }
                catch (Exception innerException)
                {
                    throw new Exception($"Error accessing HID device '{_devicePath}'.", innerException);
                }
                finally
                {
                    if (intPtr != IntPtr.Zero)
                    {
                        CloseDeviceIO(intPtr);
                    }
                }
                return flag;
            }
            data = new byte[0];
            return false;
        }

        public bool ReadProduct(out byte[] data)
        {
            data = new byte[64];
            IntPtr intPtr = IntPtr.Zero;
            bool result = false;
            try
            {
                intPtr = OpenDeviceIO(_devicePath, 0u);
                result = NativeMethods.HidD_GetProductString(intPtr, ref data[0], data.Length);
            }
            catch (Exception innerException)
            {
                throw new Exception($"Error accessing HID device '{_devicePath}'.", innerException);
            }
            finally
            {
                if (intPtr != IntPtr.Zero)
                {
                    CloseDeviceIO(intPtr);
                }
            }
            return result;
        }

        public bool ReadManufacturer(out byte[] data)
        {
            data = new byte[64];
            IntPtr intPtr = IntPtr.Zero;
            bool result = false;
            try
            {
                intPtr = OpenDeviceIO(_devicePath, 0u);
                result = NativeMethods.HidD_GetManufacturerString(intPtr, ref data[0], data.Length);
            }
            catch (Exception innerException)
            {
                throw new Exception($"Error accessing HID device '{_devicePath}'.", innerException);
            }
            finally
            {
                if (intPtr != IntPtr.Zero)
                {
                    CloseDeviceIO(intPtr);
                }
            }
            return result;
        }

        public bool ReadSerialNumber(out byte[] data)
        {
            data = new byte[64];
            IntPtr intPtr = IntPtr.Zero;
            bool result = false;
            try
            {
                intPtr = OpenDeviceIO(_devicePath, 0u);
                result = NativeMethods.HidD_GetSerialNumberString(intPtr, ref data[0], data.Length);
            }
            catch (Exception innerException)
            {
                throw new Exception($"Error accessing HID device '{_devicePath}'.", innerException);
            }
            finally
            {
                if (intPtr != IntPtr.Zero)
                {
                    CloseDeviceIO(intPtr);
                }
            }
            return result;
        }

        public void Write(byte[] data, WriteCallback callback)
        {
            WriteDelegate writeDelegate = Write;
            HidAsyncState @object = new HidAsyncState(writeDelegate, callback);
            writeDelegate.BeginInvoke(data, EndWrite, @object);
        }

        public bool Write(byte[] data)
        {
            return Write(data, 0);
        }

        public bool Write(byte[] data, int timeout)
        {
            if (IsConnected)
            {
                if (!IsOpen)
                {
                    OpenDevice();
                }
                try
                {
                    return WriteData(data, timeout);
                }
                catch
                {
                    return false;
                }
            }
            return false;
        }

        public void WriteReport(HidReport report, WriteCallback callback)
        {
            WriteReportDelegate writeReportDelegate = WriteReport;
            HidAsyncState @object = new HidAsyncState(writeReportDelegate, callback);
            writeReportDelegate.BeginInvoke(report, EndWriteReport, @object);
        }

        public bool WriteReport(HidReport report)
        {
            return WriteReport(report, 0);
        }

        public bool WriteReport(HidReport report, int timeout)
        {
            return Write(report.GetBytes(), timeout);
        }

        public HidReport CreateReport()
        {
            return new HidReport(Capabilities.OutputReportByteLength);
        }

        public bool WriteFeatureData(byte[] data)
        {
            if (_deviceCapabilities.FeatureReportByteLength > 0)
            {
                byte[] array = CreateFeatureOutputBuffer();
                Array.Copy(data, 0, array, 0, Math.Min(data.Length, _deviceCapabilities.FeatureReportByteLength));
                IntPtr intPtr = IntPtr.Zero;
                bool result = false;
                try
                {
                    intPtr = OpenDeviceIO(_devicePath, 0u);
                    result = NativeMethods.HidD_SetFeature(intPtr, array, array.Length);
                }
                catch (Exception innerException)
                {
                    throw new Exception($"Error accessing HID device '{_devicePath}'.", innerException);
                }
                finally
                {
                    if (intPtr != IntPtr.Zero)
                    {
                        CloseDeviceIO(intPtr);
                    }
                }
                return result;
            }
            return false;
        }

        protected static void EndRead(IAsyncResult ar)
        {
            HidAsyncState hidAsyncState = (HidAsyncState)ar.AsyncState;
            ReadDelegate readDelegate = (ReadDelegate)hidAsyncState.CallerDelegate;
            ReadCallback readCallback = (ReadCallback)hidAsyncState.CallbackDelegate;
            HidDeviceData data = readDelegate.EndInvoke(ar);
            readCallback?.Invoke(data);
        }

        protected static void EndReadReport(IAsyncResult ar)
        {
            HidAsyncState hidAsyncState = (HidAsyncState)ar.AsyncState;
            ReadReportDelegate readReportDelegate = (ReadReportDelegate)hidAsyncState.CallerDelegate;
            ReadReportCallback readReportCallback = (ReadReportCallback)hidAsyncState.CallbackDelegate;
            HidReport report = readReportDelegate.EndInvoke(ar);
            readReportCallback?.Invoke(report);
        }

        private static void EndWrite(IAsyncResult ar)
        {
            HidAsyncState hidAsyncState = (HidAsyncState)ar.AsyncState;
            WriteDelegate writeDelegate = (WriteDelegate)hidAsyncState.CallerDelegate;
            WriteCallback writeCallback = (WriteCallback)hidAsyncState.CallbackDelegate;
            bool success = writeDelegate.EndInvoke(ar);
            writeCallback?.Invoke(success);
        }

        private static void EndWriteReport(IAsyncResult ar)
        {
            HidAsyncState hidAsyncState = (HidAsyncState)ar.AsyncState;
            WriteReportDelegate writeReportDelegate = (WriteReportDelegate)hidAsyncState.CallerDelegate;
            WriteCallback writeCallback = (WriteCallback)hidAsyncState.CallbackDelegate;
            bool success = writeReportDelegate.EndInvoke(ar);
            writeCallback?.Invoke(success);
        }

        private byte[] CreateInputBuffer()
        {
            return CreateBuffer(Capabilities.InputReportByteLength - 1);
        }

        private byte[] CreateOutputBuffer()
        {
            return CreateBuffer(Capabilities.OutputReportByteLength - 1);
        }

        private byte[] CreateFeatureOutputBuffer()
        {
            return CreateBuffer(Capabilities.FeatureReportByteLength - 1);
        }

        private static byte[] CreateBuffer(int length)
        {
            byte[] array = null;
            Array.Resize(ref array, length + 1);
            return array;
        }

        private static HidDeviceAttributes GetDeviceAttributes(IntPtr hidHandle)
        {
            NativeMethods.HIDD_ATTRIBUTES attributes = default(NativeMethods.HIDD_ATTRIBUTES);
            attributes.Size = Marshal.SizeOf((object)attributes);
            NativeMethods.HidD_GetAttributes(hidHandle, ref attributes);
            return new HidDeviceAttributes(attributes);
        }

        private static HidDeviceCapabilities GetDeviceCapabilities(IntPtr hidHandle)
        {
            NativeMethods.HIDP_CAPS capabilities = default(NativeMethods.HIDP_CAPS);
            IntPtr preparsedData = default(IntPtr);
            if (NativeMethods.HidD_GetPreparsedData(hidHandle, ref preparsedData))
            {
                NativeMethods.HidP_GetCaps(preparsedData, ref capabilities);
                NativeMethods.HidD_FreePreparsedData(preparsedData);
            }
            return new HidDeviceCapabilities(capabilities);
        }

        private bool WriteData(byte[] data, int timeout)
        {
            if (_deviceCapabilities.OutputReportByteLength > 0)
            {
                byte[] array = CreateOutputBuffer();
                uint lpNumberOfBytesWritten = 0u;
                Array.Copy(data, 0, array, 0, Math.Min(data.Length, _deviceCapabilities.OutputReportByteLength));
                if (_deviceWriteMode != DeviceMode.Overlapped)
                {
                    try
                    {
                        NativeOverlapped lpOverlapped = default(NativeOverlapped);
                        return NativeMethods.WriteFile(WriteHandle, array, (uint)array.Length, out lpNumberOfBytesWritten, ref lpOverlapped);
                    }
                    catch
                    {
                        return false;
                    }
                }
                NativeMethods.SECURITY_ATTRIBUTES securityAttributes = default(NativeMethods.SECURITY_ATTRIBUTES);
                NativeOverlapped lpOverlapped2 = default(NativeOverlapped);
                int dwMilliseconds = (timeout <= 0) ? 65535 : timeout;
                securityAttributes.lpSecurityDescriptor = IntPtr.Zero;
                securityAttributes.bInheritHandle = true;
                securityAttributes.nLength = Marshal.SizeOf((object)securityAttributes);
                lpOverlapped2.OffsetLow = 0;
                lpOverlapped2.OffsetHigh = 0;
                lpOverlapped2.EventHandle = NativeMethods.CreateEvent(ref securityAttributes, Convert.ToInt32(false), Convert.ToInt32(true), "");
                try
                {
                    NativeMethods.WriteFile(WriteHandle, array, (uint)array.Length, out lpNumberOfBytesWritten, ref lpOverlapped2);
                }
                catch
                {
                    return false;
                }
                switch (NativeMethods.WaitForSingleObject(lpOverlapped2.EventHandle, dwMilliseconds))
                {
                    case 0u:
                        return true;
                    case 258u:
                        return false;
                    case uint.MaxValue:
                        return false;
                    default:
                        return false;
                }
            }
            return false;
        }

        protected HidDeviceData ReadData(int timeout)
        {
            byte[] array = new byte[0];
            HidDeviceData.ReadStatus status = HidDeviceData.ReadStatus.NoDataRead;
            if (_deviceCapabilities.InputReportByteLength > 0)
            {
                uint lpNumberOfBytesRead = 0u;
                array = CreateInputBuffer();
                if (_deviceReadMode != DeviceMode.Overlapped)
                {
                    try
                    {
                        NativeOverlapped lpOverlapped = default(NativeOverlapped);
                        NativeMethods.ReadFile(ReadHandle, array, (uint)array.Length, out lpNumberOfBytesRead, ref lpOverlapped);
                        status = HidDeviceData.ReadStatus.Success;
                    }
                    catch
                    {
                        status = HidDeviceData.ReadStatus.ReadError;
                    }
                }
                else
                {
                    NativeMethods.SECURITY_ATTRIBUTES securityAttributes = default(NativeMethods.SECURITY_ATTRIBUTES);
                    NativeOverlapped lpOverlapped2 = default(NativeOverlapped);
                    int dwMilliseconds = (timeout <= 0) ? 65535 : timeout;
                    securityAttributes.lpSecurityDescriptor = IntPtr.Zero;
                    securityAttributes.bInheritHandle = true;
                    securityAttributes.nLength = Marshal.SizeOf((object)securityAttributes);
                    lpOverlapped2.OffsetLow = 0;
                    lpOverlapped2.OffsetHigh = 0;
                    lpOverlapped2.EventHandle = NativeMethods.CreateEvent(ref securityAttributes, Convert.ToInt32(false), Convert.ToInt32(true), string.Empty);
                    try
                    {
                        NativeMethods.ReadFile(ReadHandle, array, (uint)array.Length, out lpNumberOfBytesRead, ref lpOverlapped2);
                        switch (NativeMethods.WaitForSingleObject(lpOverlapped2.EventHandle, dwMilliseconds))
                        {
                            case 0u:
                                status = HidDeviceData.ReadStatus.Success;
                                break;
                            case 258u:
                                status = HidDeviceData.ReadStatus.WaitTimedOut;
                                array = new byte[0];
                                break;
                            case uint.MaxValue:
                                status = HidDeviceData.ReadStatus.WaitFail;
                                array = new byte[0];
                                break;
                            default:
                                status = HidDeviceData.ReadStatus.NoDataRead;
                                array = new byte[0];
                                break;
                        }
                    }
                    catch
                    {
                        status = HidDeviceData.ReadStatus.ReadError;
                    }
                    finally
                    {
                        CloseDeviceIO(lpOverlapped2.EventHandle);
                    }
                }
            }
            return new HidDeviceData(array, status);
        }

        private static IntPtr OpenDeviceIO(string devicePath, uint deviceAccess)
        {
            return OpenDeviceIO(devicePath, DeviceMode.NonOverlapped, deviceAccess);
        }

        private static IntPtr OpenDeviceIO(string devicePath, DeviceMode deviceMode, uint deviceAccess)
        {
            NativeMethods.SECURITY_ATTRIBUTES lpSecurityAttributes = default(NativeMethods.SECURITY_ATTRIBUTES);
            int dwFlagsAndAttributes = 0;
            if (deviceMode == DeviceMode.Overlapped)
            {
                dwFlagsAndAttributes = 1073741824;
            }
            lpSecurityAttributes.lpSecurityDescriptor = IntPtr.Zero;
            lpSecurityAttributes.bInheritHandle = true;
            lpSecurityAttributes.nLength = Marshal.SizeOf((object)lpSecurityAttributes);
            return NativeMethods.CreateFile(devicePath, deviceAccess, 3, ref lpSecurityAttributes, 3, dwFlagsAndAttributes, 0);
        }

        private static void CloseDeviceIO(IntPtr handle)
        {
            if (Environment.OSVersion.Version.Major > 5)
            {
                NativeMethods.CancelIoEx(handle, IntPtr.Zero);
            }
            NativeMethods.CloseHandle(handle);
        }

        private void DeviceEventMonitorInserted()
        {
            if (IsOpen)
            {
                OpenDevice();
            }
            if (this.Inserted != null)
            {
                this.Inserted();
            }
        }

        private void DeviceEventMonitorRemoved()
        {
            if (IsOpen)
            {
                CloseDevice();
            }
            if (this.Removed != null)
            {
                this.Removed();
            }
        }

        public void Dispose()
        {
            if (MonitorDeviceEvents)
            {
                MonitorDeviceEvents = false;
            }
            if (IsOpen)
            {
                CloseDevice();
            }
        }
    }
}
