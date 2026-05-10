#include "stdafx.h"
#include "PC_1.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CPC_1,0,"Поршневой компрессор","Одна ступень поршневого компрессора")

