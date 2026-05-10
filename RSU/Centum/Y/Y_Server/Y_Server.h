#pragma once
#include <rsuModel.h>
#include "../Y_MainLogic/Y_MainLogic.h"

class KServer 
  : public IRsuModel
  , public CY_MainLogic
{
  static const DWORD s_dwStateVersion, s_dwParmVersion;
  bool m_bInited, m_bStep0;
  char m_szShowObject[256];
public:
  KServer( LPCSTR pszModelName );
  NEWY
private:
  static LRESULT WINAPI s_WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  static DWORD WINAPI s_WndThread( LPVOID lpParam );
  void WndThread();
  LRESULT WndProcImpl( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  HANDLE m_hWndThread;
  HWND m_hCmdWnd;
protected:
  void ChangedParamsSave( LPCSTR pszPath );
  void ChangedParamsRestore( LPCSTR pszPath );
protected:
  //IRsuModel
  virtual int Initialize();
  virtual int AfterInit();
  virtual int AfterRestored();
  virtual int CalcStep( int dtMs );
  virtual int StateSave( LPCSTR pszPath );
  virtual int StateRestore( LPCSTR pszPath );
  virtual int ParamsSave( LPCSTR pszPath );
  virtual int ParamsRestore( LPCSTR pszPath );
  virtual void ShowObject( LPCSTR pszName );
};
