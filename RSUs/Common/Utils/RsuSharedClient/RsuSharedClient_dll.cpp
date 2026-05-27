#include "stdafx.h"
#include "rsuSharedClient.h"

// #ifdef LINUX
// #include "LogFile.h"
// #endif // LINUX

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
// #ifdef LINUX
// 		CLogFile::Initialize ("RsuSharedClient");
// 		CLogFile::Log ("RsuSharedClient started");
// #endif // LINUX
		break;
	}

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
