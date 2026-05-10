#pragma once

#ifdef SAFETYMANAGERDEBUG_EXPORTS
#define SAFETYMANAGERDEBUG_API _EXP
#else
#define SAFETYMANAGERDEBUG_API _IMP
#endif

void SAFETYMANAGERDEBUG_API ShowDebugWindow( LPCSTR pszPrj, IFscStorage *pOwner );
