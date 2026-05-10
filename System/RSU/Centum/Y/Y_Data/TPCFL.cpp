#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create TPCFL ( "TPCFL", CY_TPCFL::Create );

CY_TPCFL::CY_TPCFL( CTPCFL  * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	IN = NULL;
	Q01 = NULL;
	Q02 = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
  SUB.Name = "SUB";
  SUB.Null();//- в параметрах
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_TPCFL,CTPCFL,30,50)

void CY_TPCFL::InitParm()
  {
#include "Yokogawa\TPCFL_P.hpp" 
#include "Yokogawa\TPCFL.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_TPCFL::GetOUT ( char * Name )
{
	GETOUT
  GETSUB
	return NULL;
}

CSignal ** CY_TPCFL::GetIN ( char * Name )
{
	GET_IN(IN);
	GET_IN(Q01);
	GET_IN(Q02);
	return NULL;
}

void CY_TPCFL::Work ( double dt )
{
  ASSD( 0==W->TPUN );//C;F

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
		*IN >> W->RV;
  if( Q01 )
    *Q01 >> W->TMP;
  if( Q02 )
    *Q02 >> W->PRS;

  double T1  = 1;
  double T1b = 1;

  if( 1==W->PRUN )
  {
    T1  = (W->TMP - 32) * 5./  9.;
    T1b = (W->TB - 32) * 5./  9.;
  }

  double F0 = 1.;
  double Fi = W->RV;

  if( 0==W->CLTP )//TP;P;T
  {
    //HZ0;KGF/CM2;PA;KPA;MPA
    switch( W->PRUN )
    {
    case 1://KGF/CM2
      F0 = sqrt( ((W->PRS+101.324)/(W->PB+101.324)) * ((T1b+273.15)/(T1+273.15)) ) * Fi;
      break;
    case 3://KPA
      F0 = sqrt( ((W->PRS+101.324)/(W->PB+101.324)) * ((W->TB+273.15)/(W->TMP+273.15)) ) * Fi;
      break;
    case 4://MPA
      F0 = sqrt( (( (W->PRS*1000.)+101.324)/(W->PB+101.324)) * ((W->TB+273.15)/(W->TMP+273.15)) ) * Fi;
      break;
    default:
      ASSD(0);
      break;
    }
  }
  else if( 1==W->CLTP )//P
  {
    //HZ0;KGF/CM2;PA;KPA;MPA
    switch( W->PRUN )
    {
    case 1://KGF/CM2
      F0 = sqrt( ((W->PRS+1.0332)/(W->PB+1.0332)) ) * Fi;
      break;
    case 3://KPA
      F0 = sqrt( ((W->PRS+101.324)/(W->PB+101.324)) ) * Fi;
      break;
    case 4://MPA
      F0 = sqrt( (( (W->PRS*1000.)+101.324)/(W->PB+101.324)) )* Fi;
      break;
    default:
      ASSD(0);
      break;
    }
  }
  else if( 2==W->CLTP )//T
  {
    //HZ0;KGF/CM2;PA;KPA;MPA
    switch( W->PRUN )
    {
    case 1://KGF/CM2
      F0 = sqrt( ((T1b+273.15)/(T1+273.15)) ) * Fi;
      break;
    case 3://KPA
      F0 = sqrt( ((W->TB+273.15)/(W->TMP+273.15)) ) * Fi;
      break;
    case 4://MPA
      F0 = sqrt(((W->TB+273.15)/(W->TMP+273.15)) ) * Fi;
      break;
    default:
      ASSD(0);
      break;
    }
  }
  else
  {
    ASSD(0);
  }
  W->CPV = W->GAIN * F0;
}

void CY_TPCFL::Init( )
{
  switch ( W->Aux )
  {
  case 0:
    SUB << W->CPV;
    break;
  case 1:
    SUB << dCPVn;
    break;
  }
  ASS(SUB.pDbl!=NULL)
}

int CY_TPCFL::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\TPCFL_P.hpp" 
	return 0;
	}

int CY_TPCFL::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_TPCFL::SaveState( )
	{
	OUT.SaveState();
	SAVE_CLASS("W",CY_TPCFL_W)
	return 0;
	}

int CY_TPCFL::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_TPCFL_W)
	return 1;
	}

int CY_TPCFL::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->Comment, "#Comment" );
  PARM ( W->CPV, "#CPV" );
#include "IO_Parms.h"
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\TPCFL_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\TPCFL.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_TPCFL::SetLimits( )
  {
  return true;
  }
