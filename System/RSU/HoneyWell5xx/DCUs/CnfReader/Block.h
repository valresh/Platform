#pragma once
#include <TinyXmlReader.h>
#include "Coord.h"
#include "IBlock.h"
#include "ISymbolAttr.h"
#include "GuidConverter.h"

/*
#include <QtCore/QtGlobal>
#ifdef EMF_DLL
#define IN_DLL Q_DECLQ_DECL_EXPORTORT
#else
#define IN_DLL Q_DECLQ_DECL_IMPORTORT
#endif

*/

class KBlock 
  : protected KTinyXmlReader<KBlock>
  , public TIntrusivePtrImpl<IBlock>
{
  KCoord Coord;
  std::string TemplateName;
  int BlockId;
  GUID BlockGUID;
  DWORD m_maskRead;
  SRECT m_InflateRect;
public:
  KBlock( tBlockPtr_ parent );
  ~KBlock(void);
protected:
  //IBlock
  virtual void Parse( tinyxml2::XMLElementPtr &root, DWORD mskRead );
  virtual void AddChildBlock( tBlockPtr_ &bl );
  virtual SRECT GetCoord();
  virtual bool IsCoordSame();
  virtual SRECT GetBlkBodyCoord();
  virtual tSymbolAttrPtr_ GetSymbAttr( ns_AttrType::eAttrType attr, size_t index );
protected:
  void OnSkip( tinyxml2::XMLElementPtr &root )
  {
#ifdef _DEBUG
    LPCSTR nType = root->Value();
#endif
  }

  void OnParameters( tinyxml2::XMLElementPtr &root );
  void OnParameter( tinyxml2::XMLElementPtr &root );

  void OnSymbolAttrs( tinyxml2::XMLElementPtr &root );
  void OnSymbolAttr( tinyxml2::XMLElementPtr &root );

  void OnConnections( tinyxml2::XMLElementPtr &root );
  void OnConnection( tinyxml2::XMLElementPtr &root );
  void OnConInputEnd( tinyxml2::XMLElementPtr &root );
  void OnConOutputEnd( tinyxml2::XMLElementPtr &root );
  void OnConConnectionForm( tinyxml2::XMLElementPtr &root );
  void OnConGraphicalForm( tinyxml2::XMLElementPtr &root );
  void OnConVertex( tinyxml2::XMLElementPtr &root );
  void OnNonCeePoints( tinyxml2::XMLElementPtr &root );
  void OnNonCeePointName( tinyxml2::XMLElementPtr &root );

  void OnEmbBlocks( tinyxml2::XMLElementPtr &root );

  void OnBlockDef( tinyxml2::XMLElementPtr &root );
  void OnBlockName( tinyxml2::XMLElementPtr &root );
  void OnEntityName( tinyxml2::XMLElementPtr &root );
  void OnBlockId( tinyxml2::XMLElementPtr &root );
  void OnBlockGUID( tinyxml2::XMLElementPtr &root );
  void OnCoord( tinyxml2::XMLElementPtr &root );
  void OnAssignedTo( tinyxml2::XMLElementPtr &root );
  void OnHandlerName( tinyxml2::XMLElementPtr &root );
  void OnClassName( tinyxml2::XMLElementPtr &root );
  void OnTemplateName( tinyxml2::XMLElementPtr &root );
  void OnContainer( tinyxml2::XMLElementPtr &root );
};
