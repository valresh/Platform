#pragma once
#include <rsuModel.h>
#include "../QuickBuilderH/QuickBuilder.h"
#include <MemoryServiceQB5xx.h>

class KServer 
  : public IRsuModel
  , protected KQuickBuilder
{
  static const DWORD s_dwStateVersion;
  bool m_bInited, m_bStep0, m_bStep1;
public:
  KServer( LPCSTR pszModelName );
  NEWQB
public:
    int DoInitialize() { return Initialize(); };
    int DoAfterInit() { return AfterInit(); };
    int DoAfterRestored() { return AfterRestored(); };
    int DoCalcStep(int dtMs) { return CalcStep(dtMs); };
    int DoStateSave(LPCSTR pszPath) { return StateSave(pszPath); };
    int DoStateRestore(LPCSTR pszPath) { return StateRestore(pszPath); };
    int DoParamsSave(LPCSTR pszPath) { return ParamsSave(pszPath); };
    int DoParamsRestore(LPCSTR pszPath) { return ParamsRestore(pszPath); };
    void DoShowObject(LPCSTR pszName) {
#ifdef _WIN32
        ShowObject(pszName);
#endif
    };
#ifdef _WIN32
private:
  static LRESULT WINAPI s_WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  static DWORD WINAPI s_WndThread( LPVOID lpParam );
  void WndThread();
  LRESULT WndProcImpl( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  HANDLE m_hWndThread;
  HWND m_hCmdWnd;
  bool ShowACSObject( HWND hMainWnd, char * Name, void * pData );
#endif
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
#ifdef _WIN32
  virtual void ShowObject( LPCSTR pszName );
#endif
};
