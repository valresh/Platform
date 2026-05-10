#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.19
static CY_Create FUNC_VAR ( "FUNC-VAR", CY_FUNC_VAR::Create );

CY_FUNC_VAR::CY_FUNC_VAR( CFUNC_VAR * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
//
	IN = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
	SUB.Name = "SUB";
  SUB.Null();
	W->GAIN = 1.;
  int n = 0;
#define SET_XY(N) \
  X[n] = &W->X##N;\
  Y[n++] = &W->Y##N;
  SET_XY(01)
  SET_XY(02)
  SET_XY(03)
  SET_XY(04)
  SET_XY(05)
  SET_XY(06)
  SET_XY(07)
  SET_XY(08)
  SET_XY(09)
  SET_XY(10)
  SET_XY(11)
  SET_XY(12)
  SET_XY(13)
  SET_XY(14)
  SET_XY(15)
  CPV_RV = false;
  }

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FUNC_VAR,CFUNC_VAR,50,70)

void CY_FUNC_VAR::InitParm()
  {
#include "Yokogawa\FUNC_VAR_P.hpp" 
#include "Yokogawa\FUNC_VAR.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_FUNC_VAR::GetOUT ( char * Name )
	{
  if ( strcmp ( Name, "CPV") == 0 )
    {
    OUT.Name = "CPV";
    return &OUT;
    }
	GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_FUNC_VAR::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_FUNC_VAR::Work ( double dt )
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
  if ( IN )
    {
	  *IN >> W->RV;
    IN->BackVar = W->RV;
	  IN -> Status &= ~CSignal::SIGNALS_MASK;
    }
  double x = W->RV;
	if ( CPV_RV || W->SECT == 0 )
		{
		W->CPV = W->RV;
    return;
		}
  if( x < *X[0] )
    W->CPV = *Y[0];
  else if( x >= *X[W->SECT] )
    W->CPV = *Y[W->SECT];
  else
  {
	for ( int n = 0; n < W->SECT; n++ )
		{
		if ( x >= *X[n] && x < *X[n+1] )
			{
		  double A = ( x - *X[n] ) / ( *X[n+1] - *X[n] );
		  W->CPV = A * *Y[n+1] + ( 1. - A ) * *Y[n];
			break;
			}
		}
	}
}

void CY_FUNC_VAR::Init( )
	{
	}

int CY_FUNC_VAR::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FUNC_VAR_P.hpp" 
	return 0;
	}

bool CY_FUNC_VAR::SetLimits( )
	{
  if ( IN )
    {
    IN ->Min = W->SL;
    IN ->Max = W->SH;
    IN ->IsLimits = true;
    }
  return true;
	}

int CY_FUNC_VAR::Prepare1()
{
//--------------------------------------
	switch ( W->Aux )//PV, dPV, MV, DMV
		{
		case 0:
			SUB << W->CPV;
		break;
		case 1:
			SUB << dCPV;
		break;
		}
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

static LPCSTR s_Name = "W";
int CY_FUNC_VAR::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
  SAVE_CLASS(s_Name,CY_FUNC_VAR_W)
	return 0;
}

int CY_FUNC_VAR::RestoreState ( char * StrName )
{
	W->ALRM = 0;
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS(s_Name,CY_FUNC_VAR_W)
	return 1;
}

int CY_FUNC_VAR::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );
  return 0;
}

int CY_FUNC_VAR::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPV", dCPV );
  return 0;
}

int CY_FUNC_VAR::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->CPV, "#CPV" );
  PARM ( CPV_RV, "CPV = RV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FUNC_VAR_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\FUNC_VAR.hpp"
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

