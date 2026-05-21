#pragma once

#ifdef RSUSHAREDCLIENT_EXPORTS
#define RSUSHAREDCLIENT_API __declspec(dllexport)
#else
#define RSUSHAREDCLIENT_API __declspec(dllimport)
#pragma comment( lib, "RsuSharedClient.lib")
#endif

class RSUSHAREDCLIENT_API KRsuSharedClient
{
  HWND m_hwndServerImpl;
  HANDLE m_hCalcComplete;
public:
  KRsuSharedClient();
  virtual ~KRsuSharedClient();
protected:
  int InitializeServer( LPCSTR pszImplName, LPCSTR pszObjName, bool bAttachStudio = false );
  int StepAfterInit();
  int StepAfterRestored();
  int CalcStep( int dtMs );
  bool IsCalcComplete( DWORD dwMilliseconds );
  int ParamsSave( LPCSTR pszPath );
  int ParamsRestore( LPCSTR pszPath );
  int StateSave( LPCSTR pszPath );
  int StateRestore( LPCSTR pszPath );
  void ShowObject( LPCSTR pszName );
private:
  int SaveRestoreCmd( LPCSTR pszPath, enum EPackageType type );
};    
