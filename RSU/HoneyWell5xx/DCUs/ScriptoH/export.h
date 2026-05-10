#pragma once
#include <crossplatform.h>

#ifdef SCRIPTOH5XX_EXPORTS
#define SCRIPTOH5XX_API _EXP
#else
#define SCRIPTOH5XX_API _IMP
#endif
