#pragma once
#include "ISymbolAttr.h"
#include <TinyXmlReader.h>

class KSymbolAttr : protected KTinyXmlReader<KSymbolAttr>
                  , public TIntrusivePtrImpl<ISymbolAttr>
{
public:
protected:
  //ISymbolAttr
  virtual void Parse( tinyxml2::XMLElementPtr &root );
protected:
  void OnParamName( tinyxml2::XMLElementPtr &root );
  void OnAttrType( tinyxml2::XMLElementPtr &root );
  void OnAttrOrder( tinyxml2::XMLElementPtr &root );
  void OnAttrViewValue( tinyxml2::XMLElementPtr &root );
  void OnAttrViewLabel( tinyxml2::XMLElementPtr &root );
  void OnXCoord( tinyxml2::XMLElementPtr &root );
  void OnYCoord( tinyxml2::XMLElementPtr &root );
};