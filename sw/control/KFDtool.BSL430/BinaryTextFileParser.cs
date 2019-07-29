using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.BSL430
{
    public class BinaryTextFileParser
    {
        /*
        *   This class takes in input a text file containing the binary
        *   code to be uploaded to the MSP430. It generates a list of 
        *   CodeSection instances, each containg a portion of the code to be
        *   loaded.
        */

        private const int RVECTOR = 0xFFFE;

        // Outputs
        public List<CodeSection> sections = new List<CodeSection>();
        // After parsing, if file is indeed a program code, returns true. False otherwise.
        private bool formatCorrect = true;
        public bool IsFileFormatConform()
        {
            if (!formatCorrect)
                return false;

            if (sections.Count <= 0)
                return false;

            foreach (CodeSection section in sections)
            {
                if (section.binaryCode.Count() <= 0)
                    return false;
            }

            return true;
        }

        public BinaryTextFileParser(string text)
        {
            try
            {
                StringReader strReader = new StringReader(text);

                string line = strReader.ReadLine();
                while (line != null)
                {
                    // End of the text read
                    if (line.Contains("q"))
                        return;

                    else if (line.Contains("@"))
                    {
                        // Read the line
                        int address = readAddressLine(line);

                        // Create a new code section
                        sections.Add(new CodeSection());
                        sections.Last().StartAddress = address;
                    }
                    else
                    {
                        // Read the binary code
                        byte[] bLine = readBinaryCodeLine(line);

                        // Add this to the section
                        sections.Last().AppendBinaryCode(bLine);
                    }

                    line = strReader.ReadLine();
                }
            }
            catch
            {
                formatCorrect = false;
            }
        }

        public static BinaryTextFileParser FromTextFile(string filename)
        {
            string str = File.ReadAllText(filename);
            return new BinaryTextFileParser(str);
        }

        // Returns the address (if found) of the program reset, contained in Reset Vector (0xFFFE)
        public int ResetAddress()
        {
            foreach (CodeSection section in sections)
            {
                // If the section contains the reset vector
                if (section.StartAddress <= RVECTOR &&
                    section.StartAddress + section.binaryCode.Length >= RVECTOR + 1)
                {
                    byte[] address = new byte[4];

                    int i = RVECTOR - section.StartAddress;
                    address[0] = 0;
                    address[1] = 0;
                    address[2] = section.binaryCode[i + 1];
                    address[3] = section.binaryCode[i];

                    if (BitConverter.IsLittleEndian)
                        Array.Reverse(address);

                    return BitConverter.ToInt32(address, 0);
                }
            }

            return 0;
        }

        private byte[] readBinaryCodeLine(string line)
        {
            return StringToByteArray(line.Replace(" ", ""));
        }

        private int readAddressLine(string line)
        {
            string addressText = line.Replace("@", string.Empty);

            int nbAdditionalCharacters = 8 - addressText.Length;
            int i;
            for (i = 0; i < nbAdditionalCharacters; i++)
                addressText = "0" + addressText;

            byte[] byteAddress = StringToByteArray(addressText);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(byteAddress);

            i = BitConverter.ToInt32(byteAddress, 0);
            return i;
        }

        private static byte[] StringToByteArray(string hex)
        {
            return Enumerable.Range(0, hex.Length)
                             .Where(x => x % 2 == 0)
                             .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                             .ToArray();
        }
    }
}
