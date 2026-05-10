#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.32
static CY_Create BNOT ( "BNOT", CY_BNOT::Create );

CY_BNOT::CY_BNOT( CBNOT * _W ) : CY_Base()
	{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	IN = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
	}

//YOKO   1
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BNOT,CBNOT,30,50)

void CY_BNOT::InitParm()
  {
#include "Yokogawa\BNOT_P.hpp" 
#include "Yokogawa\BNOT.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_BNOT::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_BNOT::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_BNOT::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  if( IN )
	  *IN >> W->RV;
  W->CPV = ~W->RV;
}

void CY_BNOT::Init( )
{
}

int CY_BNOT::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\BNOT_P.hpp" 
	return 0;
	}

int CY_BNOT::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

static LPCSTR s_Name = "W";
int CY_BNOT::SaveState( )
{
	OUT.SaveState();
  SAVE_CLASS(s_Name,CY_BNOT_W)
	return 0;
}

int CY_BNOT::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS(s_Name,CY_BNOT_W)
	return 1;
}

int CY_BNOT::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  W_PARM(RV)
	W_PARM(CPV)
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\BNOT_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\BNOT.hpp"
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

bool CY_BNOT::SetLimits( )
  {
  return true;
  }

