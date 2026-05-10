using FlexDesigner;
using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Markup;

namespace FlexelDiscretes
{
  [MarkupExtensionReturnType(typeof(IValueConverter))]
  public class CopierMode : MarkupExtension, IValueConverter
  {
    private static CopierMode _converter = null;

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
        _converter = new CopierMode();
      return _converter;
    }
  }
}
