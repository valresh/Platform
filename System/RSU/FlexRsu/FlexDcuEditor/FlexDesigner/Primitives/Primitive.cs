using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace FlexDesigner.Primitives
{
  public class Primitive : Control, ISelectable, IGroupable, INotifyPropertyChanged
  {
    //public static readonly DependencyProperty ParentIDProperty = DependencyProperty.Register("ParentID", typeof(Guid), typeof(DesignerItem));
    // we could use DependencyProperties as well to inform others of property changes
    public event PropertyChangedEventHandler PropertyChanged;

    public Primitive(Guid id)
    {
#if DEBUG
#else
      FocusVisualStyle = null;
#endif
      ID = id;
    }

    protected void OnPropertyChanged(string name)
    {
      PropertyChangedEventHandler handler = PropertyChanged;
      if (handler != null)
      {
        handler(this, new PropertyChangedEventArgs(name));
      }
    }

    public Guid ID { get; set; }
    public Guid ParentID { get; set; }
    public bool IsGroup { get; set; }

    // pattern of dashes and gaps that is used to outline the connection path
    private DoubleCollection strokeDashArray;
    public DoubleCollection StrokeDashArray
    {
      get
      {
        return strokeDashArray;
      }
      set
      {
        if (strokeDashArray != value)
        {
          strokeDashArray = value;
          OnPropertyChanged("StrokeDashArray");
        }
      }
    }

    private double strokeThickness;
    public double StrokeThickness
    {
      get
      {
        return strokeThickness;
      }
      set
      {
        if (strokeThickness != value)
        {
          strokeThickness = value;
          OnPropertyChanged("StrokeThickness");
        }
      }
    }

    private bool isSelected;
    public bool IsSelected
    {
      get { return isSelected; }
      set
      {
        if (isSelected != value)
        {
          isSelected = value;
          OnPropertyChanged("IsSelected");
          if (isSelected)
            ShowAdorner();
          else
            HideAdorner();
        }
      }
    }

    virtual protected void ShowAdorner()
    {
    }

    virtual protected void HideAdorner()
    {
    }

    virtual public void MoveThumbDragged( double dx, double dy )
    {
    }

    virtual public void MoveThumbDraggedCompleted()
    {
    }

    private Point anchorPositionSource;
    public Point AnchorPositionSource
    {
      get { return anchorPositionSource; }
      set
      {
        if (anchorPositionSource != value)
        {
          anchorPositionSource = value;
          OnPropertyChanged("AnchorPositionSource");
        }
      }
    }

    private Point anchorPositionSink;
    public Point AnchorPositionSink
    {
      get { return anchorPositionSink; }
      set
      {
        if (anchorPositionSink != value)
        {
          anchorPositionSink = value;
          OnPropertyChanged("AnchorPositionSink");
        }
      }
    }
  }
}
