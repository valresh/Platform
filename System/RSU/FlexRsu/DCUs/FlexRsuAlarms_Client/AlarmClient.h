#pragma once
#include <basemodel.h>
#include <DataTypes_.h>
#include <SysDataTypes.h>
#include <rsu_client.h>

class KAlarmClient
{
  //CCentumSystem* m_pCBSystem;
  //CSystem* m_pSystem;
	std::string m_ObjName = "";
	rsu_cp::RsuClientProxy m_proxy;
public:
  KAlarmClient( LPSTR ObjName, rsu_cp::RsuClient* a_cl);
  // IBaseModel
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int StepT(double dt);
  LPCSTR ObjName();
  NEW
};
