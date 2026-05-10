#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "ReadParms.h"
#include <_CALCU.h>
#include <yokoheaders.h>

static CY_Create SFCPB ( "_SFCPB", CY_SFCPB::Create );

CY_SFCPB::CY_SFCPB( C_SFCPB * _W )
{
  kOut = 0;
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
      
}

bool CY_SFCPB::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "UDIT", 4) )
    return C_SFC_BASE_Impl::ReadUserDefinition( W->UserDefinition, _countof(W->UserDefinition), sizeof(W->USER), ParmName, ParmValue, pData );

  if( !strncmp(ParmName, "HISR", 4) )
    return C_SFC_BASE_Impl::ReadSfc( ParmName, ParmValue );

  return __super::ReadParm(ParmName, ParmValue, pData);
}

#include "Yokogawa\YPARM_INIT.h" 


LIST_PARM(CY_SFCPB,C_SFCPB,150,150)

void CY_SFCPB::InitParm()
  {
#include "Yokogawa\_SFCPB.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_SFCPB::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_SFCPB::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_SFCPB::Work ( double dt )
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

  if( C_SFCPB::_BSTS::RUN==W->BSTS.V )
    pAlg->Work( W, deltaT );

  if( W->PREBSTS.V != W->BSTS.V )
    pAlg->OnStatusChange( W, deltaT );

  W->PREBSTS.V = W->BSTS.V;
}

void CY_SFCPB::Init( )
	{
	}

int CY_SFCPB::Prepare0()
{
  pAlg = CAlg::Find( Name );
  if( pAlg )
    pAlg->Prepare0();
	return 0;
}

int CY_SFCPB::Prepare1()
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

int CY_SFCPB::SaveState( )
	{
	return 0;
	}

int CY_SFCPB::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

int CY_SFCPB::GetParams( KYBridge2SysParam &params )
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

	return 0;
	}

bool CY_SFCPB::SetLimits( )
  {
  return true;
  }

bool CY_SFCPB::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool r = CY_Base::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( r )
    return r;
  return GetVarFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
}

#include <sbsts.h>
void CY_SFCPB::OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue )
{
  if( !strcmp(pszParm, "BSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    case BS_STOP:
      W->BSTS.V = C_SFCPB::_BSTS::STOP;
      W->PREBSTS.V = C_SFCPB::_PREBSTS::STOP;
      break;
    case BS_RUN:
      W->BSTS.V = C_SFCPB::_BSTS::RUN;
      W->PREBSTS.V = C_SFCPB::_PREBSTS::RUN;
      break;
    case BS_PAUS:
      W->BSTS.V = C_SFCPB::_BSTS::PAUS;
      W->PREBSTS.V = C_SFCPB::_PREBSTS::PAUS;
      break;
    default:
      ASSD(!"подобрать");
    }
  }
}
