#include "stdafx.h"
#include "Klap3x.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CKlap3x,0,"Клапан 2","Клапан с 2-мя выходами")

