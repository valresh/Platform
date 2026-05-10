#include "stdafx.h"
#define DLL_COLUMN
#include "Column.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CColumn,0,"Ректификация","Ректификационная колонна")
