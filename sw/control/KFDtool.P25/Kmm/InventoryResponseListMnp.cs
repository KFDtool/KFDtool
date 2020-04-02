using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class InventoryResponseListMnp : KmmBody
    {
        public int MessageNumberPeriod { get; private set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.InventoryResponse;
            }
        }

        public InventoryType InventoryType
        {
            get
            {
                return InventoryType.ListMnp;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public InventoryResponseListMnp()
        {

        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length != 3)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected 3, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* inventory type */
            if (contents[0] != (byte)InventoryType)
            {
                throw new Exception("inventory type mismatch");
            }

            /* message number period */
            MessageNumberPeriod |= contents[1] << 8;
            MessageNumberPeriod |= contents[2];
        }
    }
}
