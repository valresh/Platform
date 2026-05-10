#pragma once
#include <rsuModel.h>
#include "../FlexRsuAlarmsKernel/AlarmsFlex.h"

class KAlarmServer 
  : public IRsuModel
  , public KAlarmsFlex
{
  bool m_bInited, m_bStep0, m_bStep1;
public:
  KAlarmServer( LPCSTR pszModelName );

protected:
  //IRsuModel
  virtual int Initialize();
  virtual int AfterInit();
  virtual int AfterRestored();
  virtual int CalcStep( int dtMs );
  virtual int StateSave( LPCSTR pszPath ){return 0;}
  virtual int StateRestore( LPCSTR pszPath ){return 0;}
  virtual int ParamsSave( LPCSTR pszPath ){return 0;}
  virtual int ParamsRestore( LPCSTR pszPath ){return 0;}
#ifdef _WIN32
  virtual void ShowObject(LPCSTR pszName);
#else
  virtual void ShowObject(LPCSTR pszName) {}
#endif
public:
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
  void WriteAlarm( CAlarmEntry& rAlarm );
  void AckAlarm( DWORD ID_PNT_NAME, __int64 moment, bool bInAlarm );
protected:
  virtual CFlexSystem* GetFlexSystemDataL();
};
