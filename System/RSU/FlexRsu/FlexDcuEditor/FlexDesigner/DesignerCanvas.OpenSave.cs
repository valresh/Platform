using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Threading;
using System.Xml;
using System.Xml.Linq;

namespace FlexDesigner
{
  partial class DesignerCanvas
  {
    readonly static string filterOpenSave = "FlexChemEditor Files (*.xml)|*.xml|All Files (*.*)|*.*";

    struct SConSigTT
    {
      public Connection con;
      public SignalToolTip sigTT;
    };

    public List<SSignalDescriptor> m_memoriesOfView = null;
    List<SConSigTT> ToolTips;
    DispatcherTimer mDt;

    XElement m_root = null;
    int m_nDiOnOpen = 0;
    private void Open_Executed(object sender, ExecutedRoutedEventArgs e)
    {
      string fileName = SelectFile();
      if (string.IsNullOrEmpty(fileName))
        return;
      OpenFile(fileName);
    }

    private void Save_Executed(object sender, ExecutedRoutedEventArgs e)
    {
      IEnumerable<DesignerItem> designerItems = this.Children.OfType<DesignerItem>();
      IEnumerable<Connection> connections = this.Children.OfType<Connection>();

      XElement designerItemsXML = SerializeDesignerItems(designerItems);
      XElement connectionsXML = SerializeConnections(connections);

      XElement root = new XElement("FbdLogic");
      root.Add(designerItemsXML);
      root.Add(connectionsXML);

      SaveFile(root);
    }

    private void SaveAs_Executed(object sender, ExecutedRoutedEventArgs e)
    {
      SaveFileDialog saveFile = new SaveFileDialog();
      saveFile.Filter = filterOpenSave;
      if (saveFile.ShowDialog() == true)
        SetCurFile(saveFile.FileName);
      else
        return;
      Save_Executed(sender, e);
    }

    private XElement SerializeDesignerItems(IEnumerable<DesignerItem> designerItems)
    {
      /*XElement serializedItems = new XElement("Items",
                                              from item in designerItems
                                                //let contentXaml = XamlWriter.Save(((DesignerItem)item).Content)
                                              select new XElement("Item",
                                                                    new XElement("Left", Canvas.GetLeft(item)),
                                                                    new XElement("Top", Canvas.GetTop(item)),
                                                                    new XElement("ID", item.ID),
                                                                    new XElement("VisibleName", item.VisibleName),
                                                                    new XElement("ParentID", item.ParentID),
                                                                    new XElement("Type", ((FrameworkElement)((DesignerItem)item).Content).Name )
                                                                    )
                                                                  )
                                                    );*/
      XElement serializedItems = new XElement("Items");
      foreach(DesignerItem item in designerItems )
      {
        DesignerItem di = (DesignerItem)item;
        FrameworkElement fe = (FrameworkElement)di.Content;
        string name = fe.Name;
        object o = fe.GetValue(DesignerItem.VendorProperty);
        if (null != o)
        {
          name = fe.Name.Insert(0, ":");
          name = name.Insert(0, o.ToString());
        }
        XElement i = new XElement("Item",
                                    new XElement("Left", Canvas.GetLeft(item)),
                                    new XElement("Top", Canvas.GetTop(item)),
                                    new XElement("ID", item.ID),
                                    new XElement("ParentID", item.ParentID),
                                    new XElement("Type", name) );
        if (!string.IsNullOrEmpty(item.ExecOrder) && !string.IsNullOrWhiteSpace(item.ExecOrder))
          i.Add( new XElement("ExecOrder", item.ExecOrder));
        if (!string.IsNullOrEmpty(item.VisibleName) && !string.IsNullOrWhiteSpace(item.VisibleName))
          i.Add( new XElement("VisibleName", item.VisibleName));
        HiddenPropertiesContainer hpc = DesignerItem.GetHiddenProperties((DependencyObject)item.Content);
        if ( null!=hpc )
        {
          XElement ihp = new XElement("HiddenProperties",
                                      from hp in hpc
                                      select new XElement(hp.Key, hp.Value ) );
          i.Add(ihp);
        }
        serializedItems.Add(i);
      }

      return serializedItems;
    }

