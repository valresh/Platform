#include "stdafx.h"
#include "Valve_R.h"
#include "Info.h"


#ifndef LINUX
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }
#endif
MODEL_INFO(CValve_R,0,"","")

