#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"


static CY_Create ONOFF_GE( "ONOFF-GE", CY_ONOFF_GE::Create );

CY_ONOFF_GE_W::CY_ONOFF_GE_W()
{
  memset ( this, 0, sizeof(*this) );
}

CY_ONOFF_GE::CY_ONOFF_GE(CONOFF_GE * _W ) : CY_Base()
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
  Out[0].Name = "OUT1";
  Out[0] << pinOut0;
  Out[1].Name = "OUT2";
  Out[1] << pinOut1;
  kOut = 2;
  SUB.Name = "SUB";
  SUB.Null();//- в параметрах

  Alarm.Possible = A_HHLL|A_HL|A_DV|A_MHL|A_VEL;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.pHHLL = &W->HHLL;
  Alarm.pHL = &W->HL;
  //
  Alarm.pPV = &W->PV;
  Alarm.pSH = &W->SH;
  Alarm.pSL = &W->SL;
  Alarm.pSV = &W->SV;
  Alarm.pHH = &W->HH;
  Alarm.pLL = &W->LL;
  Alarm.pH = &W->PH;
  Alarm.pL = &W->PL;
  Alarm.pHYS_HL = &W->HYS_HL;

  Alarm.pObj = this;
}

void CY_ONOFF_GE::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_ONOFF_GE,CONOFF_GE,35,55)

void CY_ONOFF_GE::InitParm()
{
#include "Yokogawa\ONOFF-GE_P.hpp" 
#include "Yokogawa\ONOFF-GE.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_ONOFF_GE::GetOUT ( char * Name )
{
  if( !strcmp(Name,"OUT") )
  {
    ASSD(!"нет такого выхода")
  }
  GET_STD_OUT
  GETSUB
  return NULL;
}

CSignal ** CY_ONOFF_GE::GetIN ( char * Name )
{
  GET_IN(IN);
  GET_IN(SET);
  GET_IN(BIN);
  GET_IN(INT);
  return NULL;
}

bool CY_ONOFF_GE::Double_OUT()
{
  if( !Out[0].pTo[0] )
    return false;
  if( Out[1].pTo[0] )
    return true;
  CY_Base * pObj = (*pFindDObj)( Out[0].pTo[0], pDR->NumbFCS, pDR->domain );
  ASSD(pObj);
  if( !pObj )
    return false;
  CY_DI * pDI = NULL;
  int N = 0;
  switch( pObj->Y_Type )
  {
  case id__Z:
  case id__Y:
    {
      pDI = (CY_DI*)pObj; 
      N = pDI->Numb;
      N++;
    }
    break;
  default:
    ASS(FALSE)
      return false;
  }
  if( pDI == NULL || !N )
    return false;
  CY_Base * pObj_1 = (*pFindDI)( pDR->NumbFCS, pDR->domain, N, pDI->Y_Type );
  ASS( pObj && pObj_1 );
  CSignal ** IN1 = pObj_1->GetIN ( "IN" );
  *IN1 = &Out[1];
  Out[1].pTo[0] = pDI->Name;
  return true;
}

void CY_ONOFF_GE::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
    return ;
  }
  else
  {
    Out[0].Status &= ~CSignal::BAD;
  }

  int Status = 0;
  if( IN )
  {
    *IN >> W->PV;
    //Alarm.TestIn( IN, dt, Status );
  }

  if( W->PSW > 0 /*&& !prevPSW*/ )
  {
    W->mode.M = SMode::MAN;
    switch ( W->PSW )
    {
    case 1:
      W->MV = W->MSL;
      break;
    case 2:
      W->MV = W->MSH;
      break;
    default:
      W->MV = W->PMV;
      break;
    }
    W->PSW = 0;//C4.4 Preset Manipulated Output
  }

  if( W->mode.M == SMode::MAN && W->Trk_MAN )
    W->SV = W->PV;

  if( W->mode.M == SMode::AUT )
    W->CSV = W->SV;
  if ( W->mode.M == SMode::RCAS )
    W->SV = W->RSV;
  else
    W->RSV = W->SV;
  if ( W->mode.M == SMode::ROUT )
    W->MV = W->RMV;
  else 
    W->RMV = W->MV;

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
    {
      SET->Status |= CSignal::CND;
    }
    SET->BackVar = W->SV;
  }

  if( SMode::MAN!=W->mode.M && SMode::ROUT!=W->mode.M )
  {
    double En = W->PV - W->SV;
    const double hys = (W->SH - W->SL) / 100 * W->onfHYS;
    double m = -100;
    if( W->PIDD )//Direct
    {
      if( En >= prevEn )
      {
        if( En < (-W->DB + hys) )
          m = 0;
        else if( En >= (-W->DB + hys) && En < W->DB )
          m = 50;
        else if( En >= W->DB )
          m = 100;
      }
      else
      {
        if( En > (W->DB - hys) )
          m = 100;
        else if( En <= (W->DB - hys) && En > -W->DB )
          m = 50;
        else if( En <= -W->DB )
          m = 0;
      }
    }
    else
    {
      if( En >= prevEn )
      {
        if( En < (-W->DB + hys) )
          m = 100;
        else if( En >= (-W->DB + hys) && En < W->DB )
          m = 50;
        else if( En >= W->DB )
          m = 0;
      }
      else
      {
        if( En > (W->DB - hys) )
          m = 0;
        else if( En <= (W->DB - hys) && En > -W->DB )
          m = 50;
        else if( En <= -W->DB )
          m = 100;
      }
    }

    W->MV = m;
    prevEn = En;
  }


  if( W->MV <= 24.5 )
    pinOut0 = 0, pinOut1 = 1;
  else if( W->MV > 24.5 && W->MV <= 74.9 )
    pinOut0 = 0, pinOut1 = 0;
  else if( W->MV > 74.9 )
    pinOut0 = 1, pinOut1 = 0;
}

void CY_ONOFF_GE::Init()
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

int CY_ONOFF_GE::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\ONOFF-GE_P.hpp" 
  return 0;
}

int CY_ONOFF_GE::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  Double_OUT();
  return 0;
}

int CY_ONOFF_GE::SaveState( )
{
  SAVE_OUT
  SUB.SaveState();
  SAVE_CLASS("W",CY_ONOFF_GE_W)
  return 0;
}

int CY_ONOFF_GE::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_OUT
    if ( SUB.RestoreState ( StrName ) == 0 )
      return 0;
  REST_CLASS("W",CY_ONOFF_GE_W)
    return 1;
}

int CY_ONOFF_GE::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 4 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dPV", dPV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "prevEn", prevEn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pinOut0", pinOut0 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pinOut1", pinOut1 );

  return 0;
}

int CY_ONOFF_GE::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 4 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPV", dPV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "prevEn", prevEn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pinOut0", pinOut0 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pinOut1", pinOut1 );

  return 0;
}

int CY_ONOFF_GE::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->MV, "#MV" );
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\ONOFF-GE_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\ONOFF-GE.hpp"
    E_MENU("Все состояние")
    MENU("Входы")
    PARM_IN(IN);
  PARM_IN(SET);
  PARM_IN(BIN);
  PARM_IN(INT);
  PARM( pinOut0, "#OUT" )
    PARM( pinOut1, "#OUT1" )
    PARM_SUB
    E_MENU("Входы")
    MENU("Base")
    CY_Base::GetParams( params );
  E_MENU("Base")
    return 0;
}

bool CY_ONOFF_GE::SetLimits( )
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
