using KFDtool.P25.DataLinkIndependent;
using KFDtool.P25.ManualRekey;
using KFDtool.P25.NetworkProtocol;
using System;
using System.Collections.Generic;

namespace KFDtool.P25.TransferConstructs
{
    public class InteractDliIp
    {
        private static DataLinkIndependentProtocol GetDli(BaseDevice device)
        {
            if (device.DliIpDevice.Protocol == DliIpDevice.ProtocolOptions.UDP)
            {
                int timeout = 5000;

                UdpProtocol udpProtocol = new UdpProtocol(device.DliIpDevice.Hostname, device.DliIpDevice.Port, timeout);

                bool motVariant;

                if (device.DliIpDevice.Variant == DliIpDevice.VariantOptions.Standard)
                {
                    motVariant = false;
                }
                else if (device.DliIpDevice.Variant == DliIpDevice.VariantOptions.Motorola)
                {
                    motVariant = true;
                }
                else
                {
                    throw new ArgumentOutOfRangeException("Variant");
                }

                return new DataLinkIndependentProtocol(udpProtocol, motVariant);
            }
            else
            {
                throw new ArgumentOutOfRangeException("Protocol");
            }
        }

        private static ManualRekeyApplication GetMra(BaseDevice device)
        {
            if (device.DliIpDevice.Protocol == DliIpDevice.ProtocolOptions.UDP)
            {
                int timeout = 5000;

                UdpProtocol udpProtocol = new UdpProtocol(device.DliIpDevice.Hostname, device.DliIpDevice.Port, timeout);

                bool motVariant;

                if (device.DliIpDevice.Variant == DliIpDevice.VariantOptions.Standard)
                {
                    motVariant = false;
                }
                else if (device.DliIpDevice.Variant == DliIpDevice.VariantOptions.Motorola)
                {
                    motVariant = true;
                }
                else
                {
                    throw new ArgumentOutOfRangeException("Variant");
                }

                return new ManualRekeyApplication(udpProtocol, motVariant);
            }
            else
            {
                throw new ArgumentOutOfRangeException("Protocol");
            }
        }

        public static void CheckTargetMrConnection(BaseDevice device)
        {
            try
            {
                DataLinkIndependentProtocol dli = GetDli(device);

                dli.CheckTargetMrConnection();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static void Keyload(BaseDevice device, List<CmdKeyItem> keys)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                mra.Keyload(keys);
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static void EraseKey(BaseDevice device, List<CmdKeyItem> keys)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                mra.EraseKeys(keys);
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static void EraseAllKeys(BaseDevice device)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                mra.EraseAllKeys();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static List<RspKeyInfo> ViewKeyInfo(BaseDevice device)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.ViewKeyInfo();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static RspRsiInfo LoadConfig(BaseDevice device, int kmfRsi, int mnp)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.LoadConfig(kmfRsi, mnp);
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static RspRsiInfo ChangeRsi(BaseDevice device, int rsiOld, int rsiNew, int mnp)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.ChangeRsi(rsiOld, rsiNew, mnp);
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static List<RspRsiInfo> ViewRsiItems(BaseDevice device)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.ViewRsiItems();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static int ViewMnp(BaseDevice device)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.ViewMnp();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static int ViewKmfRsi(BaseDevice device)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.ViewKmfRsi();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static List<RspKeysetInfo> ViewKeysetTaggingInfo(BaseDevice device)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.ViewKeysetTaggingInfo();
            }
            catch (Exception)
            {
                throw;
            }
        }

        public static RspChangeoverInfo ActivateKeyset(BaseDevice device, int keysetSuperseded, int keysetActivated)
        {
            try
            {
                ManualRekeyApplication mra = GetMra(device);

                return mra.ActivateKeyset(keysetSuperseded, keysetActivated);
            }
            catch (Exception)
            {
                throw;
            }
        }
    }
}
