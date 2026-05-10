using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Markup;
using System.Linq;

namespace FlexDesigner
{
  [MarkupExtensionReturnType(typeof(IValueConverter))]
  public class fixed_out_find : MarkupExtension, IValueConverter
  {
    private static fixed_out_find _converter = null;

    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
      HiddenPropertiesContainer hpc = value as HiddenPropertiesContainer;
      if (null == hpc)
        return parameter;

      HiddenProperty hp = hpc.Single(x => x.Key == "FIXED_OUT");
      if (null == hp)
        return parameter;

      return hp.Value;
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
      return value;
    }

    public override object ProvideValue(IServiceProvider serviceProvider)
    {
      if (_converter == null)
        _converter = new fixed_out_find();
      return _converter;
    }

  }
}
