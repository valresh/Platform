#pragma once
#include <TinyXmlReader.h>
#include <vector>
#include "Block.h"
//#include <crossplatform.h>
#include <thread>
#include "CommProc.h"


#include <QtCore/QtGlobal>

#ifdef CNFREADERQ_DECL_EXPORTORTS
#undef CNFREADER_API
#define CNFREADER_API Q_DECL_EXPORT
#else
#define CNFREADER_API Q_DECL_IMPORT
#endif

class CNFREADER_API KCnfReader : protected KTinyXmlReader<KCnfReader>
{
  struct SCnfParam
  {
    SCnfParam() : pOwner(NULL)
    {}
    std::string szFilePath;
    KCnfReader *pOwner;
    std::thread hThread;
  };

protected:
  tBlocksCont_ m_allBlocks;
  tBlockPtr_ m_entryBlock;
  //std::string m_szCurFile;
  DWORD m_MaskRead;

  tDepConnectionsCont_::value_type curDependencyInstance;
  tBlockPtr_ DependencyTagName, DependencyObjectName;
public:
  KCnfReader(void);
  ~KCnfReader(void);

  int ReadCnf( LPCSTR pszMask, DWORD maskRead );
  tBlockPtr_ SortFor( std::string &cntlName );
protected:
  static DWORD ThreadSingleWin( LPVOID lpParam );
  void ThreadReadCnf( LPCSTR pszCnfFile );

  int AddDependencyListFrom( LPCSTR pszPath );

  void OnDependencyListObj( tinyxml2::XMLElementPtr &root );
  void OnCategory( tinyxml2::XMLElementPtr &root );
  void OnDependency( tinyxml2::XMLElementPtr &root );
  void OnDependencyInstances( tinyxml2::XMLElementPtr &root );
  void OnDependencyInstance( tinyxml2::XMLElementPtr &root );
  void OnAttribute( tinyxml2::XMLElementPtr &root );

  void OnSkip( tinyxml2::XMLElementPtr &root ){}
  void OnBlock( tinyxml2::XMLElementPtr &root );
  void OnBlock( tinyxml2::XMLElementPtr &root, LPCSTR pszFileName );

  tBlockPtr_ GetBlock( std::string &name, bool bRemove = true );
  tBlockPtr_ GetBlockAnyThere( std::string &name, tBlocksCont_ *pCont = NULL );
  tBlockPtr_ GetAssignedTo( std::string &parentName );
  void EnumAssignedToEmpty( tBlocksCont_ &arr );

  void FindChildFor( tBlockPtr_ &root );

private: 
  void PrepareUTF8(LPCSTR pszXml);
};
#pragma warning( pop )
