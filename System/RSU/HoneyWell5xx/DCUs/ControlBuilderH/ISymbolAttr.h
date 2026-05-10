#pragma once
#include <TINYXML/tinyxml2.h>
#include <macros/IntrusivePtr.h>
#include <string>
#include <macros/en2str.h>
#include <vector>

namespace ns_AttrType
{
  DECLARE_ENUM( eAttrType,
                noType,
                ConfigBlockSymbol,
                ConfigDebug,
                MonitorBlockSymbol,
                MonitorDebug,
                InputTop,
                InputLeft,
                OutputBottom,
                OutputRight,
                ViewPinLabels,
                ViewPinValues,
                ViewParameters,
                BlockForeColor,
                ChartSizeWidth,
                ChartSizeHeight,
                ViewBlockName,
                ViewTypeName,
                ScoShowDetailsBox,
                ScoWhatToShow);
}

struct ISymbolAttr : public ref_counted
{
  ISymbolAttr() : AttrOrder(0), AttrViewLabel(false), AttrViewValue(false), AttrType(ns_AttrType::noType)
  {
    coord.cx = coord.cy = 0;
  }
  std::string ParamName;
  ns_AttrType::eAttrType AttrType;
  int AttrOrder;
  bool AttrViewValue;
  bool AttrViewLabel;
  SIZE coord;
  virtual void Parse( tinyxml2::XMLElementPtr &root ) = 0;
};

typedef TIntrusivePtr<ISymbolAttr> tSymbolAttrPtr_;

typedef std::vector<tSymbolAttrPtr_> tSymbolAttrsCont_;
