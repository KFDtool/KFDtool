using System;
using System.Collections.Generic;
using System.Management;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Adapter.Device
{
    public class ManualDetection
    {
        private const string APP_USB_VID = "2047";
        private const string APP_USB_PID = "0A7C";

        private const string BSL_USB_VID = "2047";
        private const string BSL_USB_PID = "0200";

        public static List<string> DetectConnectedAppDevices()
        {
            List<string> devices = new List<string>();

            ManagementObjectSearcher searcher = new ManagementObjectSearcher(
                "root\\CIMV2",
                string.Format("SELECT * FROM Win32_PnPEntity WHERE DeviceID LIKE '%USB\\\\VID_{0}&PID_{1}%'", APP_USB_VID, APP_USB_PID)
                );

            foreach (ManagementObject queryObj in searcher.Get())
            {
                string caption = queryObj["Caption"].ToString();

                int captionIndex = caption.IndexOf("(COM");

                string captionInfo = caption.Substring(captionIndex + 1).TrimEnd(')');

                devices.Add(captionInfo);
            }

            return devices;
        }

        public static int DetectConnectedBslDevices()
        {
            List<string> devices = new List<string>();

            ManagementObjectSearcher searcher = new ManagementObjectSearcher(
                "root\\CIMV2",
                string.Format("SELECT * FROM Win32_PnPEntity WHERE DeviceID LIKE '%USB\\\\VID_{0}&PID_{1}%'", BSL_USB_VID, BSL_USB_PID)
                );

            int count = 0;

            foreach (ManagementObject queryObj in searcher.Get())
            {
                count++;
            }

            return count;
        }
    }
}
