#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D1.19
static CY_Create PG_L13 ( "PG-L13", CY_PG_L13::Create );

CY_PG_L13::CY_PG_L13( CPG_L13 * _W ) : CY_Base()
	{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	INT = NULL;
	OUT.Name = "OUT";
	OUT << W->MV;
	SUB.Name = "SUB";
	SUB << vSUB;
  minSV_index = maxSV_index = 0;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_PG_L13,CPG_L13,50,70)

void CY_PG_L13::InitParm()
  {
#include "Yokogawa\PG_L13_P.hpp" 
#include "Yokogawa\PG_L13.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_PG_L13::GetOUT ( char * Name )
	{
	GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_PG_L13::GetIN ( char * Name )
	{
	GET_IN(INT)
	return NULL;
	}

void CY_PG_L13::Work ( double dt )
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

  /*if( W->ZONE > 0 )
  {
    if( W->ZONE >= _countof(W->X) )
      W->ZONE = _countof(W->X) - 1;
    W->SV = W->X[ W->ZONE-1 ];
    W->ZONE = 0;
  }*/

  switch( W->mode.M )
  {
  case SMode::AUT:
  case SMode::CAS:
  case SMode::RCAS:
    svInSecs += dt;
    if( 1==W->Time_Unit )
      W->SV = svInSecs / 60.;
    else
      W->SV = svInSecs;
    if( W->SV > W->Total_Elapsed_Time || W->SV > W->X[maxSV_index] )
    {
      if( W->SV > W->Total_Elapsed_Time )
        W->SV = W->Total_Elapsed_Time;
      if( W->SV > W->X[maxSV_index] )
        W->SV = W->X[maxSV_index];
      vSUB = 1;
      W->mode.M = SMode::MAN;
      if( W->When_program_exits==1 )
      {
        W->SV = W->X[minSV_index];
      }
      svInSecs = W->X[minSV_index];
    }
    else
      vSUB = 0;
    for( int i=1; i<_countof(W->X); ++i )
    {
      if( W->SV>=W->X[i-1] && W->SV<W->X[i] )
      {
        W->ZONE = i;
        W->PL = W->X[i-1];
        W->PH = W->X[i];
        W->MH = W->Y[i-1];
        W->ML = W->Y[i];
        if( W->MH < W->ML )
          std::swap( W->MH, W->ML );
        W->CALC = W->Y[i-1] + ((W->SV -W->X[i-1])*(W->Y[i]-W->Y[i-1])) / (W->X[i]-W->X[i-1]);
        break;
      }
    }
    W->MV = W->CALC;
    break;
  }
}

void CY_PG_L13::Init( )
	{
	}

int CY_PG_L13::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\PG_L13_P.hpp" 
	return 0;
	}

int CY_PG_L13::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  vSUB = 0;
  minSV_index = 0;
  maxSV_index = 0;
  double mi = W->X[minSV_index];
  double ma = W->X[maxSV_index];
  for( int i=1; i<_countof(W->X); ++i )
  {
    if( W->X[i-1] > W->X[i] )
      W->X[i] = W->X[i-1];
    if( W->X[i]<mi )
      mi = W->X[i], minSV_index = i;
    if( W->X[i]>ma )
      ma = W->X[i], maxSV_index = i;
  }
  ASS( maxSV_index >= minSV_index );
	return 0;
}

int CY_PG_L13::SaveState( )
	{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_PG_L13_W)
	return 0;
	}

int CY_PG_L13::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_PG_L13_W)
	return 1;
	}

int CY_PG_L13::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "vSUB", vSUB );
  WriteFullStruct( pStateSer, eKeyAnyVar, "svInSecs", svInSecs );

  return 0;
}

int CY_PG_L13::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "vSUB", vSUB );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "svInSecs", svInSecs );

  return 0;
}

int CY_PG_L13::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\PG_L13_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\PG_L13.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

