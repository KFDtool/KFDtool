using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Kmm
{
    public class KmmFrame
    {
        public KmmBody KmmBody { get; private set; }

        // TODO src rsi

        // TODO dest rsi

        public KmmFrame(KmmBody kmmBody)
        {
            if (kmmBody == null)
            {
                throw new ArgumentNullException("kmmBody");
            }

            KmmBody = kmmBody;
        }

        public KmmFrame(byte[] contents)
        {
            Parse(contents);
        }

        public byte[] ToBytes()
        {
            byte[] body = KmmBody.ToBytes();

            int length = 10 + body.Length;

            byte[] frame = new byte[length];

            /* message id */
            frame[0] = (byte)KmmBody.MessageId;

            /* message length */
            int messageLength = 7 + body.Length;
            frame[1] = (byte)((messageLength >> 8) & 0xFF);
            frame[2] = (byte)(messageLength & 0xFF);

            /* message format */
            BitArray messageFormat = new BitArray(8, false);
            messageFormat.Set(7, Convert.ToBoolean(((byte)KmmBody.ResponseKind & 0x02) >> 1));
            messageFormat.Set(6, Convert.ToBoolean((byte)KmmBody.ResponseKind & 0x01));
            messageFormat.CopyTo(frame, 3);

            /* destination rsi */
            frame[4] = 0xFF;
            frame[5] = 0xFF;
            frame[6] = 0xFF;

            /* source rsi */
            frame[7] = 0xFF;
            frame[8] = 0xFF;
            frame[9] = 0xFF;

            /* message body */
            Array.Copy(body, 0, frame, 10, body.Length);

            return frame;
        }

        private void Parse(byte[] contents)
        {
            if (contents.Length < 10)
            {
                throw new ArgumentOutOfRangeException(string.Format("length mismatch - expected at least 10, got {0} - {1}", contents.Length.ToString(), BitConverter.ToString(contents)));
            }

            byte messageId = contents[0];

            int messageLength = 0;
            messageLength |= contents[1] << 8;
            messageLength |= contents[2];

            int messageBodyLength = messageLength - 7;
            byte[] messageBody = new byte[messageBodyLength];
            Array.Copy(contents, 10, messageBody, 0, messageBodyLength);

            if ((MessageId)messageId == MessageId.InventoryCommand)
            {
                if (messageBody.Length > 0)
                {
                    InventoryType inventoryType = (InventoryType)messageBody[0];

                    if (inventoryType == InventoryType.ListActiveKsetIds)
                    {
                        KmmBody kmmBody = new InventoryCommandListActiveKsetIds();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;

                    }
                    else if (inventoryType == InventoryType.ListRsiItems)
                    {
                        KmmBody kmmBody = new InventoryCommandListRsiItems();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;
                    }
                    else if (inventoryType == InventoryType.ListActiveKeys)
                    {
                        KmmBody kmmBody = new InventoryCommandListActiveKeys();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;
                    }
                    else
                    {
                        throw new Exception(string.Format("unknown inventory command type: 0x{0:X2}", (byte)inventoryType));
                    }
                }
                else
                {
                    throw new Exception("inventory command length zero");
                }
            }
            else if ((MessageId)messageId == MessageId.InventoryResponse)
            {
                if (messageBody.Length > 0)
                {
                    InventoryType inventoryType = (InventoryType)messageBody[0];

                    if (inventoryType == InventoryType.ListActiveKsetIds)
                    {
                        KmmBody kmmBody = new InventoryResponseListActiveKsetIds();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;

                    }
                    else if (inventoryType == InventoryType.ListActiveKeys)
                    {
                        KmmBody kmmBody = new InventoryResponseListActiveKeys();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;
                    }
                    else if (inventoryType == InventoryType.ListRsiItems)
                    {
                        //cg
                        KmmBody kmmBody = new InventoryResponseListRsiItems();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;
                    }
                    else if (inventoryType == InventoryType.ListMnp)
                    {
                        //cg
                        KmmBody kmmBody = new InventoryResponseListMnp();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;
                    }
                    else if (inventoryType == InventoryType.ListKmfRsi)
                    {
                        //cg
                        KmmBody kmmBody = new InventoryResponseListKmfRsi();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;
                    }
                    else if (inventoryType == InventoryType.ListKeysetTaggingInfo)
                    {
                        //cg
                        KmmBody kmmBody = new InventoryResponseListKeysetTaggingInfo();
                        kmmBody.Parse(messageBody);
                        KmmBody = kmmBody;
                    }
                    else
                    {
                        throw new Exception(string.Format("unknown inventory response type: 0x{0:X2}", (byte)inventoryType));
                    }
                }
                else
                {
                    throw new Exception("inventory response length zero");
                }
            }
            else if ((MessageId)messageId == MessageId.ModifyKeyCommand)
            {
                KmmBody kmmBody = new ModifyKeyCommand();
                kmmBody.Parse(messageBody);
                KmmBody = kmmBody;
            }
            else if ((MessageId)messageId == MessageId.NegativeAcknowledgment)
            {
                KmmBody kmmBody = new NegativeAcknowledgment();
                kmmBody.Parse(messageBody);
                KmmBody = kmmBody;
            }
            else if ((MessageId)messageId == MessageId.RekeyAcknowledgment)
            {
                KmmBody kmmBody = new RekeyAcknowledgment();
                kmmBody.Parse(messageBody);
                KmmBody = kmmBody;
            }
            else if ((MessageId)messageId == MessageId.ZeroizeResponse)
            {
                KmmBody kmmBody = new ZeroizeResponse();
                kmmBody.Parse(messageBody);
                KmmBody = kmmBody;
            }
            else if ((MessageId)messageId == MessageId.LoadConfigResponse)
            {
                //cg
                KmmBody kmmBody = new LoadConfigResponse();
                kmmBody.Parse(messageBody);
                KmmBody = kmmBody;
            }
            else if ((MessageId)messageId == MessageId.ChangeRsiResponse)
            {
                //cg
                KmmBody kmmBody = new ChangeRsiResponse();
                kmmBody.Parse(messageBody);
                KmmBody = kmmBody;
            }
            else if ((MessageId)messageId == MessageId.ChangeoverResponse)
            {
                //cg
                KmmBody kmmBody = new ChangeoverResponse();
                kmmBody.Parse(messageBody);
                KmmBody = kmmBody;
            }
            else
            {
                throw new Exception(string.Format("unknown kmm - message id: 0x{0:X2}", messageId));
            }
        }
    }
}
