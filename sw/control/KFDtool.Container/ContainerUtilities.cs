using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Security.Cryptography;
using System.Security.Cryptography.Xml;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.XPath;

namespace KFDtool.Container
{
    public class ContainerUtilities
    {
        public static byte[] GenerateSalt(int saltLength)
        {
            byte[] salt = new byte[saltLength];

            using (RNGCryptoServiceProvider rngCsp = new RNGCryptoServiceProvider())
            {
                rngCsp.GetBytes(salt);
            }

            return salt;
        }

        public static byte[] Pbkdf2DeriveKeyFromPassword(string password, byte[] salt, int iterationCount, string hashAlgorithm, int keyLength)
        {
            Rfc2898DeriveBytes pbkdf2;

            if (hashAlgorithm == "SHA512")
            {
                pbkdf2 = new Rfc2898DeriveBytes(Encoding.UTF8.GetBytes(password), salt, iterationCount, HashAlgorithmName.SHA512);
            }
            else
            {
                throw new Exception(string.Format("invalid hash algorithm: {0}", hashAlgorithm));
            }

            return pbkdf2.GetBytes(keyLength);
        }

        public static InnerContainer CreateInnerContainer()
        {
            InnerContainer innerContainer = new InnerContainer();
            innerContainer.Version = "1.0";
            innerContainer.Keys = new ObservableCollection<KeyItem>();
            innerContainer.NextKeyNumber = 1;
            innerContainer.Groups = new ObservableCollection<GroupItem>();
            innerContainer.NextGroupNumber = 1;
            return innerContainer;
        }

        public static XmlDocument SerializeInnerContainer(InnerContainer innerContainer)
        {
            XmlDocument doc = new XmlDocument();
            XmlDeclaration xmlDeclaration = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
            doc.InsertBefore(xmlDeclaration, doc.DocumentElement);
            XPathNavigator nav = doc.CreateNavigator();
            using (XmlWriter w = nav.AppendChild())
            {
                XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
                ns.Add(string.Empty, string.Empty); // remove xsd and xsi attributes
                XmlSerializer s = new XmlSerializer(typeof(InnerContainer));
                s.Serialize(w, innerContainer, ns);
            }
            return doc;
        }

        public static (OuterContainer Container, byte[] Key) CreateOuterContainer(string password)
        {
            string outerVersion = "1.0";

            int saltLength = 32; // critical security parameter

            int iterationCount = 100000; // critical security parameter

            string hashAlgorithm = "SHA512"; // critical security parameter

            int keyLength = 32; // critical security parameter

            byte[] salt = GenerateSalt(saltLength);

            byte[] key = Pbkdf2DeriveKeyFromPassword(password, salt, iterationCount, hashAlgorithm, keyLength);

            OuterContainer outerContainer = new OuterContainer();

            outerContainer.Version = outerVersion;
            outerContainer.KeyDerivation = new KeyDerivation();
            outerContainer.KeyDerivation.DerivationAlgorithm = "PBKDF2";
            outerContainer.KeyDerivation.HashAlgorithm = hashAlgorithm;
            outerContainer.KeyDerivation.Salt = salt;
            outerContainer.KeyDerivation.IterationCount = iterationCount;
            outerContainer.KeyDerivation.KeyLength = keyLength;
            outerContainer.EncryptedDataPlaceholder = "placeholder";

            return (outerContainer, key);
        }

        public static XmlDocument SerializeOuterContainer(OuterContainer outerContainer)
        {
            XmlDocument doc = new XmlDocument();
            XmlDeclaration xmlDeclaration = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
            doc.InsertBefore(xmlDeclaration, doc.DocumentElement);
            XPathNavigator nav = doc.CreateNavigator();
            using (XmlWriter w = nav.AppendChild())
            {
                XmlSerializerNamespaces ns = new XmlSerializerNamespaces();
                ns.Add(string.Empty, string.Empty); // remove xsd and xsi attributes
                XmlSerializer s = new XmlSerializer(typeof(OuterContainer));
                s.Serialize(w, outerContainer, ns);
            }
            return doc;
        }

