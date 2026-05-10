#pragma once
#include "XmlMarker.h"
#include "IXmlNode.h"

#pragma warning( push )
#pragma warning( disable : 4275)
#pragma warning( disable : 4251)

class KXmlNode : public TIntrusivePtrImpl<IXmlNode>
{
public:
  KXmlNode( LPCSTR pszNodeName )
  {
    m_name = pszNodeName;
  }
  //IXmlNode
  virtual tXmlNodePtr_ AddChild( tXmlNodePtr_ pChild )
  {
    m_children.push_back( pChild );
    tXmlNodePtr_ b = m_children.back();
    return b;
  }
  virtual void AddAttr( LPCSTR pszName, LPCSTR pszValue, LPCSTR sep = " " )
  {
    if( !pszValue || !*pszValue )
      return;
    std::string &val = GetAttr( GetName(pszName) );
    if( val.empty() )
    {
      val.assign( pszValue );
    }
    else
    {
      if( sep )
        val.append( sep );
      else
        val.clear();
      val.append( pszValue );
    }
  }
  virtual LPCSTR GetNodeType()
  {
    return m_name.c_str();
  }
  virtual tChildrenIterator_ EnumChildren()
  {
    tChildrenIterator_ o( m_children );
    return o;
  }
  virtual tAttrsIterator_ EnumAttrs()
  {
    tAttrsIterator_ o( m_attrs );
    return o;
  }
  virtual LPCSTR GetData()
  {
    if( m_data.empty() )
      return NULL;
    return m_data.c_str();
  }

  //
  void AddAttrE( LPCSTR pszName, size_t value, LPCSTR sep = " " )
  {
    char szValue[16];
    sprintf_s( szValue, "%d", value );
    AddAttr( pszName, szValue, sep );
  }
  void SetData( LPCSTR pData )
  {
    m_data = pData;
  }

protected:
  std::string& GetAttr( LPCSTR pszName )
  {
    pszName = GetName(pszName);
    for( tAttrCont_::iterator it = m_attrs.begin(), end( m_attrs.end()); it!=end; ++it )
    {
      if( !it->szName.compare(pszName) )
        return it->szValue;
    }

    SAttribute attr;
    attr.szName.assign( pszName );
    m_attrs.push_back( attr );

    return m_attrs.back().szValue;
  }
  LPCSTR GetName( LPCSTR pszName )
  {
    return pszName;
  }
protected:
  tChildrenCont_ m_children;
  tAttrCont_ m_attrs;
  std::string m_name;
  std::string m_data;
};

#pragma warning( pop )
