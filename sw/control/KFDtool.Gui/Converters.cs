using KFDtool.P25.Constant;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace KFDtool.Gui.Converters
{
    public class AlgorithmFormatConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            int intValue = (int)value;

            if (Enum.IsDefined(typeof(AlgorithmId), (byte)intValue))
            {
                return string.Format("{0} (0x{0:X}) {1}", intValue, ((AlgorithmId)intValue).ToString());
            }
            else
            {
                return string.Format("{0} (0x{0:X} UNKNOWN)", intValue);
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
