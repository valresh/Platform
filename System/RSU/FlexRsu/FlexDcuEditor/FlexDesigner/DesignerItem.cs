using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace FlexDesigner
{
  //These attributes identify the types of the named parts that are used for templating
  [TemplatePart(Name = "PART_MoveThumb", Type = typeof(MoveThumb))]
  [TemplatePart(Name = "PART_ConnectorDecorator", Type = typeof(Control))]
  [TemplatePart(Name = "PART_ContentPresenter", Type = typeof(ContentPresenter))]
  public class DesignerItem : ContentControl, ISelectable, IGroupable
  {
    public static readonly DependencyProperty ShowItemNameProperty = DependencyProperty.RegisterAttached("ShowItemName", typeof(bool), typeof(DesignerItem), new FrameworkPropertyMetadata(false));
    public static readonly DependencyProperty ShowExecOrderProperty = DependencyProperty.RegisterAttached("ShowExecOrder", typeof(bool), typeof(DesignerItem), new FrameworkPropertyMetadata(true));
    public static readonly DependencyProperty HiddenPropertiesProperty = DependencyProperty.RegisterAttached("HiddenProperties", typeof(HiddenPropertiesContainer), typeof(DesignerItem) );
    public static readonly DependencyProperty SpacesInNamePermittedProperty = DependencyProperty.RegisterAttached("SpacesInNamePermitted", typeof(bool), typeof(DesignerItem), new FrameworkPropertyMetadata(false));

    public static readonly DependencyProperty VisibleNameProperty = DependencyProperty.Register("VisibleName", typeof(string), typeof(DesignerItem) );
    public static readonly DependencyProperty ExecOrderProperty = DependencyProperty.Register("ExecOrder", typeof(string), typeof(DesignerItem), new FrameworkPropertyMetadata("10"));

    public static readonly DependencyProperty IsSelectedProperty = DependencyProperty.Register("IsSelected", typeof(bool), typeof(DesignerItem), new FrameworkPropertyMetadata(false));
    // can be used to replace the default template for the MoveThumb/DragThumb
    public static readonly DependencyProperty MoveThumbTemplateProperty = DependencyProperty.RegisterAttached("MoveThumbTemplate", typeof(ControlTemplate), typeof(DesignerItem));
    public static readonly DependencyProperty ConnectorDecoratorTemplateProperty = DependencyProperty.RegisterAttached("ConnectorDecoratorTemplate", typeof(ControlTemplate), typeof(DesignerItem));
    public static readonly DependencyProperty IsDragConnectionOverProperty = DependencyProperty.Register("IsDragConnectionOver", typeof(bool), typeof(DesignerItem), new FrameworkPropertyMetadata(false));
    public static readonly DependencyProperty ParentIDProperty = DependencyProperty.Register("ParentID", typeof(Guid), typeof(DesignerItem));
    public static readonly DependencyProperty IsGroupProperty = DependencyProperty.Register("IsGroup", typeof(bool), typeof(DesignerItem));
    public static readonly DependencyProperty IDProperty = DependencyProperty.Register("ID", typeof(Guid), typeof(DesignerItem));

    public static readonly DependencyProperty VendorProperty = DependencyProperty.Register("Vendor", typeof(string), typeof(DesignerItem) );
    static DesignerItem()
    {
      FrameworkElement.DefaultStyleKeyProperty.OverrideMetadata(typeof(DesignerItem), new FrameworkPropertyMetadata(typeof(DesignerItem)));
    }
    
    // Current angle of the DesignerItem
    public double Angle { get; set; }

    public string VisibleName
    {
      get { return (string)GetValue(VisibleNameProperty); }
      set { SetValue(VisibleNameProperty, value); }
    }

    public string ExecOrder
    {
      get { return (string)GetValue(ExecOrderProperty); }
      set { SetValue(ExecOrderProperty, value); }
    }

    //private Guid id;
    public Guid ID
    {
      get { return (Guid)GetValue(IDProperty); }
      set { SetValue(IDProperty, value); }
    }

    public Guid ParentID
    {
      get { return (Guid)GetValue(ParentIDProperty); }
      set { SetValue(ParentIDProperty, value); }
    }

    public bool IsGroup
    {
      get { return (bool)GetValue(IsGroupProperty); }
      set { SetValue(IsGroupProperty, value); }
    }
    
    public bool IsSelected
    {
      get { return (bool)GetValue(IsSelectedProperty); }
      set { SetValue(IsSelectedProperty, value); }
    }

    public static bool GetShowItemName(DependencyObject element)
    {
      return (bool)element.GetValue(ShowItemNameProperty);
    }

    public static void SetShowItemName(DependencyObject element, bool value)
    {
      element.SetValue(ShowItemNameProperty, value);
    }

    public static bool GetSpacesInNamePermitted(DependencyObject element)
    {
      return (bool)element.GetValue(SpacesInNamePermittedProperty);
    }

    public static void SetSpacesInNamePermitted(DependencyObject element, bool value)
    {
      element.SetValue(SpacesInNamePermittedProperty, value);
    }

    public bool SpacesInNamePermitted
    {
      get { return (bool)GetValue(SpacesInNamePermittedProperty); }
      set { SetValue(SpacesInNamePermittedProperty, value); }
    }

    public static bool GetShowExecOrder(DependencyObject element)
    {
      return (bool)element.GetValue(ShowExecOrderProperty);
    }

    public static void SetShowExecOrder(DependencyObject element, bool value)
    {
      element.SetValue(ShowExecOrderProperty, value);
    }

    public static bool GetVendor(DependencyObject element)
    {
        return (bool)element.GetValue(VendorProperty);
    }

    public static void SetVendor(DependencyObject element, string value)
    {
        element.SetValue(VendorProperty, value);
    }

    public static HiddenPropertiesContainer GetHiddenProperties(DependencyObject element)
    {
      return (HiddenPropertiesContainer)element.GetValue(HiddenPropertiesProperty);
    }

    public static void SetHiddenProperties(DependencyObject element, HiddenPropertiesContainer value)
    {
      element.SetValue(HiddenPropertiesProperty, value);
    }

    public static ControlTemplate GetMoveThumbTemplate(UIElement element)
    {
      return (ControlTemplate)element.GetValue(MoveThumbTemplateProperty);
    }

    public static void SetMoveThumbTemplate(UIElement element, ControlTemplate value)
    {
      element.SetValue(MoveThumbTemplateProperty, value);
    }

    public static ControlTemplate GetConnectorDecoratorTemplate(UIElement element)
    {
      return (ControlTemplate)element.GetValue(ConnectorDecoratorTemplateProperty);
    }

    public static void SetConnectorDecoratorTemplate(UIElement element, ControlTemplate value)
    {
      element.SetValue(ConnectorDecoratorTemplateProperty, value);
    }

    // while drag connection procedure is ongoing and the mouse moves over 
    // this item this value is true; if true the ConnectorDecorator is triggered
    // to be visible, see template
    public bool IsDragConnectionOver
    {
      get { return (bool)GetValue(IsDragConnectionOverProperty); }
      set { SetValue(IsDragConnectionOverProperty, value); }
    }

    public DesignerItem(Guid _id)
    {
      ID = _id;
      this.Loaded += new RoutedEventHandler(this.DesignerItem_Loaded);
    }

    public DesignerItem() : this(Guid.NewGuid())
    {
    }

    protected override void OnPreviewMouseDown(MouseButtonEventArgs e)
    {
      base.OnPreviewMouseDown(e);
      DesignerCanvas designer = VisualTreeHelper.GetParent(this) as DesignerCanvas;

      e.Handled = false;
      // update selection
      if (designer == null)
        return;
      if (EditorModes.ViewOnly == designer.Mode)
        return;
      if ((Keyboard.Modifiers & (ModifierKeys.Shift | ModifierKeys.Control)) != ModifierKeys.None)
      {
        if (this.IsSelected)
          designer.SelectionService.RemoveFromSelection(this);
        else
          designer.SelectionService.AddToSelection(this);
      }
      else if (!this.IsSelected)
        designer.SelectionService.SelectItem(this);
      Focus();
    }

    private void DesignerItem_Loaded(object sender, RoutedEventArgs e)
    {
      if (this.Template == null)
        return;
      ContentPresenter contentPresenter = this.Template.FindName("PART_ContentPresenter", this) as ContentPresenter;
      if (contentPresenter == null )
        return;
      int cc = VisualTreeHelper.GetChildrenCount(contentPresenter);
      if (0 == cc)
        return;

      UIElement contentVisual = VisualTreeHelper.GetChild(contentPresenter, 0) as UIElement;
      if (contentVisual == null)
        return;

      MoveThumb thumb = this.Template.FindName("PART_MoveThumb", this) as MoveThumb;
      if (thumb != null)
      {
        ControlTemplate template = DesignerItem.GetMoveThumbTemplate(contentVisual) as ControlTemplate;
        if (template != null)
          thumb.Template = template;
      }

      //Control connectorDecorator = this.Template.FindName("PART_ConnectorDecorator", this) as Control;
      //if (connectorDecorator != null)
      //{
      //  ControlTemplate template = DesignerItem.GetConnectorDecoratorTemplate(contentVisual) as ControlTemplate;
      //  if (template != null)
      //    connectorDecorator.Template = template;
      //}
    }

  }
}
