#include "stdafx.h"
#include "Collector.h"
#include "Info.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CCollector,0,"Коллектор","Коллектор")

