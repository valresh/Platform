using System;
using System.Collections;
using System.IO;
using System.Reflection;
using System.Resources;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Markup;

namespace FlexChemEditor
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    public static readonly DependencyProperty ToolbarButtonSizeProperty = DependencyProperty.Register("ToolbarButtonSize", typeof(double), typeof(MainWindow), new PropertyMetadata(72.0));
    public MainWindow()
    {
      InitializeComponent();
      
      ApplicationCommands.Save.InputGestures.Add(new KeyGesture(Key.S, ModifierKeys.Control));
      ApplicationCommands.Copy.InputGestures.Add(new KeyGesture(Key.C, ModifierKeys.Control));
      ApplicationCommands.Paste.InputGestures.Add(new KeyGesture(Key.V, ModifierKeys.Control));
      Loaded += MainWindow_Loaded;

      DataObject.AddPastingHandler(DgName, OnPaste2DgName);
    }

    private void MainWindow_Loaded(object sender, RoutedEventArgs e)
    {
      Loaded -= MainWindow_Loaded;

      string d = AppDomain.CurrentDomain.BaseDirectory;
      string[] plugins = Directory.GetFiles(d, "Flexel*.dll");

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
            Elements.Children.Add(expander);
            foreach (FrameworkElement i in tb.Items)
            {
              string name = i.Name;
              string xamlString = XamlWriter.Save(i);
              object v = i.GetValue(FlexDesigner.DesignerItem.VendorProperty);
              if (null != v)
              {
                name = i.Name.Insert(0, ":" );
                name = name.Insert(0, v.ToString() );
              }
              designerCanvas.AddPrimitiveElements( name, xamlString);
            }
          }//if
        }//foreach resources
      }

      designerCanvas.SelectionService.ReselectedItem += SelectionService_ReselectedItem;
    }

    private void SelectionService_ReselectedItem(FlexDesigner.HiddenPropertiesContainer c, UIElement name, UIElement ExecOrder)
    {
      DgProperties.ItemsSource = c;
      DgProperties.Items.Refresh();

      DgName.DataContext = name;
      if (null == DgName.DataContext )
        DgName.Visibility = Visibility.Collapsed;
      else
        DgName.Visibility = Visibility.Visible;

      DgExecOrder.DataContext = ExecOrder;
      if (null == DgExecOrder.DataContext)
        DgExecOrder.Visibility = Visibility.Collapsed;
      else
        DgExecOrder.Visibility = Visibility.Visible;
    }

        private void DgName_PreviewKeyDown(object sender, KeyEventArgs e)
        {
          if( null!= DgName.DataContext )
          {
            FlexDesigner.DesignerItem di = DgName.DataContext as FlexDesigner.DesignerItem;
            if (null != di)
            {
              bool b = di.SpacesInNamePermitted;
              string vn = di.VisibleName;
              if (b)
                return;
            }
          }
          if (Key.Space == e.Key)
              e.Handled = true;
        }

        private void OnPaste2DgName(object sender, DataObjectPastingEventArgs e)
        {
            bool isText = e.SourceDataObject.GetDataPresent(DataFormats.UnicodeText, true);
            if (!isText)
                return;

            string pastedText = e.SourceDataObject.GetData(DataFormats.UnicodeText) as string;

            if (string.IsNullOrEmpty(pastedText))
                return;

            if (null != DgName.DataContext)
            {
              FlexDesigner.DesignerItem di = DgName.DataContext as FlexDesigner.DesignerItem;
              if (null != di)
              {
                bool b = di.SpacesInNamePermitted;
                string vn = di.VisibleName;
                if (b)
                  return;
              }
            }

            string s = string.Join("", pastedText.Split(default(string[]), StringSplitOptions.RemoveEmptyEntries));
            pastedText = s;
            
            TextBox txtBox = (TextBox)sender;

            int caretIndex = txtBox.CaretIndex;

            if (txtBox.SelectionLength > 0)
                txtBox.Text = txtBox.Text.Remove(txtBox.SelectionStart, txtBox.SelectionLength);

            string before = ""; //Text before pasted text
            string after = txtBox.Text; //Text after pasted text

            //Get before and after text
            if (caretIndex > 0)
            {
                before = txtBox.Text.Substring(0, caretIndex);
                after = txtBox.Text.Substring(caretIndex);
            }

            int newCaretIndex = before.Length + pastedText.Length;

            e.CancelCommand(); //Cancels the paste, we do it manually
            //txtBox.Text = $"{before}{pastedText}{after}"; //Set new text
            txtBox.Text = before + pastedText + after;
            txtBox.CaretIndex = newCaretIndex; //Set new caret index
        }
    }
}
