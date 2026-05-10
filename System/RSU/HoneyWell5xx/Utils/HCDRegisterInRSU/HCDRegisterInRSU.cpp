#include <ConnectHCD.h>


#ifdef _MANAGED
#pragma managed(push, off)
#endif

void RegisterHCDstructs();

#ifndef _WIN32
void OnLoadLibraryRegHCD() __attribute__((constructor));
#else
void OnLoadLibraryRegHCD();

#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  switch ( ul_reason_for_call )
  {
  case DLL_PROCESS_ATTACH:
      OnLoadLibraryRegHCD();
    break;
  };
  return TRUE;
}
#endif
void OnLoadLibraryRegHCD()
{
    pRegisterHCDstructs = RegisterHCDstructs;
}


#ifdef _MANAGED
#pragma managed(pop)
#endif

