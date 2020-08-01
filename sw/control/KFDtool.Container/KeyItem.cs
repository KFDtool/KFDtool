using System.ComponentModel;

namespace KFDtool.Container
{
    public class KeyItem : INotifyPropertyChanged
    {
        public int Id { get; set; }

        private string _name;

        public string Name
        {
            get { return _name; }

            set
            {
                if (_name != value)
                {
                    _name = value;

                    NotifyPropertyChanged("Name");
                }
            }
        }

        public bool ActiveKeyset { get; set; }

        public int KeysetId { get; set; }

        public int Sln { get; set; }

        public bool KeyTypeAuto { get; set; }

        public bool KeyTypeTek { get; set; }

        public bool KeyTypeKek { get; set; }

        public int KeyId { get; set; }

        public int AlgorithmId { get; set; }

        public string Key { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        public void NotifyPropertyChanged(string propName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propName));
            }
        }

        public override string ToString()
        {
            return Name.ToString();
        }
    }
}
