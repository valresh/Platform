#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

//MODEL_INFO(CColumn_NE,0,"Ректификация","Ректификационная колонна")
CColumn_NE * pCurrentCol;
CColumn_NE * pClass = (CColumn_NE*)0;
extern "C" 
  {
  EXP int Project = 1;
  EXP int Flags = 0;
  EXP char * Model = "Ректификация";
  EXP char * Descr = "Ректификационная колонна";
  EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Type )
    {
    CColumn_NE * pCol = new CColumn_NE((char*)ObjName );
    if ( strstr ( (char*)Type, "CHEM" ))
      pCol->HaveExtensions = true;
    if ( strstr ( (char*)Type, "Абсорбер" ))
      pCol->Absorber = true;
    if ( strstr ( (char*)Type, "Десорбер" ))
      pCol->Desorber = true;
    return pCol;
    };
  };

