#include "stdafx.h"
#include "Furnace.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

//MODEL_INFO(CFurnace,0,"Печь","Радиантная зона печи")

//#define MODEL_INFO(Class,_Flags,_Model,_Desc) 

CFurnace * pClass = (CFurnace*)0;
extern "C" 
  {
  EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Params )
    { 
    return new CFurnace((char*)ObjName, Params );
    }
EXP int Project = 1;
EXP int Flags = 0;
EXP char * Model = "Печь";
EXP char * Descr = "Радиантная зона печи"; 
  };

