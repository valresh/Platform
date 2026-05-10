using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Markup;
using System.Windows.Controls;
using System.Windows.Data;
using System.Globalization;
using System.Windows.Input;

namespace FlexDesigner
{
  public partial class DesignerCanvas
  {
    private ICommand _ChangeMode;

    public static readonly DependencyProperty ModeProperty = DependencyProperty.Register("Mode", typeof(EditorModes), typeof(DesignerCanvas), new PropertyMetadata(EditorModes.Empty, ModeChanged));

    public ICommand ChangeMode
    {
      get
      {
        if (null == _ChangeMode)
        {
          _ChangeMode = new RelayCommand(param => this.ChangeMode_Executed(param), null);
        }
        return _ChangeMode;
      }
    }

    public EditorModes Mode
    {
      get { return (EditorModes)GetValue(ModeProperty); }
      set 
      { SetValue(ModeProperty, value); }
    }

    private static void ModeChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
    }

    private void ChangeMode_Executed( object obj )
    {
      if (null == obj)
        return;
      Mode = (EditorModes)obj;
    }

  }

}
