using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.BSL430
{
    /*
    *   This class contains a portion binary code to be written to
    *   the MSP430 and the start address. The code can be made of 
    *   several portions, involving several instances of CodeSection.
    */
    public class CodeSection
    {
        // Outputs
        public int StartAddress;
        public byte[] binaryCode = null;

        public void AppendBinaryCode(byte[] data)
        {
            if (binaryCode == null)
            {
                binaryCode = data;
                return;
            }

            byte[] rv = new byte[binaryCode.Length + data.Length];
            System.Buffer.BlockCopy(binaryCode, 0, rv, 0, binaryCode.Length);
            System.Buffer.BlockCopy(data, 0, rv, binaryCode.Length, data.Length);
            binaryCode = rv;
        }
    }
}
