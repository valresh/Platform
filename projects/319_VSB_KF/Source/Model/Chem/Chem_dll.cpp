
#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "Info.h"

#ifndef LINUX
BOOL APIENTRY DllMain( _HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
  return TRUE;
}
#endif


MODEL_INFO(CChem,0,"Химия","Термодинамические характеристикт веществ")

NO_NODE_INFO
NO_PARM_INFO
NO_ACS_INFO

