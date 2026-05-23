#pragma once
#include <basemodel.h>
#include <rsuSharedClient.h>
#include <rsuNoNames.h>
#include <DataTypes_.h>

class KQB_RemoteClient
  : public IBaseModel
  , protected KRsuSharedClient
{
  KNoName m_IOs;
  DWORD m_nAcsFindStep;
public:
  KQB_RemoteClient( LPSTR ObjName );
  // Интерфейс IBaseModel
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int StepT(double dt);
  virtual int SetData( int TypeData, void * pData );
//  NEW
  virtual LPCSTR GetName();
  virtual bool GetFirstACSObject();
  virtual bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
  virtual bool ShowACSObject( HWND hMainWnd, char * Name, void * pData );
};