    private XElement SerializeConnections(IEnumerable<Connection> connections)
    {
      var serializedConnections = new XElement("Connections",
                                               from connection in connections
                                               select new XElement("Connection",
                                                                    new XElement("SourceID", connection.Source.ParentDesignerItem.ID),
                                                                    new XElement("SinkID", connection.Sink.ParentDesignerItem.ID),
                                                                    new XElement("SourceConnectorName", connection.Source.Name),
                                                                    new XElement("SinkConnectorName", connection.Sink.Name)
                                                                  )
                                                      );

      return serializedConnections;
    }

    void SaveFile(XElement xElement)
    {
      if (string.IsNullOrEmpty(FullPath2CurFile))
      {
        SaveFileDialog saveFile = new SaveFileDialog();
        saveFile.Filter = filterOpenSave;
        if (saveFile.ShowDialog() == true)
          SetCurFile(saveFile.FileName);
      }

      if( !string.IsNullOrEmpty(FullPath2CurFile))
      {
        try
        {
                    XDocument encoded_utf8 = new XDocument(new XDeclaration("1.0", "utf-8", "yes"), xElement);
                    //encoded_utf8.Save(FullPath2CurFile);
                    using (var writer = new XmlTextWriter(FullPath2CurFile, new UTF8Encoding(false)))
                    {
			writer.Formatting = Formatting.Indented;	
                        encoded_utf8.Save(writer);
                    }
                    
                }
        catch (Exception ex)
        {
          MessageBox.Show(ex.StackTrace, ex.Message, MessageBoxButton.OK, MessageBoxImage.Error);
        }
      }
    }

    private string SelectFile()
    {
      OpenFileDialog openFile = new OpenFileDialog();
      openFile.Filter = filterOpenSave;

      if (openFile.ShowDialog() == true)
        return openFile.FileName;

      return null;
    }

    public void OpenFile(string fullPath)
    {
      Clear();

      //MessageBox.Show(fullPath);

      try
      {
        m_root = XElement.Load(fullPath);
      }
      catch (Exception e)
      {
        MessageBox.Show(e.StackTrace, e.Message, MessageBoxButton.OK, MessageBoxImage.Error);
      }

      if (m_root == null)
        return;

      ResetUnicalNumber();

      this.Children.Clear();
      this.SelectionService.ClearSelection();

      IEnumerable<XElement> itemsXML = m_root.Elements("Items").Elements("Item");
      m_nDiOnOpen = 0;
      foreach (XElement itemXML in itemsXML)
      {
        Guid id = new Guid(itemXML.Element("ID").Value);
        DesignerItem item = DeserializeDesignerItem(itemXML, id, 0, 0);
        this.Children.Add(item);
        item.Loaded += DesignerCanvas_Loaded;
        ++m_nDiOnOpen;
        SetConnectorDecoratorTemplate(item);
        if (string.IsNullOrEmpty(item.ExecOrder))
          continue;
        int curE = int.Parse(item.ExecOrder);
        if ( curE >= UnicalNumber)
          UnicalNumber = curE + 10;
      }

      SetCurFile(fullPath);
    }

    private void DesignerCanvas_Loaded(object sender, RoutedEventArgs e)
    {
      --m_nDiOnOpen;
      DesignerItem item = sender as DesignerItem;
      item.Loaded -= DesignerCanvas_Loaded;
      if( 0== m_nDiOnOpen )
      {
        IEnumerable<XElement> connectionsXML = m_root.Elements("Connections").Elements("Connection");
        foreach (XElement connectionXML in connectionsXML)
        {
          Guid sourceID = new Guid(connectionXML.Element("SourceID").Value);
          Guid sinkID = new Guid(connectionXML.Element("SinkID").Value);

          String sourceConnectorName = connectionXML.Element("SourceConnectorName").Value;
          String sinkConnectorName = connectionXML.Element("SinkConnectorName").Value;

          Connector sourceConnector = GetConnector(sourceID, sourceConnectorName);
          Connector sinkConnector = GetConnector(sinkID, sinkConnectorName);

          if (null != sourceConnector && null != sinkConnector)
          {
            if (Connection.TestConnection( ref sourceConnector, ref sinkConnector))
            {
              Connection connection = new Connection(sourceConnector, sinkConnector);
              if (null != m_memoriesOfView)
              {
                foreach(SSignalDescriptor sig in m_memoriesOfView )
                {
                  if ( !sig.src.objID.Equals(sourceID) || !sig.dst.objID.Equals(sinkID))
                    continue;
                  if ( !string.Equals(sig.src.szPntName, sourceConnectorName) || !string.Equals(sig.dst.szPntName, sinkConnectorName) )
                    continue;
                  ToolTip t = new ToolTip();

                  SConSigTT cs = new SConSigTT();
                  cs.sigTT = new SignalToolTip();
                  cs.sigTT.srcMemory = sig.src;
                  cs.sigTT.destMemory = sig.dst;

                  cs.sigTT.Refresh();
                  connection.IsDigital = cs.sigTT.IsDigital();
                  cs.con = connection;

                  if (null == ToolTips)
                    ToolTips = new List<SConSigTT>();
                  ToolTips.Add(cs);

                  t.Content = cs.sigTT;
                  connection.ToolTip = t;
                  connection.IsHitTestVisible = true;
                }
                if (null != ToolTips)
                {
                  mDt = new DispatcherTimer();
                  mDt.Tick += new EventHandler(DispatcherTimer_Tick);
                  mDt.Interval = new TimeSpan(0, 0, 1);
                  mDt.Start();
                }
              }
              this.Children.Add(connection);
            }
          }
        }
        m_root = null;
        m_memoriesOfView = null;
      }

    }

