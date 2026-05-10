using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace FlexDesigner
{
  partial class DesignerCanvas
  {
    public DesignerCanvas()
    {
      this.CommandBindings.Add(new CommandBinding(ApplicationCommands.Delete, Delete_Executed, Delete_Enabled) );
      this.CommandBindings.Add(new CommandBinding(ApplicationCommands.New, New_Executed));
      this.CommandBindings.Add(new CommandBinding(ApplicationCommands.Open, Open_Executed));
      this.CommandBindings.Add(new CommandBinding(ApplicationCommands.Save, Save_Executed));
      this.CommandBindings.Add(new CommandBinding(ApplicationCommands.SaveAs, SaveAs_Executed));
      this.CommandBindings.Add(new CommandBinding(ApplicationCommands.Copy, Copy_Executed, Copy_Enabled));
      this.CommandBindings.Add(new CommandBinding(ApplicationCommands.Paste, Paste_Executed, Paste_Enabled));

      ResetUnicalNumber();

      KeyUp += new KeyEventHandler(DesignerCanvas_KeyUp);
      KeyDown += new KeyEventHandler(DesignerCanvas_KeyDown);
    }

    void DesignerCanvas_KeyDown(object sender, KeyEventArgs e)
    {
      e.Handled = true;
      switch (e.Key)
      {
        case Key.Up:
        case Key.Down:
        case Key.Right:
        case Key.Left:
          break;
        default:
          return;
      }

      DesignerCanvas designer = sender as DesignerCanvas;
      if (designer == null)
        return;
      if (EditorModes.ViewOnly == designer.Mode)
        return;

      var designerItems = designer.SelectionService.CurrentSelection.OfType<DesignerItem>();
      if (0 == designerItems.Count())
        return;
      Focus();
    }

    void DesignerCanvas_KeyUp(object sender, KeyEventArgs e)
    {
      e.Handled = true;
      double HorizontalChange = 0, VerticalChange = 0;
      switch (e.Key)
      {
      case Key.Up:
        VerticalChange = -1;
        break;
      case Key.Down:
        VerticalChange = 1;
        break;
      case Key.Right:
        HorizontalChange = 1;
        break;
      case Key.Left:
        HorizontalChange = -1;
        break;
      default:
        return;
      }

      DesignerCanvas designer = sender as DesignerCanvas;
      if (designer == null)
        return;
      if (EditorModes.ViewOnly == designer.Mode)
        return;
      var designerItems = designer.SelectionService.CurrentSelection.OfType<DesignerItem>();
      if (0 == designerItems.Count())
        return;

      if( (Keyboard.Modifiers & ModifierKeys.Control) != ModifierKeys.None )
      {
        Size gridSize = GetSnapGridSize();
        HorizontalChange *= gridSize.Width;
        VerticalChange *= gridSize.Height;
      }

      double minLeft = double.MaxValue;
      double minTop = double.MaxValue;

      foreach (DesignerItem item in designerItems)
      {
        double left = Canvas.GetLeft(item);
        double top = Canvas.GetTop(item);

        minLeft = double.IsNaN(left) ? 0 : Math.Min(left, minLeft);
        minTop = double.IsNaN(top) ? 0 : Math.Min(top, minTop);
      }

      double deltaHorizontal = Math.Max(-minLeft, HorizontalChange);
      double deltaVertical = Math.Max(-minTop, VerticalChange);

      foreach (DesignerItem item in designerItems)
      {
        double left = Canvas.GetLeft(item);
        double top = Canvas.GetTop(item);

        if (double.IsNaN(left)) left = 0;
        if (double.IsNaN(top)) top = 0;

        Canvas.SetLeft(item, left + deltaHorizontal);
        Canvas.SetTop(item, top + deltaVertical);
      }

      designer.InvalidateMeasure();
      Focus();
    }

    private bool TestForUnicalNumber( int test )
    {
      IEnumerable<DesignerItem> designerItems = this.Children.OfType<DesignerItem>();
      foreach (DesignerItem item in designerItems)
      {
        if (string.IsNullOrEmpty(item.ExecOrder))
          continue;
        int curE = int.Parse(item.ExecOrder);
        if (test == curE)
          return false;
      }
      return true;
    }

    private void ResetUnicalNumber()
    {
      UnicalNumber = 100;
    }
    

    private void New_Executed(object sender, ExecutedRoutedEventArgs e)
    {
      ResetUnicalNumber();
      this.Children.Clear();
      this.SelectionService.ClearSelection();
      Clear();
    }

    private void Delete_Executed(object sender, ExecutedRoutedEventArgs e)
    {
      DeleteCurrentSelection();
    }

    private void Delete_Enabled(object sender, CanExecuteRoutedEventArgs e)
    {
      e.CanExecute = this.SelectionService.CurrentSelection.Count() > 0;
    }

    private void DeleteCurrentSelection()
    {
      foreach (Connection connection in SelectionService.CurrentSelection.OfType<Connection>())
      {
        this.Children.Remove(connection);
      }

      foreach (DesignerItem item in SelectionService.CurrentSelection.OfType<DesignerItem>())
      {
        Control cd = item.Template.FindName("PART_ConnectorDecorator", item) as Control;

        List<Connector> connectors = new List<Connector>();
        GetConnectors(cd, connectors);

        foreach (Connector connector in connectors)
        {
          foreach (Connection con in connector.Connections)
          {
            this.Children.Remove(con);
          }
        }
        this.Children.Remove(item);
      }

      foreach (Primitives.Primitive prim in SelectionService.CurrentSelection.OfType<Primitives.Primitive>())
      {
        this.Children.Remove(prim);
      }

      SelectionService.ClearSelection();
      //UpdateZIndex();
    }

    private void GetConnectors(DependencyObject parent, List<Connector> connectors)
    {
      int childrenCount = VisualTreeHelper.GetChildrenCount(parent);
      for (int i = 0; i < childrenCount; i++)
      {
        DependencyObject child = VisualTreeHelper.GetChild(parent, i);
        if (child is Connector)
        {
          connectors.Add(child as Connector);
        }
        else
          GetConnectors(child, connectors);
      }
    }

    void Clear()
    {
      var metadata = OpennedFileProperty.GetMetadata(typeof(DesignerCanvas));
      SetValue(OpennedFileProperty, metadata.DefaultValue);
      FullPath2CurFile = null;
    }

    void SetCurFile( string fullPath )
    {
      SetValue(OpennedFileProperty, fullPath);
      FullPath2CurFile = fullPath;
    }

  }
}
