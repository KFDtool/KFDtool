using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public enum MessageId : byte
    {
        InventoryCommand = 0x0D,
        InventoryResponse = 0x0E,
        ModifyKeyCommand = 0x13,
        NegativeAcknowledgment = 0x16,
        RekeyAcknowledgment = 0x1D,
        ZeroizeCommand = 0x21,
        ZeroizeResponse = 0x22
    }
}
