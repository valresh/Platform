#include "stdafx.h"
#include "Driver.h"
#include "Info.h"

#ifdef LINUX
#else
BOOL APIENTRY DllMain( _HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved	 )
{
  return TRUE;
}
#endif


MODEL_INFO(CDriver,0,"","")

