#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"


static CY_Create ONOFF ( "ONOFF", CY_ONOFF::Create );

CY_ONOFF_W::CY_ONOFF_W()
	{
	memset ( this, 0, sizeof(*this) );
	}

CY_ONOFF::CY_ONOFF(CONOFF * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	IN = NULL;
	SET = NULL;
	BIN = NULL;
	INT = NULL;
//
	OUT.Name = "OUT";
	OUT << W->MV;
	SUB.Name = "SUB";
	SUB.Null();//- в параметрах
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_ONOFF,CONOFF,30,50)

void CY_ONOFF::InitParm()
  {
#include "Yokogawa\ONOFF_P.hpp" 
#include "Yokogawa\ONOFF.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_ONOFF::GetOUT ( char * Name )
	{
	GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_ONOFF::GetIN ( char * Name )
	{
	GET_IN(IN);
	GET_IN(SET);
	GET_IN(BIN);
	GET_IN(INT);
	return NULL;
	}

void CY_ONOFF::Work ( double dt )
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

  if( IN )
    *IN >> W->PV;

  double SV = W->SV;

  if( SET )
  {
    if( W->mode.M == SMode::CAS)
    {
      SET->Status &= ~CSignal::CND;
      *SET >> W->CSV;
      W->SV = W->CSV;
      if( W->SV < W->SVL )
        W->SV = W->SVL;
      if( W->SV > W->SVH )
        W->SV = W->SVH;
    }
    else
      SET->Status |= CSignal::CND;
  }
  if ( W->mode.M == SMode::RCAS )
    SV = W->RSV;

  double En = W->PV - SV;

  const double hys = (W->SH - W->SL) / 100. * W->onfHYS;

  #pragma message( "!!!! проверить ONOFF")
  double m = -100;
  if( prevEn < En )
  {
    if( En > (W->DB + hys) )
      m = 100;
    else
      m = 0;
  }
  else if( prevEn > En )
  {
    if( En < (W->DB - hys) )
      m = 0;
    else
      m = 100;
  }
  if( m > -99 )
  {
    if( 1==W->PIDD )
    {
      if( m<49 )
        m = 100;
      else if( m > 51 )
        m = 0;
    }
    W->MV = m;
  }
  prevEn = En;
}

void CY_ONOFF::Init( )
	{
	switch ( W->Aux )//PV, dPV, MV, DMV
		{
		case 0:
			SUB << W->PV;
		break;
		case 1:
			SUB << dPV;
		break;
		}
	}

int CY_ONOFF::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\ONOFF_P.hpp" 
  return 0;
	}

int CY_ONOFF::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_ONOFF::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_ONOFF_W)
	return 0;
}

int CY_ONOFF::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_ONOFF_W)
	return 1;
}

int CY_ONOFF::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dPV", dPV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "prevEn", prevEn );

  return 0;
}

int CY_ONOFF::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPV", dPV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "prevEn", prevEn );

  return 0;
}

int CY_ONOFF::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->MV, "#MV" );
  PARM ( GetOut, "#Взять выход" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\ONOFF_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\ONOFF.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN);
	PARM_IN(SET);
	PARM_IN(BIN);
	PARM_IN(INT);
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_ONOFF::SetLimits( )
{
  if ( IN )
  {
    IN ->Min = W->SL;
    IN ->Max = W->SH;
    IN ->IsLimits = true;
  }
  if ( SET )
  {
    SET ->Min = W->SL;
    SET ->Max = W->SH;
    SET ->IsLimits = true;
  }
  return true;
}
