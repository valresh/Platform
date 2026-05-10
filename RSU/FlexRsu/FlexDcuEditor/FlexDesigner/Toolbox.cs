using System.Windows;
using System.Windows.Controls;

namespace FlexDesigner
{
  public class Toolbox : ItemsControl
  {
    private Size itemSize = new Size(50, 50);

    public Toolbox()
    {
    }

    public Size ItemSize
    {
      get { return itemSize; }
      set { itemSize = value; }
    }

    public string Header
    {
      get;
      set;
    }

    protected override DependencyObject GetContainerForItemOverride()
    {
      return new ToolboxItem();
    }

    protected override bool IsItemItsOwnContainerOverride(object item)
    {
      return (item is ToolboxItem);
    }
  }
}
