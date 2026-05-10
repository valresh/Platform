#include <RsuX.h>
#include <rsuNoNames.h>
#include <SafetyManagerStruct.h>
#include <stdlib.h>
#include <rsuErr.h>
#include <rsuStr.h>

using namespace nRSUx;

extern KNoName *g_IOs;

static const DWORD s_iosID[] = { W_SMDISCRET::TypeID, W_SMANALOG::TypeID,
                                  W_SMAI::TypeID, W_SMAO::TypeID, W_SMDI::TypeID, W_SMDO::TypeID, W_SMDOCOM::TypeID, W_SMDICOM::TypeID,
                                  //W_SMBI::TypeID, W_SMBO::TypeID
                                };

SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  SParamInfo info;

  char splitName[128*4];
  strcpy_s( splitName, pszPointName );
  _Ktoupper(splitName);
  LPSTR pField = strrchr( splitName, '.' );
  if( pField )
  {
    *pField = 0;
    ++pField;
  }

  CBase *pBase = NULL;
  for( int i=0; i<_countof(s_iosID); ++i )
  {
    pBase = g_IOs->FindStruct( splitName, s_iosID[i], 0 );
    if( !pBase )
      continue;
    switch( pBase->ID_CLASS )
    {
    case W_SMDI::TypeID:
      if( strcmp(pszIOtype,"DI") )
        continue;
      break;
    case W_SMDO::TypeID:
      if( strcmp(pszIOtype,"DO") )
        continue;
      break;
    }
    break;
  }

  ASSD( pBase );
  if( !pBase )
    return info;

//  if( !strcmp(pszPointName, "FV461") )
//    KKK();
  switch( pBase->ID_CLASS )
  {
  default:
    ASSD(0);
    break;
  case W_SMDI::TypeID:
    {
      W_SMDI *W = (W_SMDI*)pBase;
      if( !pField )
        info << &W->PVFL;
      else
      {
        ASS(0);
      }
    }
    break;
  case W_SMDO::TypeID:
    {
      W_SMDO *W = (W_SMDO*)pBase;
      if( !pField )
        info << &W->OPFL;
      else
      {
        ASS(0);
      }
    }
    break;
  case W_SMAI::TypeID:
    {
      W_SMAI *W = (W_SMAI*)pBase;
      if( !pField )
        info << &W->PV;
      else
      {
        ASS(0);
      }
    }
    break;
  case W_SMAO::TypeID:
    {
      W_SMAO *W = (W_SMAO*)pBase;
      if( !pField )
        info << &W->OP;
      else
      {
        ASS(0);
      }
    }
    break;
  case W_SMDISCRET::TypeID:
    {
      W_SMDISCRET *W = (W_SMDISCRET*)pBase;
      if( !pField )
        info << &W->PVFL;
      else
      {
        ASS(0);
      }
    }
    break;
  case W_SMANALOG::TypeID:
    {
      W_SMANALOG *W = (W_SMANALOG*)pBase;
      if( !pField )
        info << &W->PV;
      else
      {
        ASS(0);
      }
    }
    break;
  case W_SMDICOM::TypeID:
    {
      W_SMDICOM *W = (W_SMDICOM*)pBase;
      if( !pField )
        info << &W->PVFL;
      else
      {
        ASS(0);
      }
    }
    break;
  }

  return info;
}
