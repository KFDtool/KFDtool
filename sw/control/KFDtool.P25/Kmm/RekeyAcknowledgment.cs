using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    /* TIA 102.AACA-A 10.2.26 */
    public class RekeyAcknowledgment : KmmBody
    {
        public MessageId MessageIdAcknowledged { get; set; }

        public int NumberOfItems { get; set; }

        public List<KeyStatus> Keys { get; set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.RekeyAcknowledgment;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public RekeyAcknowledgment()
        {
            Keys = new List<KeyStatus>();
        }

        public override byte[] ToBytes()
        {
            List<byte> contents = new List<byte>();

            /* message id */
            contents.Add((byte)MessageIdAcknowledged);

            /* number of items */
            contents.Add((byte)NumberOfItems);

            /* items */
            foreach (KeyStatus status in Keys)
            {
                contents.AddRange(status.ToBytes());
            }

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length < 2)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected at least 2, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* message id */
            MessageIdAcknowledged = (MessageId)contents[0];

            /* number of items */
            NumberOfItems |= contents[1];

            /* items */
            if ((NumberOfItems == 0) && (contents.Length == 2))
            {
                return;
            }
            else if (((NumberOfItems * 4) % (contents.Length - 2)) == 0)
            {
                for (int i = 0; i < NumberOfItems; i++)
                {
                    byte[] status = new byte[4];
                    Array.Copy(contents, 2 + (i * 4), status, 0, 4);
                    KeyStatus status2 = new KeyStatus();
                    status2.Parse(status);
                    Keys.Add(status2);
                }
            }
            else
            {
                throw new Exception("number of items field and length mismatch");
            }
        }
    }
}
