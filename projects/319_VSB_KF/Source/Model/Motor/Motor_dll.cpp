#include "stdafx.h"
#include "Motor.h"
#include "Info.h"

#ifndef LINUX
BOOL APIENTRY DllMain( _HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved	 )
{
  return TRUE;
}

#endif

MODEL_INFO(CMotor,0,"Двигатель","Двигатель")

NO_NODE_INFO
NO_PARM_INFO
NO_ACS_INFO

