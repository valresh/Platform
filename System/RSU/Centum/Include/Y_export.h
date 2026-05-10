#pragma once
#include <crossplatform.h>

#ifdef Y_DATA_EXPORTS
#define Y_DATA_API _EXP
#else
#define Y_DATA_API _IMP
#endif
