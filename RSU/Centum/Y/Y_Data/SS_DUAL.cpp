#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create SS_DUAL ( "SS-DUAL", CY_SS_DUAL::Create );

CY_SS_DUAL::CY_SS_DUAL( CSS_DUAL * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	IN1 = NULL;
	IN2 = NULL;
	OUT.Name = "OUT";
	OUT << W->PV;
	INFO(W->PV,"PV")
	INFO(SEL,"SEL")
  W->SEL = 1;
  W->SW = 1;
//
  ObrIn.pSH = &W->SH;
  ObrIn.pSL = &W->SL;
  ObrIn.pPV_Overshoot = &W->PV_Overshoot;
  ObrIn.pPV_Limit = &W->PV_Limit;
//
  Alarm.pObj = this;
  Alarm.pCALI = &W->CALI;
	Alarm.pTime_DL = NULL;
	Alarm.pPV = &W->RV1;
	Alarm.pSV = &W->RV2;
	Alarm.pGAIN_DL = NULL;
	Alarm.pDL = &W->DL;
	Alarm.pDeviation_DL = &W->Deviation_DL;
	Alarm.pHYS_DL = &W->Deviation_HYS;
	Alarm.pSH = &W->SH;
	Alarm.pSL = &W->SL;
	Alarm.SetBlk( W );
	Alarm.pAlarmLevel = &W->AlarmLevel;
	}

void CY_SS_DUAL::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SS_DUAL,CSS_DUAL,30,50)

void CY_SS_DUAL::InitParm()
{
#include "Yokogawa\SS_DUAL_P.hpp" 
#include "Yokogawa\SS_DUAL.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );

  _W.SW = 3;
  CYVarInfo Key;
  Key.VarName = "SW";
  CYVarInfo * pI = (CYVarInfo*)bsearch ( &Key, VarInfo, kVarInfo, sizeof( CYVarInfo ), CompVarInfo );
  if( pI )
    pI->Work = false;
}

CSignal * CY_SS_DUAL::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_SS_DUAL::GetIN ( char * Name )
	{
	GET_IN(IN1)
	GET_IN(IN2)
	return NULL;
	}

void CY_SS_DUAL::Work ( double dt )
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
  if( W->SW==0 )
    W->SW = 3;
	Signal_selection( );
  if ( SEL < 0 )
    SEL = 0;
  if ( SEL > 2 )
    SEL = 2;
  W->SEL = SEL;
//	if ( W->PV < W->SL )
//		W->PV = W->SL;
//	if ( W->PV > W->SH )
//		W->PV = W->SH;
	W->DV = W->RV1 - W->RV2;
	Alarm.DEV( dt );
	}

void CY_SS_DUAL::Signal_selection( )
	{
///////////////////////////////////////////////
	W->mode.TRK = false;
///////////////////////////////////////////////
	ObrIn.PV_Overshoot ( IN1, W->RV1, &Bad1, W->CALI );
	ObrIn.PV_Overshoot ( IN2, W->RV2, &Bad2, W->CALI );
/*
  if ( IN1 )
    *IN1 >> W->RV1;
	if ( IN2 )
    *IN2 >> W->RV1;
	Bad1 = Bad2 = false;
*/
//
	if ( W->SW <= 0 )
		return;
	if ( W->SW >= 1 && W->SW <= 2	)
		{
		SEL = W->SW;
		}
	else
		{
// SW = 3 автомат
		bool Bad = Bad1 && Bad2;
		if ( Bad )
			{
			W->SW = 0;
			SEL = 0;
			}
		else
			{
			if ( SEL == 0 )
        {
        if ( !Bad1 )
          SEL = 1;
        else
          SEL = 2;
        }
      else
        {
			  if ( SEL == 1 && Bad1 )
          SEL = 2;
			  if ( SEL == 2 && Bad2 )
          SEL = 1;
        }
			}
		}
//
	if ( SEL == 1 )
		{
		W->PV = W->RV1;
		W->SV = W->RV2;
		return ;
		}
	if ( SEL == 2 )
		{
		W->PV = W->RV2;
		W->SV = W->RV1;
		return ;
		}
	}

void CY_SS_DUAL::Init( )
	{
	}

int CY_SS_DUAL::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\SS_DUAL_P.hpp" 
	return 0;
	}

#undef VAR_DBL 
#define VAR_DBL(Var) W->Var = Var;
int CY_SS_DUAL::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_SS_DUAL::SaveState( )
{
	OUT.SaveState();
	SAVE_CLASS("W",CY_SS_DUAL_W)
	return 0;
}

int CY_SS_DUAL::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_SS_DUAL_W)
	return 1;
}

int CY_SS_DUAL::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "SEL", SEL );

  return 0;
}

int CY_SS_DUAL::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "SEL", SEL );

  return 0;
}

int CY_SS_DUAL::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM ( W->Comment, "#Comment" );
	PARM_MODE ( W->mode, "#Mode" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SS_DUAL_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\SS_DUAL.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN1)
	PARM_IN(IN2)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_SS_DUAL::SetLimits( )
  {
	if ( IN1 )
		{
		IN1 ->Min = W->SL;
		IN1 ->Max = W->SH;
		IN1 ->IsLimits = true;
		}
	if ( IN2 )
		{
	  IN2 ->Min = W->SL;
		IN2 ->Max = W->SH;
		IN2 ->IsLimits = true;
		}
  return true;
  }

