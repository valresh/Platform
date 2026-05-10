#pragma once
#include <basemodel.h>
#include <rsu_client.h>
//#include <rsuSharedClient.h>

class KAlarmQBClient
  //: public IBaseModel
  //, protected KRsuSharedClient
{
	std::string m_ObjName = "";
	rsu_cp::RsuClientProxy m_proxy;
public:
  KAlarmQBClient(LPSTR ObjName, rsu_cp::RsuClient* a_cl);
  // Интерфейс IBaseModel
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int StepT(double dt);
  NEW
};
