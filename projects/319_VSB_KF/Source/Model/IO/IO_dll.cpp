#include "stdafx.h"
#include "IO.h"
#include "Info.h"
//#include "Support.h"


BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

MODEL_INFO(CIO,0,"IO","Источник-потребитель")


#if 0
extern "C"
__declspec(dllexport) int Support ( int TypeData, void * pData )   
  {
  switch ( TypeData )
    {
    case sd_TestConnectionPoints:
      {
      supp_TestConnectionPoints * pCP = (supp_TestConnectionPoints*)pData;
      if ( pCP->kConnPoints != 1 && pCP->kConnPoints != 2 )
        {
        (*(pCP->pSysMsg)) ( "У источника a[%s] число входов = %d", pCP->ObjName, pCP->kConnPoints );
        return 2;
        }
      for ( int n = 0; n < pCP->kConnPoints; n++ )
        {
        if ( lstrcmp ( pCP->pPoints[n].Name, "i" ) != 0 && lstrcmp ( pCP->pPoints[n].Name, "o" ) != 0 )
          {
          (*(pCP->pSysMsg)) ( "У источника a[%s] ошибочный вход b[%s]", 
                              pCP->ObjName, pCP->pPoints[n].Name );
          }
        }
      }
    return 0;
    default:
    return 1;
    }
  return 1;
  }
#endif
