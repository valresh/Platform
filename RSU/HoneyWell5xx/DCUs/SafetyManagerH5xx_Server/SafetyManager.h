#pragma once
#include <rsuPaths.h>
#include <FSC_Info.h>
#include "DbfFiles.h"
#include "FscFiles.h"

class KSafetyManager : public CFscFiles
{
protected:
  CFSC_Info m_FSC_Info[CFSC_Info::MAX_FSC_INFO];
  int m_kFSC_Info;
  BOOL m_Ready;
public:
  KSafetyManager();
protected:
  enum e_RestResult
  {
    err_WrongKey,
    err_CantOpen,
    err_WrongVer,
    err_CommonErr,
    err_DatVerGreaterSrcVer,
    err_Ok
  };
protected:
  int Initialize( LPCSTR pszPrj );
  int StepTL( double dtS );
#ifdef _WIN32
  void ShowACSObject( HWND hMainWnd, char * Name, void * pData );
#endif
protected:
  virtual LPCSTR GetCsvPrefix() = 0;
private:
  e_RestResult RestoryData( LPCSTR pszPrj );
  int ReadProject( LPCSTR pszPrj );
  e_RestResult Load( LPCSTR pszPrj );

  template<size_t _S>
  void GetDatFilePath( char (&Path)[_S], LPCSTR pszPrj )
  {
      std::string fn(pszPrj);
      fn += ".datx";
      fs::path path = fs::path(rsuGetProjectPath()) / "SM" / fn;
      strcpy_s(Path, path.generic_string().c_str());
  }
  template<size_t _S>
  void GetInMemFilePath( char (&Path)[_S], LPCSTR pszPrj, LPCSTR pszFile, LPCSTR pszExt )
  {
    Path[0] = 0;
    fs::path path = fs::path(rsuGetMemoryPath()) / pszFile;
    //_makepath_s( Path, NULL, rsuGetMemoryPath(), pszFile, NULL );
    strcpy_s(Path, path.generic_string().c_str());
    strcat_s( Path, pszPrj );
    if( !pszExt )
      return;
    if( '.'!=pszExt[0] )
      strcat_s( Path, "." );
    strcat_s( Path, pszExt );
  }

  template<size_t _S>
  void GetPathToSmFile( char (&Path)[_S], LPCSTR pszPrj, LPCSTR pszFile, LPCSTR pszExt )
  {
    Path[0] = 0;
    fs::path path = fs::path(rsuGetProjectPath()) / "SM" / pszPrj;
    strcpy_s(Path, path.generic_string().c_str());
    if( !pszFile )
      return;
    strcat_s( Path, "/" );
    strcat_s( Path, pszFile );
    if( !pszExt )
      return;
    if( '.'!=pszExt[0] )
      strcat_s( Path, "." );
    strcat_s( Path, pszExt );
  }
  //IFscStorage
  virtual int FindDbfTag( LPCSTR ptr, CFSC_Info::ePointType ept, CFSC_Info::eType *pPntType = NULL );
  virtual void* VoidInOut( int nDbf, EValueType eType, bool bIn );
  virtual void* CreateSharedIO( int nDbf, EValueType eType, unsigned char mT, LFscBase::typeB mB );
  virtual LPCSTR DbfName( int n, int m );
#ifdef _WIN32
  virtual void FscFileDraw( CMyFont& font, UINT nFile, UINT nSelect, bool bNumber );
#endif
  //
  virtual double GetTimeAccum() = 0;
  virtual __int64 GetNSteps() = 0;
};
