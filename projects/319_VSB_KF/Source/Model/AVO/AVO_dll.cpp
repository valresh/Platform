#include "stdafx.h"
#include "AVO.h"
#include "Info.h"


#ifndef LINUX
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }
#endif

MODEL_INFO(CAVO,0,"АВО;АВГ","Аппарат воздушного охлаждения")

 /* extern "C" {
 IBaseModel * CreateObject(LPCTSTR ObjName )
  {
   return new CAVO((char*)ObjName);
  }
  }
*/