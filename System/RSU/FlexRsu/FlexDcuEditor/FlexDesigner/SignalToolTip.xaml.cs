using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;

namespace FlexDesigner
{
  /// <summary>
  /// Interaction logic for SignalToolTip.xaml
  /// </summary>
  public partial class SignalToolTip : UserControl
  {
    public static readonly DependencyProperty srcValueProperty = DependencyProperty.RegisterAttached("srcValue", typeof(string), typeof(SignalToolTip));
    public static readonly DependencyProperty destValueProperty = DependencyProperty.RegisterAttached("destValue", typeof(string), typeof(SignalToolTip));

    /*public static readonly DependencyProperty srcNameProperty = DependencyProperty.RegisterAttached("srcName", typeof(string), typeof(SignalToolTip));
    public static readonly DependencyProperty destNameProperty = DependencyProperty.RegisterAttached("destName", typeof(string), typeof(SignalToolTip));*/

    public SignalToolTip()
    {
      InitializeComponent();
    }

    public string srcValue
    {
      get { return (string)GetValue(srcValueProperty); }
      set { SetValue(srcValueProperty, value); }
    }

    public string destValue
    {
      get { return (string)GetValue(destValueProperty); }
      set { SetValue(destValueProperty, value); }
    }

    public string srcName
    {
      get
      {
        return srcMemory.szPntName;
      }
      set {}
    }

    public string destName
    {
      get
      {
        return destMemory.szPntName;
      }
      set {}
    }

    public SSignalMemDescr srcMemory
    {
      get;
      set;
    }

    public SSignalMemDescr destMemory
    {
      get;
      set;
    }

    string GetValue(SSignalMemDescr sig)
    {
      string res = String.Empty;
      if (IntPtr.Zero == sig.ptrToMem)
        return res;

      switch (sig.type)
      {
        case eVarType.evtDigital32:
          {
            Int32 test = (Int32)Marshal.PtrToStructure(sig.ptrToMem, typeof(Int32));
            if( 1==test )
              res = "true";
            else if( 0==test )
              res = "false";
            else
              res = test.ToString();
          }
          break;
        case eVarType.evtAnalog32:
          {
            float test = (float)Marshal.PtrToStructure(sig.ptrToMem, typeof(float));
            res = test.ToString();
          }
          break;
      }

      return res;
    }

    public int GetDigValue(SSignalMemDescr sig)
    {
      switch (sig.type)
      {
        case eVarType.evtDigital32:
          {
            Int32 test = (Int32)Marshal.PtrToStructure(sig.ptrToMem, typeof(Int32));
            return test;
          }
      }
      return Int32.MinValue;
    }

    public void Refresh()
    {
      srcValue = GetValue(srcMemory);
      destValue = GetValue(destMemory);
    }

    public bool IsDigital()
    {
      if (eVarType.evtDigital32 != srcMemory.type || eVarType.evtDigital32 != destMemory.type)
        return false;
      return true;
    }

  }
}
