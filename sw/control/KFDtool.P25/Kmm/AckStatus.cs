using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public enum AckStatus : byte
    {
        CommandWasNotPerformed = 0x01,
        ItemDoesNotExist = 0x02,
        InvalidMessageId = 0x03,
        InvalidMac = 0x04,
        InvalidMessageNumber = 0x07
    }
}
