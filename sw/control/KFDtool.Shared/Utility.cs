using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Shared
{
    public class Utility
    {
        public static List<byte> ByteStringToByteList(string hex)
        {
            int NumberChars = hex.Length;
            List<byte> bytes = new List<byte>();
            for (int i = 0; i < NumberChars; i += 2)
            {
                bytes.Add(Convert.ToByte(hex.Substring(i, 2), 16));
            }
            return bytes;
        }
    }
}
