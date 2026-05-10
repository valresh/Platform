#pragma once
#include <basemodel.h>
#include <rsu_client.h>
#include <rsuNoNames2D.h>
#include <DataTypes_.h>

class KY_RemoteClient 
  : public IBaseModel
{
  KNoName2D *m_yokoIOs, *m_yokoObjs;
  CCentumSystem* m_pCentumSystem;
  DWORD m_nAcsFindStep;
  std::string m_ObjName = "";
  rsu_cp::RsuClientProxy m_proxy;
public:
  KY_RemoteClient( LPSTR ObjName, rsu_cp::RsuClient* a_cl);
  // Интерфейс IBaseModel
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int StepT(double dt);
  virtual int SetData( int TypeData, const char * pData );
  NEW
  //
  bool GetFirstACSObject();
  bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
#ifdef _WIN32
  bool ShowACSObject( HWND hMainWnd, char * Name, void * pData );
#endif
};
