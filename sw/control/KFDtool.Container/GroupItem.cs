using System.Collections.Generic;
using System.ComponentModel;

namespace KFDtool.Container
{
    public class GroupItem : INotifyPropertyChanged
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

        public List<int> Keys { get; set; }

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
