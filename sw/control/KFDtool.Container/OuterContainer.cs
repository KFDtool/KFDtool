using System.Xml.Serialization;

namespace KFDtool.Container
{
    public class OuterContainer
    {
        [XmlAttribute("version")]
        public string Version { get; set; }

        public KeyDerivation KeyDerivation { get; set; }

        public string EncryptedDataPlaceholder { get; set; }
    }
}
