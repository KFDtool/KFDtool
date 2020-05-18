using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.TransferConstructs
{
    public class DliIpDevice
    {
        public enum ProtocolOptions
        {
            UDP
        }

        public enum VariantOptions
        {
            Standard,
            Motorola
        }

        public ProtocolOptions Protocol { get; set; }

        public string Hostname { get; set; }

        public int Port { get; set; }

        public VariantOptions Variant { get; set; }
    }
}
