#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.10

static CY_Create LAG ( "LAG", CY_LAG::Create );

CY_LAG_W::CY_LAG_W()
	{
	memset ( this, 0, sizeof ( CY_LAG_W ));
	}

CY_LAG::CY_LAG( CLAG * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	IN = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
	SUB.Name = "SUB";
  SUB.Null();
	State = 0;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_LAG,CLAG,10,20)

void CY_LAG::InitParm()
  {
#include "Yokogawa\LAG_P.hpp" 
#include "Yokogawa\LAG.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_LAG::GetOUT ( char * Name )
	{
  GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_LAG::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_LAG::Work ( double dt )
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

	double Ksi;
	if ( W -> I < dt )
		Ksi = 1.;
	else
		Ksi = dt / W -> I;
  
  double prevCV = W->CPV;

	W -> CPV = ( W->CPV + Ksi * W->RV * W->GAIN ) / ( 1. + Ksi );
  if( W->PV_Overshoot )
  {
	if ( W -> CPV < W -> SL )
		W -> CPV = W -> SL;
	if ( W -> CPV > W -> SH )
		W -> CPV = W -> SH;
  }
  dCPV = W->CPV - prevCV;
}

void CY_LAG::Init( )
{
  switch ( W->Aux )//CPV, dCPV
  {
  case 0:
    SUB << W->CPV;
    break;
  case 1:
    SUB << dCPV;
    break;
  default:
    ASS(0);
  }
}

int CY_LAG::Prepare0()
	{
  W->RV = 0;
  W->CPV = 0;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\LAG_P.hpp" 
	return 0;
	}


int CY_LAG::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_LAG::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_LAG_W)
	return 0;
}

int CY_LAG::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_LAG_W)
	return 1;
}

static LPCSTR s_Name = "dCPV";
int CY_LAG::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );
  return 0;
}

int CY_LAG::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );
  return 0;
}

int CY_LAG::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->RV, "#RV" );
	PARM ( W->CPV,"#CPV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\LAG_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\LAG.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
		PARM_IN(IN)
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_LAG::SetLimits( )
  {
  return true;
  }

