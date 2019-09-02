using KFDtool.Adapter.Bundle;
using Mono.Options;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Cmd
{
    class Program
    {
        static void Main(string[] args)
        {
            bool create = false;
            string input = string.Empty;
            string output = string.Empty;
            string port = string.Empty;
            bool read = false;

            OptionSet commandLineOptions = new OptionSet
            {
                { "create", "create update file", v => create = v != null },
                { "input=", "input file", v => input = v },
                { "output=", "output file", v => output = v },
                { "port=", "port number", v => port = v },
                { "read", "free run read", v => read = v != null }
            };

            try
            {
                commandLineOptions.Parse(args);
            }
            catch (OptionException ex)
            {
                Console.WriteLine(ex.Message);
            }

            if (create)
            {
                if (input == string.Empty)
                {
                    Console.WriteLine("no input file specified");
                    return;
                }

                if (output == string.Empty)
                {
                    Console.WriteLine("no output file specified");
                    return;
                }

                Console.WriteLine("creating update file");

                try
                {
                    Firmware.GenerateUpdate(input, output);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("error when generating update file -- {0}", ex.Message);
                }
            }
            else if (read)
            {
                if (port == string.Empty)
                {
                    Console.WriteLine("no port number specified");
                    return;
                }

                Analyzer.FreeRunRead(port);
            }
            else
            {
                Console.WriteLine("no action specified");
            }

            Console.WriteLine("exiting");
        }
    }
}
