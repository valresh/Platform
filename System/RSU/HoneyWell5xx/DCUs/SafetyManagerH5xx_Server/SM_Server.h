#pragma once
#include <rsuModel.h>
#include <MemoryServiceSM5xx.h>
#include "SafetyManager.h"

class KServer 
  : public IRsuModel
  , protected KSafetyManager
{
  bool m_bInited, m_bStep0, m_bStep1;
  char m_szShowObject[256];
  char m_szRealName[64*4];
  double m_timeAccum;
  __int64 m_nCalls;
public:
  KServer( LPCSTR pszModelName );
  NEWSM
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
    virtual LPCSTR GetName();
    virtual bool FindPoint(int nPlcAddress, SDotValue** ppV, void*** ppIO);	
#ifdef _WIN32
private:
  static LRESULT WINAPI s_WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  static DWORD WINAPI s_WndThread( LPVOID lpParam );
  void WndThread();
  LRESULT WndProcImpl( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
  HANDLE m_hWndThread;
  HWND m_hCmdWnd;
#endif
private:
  virtual LPCSTR GetCsvPrefix();
  virtual double GetTimeAccum();
  virtual __int64 GetNSteps();
protected:
  //IRsuModel
  virtual int Initialize();
  virtual int AfterInit();
  virtual int AfterRestored();
  virtual int CalcStep( int dtMs );
  virtual int StateSave( LPCSTR pszPath );
  virtual int StateRestore( LPCSTR pszPath );
  virtual int ParamsSave( LPCSTR pszPath ){return 0;}
  virtual int ParamsRestore( LPCSTR pszPath ){return 0;}
#ifdef _WIN32
  virtual void ShowObject( LPCSTR pszName );
#endif
protected:
  void SaveStateImpl( IStateSer *psaver );
  int RestoreStateImpl( IStateSer *pRest );
  int RestoreFld( IStateSer *pRest, LFscFile *pSavedFld );
};