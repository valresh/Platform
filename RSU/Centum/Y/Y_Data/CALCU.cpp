#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include <_CALCU.h>
#include <SR_DCU.h>
#include "CalcVar.h"
#include "GET_DATA.h"
#include "GlobalSettings.h"

static CY_Create CALCU ( "CALCU", CY_CALCU::Create );

bool CY_CALCU::s_globalUseInGainBias = true;

////////////////////////////////////////////////////////

CY_CALCU::CY_CALCU( CCALCU * _W )
{
  W = _W;
	// to P08 Calculation parameters 1 to 8 ----- 0
	INIT(EERL)// Error statement number ----- 0
	INIT(ERRC)// (* 2) Error code ----- 0
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
//-------------------------------
  CY_Base::pMode = &W->mode;
  CY_Base::InitFlagUse( W->FlagUse );
//
	INFO(W->RV,"RV")
	INFO(W->CPV,"CPV")
//
  kOut = 0;
  Out[kOut].Name = "SUB";
  Out[kOut].pFrom = Name;
  Out[kOut] << dCPV;
  kOut++;
	SET_OUT("OUT",W->CPV)
	SET_OUT("J01",W->CPV1)
	SET_OUT("J02",W->CPV2)
	SET_OUT("J03",W->CPV3)
//	Alg = 1;
	CLEAR( PID )
	CLEAR( SEL )
  CY_Base::pComm = W->Comment;
  pAlg = NULL;
  UseAlg = 0;
  bSaveP0StoState = false;
  UseIn = 3;

  Alarm.Possible = A_CERR;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pObj = this;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_CALCU,CCALCU,41,61)

void CY_CALCU::InitParm()
  {
#include "Yokogawa\CALCU_P.hpp" 
#include "Yokogawa\CALCU.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_CALCU::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_CALCU::GetIN ( char * Name )
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

static const double To_Day = 1. / (3600. * 24.);
static const double To_h = 1. / 3600.;
static const double To_min = 1. / 60.;
static const double To_Sec = 1.;

void CY_CALCU::Work ( double dt )
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
    m_stepActOn = CY_Base::s_nCalcSteps;
  }

	if ( W->mode.M != SMode::AUT )
    {
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
      dt = nStepDuration;
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
    double _gain = 1, _bias = 0;
    if( s_globalUseInGainBias )
    {
      if( 4==W->InputSignalConversion )
      {
        if( UseIn & 1 )
          _gain = W->Data_GAINin;

        if( UseIn & 2 )
          _bias = W->Data_BIASin;
      }
    }
    W->RV = RV * _gain + _bias;//W->RV = RV * BIAS;
  }

	GET_DATA(Q01,W->RV1,BadRVn);
	GET_DATA(Q02,W->RV2,BadRVn);
	GET_DATA(Q03,W->RV3,BadRVn);
	GET_DATA(Q04,W->RV4,BadRVn);
	GET_DATA(Q05,W->RV5,BadRVn);
	GET_DATA(Q06,W->RV6,BadRVn);
	GET_DATA(Q07,W->RV7,BadRVn);
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
  double prevСPVs[] = { W->CPV, W->CPV1, W->CPV2, W->CPV3 };
  switch ( UseAlg )
    {
    case -1:
    break;
    case 0:
      if ( pAlg )
        pAlg->Work( W );
      else if( cVars || cOPs )
        WorkTranslation( nStepDuration, W->mode );
      if( W->CALI )
      {
        W->CPV = prevСPVs[0];
        W->CPV1 = prevСPVs[1];
        W->CPV2 = prevСPVs[2];
        W->CPV3 = prevСPVs[3];
      }
      if( W->CPV_RangeLimit )
      {
        if( W->CPV < W->SL )
          W->CPV = W->SL;
        if( W->CPV > W->SH )
          W->CPV = W->SH;
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
    case 14:
      W->CPV3 = W->RV3;
    case 13:
      W->CPV2 = W->RV2;
    case 12:
      W->CPV1 = W->RV1;
    case 11:
      W->CPV = W->RV;
      break;
    case 5:
      W->CPV = W->RV;
      W->CPV1 = W->RV1;
      W->CPV2 = W->RV2;
      W->CPV3 = W->RV3;
      break;
    }

  dCPV = W->CPV - prevСPVs[0];

  switch( W->TotalizerTimeUnit )
  {
  default:
  case 0:
    break;
  case 1:
    W->SUM += W->CPV * dt * To_Sec;
    break;
  case 2:
    W->SUM += W->CPV * dt * To_min;
    break;
  case 3:
    W->SUM += W->CPV * dt * To_h;
    break;
  case 4:
    W->SUM += W->CPV * dt * To_Day;
    break;
  }

  if( PeriodicExecution==W->ProcessingTiming )
  {
    if( abs(W->CPV) < 0.000001 )
      W->ACT = 0;
    else
    {
      if( W->ACT && prevСPVs[0] != W->CPV )
        W->ACT++;
      else
        W->ACT = 1;
      m_stepActOn = CY_Base::s_nCalcSteps;
    }
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

void CY_CALCU::Init( )
	{
    switch( W->Aux )//PV, dPV, MV, DMV
    {
    case 0:
      Out[0] << W -> CPV;
      break;
    case 1:
      Out[0] << dCPV;
      break;
    }
	}

#undef VAR_DBL 
#define VAR_DBL(Var) W->Var = Var;

int CY_CALCU::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\CALCU_P.hpp" 
  //W->mode.M = SMode::AUT;
	return 0;
}

struct SSignal2Var
{
  LPCSTR pszVar;
  CSignal *pSignal;
};

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

int CY_CALCU::Prepare1()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\CALCU_P.hpp" 

  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  InitTrans();
  pAlg = CAlg::Find( Name );
  CRefAny::InitAll();
  CCond::InitAll();

  SSignal2Var _sv[] = 
  {
    "RV", IN,
    "RV1", Q01,
    "RV2", Q02,
    "RV3", Q03,
    "RV4", Q04,
    "RV5", Q05,
    "RV6", Q06,
    "RV7", Q07,
  };

  for( int s=0; s<_countof(_sv); ++s )
  {
    if( !_sv[s].pSignal )
      continue;
    if( CSignal::Int!=_sv[s].pSignal->Type )
      continue;

    for( int i=0; i<cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)pVars)[i];
      if( !_strcmpi(cv.NameValue, _sv[s].pszVar) )
      {
        cv.m_TypeVar = ns_CALCU::IntFromReal;
        break;
      }
    }
  }

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

