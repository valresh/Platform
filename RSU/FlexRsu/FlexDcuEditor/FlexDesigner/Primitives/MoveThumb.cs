using System;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;

namespace FlexDesigner.Primitives
{
  public class MoveThumb : Thumb
  {
    public MoveThumb()
    {
      DragDelta += new DragDeltaEventHandler(this.MoveThumb_DragDelta);
      DragCompleted += new DragCompletedEventHandler(MoveThumb_DragCompleted);
    }

    void MoveThumb_DragCompleted(object sender, DragCompletedEventArgs e)
    {
      e.Handled = true;
      Primitive designerItem = this.DataContext as Primitive;
      if (designerItem == null || !designerItem.IsSelected)
        return;
      designerItem.MoveThumbDraggedCompleted();
    }

    private void MoveThumb_DragDelta(object sender, DragDeltaEventArgs e)
    {
      e.Handled = true;
      Primitive designerItem = this.DataContext as Primitive;
      DesignerCanvas designer = VisualTreeHelper.GetParent(designerItem) as DesignerCanvas;
      if (designerItem == null || designer == null || !designerItem.IsSelected)
        return;
      double minLeft = double.MaxValue;
      double minTop = double.MaxValue;

      Primitive item = designerItem;

      double left = Canvas.GetLeft(item);
      double top = Canvas.GetTop(item);

      minLeft = double.IsNaN(left) ? 0 : Math.Min(left, minLeft);
      minTop = double.IsNaN(top) ? 0 : Math.Min(top, minTop);

      double deltaHorizontal = Math.Max(-minLeft, e.HorizontalChange);
      double deltaVertical = Math.Max(-minTop, e.VerticalChange);

      /*left = Canvas.GetLeft(item);
      top = Canvas.GetTop(item);

      if (double.IsNaN(left)) left = 0;
      if (double.IsNaN(top)) top = 0;*/

      Canvas.SetLeft(item, left + deltaHorizontal);
      Canvas.SetTop(item, top + deltaVertical);
      designerItem.MoveThumbDragged(deltaHorizontal, deltaVertical);
      designer.InvalidateMeasure();
    }
  }
}
