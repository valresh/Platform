#pragma once
#ifdef RSUSERVERLIB_EXPORTS
#define RSUSERVERLIB_API _EXP
#else
#define RSUSERVERLIB_API _IMP
#endif

bool RSUSERVERLIB_API RsuServerStart();
void RSUSERVERLIB_API RsuServerStop();

/*BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
);*/

