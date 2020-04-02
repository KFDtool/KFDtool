using KFDtool.Adapter.Protocol.Adapter;
using KFDtool.P25.ManualRekey;
using KFDtool.P25.ThreeWire;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.TransferConstructs
{
    public class Interact
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        public static string ReadAdapterProtocolVersion(string port)
        {
            string version = string.Empty;

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                byte[] ver = ap.ReadAdapterProtocolVersion();

                version = string.Format("{0}.{1}.{2}", ver[0], ver[1], ver[2]);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return version;
        }

        public static string ReadFirmwareVersion(string port)
        {
            string version = string.Empty;

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                byte[] ver = ap.ReadFirmwareVersion();

                version = string.Format("{0}.{1}.{2}", ver[0], ver[1], ver[2]);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return version;
        }

        public static string ReadUniqueId(string port)
        {
            string uniqueId = string.Empty;

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                byte[] id = ap.ReadUniqueId();

                if (id.Length == 0)
                {
                    uniqueId = "NONE";
                }
                else
                {
                    uniqueId = BitConverter.ToString(id).Replace("-", string.Empty);

                }
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return uniqueId;
        }

        public static string ReadModel(string port)
        {
            string model = string.Empty;

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                byte mod = ap.ReadModelId();

                if (mod == 0x00)
                {
                    model = "NOT SET";
                }
                else if (mod == 0x01)
                {
                    model = "KFD100";
                }
                else
                {
                    model = "UNKNOWN";
                }
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return model;
        }

        public static string ReadHardwareRevision(string port)
        {
            string version = string.Empty;

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                byte[] ver = ap.ReadHardwareRevision();

                if (ver[0] == 0x00 && ver[1] == 0x00)
                {
                    version = "NOT SET";
                }
                else
                {
                    version = string.Format("{0}.{1}", ver[0], ver[1]);
                }
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return version;
        }

        public static string ReadSerialNumber(string port)
        {
            string serialNumber = string.Empty;

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                byte[] ser = ap.ReadSerialNumber();

                if (ser.Length == 0)
                {
                    serialNumber = "NONE";
                }
                else
                {
                    serialNumber = Encoding.ASCII.GetString(ser);
                }
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return serialNumber;
        }

        public static void EnterBslMode(string port)
        {
            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ap.EnterBslMode();
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }
        }

        public static string SelfTest(string port)
        {
            string result = string.Empty;

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                byte res = ap.SelfTest();

                if (res == 0x01)
                {
                    result = string.Format("Data shorted to ground (0x{0:X2})", res);
                }
                else if (res == 0x02)
                {
                    result = string.Format("Sense shorted to ground (0x{0:X2})", res);
                }
                else if (res == 0x03)
                {
                    result = string.Format("Data shorted to power (0x{0:X2})", res);
                }
                else if (res == 0x04)
                {
                    result = string.Format("Sense shorted to power (0x{0:X2})", res);
                }
                else if (res == 0x05)
                {
                    result = string.Format("Data and Sense shorted (0x{0:X2})", res);
                }
                else if (res == 0x06)
                {
                    result = string.Format("Sense and Data shorted (0x{0:X2})", res);
                }
                else if (res != 0x00)
                {
                    result = string.Format("Unknown self test result (0x{0:X2})", res);
                }
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static void CheckTargetMrConnection(string port)
        {
            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ThreeWireProtocol twp = new ThreeWireProtocol(ap);

                twp.CheckTargetMrConnection();
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }
        }

        public static void Keyload(string port, List<CmdKeyItem> keys)
        {
            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                mra.Keyload(keys);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }
        }

        public static void EraseKey(string port, List<CmdKeyItem> keys)
        {
            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                mra.EraseKeys(keys);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }
        }

        public static void EraseAllKeys(string port)
        {
            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                mra.EraseAllKeys();
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }
        }

        public static List<RspKeyInfo> ViewKeyInfo(string port)
        {
            List<RspKeyInfo> result = new List<RspKeyInfo>();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                result.AddRange(mra.ViewKeyInfo());
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static RspRsiInfo LoadConfig(string port, int kmfRsi, int mnp)
        {
            RspRsiInfo result = new RspRsiInfo();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                result = mra.LoadConfig(kmfRsi, mnp);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static RspRsiInfo ChangeRsi(string port, int rsiOld, int rsiNew, int mnp)
        {
            RspRsiInfo result = new RspRsiInfo();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                result = mra.ChangeRsi(rsiOld, rsiNew, mnp);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static List<RspRsiInfo> ViewRsiItems(string port)
        {
            List<RspRsiInfo> result = new List<RspRsiInfo>();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                result.AddRange(mra.ViewRsiItems());
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static int ViewMnp(string port)
        {
            int result = new int();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                result = mra.ViewMnp();
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static int ViewKmfRsi(string port)
        {
            int result = new int();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                result = mra.ViewKmfRsi();
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static List<RspKeysetInfo> ViewKeysetTaggingInfo(string port)
        {
            List<RspKeysetInfo> result = new List<RspKeysetInfo>();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                result = mra.ViewKeysetTaggingInfo();
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }

        public static RspChangeoverInfo ActivateKeyset(string port, int keysetSuperseded, int keysetActivated)
        {
            RspChangeoverInfo result = new RspChangeoverInfo();

            if (port == string.Empty)
            {
                throw new ArgumentException("port empty");
            }

            AdapterProtocol ap = null;

            try
            {
                ap = new AdapterProtocol(port);

                ap.Open();

                ap.Clear();

                ManualRekeyApplication mra = new ManualRekeyApplication(ap);

                //result = mra.LoadConfig(kmfRsi, mnp);
                result = mra.ActivateKeyset(keysetSuperseded, keysetActivated);
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                try
                {
                    if (ap != null)
                    {
                        ap.Close();
                    }
                }
                catch (System.IO.IOException ex)
                {
                    Logger.Warn("could not close serial port: {0}", ex.Message);
                }
            }

            return result;
        }
    }
}