/*#define S_FIELD( Field ) pSRFile->WStruct( #Field, sizeof(W->Field), &W->Field );
#define R_FIELD( Field ) \
  if ( lstrcmp ( StrName, #Field ) == 0 )\
  {\
    ASS(pSRFile->RStruct( sizeof( W->Field ), &W->Field ));\
    return 0;\
  }*/

int CY_CALCU::SaveState( )
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

int CY_CALCU::RestoreState ( char * StrName )
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

int CY_CALCU::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  if( bSaveP0StoState || 1==KGlobalSettings::Instance().ExtendState() )
  {
    DWORD key = emkcCalcuPsave;
    pStateSer->SimpleWrite( key );
    pStateSer->SimpleWrite( W->P01 );
    pStateSer->SimpleWrite( W->P02 );
    pStateSer->SimpleWrite( W->P03 );
    pStateSer->SimpleWrite( W->P04 );
    pStateSer->SimpleWrite( W->P05 );
    pStateSer->SimpleWrite( W->P06 );
    pStateSer->SimpleWrite( W->P07 );
    pStateSer->SimpleWrite( W->P08 );
  }
  return 0;
}

int CY_CALCU::StateRestore( IYokoStateSer* pStateSer )
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
  if( bSaveP0StoState || 1==KGlobalSettings::Instance().ExtendState() )
  {
    pStateSer->SimpleRead( W->P01 );
    pStateSer->SimpleRead( W->P02 );
    pStateSer->SimpleRead( W->P03 );
    pStateSer->SimpleRead( W->P04 );
    pStateSer->SimpleRead( W->P05 );
    pStateSer->SimpleRead( W->P06 );
    pStateSer->SimpleRead( W->P07 );
    pStateSer->SimpleRead( W->P08 );
  }
  else
    pStateSer->Shift( sizeof(W->P01) * 8 );
  return 0;
}

//......................................................................
int CY_CALCU::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->CPV, "#CPV" );
  //PARM ( UseAlg, "Способ расчета" )
  {
    static SYParam::SYParamStrVal vals[] = 
    {
      {"Пропускать", -1},
      {"Честно", 0},
      {"CPV = RV1", 1},
      {"CPV = RV2", 2},
      {"CPV = RV3", 3},
      {"CPV3 = RV3", 14},
      {"CPV2 = RV2", 13},
      {"CPV1 = RV1", 12},
      {"CPV = RV", 11},
      //{"CPV=RV,CPV1=RV1,CPV2=RV2,CPV3=RV3", 5},
    };
    SYParam &added = params.Add( *(unsigned int*)&UseAlg, "Способ расчета", _countof(vals), vals );
    added.bFullEqValue = true;
  }
  /*if( pAlg )
  {
    MENU("Алгоритм")
    pAlg->GetParam( StrName );
    E_MENU("Алгоритм")
  }*/
  PARM ( UseIn, "Использовать локальный Gain(1)Bias(2)" )
  PARM ( s_globalUseInGainBias, "Использовать глобальный InGainBias")
  PARM( bSaveP0StoState, "Сохранять P01-P08 в состоянии")
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\CALCU_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\CALCU.hpp"
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
  PARM( m_stepActOn, "#Шаг ACT.ON")
	E_MENU("Base")
	return 0;
	}

bool CY_CALCU::SetLimits( )
{
	if ( IN )
		{
		IN ->Min = W->SL;
		IN ->Max = W->SH;
		IN ->IsLimits = true;
		}
//  if ( Out[0].pTo[0] == NULL )
//    return true;
//  if ( !Out[0].IsLimits )
//    return false;
  return true;
}

void CY_CALCU::OnAssigned( void *pAddr )
{
  if( pAddr == &W->ACT && W->ACT )
  {
    int prevnStepDuration = nStepDuration;
    nStepDuration = -1;
    m_stepActOn = CY_Base::s_nCalcSteps;
    Work( 0.36 );
    nStepDuration = prevnStepDuration;
  }
}
