#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.10

static CY_Create FF_AO ( "FF-AO", CY_FF_AO::Create );

CY_FF_AO::CY_FF_AO( CFF_AO * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	OUT.Name = "OUT";
  OUT << W->MV;
  SET = NULL;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FF_AO,CFF_AO,20,40)

void CY_FF_AO::InitParm()
  {
#include "Yokogawa\FF_AO_P.hpp" 
#include "Yokogawa\FF_AO.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_FF_AO::GetOUT ( char * Name )
	{
  GETOUT
	return NULL;
	}

CSignal ** CY_FF_AO::GetIN ( char * Name )
	{
  GET_IN(SET)
	return NULL;
	}

void CY_FF_AO::Work ( double dt )
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

  if ( SET )
    *SET >> W->CSV;
  else
    KKK();

  if ( W->mode.M == SMode::CAS )
    W->MV = W->PV = W->SV = W->CSV;
  else if ( W->mode.M == SMode::AUT )
    W->MV = W->PV = W->SV;
  else if ( W->mode.M == SMode::MAN )
    W->PV = W->SV = W->MV;
  }

void CY_FF_AO::Init( )
	{
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	}

int CY_FF_AO::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FF_AO_P.hpp" 
	return 0;
	}


int CY_FF_AO::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::CAS;
	return 0;
}

int CY_FF_AO::SaveState( )
	{
	OUT.SaveState();
	return 0;
	}

int CY_FF_AO::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

int CY_FF_AO::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->CSV, "#CSV" );
  PARM ( W->SV, "#SV" );
	PARM ( W->MV, "#MV" );
  if ( pDR && pDR->FCS )
    {
    PARM_CALLBACK( pDR->FCS, "#FCS" );
    } 
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FF_AO_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\FF_AO.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
  PARM_IN(SET)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_FF_AO::SetLimits( )
  {
  return true;
  }

