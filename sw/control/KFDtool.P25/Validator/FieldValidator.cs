using KFDtool.P25.Constant;
using KFDtool.Shared;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KFDtool.P25.Validator
{
    public class FieldValidator
    {
        public static bool IsValidKeysetId(int keysetId)
        {
            /* TIA 102.AACA-A 10.3.11 */
            if (keysetId < 0x01 || keysetId > 0xFF)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        public static bool IsValidSln(int sln)
        {
            /* TIA 102.AACA-A 10.3.25 */
            if (sln < 1 || sln > 65535)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        public static bool IsValidKeyId(int keyId)
        {
            /* TIA 102.AACA-A 10.3.10 */
            if (keyId < 0x0000 || keyId > 0xFFFF)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        public static bool IsValidAlgorithmId(int algId)
        {
            /* TIA-102.BAAC-D 2.8 */
            if (algId < 0x00 || algId > 0xFF)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        public static bool IsValidSingleDesKeyParity(List<byte> key)
        {
            if (key.Count != 8)
            {
                throw new ArgumentOutOfRangeException();
            }

            bool result = true;

            foreach (byte b in key)
            {
                bool set = Convert.ToBoolean(b & 0x01); // least significant bit is the parity bit

                // from .NET 4.8 refsrc system\security\cryptography\utils.cs FixupKeyParity()
                byte c = (byte)(b & 0xfe);
                byte tmp1 = (byte)((c & 0xF) ^ (c >> 4));
                byte tmp2 = (byte)((tmp1 & 0x3) ^ (tmp1 >> 2));
                byte sumBitsMod2 = (byte)((tmp2 & 0x1) ^ (tmp2 >> 1));

                bool calc = false;

                if (sumBitsMod2 == 0)
                {
                    calc = true;
                }

                if (set != calc) // parity bit is incorrect
                {
                    return false;
                }
            }

            return result;
        }

        public static Tuple<ValidateResult,string> KeyloadValidate(int keysetId, int sln, int keyId, int algId, List<byte> key)
        {
            if (!IsValidKeysetId(keysetId))
            {
                return Tuple.Create(ValidateResult.Error, "Keyset ID invalid - valid range 1 to 255 (dec), 0x01 to 0xFF (hex)");
            }

            if (!IsValidSln(sln))
            {
                return Tuple.Create(ValidateResult.Error, "SLN invalid - valid range 1 to 65553 (dec), 0x0001 to 0x1999 (hex)");
            }

            if (!IsValidKeyId(keyId))
            {
                return Tuple.Create(ValidateResult.Error, "Key ID invalid - valid range 0 to 65535 (dec), 0x0000 to 0xFFFF (hex)");
            }

            if (!IsValidAlgorithmId(algId))
            {
                return Tuple.Create(ValidateResult.Error, "Algorithm ID invalid - valid range 0 to 255 (dec), 0x00 to 0xFF (hex)");
            }

            if (algId == (byte)AlgorithmId.CLEAR)
            {
                return Tuple.Create(ValidateResult.Error, "Algorithm ID 0x80 is reserved for clear operation");
            }
            else if (algId == (byte)AlgorithmId.ACCORDION ||
                     algId == (byte)AlgorithmId.BATON_ODD ||
                     algId == (byte)AlgorithmId.FIREFLY ||
                     algId == (byte)AlgorithmId.MAYFLY ||
                     algId == (byte)AlgorithmId.SAVILLE ||
                     algId == (byte)AlgorithmId.PADSTONE ||
                     algId == (byte)AlgorithmId.BATON_EVEN)
            {
                return Tuple.Create(ValidateResult.Warning, string.Format("Algorithm ID 0x{0:X2} is a Type 1 algorithm - no key validation has been performed", algId));
            }
            else if (algId == (byte)AlgorithmId.DESOFB || algId == (byte)AlgorithmId.DESXL)
            {
                if (key.Count != 8)
                {
                    return Tuple.Create(ValidateResult.Error, string.Format("Key length invalid - expected 8 bytes, got {0} bytes", key.Count));
                }

                if (!IsValidSingleDesKeyParity(key))
                {
                    return Tuple.Create(ValidateResult.Error, "Key parity invalid");
                }

                // des weak keys per NIST SP 800-67 Rev 2 3.3.2

                List<List<byte>> weakKeys = new List<List<byte>>
                {
                    // des weak keys (4)
                    new List<byte> { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                    new List<byte> { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE },
                    new List<byte> { 0xE0, 0xE0, 0xE0, 0xE0, 0xF1, 0xF1, 0xF1, 0xF1 },
                    new List<byte> { 0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x0E },

                    // des semi weak keys (12)
                    new List<byte> { 0x01, 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E },
                    new List<byte> { 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E, 0x01 },
                    new List<byte> { 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1 },
                    new List<byte> { 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1, 0x01 },
                    new List<byte> { 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE },
                    new List<byte> { 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01 },
                    new List<byte> { 0x1F, 0xE0, 0x1F, 0xE0, 0x0E, 0xF1, 0x0E, 0xF1 },
                    new List<byte> { 0xE0, 0x1F, 0xE0, 0x1F, 0xF1, 0x0E, 0xF1, 0x0E },
                    new List<byte> { 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E, 0xFE },
                    new List<byte> { 0xFE, 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E },
                    new List<byte> { 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1, 0xFE },
                    new List<byte> { 0xFE, 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1 },

                    // des possibly weak keys (48)
                    new List<byte> { 0x01, 0x01, 0x1F, 0x1F, 0x01, 0x01, 0x0E, 0x0E },
                    new List<byte> { 0x01, 0x01, 0xE0, 0xE0, 0x01, 0x01, 0xF1, 0xF1 },
                    new List<byte> { 0x01, 0x01, 0xFE, 0xFE, 0x01, 0x01, 0xFE, 0xFE },
                    new List<byte> { 0x01, 0x1F, 0x1F, 0x01, 0x01, 0x0E, 0x0E, 0x01 },
                    new List<byte> { 0x01, 0x1F, 0xE0, 0xFE, 0x01, 0x0E, 0xF1, 0xFE },
                    new List<byte> { 0x01, 0x1F, 0xFE, 0xE0, 0x01, 0x0E, 0xFE, 0xF1 },
                    new List<byte> { 0x01, 0xE0, 0x1F, 0xFE, 0x01, 0xF1, 0x0E, 0xFE },
                    new List<byte> { 0xFE, 0x01, 0xE0, 0x1F, 0xFE, 0x01, 0xF1, 0x0E },
                    new List<byte> { 0x01, 0xE0, 0xE0, 0x01, 0x01, 0xF1, 0xF1, 0x01 },
                    new List<byte> { 0x01, 0xE0, 0xFE, 0x1F, 0x01, 0xF1, 0xFE, 0x0E },
                    new List<byte> { 0x01, 0xFE, 0x1F, 0xE0, 0x01, 0xFE, 0x0E, 0xF1 },
                    new List<byte> { 0x01, 0xFE, 0xE0, 0x1F, 0x01, 0xFE, 0xF1, 0x0E },
                    new List<byte> { 0x01, 0xFE, 0xFE, 0x01, 0x01, 0xFE, 0xFE, 0x01 },
                    new List<byte> { 0x1F, 0x01, 0x01, 0x1F, 0x0E, 0x01, 0x01, 0x0E },
                    new List<byte> { 0x1F, 0x01, 0xE0, 0xFE, 0x0E, 0x01, 0xF1, 0xFE },
                    new List<byte> { 0x1F, 0x01, 0xFE, 0xE0, 0x0E, 0x01, 0xFE, 0xF1 },

                    new List<byte> { 0x1F, 0x1F, 0x01, 0x01, 0x0E, 0x0E, 0x01, 0x01 },
                    new List<byte> { 0x1F, 0x1F, 0xE0, 0xE0, 0x0E, 0x0E, 0xF1, 0xF1 },
                    new List<byte> { 0x1F, 0x1F, 0xFE, 0xFE, 0x0E, 0x0E, 0xFE, 0xFE },
                    new List<byte> { 0x1F, 0xE0, 0x01, 0xFE, 0x0E, 0xF1, 0x01, 0xFE },
                    new List<byte> { 0x1F, 0xE0, 0xE0, 0x1F, 0x0E, 0xF1, 0xF1, 0x0E },
                    new List<byte> { 0x1F, 0xE0, 0xFE, 0x01, 0x0E, 0xF1, 0xFE, 0x01 },
                    new List<byte> { 0x1F, 0xFE, 0x01, 0xE0, 0x0E, 0xFE, 0x01, 0xF1 },
                    new List<byte> { 0x1F, 0xFE, 0xE0, 0x01, 0x0E, 0xFE, 0xF1, 0x01 },
                    new List<byte> { 0x1F, 0xFE, 0xFE, 0x1F, 0x0E, 0xFE, 0xFE, 0x0E },
                    new List<byte> { 0x1F, 0xFE, 0xFE, 0x1F, 0x0E, 0xFE, 0xFE, 0x0E },
                    new List<byte> { 0xE0, 0x01, 0x1F, 0xFE, 0xF1, 0x01, 0x0E, 0xFE },
                    new List<byte> { 0xE0, 0x01, 0xFE, 0x1F, 0xF1, 0x01, 0xFE, 0x0E },
                    new List<byte> { 0xE0, 0x1F, 0x01, 0xFE, 0xF1, 0x0E, 0x01, 0xFE },
                    new List<byte> { 0xE0, 0x1F, 0x1F, 0xE0, 0xF1, 0x0E, 0x0E, 0xF1 },
                    new List<byte> { 0xE0, 0x1F, 0xFE, 0x01, 0xF1, 0x0E, 0xFE, 0x01 },
                    new List<byte> { 0xE0, 0xE0, 0x01, 0x01, 0xF1, 0xF1, 0x01, 0x01 },

                    new List<byte> { 0xE0, 0xE0, 0x1F, 0x1F, 0xF1, 0xF1, 0x0E, 0x0E },
                    new List<byte> { 0xE0, 0xE0, 0xFE, 0xFE, 0xF1, 0xF1, 0xFE, 0xFE },
                    new List<byte> { 0xE0, 0xFE, 0x01, 0x1F, 0xF1, 0xFE, 0x01, 0x0E },
                    new List<byte> { 0xE0, 0xFE, 0x1F, 0x01, 0xF1, 0xFE, 0x0E, 0x01 },
                    new List<byte> { 0xE0, 0xFE, 0xFE, 0xE0, 0xF1, 0xFE, 0xFE, 0xF1 },
                    new List<byte> { 0xFE, 0x01, 0x01, 0xFE, 0xFE, 0x01, 0x01, 0xFE },
                    new List<byte> { 0xFE, 0x01, 0x1F, 0xE0, 0xFE, 0x01, 0x0E, 0xF1 },
                    new List<byte> { 0xFE, 0x1F, 0x01, 0xE0, 0xFE, 0x0E, 0x01, 0xF1 },
                    new List<byte> { 0xFE, 0x1F, 0xE0, 0x01, 0xFE, 0x0E, 0xF1, 0x01 },
                    new List<byte> { 0xFE, 0x1F, 0x1F, 0xFE, 0xFE, 0x0E, 0x0E, 0xFE },
                    new List<byte> { 0xFE, 0xE0, 0x01, 0x1F, 0xFE, 0xF1, 0x01, 0x0E },
                    new List<byte> { 0xFE, 0xE0, 0x1F, 0x01, 0xFE, 0xF1, 0x0E, 0x01 },
                    new List<byte> { 0xFE, 0xE0, 0xE0, 0xFE, 0xFE, 0xF1, 0xF1, 0xFE },
                    new List<byte> { 0xFE, 0xFE, 0x01, 0x01, 0xFE, 0xFE, 0x01, 0x01 },
                    new List<byte> { 0xFE, 0xFE, 0x1F, 0x1F, 0xFE, 0xFE, 0x0E, 0x0E },
                    new List<byte> { 0xFE, 0xFE, 0xE0, 0xE0, 0xFE, 0xFE, 0xF1, 0xF1 }
                };

                foreach (List<byte> weak in weakKeys)
                {
                    if (weak.SequenceEqual(key))
                    {
                        return Tuple.Create(ValidateResult.Warning, "This key is cryptographically weak");
                    }
                }

                List<List<byte>> guessableKeys = new List<List<byte>>
                {
                    new List<byte> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
                    new List<byte> { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 },
                    new List<byte> { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF }
                };

                foreach (List<byte> weak in guessableKeys)
                {
                    if (weak.SequenceEqual(key))
                    {
                        return Tuple.Create(ValidateResult.Warning, "This key is easily guessable");
                    }
                }
            }
            else if (algId == (byte)AlgorithmId.AES256)
            {
                if (key.Count != 32)
                {
                    return Tuple.Create(ValidateResult.Error, string.Format("Key length invalid - expected 32 bytes, got {0} bytes", key.Count));
                }

                List<List<byte>> guessableKeys = new List<List<byte>>
                {
                    new List<byte>
                    {
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    },
                    new List<byte>
                    {
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
                    },
                    new List<byte>
                    {
                        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
                    }
                };

                foreach (List<byte> weak in guessableKeys)
                {
                    if (weak.SequenceEqual(key))
                    {
                        return Tuple.Create(ValidateResult.Warning, "This key is easily guessable");
                    }
                }
            }
            else if (algId == (byte)AlgorithmId.ADP)
            {
                if (key.Count != 5)
                {
                    return Tuple.Create(ValidateResult.Error, string.Format("Key length invalid - expected 5 bytes, got {0} bytes", key.Count));
                }

                List<List<byte>> guessableKeys = new List<List<byte>>
                {
                    new List<byte>
                    {
                        0x00, 0x00, 0x00, 0x00, 0x00
                    },
                    new List<byte>
                    {
                        0x00, 0x01, 0x02, 0x03, 0x04
                    },
                    new List<byte>
                    {
                        0x01, 0x23, 0x45, 0x67, 0x89
                    }
                };

                foreach (List<byte> weak in guessableKeys)
                {
                    if (weak.SequenceEqual(key))
                    {
                        return Tuple.Create(ValidateResult.Warning, "This key is easily guessable");
                    }
                }
            }
            else // all other algorithm IDs
            {
                return Tuple.Create(ValidateResult.Warning, string.Format("Algorithm ID 0x{0:X2} is unassigned - no key validation has been performed", algId));
            }

            return Tuple.Create(ValidateResult.Success, string.Empty);
        }
    }
}
