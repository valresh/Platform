#pragma once
#include <crossplatform.h>

#ifdef UTILSQB5XX_EXPORTS
#define UTILSQB5XX_API _EXP
#else
#define UTILSQB5XX_API _IMP
#endif

UTILSQB5XX_API SValueDef* NameToValueQB( DWORD eData, LPCTSTR name );
UTILSQB5XX_API LPCSTR EnumStrQB( SValueDef& def, BYTE val );
UTILSQB5XX_API BYTE   EnumValQB( SValueDef& def, LPCSTR name );
