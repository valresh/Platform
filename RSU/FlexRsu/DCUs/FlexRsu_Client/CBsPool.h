#pragma once
#include <crossplatform.h>
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
#ifdef _WIN32
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
  bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif
};
