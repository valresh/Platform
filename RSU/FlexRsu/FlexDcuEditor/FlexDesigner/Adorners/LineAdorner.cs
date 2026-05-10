using System.Collections.Generic;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;

namespace FlexDesigner.Primitives
{
  public class LineAdorner : Adorner
  {
    private Point startPoint;
    private Point? endPoint;
    private PathGeometry pathGeometry;
    private DesignerCanvas designerCanvas;
    private Pen drawingPen;
    private LinePrimitive linePrimitive;
    private Thumb sourceDragThumb, sinkDragThumb;
    private Canvas adornerCanvas;

    private bool bNewDraw;//рисуется первый раз
    private bool bDragPointIsSink;//перемещаю конечную точку

    public LineAdorner(DesignerCanvas designer, Point startPoint) 
      : base(designer)
    {
      this.designerCanvas = designer;
      this.startPoint = designerCanvas.CorrectToGrid(startPoint);
      drawingPen = new Pen(Brushes.Black, 1);
      drawingPen.LineJoin = PenLineJoin.Round;
      this.Cursor = Cursors.Cross;
      bNewDraw = true;
      bDragPointIsSink = true;
    }

    public LineAdorner(DesignerCanvas designer, LinePrimitive lp, Point offsetLT )
      : base(designer)
    {
      bNewDraw = false;
      bool s = false;
      bool e = false;
      lp.PathGeometry.DoForAllPointDPs(new DependencyPropertyCallback((DependencyObject obj, DependencyProperty dp) =>
        {
          if (dp.PropertyType == typeof(Point))
          {
            Point pt = (Point)obj.GetValue(dp);
            if (!s)
            {
              this.startPoint = pt; s = true;
              startPoint.Offset(offsetLT.X, offsetLT.Y);
            }
            else if( !e )
            {
              this.endPoint = pt; e = true;
              this.endPoint.Value.Offset(offsetLT.X, offsetLT.Y);
            }
          }
        }));

      //this.Cursor = Cursors.Cross;
      this.designerCanvas = designer;
      adornerCanvas = new Canvas();
      this.visualChildren = new VisualCollection(this);
      this.visualChildren.Add(adornerCanvas);

      this.linePrimitive = lp;
      this.linePrimitive.PropertyChanged += new System.ComponentModel.PropertyChangedEventHandler(AnchorPositionChanged);

      InitializeDragThumbs();

      drawingPen = new Pen(Brushes.Black, 1);
      drawingPen.LineJoin = PenLineJoin.Round;

      base.Unloaded += new RoutedEventHandler(LineAdorner_Unloaded);
    }

    void AnchorPositionChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
    {
      if (e.PropertyName.Equals("AnchorPositionSource"))
      {
        Canvas.SetLeft(sourceDragThumb, linePrimitive.AnchorPositionSource.X);
        Canvas.SetTop(sourceDragThumb, linePrimitive.AnchorPositionSource.Y);
      }

      if (e.PropertyName.Equals("AnchorPositionSink"))
      {
        Canvas.SetLeft(sinkDragThumb, linePrimitive.AnchorPositionSink.X);
        Canvas.SetTop(sinkDragThumb, linePrimitive.AnchorPositionSink.Y);
      }
    }

    void LineAdorner_Unloaded(object sender, RoutedEventArgs e)
    {
      adornerCanvas.Children.Remove(sourceDragThumb);
      adornerCanvas.Children.Remove(sinkDragThumb);

      sourceDragThumb.DragDelta -= new DragDeltaEventHandler(thumbDragThumb_DragDelta);
      sourceDragThumb.DragStarted -= new DragStartedEventHandler(thumbDragThumb_DragStarted);
      sourceDragThumb.DragCompleted -= new DragCompletedEventHandler(thumbDragThumb_DragCompleted);

      sinkDragThumb.DragDelta -= new DragDeltaEventHandler(thumbDragThumb_DragDelta);
      sinkDragThumb.DragStarted -= new DragStartedEventHandler(thumbDragThumb_DragStarted);
      sinkDragThumb.DragCompleted -= new DragCompletedEventHandler(thumbDragThumb_DragCompleted);
    }

    private VisualCollection visualChildren;
    protected override int VisualChildrenCount
    {
      get
      {
        if (null == visualChildren)
          return 0;
        return this.visualChildren.Count;
      }
    }

    protected override Visual GetVisualChild(int index)
    {
      return this.visualChildren[index];
    }