    void DispatcherTimer_Tick(object o, EventArgs e)
    {
      DispatcherTimer dt = o as DispatcherTimer;
      if (dt == null)
        return;

      lock (dt)
      {
        dt.Stop();

        foreach (SConSigTT el in ToolTips)
        {
          el.sigTT.Refresh();
          if (!el.sigTT.IsDigital())
            continue;
          Int32 src = el.sigTT.GetDigValue(el.sigTT.srcMemory);
          Int32 dst = el.sigTT.GetDigValue(el.sigTT.destMemory);
          if (src == dst)
            el.con.DigValue = src;
          else
            el.con.DigValue = Int32.MinValue;
        }

        dt.Start();
      }
    }
    private Connector GetConnector(Guid itemID, String connectorName)
    {
      DesignerItem designerItem = (from item in this.Children.OfType<DesignerItem>()
                                   where item.ID == itemID
                                   select item).FirstOrDefault();
      if (null == designerItem)
        return null;

      Control connectorDecorator = designerItem.Template.FindName("PART_ConnectorDecorator", designerItem) as Control;
      connectorDecorator.ApplyTemplate();

      return connectorDecorator.Template.FindName(connectorName, connectorDecorator) as Connector;
    }

    private DesignerItem DeserializeDesignerItem(XElement itemXML, Guid id, double OffsetX, double OffsetY)
    {
      DesignerItem item = new DesignerItem(id);
      item.ParentID = new Guid(itemXML.Element("ParentID").Value);
      if( null!=itemXML.Element("VisibleName") )
        item.VisibleName = itemXML.Element("VisibleName").Value;
      if( null!=itemXML.Element("ExecOrder") )
        item.ExecOrder = itemXML.Element("ExecOrder").Value;
      Canvas.SetLeft(item, Double.Parse(itemXML.Element("Left").Value, CultureInfo.InvariantCulture) + OffsetX);
      Canvas.SetTop(item, Double.Parse(itemXML.Element("Top").Value, CultureInfo.InvariantCulture) + OffsetY);
      string Type = itemXML.Element("Type").Value;

      string xamlString =_PrimitiveElements[Type];

      FrameworkElement content = XamlReader.Load(XmlReader.Create(new StringReader(xamlString))) as FrameworkElement;
      if (content == null)
        return item;

      if (!DesignerItem.GetShowExecOrder(content))
        item.ExecOrder = string.Empty;
      item.Content = content;

      IEnumerable<XNode> hps = itemXML.Elements("HiddenProperties").Nodes();
      if ( null!=hps )
      {
        HiddenPropertiesContainer hpcRest = null;
        foreach (XNode v in hps)
        {
          XElement x = v as XElement;
          if( null==hpcRest )
            hpcRest = new HiddenPropertiesContainer();
          hpcRest.Add( new HiddenProperty(x.Name.LocalName, x.Value) );
        }
        HiddenPropertiesContainer hpCur = DesignerItem.GetHiddenProperties(content);
        if (null != hpcRest)
        {
          if (null == hpCur)
          {
            //DesignerItem.SetHiddenProperties(content, hpcRest);
          }
          else
          {
            foreach (HiddenProperty prop in hpcRest)
            {
              foreach (HiddenProperty cur in hpCur)
              {
                if (cur.Key == prop.Key)
                {
                  cur.Owner = content;
                  cur.Value = null;
                  cur.Value = prop.Value;
                  break;
                }
              }
            }
          }
        }
      }

      return item;
    }

  }
}
