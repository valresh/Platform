#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create TOFF ( "TOFF", CY_TOFF::Create );

CY_TOFF::CY_TOFF( CTOFF * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
	OUT.Name = "OUT";
  OUT << W->CPV;
  State = 0;
	IN = NULL;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_TOFF,CTOFF,31,51)

void CY_TOFF::InitParm()
  {
#include "Yokogawa\TOFF_P.hpp" 
#include "Yokogawa\TOFF.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_TOFF::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_TOFF::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_TOFF::Work ( double dt )
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
  if( !CY_Base::s_flagBasicScanControl )
    return;
  m_nBasicControlPeriodCounter = 0;

  if( IN )
    *IN >> W->RV;
  if( State && !W->RV )
    W->CPV = 1;
  else
    W->CPV = 0;
  State = W->RV;
}

void CY_TOFF::Init( )
	{
	}

int CY_TOFF::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\TOFF_P.hpp" 
	return 0;
	}


int CY_TOFF::Prepare1()
{
  State = W->RV;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_TOFF::SaveState( )
	{
	OUT.SaveState();
	return 0;
	}

int CY_TOFF::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

int CY_TOFF::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
//	PARM_MODE ( W->mode, "#Mode" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\TOFF_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\TOFF.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
    PARM_IN(IN)
    PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_TOFF::SetLimits( )
  {
  return true;
  }

