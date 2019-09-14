using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Generator
{
    public class KeyGenerator
    {
        // internal method from .NET 4.8 refsrc system\security\cryptography\utils.cs FixupKeyParity()
        public static byte[] FixupKeyParity(byte[] key)
        {
            byte[] oddParityKey = new byte[key.Length];
            for (int index = 0; index < key.Length; index++)
            {
                // Get the bits we are interested in
                oddParityKey[index] = (byte)(key[index] & 0xfe);
                // Get the parity of the sum of the previous bits
                byte tmp1 = (byte)((oddParityKey[index] & 0xF) ^ (oddParityKey[index] >> 4));
                byte tmp2 = (byte)((tmp1 & 0x3) ^ (tmp1 >> 2));
                byte sumBitsMod2 = (byte)((tmp2 & 0x1) ^ (tmp2 >> 1));
                // We need to set the last bit in oddParityKey[index] to the negation
                // of the last bit in sumBitsMod2
                if (sumBitsMod2 == 0)
                    oddParityKey[index] |= 1;
            }
            return oddParityKey;
        }

        public static List<byte> GenerateVarKey(int keyLenBytes)
        {
            if (keyLenBytes < 1)
            {
                throw new ArgumentOutOfRangeException();
            }
            RNGCryptoServiceProvider rngCsp = new RNGCryptoServiceProvider();
            byte[] key = new byte[keyLenBytes];
            rngCsp.GetBytes(key);
            List<byte> key2 = new List<byte>();
            key2.AddRange(key);
            rngCsp.Dispose();
            return key2;
        }

        public static List<byte> GenerateSingleDesKey()
        {
            // DESCryptoServiceProvider.GenerateKey() does NOT create a DES key with the correct parity
            // https://social.msdn.microsoft.com/Forums/vstudio/en-US/8439cc0b-486b-4083-a35e-eeef3571b064/des-generated-key-incorrect-parity?forum=clr
            byte[] key = GenerateVarKey(8).ToArray();
            return FixupKeyParity(key).ToList();
        }
    }
}
