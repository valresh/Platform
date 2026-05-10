#pragma once
#include <crossplatform.h>

#ifdef CONNECTHCD_EXPORTS
#define CONNECTHCD_API _EXP
#else
#define CONNECTHCD_API _IMP
#endif

typedef void (*tRegisterHCDstructs)();
extern CONNECTHCD_API tRegisterHCDstructs pRegisterHCDstructs;
