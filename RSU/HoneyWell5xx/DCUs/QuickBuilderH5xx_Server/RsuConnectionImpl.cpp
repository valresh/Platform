#include <RsuX.h>
#include <macros/StrHelps.h>
#include "QB_Server.h"
#include <rsuErr.h>

using namespace nRSUx;

extern KQuickBuilder* gl_pQuickBuilder;

SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  SParamInfo info;
  if( !gl_pQuickBuilder )
    return info;
  char buf[4][64*4];
  int C = SplitString( pszPointName, buf, '.', false );

  char bigName[48];
  if( 2==C )
    strcpy_s( bigName, buf[1] );
  else if( 3==C )
    sprintf_s( bigName, "%s.%s", buf[1], buf[2] );

  KQbBase *pObj = gl_pQuickBuilder->Find( buf[0] );
  if( !pObj )
    return info;
  BYTE *pMem = NULL;
  eVarType type = evtHZ;
  if( !pObj->GetVar( bigName, &pMem, &type ) )
    return info;
  switch( type )
  {
  default:
    ASSD(0);
    break;
  case evtInt2:
    info << (short*)pMem;
    break;
  case evtFloat:
    info << (float*)pMem;
    break;
  case evtDouble:
    info << (double*)pMem;
    break;
  }
  return info;
}
