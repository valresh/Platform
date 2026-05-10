#include "stdafx.h"
#include "UniModel.h"
#include "Info.h"


#ifndef LINUX
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }
#endif
//MODEL_INFO(CUniModel,0,"Универсальная модель","Универсальная модель")

CUniModel * pClass = (CUniModel*)NULL;
extern "C" 
  {
  EXP int Project = 1;
  EXP int Flags = 0;
  EXP char * Model =  "UniModel";
  EXP char * Descr = "Универсальная модель";
  EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Type )
    {
    CUniModel * pUniModel = NULL;
    pUniModel = new CUniModel((char*)ObjName, Type );
    return (IBaseModel*)pUniModel;
    }
  };

