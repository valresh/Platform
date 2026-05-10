using FlexDesigner;
using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Markup;

namespace FlexelAlarmMsg
{
  [MarkupExtensionReturnType(typeof(IValueConverter))]
  public class HysMode : MarkupExtension, IValueConverter
  {
    private static HysMode _converter = null;

    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
      return value;
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
      return null;
    }

    public override object ProvideValue(IServiceProvider serviceProvider)
    {
      if (_converter == null)
        _converter = new HysMode();
      return _converter;
    }
  }
}
