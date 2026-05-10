#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

#define CLASS CY_MLD_SW
#define WORK CY_MLD_SW_W
#define PARMS CY_MLD_SW_P

static CY_Create MLD_SW ( "MLD-SW", CY_MLD_SW::Create );

CY_MLD_SW_W::CY_MLD_SW_W()
{
  ZeroMemory( this, sizeof(*this) );
}

CY_MLD_SW::CY_MLD_SW(CMLD_SW * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	CY_Base::pMode = &W->mode;
	INFO(W->SV,"SV")
	INFO(W->MV,"MV")
//
	SET = NULL;
	TIN = NULL;
	TSI = NULL;
	INT = NULL;
	OUT.Name = "OUT";
	OUT << W->MV;
	SUB.Name = "SUB";
	SUB.Null();//- в параметрах
  W->PMV = 0.;
}

void CY_MLD_SW::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_MLD_SW,CMLD_SW,30,50)

void CY_MLD_SW::InitParm()
  {
#include "Yokogawa\MLD_SW_P.hpp" 
#include "Yokogawa\MLD_SW.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_MLD_SW::GetOUT ( char * Name )
	{
	GETOUT
  GETSUB
	return NULL;
	}

CSignal ** CY_MLD_SW::GetIN ( char * Name )
	{
	GET_IN(SET)
	GET_IN(TIN)
	GET_IN(TSI)
	GET_IN(INT)
	return NULL;
	}

void CY_MLD_SW::Work ( double dt )
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
	if ( OUT.Status & CSignal::OOP )
		Alarm.SEND_ON( A_OOP, -1. );
	else
		Alarm.SEND_OFF( A_OOP );
//
	if ( OUT.Status & CSignal::CND )
		W->mode.IMAN = true;
	else
		W->mode.IMAN = false;
	if ( INT )
		{
		int PV;
		*INT >> PV;
		if ( PV )
			{
			W->mode.M = SMode::MAN;
			W->INTERLOCK = 1;
			}
		else 
			W->INTERLOCK = 0;
		}
	else 
		{
		W->INTERLOCK = 0;
    W->FlagUse |= CAlarmBase::fuYoko;
		}
  double prevMV = W->MV;
	if ( TSI )
	{
		int PV = 0;
		*TSI >> PV;
		if ( PV )
			W->TSW = 1;
		else
			W->TSW = 0;
	}

  if( W->TSW )
  {
    W->mode.TRK = true;
    if( TIN )
      *TIN >> W->MV; 
  }
  else
    W->mode.TRK = false;

  if( W->PSW > 0 )
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
    W->PSW = 0;
  }

  if( PW==W->OutputSignalConvertion && TIN )
    *TIN >> prevMV;

  double prev_dMV = dMV;
  dMV =  W->MV - prevMV;


  if( PW==W->OutputSignalConvertion )
  {
    bool bClear = true;
    if( !(CY_Base::s_nCalcSteps % 3) && abs(dMV)>0.05 )
    {
      double Tout = W->FullStrokeValue * abs(dMV) / 100. ;
      //if( Tout > W->MinOutputWidth )
      {
        if( dMV > 0 )
          pwmUP = 1, pwmDOWN = 0, bClear = false;
        else
          pwmUP = 0, pwmDOWN = 1, bClear = false;
      }
    }
    if( bClear )
      pwmUP = pwmDOWN = 0;
  }

  W->BIAS_Trk = false;
	if ( W->mode.M == SMode::MAN || W->mode.IMAN )
	{
    if( SET )
		  SET->On( CSignal::CND );
//??		if ( W->Out_Trk )
//??			W->MV = OUT.BackVar;
		if ( W->BIAS_Trk )
		{
// Bias trackig
			if( SET )
				*SET >> W->CSV;
			W->SV = ( W->SSH - W->SSL ) /	( W->MSH - W->MSL ) * ( W->MV - W->MSL ) + W->SSL;
			W->BIAS = 0.;//W->SV - W->GAIN * W->CSV;
		}
		else
		{
// Output pushback
      if( SET )
			SET ->On( CSignal::CND );
			W->SV = ( W->SSH - W->SSL ) /	( W->MSH - W->MSL ) * ( W->MV - W->MSL ) + W->SSL;
			W->CSV = ( W->SV - W->BIAS ) / W->GAIN;
	      if( SET )
	        SET->BackVar = W->CSV;
		}
	}
	else
	{
// Для работы CAS или AUT задается в параметрах FCS  
    double SV_new = W->SV;
    if( SET )
    {
      SET->Off( CSignal::CND );
		  SV_new = W->GAIN * W->CSV + W->BIAS;
    }
		double dSV = SV_new - W->SV;
    bool OK = true;
		if ( OUT.Status & CSignal::CLP && W->GAIN > 0. )
		{
      if ( OUT.Status & CSignal::CLPp && dSV > 0. )
      {
// Clamp Output
			  W->CSV = ( W->SV - W->BIAS ) / W->GAIN;
        if( SET )
          SET->BackVar = W->CSV;
        OK = false;
        }
      if ( OUT.Status & CSignal::CLPm && dSV < 0. )
      {
// Clamp Output
			  W->CSV = ( W->SV - W->BIAS ) / W->GAIN;
        if( SET )
          SET->BackVar = W->CSV;
        OK = false;
        }
			}
		if ( OK )
		{
      if( SET )
      {
        *SET >> W->CSV;
//			if ( !W->BIAS_Trk )
//				{
//// Ramp Action
//				if ( dSV > W->RP )
//					dSV = W->RP;
//				if ( dSV < -W->RP )
//					dSV = -W->RP;
//				W->SV += dSV;
//				}
//			else
			  W->SV = W->GAIN * W->CSV + W->BIAS;
			}
		}
		if ( W->SV > W->SSH )
			W->SV = W->SSH;
		if ( W->SV < W->SSL )
			W->SV = W->SSL;
		W->MV = ( W->MSH - W->MSL ) / ( W->SSH - W->SSL ) * ( W->SV - W->SSL ) + W->MSL;
	}
	if( SET )
	{
		if ( W->mode.IMAN || W->mode.M == SMode::MAN )
			SET->On ( CSignal::CND );
		else
			SET->Off ( CSignal::CND );
	}
}

