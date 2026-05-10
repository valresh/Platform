#include "stdafx.h"
#include "AirDry.h"
#include "Info.h"//create


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CAirDry,0,"Осушитель","Осушитель воздуха КИП") //create

