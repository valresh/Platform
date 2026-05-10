#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <_CALCU.h>
#include <yokoheaders.h>

static CY_Create SFCSW ( "_SFCSW", CY_SFCSW::Create );

CY_SFCSW::CY_SFCSW( C_SFCSW * _W )
{
  kOut = 0;
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  W->BSTS.V = C_SFCSW::_BSTS::RUN;
  IN = NULL;
}

bool CY_SFCSW::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "UDIT", 4) )
    return C_SFC_BASE_Impl::ReadUserDefinition( W->UserDefinition, _countof(W->UserDefinition), sizeof(W->USER), ParmName, ParmValue, pData );

  if( !strncmp(ParmName, "HISR", 4) )
    return C_SFC_BASE_Impl::ReadSfc( ParmName, ParmValue );

  return __super::ReadParm(ParmName, ParmValue, pData);
}

#include "Yokogawa\YPARM_INIT.h" 


LIST_PARM(CY_SFCSW,C_SFCSW,150,150)

void CY_SFCSW::InitParm()
  {
#include "Yokogawa\_SFCSW.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_SFCSW::GetOUT ( char * Name )
    {
	GET_STD_OUT
	return NULL;
  }

CSignal ** CY_SFCSW::GetIN ( char * Name )
{
  GET_IN(IN)
	return NULL;
}

void CY_SFCSW::Work ( double dt )
{
	SET_HBP
		{
		KKK();
  }
  if( SMode::AUT!=W->mode.M )
    return;

  if( !pAlg )
    return;
  if( !CY_Base::s_flagBasicScanControl )
    return;
  double deltaT = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
  m_nBasicControlPeriodCounter = 0;

  W->FlagUse |= CAlarmBase::fuYConnect;
  pAlg->m_pOwner = this;
  
  if( W->PREBSTS.V != W->BSTS.V )
    pAlg->OnStatusChange( W, deltaT );

  if( C_SFCSW::_BSTS::RUN==W->BSTS.V )
    pAlg->Work( W, deltaT );

  if( W->PREBSTS.V != W->BSTS.V )
    pAlg->OnStatusChange( W, deltaT );

  W->PREBSTS.V = W->BSTS.V;
}

void CY_SFCSW::Init( )
	{
	}

int CY_SFCSW::Prepare0()
{
  pAlg = CAlg::Find( Name );
  if( pAlg )
    pAlg->Prepare0();
	return 0;
}

int CY_SFCSW::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  if( pAlg )
  {
    pAlg->Prepare1();
    pAlg->SetStep( W->STEPNO );
  }
	return 0;
}

int CY_SFCSW::SaveState( )
{
  if( pAlg )
  {
    pAlg->SaveState();
  }
	return 0;
}

int CY_SFCSW::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
  	return 0;
  if( pAlg )
  {
    if( pAlg->RestoreState( StrName ) )
      return 2;
    return 0;
  }
	return 1;
}

int CY_SFCSW::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  __super::UserStateSave( pStateSer, W->USER, W->UserDefinition, _countof(W->UserDefinition) );
  if( pAlg )
    pAlg->StateSave( pStateSer );
  return 0;
}

int CY_SFCSW::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  __super::UserStateRestore( pStateSer, W->USER, W->UserDefinition, _countof(W->UserDefinition) );
  if( pAlg )
    pAlg->StateRestore( pStateSer );
  return 0;
}


int CY_SFCSW::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM( W->STEPNO, "#STEPNO" );
  
  static SYParam::SYParamStrVal vals[] = {{"STOP", W->BSTS.STOP}, {"PAUS", W->BSTS.PAUS}, {"RUN", W->BSTS.RUN} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );

	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
  GetParamsFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), params );
  if( pAlg )
  {
    MENU("ALG")
    pAlg->GetParam( params );
    E_MENU("ALG")
  }

	return 0;
	}

bool CY_SFCSW::SetLimits( )
	{
  return true;
	}

bool CY_SFCSW::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool r = CY_Base::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( r )
    return r;
  return GetVarFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
}

#include <sbsts.h>
void CY_SFCSW::OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue )
{
  if( !strcmp(pszParm, "BSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    case BS_STOP:
      W->BSTS.V = C_SFCSW::_BSTS::STOP;
      W->PREBSTS.V = C_SFCSW::_PREBSTS::STOP;
      break;
    case BS_RUN:
      W->BSTS.V = C_SFCSW::_BSTS::RUN;
      W->PREBSTS.V = C_SFCSW::_PREBSTS::RUN;
      break;
    case BS_WAIT:
      W->BSTS.V = C_SFCSW::_BSTS::PAUS;
      W->PREBSTS.V = C_SFCSW::_PREBSTS::PAUS;
      break;
    default:
      ASSD(!"подобрать");
    }
  }
}
