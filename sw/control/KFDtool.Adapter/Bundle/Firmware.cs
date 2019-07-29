using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Security.Cryptography;
using System.Security.Cryptography.Xml;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;

namespace KFDtool.Adapter.Bundle
{
    public class Firmware
    {
        private const string PACKAGE_VERSION = "1.0";
        private const string BODY_VERSION = "1.0";
        private const string SIGNATURE_VERSION = "1.0";
        private const string GENERATE_VERSION = "1.0";
        private const string CANONICALIZATION_METHOD_URL = "http://www.w3.org/TR/2001/REC-xml-c14n-20010315";
        private const string DIGEST_METHOD_URL = "http://www.w3.org/2001/04/xmlenc#sha512";

        private static byte[] Compress(byte[] data)
        {
            byte[] buffer = data;
            var memoryStream = new MemoryStream();
            using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Compress, true))
            {
                gZipStream.Write(buffer, 0, buffer.Length);
            }

            memoryStream.Position = 0;

            var compressedData = new byte[memoryStream.Length];
            memoryStream.Read(compressedData, 0, compressedData.Length);

            var gZipBuffer = new byte[compressedData.Length + 4];
            Buffer.BlockCopy(compressedData, 0, gZipBuffer, 4, compressedData.Length);
            Buffer.BlockCopy(BitConverter.GetBytes(buffer.Length), 0, gZipBuffer, 0, 4);
            return gZipBuffer;
        }

        private static byte[] Decompress(byte[] data)
        {
            byte[] gZipBuffer = data;
            using (var memoryStream = new MemoryStream())
            {
                int dataLength = BitConverter.ToInt32(gZipBuffer, 0);
                memoryStream.Write(gZipBuffer, 4, gZipBuffer.Length - 4);

                var buffer = new byte[dataLength];

                memoryStream.Position = 0;
                using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Decompress))
                {
                    gZipStream.Read(buffer, 0, buffer.Length);
                }

                return buffer;
            }
        }

        private static string GetDigest(XmlDocument doc)
        {
            XmlDsigC14NTransform t = new XmlDsigC14NTransform();
            t.LoadInput(doc);
            Stream s = (Stream)t.GetOutput(typeof(Stream));

            byte[] hash;

            using (SHA512 sha = new SHA512Managed())
            {
                hash = sha.ComputeHash(s);
            }

            return Convert.ToBase64String(hash);
        }

        private static XmlDocument SerializeBody(List<Update> fw)
        {
            XmlDocument bodyDoc = new XmlDocument();

            XmlElement body = bodyDoc.CreateElement(string.Empty, "Body", string.Empty);
            body.SetAttribute("version", BODY_VERSION);
            bodyDoc.AppendChild(body);

            foreach (Update fwi in fw)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(Update));
                string temp;
                using (StringWriter writer = new StringWriter())
                {
                    serializer.Serialize(writer, fwi);
                    temp = writer.ToString();
                }

                XmlDocument updDoc = new XmlDocument();
                updDoc.LoadXml(temp);
                XmlNode fwu = bodyDoc.ImportNode(updDoc.SelectSingleNode("/Update"), true);
                body.AppendChild(fwu);
            }

            return bodyDoc;
        }

        private static List<Update> DeserializeBody(XmlDocument doc)
        {
            // check body version
            string bv = doc.SelectSingleNode("/Body/@version").Value;

            if (new Version(bv).Major != new Version(BODY_VERSION).Major)
            {
                throw new Exception(string.Format("unexpected body major version - expected: {0}, got: {1}", BODY_VERSION, bv));
            }

            List<Update> updates = new List<Update>();

            XmlSerializer serializer = new XmlSerializer(typeof(Update));

            XmlNodeList nodeList = doc.SelectNodes("/Body/Update");

            foreach (XmlNode no in nodeList)
            {
                Update upd;

                using (TextReader reader = new StringReader(no.OuterXml))
                {
                    upd = (Update)serializer.Deserialize(reader);
                }

                updates.Add(upd);
            }

            return updates;
        }

        private static List<Update> CreateUpdateList(string templatePath)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(File.ReadAllText(templatePath));

            // check generate version
            string gv = doc.SelectSingleNode("/Generate/@version").Value;

            if (new Version(gv).Major != new Version(GENERATE_VERSION).Major)
            {
                throw new Exception(string.Format("unexpected generate major version - expected: {0}, got: {1}", GENERATE_VERSION, gv));
            }

            XmlNodeList updateNodes = doc.SelectNodes("/Generate/Update");

            List<Update> updateList = new List<Update>();

            foreach (XmlNode updateNode in updateNodes)
            {
                Update update = new Update();

                update.AppData = File.ReadAllBytes(updateNode.SelectSingleNode("AppDataPath").InnerText);
                update.AppVersion = updateNode.SelectSingleNode("AppVersion").InnerText;
                update.Audience = updateNode.SelectSingleNode("Audience").InnerText;
                update.RamBslData = File.ReadAllBytes(updateNode.SelectSingleNode("RamBslDataPath").InnerText);
                update.RamBslVersion = updateNode.SelectSingleNode("RamBslVersion").InnerText;

                XmlNodeList modelNodes = updateNode.SelectSingleNode("ProductModel").ChildNodes;

                List<Model> modelList = new List<Model>();

                foreach (XmlNode modelNode in modelNodes)
                {
                    Model model = new Model();

                    model.Name = modelNode.SelectSingleNode("Name").InnerText;

                    XmlNodeList revisionNodes = modelNode.SelectSingleNode("Revision").ChildNodes;

                    List<string> revisionList = new List<string>();

                    foreach (XmlNode revisionNode in revisionNodes)
                    {
                        revisionList.Add(revisionNode.InnerText);
                    }

                    model.Revision = revisionList;

                    modelList.Add(model);
                }

                update.ProductModel = modelList;

                updateList.Add(update);
            }

            return updateList;
        }

        private static void CreateUpdatePackage(List<Update> fw, string outPath)
        {
            XmlDocument doc = new XmlDocument();

            XmlDeclaration xmlDeclaration = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
            doc.InsertBefore(xmlDeclaration, doc.DocumentElement);

            XmlElement fwupdate = doc.CreateElement(string.Empty, "FirmwarePackage", string.Empty);
            fwupdate.SetAttribute("version", PACKAGE_VERSION);
            doc.AppendChild(fwupdate);

            XmlDocument ext = SerializeBody(fw);
            XmlNode body = doc.ImportNode(ext.SelectSingleNode("/Body"), true);
            fwupdate.AppendChild(body);

            XmlDocument strippedDoc = new XmlDocument();
            strippedDoc.LoadXml(body.OuterXml);

            XmlElement signature = doc.CreateElement(string.Empty, "Signature", string.Empty);
            signature.SetAttribute("version", SIGNATURE_VERSION);
            fwupdate.AppendChild(signature);

            XmlElement canonicalizationmethod = doc.CreateElement(string.Empty, "CanonicalizationMethod", string.Empty);
            canonicalizationmethod.SetAttribute("Algorithm", CANONICALIZATION_METHOD_URL);
            signature.AppendChild(canonicalizationmethod);

            XmlElement digestmethod = doc.CreateElement(string.Empty, "DigestMethod", string.Empty);
            digestmethod.SetAttribute("Algorithm", DIGEST_METHOD_URL);
            signature.AppendChild(digestmethod);

            XmlElement digestvalue = doc.CreateElement(string.Empty, "DigestValue", string.Empty);
            digestvalue.AppendChild(doc.CreateTextNode(GetDigest(strippedDoc)));
            signature.AppendChild(digestvalue);

            byte[] pkgData = Encoding.UTF8.GetBytes(doc.OuterXml);

            // write uncompressed firmware package file
            File.WriteAllBytes(outPath + ".ufp", pkgData);

            byte[] outData = Compress(pkgData);

            // write compressed firmware package file
            File.WriteAllBytes(outPath + ".cfp", outData);
        }

        public static List<Update> OpenCompressedUpdatePackage(string inPath)
        {
            byte[] inData = File.ReadAllBytes(inPath);

            byte[] pkgData = Decompress(inData);

            return OpenUpdatePackage(pkgData);
        }

        public static List<Update> OpenUncompressedUpdatePackage(string inPath)
        {
            byte[] pkgData = File.ReadAllBytes(inPath);

            return OpenUpdatePackage(pkgData);
        }

        private static List<Update> OpenUpdatePackage(byte[] pkgData)
        {
            string pkg = Encoding.UTF8.GetString(pkgData);

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(pkg);

            // check package version
            string pv = doc.SelectSingleNode("/FirmwarePackage/@version").Value;

            if (new Version(pv).Major != new Version(PACKAGE_VERSION).Major)
            {
                throw new Exception(string.Format("unexpected package major version - expected: {0}, got: {1}", PACKAGE_VERSION, pv));
            }

            string sv = doc.SelectSingleNode("/FirmwarePackage/Signature/@version").Value;

            // check signature version
            if (new Version(sv).Major != new Version(SIGNATURE_VERSION).Major)
            {
                throw new Exception(string.Format("unexpected signature major version - expected: {0}, got: {1}", SIGNATURE_VERSION, sv));
            }

            // check canonicalization method
            string cm = doc.SelectSingleNode("/FirmwarePackage/Signature/CanonicalizationMethod/@Algorithm").Value;

            if (!cm.Equals(CANONICALIZATION_METHOD_URL))
            {
                throw new Exception(string.Format("unexpected canonicalization method - expected: {0}, got: {1}", CANONICALIZATION_METHOD_URL, cm));
            }

            // check digest method
            string dm = doc.SelectSingleNode("/FirmwarePackage/Signature/DigestMethod/@Algorithm").Value;

            if (!dm.Equals(DIGEST_METHOD_URL))
            {
                throw new Exception(string.Format("unexpected digest method - expected: {0}, got: {1}", DIGEST_METHOD_URL, dm));
            }

            string expectedDigest = doc.SelectSingleNode("/FirmwarePackage/Signature/DigestValue").InnerText;

            XmlDocument strippedDoc = new XmlDocument();
            strippedDoc.LoadXml(doc.SelectSingleNode("/FirmwarePackage/Body").OuterXml);

            string calculatedDigest = GetDigest(strippedDoc);

            // check package integrity
            if (!expectedDigest.Equals(calculatedDigest))
            {
                throw new Exception(string.Format("digest value does not match - expected: {0}, calculated: {1}", expectedDigest, calculatedDigest));
            }

            return DeserializeBody(strippedDoc);
        }

        public static void GenerateUpdate(string input, string output)
        {
            List<Update> updates = CreateUpdateList(input);

            CreateUpdatePackage(updates, output);
        }
    }
}
