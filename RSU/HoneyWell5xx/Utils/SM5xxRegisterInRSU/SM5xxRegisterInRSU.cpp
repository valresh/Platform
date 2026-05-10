#include <ConnectSM.h>


#ifdef _MANAGED
#pragma managed(push, off)
#endif

void RegisterSMstructs();

#ifndef _WIN32
void OnLoadLibraryRegisterSMstructs() __attribute__((constructor));
#else
void OnLoadLibraryRegisterSMstructs();
#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch ( ul_reason_for_call )
  {
  case DLL_PROCESS_ATTACH:
      OnLoadLibraryRegisterSMstructs();
    break;
  };
  return TRUE;
}
#endif

void OnLoadLibraryRegisterSMstructs()
{
    pRegisterSMstructs = RegisterSMstructs;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

