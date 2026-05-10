#include "ServerExPlugin_Honey.h"
#include <ConnectH.h>
#include <RsuX.h>
#include <dylib.hpp>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

nRSUx::SParamInfo RsuConnectParamInfoTypesImpl( LPCSTR pszPointName, DWORD TypeID );

#ifndef _WIN32
void OnLoadLibraryServExPlugHon5xx() __attribute__((constructor));
#else
void OnLoadLibraryServExPlugHon5xx();
#include <windows.h>
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        OnLoadLibraryServExPlugHon5xx();
    break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
#endif

void OnLoadLibraryServExPlugHon5xx()
{
    if (!pRegisterHWstructs)
    {
        auto lib = new dylib("UtilsH5xx", true);
    }
    if (pRegisterHWstructs)
        pRegisterHWstructs();
    /*if( pRegisterRsuConnection )
      pRegisterRsuConnection( NULL, NULL, RsuConnectParamInfoTypesImpl );*/
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
