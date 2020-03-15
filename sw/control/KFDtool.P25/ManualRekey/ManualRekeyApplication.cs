using KFDtool.Adapter.Protocol.Adapter;
using KFDtool.P25.Kmm;
using KFDtool.P25.ThreeWire;
using KFDtool.P25.TransferConstructs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.ManualRekey
{
    public class ManualRekeyApplication
    {
        private static NLog.Logger Logger = NLog.LogManager.GetCurrentClassLogger();

        private ThreeWireProtocol ThreeWireProtocol;

        public ManualRekeyApplication(AdapterProtocol adapterProtocol)
        {
            ThreeWireProtocol = new ThreeWireProtocol(adapterProtocol);
        }

        private void Begin()
        {
            ThreeWireProtocol.SendKeySignature();

            ThreeWireProtocol.InitSession();
        }

        private KmmBody TxRxKmm(KmmBody commandKmmBody)
        {
            KmmFrame commandKmmFrame = new KmmFrame(commandKmmBody);

            byte[] toRadio = commandKmmFrame.ToBytes();

            Logger.Debug("KFD -> MR KMM FRAME: {0}", BitConverter.ToString(toRadio));

            byte[] fromRadio = ThreeWireProtocol.PerformKmmTransfer(toRadio);

            Logger.Debug("MR -> KFD KMM FRAME: {0}", BitConverter.ToString(fromRadio));

            KmmFrame responseKmmFrame = new KmmFrame(fromRadio);

            return responseKmmFrame.KmmBody;
        }

        private void End()
        {
            ThreeWireProtocol.EndSession();
        }

        /* TIA 102.AACD-A 3.8.1 */
        public void Keyload(bool useActiveKeyset, int keysetId, int sln, bool isKek, int keyId, int algId, List<byte> key)
        {
            Begin();

            try
            {
                InventoryCommandListActiveKsetIds cmdKmmBody1 = new InventoryCommandListActiveKsetIds();

                KmmBody rspKmmBody1 = TxRxKmm(cmdKmmBody1);

                int ksid = 0;

                if (rspKmmBody1 is InventoryResponseListActiveKsetIds)
                {
                    InventoryResponseListActiveKsetIds kmm = rspKmmBody1 as InventoryResponseListActiveKsetIds;

                    Logger.Debug("number of active keyset ids: {0}", kmm.KsetIds.Count);

                    for (int i = 0; i < kmm.KsetIds.Count; i++)
                    {
                        Logger.Debug("* keyset id index {0} *", i);
                        Logger.Debug("keyset id: {0} (dec), {0:X} (hex)", kmm.KsetIds[i]);
                    }

                    if (!useActiveKeyset)
                    {
                        ksid = keysetId;
                    }
                    else if (useActiveKeyset && isKek)
                    {
                        ksid = 0xFF;
                    }
                    else if (useActiveKeyset && kmm.KsetIds.Count > 0)
                    {
                        ksid = kmm.KsetIds[0];
                    }
                    else
                    {
                        ksid = 1; // to match KVL3000+ R3.53.03 behavior
                    }
                }
                else if (rspKmmBody1 is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = rspKmmBody1 as NegativeAcknowledgment;

                    string statusDescr = OperationStatusExtensions.ToStatusString(kmm.Status);
                    string statusReason = OperationStatusExtensions.ToReasonString(kmm.Status);
                    throw new Exception(string.Format("received negative acknowledgment{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, kmm.Status, statusReason));
                }
                else
                {
                    throw new Exception("unexpected kmm");
                }

                // TODO support more than one key per keyload operation

                KeyItem keyItem = new KeyItem();
                keyItem.SLN = sln;
                keyItem.KeyId = keyId;
                keyItem.Key = key.ToArray();
                keyItem.KEK = isKek;
                keyItem.Erase = false;

                ModifyKeyCommand modifyKeyCommand = new ModifyKeyCommand();

                modifyKeyCommand.KeysetId = ksid;

                modifyKeyCommand.AlgorithmId = algId;
                modifyKeyCommand.KeyItems.Add(keyItem);

                KmmBody rspKmmBody2 = TxRxKmm(modifyKeyCommand);

                if (rspKmmBody2 is RekeyAcknowledgment)
                {
                    RekeyAcknowledgment kmm = rspKmmBody2 as RekeyAcknowledgment;

                    Logger.Debug("number of key status: {0}", kmm.Keys.Count);

                    for (int i = 0; i < kmm.Keys.Count; i++)
                    {
                        KeyStatus status = kmm.Keys[i];

                        Logger.Debug("* key status index {0} *", i);
                        Logger.Debug("algorithm id: {0} (dec), {0:X} (hex)", status.AlgorithmId);
                        Logger.Debug("key id: {0} (dec), {0:X} (hex)", status.KeyId);
                        Logger.Debug("status: {0} (dec), {0:X} (hex)", status.Status);

                        if (status.Status != 0)
                        {
                            string statusDescr = OperationStatusExtensions.ToStatusString((OperationStatus)status.Status);
                            string statusReason = OperationStatusExtensions.ToReasonString((OperationStatus)status.Status);
                            throw new Exception(string.Format("received unexpected key status{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, status.Status, statusReason));
                        }
                    }
                }
                else if (rspKmmBody2 is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = rspKmmBody2 as NegativeAcknowledgment;

                    string statusDescr = OperationStatusExtensions.ToStatusString(kmm.Status);
                    string statusReason = OperationStatusExtensions.ToReasonString(kmm.Status);
                    throw new Exception(string.Format("received negative acknowledgment{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, kmm.Status, statusReason));
                }
                else
                {
                    throw new Exception("received unexpected kmm");
                }
            }
            catch
            {
                End();

                throw;
            }

            End();
        }

        /* TIA 102.AACD-A 3.8.2 */
        public void LoadIndividualRsi()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.3 */
        public void LoadKmfRsi()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.4 */
        public void LoadMnp()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.5 */
        public void EraseKeys(bool useActiveKeyset, int keysetId, int sln, bool isKek)
        {
            Begin();

            try
            {
                InventoryCommandListActiveKsetIds cmdKmmBody1 = new InventoryCommandListActiveKsetIds();

                KmmBody rspKmmBody1 = TxRxKmm(cmdKmmBody1);

                int ksid = 0;

                if (rspKmmBody1 is InventoryResponseListActiveKsetIds)
                {
                    InventoryResponseListActiveKsetIds kmm = rspKmmBody1 as InventoryResponseListActiveKsetIds;

                    Logger.Debug("number of active keyset ids: {0}", kmm.KsetIds.Count);

                    for (int i = 0; i < kmm.KsetIds.Count; i++)
                    {
                        Logger.Debug("* keyset id index {0} *", i);
                        Logger.Debug("keyset id: {0} (dec), {0:X} (hex)", kmm.KsetIds[i]);
                    }

                    if (!useActiveKeyset)
                    {
                        ksid = keysetId;
                    }
                    else if (useActiveKeyset && isKek)
                    {
                        ksid = 0xFF;
                    }
                    else if (useActiveKeyset && kmm.KsetIds.Count > 0)
                    {
                        ksid = kmm.KsetIds[0];
                    }
                    else
                    {
                        ksid = 1; // to match KVL3000+ R3.53.03 behavior
                    }
                }
                else if (rspKmmBody1 is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = rspKmmBody1 as NegativeAcknowledgment;

                    string statusDescr = OperationStatusExtensions.ToStatusString(kmm.Status);
                    string statusReason = OperationStatusExtensions.ToReasonString(kmm.Status);
                    throw new Exception(string.Format("received negative acknowledgment{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, kmm.Status, statusReason));
                }
                else
                {
                    throw new Exception("unexpected kmm");
                }

                // TODO support more than one key per erase operation

                KeyItem keyItem = new KeyItem();
                keyItem.SLN = sln;
                keyItem.KeyId = 65535; // to match KVL3000+ R3.53.03 behavior
                keyItem.Key = new byte[] { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // to match KVL3000+ R3.53.03 behavior
                keyItem.KEK = isKek;
                keyItem.Erase = true;

                ModifyKeyCommand modifyKeyCommand = new ModifyKeyCommand();

                modifyKeyCommand.KeysetId = ksid;

                modifyKeyCommand.AlgorithmId = 0x81; // to match KVL3000+ R3.53.03 behavior
                modifyKeyCommand.KeyItems.Add(keyItem);

                KmmBody rspKmmBody2 = TxRxKmm(modifyKeyCommand);

                if (rspKmmBody2 is RekeyAcknowledgment)
                {
                    RekeyAcknowledgment kmm = rspKmmBody2 as RekeyAcknowledgment;

                    Logger.Debug("number of key status: {0}", kmm.Keys.Count);

                    for (int i = 0; i < kmm.Keys.Count; i++)
                    {
                        KeyStatus status = kmm.Keys[i];

                        Logger.Debug("* key status index {0} *", i);
                        Logger.Debug("algorithm id: {0} (dec), {0:X} (hex)", status.AlgorithmId);
                        Logger.Debug("key id: {0} (dec), {0:X} (hex)", status.KeyId);
                        Logger.Debug("status: {0} (dec), {0:X} (hex)", status.Status);

                        if (status.Status != 0)
                        {
                            string statusDescr = OperationStatusExtensions.ToStatusString((OperationStatus)status.Status);
                            string statusReason = OperationStatusExtensions.ToReasonString((OperationStatus)status.Status);
                            throw new Exception(string.Format("received unexpected key status{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, status.Status, statusReason));
                        }
                    }
                }
                else if (rspKmmBody2 is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = rspKmmBody2 as NegativeAcknowledgment;

                    string statusDescr = OperationStatusExtensions.ToStatusString(kmm.Status);
                    string statusReason = OperationStatusExtensions.ToReasonString(kmm.Status);
                    throw new Exception(string.Format("received negative acknowledgment{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, kmm.Status, statusReason));
                }
                else
                {
                    throw new Exception("unexpected kmm");
                }
            }
            catch
            {
                End();

                throw;
            }

            End();
        }

        /* TIA 102.AACD-A 3.8.6 */
        public void EraseAllKeys()
        {
            Begin();

            try
            {
                ZeroizeCommand commandKmmBody = new ZeroizeCommand();

                KmmBody responseKmmBody = TxRxKmm(commandKmmBody);

                if (responseKmmBody is ZeroizeResponse)
                {
                    Logger.Debug("zerozied");
                }
                else if (responseKmmBody is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = responseKmmBody as NegativeAcknowledgment;

                    string statusDescr = OperationStatusExtensions.ToStatusString(kmm.Status);
                    string statusReason = OperationStatusExtensions.ToReasonString(kmm.Status);
                    throw new Exception(string.Format("received negative acknowledgment{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, kmm.Status, statusReason));
                }
                else
                {
                    throw new Exception("unexpected kmm");
                }
            }
            catch
            {
                End();

                throw;
            }

            End();
        }

        /* TIA 102.AACD-A 3.8.7 */
        public List<RspKeyInfo> ViewKeyInfo()
        {
            List<RspKeyInfo> result = new List<RspKeyInfo>();

            Begin();

            try
            {
                bool more = true;
                int marker = 0;

                while (more)
                {
                    InventoryCommandListActiveKeys commandKmmBody = new InventoryCommandListActiveKeys();
                    commandKmmBody.InventoryMarker = marker;
                    commandKmmBody.MaxKeysRequested = 78;

                    KmmBody responseKmmBody = TxRxKmm(commandKmmBody);

                    if (responseKmmBody is InventoryResponseListActiveKeys)
                    {
                        InventoryResponseListActiveKeys kmm = responseKmmBody as InventoryResponseListActiveKeys;

                        marker = kmm.InventoryMarker;

                        Logger.Debug("inventory marker: {0}", marker);

                        if (marker == 0)
                        {
                            more = false;
                        }

                        Logger.Debug("number of keys returned: {0}", kmm.Keys.Count);

                        for (int i = 0; i < kmm.Keys.Count; i++)
                        {
                            KeyInfo info = kmm.Keys[i];

                            Logger.Debug("* key index {0} *", i);
                            Logger.Debug("keyset id: {0} (dec), {0:X} (hex)", info.KeySetId);
                            Logger.Debug("sln: {0} (dec), {0:X} (hex)", info.SLN);
                            Logger.Debug("algorithm id: {0} (dec), {0:X} (hex)", info.AlgorithmId);
                            Logger.Debug("key id: {0} (dec), {0:X} (hex)", info.KeyId);

                            RspKeyInfo res = new RspKeyInfo();

                            res.KeysetId = info.KeySetId;
                            res.Sln = info.SLN;
                            res.AlgorithmId = info.AlgorithmId;
                            res.KeyId = info.KeyId;

                            result.Add(res);
                        }
                    }
                    else if (responseKmmBody is NegativeAcknowledgment)
                    {
                        NegativeAcknowledgment kmm = responseKmmBody as NegativeAcknowledgment;

                        string statusDescr = OperationStatusExtensions.ToStatusString(kmm.Status);
                        string statusReason = OperationStatusExtensions.ToReasonString(kmm.Status);
                        throw new Exception(string.Format("received negative acknowledgment{0}status: {1} (0x{2:X2}){0}{3}", Environment.NewLine, statusDescr, kmm.Status, statusReason));
                    }
                    else
                    {
                        throw new Exception("unexpected kmm");
                    }
                }
            }
            catch
            {
                End();

                throw;
            }

            End();

            return result;
        }

        /* TIA 102.AACD-A 3.8.8 */
        public void ViewIndividualRsi()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.9 */
        public void ViewKmfRsi()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.10 */
        public void ViewMnp()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.11 */
        public void ViewKeysetInfo()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.12 */
        public void ActivateKeyset()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.13 */
        public void LoadAuthenticationKey()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.14 */
        public void DeleteAuthenticationKey()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.15 */
        public void ViewSuidInfo()
        {
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.16 */
        public void ViewActiveSuidInfo()
        {
            throw new NotImplementedException();
        }
    }
}
