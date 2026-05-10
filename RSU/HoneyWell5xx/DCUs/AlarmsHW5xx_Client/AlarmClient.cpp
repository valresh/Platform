#include "AlarmClient.h"
#include <CommonH.h>
#include <Names.h>
#include <rsuNoNames.h>
#include <MemoryServiceH5xx.h>

KAlarmClient::KAlarmClient( LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
    :m_proxy(a_cl), m_ObjName(_ObjName)
{
}

KNoName *g_Objs;


static CBase* _StructFinder( LPCSTR ObjName, DWORD TypeID, int number )
{
  return g_Objs->FindStruct( ObjName, TypeID, number );
}

int KAlarmClient::Init( int )
{
    int r = m_proxy.Init(0);
    if (r)
        return 1;
    return 0;
    /*
  g_Objs = KNoName::Create( "H5xx_Objs", false );
  KMemoryServiceH5xx::Instance().InitObjectsStructAllocator( NULL, NULL, _StructFinder );

  m_pSystem = ::GetSystemData();
  m_pCBSystem = GetControlBuilderHSystemData();
  int r = KRsuSharedClient::InitializeServer( "AlarmsHW5xx_Server", ObjName );
  if( r )
    return 1;
  return 0;*/
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
    return m_proxy.StepT(dt);
}
