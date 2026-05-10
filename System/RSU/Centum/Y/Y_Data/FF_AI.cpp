#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.10

static CY_Create FF_AI ( "FF-AI", CY_FF_AI::Create );

CY_FF_AI::CY_FF_AI( CFF_AI * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	OUT.Name = "OUT";
  OUT << W->OUT_V;
	}

void CY_FF_AI::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FF_AI,CFF_AI,20,40)

void CY_FF_AI::InitParm()
  {
#include "Yokogawa\FF_AI_P.hpp" 
#include "Yokogawa\FF_AI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_FF_AI::GetOUT ( char * Name )
	{
  GET_IN_NAME( OUT, "PV" )
  GETOUT
	return NULL;
	}

CSignal ** CY_FF_AI::GetIN ( char * Name )
	{
	ASS(0)
	return NULL;
	}

void CY_FF_AI::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}

  if( W && (W->FlagUse & CAlarmBase::fuY_Mask) )
  {
    CSignal *outs[] = { &OUT };
    for( int k=0;k<_countof(outs); k++ )
      outs[k]->On( Alarm2SignalFlag(W) );
  }

	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return;
  }
// W->PV присваивается снаружи
  W->OUT_V = W->PV;
  Alarm.TestIn( NULL, dt, OUT.Status );
  }

void CY_FF_AI::Init( )
	{
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  Alarm.Possible = A_HHLL|A_HL;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.pHHLL = &W->HHLL;
  Alarm.pHL = &W->HL;
  //
  Alarm.pPV = &W->PV;
  Alarm.pSH = &W->SH;
  Alarm.pSL = &W->SL;
  //  Alarm.pSV = &W->SV;
  Alarm.pHH = &W->HH;
  Alarm.pLL = &W->LL;
  Alarm.pH = &W->PH;
  Alarm.pL = &W->PL;
  Alarm.pHYS_HL = &W->HYS_HL;
  //
  Alarm.pObj = this;
  //
  Alarm.AF = &W->AF;
	}

int CY_FF_AI::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FF_AI_P.hpp" 
	return 0;
	}


int CY_FF_AI::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  if( W->HYS_HL > 1. )
    W->HYS_HL /= 100.;
  if( !(W->HHLL&1) ) W->AF |= 0x00080000;//HH
   else W->AF &= ~0x00080000;
  if( !(W->HHLL&2) ) W->AF |= 0x00040000;//LL
    else W->AF &= ~0x00040000;
  if( !(W->HL&1) ) W->AF |= 0x00008000;//H
    else W->AF &= ~0x00008000;
  if( !(W->HL&2) ) W->AF |= 0x00004000;//L
    else W->AF &= ~0x00004000;
	return 0;
}

int CY_FF_AI::SaveState( )
	{
	OUT.SaveState();
	return 0;
	}

int CY_FF_AI::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

int CY_FF_AI::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->PV, "#PV" );
  if ( pDR && pDR->FCS )
    {
    PARM_CALLBACK( pDR->FCS, "#FCS" );
    } 
  PARM(W->SH, "#SH")
  PARM(W->SL, "#SL")
  PARM(W->HH, "#HH")
  PARM(W->PH, "#PH")
  PARM(W->PL, "#PL")
  PARM(W->LL, "#LL")
  PARM(W->VL, "#VL")
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FF_AI_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\FF_AI.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_FF_AI::SetLimits( )
  {
  return true;
  }

