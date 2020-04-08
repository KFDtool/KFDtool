using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class ChangeoverResponse : KmmBody
    {
        public int KeysetIdSuperseded { get; private set; }
        public int KeysetIdActivated { get; private set; }

        //public List<ChangeoverItem> ChangeoverItems { get; private set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.ChangeoverResponse;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public ChangeoverResponse()
        {
        }

        public override byte[] ToBytes()
        {
            throw new NotImplementedException();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length < 1)
            {
                throw new ArgumentOutOfRangeException("contents", string.Format("length mismatch - expected at least 1, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* changeover responses */
            int responses = contents[0];

            for (int i = 0; i < responses; i++)
            {
                //ChangeoverItem item = new ChangeoverItem();

                /* superseded keyset */
                KeysetIdSuperseded = contents[1 + i * 2];
                //item.KeysetIdSuperseded = contents[1 + i * 2];

                /* activated keyset */
                KeysetIdActivated = contents[2 + i * 2];
                //item.KeysetIdActivated = contents[2 + i * 2];

                //ChangeoverItems.Add(item);
            }

        }
    }
}
