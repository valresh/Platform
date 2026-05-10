#include "stdafx.h"
#include "Ejector.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CEjector,0,"Эжектор","Газовый эжектор")

