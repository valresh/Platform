#pragma once
#include "ICBsPool.h"

class KCBsPool
{
  struct SCtrlInfo
  {
    LPCSTR pName;
    ICBsPool *pCB;
  }m_Ctrls[20];
  int m_Count;
  int m_ActiveI;
  DWORD m_nAcsFindObjStep;
  DWORD m_nAcsFindIOStep;
public:
  KCBsPool();
  void RegisterCB( ICBsPool *pCB );
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
#ifdef  _WIN32
  bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif
};
