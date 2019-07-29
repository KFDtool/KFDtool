using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Constant
{
    public enum AlgorithmId : byte
    {
        /* TIA-102.BAAC-D 2.8 */
        /* ALGID Guide 2015-04-15.pdf */
        ACCORDION = 0x00,
        BATON_ODD = 0x01,
        FIREFLY = 0x02,
        MAYFLY = 0x03,
        SAVILLE = 0x04,
        PADSTONE = 0x05,
        BATON_EVEN = 0x41,
        CLEAR = 0x80,
        DESOFB = 0x81,
        TDES = 0x83,
        AES256 = 0x84,
        AES128 = 0x85,
        DESXL = 0x9F,
        DVIXL = 0xA0,
        DVPXL = 0xA1,
        ADP = 0xAA
    }
}
