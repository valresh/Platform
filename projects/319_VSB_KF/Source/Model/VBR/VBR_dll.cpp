#include "stdafx.h"
#include "VBR.h"
#include "Info.h"

#ifndef LINUX

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }
#endif

MODEL_INFO(CVBR,0,"Модель","Описание")

// BEGIN_NODE_INFO
// #include "Node.h"
// END_NODE_INFO

NO_PARM_INFO
//BEGIN_PARM_INFO
//#include "Points.h"
//END_PARM_INFO

NO_ACS_INFO
//BEGIN_ACS_INFO
//#include "ACS.h"
//END_ACS_INFO
