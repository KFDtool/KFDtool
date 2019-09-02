using KFDtool.Adapter.Protocol.Adapter;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.Cmd
{
    class Analyzer
    {
        public static void FreeRunRead(string port)
        {
            AdapterProtocol ap = new AdapterProtocol(port);

            Task.Run(() =>
            {
                Console.WriteLine("press any key to cancel...");
                Console.ReadKey();
                ap.Cancel();
            });

            try
            {
                ap.Open();

                while (true)
                {
                    byte data = ap.GetByte(0); // no timeout

                    Console.WriteLine("0x{0:X2}", data);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("fatal error: {0}", ex.Message);
            }
            finally
            {
                Console.WriteLine("closed serial port");
                ap.Close();
            }
        }
    }
}
