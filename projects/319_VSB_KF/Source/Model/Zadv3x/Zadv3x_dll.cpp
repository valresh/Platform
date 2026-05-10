#include "stdafx.h"
#include "Zadv3x.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CZadv3x,0,"Задвижка 3х","Задвижка с 1 входом и 2-мя выходами")

