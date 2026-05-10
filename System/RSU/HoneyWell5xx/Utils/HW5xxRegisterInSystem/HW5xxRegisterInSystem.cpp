#include <ConnectH.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

void RegisterHWstructs();

#ifndef _WIN32
void OnLoadLibraryHWstructs() __attribute__((constructor));
#else
void OnLoadLibraryHWstructs();
#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
  switch( ul_reason_for_call )
  {
  case DLL_PROCESS_ATTACH:
    {
      OnLoadLibraryHWstructs();
    }
    break;
  };
  return TRUE;
}
#endif

void OnLoadLibraryHWstructs()
{
    pRegisterHWstructs = RegisterHWstructs;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

