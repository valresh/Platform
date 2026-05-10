#pragma once
#include <basemodel.h>
//#include <rsuSharedClient.h>
#include <rsuNoNames.h>
#include <DataTypes_.h>
#include <rsu_client.h>

class KQB_RemoteClient
  //: public IBaseModel
  //, protected KRsuSharedClient
{
  KNoName *m_IOs;
  DWORD m_nAcsFindStep;
  std::string m_ObjName = "";
  rsu_cp::RsuClientProxy m_proxy;
public:
  KQB_RemoteClient( LPSTR ObjName, rsu_cp::RsuClient* a_cl);
  // Интерфейс IBaseModel
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int StepT(double dt);
  virtual int SetData( int TypeData, const char *pData );
  NEW
  virtual LPCSTR GetName();
  virtual bool GetFirstACSObject();
  virtual bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
#ifdef _WIN32
  virtual bool ShowACSObject( HWND hMainWnd, char * Name, void * pData );
#endif
};