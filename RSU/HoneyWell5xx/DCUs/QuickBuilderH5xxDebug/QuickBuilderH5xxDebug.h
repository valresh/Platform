#pragma once
#include <crossplatform.h>
#include "../QuickBuilderH/QbBase.h"
#ifdef _WIN32

#ifdef QUICKBUILDERH5XXDEBUG_EXPORTS
#define QUICKBUILDERH5XXDEBUG_API _EXP
#else
#define QUICKBUILDERH5XXDEBUG_API _IMP
#endif

QUICKBUILDERH5XXDEBUG_API bool ShowDebugWnd( KQbBase *pPnt );
#endif