#include "Alarms.h"
#include <crossplatform.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

#ifdef _WIN32
#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif

#ifdef _MANAGED
#pragma managed(pop)
#endif


extern "C" _EXP IBaseModel* CreateObject(LPCTSTR ObjName )
{
  return new CAlarmsW(ObjName);
}
