#pragma once
#include <crossplatform.h>

#ifdef UTILSSM5XX_EXPORTS
#define UTILSSM5XX_API _EXP
#else
#define UTILSSM5XX_API _IMP
#endif

UTILSSM5XX_API SValueDef* NameToValueSM( DWORD eData, LPCTSTR name );
UTILSSM5XX_API LPCSTR EnumStrSM( SValueDef& def, BYTE val );
UTILSSM5XX_API BYTE   EnumValSM( SValueDef& def, LPCSTR name );
