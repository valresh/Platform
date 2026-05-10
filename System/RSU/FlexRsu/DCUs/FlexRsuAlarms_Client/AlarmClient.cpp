#include "AlarmClient.h"
#include <rsuNoNames.h>

KAlarmClient::KAlarmClient(LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
    :m_proxy(a_cl), m_ObjName(_ObjName)
{
  
}

LPCSTR KAlarmClient::ObjName()
{
    return m_ObjName.c_str();
}

int KAlarmClient::Init( int )
{
  int r = m_proxy.Init(0);
  if (r)
      return 1;
  return 0;
}

int KAlarmClient::Step0()
{
  //return KRsuSharedClient::StepAfterInit();
  return m_proxy.Step0();
}

int KAlarmClient::Step1()
{
  //return KRsuSharedClient::StepAfterRestored();
  return m_proxy.Step1();
}

int KAlarmClient::StepT(double dt)
{
  double dtMs = dt * 3600. * 1000;
  return m_proxy.StepT(dt);
}
