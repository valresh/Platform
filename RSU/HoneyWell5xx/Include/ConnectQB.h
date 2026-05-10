#pragma once
#include <RsuX.h>
#include <crossplatform.h>

#ifdef CONNECTQB5XX_EXPORTS
#define CONNECTQB5XX_API _EXP
#else
#define CONNECTQB5XX_API _IMP
#endif

typedef void (*tRegisterQBstructs)();
extern CONNECTQB5XX_API tRegisterQBstructs pRegisterQBstructs;

typedef nRSUx::SParamInfo (*tGetSourceFromMOD)(LPCSTR pszController, int nAddress1, LPCSTR pszAddress2, int ID, bool bBit );
extern CONNECTQB5XX_API tGetSourceFromMOD pGetSourceFromMOD;

typedef void (*tAlarmQB)( LPCSTR PointName, int Alarm, BYTE Prty, bool ON, double Value, void * bMod, BYTE qbaN );
extern CONNECTQB5XX_API tAlarmQB pAlarmQB;
