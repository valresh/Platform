#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "ReadParms.h"

// D2.14

static CY_Create LDLAG ( "LDLAG", CY_LDLAG::Create );

CY_LDLAG_W::CY_LDLAG_W()
{
	memset ( this, 0, sizeof ( *this ));
}

CY_LDLAG::CY_LDLAG( CLDLAG * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	IN = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
	SUB.Name = "SUB";
  SUB.Null();
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_LDLAG,CLDLAG,10,20)

void CY_LDLAG::InitParm()
  {
#include "Yokogawa\LDLAG_P.hpp" 
#include "Yokogawa\LDLAG.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_LDLAG::GetOUT ( char * Name )
	{
  GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_LDLAG::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_LDLAG::Work ( double dt )
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
	if ( W->I < dt || 0==W->I)
		Ksi = 1.;
	else
		Ksi = dt / W -> I;

	double CPV_new = ( 1 + W->D ) * W->RV * W->GAIN / ( 1. + Ksi );

  dCPV = CPV_new - W->CPV;

  if( CPV_new < W -> SL )
		CPV_new = W -> SL;
	if( CPV_new > W -> SH )
		CPV_new = W -> SH;

  W->CPV = CPV_new;
}

void CY_LDLAG::Init( )
{
  switch ( W->Aux )//CPV dCPV
  {
  case 0:
    SUB << W->CPV;
    break;
  case 1:
    SUB << dCPV;
    break;
  }
}

int CY_LDLAG::Prepare0()
	{
  W->RV = 0;
  W->CPV = 0;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\LDLAG_P.hpp" 
	return 0;
	}


int CY_LDLAG::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_LDLAG::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_LDLAG_W)
	return 0;
}

int CY_LDLAG::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_LDLAG_W)
	return 1;
}

static LPCSTR s_Name = "dCPV";
int CY_LDLAG::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );
  return 0;
}

int CY_LDLAG::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );
  return 0;
}

int CY_LDLAG::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->RV, "#RV" );
	PARM ( W->CPV,"#CPV" );
  PARM ( dCPV,"#dCPV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\LDLAG_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\LDLAG.hpp"
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

bool CY_LDLAG::SetLimits( )
  {
  return true;
  }

