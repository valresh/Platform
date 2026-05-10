#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <SR_DCU.h>
#include "CalcVar.h"
#include "GET_DATA.h"
#include "GlobalSettings.h"

static CY_Create CALCU_C( "CALCU-C", CY_CALCU_C::Create );

////////////////////////////////////////////////////////

CY_CALCU_C::CY_CALCU_C( CCALCU_C * _W )
{
  Type = Character;
  W = _W;
  INIT(EERL)
  INIT(ERRC)
    //
  IN = NULL;
  Q01 = NULL;
  Q02 = NULL;
  Q03 = NULL;
  Q04 = NULL;
  Q05 = NULL;
  Q06 = NULL;
  Q07 = NULL;
  //
  Start = true;
  BIAS = 1.;
  //-------------------------------
  CY_Base::pMode = &W->mode;
  CY_Base::InitFlagUse( W->FlagUse );

  kOut = 0;
  SET_OUT("OUT",W->CPV)
  SET_OUT("J01",W->CPV1)
  SET_OUT("J02",W->CPV2)
  SET_OUT("J03",W->CPV3)
  CY_Base::pComm = W->Comment;
  UseAlg = 0;
  bSaveP0StoState = false;

  Alarm.Possible = A_CERR;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pObj = this;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_CALCU_C,CCALCU_C,40,60)

void CY_CALCU_C::InitParm()
{
#include "Yokogawa\CALCU_C_P.hpp" 
#include "Yokogawa\CALCU_C.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_CALCU_C::GetOUT ( char * Name )
{
  GET_STD_OUT
  return NULL;
}

CSignal ** CY_CALCU_C::GetIN ( char * Name )
{
  GET_IN(IN)
  GET_IN(Q01)
  GET_IN(Q02)
  GET_IN(Q03)
  GET_IN(Q04)
  GET_IN(Q05)
  GET_IN(Q06)
  GET_IN(Q07)
  return NULL;
}

#define GET_STATUS(IN,VAL,Bad) \
  if ( IN )\
{\
  if ( IN ->Status & CSignal::BAD )\
  Bad = true;\
}

void CY_CALCU_C::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
  
  if( 0==UseAlg && !W->CALI )
    SetSkipAllOutTerminals();

  if( eOne_shotProcessing==W->ProcessingTiming )
  {
    if( !W->ACT )
      return;
    W->ACT = 0;
  }
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
    return ;
  }
  else
    Out[0].Status &= ~CSignal::BAD;

  if( PeriodicExecution==W->ProcessingTiming )
  {
    if( 0==dt )
      return;//вызов, например, из ST16
    if( nStepDuration > 0 )
    {
      if( m_nBasicControlPeriodCounter < nStepDuration )
        return;
      m_nBasicControlPeriodCounter = 0;
    }
  }

  bool BadRV = false;
  bool BadRVn = false;
  GET_STATUS(IN,RV,BadRV)
  GET_STATUS(Q01,RV1,BadRVn)
  GET_STATUS(Q02,RV2,BadRVn)
  GET_STATUS(Q03,RV3,BadRVn)
  GET_STATUS(Q04,RV4,BadRVn)
  GET_STATUS(Q05,RV5,BadRVn)
  GET_STATUS(Q06,RV6,BadRVn)
  GET_STATUS(Q07,RV7,BadRVn)
  //Входы
  bool Calculation = true;
  switch ( W->Calc_input_err_detect )//"%{NO;CALC;ALL;}"
  {
  case 0://NO
    break;
  case 1://CALC
    {
      if ( BadRV )
      {
        Calculation = false;
        Out[0].Status |= CSignal::BAD;
        break;
      }
      if ( BadRVn )
      {
        Out[0].Status |= CSignal::QST;
        break;
      }
      Out[0].Status &= ~(CSignal::QST|CSignal::BAD);
    }
    break;
  case 2://ALL
    {
      if ( BadRV || BadRVn )
      {
        Calculation = false;
        Out[0].Status |= CSignal::BAD;
        break;
      }
      Out[0].Status &= ~CSignal::BAD;
    }
    break;
  }

  if( IN )
  {
    double RV = 0.;
    GET_DATA(IN,RV,BadRV);
    W->RV = RV * BIAS;
  }
  GET_DATA(Q01,W->RV1,BadRVn);
  GET_DATA(Q02,W->RV2,BadRVn);
  GET_DATA(Q03,W->RV3,BadRVn);
  GET_DATA(Q04,W->RV4,BadRVn);
  GET_DATA(Q05,W->RV5,BadRVn);
  GET_DATA(Q06,W->RV6,BadRVn);
  GET_DATA(Q07,W->RV7,BadRVn);
  W->mode.M = SMode::AUT;
  if ( Out[0].Status & CSignal::CND )
  {
    W->mode.IMAN = true;
    // Track output
  }
  else
  {
    W->mode.IMAN = false;
  }
  if ( !Calculation )
    return;
  SET_HBP
  {
    KKK();
  }
  // CPV overshoot не реализован (p64)
  double prevСPVs[] = { W->CPV, W->CPV1 };
  switch ( UseAlg )
  {
  case -1:
    break;
  case 0:
    if( cVars || cOPs )
      WorkTranslation( nStepDuration, W->mode );
    if( W->CALI )
    {
      W->CPV = prevСPVs[0];
      W->CPV1 = prevСPVs[1];
    }
    break;
  case 1:
    W->CPV = W->RV1;
    break;
  case 2:
    W->CPV = W->RV2;
    break;
  case 3:
    W->CPV = W->RV3;
    break;
  case 11:
    W->CPV = W->RV;
    break;
  }

  CAlarmBase::tFlagUse_ *pF = GetFlagUse();
  if( !pF )
    return;
  CAlarmBase::tFlagUse_ &src = *pF;
  for( int i=0; i<cVars; i++ )
  {
    KCalcVar &cv = ((KCalcVar*)pVars)[i];
    if( !cv.m_Obj )
      continue;
    CAlarmBase::tFlagUse_ *pF1 = cv.m_Obj->GetFlagUse();
    if( !pF1 )
      continue;
    CAlarmBase::tFlagUse_ &dst = *pF1;
    if( !(src&CAlarmBase::fuYConnect) && !(dst&CAlarmBase::fuYConnect) )
      continue;
    dst |= CAlarmBase::fuYConnect;
    src |= CAlarmBase::fuYConnect;
  }
}

