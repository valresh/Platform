#pragma once
#include <crossplatform.h>

struct IRsuModel
{
  virtual int Initialize() = 0;
  virtual int AfterInit() = 0;
  virtual int AfterRestored() = 0;
  virtual int CalcStep( int dtMs ) = 0;
  virtual int StateSave( LPCSTR pszPath ) = 0;
  virtual int StateRestore( LPCSTR pszPath ) = 0;
  virtual int ParamsSave( LPCSTR pszPath ) = 0;
  virtual int ParamsRestore( LPCSTR pszPath ) = 0;
#ifdef _WIN32
  virtual void ShowObject( LPCSTR pszName ) = 0;
#endif
};

typedef IRsuModel* (*tCreateServer)( LPCSTR ObjName );
