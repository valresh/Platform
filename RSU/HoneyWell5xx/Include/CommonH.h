#pragma once
#include <DataTypes_.h>
#include <crossplatform.h>


#ifdef COMMONH5XX_EXPORTS
#define COMMONH5XX_DLL _EXP
#else
#define COMMONH5XX_DLL _IMP
#endif

// Возвращает полный путь к аларм лог-файлу
COMMONH5XX_DLL LPCSTR GetAlarmFileName(bool bDubl);
COMMONH5XX_DLL LPCSTR GetNameControlBuilderHoneywell();
COMMONH5XX_DLL LPCSTR GetOneMnemoNameControlBuilderHoneywell();
COMMONH5XX_DLL LPCSTR GetOneAlmGrNameControlBuilderHoneywell();
COMMONH5XX_DLL CBase* CreateControlBuilderHSystemData();
COMMONH5XX_DLL CCBHSystem* GetControlBuilderHSystemData();
