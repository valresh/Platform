#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include "Tunning.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.38

static CY_Create DSET ( "DSET", CY_DSET::Create );

CY_DSET::CY_DSET( CDSET  * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	OUT.Name = "OUT";
  OUT << W->SV;
  W->mode.M = SMode::MAN;
  W->SV = 0.;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_DSET,CDSET,10,20)

void CY_DSET::InitParm()
  {
#include "Yokogawa\DSET_P.hpp" 
#include "Yokogawa\DSET.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_DSET::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_DSET::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_DSET::Work ( double dt )
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
	/*if ( W->SV < W->SL )
		W->SV = W->SL;
	if ( W->SV > W->SH )
		W->SV = W->SH;*/
	}

void CY_DSET::Init( )
	{
	}

int CY_DSET::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\DSET_P.hpp" 
	return 0;
	}

int CY_DSET::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_DSET::SaveState( )
	{
	OUT.SaveState();
	SAVE_CLASS("W",CY_DSET_W)
	return 0;
	}

int CY_DSET::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_DSET_W)
	return 1;
	}

int CY_DSET::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  W_PARM(SV)
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\DSET_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\DSET.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_DSET::SetLimits( )
  {
  return true;
  }