        public static byte[] EncryptOuterContainer(OuterContainer outerContainer, InnerContainer innerContainer, byte[] key)
        {
            XmlDocument outerContainerXml = SerializeOuterContainer(outerContainer);

            XmlDocument innerContainerXml = SerializeInnerContainer(innerContainer);

            XmlElement encryptedDataPlaceholder = outerContainerXml.GetElementsByTagName("EncryptedDataPlaceholder")[0] as XmlElement;

            XmlElement plaintextInnerContainer = innerContainerXml.GetElementsByTagName("InnerContainer")[0] as XmlElement;

            EncryptedData encryptedData = new EncryptedData();

            encryptedData.Type = EncryptedXml.XmlEncElementUrl;
            encryptedData.EncryptionMethod = new EncryptionMethod(EncryptedXml.XmlEncAES256Url);

            EncryptedXml encryptedXml = new EncryptedXml();

            using (AesCryptoServiceProvider aesCsp = new AesCryptoServiceProvider())
            {
                aesCsp.KeySize = 256; // critical security parameter
                aesCsp.Key = key; // critical security parameter
                aesCsp.Mode = CipherMode.CBC; // critical security parameter
                aesCsp.GenerateIV(); // critical security parameter

                encryptedData.CipherData.CipherValue = encryptedXml.EncryptData(plaintextInnerContainer, aesCsp, false);
            }

            EncryptedXml.ReplaceElement(encryptedDataPlaceholder, encryptedData, false);

            byte[] outerContainerBytes = Encoding.UTF8.GetBytes(outerContainerXml.OuterXml);

            byte[] fileBytes = Shared.Utility.Compress(outerContainerBytes);

            return fileBytes;
        }

