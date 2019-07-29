using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Gui
{
    class Settings
    {
        public static string AssemblyVersion { get; private set; }

        public static string AssemblyInformationalVersion { get; private set; }

        public static string Port { get; set; }

        static Settings()
        {
            AssemblyVersion = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            AssemblyInformationalVersion = FileVersionInfo.GetVersionInfo(Assembly.GetExecutingAssembly().Location).ProductVersion;
            Port = string.Empty;
        }
    }
}
