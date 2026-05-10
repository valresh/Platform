#pragma once
#include "crossplatform.h"

#ifdef SERVEREXBASE_EXPORTS
    #define SERVEREXBASE_API _EXP
#else
    #define SERVEREXBASE_API _IMP
#endif

extern SERVEREXBASE_API void LoadPlugins();
