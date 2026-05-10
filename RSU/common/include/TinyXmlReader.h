#pragma once
#include <macros/AutoRestore.h>
#include <TinyXmlHelper.h>
#include <map>
#include <string>
#include <rsuErr.h>

/*
пример вызова
typedef void (KPgmBlockBase::*pfHandler)( MSXML::IXMLDOMElementPtr&);
typedef std::map<std::string, pfHandler> tHandlersCont_;
static tHandlersCont_ cont;
if( cont.empty() )
{
cont["Section"] = &KPgmBlockBase::OnSection;
}
EnumerateChild<KPgmBlockBase, pfHandler>( this, n, cont );
*/
template<class owner, typename H>
void EnumerateChild( owner *This, tinyxml2::XMLElementPtr &root, std::map<std::string, H> &cont, bool bControlNames = true )
{
  using namespace tinyxml2;
  typedef std::map<std::string, H> tHandlersCont_;

  for( XMLElementPtr item = root->FirstChildElement(); item != NULL; item = item->NextSiblingElement() )
  {
    LPCSTR element_name = item->Name();
    auto it = cont.find( element_name );
    if( bControlNames )
    {
      ASS( cont.end()!=it );
    }
    if( cont.end()==it )
      continue;
    (This->*(it->second))(item);
  }
}
/*
template<class owner, typename H>
void ForEachAttr( owner *This, MSXML::IXMLDOMElementPtr &root, H func )
{
  MSXML::IXMLDOMNamedNodeMapPtr attrs = root->Getattributes();
  for( long i = 0; i < attrs->length; ++i )
  {
    MSXML::IXMLDOMNodePtr item(attrs->item[i]);
    if( item == NULL )
      continue;
    _bstr_t element_name( item->GetnodeName() );
    _variant_t var = item->GetnodeValue();
    if( var.vt == VT_BSTR )
      (This->*func)(element_name, XMLHELPER::variant_cast<std::string>(var).c_str() );
    else
    {
      ASS(0);
    }
  }
}

template<class owner, typename H>
void ForEachNode( owner *This, MSXML::IXMLDOMElementPtr &root, H func )
{
  MSXML::IXMLDOMNodeListPtr spNodes(root->childNodes);
  for( long i = 0; i < spNodes->length; ++i ) 
  {
    MSXML::IXMLDOMElementPtr item(spNodes->item[i]);
    if( item == NULL )
      continue;
    MSXML::IXMLDOMNodePtr ch = item->GetfirstChild();
    if( NULL==ch )
      continue;
    if( wcscmp( L"#text", ch->nodeName) )
      continue;

    _bstr_t element_name(item->tagName);
    _variant_t var = ch->GetnodeValue();
    if( var.vt == VT_BSTR )
      (This->*func)(element_name, XMLHELPER::variant_cast<std::string>(var).c_str() );
    else
    {
      ASS(0);
    }
  }
}*/

template<typename DERIVED>
class KTinyXmlReader
{
protected:
  typedef void (DERIVED::*pfHandlerNode)( LPCSTR pszName, LPCSTR pszText );

  typedef void (DERIVED::*pfHandler)( tinyxml2::XMLElementPtr &item );
  typedef std::map<std::string, pfHandler> tHandlersCont_;
public:
protected:
  void Read( tinyxml2::XMLElementPtr &root, tHandlersCont_ &cont, bool bControlNames = true )
  {
    using namespace tinyxml2;
    for( XMLElementPtr item = root->FirstChildElement(); item != NULL; item = item->NextSiblingElement() )
    {
      LPCSTR element_name = item->Name();
      auto it = cont.find( element_name );
      if( bControlNames )
      {
        ASS( cont.end()!=it );
      }
      if( cont.end()==it )
        continue;
      (((DERIVED*)this)->*(it->second))(item);
    }
  }

  /*void ForEachNode( MSXML::IXMLDOMElementPtr &root, pfHandlerNode func )
  {
    MSXML::IXMLDOMNodeListPtr spNodes(root->childNodes);
    for( long i = 0; i < spNodes->length; ++i ) 
    {
      MSXML::IXMLDOMElementPtr item(spNodes->item[i]);
      if( item == NULL )
        continue;
      MSXML::IXMLDOMNodePtr ch = item->GetfirstChild();
      if( NULL==ch )
        continue;
      if( wcscmp( L"#text", ch->nodeName) )
        continue;

      _bstr_t element_name(item->tagName);
      _variant_t var = ch->GetnodeValue();
      if( var.vt == VT_BSTR )
        (((DERIVED*)this)->*func)(element_name, XMLHELPER::variant_cast<std::string>(var).c_str() );
      else
      {
        ASS(0);
      }
    }
  }*/
};

inline tinyxml2::XMLElementPtr RecursiveFirstChildElement( tinyxml2::XMLElementPtr root, LPSTR pszQuery, const char sep = '/')
{
  LPSTR pEnd = pszQuery;
  for( pEnd; *pEnd; ++pEnd )
  {
    if( sep==(*pEnd) )
      break;
  }

  tinyxml2::XMLElementPtr firstChild;

  if( *pEnd )
  {
    TAutoRestore<char> ar(*pEnd);
    *pEnd = 0;
    firstChild = root->FirstChildElement( pszQuery );
  }
  else
    return root->FirstChildElement( pszQuery );
  if( firstChild && *pEnd )
    return RecursiveFirstChildElement( firstChild, pEnd+1, sep );
  return firstChild;
}
