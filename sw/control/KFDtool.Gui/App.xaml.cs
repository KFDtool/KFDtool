using NLog;
using System;
using System.Windows;

namespace KFDtool.Gui
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private static Logger Log = LogManager.GetCurrentClassLogger();

        public App()
        {
            AppDomain currentDomain = AppDomain.CurrentDomain;
            currentDomain.UnhandledException += CurrentDomain_UnhandledException;
        }

        private void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Exception ex = (Exception)e.ExceptionObject;
            Log.Error("UnhandledException caught: {0}", ex.Message);
            Log.Error("UnhandledException StackTrace: {0}", ex.StackTrace);
            Log.Fatal("Runtime terminating: {0}", e.IsTerminating);
        }
    }
}
