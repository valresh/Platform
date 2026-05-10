#include "stdafx.h"
#include "Gag.h"
#include "Info.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CGag,0,"Заглушка","Заглушка")

