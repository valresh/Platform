#pragma once
#include <RsuX.h>
#include "IFlexRsuModel.h"

class KFlexPool
{
  struct SStationInfo
  {
    LPCSTR pPrjName;
    LPCSTR pszObjName;
    IFlexRsuModel *pFlex;
  }m_Flexes[20];
  int m_Count;
  int m_ActiveI;
public:
  KFlexPool();
  int GetCount();
  int Register( IFlexRsuModel *pStn, LPCSTR pszPrjName, LPCSTR pszObjName );
  nRSUx::SParamInfo GetObj( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix );

  BYTE* GetVar( LPCSTR pszPrjName, LPSTR pszPointName, eVarType *pType = NULL, tVarSizeType_ *pVarSize = NULL );
#ifdef _WIN32
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
  bool ShowACSObject( void * pInfo, HWND hMainWnd, char * Name, void * pData );
#endif
};
