using KFDtool.Adapter.Protocol.Adapter;
using KFDtool.P25.Kmm;
using KFDtool.P25.Partition;
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
        public void Keyload(List<CmdKeyItem> keyItems)
        {
            List<List<CmdKeyItem>> keyGroups = KeyPartitioner.PartitionKeys(keyItems);

            Begin();

            try
            {
                InventoryCommandListActiveKsetIds cmdKmmBody1 = new InventoryCommandListActiveKsetIds();

                KmmBody rspKmmBody1 = TxRxKmm(cmdKmmBody1);

                int activeKeysetId = 0;

                if (rspKmmBody1 is InventoryResponseListActiveKsetIds)
                {
                    InventoryResponseListActiveKsetIds kmm = rspKmmBody1 as InventoryResponseListActiveKsetIds;

                    Logger.Debug("number of active keyset ids: {0}", kmm.KsetIds.Count);

                    for (int i = 0; i < kmm.KsetIds.Count; i++)
                    {
                        Logger.Debug("* keyset id index {0} *", i);
                        Logger.Debug("keyset id: {0} (dec), {0:X} (hex)", kmm.KsetIds[i]);
                    }

                    // TODO support more than one crypto group
                    if (kmm.KsetIds.Count > 0)
                    {
                        activeKeysetId = kmm.KsetIds[0];
                    }
                    else
                    {
                        activeKeysetId = 1; // to match KVL3000+ R3.53.03 behavior
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

                foreach (List<CmdKeyItem> keyGroup in keyGroups)
                {
                    ModifyKeyCommand modifyKeyCommand = new ModifyKeyCommand();

                    // TODO support more than one crypto group
                    if (keyGroup[0].UseActiveKeyset && !keyGroup[0].IsKek)
                    {
                        modifyKeyCommand.KeysetId = activeKeysetId;
                    }
                    else if (keyGroup[0].UseActiveKeyset && keyGroup[0].IsKek)
                    {
                        modifyKeyCommand.KeysetId = 0xFF; // to match KVL3000+ R3.53.03 behavior
                    }
                    else
                    {
                        modifyKeyCommand.KeysetId = keyGroup[0].KeysetId;
                    }

                    modifyKeyCommand.AlgorithmId = keyGroup[0].AlgorithmId;

                    foreach (CmdKeyItem key in keyGroup)
                    {
                        Logger.Debug(key.ToString());

                        KeyItem keyItem = new KeyItem();
                        keyItem.SLN = key.Sln;
                        keyItem.KeyId = key.KeyId;
                        keyItem.Key = key.Key.ToArray();
                        keyItem.KEK = key.IsKek;
                        keyItem.Erase = false;

                        modifyKeyCommand.KeyItems.Add(keyItem);
                    }

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
            //cg
            // This is implemented with ChangeRSI()
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.3 */
        public void LoadKmfRsi()
        {
            //cg
            // This command is actually LoadConfig
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.4 */
        public void LoadMnp()
        {
            //cg
            // This process actually takes two commands:
            // List KMF RSI
            // Load Config, with RSI and new MNP
            throw new NotImplementedException();
        }

        /* TIA 102.AACD-A 3.8.5 */
        public void EraseKeys(List<CmdKeyItem> keyItems)
        {
            List<List<CmdKeyItem>> keyGroups = KeyPartitioner.PartitionKeys(keyItems);

            Begin();

            try
            {
                InventoryCommandListActiveKsetIds cmdKmmBody1 = new InventoryCommandListActiveKsetIds();

                KmmBody rspKmmBody1 = TxRxKmm(cmdKmmBody1);

                int activeKeysetId = 0;

                if (rspKmmBody1 is InventoryResponseListActiveKsetIds)
                {
                    InventoryResponseListActiveKsetIds kmm = rspKmmBody1 as InventoryResponseListActiveKsetIds;

                    Logger.Debug("number of active keyset ids: {0}", kmm.KsetIds.Count);

                    for (int i = 0; i < kmm.KsetIds.Count; i++)
                    {
                        Logger.Debug("* keyset id index {0} *", i);
                        Logger.Debug("keyset id: {0} (dec), {0:X} (hex)", kmm.KsetIds[i]);
                    }

                    // TODO support more than one crypto group
                    if (kmm.KsetIds.Count > 0)
                    {
                        activeKeysetId = kmm.KsetIds[0];
                    }
                    else
                    {
                        activeKeysetId = 1; // to match KVL3000+ R3.53.03 behavior
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

                foreach (List<CmdKeyItem> keyGroup in keyGroups)
                {
                    ModifyKeyCommand modifyKeyCommand = new ModifyKeyCommand();

                    // TODO support more than one crypto group
                    if (keyGroup[0].UseActiveKeyset && !keyGroup[0].IsKek)
                    {
                        modifyKeyCommand.KeysetId = activeKeysetId;
                    }
                    else if (keyGroup[0].UseActiveKeyset && keyGroup[0].IsKek)
                    {
                        modifyKeyCommand.KeysetId = 0xFF; // to match KVL3000+ R3.53.03 behavior
                    }
                    else
                    {
                        modifyKeyCommand.KeysetId = keyGroup[0].KeysetId;
                    }

                    modifyKeyCommand.AlgorithmId = 0x81; // to match KVL3000+ R3.53.03 behavior

                    foreach (CmdKeyItem key in keyGroup)
                    {
                        Logger.Debug(key.ToString());

                        KeyItem keyItem = new KeyItem();
                        keyItem.SLN = key.Sln;
                        keyItem.KeyId = 65535; // to match KVL3000+ R3.53.03 behavior
                        keyItem.Key = new byte[] { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // to match KVL3000+ R3.53.03 behavior
                        keyItem.KEK = key.IsKek;
                        keyItem.Erase = true;

                        modifyKeyCommand.KeyItems.Add(keyItem);
                    }

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

        /* TIA 102.AACD-A 3.7.2.15 */
        public int ViewKmfRsi()
        {
            //cg
            int result = new int();

            Begin();

            try
            {
                InventoryCommandListKmfRsi commandKmmBody = new InventoryCommandListKmfRsi();

                KmmBody responseKmmBody = TxRxKmm(commandKmmBody);

                if (responseKmmBody is InventoryResponseListKmfRsi)
                {
                    Logger.Debug("MNP response");

                    InventoryResponseListKmfRsi kmm = responseKmmBody as InventoryResponseListKmfRsi;

                    result = kmm.KmfRsi;
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

            return result;
        }

        /* TIA 102.AACD-A 3.7.2.13 */
        public int ViewMnp()
        {
            //cg
            int result = new int();

            Begin();

            try
            {
                InventoryCommandListMnp commandKmmBody = new InventoryCommandListMnp();

                KmmBody responseKmmBody = TxRxKmm(commandKmmBody);

                if (responseKmmBody is InventoryResponseListMnp)
                {
                    Logger.Debug("MNP response");

                    InventoryResponseListMnp kmm = responseKmmBody as InventoryResponseListMnp;

                    result = kmm.MessageNumberPeriod;
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

            return result;
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

        /* TIA 102.AACD-A 3.7.2.22 */
        public RspRsiInfo LoadConfig(int kmfRsi, int mnp)
        {
            //cg
            RspRsiInfo result = new RspRsiInfo();

            Begin();

            try
            {
                LoadConfigCommand cmdKmmBody = new LoadConfigCommand();
                cmdKmmBody.KmfRsi = kmfRsi;
                cmdKmmBody.MessageNumberPeriod = mnp;
                KmmBody rspKmmBody = TxRxKmm(cmdKmmBody);
                if (rspKmmBody is LoadConfigResponse)
                {
                    LoadConfigResponse kmm = rspKmmBody as LoadConfigResponse;
                    result.RSI = kmm.RSI;
                    result.MN = kmm.MN;
                    result.Status = kmm.Status;
                    //Console.WriteLine("response status: {0}", kmm.Status);
                }
                else if (rspKmmBody is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = rspKmmBody as NegativeAcknowledgment;

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

            return result;
        }

        /* TIA 102.AACD-A 3.7.2.1 */
        public RspRsiInfo ChangeRsi(int rsiOld, int rsiNew, int mnp)
        {
            //cg
            RspRsiInfo result = new RspRsiInfo();

            Begin();

            try
            {
                ChangeRsiCommand cmdKmmBody = new ChangeRsiCommand();
                cmdKmmBody.RsiOld = rsiOld;
                cmdKmmBody.RsiNew = rsiNew;
                cmdKmmBody.MessageNumber = mnp;
                KmmBody rspKmmBody = TxRxKmm(cmdKmmBody);
                if (rspKmmBody is ChangeRsiResponse)
                {
                    ChangeRsiResponse kmm = rspKmmBody as ChangeRsiResponse;
                    //Console.WriteLine("response status: {0}", kmm.Status);
                    result.RSI = rsiNew;
                    result.MN = mnp;
                    result.Status = kmm.Status;
                }
                else if (rspKmmBody is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = rspKmmBody as NegativeAcknowledgment;

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

            return result;
        }

        /* TIA 102.AACD-A 3.7.2.7 */
        public List<RspRsiInfo> ViewRsiItems()
        {
            //cg
            List<RspRsiInfo> result = new List<RspRsiInfo>();

            Begin();

            try
            {
                bool more = true;
                int marker = 0;

                while (more)
                {
                    InventoryCommandListRsiItems commandKmmBody = new InventoryCommandListRsiItems();
                    //commandKmmBody.InventoryMarker = marker;
                    //commandKmmBody.MaxKeysRequested = 78;

                    KmmBody responseKmmBody = TxRxKmm(commandKmmBody);

                    if (responseKmmBody is InventoryResponseListRsiItems)
                    {
                        InventoryResponseListRsiItems kmm = responseKmmBody as InventoryResponseListRsiItems;

                        //marker = kmm.InventoryMarker;

                        Logger.Debug("inventory marker: {0}", marker);

                        if (marker == 0)
                        {
                            more = false;
                        }

                        Logger.Debug("number of RSIs returned: {0}", kmm.RsiItems.Count);

                        for (int i = 0; i < kmm.RsiItems.Count; i++)
                        {
                            RsiItem item = kmm.RsiItems[i];

                            Logger.Debug("* rsi index {0} *", i);
                            Logger.Debug("rsi id: {0} (dec), {0:X} (hex)", item.RSI);
                            Logger.Debug("mn: {0} (dec), {0:X} (hex)", item.MessageNumber);

                            RspRsiInfo res = new RspRsiInfo();

                            res.RSI = (int)item.RSI;
                            res.MN = item.MessageNumber;

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

        /* TIA 102.AACD-A 3.7.2.9 */
        public List<RspKeysetInfo> ViewKeysetTaggingInfo()
        {
            //cg
            List<RspKeysetInfo> result = new List<RspKeysetInfo>();

            Begin();

            try
            {
                InventoryCommandListKeysetTaggingInfo commandKmmBody = new InventoryCommandListKeysetTaggingInfo();

                KmmBody responseKmmBody = TxRxKmm(commandKmmBody);

                if (responseKmmBody is InventoryResponseListKeysetTaggingInfo)
                {
                    Logger.Debug("KeysetTaggingInfo response");

                    InventoryResponseListKeysetTaggingInfo kmm = responseKmmBody as InventoryResponseListKeysetTaggingInfo;

                    for (int i = 0; i < kmm.KeysetItems.Count; i++)
                    {
                        KeysetItem item = kmm.KeysetItems[i];

                        RspKeysetInfo res = new RspKeysetInfo();

                        res.KeysetId = item.KeysetId;
                        res.KeysetName = item.KeysetName;
                        res.KeysetType = item.KeysetType;
                        res.ActivationDateTime = item.ActivationDateTime;
                        res.ReservedField = item.ReservedField;

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
            catch
            {
                End();

                throw;
            }

            End();

            return result;
        }

        /* TIA 102.AACD-A 3.7.2.3 */
        public RspChangeoverInfo ActivateKeyset(int keysetSuperseded, int keysetActivated)
        {
            //cg
            RspChangeoverInfo result = new RspChangeoverInfo();

            Begin();

            try
            {
                ChangeoverCommand cmdKmmBody = new ChangeoverCommand();
                cmdKmmBody.KeysetIdSuperseded = keysetSuperseded;
                cmdKmmBody.KeysetIdActivated = keysetActivated;
                KmmBody rspKmmBody = TxRxKmm(cmdKmmBody);
                if (rspKmmBody is ChangeoverResponse)
                {
                    ChangeoverResponse kmm = rspKmmBody as ChangeoverResponse;
                    /*
                    for (int i = 0; i < kmm.KeysetItems.Count; i++)
                    {
                        KeysetItem item = kmm.KeysetItems[i];

                        RspKeysetInfo res = new RspKeysetInfo();

                        res.KeysetId = item.KeysetId;
                        res.KeysetName = item.KeysetName;
                        res.KeysetType = item.KeysetType;
                        res.ActivationDateTime = item.ActivationDateTime;
                        res.ReservedField = item.ReservedField;

                        result.Add(res);
                    }
                    */

                    result.KeysetIdSuperseded = kmm.KeysetIdSuperseded;
                    result.KeysetIdActivated = kmm.KeysetIdActivated;
                    //Console.WriteLine("response status: {0}", kmm.Status);
                }
                else if (rspKmmBody is NegativeAcknowledgment)
                {
                    NegativeAcknowledgment kmm = rspKmmBody as NegativeAcknowledgment;

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

            return result;
        }



    }
}
