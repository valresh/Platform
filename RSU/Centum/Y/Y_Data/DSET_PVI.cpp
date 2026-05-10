#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include "Tunning.h"
#include <ConnectY.h>
#include "ReadParms.h"



static CY_Create DSET_PVI ( "DSET-PVI", CY_DSET_PVI::Create );

CY_DSET_PVI::CY_DSET_PVI( CDSET_PVI  * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  OUT.Name = "OUT";
  OUT << W->SV;
  W->mode.M = SMode::MAN;
  W->SV = 0.;
  SUB.Name = "SUB";
  SUB << W->CPV;
  IN = NULL;

  W->SUM = 0.;

  ObrIn.pSH = &W->SH;
  ObrIn.pSL = &W->SL;
  ObrIn.pPV_Overshoot = &W->PV_Overshoot;
  ObrIn.pPV_Limit = &W->PV_Limit;
  ObrIn.BIAS = 0;

  Alarm.Possible = A_HHLL|A_HL|A_VEL;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.pInput_Open_alarm = &W->Input_Open_alarm;
  Alarm.pHHLL = &W->HHLL;
  Alarm.pHL = &W->HL;

  Alarm.pPV = &W->CPV;
  Alarm.pSH = &W->SH;
  Alarm.pSL = &W->SL;
  Alarm.pHH = &W->HH;
  Alarm.pLL = &W->LL;
  Alarm.pH = &W->PH;
  Alarm.pL = &W->PL;
  Alarm.pHYS_HL = &W->HYS_HL;

  Alarm.pVL = &W->VL;
  Alarm.pSides_VL = &W->Sides_VL;
  Alarm.pdT_V_scan = NULL;//??&dT_V_scan;
  Alarm.pHYS_VL = &W->HYS_VL;
  Alarm.pObj = this;

  Alarm.AF = &W->AF;
}

void CY_DSET_PVI::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_DSET_PVI,CDSET_PVI,31,41)

void CY_DSET_PVI::InitParm()
{
#include "Yokogawa\DSET_PVI_P.hpp" 
#include "Yokogawa\DSET_PVI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_DSET_PVI::GetOUT ( char * Name )
{
  GETOUT
  //GET_IN_NAME( OUT, "SV" );
  GET_IN( SUB );
  return NULL;
}

CSignal ** CY_DSET_PVI::GetIN ( char * Name )
{
  GET_IN(IN)
  return NULL;
}

static const double To_h = 1. / 3600.;

void CY_DSET_PVI::Work ( double dt )
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
  if ( W->SV < W->SL )
    W->SV = W->SL;
  if ( W->SV > W->SH )
    W->SV = W->SH;

  Alarm.TestIn ( IN, dt, OUT.Status );
  double dPVn = 0;
  ObrIn.Work ( IN, W->CPV, dPVn, false );

  W->SUM += W->CPV * dt * To_h;
}

void CY_DSET_PVI::Init( )
{
}

int CY_DSET_PVI::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\DSET_PVI_P.hpp" 
  return 0;
}

int CY_DSET_PVI::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
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

int CY_DSET_PVI::SaveState( )
{
  SAVE_BASE_OUT
  SAVE_CLASS("W",CY_DSET_PVI_W)
  return 0;
}

int CY_DSET_PVI::RestoreState ( char * StrName )
{
  REST_BASE_OUT
  REST_CLASS("W",CY_DSET_PVI_W)
  return 1;
}

int CY_DSET_PVI::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  W_PARM(SV)
    MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\DSET_PVI_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\DSET_PVI.hpp"
    E_MENU("Все состояние")
    MENU("Входы")
    PARM_OUT
    E_MENU("Входы")
 	if ( pDR && pDR->FCS )
		PARM_CALLBACK( pDR->FCS, "#FCS" );
   MENU("Base")
    CY_Base::GetParams( params );
  E_MENU("Base")
    return 0;
}

bool CY_DSET_PVI::SetLimits( )
{
  if( IN )
  {
    IN->Min = W->SL;
    IN->Max = W->SH;
    IN->IsLimits = true;
  }
  return true;
}
