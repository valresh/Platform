using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Markup;

namespace FlexDesigner
{
  [MarkupExtensionReturnType(typeof(IValueConverter))]
  public class ModeIsCheckConverter : MarkupExtension, IValueConverter
  {
    private static ModeIsCheckConverter _converter = null;

    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
      EditorModes emp = (EditorModes)parameter;
      EditorModes emv = (EditorModes)value;
      return emp == emv;
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
      return null;
    }

    public override object ProvideValue(IServiceProvider serviceProvider)
    {
      if (_converter == null)
        _converter = new ModeIsCheckConverter();
      return _converter;
    }
  }
}
