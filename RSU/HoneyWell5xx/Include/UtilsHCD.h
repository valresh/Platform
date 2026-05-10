#pragma once
#include <BaseType.h>
#include <crossplatform.h>


#ifdef UTILSHCD_EXPORTS
#define UTILSHCD_API _EXP
#else
#define UTILSHCD_API _IMP
#endif

UTILSHCD_API SValueDef* NameToValueHCD( DWORD eData, LPCTSTR name );
UTILSHCD_API LPCSTR EnumStrHCD( SValueDef& def, BYTE val );
UTILSHCD_API BYTE   EnumValHCD( SValueDef& def, LPCSTR name );
