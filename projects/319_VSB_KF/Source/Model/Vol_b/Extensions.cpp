#include "stdafx.h"
#include "Vol.h"
#include "Err.h"



bool ICreateVolExtensions::ReadData( IBaseModel * pModel )
  {
  char * ParamsName[] = {
    "Dll",
    "Sub",
    NULL };
    char Params[2][256];
  CLEAR(Params)
  int K = Split( pModel->Model.Str, "Реакции", (const char **)ParamsName, Params );
  if ( K != 2 )
    return false;
  CVol * pVol = (CVol*)pModel;
  pVol->VolBase.pReactions = (IReactions*)Get_Ext( Params[0], Params[1] );
  if ( pVol->VolBase.pReactions == NULL )
    return false;
  pVol->VolBase.pReactions->Init ( pModel );
  pVol->VolBase.pReactions->Name = "Реакции";
  return true;
  }


// extern "C"
//   {
//   _declspec(dllexport) IExtension * GetModelExtension( char * Type )
//     {
//     //if ( lstrcmp ( Type, "Расширения" ) == 0 )
//     //  {
//     //  ICreateColumnExtensions * pCreateColumnExtensions = new ICreateColumnExtensions;
//     //  return pCreateColumnExtensions;
//     //  }
//     return NULL;
//     };
//   };
