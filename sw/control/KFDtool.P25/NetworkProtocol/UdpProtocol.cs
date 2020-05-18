using System.Net;
using System.Net.Sockets;

namespace KFDtool.P25.NetworkProtocol
{
    public class UdpProtocol
    {
        private string IpAddress;

        private int PortNumber;

        private int Timeout;

        public UdpProtocol(string ipAddress, int portNumber, int timeout)
        {
            IpAddress = ipAddress;
            PortNumber = portNumber;
            Timeout = timeout;
        }

        public byte[] TxRx(byte[] toRadio)
        {
            using (UdpClient udpClient = new UdpClient(IpAddress, PortNumber))
            {
                udpClient.Client.SendTimeout = Timeout;
                udpClient.Client.ReceiveTimeout = Timeout;
                udpClient.Send(toRadio, toRadio.Length);
                IPEndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
                byte[] fromRadio = udpClient.Receive(ref remoteEndPoint);
                return fromRadio;
            }
        }
    }
}
