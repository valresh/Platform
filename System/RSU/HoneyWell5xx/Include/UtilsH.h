#pragma once
#include <crossplatform.h>

#ifdef UTILSH5XX_EXPORTS
#define UTILSH5XX_API _EXP
#else
#define UTILSH5XX_API _IMP
#endif

UTILSH5XX_API SValueDef* NameToValueH( DWORD eData, LPCTSTR name );
UTILSH5XX_API LPCSTR EnumStrH( SValueDef& def, BYTE val );
UTILSH5XX_API BYTE   EnumValH( SValueDef& def, LPCSTR name );
