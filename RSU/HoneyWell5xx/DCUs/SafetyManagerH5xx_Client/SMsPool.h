#pragma once
#include "ISMsPool.h"

class KSMsPool
{
  struct SCtrlInfo
  {
    LPCSTR pName;
    ISMsPool *pSM;
  }m_Ctrls[50];
  int m_Count;
  int m_ActiveI;
  DWORD m_nAcsFindObjStep;
  DWORD m_nAcsFindIOStep;
public:
  KSMsPool();
  void RegisterSM( ISMsPool *pSM );
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
#ifdef  _WIN32
  bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif
};
