using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.TransferConstructs
{
    public class Interact
    {
        public static string ReadAdapterProtocolVersion(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ReadAdapterProtocolVersion(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ReadAdapterProtocolVersion", device.DeviceType.ToString()));
            }
        }

        public static string ReadFirmwareVersion(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ReadFirmwareVersion(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ReadFirmwareVersion", device.DeviceType.ToString()));
            }
        }

        public static string ReadUniqueId(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ReadUniqueId(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ReadUniqueId", device.DeviceType.ToString()));
            }
        }

        public static string ReadModel(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ReadModel(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ReadModel", device.DeviceType.ToString()));
            }
        }

        public static string ReadHardwareRevision(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ReadHardwareRevision(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ReadHardwareRevision", device.DeviceType.ToString()));
            }
        }

        public static string ReadSerialNumber(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ReadSerialNumber(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ReadSerialNumber", device.DeviceType.ToString()));
            }
        }

        public static void EnterBslMode(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                InteractTwiKfdtool.EnterBslMode(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support EnterBslMode", device.DeviceType.ToString()));
            }
        }

        public static string SelfTest(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.SelfTest(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support SelfTest", device.DeviceType.ToString()));
            }
        }

        public static void CheckTargetMrConnection(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                InteractTwiKfdtool.CheckTargetMrConnection(device);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                InteractDliIp.CheckTargetMrConnection(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support CheckTargetMrConnection", device.DeviceType.ToString()));
            }
        }

        public static void Keyload(BaseDevice device, List<CmdKeyItem> keys)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                InteractTwiKfdtool.Keyload(device, keys);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                InteractDliIp.Keyload(device, keys);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support Keyload", device.DeviceType.ToString()));
            }
        }

        public static void EraseKey(BaseDevice device, List<CmdKeyItem> keys)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                InteractTwiKfdtool.EraseKey(device, keys);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                InteractDliIp.EraseKey(device, keys);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support EraseKey", device.DeviceType.ToString()));
            }
        }

        public static void EraseAllKeys(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                InteractTwiKfdtool.EraseAllKeys(device);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                InteractDliIp.EraseAllKeys(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support EraseAllKeys", device.DeviceType.ToString()));
            }
        }

        public static List<RspKeyInfo> ViewKeyInfo(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ViewKeyInfo(device);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.ViewKeyInfo(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ViewKeyInfo", device.DeviceType.ToString()));
            }
        }

        public static RspRsiInfo LoadConfig(BaseDevice device, int kmfRsi, int mnp)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.LoadConfig(device, kmfRsi, mnp);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.LoadConfig(device, kmfRsi, mnp);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support LoadConfig", device.DeviceType.ToString()));
            }
        }

        public static RspRsiInfo ChangeRsi(BaseDevice device, int rsiOld, int rsiNew, int mnp)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ChangeRsi(device, rsiOld, rsiNew, mnp);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.ChangeRsi(device, rsiOld, rsiNew, mnp);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ChangeRsi", device.DeviceType.ToString()));
            }
        }

        public static List<RspRsiInfo> ViewRsiItems(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ViewRsiItems(device);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.ViewRsiItems(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ViewRsiItems", device.DeviceType.ToString()));
            }
        }

        public static int ViewMnp(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ViewMnp(device);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.ViewMnp(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ViewMnp", device.DeviceType.ToString()));
            }
        }

        public static int ViewKmfRsi(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ViewKmfRsi(device);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.ViewKmfRsi(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ViewKmfRsi", device.DeviceType.ToString()));
            }
        }

        public static List<RspKeysetInfo> ViewKeysetTaggingInfo(BaseDevice device)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ViewKeysetTaggingInfo(device);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.ViewKeysetTaggingInfo(device);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ViewKeysetTaggingInfo", device.DeviceType.ToString()));
            }
        }

        public static RspChangeoverInfo ActivateKeyset(BaseDevice device, int keysetSuperseded, int keysetActivated)
        {
            if (device.DeviceType == BaseDevice.DeviceTypeOptions.TwiKfdtool)
            {
                return InteractTwiKfdtool.ActivateKeyset(device, keysetSuperseded, keysetActivated);
            }
            else if (device.DeviceType == BaseDevice.DeviceTypeOptions.DliIp)
            {
                return InteractDliIp.ActivateKeyset(device, keysetSuperseded, keysetActivated);
            }
            else
            {
                throw new Exception(string.Format("The device type {0} does not support ActivateKeyset", device.DeviceType.ToString()));
            }
        }
    }
}
