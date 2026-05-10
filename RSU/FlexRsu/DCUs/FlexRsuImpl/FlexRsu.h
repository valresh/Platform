#pragma once
#include <macros/NonCopyable.h>
#include <TinyXmlReader.h>
#include <FlexDR.h>
#include <rsuMemoryManager.h>
#include <IFlexStateSer.h>

#ifdef _WIN32
#include "../FlexRsuDebugBridge/FlexRsuDebugBridge.h"
#endif
#ifdef FLEXRSUIMPL_EXPORTS
#define FLEXRSUIMPL_API _EXP
#else
#define FLEXRSUIMPL_API _IMP
#endif

#pragma warning( push )
#pragma warning( disable : 4275 )

const DWORD StateVersion_win1251 = 0x00000001;
const DWORD StateVersion_utf8 = 0x01000002;

struct SLocalPageConnector : SPageConnector
{
  KDynString name;
  SLocalPageConnector *pNext;
  void *operator new( size_t size );
};

class FLEXRSUIMPL_API KFlexRsu 
  : n_RSUs::NonCopyable
  , protected KTinyXmlReader<KFlexRsu>
  , public KFlexBlockBase::IProjectContainer
{
  struct CFlexSystem *m_pFlexSystem;
protected:
  KFlexRsu( LPCSTR ObjName );
  int TCoreInitialize( LPCSTR pszCsvPrefix );
  int Initialize( LPCSTR pszCsvPrefix );
  int StepBeforRestore();
  int StepAfterRestore();
  int StepCalc( double dtS, bool addDt );
  int TCoreStepCalc( double dtS, __time64_t curMoment );
  int SaveParamToDRs();
  int SaveState( IFlexStateSer &saver);
  int RestoreState( IFlexStateSer &restorer, DWORD version );
protected:
  virtual void LoadFlexBlocksImpls();
  virtual bool BeforReInitialize();
protected:
  //KFlexBlockBase::IProjectContainer
  virtual BYTE* GetTagField( LPSTR pszPntName, eVarType *pType = NULL, tVarSizeType_ *pVarSize = NULL );
  virtual LPCSTR GetProjectDir();
  virtual KFlexBlockBase* FindBlock( LPCSTR pszName );
  virtual SPageConnector* RegisterPageConnector( LPCSTR pszName );
private:
  void InitFlexSystem();
private:
  int ReadProject();
  int ReadSchema( LPCSTR pszFile );
  void OnItems( tinyxml2::XMLElementPtr &root );
  void OnItem( tinyxml2::XMLElementPtr &root );
  void OnConnections( tinyxml2::XMLElementPtr &root );
  void OnConnection( tinyxml2::XMLElementPtr &root );
private:
  template<size_t _S>
  void GetOutCsvFile( char (&szBuf)[_S] )
  {
    return GetOutCsvFile( szBuf, _S );
  }
  void GetOutCsvFile( char *pszBuf, size_t _S );
protected:
  //ACS
  BYTE* GetVar( LPSTR pszPntName, eVarType *pType = NULL, tVarSizeType_ *pVarSize = NULL );
#ifdef _WIN32
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );

  bool ShowACSObject( void * pInfo, HWND hMainWnd, LPCSTR Name, void * pData );
  void CloseAllDebugWnd();
#endif
protected:
  char m_szObjName[64 * 4];
  char m_szDirPrj[_MAX_PATH * 4];
  char m_szAuditDir[_MAX_PATH * 4];

  KFlexDR **m_ppDR;
  DWORD m_nDR;
  DWORD m_nStep;
  DWORD m_nInitCounter;
#ifdef _WIN32
  static SPrjCfg::tSkipSimIO_ s_bSkipSimIO;
#endif
  int m_nAcsFindStep;

  SLocalPageConnector *m_PageConnectors;
};
#pragma warning( pop )
