using KFDtool.BSL430;
using KFDtool.Adapter.Bundle;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace KFDtool.Adapter.Device
{
    public class FirmwareUpdate
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        private FirmwareUpdater firmwareUpdater;

        private void OnProgressUpdated(object sender, EventArgs e)
        {
            Logger.Trace("progress: {0}", (int)(firmwareUpdater.Percentage * 100.0));
        }

        public bool UpdateDevice(Update updates)
        {
            bool status = false;

            string appFw = updates.GetAppDataString();
            string bslFw = updates.GetRamBslDataString();

            firmwareUpdater = new FirmwareUpdater(appFw, bslFw, false);

            firmwareUpdater.ProgressUpdated += OnProgressUpdated;

            int retryAttempt = 0;

            while (retryAttempt < 10)
            {
                Logger.Debug("retry attempt: {0}", retryAttempt);

                if (firmwareUpdater.Connected)
                {
                    Logger.Debug("device found");

                    firmwareUpdater.startFirmwareUpdate();

                    while (firmwareUpdater.Status == FirmwareUpdater.CONNECTED || firmwareUpdater.Status == FirmwareUpdater.SENDINGDATA)
                    {
                        Thread.Sleep(1000);
                    }

                    if (firmwareUpdater.Status == FirmwareUpdater.COMPLETE)
                    {
                        Logger.Debug("update completed");
                        status = true;
                        break;
                    }
                    else if (firmwareUpdater.Status == FirmwareUpdater.CANCELED)
                    {
                        Logger.Debug("update canceled");
                    }
                    else
                    {
                        Logger.Debug("update failed");
                    }
                }
                else
                {
                    Logger.Debug("device not found");
                }

                Thread.Sleep(1000);

                retryAttempt++;
            }

            return status;
        }
    }
}
