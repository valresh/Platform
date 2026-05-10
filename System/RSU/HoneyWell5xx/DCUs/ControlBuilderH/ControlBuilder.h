#pragma once
#include <H_Data.h>
#include "../CnfReader/IBlock.h"
#include "./../../HoneyWell/Include/IStateSer.h"
#include <IControlBuilder.h>
#include <rsuPaths.h>

#ifdef CONTROLBUILDERH5XX_EXPORTS
#define CONTROLBUILDERH5XX_API _EXP
#else
#define CONTROLBUILDERH5XX_API _IMP
#endif

struct _W
{
  DWORD ver;
  _W()
  {
    ver = 0;
  }
};

#pragma warning( push )
#pragma warning( disable : 4251 4275 )

class CONTROLBUILDERH5XX_API KControlBuilder 
  : IControlBuilder
  , protected _W
{
  bool m_bStep0;
  DWORD m_nFirstSteps;
public:
  KControlBuilder( LPCSTR ObjName );
  virtual ~KControlBuilder();
protected:
  int InitL();
  int Step0L();
  int Step1L();
  int StepTL(double dt_sec);
  int SaveState( IStateSer *psaver );
  int RestoreState( IStateSer *psaver );
  int GetParams( KHBridge2SysParam &params );
  DWORD GetLastVersion();
protected:
  LPCSTR GetCsvPrefix();
  int ReadProject( SH_Module *&pRoot );
  enum e_RestResult
  {
    err_WrongKey,
    err_CantOpen,
    err_WrongVer,
    err_CommonErr,
    err_DatVerGreaterSrcVer,
    err_Ok
  };
  template<size_t _S>
  void GetDatFilePath( char (&Path)[_S], LPCSTR pExt = "datx" )
  {
    fs::path t = fs::path(rsuGetProjectPath()) / "CBH" / "bin";
    auto pth = rsu_cp::makepath_with_ext_cp(t.generic_string().c_str(), m_szObjName, pExt);
    strcpy_s(Path, pth.generic_string().c_str());
  }
  std::fstream GetSavedProject( LPCSTR pszFile );
  void SaveProject( SH_Module *pM );
  void SaveModule( SH_Module *pM, std::fstream& hFile );
  void SaveBlock( SH_Block *pB, std::fstream& hFile );
  void SaveBase( KBmBase *pB, std::fstream& hFile );

  e_RestResult RestoreProject( SH_Module *&pRoot );
  e_RestResult RestoreModule( SH_Module *&pM, std::fstream& hFile );
  e_RestResult RestoreBlock( SH_Block *&pB, std::fstream& hFile );
  KBmBase* RestoreBase(std::fstream& hFile );
public:
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
#ifdef _WIN32
  virtual bool ShowACSObject( HWND hMainWnd, char * Name, void * pData, bool bEnternalCall );
#endif
protected:
  virtual void SelectAllConnections( LPCSTR pszName, LPCSTR pszField, bool bInput, std::vector<std::string> &cont );
  virtual void GetPathToCnfFile( char *Path, size_t _S, LPCSTR pszFile, LPCSTR pszExt )
  {
    auto path = rsu_cp::makepath_cp( Path, rsuGetProjectPath(), "CBH");
    path /= m_szPrjName;
    if (!pszFile)
    {
        strcpy_s(Path, _S, path.generic_string().c_str());
        return;
    }
    path /= pszFile;
    if (!pszExt)
    {
        strcpy_s(Path, _S, path.generic_string().c_str());
        return;
    }
    path.replace_extension(pszExt);
    strcpy_s(Path, _S, path.generic_string().c_str());
  }
  virtual void EnableMomStateDEVCTL();
public:
  virtual bool GetVar( LPCSTR pBlock, LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize = NULL, LPCSTR *ppszEnum = NULL );
  template<size_t _S>
  void GetPathToCnfFile( char (&Path)[_S], LPCSTR pszFile, LPCSTR pszExt )
  {
    GetPathToCnfFile( Path, _S, pszFile, pszExt );
  }
public:
  char m_szObjName[64*4];
protected:
  bool m_usePhases;
  int m_nAcsFindStep;
  char m_szEntrySystem[64*4];
  char m_szPrjName[64*4];
  SH_Module * m_entryModule;
  int g_throughNumb;
  LPCSTR pszSelectedModule;

  int m_readNewProject;

  int m_activeModule;
  KBmBase* GetBlockAfterThis( SH_Module* pUpper, LPCSTR pCur, int *_activeModule );
  void AppendTo( SH_Module *module, tBlockPtr_ &ptr );
  int Create( LPCSTR dcuName, LPCSTR hmiName, LPCSTR pszRef2RealBlock, LPCSTR pType, SH_Module **pM, SH_Block **pB );
  void AssignParams( KBmBase *obj, tBlockPtr_ &ptr );

  void AssignToContainers( SH_Module *pM, FILE* pOut );
  void AssignToContainers( SH_Block *pB, FILE* pOut );
  typedef void (KControlBuilder::*tfnAssignToContainers)( SH_Block *, FILE* );
  void fnAssignToContainersSimpleIO( SH_Block *pB, FILE* pOut );
  void fnAssignToContainersMAI( SH_Block *pB, FILE* pOut );
  void fnLinkRefWithIO( SH_Block *pB, FILE* pOut );
  void fnAssignToContainersPCDINUMARRCH( SH_Block *pB, FILE* pOut );
  void fnAssignToContainersSP_( SH_Block *pB, FILE* pOut );

  void LinkConnections( SH_Module *pM );
  void LinkConnections( SH_Block *pB );

  void SelectAllConnections( SH_Module *pM, LPCSTR pszName, LPCSTR pszField, bool bInput, std::vector<std::string> &cont );
  void SelectAllConnections( SH_Block *pB, LPCSTR pszName, LPCSTR pszField, bool bInput, std::vector<std::string> &cont );

  void SaveState( IStateSer *psaver, SH_Module *pM );
  void SaveStateVars( IStateSer *psaver, KBmBase *pB );

  void AssignValue( KBmBase *src, KBmBase *dest, LPCSTR pszField );
};

#pragma warning( pop )
