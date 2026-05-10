#include "stdafx.h"
#include "HeatChange.h"
#include "Info.h"



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }


MODEL_INFO(CHeatChange,0,"Теплообменник","Трубчатый теплообменник")