    private void InitializeDragThumbs()
    {
      Style dragThumbStyle = linePrimitive.FindResource("LinePrimitiveAdornerThumbStyle") as Style;

      //source drag thumb
      sourceDragThumb = new Thumb();
      Canvas.SetLeft(sourceDragThumb, linePrimitive.AnchorPositionSource.X);
      Canvas.SetTop(sourceDragThumb, linePrimitive.AnchorPositionSource.Y);
      this.adornerCanvas.Children.Add(sourceDragThumb);
      if (dragThumbStyle != null)
        sourceDragThumb.Style = dragThumbStyle;

      sourceDragThumb.DragDelta += new DragDeltaEventHandler(thumbDragThumb_DragDelta);
      sourceDragThumb.DragStarted += new DragStartedEventHandler(thumbDragThumb_DragStarted);
      sourceDragThumb.DragCompleted += new DragCompletedEventHandler(thumbDragThumb_DragCompleted);

      // sink drag thumb
      sinkDragThumb = new Thumb();
      Canvas.SetLeft(sinkDragThumb, linePrimitive.AnchorPositionSink.X);
      Canvas.SetTop(sinkDragThumb, linePrimitive.AnchorPositionSink.Y);
      this.adornerCanvas.Children.Add(sinkDragThumb);
      if (dragThumbStyle != null)
        sinkDragThumb.Style = dragThumbStyle;

      sinkDragThumb.DragDelta += new DragDeltaEventHandler(thumbDragThumb_DragDelta);
      sinkDragThumb.DragStarted += new DragStartedEventHandler(thumbDragThumb_DragStarted);
      sinkDragThumb.DragCompleted += new DragCompletedEventHandler(thumbDragThumb_DragCompleted);
    }
    
    void thumbDragThumb_DragCompleted(object sender, DragCompletedEventArgs e)
    {
      if( null!=pathGeometry)
        linePrimitive.PathGeometry = pathGeometry;
      this.pathGeometry = null;
      //this.linePrimitive.StrokeDashArray = null;
      this.InvalidateVisual();
    }

    void thumbDragThumb_DragStarted(object sender, DragStartedEventArgs e)
    {
      this.pathGeometry = null;
      this.Cursor = Cursors.Cross;
      //this.linePrimitive.StrokeDashArray = new DoubleCollection(new double[] { 1, 2 });

      if (sender == sourceDragThumb)
        bDragPointIsSink = false;
      else if (sender == sinkDragThumb)
        bDragPointIsSink = true;
    }

    void thumbDragThumb_DragDelta(object sender, DragDeltaEventArgs e)
    {
      Point currentPosition = Mouse.GetPosition(this);
      this.pathGeometry = GetPathGeometry( currentPosition );
      this.InvalidateVisual();
    }

    protected override void OnMouseUp(MouseButtonEventArgs e)
    {
      if (!bNewDraw)
      {
        base.OnMouseUp(e);
        return;
      }

      if (endPoint.HasValue)
      {
        if (!startPoint.Equals(endPoint.Value))
        {
          LinePrimitive newLine = new LinePrimitive(startPoint, endPoint.Value);

          Canvas.SetZIndex(newLine, designerCanvas.Children.Count);
          this.designerCanvas.Children.Add(newLine);
        }
      }

      if (this.IsMouseCaptured) 
        this.ReleaseMouseCapture();

      AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(this.designerCanvas);
      if (adornerLayer != null)
      {
        adornerLayer.Remove(this);
      }
    }

    protected override void OnMouseMove(MouseEventArgs e)
    {
      if (!bNewDraw)
      {
        base.OnMouseMove(e);
        return;
      }
      if (e.LeftButton == MouseButtonState.Pressed)
      {
        if (!this.IsMouseCaptured) 
          this.CaptureMouse();

        endPoint = new Point?(e.GetPosition(this));
        this.pathGeometry = GetPathGeometry( endPoint.Value );
        this.InvalidateVisual();
      }
      else
      {
        if (this.IsMouseCaptured) 
          this.ReleaseMouseCapture();
      }
    }

    private PathGeometry GetPathGeometry(Point position)
    {
      position = null==designerCanvas ? position : designerCanvas.CorrectToGrid(position);
      PathGeometry geometry = new PathGeometry();
      PathFigure figure = new PathFigure();
      LineSegment segment;

      if (bDragPointIsSink)
      {
        figure.StartPoint = startPoint;
        endPoint = position;
        segment = new LineSegment(position, true);
      }
      else
      {
        startPoint = position;
        figure.StartPoint = position;
        segment = new LineSegment(endPoint.GetValueOrDefault(), true);
      }
      figure.Segments.Add(segment);
      geometry.Figures.Add(figure);

      return geometry;
    }

    protected override void OnRender(DrawingContext dc)
    {
      base.OnRender(dc);
      dc.DrawGeometry(null, drawingPen, this.pathGeometry);
      if( bNewDraw )
        dc.DrawRectangle(Brushes.Transparent, null, new Rect(RenderSize));
    }

    //обязательно надо изменять размер adornerCanvas, иначе thumb-ы не будут видны
    protected override Size ArrangeOverride(Size finalSize)
    {
      if( null!=adornerCanvas )
        adornerCanvas.Arrange(new Rect(0, 0, this.designerCanvas.ActualWidth, this.designerCanvas.ActualHeight));
      return finalSize;
    }


  }

}
