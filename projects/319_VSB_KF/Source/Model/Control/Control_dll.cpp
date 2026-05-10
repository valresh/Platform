#include "stdafx.h"
#define DLL_Control
#include "Control.h"

#ifndef LINUX
BOOL APIENTRY DllMain( _HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved	 )
{
  return TRUE;
}
#endif


