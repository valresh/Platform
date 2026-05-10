#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create DLAY ( "DLAY", CY_DLAY::Create );

CY_DLAY_W::CY_DLAY_W()
	{
	memset ( this, 0, sizeof ( CY_DLAY_W ));
	}

CY_DLAY::CY_DLAY( CDLAY * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	IN = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
	SUB.Name = "SUB";
  SUB.Null();
	CLEAR(Delay)
	CPV = 0.;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_DLAY,CDLAY,10,20)

void CY_DLAY::InitParm()
  {
#include "Yokogawa\DLAY_P.hpp" 
#include "Yokogawa\DLAY.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_DLAY::GetOUT ( char * Name )
	{
  GETOUT
  GETSUB
	return NULL;
	}

CSignal ** CY_DLAY::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_DLAY::Work ( double dt )
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
	if ( IN )
		*IN >> W->RV;
	else
		W->RV = 0;
	double Ksi;
	if ( W -> I < dt )
		Ksi = 1.;
	else
		Ksi = dt / W -> I;
	CPV = ( CPV + Ksi * W->RV * W->GAIN ) / ( 1. + Ksi );
	if ( CPV < W -> SL )
		CPV = W -> SL;
	if ( CPV > W -> SH )
		CPV = W -> SH;
	W->CPV = Delay[0];
	memmove	( &Delay[0], &Delay[1], W->SMPL * sizeof(Delay[0]) );
	Delay[W->SMPL] = CPV;
  }

void CY_DLAY::Init( )
	{
	CLEAR(Delay)
	}

int CY_DLAY::Prepare0()
	{
  W->RV = 0;
  W->CPV = 0;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\DLAY_P.hpp" 
	return 0;
	}


int CY_DLAY::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  ASSD( W->SMPL<_countof(Delay) );
	return 0;
}

static LPCSTR s_Name = "W";
int CY_DLAY::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
  SAVE_CLASS(s_Name,CY_DLAY_W)
	return 0;
}

int CY_DLAY::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS(s_Name,CY_DLAY_W)
	return 1;
}

int CY_DLAY::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "CPV", CPV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "Delay", Delay );
  return 0;
}

int CY_DLAY::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "CPV", CPV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "Delay", Delay );
  return 0;
}

int CY_DLAY::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->RV, "#RV" );
	PARM ( W->CPV,"#CPV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\DLAY_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\DLAY.hpp"
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

bool CY_DLAY::SetLimits( )
  {
  return true;
  }