        public static (OuterContainer ContainerOuter, InnerContainer ContainerInner, byte[] Key) DecryptOuterContainer(byte[] fileBytes, string password)
        {
            byte[] outerContainerBytes;

            try
            {
                outerContainerBytes = Shared.Utility.Decompress(fileBytes);
            }
            catch (Exception ex)
            {
                throw new Exception(string.Format("failed to decompress (corrupted or not a valid container file): {0}", ex.Message));
            }

            XmlDocument outerContainerXml = new XmlDocument();

            outerContainerXml.LoadXml(Encoding.UTF8.GetString(outerContainerBytes));

            string version;

            try
            {
                version = outerContainerXml.SelectSingleNode("/OuterContainer/@version").Value;
            }
            catch (Exception ex)
            {
                throw new Exception(string.Format("unable to read /OuterContainer/@version: {0}", ex.Message));
            }

            Version parsedVersion = new Version(version);

            if (parsedVersion.Major != 1)
            {
                throw new Exception(string.Format("outer container version too new (this container was written on a newer software version) - expected 1, got {0}", parsedVersion.Major));
            }

            OuterContainer outerContainer = new OuterContainer();

            string outerVersion = "1.0";

            outerContainer.Version = outerVersion;
            outerContainer.KeyDerivation = new KeyDerivation();

            byte[] key;

            string derivationAlgorithm;

            try
            {
                derivationAlgorithm = outerContainerXml.SelectSingleNode("/OuterContainer/KeyDerivation/DerivationAlgorithm").InnerText;
            }
            catch (Exception ex)
            {
                throw new Exception(string.Format("invalid KeyDerivation DerivationAlgorithm: {0}", ex.Message));
            }

            outerContainer.KeyDerivation.DerivationAlgorithm = derivationAlgorithm;

            if (derivationAlgorithm == "PBKDF2")
            {
                string hashAlgorithm;

                try
                {
                    hashAlgorithm = outerContainerXml.SelectSingleNode("/OuterContainer/KeyDerivation/HashAlgorithm").InnerText;
                }
                catch (Exception ex)
                {
                    throw new Exception(string.Format("invalid KeyDerivation HashAlgorithm: {0}", ex.Message));
                }

                outerContainer.KeyDerivation.HashAlgorithm = hashAlgorithm;

                byte[] salt;

                try
                {
                    salt = Convert.FromBase64String(outerContainerXml.SelectSingleNode("/OuterContainer/KeyDerivation/Salt").InnerText);
                }
                catch (Exception ex)
                {
                    throw new Exception(string.Format("invalid KeyDerivation Salt: {0}", ex.Message));
                }

                outerContainer.KeyDerivation.Salt = salt;

                int iterationCount;

                try
                {
                    iterationCount = Convert.ToInt32(outerContainerXml.SelectSingleNode("/OuterContainer/KeyDerivation/IterationCount").InnerText);
                }
                catch (Exception ex)
                {
                    throw new Exception(string.Format("invalid KeyDerivation IterationCount: {0}", ex.Message));
                }

                outerContainer.KeyDerivation.IterationCount = iterationCount;

                int keyLength;

                try
                {
                    keyLength = Convert.ToInt32(outerContainerXml.SelectSingleNode("/OuterContainer/KeyDerivation/KeyLength").InnerText);
                }
                catch (Exception ex)
                {
                    throw new Exception(string.Format("invalid KeyDerivation KeyLength: {0}", ex.Message));
                }

                outerContainer.KeyDerivation.KeyLength = keyLength;

                key = Pbkdf2DeriveKeyFromPassword(password, salt, iterationCount, hashAlgorithm, keyLength);
            }
            else
            {
                throw new Exception(string.Format("unsupported KeyDerivation DerivationAlgorithm: {0}", derivationAlgorithm));
            }

            outerContainer.EncryptedDataPlaceholder = "placeholder";

            XmlElement encryptedDataElement = outerContainerXml.GetElementsByTagName("EncryptedData")[0] as XmlElement;

            if (encryptedDataElement == null)
            {
                throw new Exception("unable to find EncryptedData");
            }

            EncryptedData encryptedData = new EncryptedData();

            encryptedData.LoadXml(encryptedDataElement);

            EncryptedXml encryptedXml = new EncryptedXml();

            byte[] innerContainerBytes;

            using (AesCryptoServiceProvider aesCsp = new AesCryptoServiceProvider())
            {
                aesCsp.KeySize = 256; // critical security parameter
                aesCsp.Key = key; // critical security parameter
                aesCsp.Mode = CipherMode.CBC; // critical security parameter
                aesCsp.GenerateIV(); // critical security parameter

                try
                {
                    innerContainerBytes = encryptedXml.DecryptData(encryptedData, aesCsp);
                }
                catch (Exception ex)
                {
                    throw new Exception(string.Format("unable to decrypt InnerContainer (check password): {0}", ex.Message));
                }
            }

            XmlDocument innerContainerXml = new XmlDocument();

            try
            {
                innerContainerXml.LoadXml(Encoding.UTF8.GetString(innerContainerBytes));
            }
            catch (Exception ex)
            {
                throw new Exception(string.Format("unable to parse InnerContainer (check password): {0}", ex.Message));
            }

            string innerReadVersion;

            try
            {
                innerReadVersion = innerContainerXml.SelectSingleNode("/InnerContainer/@version").Value;
            }
            catch (Exception ex)
            {
                throw new Exception(string.Format("unable to read /InnerContainer/@version: {0}", ex.Message));
            }

            Version innerReadVersionParsed = new Version(innerReadVersion);

            if (innerReadVersionParsed.Major != 1)
            {
                throw new Exception(string.Format("inner container version too new (this container was written on a newer software version) - expected 1, got {0}", innerReadVersionParsed.Major));
            }

            InnerContainer innerContainer;

            using (XmlReader xmlReader = new XmlNodeReader(innerContainerXml))
            {
                try
                {
                    innerContainer = (InnerContainer)new XmlSerializer(typeof(InnerContainer)).Deserialize(xmlReader);
                }
                catch (Exception ex)
                {
                    throw new Exception(string.Format("unable to deserialize InnerContainer: {0}", ex.Message));
                }
            }

            return (outerContainer, innerContainer, key);
        }
    }
}
