#include "stdafx.h"
#include "HeatAntiFlow.h"
#include "Info.h"
//#include "Support.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CHeatAntiFlow,0,"Противоток","Противоточный теплообменник")

