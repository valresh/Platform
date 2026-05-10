#include "stdafx.h"
#include "MultiSens.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CMultiSens,0,"Универсальный датчик","Универсальный датчик")

NO_NODE_INFO
//#include "Node.h"
//END_NODE_INFO

NO_PARM_INFO
//BEGIN_PARM_INFO
//#include "Points.h"
//END_PARM_INFO

NO_ACS_INFO
//#include "ACS.h"
//END_ACS_INFO
