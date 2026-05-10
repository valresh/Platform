#pragma once
#include <crossplatform.h>
#include <DataTypes_.h>

#ifdef COMMONY_EXPORTS
#define COMMONY_API _EXP
#else
#define COMMONY_API _IMP
#endif


/*COMMONY_API bool AliasGS( char* dst, const char* src );
COMMONY_API bool AliasToName( char* dst, const char* src, const char* fcs );
COMMONY_API int  NameToAlias( const char* mod, const char* src, const char* fcs );*/

// Возвращает полный путь к аларм лог-файлу
COMMONY_API LPCSTR GetAlarmFileName(bool bDubl);
// Возвращает полный путь к файлу сообщений
COMMONY_API LPCSTR GetMsgToOperatorFilePath();

COMMONY_API CCentumSystem* GetCentumSystemData();
