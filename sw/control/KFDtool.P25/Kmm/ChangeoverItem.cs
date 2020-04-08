using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    class ChangeoverItem
    {
        public int KeysetIdSuperseded { get; set; }
        public int KeysetIdActivated { get; set; }

        public ChangeoverItem()
        {
        }
    }
}
