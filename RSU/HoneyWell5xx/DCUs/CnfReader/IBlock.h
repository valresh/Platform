#pragma once
#include <TINYXML/tinyxml2.h>
#include <macros/IntrusivePtr.h>
#include <vector>
#include <string>
#include "Connection.h"
#include "ISymbolAttr.h"
#include "Coord.h"

class IBlock;

typedef TIntrusivePtr<IBlock> tBlockPtr_;

typedef std::vector<tBlockPtr_> tBlocksCont_;
typedef std::vector<SConnection> tConnectionsCont_;
typedef std::vector<SConnectionFromDependency> tDepConnectionsCont_;

struct SPair
{
  SPair()
  {}
  SPair( std::string &f, std::string &s, std::string &d ): first(f), second(s), def(d)
  {}
  std::string first;
  std::string second;
  std::string def;
  bool operator==( const SPair &thr )
  {
    return first == thr.first;
  }
};
typedef SPair tParam_;
typedef std::vector<tParam_> tParamsCont_;

class IBlock : public ref_counted
{
public:
  enum eMaskForRead
  {
    emrMinimum      = 0,
    emrSymbolAttrs  = 0x1,
    emrParameters   = 0x2,
    emrConVertexs   = 0x4,
  };
  enum eInflatesIO
  {
    Left = -70,
    Top = -70,
    TopDEVCTL = -96,
    Right = -70,
    Bottom = -70,
    BottomDEVCTL = -96,
  };
  typedef std::map<ns_AttrType::eAttrType, tSymbolAttrsCont_> tSortedAttrs_;
  typedef std::vector<std::string> tVectStringCont_;
public:
  std::string BlockName, EntityName, AssignedTo, Container, HandlerName;
  std::string ClassName, tnNamespace, tnClass;//разделенный TemplateName
  std::string szXmlFile;
  tBlocksCont_ m_EmbBlocks;
  tConnectionsCont_ Connections;
  tDepConnectionsCont_ DepConnections;
  tParamsCont_ Parameters;
  tSortedAttrs_ m_Attrs;
  int ORDERINCM;
  tBlockPtr_ m_ParentBlock;
  tVectStringCont_ m_NonCeePointNames;
public:
  IBlock() : ORDERINCM( 0 )
  {}
  virtual void Parse( tinyxml2::XMLElementPtr &root, DWORD mskRead ) = 0;
  virtual void AddChildBlock( tBlockPtr_ &bl ) = 0;
  virtual SRECT GetCoord() = 0;//границы блока
  virtual bool IsCoordSame() = 0;//проверка на различие координат
  virtual SRECT GetBlkBodyCoord() = 0;
  virtual tSymbolAttrPtr_ GetSymbAttr( ns_AttrType::eAttrType attr, size_t index ) = 0;
};
