using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Controls;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Markup;
using System.Xml;
using System.IO;
using System.Windows.Documents;

namespace FlexDesigner
{
  public partial class DesignerCanvas : Canvas
  {
    public static readonly DependencyProperty OpennedFileProperty = DependencyProperty.Register("OpennedFile", typeof(string), typeof(DesignerCanvas), new FrameworkPropertyMetadata("FlexDCU Editor", OnOpennedFileChanged));

    int UnicalNumber;

    private Point? dragStartPoint = null;

    Dictionary<string, string> _PrimitiveElements;

    public void AddPrimitiveElements(string key, string val)
    {
      if (null == _PrimitiveElements)
        _PrimitiveElements = new Dictionary<string, string>();
      _PrimitiveElements.Add(key, val);
    }

    private SelectionService _selectionService;
    public SelectionService SelectionService
    {
      get
      {
        if (_selectionService == null)
          _selectionService = new SelectionService(this);

        return _selectionService;
      }
    }

    public string OpennedFile
    {
      get { return (string)GetValue(OpennedFileProperty); }
      set { SetValue(OpennedFileProperty, value); }
    }

    string FullPath2CurFile
    {
      get;
      set;
    }

    static void OnOpennedFileChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
    }


    protected override Size MeasureOverride(Size constraint)
    {
      Size size = new Size();
      foreach (UIElement element in base.Children)
      {
        double left = Canvas.GetLeft(element);
        double top = Canvas.GetTop(element);
        left = double.IsNaN(left) ? 0 : left;
        top = double.IsNaN(top) ? 0 : top;

        //measure desired size for each child
        element.Measure(constraint);

        Size desiredSize = element.DesiredSize;
        if (!double.IsNaN(desiredSize.Width) && !double.IsNaN(desiredSize.Height))
        {
          size.Width = Math.Max(size.Width, left + desiredSize.Width);
          size.Height = Math.Max(size.Height, top + desiredSize.Height);
        }
      }
      //for aesthetic reasons add extra points
      size.Width += 50;
      size.Height += 50;
      return size;
    }

    protected override void OnDrop(DragEventArgs e)
    {
      base.OnDrop(e);
      
      string xamlString = e.Data.GetData("DESIGNER_ITEM") as string;
      if (String.IsNullOrEmpty(xamlString))
        return;
      FrameworkElement content = XamlReader.Load(XmlReader.Create(new StringReader(xamlString))) as FrameworkElement;
      if (content == null)
        return;
      string name = content.Name;

      DesignerItem newItem = new DesignerItem();
      newItem.Content = content;

      Point position = e.GetPosition(this);

      /*if (dragObject.DesiredSize.HasValue)
      {
        Size desiredSize = dragObject.DesiredSize.Value;
        newItem.Width = desiredSize.Width;
        newItem.Height = desiredSize.Height;

        DesignerCanvas.SetLeft(newItem, Math.Max(0, position.X - newItem.Width / 2));
        DesignerCanvas.SetTop(newItem, Math.Max(0, position.Y - newItem.Height / 2));
      }
      else*/
      {
        DesignerCanvas.SetLeft(newItem, Math.Max(0, position.X));
        DesignerCanvas.SetTop(newItem, Math.Max(0, position.Y));
      }

      int nIncr = 0;
      if (DesignerItem.GetShowItemName(content))
      {
        newItem.VisibleName = String.Format("{0}_{1}", name, UnicalNumber);
        ++nIncr;
      }

      if (DesignerItem.GetShowExecOrder(content))
      {
        newItem.ExecOrder = UnicalNumber.ToString();
        ++nIncr;
      }
      else
        newItem.ExecOrder = string.Empty;

      newItem.SpacesInNamePermitted = DesignerItem.GetSpacesInNamePermitted(content);

      if ( nIncr>0 )
        UnicalNumber += 10;
      //Canvas.SetZIndex(newItem, this.Children.Count);
      this.Children.Add(newItem);
      SetConnectorDecoratorTemplate(newItem);

      //update selection
      this.SelectionService.SelectItem(newItem);
      newItem.Focus();

      if( IsGridVisible )
        SnapToGrid(newItem, new List<UIElement>());
      e.Handled = true;
    }

    private void SetConnectorDecoratorTemplate(DesignerItem item)
    {
      if (item.ApplyTemplate() && item.Content is UIElement)
      {
        ControlTemplate template = DesignerItem.GetConnectorDecoratorTemplate(item.Content as UIElement);
        Control decorator = item.Template.FindName("PART_ConnectorDecorator", item) as Control;
        if (decorator != null && template != null)
          decorator.Template = template;
      }
    }

    protected override void OnMouseDown(MouseButtonEventArgs e)
    {
      base.OnMouseDown(e);
      if (e.Source != this)
        return;
      e.Handled = true;
      if (EditorModes.ViewOnly == Mode)
        return;
      // in case that this click is the start of a 
      // drag operation we cache the start point
      if (EditorModes.Empty == Mode)
        dragStartPoint = new Point?(e.GetPosition(this));
      else
      {
        AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(this);
        if (adornerLayer != null)
        {
          Primitives.LineAdorner adorner = new Primitives.LineAdorner(this, e.GetPosition(this) );
          adornerLayer.Add(adorner);
        }
      }

      // if you click directly on the canvas all 
      // selected items are 'de-selected'
      SelectionService.ClearSelection();
      Focus();
      e.Handled = true;
    }

    protected override void OnMouseMove(MouseEventArgs e)
    {
      base.OnMouseMove(e);
      e.Handled = true;

      if (e.LeftButton != MouseButtonState.Pressed)
        this.dragStartPoint = null;

      if (this.dragStartPoint.HasValue)
      {
        // ... but if mouse button is pressed and start
        // point value is set we do have one
        AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(this);
        if (adornerLayer != null)
        {
          RubberbandAdorner adorner = new RubberbandAdorner(this, this.dragStartPoint);
          if (adorner != null)
            adornerLayer.Add(adorner);
        }
      }
    }

  }

}
