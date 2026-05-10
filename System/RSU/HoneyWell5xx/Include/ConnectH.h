#pragma once
#include <crossplatform.h>

#ifdef CONNECTH5XX_EXPORTS
#define CONNECTH5XX_API _EXP
#else
#define CONNECTH5XX_API _IMP
#endif

typedef void (*tRegisterHWstructs)();
extern CONNECTH5XX_API tRegisterHWstructs pRegisterHWstructs;

typedef void (*tAlarm)( LPCSTR PointName, int Alarm, BYTE Prty, bool ON, double Value, void * bMod, BYTE qbaN, LPCSTR pszValue );
extern CONNECTH5XX_API tAlarm pAlarm;
