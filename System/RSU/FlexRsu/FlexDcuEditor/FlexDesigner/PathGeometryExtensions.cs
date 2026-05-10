using System;
using System.Windows;
using System.Windows.Media;
using System.Reflection;

namespace FlexDesigner
{
  public delegate void DependencyPropertyCallback(DependencyObject obj, DependencyProperty dp);

  static class PathGeometryExtensions
  {
    public static void DoForAllPointDPs(this PathGeometry path, DependencyPropertyCallback callback)
    {
      foreach (var f in path.Figures)
      {
        var t1 = f.GetType();
        var fii = t1.GetField("StartPointProperty");
        var oo = fii.GetValue(null);
        callback(f, oo as DependencyProperty);
        foreach (var s in f.Segments)
        {
          foreach (FieldInfo fi in s.GetType().GetFields())
          {
            if (fi.FieldType == typeof(DependencyProperty))
            {
              var dp = fi.GetValue(null) as DependencyProperty;
              if (dp.PropertyType == typeof(Point))
              {
                callback(s, dp);
              }
            }
            else
            {
              throw new Exception("Не обрабатываемый тип точки");
            }
          }
        }
      }
    }

  }

}
