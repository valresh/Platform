#pragma once

#include <QtCore/qglobal.h>
#ifdef UTILSH5XX_EXPORTS
#define UTILSH5XX_API Q_DECL_EXPORT
#else
#define UTILSH5XX_API Q_DECL_IMPORT
#endif

UTILSH5XX_API SValueDef* NameToValueH( DWORD eData, LPCTSTR name );
UTILSH5XX_API LPCSTR EnumStrH( SValueDef& def, BYTE val );
UTILSH5XX_API BYTE   EnumValH( SValueDef& def, LPCSTR name );

