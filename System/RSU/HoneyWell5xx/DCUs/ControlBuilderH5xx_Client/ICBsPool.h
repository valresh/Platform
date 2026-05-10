#pragma once
#include <crossplatform.h>

struct ICBsPool
{
  virtual LPCSTR GetName() = 0;
  virtual bool GetFirstACSObject() = 0;
  virtual bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd ) = 0;
#ifdef _WIN32
  virtual bool ShowACSObject( HWND hMainWnd, char * Name, void * pData ) = 0;
#endif
};
