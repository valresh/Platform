#include "stdafx.h"
#include <RsuX.h>
#include <rsuNoNames.h>
#include <BaseType_.h>
#include <HoneywellStruct.h>
#include <stdlib.h>
#include <rsuErr.h>
#include <UtilsH.h>
#pragma comment( lib, "UtilsH5xx.lib" )

using namespace nRSUx;

KNoName g_IOs;

static const DWORD s_iosID[] = { W_DICHANNEL::TypeID, W_DOCHANNEL::TypeID, W_AICHANNEL::TypeID, W_AOCHANNEL::TypeID, W_PCDINUMARRCH::TypeID, W_HAICHANNEL::TypeID,
                                  W_SP_AI::TypeID, W_SP_AO::TypeID, W_SP_DI::TypeID, W_SP_DO::TypeID, W_SP_SPDVOTE::TypeID, W_SP_SPEED::TypeID };

SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  SParamInfo info;

  char splitName[128];
  strcpy( splitName, pszPointName );
  char *p = splitName;
  while( *p )
  {
    *p = toupper( *p );
    ++p;
  }
  LPSTR pField = strrchr( splitName, '.' );
  if( !pField )
    return info;
  *pField = 0;
  ++pField;

  CBase *pBase = NULL;
  for( int i=0; i<_countof(s_iosID); ++i )
  {
    pBase = g_IOs.FindStruct( splitName, s_iosID[i], 0 );
    if( pBase )
      break;
  }

  if( !pBase )
    return info;

  SValueDef* pDef = NameToValueH( pBase->ID_CLASS, pField );
  if( pDef )
  {
    BYTE *p = (BYTE*)pBase;
    p += pDef->dwShift;
    switch( pDef->eVal )
    {
    case enumValueChr:
      info << (bool*)p;
      break;
    case enumValueDbl:
      info << (double*)p;
      break;
    default:
      ASSD(0);
      break;
    }
  }

  if( info.pMem )
    return info;

  switch( pBase->ID_CLASS )
  {
  case W_DICHANNEL::TypeID:
    {
      ASS( !strcmp(pField,"PV") );
      W_DICHANNEL *W = (W_DICHANNEL*)pBase;
      info << (bool*)&W->PV;
    }
    break;
  case W_DOCHANNEL::TypeID:
    {
      ASS( !strcmp(pField,"SO") );
      W_DOCHANNEL *W = (W_DOCHANNEL*)pBase;
      info << (bool*)&W->SO;
    }
    break;
  case W_AICHANNEL::TypeID:
    {
      ASS( !strcmp(pField,"PV") );
      W_AICHANNEL *W = (W_AICHANNEL*)pBase;
      info << &W->PV;
    }
    break;
  case W_AOCHANNEL::TypeID:
    {
      ASS( !strcmp(pField,"OP") );
      W_AOCHANNEL *W = (W_AOCHANNEL*)pBase;
      info << &W->OP;
    }
    break;
  case W_HAICHANNEL::TypeID:
    {
      ASS( !strcmp(pField,"PV") );
      W_HAICHANNEL *W = (W_HAICHANNEL*)pBase;
      info << &W->PV;
    }
    break;
  }

  return info;
}
