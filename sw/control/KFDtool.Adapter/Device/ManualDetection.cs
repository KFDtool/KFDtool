using System;
using System.Collections.Generic;
using System.Management;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace KFDtool.Adapter.Device
{
    public class ManualDetection
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

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

                Logger.Trace("caption: {0}", caption);

                // match "COM10" from "KFDtool (COM10)"
                // do not match "KFDtool" which appears before the COM port is assigned
                Regex regex = new Regex(@"\((COM\d+)\)$");

                Match match = regex.Match(caption);

                if (match.Success)
                {
                    string port = match.Groups[1].ToString();

                    Logger.Trace("port: {0}", port);

                    devices.Add(port);
                }
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
