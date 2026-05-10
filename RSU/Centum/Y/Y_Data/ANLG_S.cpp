#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create ANLG_S( "S_ANLG_S", CYS_ANLG_S::Create );

CYS_ANLG_S::CYS_ANLG_S( CS_ANLG_S * _W ) : CY_Base()
{
  W = _W;
  ASS( W );
  if ( W )
  {
    InitData();
    CY_Base::InitFlagUse( W->FlagUse );
  }
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CYS_ANLG_S,CS_ANLG_S,30,50)

void CYS_ANLG_S::InitParm()
{
  #include "Yokogawa\S_ANLG_S_P.hpp"
  #include "Yokogawa\S_ANLG_S.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  _W.HHH = _W.HTRP = _W.LTRP = _W.LLL = 1;
}

void CYS_ANLG_S::InitData( )
{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;
//
  Alarm.Possible = A_HL;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.pInput_Open_alarm = NULL;//&W->Input_Open_alarm;
  Alarm.pHHLL = NULL;//&W->HHLL;
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
  Alarm.pHYS_HL = NULL;//&W->HYS_HL;
//
  /*Alarm.pVL = &W->VL;
  Alarm.pSides_VL = &W->Sides_VL;
  Alarm.pdT_V_scan = NULL;//??&dT_V_scan;
  Alarm.pHYS_VL = &W->HYS_VL;*/
	Alarm.pObj = this;
//
  //Alarm.AF = &W->AF;
//
}

void CYS_ANLG_S::ResetAlarms()
{
  Alarm.SetBlk( W );
}

CSignal * CYS_ANLG_S::GetOUT ( char * Name )
	{
  return NULL;
	}

CSignal ** CYS_ANLG_S::GetIN ( char * Name )
	{
	ASS( 0 );
  return NULL;
	}

void CYS_ANLG_S::Work ( double dt )
{
  SET_HBP
	{
	  KKK();
	}

#ifndef _WIN64
  if( CY_Base::s_nCalcSteps < 11 )
    return;
#endif

  if( !(W->SH > W->SL) )
    return;

  if( !W->HHH )
    Alarm.SEND_ON( A_HHH, W->PH );
  if( !W->HTRP )
    Alarm.SEND_ON( A_HTRP, W->HH );

  if( W->HTRP )
    Alarm.SEND_OFF( A_HTRP );
  if( W->HHH )
    Alarm.SEND_OFF( A_HHH );

  if( !W->LLL )
    Alarm.SEND_ON( A_LLL, W->PL );
  if( !W->LTRP )
    Alarm.SEND_ON( A_LTRP, W->LL );

  if( W->LTRP )
    Alarm.SEND_OFF( A_LTRP );
  if( W->LLL )
    Alarm.SEND_OFF( A_LLL );
}

bool CYS_ANLG_S::Draw ( CDrawData * pDD, int x0, int y0 )
  {
  ASS( 0 );
	return false;
  }

void CYS_ANLG_S::Init( )
	{
	}

int CYS_ANLG_S::Prepare0()
{
  return 0;
}

bool CYS_ANLG_S::SetLimits( )
	{
  return true;
	}

int CYS_ANLG_S::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  W->notACK = 0;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
	return 0;
}

int CYS_ANLG_S::SaveState( )
	{
	return 0;
	}

int CYS_ANLG_S::RestoreState ( char * StrName )
	{
	return 1;
	}

int CYS_ANLG_S::SetData( int TypeData, void * pData )
  {
  return 0;
  }

int CYS_ANLG_S::GetParams( KYBridge2SysParam &params )
{
  #include "IO_Parms.h"
  #include "Yokogawa\YPARM_PARM.h"
  #include "Yokogawa\S_ANLG_S.hpp"
  MENU("Параметры")
  #include "Yokogawa/S_ANLG_S_P.hpp"
  E_MENU("Параметры")
	return 0;
}

