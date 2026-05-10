#include "stdafx.h"
#include "Vol.h"
#include "Info.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

ICreateVolExtensions CreateVolExtensions;

CVol * pClass = (CVol*)0;
extern "C" 
  {
  EXP int Project = 1;
  EXP int Flags = 0;
  EXP char * Model = "Vol_b";
  EXP char * Descr = "Емкость";
  EXP IBaseModel * CreateObject(LPCTSTR ObjName, LPCTSTR Type )
    {
    bool Calc2F = false;
    if ( strstr ( Type, "2 фазы" ))
      Calc2F = true;
    if ( strstr ( Type, "2PH" ))
      Calc2F = true;
    bool FullMatr = false;
    if ( strstr ( Type, "Напор" ))
      FullMatr = true;
    if ( strstr ( Type, "FORCE" ))
      FullMatr = true;
    CVol * pVol = new CVol((char*)ObjName, Calc2F, FullMatr );
    if ( strstr ( (char*)Type, "CHEM" ))
      pVol->HaveExtensions = true;
    return pVol;
    }
#ifdef LINUX
    Q_DECL_EXPORT IExtension * GetExtension( const char * Name )
#else
    _declspec(dllexport) IExtension * GetExtension( const char * Name )
#endif
    {
    return &CreateVolExtensions;
    }
  };

