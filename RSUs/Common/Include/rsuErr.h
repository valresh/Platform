#pragma once

#include <QtCore/qglobal.h>

#ifdef SYSTEMREPLACE_EXPORTS
#define SYSTEMREPLACE_API Q_DECL_EXPORT
#else
#define SYSTEMREPLACE_API Q_DECL_IMPORT
#pragma comment( lib, "SystemReplace.lib")
#endif

void SYSTEMREPLACE_API KKK();

void SYSTEMREPLACE_API rsuSysAssert( LPCSTR File, int Line );

#define FINITE(V) ASS(_finite ( V ));

#undef ASS
#define ASS(V) { if(!(V)) rsuSysAssert(__FILE__,__LINE__); }
#undef ASSD
#ifdef _DEBUG
#define ASSD(V) { if(!(V)) rsuSysAssert(__FILE__,__LINE__); }
#else
#define ASSD(V) { if(!(V)) {} }
#endif

#undef ASSR
#define ASSR(V,Ret) { if(!(V)) { rsuSysAssert(__FILE__,__LINE__); return Ret; }}
