#pragma once
#include "ControlBuilder.h"

class CONTROLBUILDERH5XX_API KCBPool
{
  struct SCtrlInfo
  {
    LPCSTR pName;
    KControlBuilder *pCB;
  }m_Ctrls[20];
  int m_Count;
  int m_ActiveI;
public:
  KCBPool();
  int GetCtrlCount();
  void RegisterCB( KControlBuilder *pcb );
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
#ifdef _WIN32
  bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif
  bool GetVar( LPCSTR pSuffix, LPCSTR pBlock, LPCSTR pField, BYTE **ppVar, eVarType *pType, USHORT *pVarSize = NULL, LPCSTR *ppszEnum = NULL );
};
