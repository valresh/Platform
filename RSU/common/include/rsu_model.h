#pragma once
#include <basemodel.h>
#include "ICBsPool.h"

class KClientModel
  : public IBaseModel
  , protected ICBsPool
{
  DWORD m_nAcsFindStep;
public:
  KClientModel( LPSTR ObjName );
  // Интерфейс IBaseModel
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int StepT(double dt);
  virtual int SetData( int TypeData, void * pData );
  virtual void Finalize();
  NEW
protected:
  char m_szObjName[256];
  //ICBsPool
  virtual LPCSTR GetName();
  virtual bool GetFirstACSObject();
  virtual bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
  virtual bool ShowACSObject( 
#ifdef _WIN32
	  HWND hMainWnd, 
#else

#endif
	char * Name, void * pData );
};
