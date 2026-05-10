using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Resources;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

namespace FlexViewer
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    public List<FlexDesigner.SSignalDescriptor> memories = null;

    static List<Window> openedDebugWindows = null;

    public string ExePath = null;
    public MainWindow()
    {
      InitializeComponent();

      Loaded += MainWindow_Loaded;

      //memories = new List<FlexDesigner.SSignalDescriptor>();
      if( null==openedDebugWindows )
        openedDebugWindows = new List<Window>();
    }

    private void MainWindow_Loaded(object sender, RoutedEventArgs e)
    {
      Loaded -= MainWindow_Loaded;
      Closed += new EventHandler(MainWindow_Closed);
      openedDebugWindows.Add(this);

      string d = AppDomain.CurrentDomain.BaseDirectory;
      string[] plugins = Directory.GetFiles(d, "Flexel*.dll");

      if( 0==plugins.Length && !String.IsNullOrEmpty(ExePath) )
        plugins = Directory.GetFiles( ExePath, "Flexel*.dll");

      foreach (string path in plugins)
      {
        var assembly = Assembly.LoadFile(path);
        var stream = assembly.GetManifestResourceStream(assembly.GetName().Name + ".g.resources");
        var resourceReader = new ResourceReader(stream);

        foreach (DictionaryEntry resource in resourceReader)
        {
          if (new FileInfo(resource.Key.ToString()).Extension.Equals(".baml"))
          {
            Uri uri = new Uri("/" + assembly.GetName().Name + ";component/" + resource.Key.ToString().Replace(".baml", ".xaml"), UriKind.Relative);
            object obj = Application.LoadComponent(uri) as FrameworkElement;
            if (null == obj)
              continue;
            Expander expander = obj as Expander;
            if (null == expander)
              continue;
            FlexDesigner.Toolbox tb = expander.Content as FlexDesigner.Toolbox;
            if (null == tb)
              continue;
            //Elements.Children.Add(expander);
            foreach (FrameworkElement i in tb.Items)
            {
              string name = i.Name;
              string xamlString = XamlWriter.Save(i);
              object v = i.GetValue(FlexDesigner.DesignerItem.VendorProperty);
              if (null != v)
              {
                name = i.Name.Insert(0, ":");
                name = name.Insert(0, v.ToString());
              }
              designerCanvas.AddPrimitiveElements(name, xamlString);
            }
          }//if
        }//foreach resources
      }
    }

    void MainWindow_Closed(object sender, EventArgs e)
    {
      Window w = sender as Window;
      if (null == w || null == openedDebugWindows )
        return;
      openedDebugWindows.Remove(w);
    }

    public static void CloseOpenedWindows()
    {
      if( null==openedDebugWindows )
        return;
      while (0!=openedDebugWindows.Count)
      {
        openedDebugWindows[0].Close();
      }
    }

    public void OpenWindow(string fullPath)
    {
      designerCanvas.m_memoriesOfView = memories;
      designerCanvas.OpenFile(fullPath);
    }
  }
}
