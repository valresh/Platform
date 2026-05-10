#include "stdafx.h"
#include "Drainage.h"
#include "Info.h"


#ifndef LINUX
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }
#endif
MODEL_INFO(CDrainage,0,"Дренаж","Дренаж")

