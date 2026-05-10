#include <RsuX.h>
#include <rsuNoNames.h>
#include <QbBaseType_.h>
#include <QuickBuilderStruct.h>
#include <stdlib.h>
#include <rsuErr.h>
#include <rsuStr.h>


using namespace nRSUx;

KNoName *g_IOs;

static const DWORD s_iosID[] = { W_QBANALOG::TypeID, W_QBSTATUS::TypeID };

SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  SParamInfo info;

  char splitName[128*4];
  strcpy_s( splitName, pszPointName );
  _Ktoupper(splitName);
  LPSTR pField = strrchr( splitName, '.' );
  if( !pField )
    return info;
  *pField = 0;
  ++pField;

  CBase *pBase = NULL;
  for( int i=0; i<_countof(s_iosID); ++i )
  {
    pBase = g_IOs->FindStruct( splitName, s_iosID[i], 0 );
    if( pBase )
      break;
  }

  ASSD( pBase );
  if( !pBase )
    return info;

  switch( pBase->ID_CLASS )
  {
  case W_QBANALOG::TypeID:
    {
      W_QBANALOG *W = (W_QBANALOG*)pBase;
      if( !strcmp(pField,"PV") )
        info << &W->PV;
      else
      {
        ASS(0);
      }
    }
    break;
  case W_QBSTATUS::TypeID:
    {
      W_QBSTATUS *W = (W_QBSTATUS*)pBase;
      if( !strcmp(pField,"OP") )
        info << &W->OP;
      else if( !strcmp(pField,"PV") )
        info << &W->PV;
      else
      {
        ASS(0);
      }
    }
    break;
  }

  return info;
}
