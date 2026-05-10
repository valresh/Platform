#include "Y_RemoteClient.h"

#ifdef _WIN32

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif

extern "C" __declspec(dllexport) IBaseModel* CreateObject(LPCTSTR ObjName , rsu_cp::RsuClient* a_cl)
{
  return new KY_RemoteClient( (char*)ObjName, a_cl);
}
