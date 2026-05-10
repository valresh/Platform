#pragma once

#include "CommonRSUs.h"

void COMMONRSUS_API KKK();

void COMMONRSUS_API rsuSysAssert( LPCSTR File, int Line );

#undef FINITE
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
