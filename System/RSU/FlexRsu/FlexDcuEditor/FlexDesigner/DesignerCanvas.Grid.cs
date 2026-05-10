using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Linq;

namespace FlexDesigner
{
  public partial class DesignerCanvas
  {
    private ICommand _GridVisibleCommand;

    public static readonly DependencyProperty IsGridVisibleProperty = DependencyProperty.Register("IsGridVisible", typeof(bool), typeof(DesignerCanvas), new FrameworkPropertyMetadata(true, IsGridVisibleChanged));

    private void SnapToGrid(UIElement masterItem, List<UIElement> itemsToMove)
    {
      Size gridSize = GetSnapGridSize();

      int gridSizeX = Convert.ToInt32(gridSize.Width);
      int gridSizeY = Convert.ToInt32(gridSize.Height);

      double xSnap = Canvas.GetLeft(masterItem) % gridSizeX;
      double ySnap = Canvas.GetTop(masterItem) % gridSizeY;

      if (double.IsNaN(xSnap) || double.IsNaN(ySnap))
        return;
      // If it's less than half the grid size, snap left/up
      // (by subtracting the remainder),
      // otherwise move it the remaining distance of the grid size right/down
      // (by adding the remaining distance to the next grid point).
      if (xSnap <= gridSizeX / 2.0)
        xSnap *= -1;
      else
        xSnap = gridSizeX - xSnap;
      if (ySnap <= gridSizeY / 2.0)
        ySnap *= -1;
      else
        ySnap = gridSizeY - ySnap;

      if (itemsToMove.Contains(masterItem) == false)
        itemsToMove.Add(masterItem);

      foreach (UIElement itemToMove in itemsToMove)
      {
        double xSnapPosition = xSnap + Canvas.GetLeft(itemToMove);
        double ySnapPosition = ySnap + Canvas.GetTop(itemToMove);

        Canvas.SetLeft(itemToMove, xSnapPosition);
        Canvas.SetTop(itemToMove, ySnapPosition);
      }
    }

    public Point CorrectToGrid(Point pt)
    {
      if (!IsGridVisible)
        return pt;

      Size gridSize = GetSnapGridSize();

      int gridSizeX = Convert.ToInt32(gridSize.Width);
      int gridSizeY = Convert.ToInt32(gridSize.Height);

      double xSnap = pt.X % gridSizeX;
      double ySnap = pt.Y % gridSizeY;

      if (xSnap <= gridSizeX / 2.0)
        xSnap *= -1;
      else
        xSnap = gridSizeX - xSnap;
      if (ySnap <= gridSizeY / 2.0)
        ySnap *= -1;
      else
        ySnap = gridSizeY - ySnap;

      pt.X = xSnap + pt.X;
      pt.Y = ySnap + pt.Y;

      return pt;
    }

    private Size GetSnapGridSize()
    {
      System.Windows.Media.VisualBrush pointBrush = this.Background as System.Windows.Media.VisualBrush;
      return new Size(pointBrush.Viewbox.Width, pointBrush.Viewbox.Height);
    }

    public ICommand GridVisible
    {
      get
      {
        if (null == _GridVisibleCommand)
        {
          _GridVisibleCommand = new RelayCommand(param => this.GridVisible_Executed(), null);
        }
        return _GridVisibleCommand;
      }
    }

    private void GridVisible_Executed()
    {
    }

    public bool IsGridVisible
    {
      get
      {
        return (bool)GetValue(IsGridVisibleProperty);
      }
      set
      {
        SetValue(IsGridVisibleProperty, value);
      }
    }

    private static void IsGridVisibleChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    {
      DesignerCanvas dc = d as DesignerCanvas;
    }

    protected override void OnPreviewMouseLeftButtonUp(MouseButtonEventArgs e)
    {
      base.OnPreviewMouseLeftButtonUp(e);

      if (!IsGridVisible)
        return;

      if (e.Source as UIElement != null)
      {
        List<UIElement> itemsToMove = new List<UIElement>();
        foreach (ISelectable selectable in SelectionService.CurrentSelection.Where(t => t as UIElement != null))
        {
          itemsToMove.Add(selectable as UIElement);
        }

        SnapToGrid((UIElement)e.Source, itemsToMove);
      }
    }

  }

}
