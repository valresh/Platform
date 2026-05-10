#include "stdafx.h"
#include "Furnace.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO_2(CFurnace,0,"Печь","Радиантная зона печи")
