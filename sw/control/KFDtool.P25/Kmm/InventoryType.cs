using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public enum InventoryType : byte
    {
        ListActiveKsetIds = 0x02,
        ListActiveKeys = 0xFD
    }
}
