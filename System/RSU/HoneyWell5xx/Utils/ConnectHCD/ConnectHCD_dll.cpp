// ConnectQB.cpp : Defines the entry point for the DLL application.
//


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
