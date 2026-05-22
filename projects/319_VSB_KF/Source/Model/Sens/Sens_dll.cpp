#include "stdafx.h"
#include "Sens.h"
#include "Info.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved	 )
  {
  return TRUE;
  }

extern char * AllMes;

MODEL_INFO(CSens,0x0100,"Датчик","Датчик")

NO_NODE_INFO
//BEGIN_NODE_INFO
//#include "Node.h"
//END_NODE_INFO

BEGIN_PARM_INFO
#include "Points.h"
END_PARM_INFO

BEGIN_ACS_INFO
#include "ACS.h"
END_ACS_INFO

// extern "C"
// Q_DECL_EXPORT int Support ( int TypeData, void * pData )
//   {
//   switch ( TypeData )
//     {
//     case sd_TestConnectionPoints:
//       {
//       supp_TestConnectionPoints * pCP = (supp_TestConnectionPoints*)pData;
//       if ( pCP->kConnPoints != 2 )
//         {
//         (*(pCP->pSysMsg)) ( "У датчика a[%s] число входов = %d", pCP->ObjName, pCP->kConnPoints );
//         return 2;
//         }
//       return 0;
//       }
//     return 0;
//     default:
//       return 1;
//     }
//   return 1;
//   }
