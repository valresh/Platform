#include "stdafx.h"
#include "H_RemoteClient.h"


// #ifdef _MANAGED
// #pragma managed(push, off)
// #endif

// BOOL APIENTRY DllMain( HMODULE hModule,
//                        DWORD  ul_reason_for_call,
//                        LPVOID lpReserved
// 					 )
// {
//     return TRUE;
// }

// #ifdef _MANAGED
// #pragma managed(pop)
// #endif


extern "C" Q_DECL_EXPORT IBaseModel* CreateObject(LPCTSTR ObjName )
{
  IBaseModel* pClient = new KH_RemoteClient( (char*)ObjName);
  pClient->Init ( 0 );
  return pClient;
}
