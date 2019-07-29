using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HidLibrary
{
    public static class Extensions
    {
        public static string ToUTF8String(this byte[] buffer)
        {
            string @string = Encoding.UTF8.GetString(buffer);
            return @string.Remove(@string.IndexOf('\0'));
        }

        public static string ToUTF16String(this byte[] buffer)
        {
            string @string = Encoding.Unicode.GetString(buffer);
            return @string.Remove(@string.IndexOf('\0'));
        }
    }
}
