#include "AlarmQBClient.h"
#include <rsuNoNames.h>

KAlarmQBClient::KAlarmQBClient(LPSTR _ObjName, rsu_cp::RsuClient* a_cl)
	:m_proxy(a_cl), m_ObjName(_ObjName)
{
}
//KAlarmQBClient::KAlarmQBClient( LPSTR _ObjName ) 
//:	IBaseModel ( _ObjName )
//{
//  TypeObj = RSU_Obj;
//}

int KAlarmQBClient::Init( int )
{
	int r = m_proxy.Init(0);
	if (r)
		return 1;
	return 0;//KRsuSharedClient::InitializeServer( "AlarmsQBbridge2HW5xx_Server", ObjName );
}

int KAlarmQBClient::Step0()
{
	return m_proxy.Step0();
}

int KAlarmQBClient::Step1()
{
	return m_proxy.Step1();
}

int KAlarmQBClient::StepT(double dt)
{
	return m_proxy.StepT(dt);
}
