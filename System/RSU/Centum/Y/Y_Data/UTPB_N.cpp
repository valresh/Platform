#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <SR_DCU.h>

static CY_Create UTPB_N ( "_UTPB-N", CY_UTPB_N::Create );

CY_UTPB_N::CY_UTPB_N( C_UTPB_N * _W )
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  W->BSTS.V = C_UTPB_N::_BSTS::STOP;
}

extern int CharToIndexColor( LPCSTR color );

bool CY_UTPB_N::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "UDIT", 4) )
    return C_SFC_BASE_Impl::ReadUserDefinition( W->UserDefinition, _countof(W->UserDefinition), sizeof(W->USER), ParmName, ParmValue, pData );

  if( !strncmp(ParmName, "HISR", 4) )
    return C_SFC_BASE_Impl::ReadSfc( ParmName, ParmValue );

  if( !strcmp(ParmName, "BTN5") )
  {
    char Fld[6][16] = { 0 };
    int K = SetFld_X( ParmValue, ':', Fld );
    ASS( K<=_countof(Fld) );
    K = atoi( Fld[0] );
    ASS( K );
    --K;
    ASS( K < _countof(switchAttrs) );
    SSwitchAttr &fp = switchAttrs[K];
    if( !lstrcmpi( Fld[1], "LAMPBUTN") ) 
      fp.swType = LAMPBUTN;
    else
    {
      ASSD(0);
    }
    strcpy_s( fp.szLabel, Fld[2] );
    fp.clrDisplay = CharToIndexColor( Fld[3] );
    ASS( -1!=fp.clrDisplay );
    strcpy_s( fp.szDirective, fp.szLabel );
    W->SWCR[K] = fp.clrDisplay;
    strcpy_s( W->SWLB[K], fp.szLabel );
    return true;
  }

  return __super::ReadParm(ParmName, ParmValue, pData);
}

#include "Yokogawa\YPARM_INIT.h" 


LIST_PARM(CY_UTPB_N,C_UTPB_N,40,40)

void CY_UTPB_N::InitParm()
  {
#include "Yokogawa\_UTPB_N.hpp"
#include "Yokogawa\_UTPB_N_P.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

void CY_UTPB_N::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
  if( SMode::AUT!=W->mode.M )
    return;
  
  W->FlagUse |= CAlarmBase::fuYConnect;
  W->PREBSTS.V = W->BSTS.V;
}

void CY_UTPB_N::Init( )
{
}

bool CY_UTPB_N::SaveParms( HANDLE hFile )
{
  __super::SaveParms( hFile );
  DWORD Lw;
  WRT_DATA( sizeof(switchAttrs), &switchAttrs );

  return false;
}

bool CY_UTPB_N::RestParms( HANDLE hFile )
{
  __super::RestParms( hFile );
  DWORD Lr;
  READ_DATA( sizeof(switchAttrs), &switchAttrs );
  return false;
}

int CY_UTPB_N::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  //PARM_MODE ( W->mode, "#Mode" );

  static SYParam::SYParamStrVal vals[] = {{"STOP", W->BSTS.STOP}, {"PAUS", W->BSTS.PAUS}, {"RUN", W->BSTS.RUN} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );

#include "Yokogawa\YPARM_PARM.h" 
  MENU("Все состояние")
#include "Yokogawa\_UTPB_N.hpp"
  E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
  GetParamsFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), params );
	return 0;
}

bool CY_UTPB_N::SetLimits( )
{
  return true;
}

bool CY_UTPB_N::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse/*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool r = CY_Base::GetVar ( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( r )
    return r;
  return GetVarFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
}

#include <sbsts.h>
void CY_UTPB_N::OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue )
{
  if( !strcmp(pszParm, "BSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    case BS_STOP:
      W->BSTS.V = C_UTPB_N::_BSTS::STOP;
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::STOP;
      break;
    case BS_RUN:
      W->BSTS.V = C_UTPB_N::_BSTS::RUN;
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::RUN;
      break;
    case BS_WAIT:
      W->BSTS.V = C_UTPB_N::_BSTS::PAUS;
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::PAUS;
      break;
    case BS_END:
      W->BSTS.V = C_UTPB_N::_BSTS::STOP;
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::STOP;
      break;
    default:
      ASSD(!"подобрать");
    }
  }
  if( !strcmp(pszParm, "PREBSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    case BS_STOP:
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::STOP;
      break;
    case BS_RUN:
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::RUN;
      break;
    case BS_WAIT:
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::PAUS;
      break;
    case BS_END:
      W->PREBSTS.V = C_UTPB_N::_PREBSTS::STOP;
      break;
    case BS_SIML:
      break;
    default:
      ASSD(!"подобрать");
    }
  }
}

int CY_UTPB_N::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  __super::UserStateSave( pStateSer, W->USER, W->UserDefinition, _countof(W->UserDefinition) );
  return 0;
}

int CY_UTPB_N::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  __super::UserStateRestore( pStateSer, W->USER, W->UserDefinition, _countof(W->UserDefinition) );
  return 0;
}
