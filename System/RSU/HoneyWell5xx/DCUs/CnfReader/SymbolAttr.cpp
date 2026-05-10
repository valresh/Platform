#include "SymbolAttr.h"
#include <TinyXmlHelper.h>

using namespace std;
using namespace TINYXMLHELPER;

void KSymbolAttr::Parse( tinyxml2::XMLElementPtr &root )
{
  tHandlersCont_ cont;
  cont["ParamName"] = &KSymbolAttr::OnParamName;
  cont["AttrType"] = &KSymbolAttr::OnAttrType;
  cont["AttrOrder"] = &KSymbolAttr::OnAttrOrder;
  cont["AttrViewValue"] = &KSymbolAttr::OnAttrViewValue;
  cont["AttrViewLabel"] = &KSymbolAttr::OnAttrViewLabel;
  cont["XCoord"] = &KSymbolAttr::OnXCoord;
  cont["YCoord"] = &KSymbolAttr::OnYCoord;
  KTinyXmlReader<KSymbolAttr>::Read( root, cont );
}

void KSymbolAttr::OnParamName( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, ParamName );
}

void KSymbolAttr::OnAttrType( tinyxml2::XMLElementPtr &root )
{
  string at;
  GetXMLData( root, at );
  AttrType = ns_AttrType::StringToeAttrType( at.c_str() );
  ASS( AttrType < ns_AttrType::MAX_NUMBER_OF_eAttrType );
}

void KSymbolAttr::OnAttrOrder( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, AttrOrder );
}

void KSymbolAttr::OnAttrViewValue( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, AttrViewValue );
}

void KSymbolAttr::OnAttrViewLabel( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, AttrViewLabel );
}

void KSymbolAttr::OnXCoord( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, coord.cx );
}

void KSymbolAttr::OnYCoord( tinyxml2::XMLElementPtr &root )
{
  GetXMLData( root, coord.cy );
}
