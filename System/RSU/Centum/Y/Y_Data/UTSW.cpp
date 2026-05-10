#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <_CALCU.h>
#include <yokoheaders.h>

static CY_Create UTSW ( "_UTSW", CY_UTSW::Create );

CY_UTSW::CY_UTSW( C_UTSW * _W )
{
  kOut = 0;
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  W->BSTS.V = C_UTSW::_BSTS::STOP;
}

bool CY_UTSW::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "UDIT", 4) )
    return C_SFC_BASE_Impl::ReadUserDefinition( W->UserDefinition, _countof(W->UserDefinition), sizeof(W->USER), ParmName, ParmValue, pData );

  if( !strncmp(ParmName, "HISR", 4) )
    return C_SFC_BASE_Impl::ReadSfc( ParmName, ParmValue );

  return __super::ReadParm(ParmName, ParmValue, pData);
}

#include "Yokogawa\YPARM_INIT.h" 


LIST_PARM(CY_UTSW,C_UTSW,15,15)

void CY_UTSW::InitParm()
  {
#include "Yokogawa\_UTSW.hpp"
#include "Yokogawa\_UTSW_P.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_UTSW::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_UTSW::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_UTSW::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
  if( SMode::AUT!=W->mode.M )
    return;

  if( !pAlg )
    return;
  
  W->FlagUse |= CAlarmBase::fuYConnect;

  if( !pAlg )
    return;
  if( !CY_Base::s_flagBasicScanControl )
    return;
  double deltaT = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
  m_nBasicControlPeriodCounter = 0;

  pAlg->m_pOwner = this;

  if( W->PREBSTS.V != W->BSTS.V )
    pAlg->OnStatusChange( W, deltaT );

  if( C_UTSW::_BSTS::RUN==W->BSTS.V )
    pAlg->Work( W, deltaT );

  if( W->PREBSTS.V != W->BSTS.V )
    pAlg->OnStatusChange( W, deltaT );

  W->PREBSTS.V = W->BSTS.V;
}

void CY_UTSW::Init( )
	{
	}

int CY_UTSW::Prepare0()
{
  pAlg = CAlg::Find( Name );
  if( pAlg )
    pAlg->Prepare0();
	return 0;
}

int CY_UTSW::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  if( pAlg )
  {
    pAlg->Prepare1();
  }
	return 0;
}

int CY_UTSW::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  __super::UserStateSave( pStateSer, W->USER, W->UserDefinition, _countof(W->UserDefinition) );
  return 0;
}

int CY_UTSW::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  __super::UserStateRestore( pStateSer, W->USER, W->UserDefinition, _countof(W->UserDefinition) );
  return 0;
}

int CY_UTSW::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );

  static SYParam::SYParamStrVal vals[] = {{"STOP", W->BSTS.STOP}, {"PAUS", W->BSTS.PAUS}, {"RUN", W->BSTS.RUN} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );

	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
  GetParamsFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), params );
	return 0;
	}

bool CY_UTSW::SetLimits( )
  {
  return true;
  }

bool CY_UTSW::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse/*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool r = CY_Base::GetVar ( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( r )
    return r;
  return GetVarFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
}

#include <sbsts.h>
void CY_UTSW::OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue )
{
  if( !strcmp(pszParm, "BSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    case BS_STOP:
      W->BSTS.V = C_UTSW::_BSTS::STOP;
      W->PREBSTS.V = C_UTSW::_PREBSTS::STOP;
      break;
    case BS_RUN:
      W->BSTS.V = C_UTSW::_BSTS::RUN;
      W->PREBSTS.V = C_UTSW::_PREBSTS::RUN;
      break;
    case BS_WAIT:
      W->BSTS.V = C_UTSW::_BSTS::PAUS;
      W->PREBSTS.V = C_UTSW::_PREBSTS::PAUS;
      break;
    case BS_END:
#pragma message("!!!в будующем реализовать UTSW BSTS\n")//111_BITUM_MSK
      W->BSTS.V = C_UTSW::_BSTS::STOP;
      W->PREBSTS.V = C_UTSW::_PREBSTS::STOP;
      break;
    case BS_SIML:
      break;
    default:
      ASSD(!"подобрать");
      KKK();
    }
  }
}