void CY_CALCU_C::Init( )
{
}

int CY_CALCU_C::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\CALCU_C_P.hpp" 
  return 0;
}

struct SOutSignalName2Var
{
  LPCSTR pszOsignal;
  LPCSTR pszVar;
};

static const SOutSignalName2Var s_OutSig2Var[] = 
{
  {"OUT","CPV"},
  {"J01","CPV1"},
  {"J02","CPV2"},
  {"J03","CPV3"},
};

int CY_CALCU_C::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  InitTrans();

  for( int i=0; i<cVars; i++ )
  {
    KCalcVar &cv = ((KCalcVar*)pVars)[i];
    for( int j=0; j<_countof(s_OutSig2Var); ++j )
    {
      if( strcmp( s_OutSig2Var[j].pszVar, cv.NameValue ) )
        continue;
      for( int s=0; s<kOut; ++s )
      {
        if( strcmp( Out[s].Name, s_OutSig2Var[j].pszOsignal ) )
          continue;
        switch( Out[s].Name[0] )
        {
        case 'S':
          break;
        case 'O':
        case 'J':
          cv.m_pSignal = &Out[s];
          break;
        }
      }
    }
  }
  if( 0==UseAlg && !W->CALI )
    SetSkipAllOutTerminals();

  return 0;
}

int CY_CALCU_C::SaveState( )
{
  SAVE_OUT
  SAVE_CLASS("W",CY_CALCU_W)
  __super::SaveState();
  if( bSaveP0StoState || 1==KGlobalSettings::Instance().ExtendState() )
  {
    SAVE_FIELD( P01 );
    SAVE_FIELD( P02 );
    SAVE_FIELD( P03 );
    SAVE_FIELD( P04 );
    SAVE_FIELD( P05 );
    SAVE_FIELD( P06 );
    SAVE_FIELD( P07 );
    SAVE_FIELD( P08 );
  }
  return 0;
}

int CY_CALCU_C::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_OUT
  REST_CLASS("W",CY_CALCU_W)
  int r = __super::RestoreState( StrName );
  if( !r )
    return 0;
  REST_FIELD( P01 );
  REST_FIELD( P02 );
  REST_FIELD( P03 );
  REST_FIELD( P04 );
  REST_FIELD( P05 );
  REST_FIELD( P06 );
  REST_FIELD( P07 );
  REST_FIELD( P08 );
  return 1;
}

int CY_CALCU_C::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  return 0;
}

int CY_CALCU_C::StateRestore( IYokoStateSer* pStateSer )
{
  int r = __super::StateRestore( pStateSer );
  if( r )
    return r;
  DWORD key = 0;
  pStateSer->SimpleRead( key );
  if( emkcCalcuPsave!=key )
  {
    pStateSer->Shift( -LONG(sizeof(key)) );
    return 0;
  }
  pStateSer->SimpleRead( W->P01 );
  pStateSer->SimpleRead( W->P02 );
  pStateSer->SimpleRead( W->P03 );
  pStateSer->SimpleRead( W->P04 );
  pStateSer->SimpleRead( W->P05 );
  pStateSer->SimpleRead( W->P06 );
  pStateSer->SimpleRead( W->P07 );
  pStateSer->SimpleRead( W->P08 );
  return 0;
}

//......................................................................
int CY_CALCU_C::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->CPV, "#CPV" );
  PARM ( BIAS, "BIAS" )
  //PARM ( UseAlg, "Способ расчета" )
  {
    static SYParam::SYParamStrVal vals[] = 
    {
      {"Пропускать", -1},
      {"Честно", 0},
      {"CPV = RV1", 1},
      {"CPV = RV2", 2},
      {"CPV = RV3", 3},
      {"CPV = RV", 11},
    };
    SYParam &added = params.Add( *(unsigned int*)&UseAlg, "Способ расчета", _countof(vals), vals );
    added.bFullEqValue = true;
  }
  PARM( bSaveP0StoState, "Сохранять P01-P08 в состоянии")
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\CALCU_C_P.hpp" 
  E_MENU("Все параметры")
  MENU("Все состояние")
#include "Yokogawa\CALCU_C.hpp"
  E_MENU("Все состояние")
  MENU("Входы")
  PARM_IN(IN)
  PARM_IN(Q01)
  PARM_IN(Q02)
  PARM_IN(Q03)
  PARM_IN(Q04)
  PARM_IN(Q05)
  PARM_IN(Q06)
  PARM_IN(Q07)
  PARM_ALL_OUT
  E_MENU("Входы")
  MENU("Base")
  __super::GetParams( params );
  E_MENU("Base")
  return 0;
}

bool CY_CALCU_C::SetLimits( )
{
  if ( IN )
  {
    IN->Min = W->SL;
    IN->Max = W->SH;
    IN->IsLimits = true;
  }
  return true;
}
