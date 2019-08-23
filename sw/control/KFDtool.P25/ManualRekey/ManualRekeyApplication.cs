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
        public void Keyload(bool useActiveKeyset, int keysetId, int sln, int keyId, int algId, List<byte> key)
        {
            Begin();

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
                else if (useActiveKeyset && kmm.KsetIds.Count > 0)
                {
                    ksid = kmm.KsetIds[0];
                }
                else
                {
                    throw new Exception("no active keyset");
                }
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
                        throw new Exception("unexpected status");
                    }
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
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
        public void EraseKeys(bool useActiveKeyset, int keysetId, int sln)
        {
            Begin();

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
                else if (useActiveKeyset && kmm.KsetIds.Count > 0)
                {
                    ksid = kmm.KsetIds[0];
                }
                else
                {
                    throw new Exception("no active keyset");
                }
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
                        throw new Exception("unexpected status");
                    }
                }
            }
            else
            {
                throw new Exception("unexpected kmm");
            }

            End();
        }

        /* TIA 102.AACD-A 3.8.6 */
        public void EraseAllKeys()
        {
            Begin();

            ZeroizeCommand commandKmmBody = new ZeroizeCommand();

            KmmBody responseKmmBody = TxRxKmm(commandKmmBody);

            if (responseKmmBody is ZeroizeResponse)
            {
                Logger.Debug("zerozied");
            }
            else
            {
                throw new Exception("unexpected kmm");
            }

            End();
        }

        /* TIA 102.AACD-A 3.8.7 */
        public List<RspKeyInfo> ViewKeyInfo()
        {
            List<RspKeyInfo> result = new List<RspKeyInfo>();

            Begin();

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
                else
                {
                    throw new Exception("unexpected kmm");
                }
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
