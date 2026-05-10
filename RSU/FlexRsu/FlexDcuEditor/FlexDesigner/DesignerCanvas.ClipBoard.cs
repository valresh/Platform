using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Xml.Linq;

namespace FlexDesigner
{
  partial class DesignerCanvas
  {
    private void Copy_Executed(object sender, ExecutedRoutedEventArgs e)
    {
      IEnumerable<DesignerItem> selectedDesignerItems = this.SelectionService.CurrentSelection.OfType<DesignerItem>();
      List<Connection> selectedConnections = this.SelectionService.CurrentSelection.OfType<Connection>().ToList();

      XElement designerItemsXML = SerializeDesignerItems(selectedDesignerItems);
      XElement connectionsXML = SerializeConnections(selectedConnections);

      XElement root = new XElement("Root");
      root.Add(designerItemsXML);
      root.Add(connectionsXML);

      root.Add(new XAttribute("OffsetX", 10));
      root.Add(new XAttribute("OffsetY", 10));

      Clipboard.Clear();
      Clipboard.SetData(DataFormats.Xaml, root);
    }

    private void Copy_Enabled(object sender, CanExecuteRoutedEventArgs e)
    {
      e.CanExecute = this.SelectionService.CurrentSelection.Count() > 0;
    }

    private XElement LoadSerializedDataFromClipBoard()
    {
      if (Clipboard.ContainsData(DataFormats.Xaml))
      {
        String clipboardData = Clipboard.GetData(DataFormats.Xaml) as String;

        if (String.IsNullOrEmpty(clipboardData))
          return null;
        try
        {
          return XElement.Load(new StringReader(clipboardData));
        }
        catch (Exception e)
        {
          MessageBox.Show(e.StackTrace, e.Message, MessageBoxButton.OK, MessageBoxImage.Error);
        }
      }
      return null;
    }

    private void Paste_Executed(object sender, ExecutedRoutedEventArgs e)
    {
      XElement root = LoadSerializedDataFromClipBoard();

      if (root == null)
        return;
      
      // create DesignerItems
      Dictionary<Guid, Guid> mappingOldToNewIDs = new Dictionary<Guid, Guid>();
      List<ISelectable> newItems = new List<ISelectable>();
      IEnumerable<XElement> itemsXML = root.Elements("Items").Elements("Item");

      double offsetX = Double.Parse(root.Attribute("OffsetX").Value, CultureInfo.InvariantCulture);
      double offsetY = Double.Parse(root.Attribute("OffsetY").Value, CultureInfo.InvariantCulture);

      foreach (XElement itemXML in itemsXML)
      {
        Guid oldID = new Guid(itemXML.Element("ID").Value);
        Guid newID = Guid.NewGuid();
        mappingOldToNewIDs.Add(oldID, newID);
        DesignerItem item = DeserializeDesignerItem(itemXML, newID, offsetX, offsetY);

        if (!string.IsNullOrEmpty(item.ExecOrder))
        {
          int curE = int.Parse(item.ExecOrder);
          while (!TestForUnicalNumber(curE))
          {
            curE += 10;
          }
          item.ExecOrder = curE.ToString();
          if (curE > UnicalNumber)
            UnicalNumber = curE;
        }

        this.Children.Add(item);
        item.Loaded += DesignerCanvas_Loaded;
        ++m_nDiOnOpen;
        SetConnectorDecoratorTemplate(item);
        newItems.Add(item);
      }

      // update group hierarchy
      SelectionService.ClearSelection();
      foreach (DesignerItem el in newItems)
      {
        if (el.ParentID != Guid.Empty)
          el.ParentID = mappingOldToNewIDs[el.ParentID];
      }


      foreach (DesignerItem item in newItems)
      {
        if (item.ParentID == Guid.Empty)
        {
          SelectionService.AddToSelection(item);
        }
      }

      m_root = root;
      // create Connections
      IEnumerable<XElement> connectionsXML = root.Elements("Connections").Elements("Connection");
      foreach (XElement connectionXML in connectionsXML)
      {
        Guid oldSourceID = new Guid(connectionXML.Element("SourceID").Value);
        Guid oldSinkID = new Guid(connectionXML.Element("SinkID").Value);

        if (mappingOldToNewIDs.ContainsKey(oldSourceID) )
        {
          Guid newSourceID = mappingOldToNewIDs[oldSourceID];
          connectionXML.Element("SourceID").Value = newSourceID.ToString();
        }

        if (mappingOldToNewIDs.ContainsKey(oldSinkID))
        {
          Guid newSinkID = mappingOldToNewIDs[oldSinkID];
          connectionXML.Element("SinkID").Value = newSinkID.ToString();
        }
      }

      // update paste offset
      root.Attribute("OffsetX").Value = (offsetX + 10).ToString();
      root.Attribute("OffsetY").Value = (offsetY + 10).ToString();
    }

    private void Paste_Enabled(object sender, CanExecuteRoutedEventArgs e)
    {
      e.CanExecute = Clipboard.ContainsData(DataFormats.Xaml);
    }
  }
}
