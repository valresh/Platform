#pragma once
#include <BaseType_.h>

struct IFlexRsuModel
{
  virtual BYTE* GetVar( LPSTR pszPntName, eVarType *pType = NULL, tVarSizeType_ *pVarSize = NULL ) = 0;
#ifdef _WIN32
  virtual bool GetFirstACSObject() = 0;
  virtual bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd ) = 0;
  virtual bool ShowACSObject( void * pInfo, HWND hMainWnd, LPCSTR Name, void * pData ) = 0;
#endif
};
