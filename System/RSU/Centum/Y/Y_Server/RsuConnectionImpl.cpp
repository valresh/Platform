#include <rsuNoNames2D.h>
#include <rsuErr.h>
#include <RsuX.h>
#include <DataTypes_.h>
#include <YokogawaStruct.h>
#include <macros/StrHelps.h>
#include <stdlib.h>

using namespace nRSUx;

extern KNoName2D *g_yokoIOs;
CCentumSystem* g_pCentumSystem = NULL;

static const DWORD s_iosID[] = { id__Z, id__Y, id__WW, id__XW, id__WB, id_FF_AI, id_FF_AO, id_FF_MAI, id_FF_DI, id_FF_DO, id__SW, id__AN,
                                 id_BOPCCHR, id_BOPCBOL, id_BOPCI16, id_BOPCU16, id_BOPCI32, id_BOPCU32, id_BOPCF32, id_BOPCF64 };

SParamInfo RsuConnectParamInfo( LPCSTR pszPointName, LPSTR pszIOtype, LPCSTR pSuffix )
{
  SParamInfo info;
  if( pSuffix && '\0'!=*pSuffix )
    return info;
  
  char szName[128];
  strcpy_s( szName, pszPointName );

  LPSTR pszName = szName;
  int nFcsNumber = 0;
  int domains[ _countof(g_pCentumSystem->domains) ] = {};

  LPSTR pSepF = NULL;
  LPSTR pField = strchr( szName, '.' );
  if( pField )
    pSepF = pField, pField++;

  TStringTerminator stF( pSepF );

  LPSTR pNumb = strchr( szName, ':' );
  if( pNumb )
  {
    nFcsNumber = atoi( szName );
    if( nFcsNumber > 100 )
    {
      domains[0] = nFcsNumber / 100;
      nFcsNumber = nFcsNumber % 100;
    }
    pszName = pNumb + 1;
  }
  if( !domains[0] )
  {
    CopyMemory( domains, g_pCentumSystem->domains, sizeof(domains) );
  }

  CBase *pBase = NULL;
  for( int i=0; i<_countof(s_iosID); ++i )
  {
    for( int d=0; d<_countof(domains); ++d )
    {
      if( !domains[d] )
        break;
      pBase = g_yokoIOs->FindStruct( pszName, s_iosID[i], nFcsNumber, domains[d] );
      if( pBase )
        break;
    }
    if( pBase )
      break;
  }

  //ASSD( pBase );
  if( !pBase )
    return info;

  switch( pBase->ID_CLASS )
  {
  case id_BOPCBOL:
    {
      CBOPCBOL* pOPC = (CBOPCBOL*)pBase;
      if( pField )
      {}
      else
      {
        info << &pOPC->PV;
        pOPC->FlagUse |= CAlarmBase::fuYConnect;
      }
    }
    break;
  case id_BOPCI16:
    {
      CBOPCI16* pOPC = (CBOPCI16*)pBase;
      if( pField )
      {}
      else
      {
        info << &pOPC->PV;
        pOPC->FlagUse |= CAlarmBase::fuYConnect;
      }
    }
    break;
  case id_BOPCU16:
    {
      CBOPCU16* pOPC = (CBOPCU16*)pBase;
      if( pField )
      {}
      else
      {
        info << &pOPC->PV;
        pOPC->FlagUse |= CAlarmBase::fuYConnect;
      }
    }
    break;
  case id_BOPCI32:
    {
      CBOPCI32* pOPC = (CBOPCI32*)pBase;
      if( pField )
      {}
      else
      {
        info << &pOPC->PV;
        pOPC->FlagUse |= CAlarmBase::fuYConnect;
      }
    }
    break;
  case id_BOPCU32:
    {
      CBOPCU32* pOPC = (CBOPCU32*)pBase;
      if( pField )
      {}
      else
      {
        info << &pOPC->PV;
        pOPC->FlagUse |= CAlarmBase::fuYConnect;
      }
    }
    break;
  case id_BOPCF32:
    {
      CBOPCF32* pOPC = (CBOPCF32*)pBase;
      if( pField )
      {}
      else
      {
        info << &pOPC->PV;
        pOPC->FlagUse |= CAlarmBase::fuYConnect;
      }
    }
    break;
  case id_BOPCF64:
    {
      CBOPCF64* pOPC = (CBOPCF64*)pBase;
      if( pField )
      {}
      else
      {
        info << &pOPC->PV;
        pOPC->FlagUse |= CAlarmBase::fuYConnect;
      }
    }
    break;
  case id__SW:
    {
      C_SW* pSW = (C_SW*)pBase;
      info << &pSW->PV;
    }
    break;
  case id__Y:
    {
      C_Y* pY = (C_Y*)pBase;
      if( pField )
      {
        switch( *pField )
        {
        case 'D':
          info << &pY->DPV;
          break;
        case 'P':
          info << &pY->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      else
      {
        ASS( pszIOtype );
        switch( *pszIOtype )
        {
        case 'A':
          info << &pY->DPV;
          break;
        case 'D':
          info << &pY->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      pY->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id__Z:
    {
      C_Z* pZ = (C_Z*)pBase;
      if( pField )
      {
        switch( *pField )
        {
        case 'D':
          info << &pZ->DPV;
          break;
        case 'P':
          info << &pZ->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      else
      {
        ASS( pszIOtype );
        switch( *pszIOtype )
        {
        case 'A':
          info << &pZ->DPV;
          break;
        case 'D':
          info << &pZ->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      pZ->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id__WW:
    {
      C_WW* pWW = (C_WW*)pBase;
      if( pField )
      {
        switch( *pField )
        {
        case 'D':
          info << &pWW->DPV;
          break;
        case 'P':
          info << &pWW->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      else
      {
        ASS( pszIOtype );
        switch( *pszIOtype )
        {
        case 'A':
          info << &pWW->DPV;
          break;
        case 'D':
          info << &pWW->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      pWW->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id__XW:
    {
      C_XW* pXW = (C_XW*)pBase;
      if( pField )
      {
        switch( *pField )
        {
        case 'D':
          info << &pXW->DPV;
          break;
        case 'P':
          info << &pXW->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      else
      {
        ASS( pszIOtype );
        switch( *pszIOtype )
        {
        case 'A':
          info << &pXW->DPV;
          break;
        case 'D':
          info << &pXW->PV;
          break;
        default:
          ASS(0);
          return info;
        }
      }
      pXW->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id__WB:
    {
      C_WB* pW = (C_WB*)pBase;
      info << &pW->PV;
      pW->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id_FF_AI:
    {
      CFF_AI* pAI = (CFF_AI*)pBase;
      info << &pAI->PV;
      pAI->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id_FF_AO:
    {
      CFF_AO* pAI = (CFF_AO*)pBase;
      info << &pAI->PV;
      pAI->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id_FF_DI:
    {
      CFF_DI* pAI = (CFF_DI*)pBase;
      info << &pAI->PV;
      pAI->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id_FF_DO:
    {
      CFF_DO* pAI = (CFF_DO*)pBase;
      info << &pAI->PV;
      pAI->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id_FF_MAI:
    ASS( pField );
    if( !pField )
      return info;
    {
      size_t l = strlen( pField );
      if( 6!=l )
        return info;
      CFF_MAI* pMAI = (CFF_MAI*)pBase;
      switch( pField[3] )
      {
      case '1':
        info << &pMAI->OUT1_V;
        break;
      case '2':
        info << &pMAI->OUT2_V;
        break;
      case '3':
        info << &pMAI->OUT3_V;
        break;
      case '4':
        info << &pMAI->OUT4_V;
        break;
      case '5':
        info << &pMAI->OUT5_V;
        break;
      case '6':
        info << &pMAI->OUT6_V;
        break;
      case '7':
        info << &pMAI->OUT7_V;
        break;
      case '8':
        info << &pMAI->OUT8_V;
        break;
      }
      pMAI->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  case id__AN:
    {
      C_AN* pAN = (C_AN*)pBase;
      info << &pAN->PV;
      pAN->FlagUse |= CAlarmBase::fuYConnect;
    }
    break;
  default:
    return info;
  }

  return info;
}
