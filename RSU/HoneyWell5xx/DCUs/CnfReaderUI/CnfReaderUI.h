#pragma once
#include <crossplatform.h>

#ifdef CNFREADERUI_EXPORTS
#define CNFREADERUI_API _EXP
#else
#define CNFREADERUI_API _IMP
#endif

CNFREADERUI_API void SetProgress( LPCSTR Title, int Proc );
CNFREADERUI_API void CloseProgress();
