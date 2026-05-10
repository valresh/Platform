using System.Windows;
using System.Windows.Controls;
using System.ComponentModel;

namespace FlexDesigner
{
  public class HiddenProperty : DependencyObject, INotifyPropertyChanged
  {
    public static readonly DependencyProperty ValueProperty = DependencyProperty.Register("Value", typeof(string), typeof(HiddenProperty), new FrameworkPropertyMetadata( "", ValueChanged));
    public HiddenProperty() { }
    public HiddenProperty( string k, string v )
    {
      Key = k;
      Value = v;
    }
    public string Key { get; set; }

    public string PropName { get; set; }

    public object Owner { get; set; }
    public string Value
    {
      get
      {
        return (string)GetValue(ValueProperty);
      }
      set
      {
        SetValue(ValueProperty, value);
      }
    }

    private static void ValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
      HiddenProperty hp = d as HiddenProperty;
      if( e.OldValue is string && e.NewValue is string )
      {
        string so = e.OldValue as string;
        string sn = e.NewValue as string;
        if (string.Equals(so, sn))
          return;
        hp.OnPropertyChanged(e.Property.ToString());
      }
      if (null == hp)
        return;
      if (string.IsNullOrEmpty(hp.PropName))
        return;
      if (null == hp.Owner)
        return;
      DependencyObject hitObject = hp.Owner as DependencyObject;
      int n = System.Windows.Media.VisualTreeHelper.GetChildrenCount(hitObject);
      for( int i=0; i<n; ++i )
      {
        DependencyObject child = System.Windows.Media.VisualTreeHelper.GetChild(hitObject, i);
        if (Change(child, hp))
          break;
      }
    }

    private static bool Change( DependencyObject hitObject, HiddenProperty hp)
    {
      string name = hitObject.GetValue(FrameworkElement.NameProperty) as string;
      if (hp.PropName == name)
      {
        hitObject.SetValue(TextBlock.TextProperty, hp.Value);
        return true;
      }
      int n = System.Windows.Media.VisualTreeHelper.GetChildrenCount(hitObject);
      for (int i = 0; i < n; ++i)
      {
        DependencyObject child = System.Windows.Media.VisualTreeHelper.GetChild(hitObject, i);
        if (Change(child, hp))
          return true;
      }
      return false;
    }

    public event PropertyChangedEventHandler PropertyChanged;
    protected void OnPropertyChanged(string propertyName)
    {
      PropertyChangedEventHandler handler = PropertyChanged;
      if (handler != null)
        handler(this, new PropertyChangedEventArgs(propertyName));
    }

  }
}
