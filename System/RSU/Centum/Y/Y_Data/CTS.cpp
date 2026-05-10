#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create TM ( "CTS", CY_CTS::Create );

CY_CTS::CY_CTS( CCTS * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
	OUT.Name = "OUT";
  OUT << W->PV;
	}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_CTS,CCTS,10,20)

void CY_CTS::InitParm()
  {
#include "Yokogawa\CTS_P.hpp" 
#include "Yokogawa\CTS.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_CTS::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_CTS::GetIN ( char * Name )
	{
	return NULL;
	}

bool CY_CTS::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  if( !strcmp(VarName,"XACT") )
  {
    *pVar = &W->ACT;
    *Type = 'I';
    if( pVarSize )
      *pVarSize = sizeof(W->ACT);
    return true;
  }
  return __super::GetVar( VarName, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
}

void CY_CTS::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return;
  }
  if ( W->ACT < 0 )
		return ;
  if ( W->ACT == 0 )
    {
// ACT.OFF
    W->BSTS = W->BSTS.STOP;
    W->ACT = -1;
		return ;
    }
// ACT.ON
  if ( W->BSTS == W->BSTS.STOP )
    {
    W->BSTS = W->BSTS.RUN;
    W->PV = 0;
    }
  W->PV++;
  W->ACT = -1;
  if ( W->PV >= W->PH )
    {
    W->BSTS = W->BSTS.CTUP;
    W->DV = 0;
    }
  else
    {
    W->DV = W->PH - W->PV;
    if ( W->DV <= W->DL )
      W->BSTS = W->BSTS.PALM;
    else
      W->BSTS = W->BSTS.RUN;
    }
  }

void CY_CTS::Init( )
	{
	}

int CY_CTS::Prepare0()
{
  W->ACT = -1;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\CTS_P.hpp" 
	return 0;
}


int CY_CTS::Prepare1()
{
  //W->PL = 0;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_CTS::SaveState( )
	{
	OUT.SaveState();
	return 0;
	}

int CY_CTS::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

int CY_CTS::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  static SYParam::SYParamStrVal vals[] = {{"STOP", W->BSTS.STOP}, {"PAUS", W->BSTS.PAUS},{"RUN", W->BSTS.RUN},{"PALM", W->BSTS.PALM},{"CTUP", W->BSTS.CTUP} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\CTS_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\CTS.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
    PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_CTS::SetLimits( )
  {
  return true;
  }

