using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class ChangeoverCommand : KmmBody
    {
        // Only supports a single changeover at this time, however response handles multiple ketsets
        private int _keysetIdSuperseded;
        private int _keysetIdActivated;

        public int KeysetIdSuperseded
        {
            get
            {
                return _keysetIdSuperseded;
            }
            set
            {
                if (value < 1 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keysetIdSuperseded = value;
            }
        }

        public int KeysetIdActivated
        {
            get
            {
                return _keysetIdActivated;
            }
            set
            {
                if (value < 1 || value > 0xFF)
                {
                    throw new ArgumentOutOfRangeException();
                }

                _keysetIdActivated = value;
            }
        }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.ChangeoverCommand;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.Immediate;
            }
        }

        public ChangeoverCommand()
        {
        }

        public override byte[] ToBytes()
        {
            //List<byte> contents = new List<byte>();
            byte[] contents = new byte[3];

            /* number of instructions */
            contents[0] = 0x01;

            /* superseded keyset */
            contents[1] = (byte)(KeysetIdSuperseded);

            /* activated keyset */
            contents[2] = (byte)(KeysetIdActivated);

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            throw new NotImplementedException();
        }
    }
}
