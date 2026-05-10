#pragma once
#include <RsuX.h>
#include <crossplatform.h>

#ifdef CONNECTSM5XX_EXPORTS
#define CONNECTSM5XX_API _EXP
#else
#define CONNECTSM5XX_API _IMP
#endif

typedef void (*tRegisterSMstructs)();
extern CONNECTSM5XX_API tRegisterSMstructs pRegisterSMstructs;

typedef nRSUx::SParamInfo (*tGetSourceFromSM)(LPCSTR pszController, int nPlcAddress, bool Direction );
extern CONNECTSM5XX_API tGetSourceFromSM pGetSourceFromSM;