void CY_MLD_SW::Init( )
{
  switch ( W->Aux )//MV;DMV
  {
  case 0:
    SUB << W->MV;
    break;
  case 1:
    SUB << dMV;
    break;
  default:
    ASS(0);
  }
  if( PW==W->OutputSignalConvertion )
    OUT << pwmUP;
}

static int C2 = 2;

int CY_MLD_SW::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\MLD_SW_P.hpp" 
	Alarm.SetBlk( W );
	Alarm.pAlarmLevel = &C2;
  W->PSW = 0;
	return 0;
	}

#undef VAR_DBL 
#define VAR_DBL(Var) W->Var = Var;

int CY_MLD_SW::Prepare1()
{ 
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	if ( W->GAIN == 0. )
		W->GAIN = 1.;
	if ( W->RP == 0. )
		W->RP = W->SSH - W->SSL;
  if( PW==W->OutputSignalConvertion )
  {
    CY_Base * pObj = (*pFindDObj)( OUT.pTo[0], pDR->NumbFCS, pDR->domain );
    ASS( pObj );
    if( pObj )
    {
      CY_DI *pN = GetNextDI( pObj );
      if( pN )
      {
        CY_Base * pObj2 = (*pFindYObjN)( pN->Name, pN->NumbFCS, pN->domain );
        ASS( pObj2 );
        if( pObj2 )
        {
          CSignal ** IN = pObj2->GetIN ( "IN" );
          OUT_DOWN << pwmDOWN;
          *IN = &OUT_DOWN;
          OUT_DOWN.pTo[0] = pN->Name;
          OUT_DOWN.pFrom = Name;
        }
      }
    }
  }
	return 0;
}

int CY_MLD_SW::SaveState( )
	{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_MLD_SW_W)
	return 0;
	}

int CY_MLD_SW::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_MLD_SW_W)
	return 1;
	}

int CY_MLD_SW::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pwmUP", pwmUP );
  WriteFullStruct( pStateSer, eKeyAnyVar, "pwmDOWN", pwmDOWN );

  return 0;
}

int CY_MLD_SW::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pwmUP", pwmUP );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "pwmDOWN", pwmDOWN );

  return 0;
}

int CY_MLD_SW::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODEFULL( W->mode, "#Mode" );
	PARM ( W->MV, "#MV" );
  if ( pDR && pDR->FCS )
    PARM ( pDR->FCS, "#FCS" );
  if( PW==W->OutputSignalConvertion )
  {
    if( TIN && TIN->pDbl )
    {
      PARM( *TIN->pDbl, "#TIN" );
    }
    PARM ( pwmUP, "#UP" )
    PARM ( pwmDOWN, "DOWN" )
  }
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\MLD_SW_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\MLD_SW.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(SET)
	PARM_IN(TIN)
	PARM_IN(TSI)
	PARM_IN(INT)
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	PARM ( W->Tune_MV, "#Тип:MV" )
	return 0;
	}

bool CY_MLD_SW::SetLimits( )
  {
	if ( SET )
		{
		SET ->Min = W->SSL;
		SET ->Max = W->SSH;
		SET ->IsLimits = true;
		}
  if ( OUT.pTo[0] == NULL )
    return true;
  if ( !OUT.IsLimits )
    return false;
  W->MSH = W->MH = OUT.Max;
  W->MSL = W->ML = OUT.Min;
  return true;
  }

