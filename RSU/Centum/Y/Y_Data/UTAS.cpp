#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <_CALCU.h>
#include <yokoheaders.h>

static CY_Create UTAS ( "_UTAS", CY_UTAS::Create );

CY_UTAS::CY_UTAS( C_UTAS * _W )
{
  kOut = 0;
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  W->BSTS.V = C_UTAS::_BSTS::PSTART;
}

bool CY_UTAS::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "UDIT", 4) )
    return C_SFC_BASE_Impl::ReadUserDefinition( W->UserDefinition, _countof(W->UserDefinition), sizeof(W->USER), ParmName, ParmValue, pData );

  if( !strncmp(ParmName, "HISR", 4) )
    return C_SFC_BASE_Impl::ReadSfc( ParmName, ParmValue );

  return __super::ReadParm(ParmName, ParmValue, pData);
}

#include "Yokogawa\YPARM_INIT.h" 


LIST_PARM(CY_UTAS,C_UTAS,150,150)

void CY_UTAS::InitParm()
  {
#include "Yokogawa\_UTAS.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_UTAS::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_UTAS::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_UTAS::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}

  if( !pAlg )
    return;
  W->FlagUse |= CAlarmBase::fuYConnect;
  /*if( C_UTAS::_BSTS::RUN==W->BSTS.V )
    pAlg->Work( W );*/

  //W->PREBSTS.V = W->BSTS.V;
  }

void CY_UTAS::Init( )
	{
	}

int CY_UTAS::Prepare0()
{
  pAlg = CAlg::Find( Name );
  if( pAlg )
    pAlg->Prepare0();
	return 0;
}

int CY_UTAS::Prepare1()
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

int CY_UTAS::SaveState( )
	{
	return 0;
	}

int CY_UTAS::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

int CY_UTAS::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM( W->STEPNO, "#STEPNO" );

  static SYParam::SYParamStrVal vals[] = {{"PSTART", W->BSTS.PSTART},
                                          {"STEP", W->BSTS.STEP},
                                          {"SCOMP", W->BSTS.SCOMP},
                                          {"EXECERR", W->BSTS.EXECERR},
                                          {"RESTART", W->BSTS.RESTART},
                                          {"END", W->BSTS.END},
                                          {"SUSPEND", W->BSTS.SUSPEND},
                                          {"PAUSE", W->BSTS.PAUSE},
                                          {"RESET", W->BSTS.RESET}};
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );

	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
  GetParamsFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), params );
	return 0;
	}

bool CY_UTAS::SetLimits( )
  {
  return true;
  }

bool CY_UTAS::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse/*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  bool r = CY_Base::GetVar ( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
  if( r )
    return r;
  return GetVarFromUserDefinition( W->USER, W->UserDefinition, _countof(W->UserDefinition), VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
}

#include <sbsts.h>
void CY_UTAS::OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue )
{
  if( !strcmp(pszParm, "BSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    default:
      ASSD(!"подобрать");
    }
  }
}
