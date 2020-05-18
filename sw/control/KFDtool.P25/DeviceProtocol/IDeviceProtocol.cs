using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.DeviceProtocol
{
    public interface IDeviceProtocol
    {
        void SendKeySignature();

        void InitSession();

        void CheckTargetMrConnection();

        void EndSession();

        byte[] PerformKmmTransfer(byte[] kmm);
    }
}
