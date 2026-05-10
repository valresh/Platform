#include <ConnectQB.h>


#ifdef _MANAGED
#pragma managed(push, off)
#endif

void RegisterQBstructs();


#ifndef _WIN32
void OnLoadLibraryQBstructs() __attribute__((constructor));
#else
void OnLoadLibraryQBstructs();
#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch ( ul_reason_for_call )
  {
  case DLL_PROCESS_ATTACH:
      OnLoadLibraryQBstructs();
    break;
  };
  return TRUE;
}
#endif

void OnLoadLibraryQBstructs()
{
    pRegisterQBstructs = RegisterQBstructs;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

