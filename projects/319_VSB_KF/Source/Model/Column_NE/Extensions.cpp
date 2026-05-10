#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Err.h"
#include "CommProc.h"


bool ICreateColumnExtensions::ReadData( IBaseModel * pModel )
  {
      ASS(0)
  // char * ParamsName[] = {
  //   "Dll",
  //   "Sub",
  //   NULL };
  // char Params[2][256];
  // CLEAR(Params)
  // int K = Split( pModel->Model.Str, "Реакции", (const char **)ParamsName, Params );
  // if ( K != 2 )
  //   return false;
  // CColumn_NE * pCol = (CColumn_NE*)pModel;
  // pCol->NewReactions( Params[0], Params[1] );
  //return true;
  return false;
  }


extern "C" 
  {
  __declspec(dllexport) IExtension * GetModelExtension( char * Type )
    {
    if ( lstrcmp ( Type, "Расширения" ) == 0 )
      {
      ICreateColumnExtensions * pCreateColumnExtensions = new ICreateColumnExtensions;
      return pCreateColumnExtensions;
      }
    return NULL;
    };
  };
