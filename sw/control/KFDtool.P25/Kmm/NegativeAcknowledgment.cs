using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    /* TIA 102.AACA-A 10.2.22 */
    public class NegativeAcknowledgment : KmmBody
    {
        public MessageId AcknowledgedMessageId { get; set; }

        public int MessageNumber { get; set; }

        public AckStatus Status { get; set; }

        public override MessageId MessageId
        {
            get
            {
                return MessageId.NegativeAcknowledgment;
            }
        }

        public override ResponseKind ResponseKind
        {
            get
            {
                return ResponseKind.None;
            }
        }

        public NegativeAcknowledgment()
        {
        }

        public override byte[] ToBytes()
        {
            List<byte> contents = new List<byte>();

            /* acknowledged message id */
            contents.Add((byte)AcknowledgedMessageId);

            /* message number */
            contents.Add((byte)((MessageNumber >> 8) & 0xFF));
            contents.Add((byte)(MessageNumber & 0xFF));

            /* status */
            contents.Add((byte)Status);

            return contents.ToArray();
        }

        public override void Parse(byte[] contents)
        {
            if (contents.Length != 4)
            {
                throw new ArgumentOutOfRangeException("contents", string.Format("length mismatch - expected 4, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            /* acknowledged message id */
            AcknowledgedMessageId = (MessageId)contents[0];

            /* message number */
            MessageNumber |= contents[1] << 8;
            MessageNumber |= contents[2];

            /* status */
            Status = (AckStatus)contents[3];
        }
    }
}
