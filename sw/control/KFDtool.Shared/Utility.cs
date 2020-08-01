using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;

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

        public static string DataFormat(byte b)
        {
            return string.Format("{0:X2}", b);
        }

        public static string DataFormat(List<byte> b)
        {
            return BitConverter.ToString(b.ToArray());
        }

        public static byte[] Compress(byte[] data)
        {
            byte[] buffer = data;
            var memoryStream = new MemoryStream();
            using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Compress, true))
            {
                gZipStream.Write(buffer, 0, buffer.Length);
            }

            memoryStream.Position = 0;

            var compressedData = new byte[memoryStream.Length];
            memoryStream.Read(compressedData, 0, compressedData.Length);

            var gZipBuffer = new byte[compressedData.Length + 4];
            Buffer.BlockCopy(compressedData, 0, gZipBuffer, 4, compressedData.Length);
            Buffer.BlockCopy(BitConverter.GetBytes(buffer.Length), 0, gZipBuffer, 0, 4);
            return gZipBuffer;
        }

        public static byte[] Decompress(byte[] data)
        {
            byte[] gZipBuffer = data;
            using (var memoryStream = new MemoryStream())
            {
                int dataLength = BitConverter.ToInt32(gZipBuffer, 0);
                memoryStream.Write(gZipBuffer, 4, gZipBuffer.Length - 4);

                var buffer = new byte[dataLength];

                memoryStream.Position = 0;
                using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Decompress))
                {
                    gZipStream.Read(buffer, 0, buffer.Length);
                }

                return buffer;
            }
        }
    }
}
