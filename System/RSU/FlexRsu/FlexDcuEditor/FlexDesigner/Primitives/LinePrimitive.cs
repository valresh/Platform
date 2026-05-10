using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls.Primitives;

namespace FlexDesigner.Primitives
{
  public class LinePrimitive : Primitive
  {
    private Adorner lineAdorner;
    Point offsetLT;
    
    public LinePrimitive(Point s, Point e)
      : base( Guid.NewGuid() )
    {
      PathGeometry geometry = new PathGeometry();
      PathFigure figure = new PathFigure();

      figure.StartPoint = s;
      LineSegment segment = new LineSegment(e, true);
      figure.Segments.Add(segment);
      geometry.Figures.Add(figure);
      this.PathGeometry = geometry;
      StrokeThickness = 1;
      //StrokeDashArray = new DoubleCollection(new double[] { 10, 20 });
    }

    //относительно 0,0 ограничевающего прямоугольника
    Point RelateToCanvas( Point pt )
    {
      pt.Offset( -offsetLT.X, -offsetLT.Y);
      return pt;
    }

    //относительно 0,0 всей диаграммы
    Point AbsFromCanvas(Point pt)
    {
      pt.Offset(offsetLT.X, offsetLT.Y);
      return pt;
    }

    // connection path geometry
    private PathGeometry pathGeometry;
    public PathGeometry PathGeometry
    {
      get
      {
        return pathGeometry;
      }
      set
      {
        if (pathGeometry != value)
        {
          pathGeometry = value;
          TranslateToCanvas(ref pathGeometry);
          UpdateAnchorPosition();
          OnPropertyChanged("PathGeometry");
        }
      }
    }

    private void TranslateToCanvas(ref PathGeometry path)
    {
      double l = double.MaxValue;
      double t = double.MaxValue;
      double r = double.MinValue;
      double b = double.MinValue;
      path.DoForAllPointDPs(new DependencyPropertyCallback((DependencyObject obj, DependencyProperty dp) =>
      {
        if (dp.PropertyType == typeof(Point))
        {
          Point pt = (Point)obj.GetValue(dp);
          l = Math.Min(pt.X, l);
          t = Math.Min(pt.Y, t);
          r = Math.Max(pt.X, r);
          b = Math.Max(pt.Y, b);
        }
      }));
      Canvas.SetLeft(this, l);
      Canvas.SetTop(this, t);
      Width = r - l;
      Height = b - t;
      offsetLT = new Point(l, t);
      path.DoForAllPointDPs(new DependencyPropertyCallback((DependencyObject obj, DependencyProperty dp) =>
      {
        if (dp.PropertyType == typeof(Point))
        {
          Point pt = (Point)obj.GetValue(dp);
          pt = RelateToCanvas(pt);
          obj.SetValue(dp, pt);
        }
      }));
    }

    override public void MoveThumbDragged(double dx, double dy)
    {
      offsetLT.Offset(dx, dy);
      UpdateAnchorPosition();
    }

    override public void MoveThumbDraggedCompleted()
    {
      offsetLT.X = Canvas.GetLeft(this);
      offsetLT.Y = Canvas.GetTop(this);
      UpdateAnchorPosition();
    }

    private void UpdateAnchorPosition()
    {
      Point pathStartPoint, pathTangentAtStartPoint;
      Point pathEndPoint, pathTangentAtEndPoint;
      if (null == this.PathGeometry)
        return;
      this.PathGeometry.GetPointAtFractionLength(0, out pathStartPoint, out pathTangentAtStartPoint);
      this.PathGeometry.GetPointAtFractionLength(1, out pathEndPoint, out pathTangentAtEndPoint);

      this.AnchorPositionSource = AbsFromCanvas( pathStartPoint);
      this.AnchorPositionSink = AbsFromCanvas( pathEndPoint );
    }

    override protected void ShowAdorner()
    {
      if (this.lineAdorner == null)
      {
        DesignerCanvas designer = VisualTreeHelper.GetParent(this) as DesignerCanvas;

        AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(designer);
        if (adornerLayer != null)
        {
          this.lineAdorner = new LineAdorner(designer, this, offsetLT);
          adornerLayer.Add(this.lineAdorner);
        }
      }
      UpdateAnchorPosition();
      this.lineAdorner.Visibility = Visibility.Visible;
    }

    override protected void HideAdorner()
    {
      /*if (this.lineAdorner != null)
        this.lineAdorner.Visibility = Visibility.Collapsed;*/

      //проверить полное удаление
      AdornerLayer adornerLayer = VisualTreeHelper.GetParent(lineAdorner) as AdornerLayer;
      if (null != adornerLayer)
        adornerLayer.Remove(this.lineAdorner);
      lineAdorner = null;
    }

    protected override void OnMouseDown(System.Windows.Input.MouseButtonEventArgs e)
    {
      base.OnMouseDown(e);

      e.Handled = true;
      // usual selection business
      DesignerCanvas designer = VisualTreeHelper.GetParent(this) as DesignerCanvas;
      if (designer == null)
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

  }
}
