using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace FlexDesigner
{
  public class ReferenceCenterPositionPanel : Panel
  {
    public static readonly DependencyProperty ReferenceCenterPositionProperty = DependencyProperty.RegisterAttached("ReferenceCenterPosition", typeof(Point), typeof(ReferenceCenterPositionPanel), new FrameworkPropertyMetadata(new Point(0, 0), new PropertyChangedCallback(ReferenceCenterPositionPanel.OnReferenceCenterPositionChanged)));

    public static Point GetReferenceCenterPosition(UIElement element)
    {
      if (element == null)
      {
        throw new ArgumentNullException("element");
      }
      return (Point)element.GetValue(ReferenceCenterPositionProperty);
    }

    public static void SetReferenceCenterPosition(UIElement element, Point value)
    {
      if (element == null)
      {
        throw new ArgumentNullException("element");
      }
      element.SetValue(ReferenceCenterPositionProperty, value);
    }

    private static void OnReferenceCenterPositionChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
      UIElement reference = d as UIElement;
      if (reference != null)
      {
        ReferenceCenterPositionPanel parent = VisualTreeHelper.GetParent(reference) as ReferenceCenterPositionPanel;
        if (parent != null)
        {
          parent.InvalidateArrange();
        }
      }
    }

    protected override Size ArrangeOverride(Size arrangeSize)
    {
      foreach (UIElement element in base.InternalChildren)
      {
        if (element != null)
        {
          Point absPosition = GetReferenceCenterPosition(element);
          double x = arrangeSize.Width/2 + absPosition.X - (element.DesiredSize.Width / 2);
          double y = absPosition.Y - (element.DesiredSize.Height / 2);

          if (double.IsNaN(x)) x = 0;
          if (double.IsNaN(y)) y = 0;

          element.Arrange(new Rect(new Point(x, y), element.DesiredSize));
        }
      }
      return arrangeSize;
    }

    protected override Size MeasureOverride(Size availableSize)
    {
      Size size = new Size(double.PositiveInfinity, double.PositiveInfinity);

      // SDK docu says about InternalChildren Property: 'Classes that are derived from Panel 
      // should use this property, instead of the Children property, for internal overrides 
      // such as MeasureCore and ArrangeCore.

      foreach (UIElement element in this.InternalChildren)
      {
        if (element != null)
          element.Measure(size);
      }

      return base.MeasureOverride(availableSize);
    }
  }
}
