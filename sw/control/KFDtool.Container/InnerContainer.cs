using System.Collections.ObjectModel;
using System.Xml.Serialization;

namespace KFDtool.Container
{
    public class InnerContainer
    {
        [XmlAttribute("version")]
        public string Version { get; set; }

        public ObservableCollection<KeyItem> Keys { get; set; }

        public int NextKeyNumber { get; set; }

        public ObservableCollection<GroupItem> Groups { get; set; }

        public int NextGroupNumber { get; set; }
    }
}
