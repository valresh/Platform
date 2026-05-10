#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create VELLIM ( "VELLIM", CY_VELLIM::Create );

CY_VELLIM::CY_VELLIM( CVELLIM * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	SET = NULL;
	INT = NULL;
	OUT.Name = "OUT";
	OUT << W->MV;
	SUB.Name = "SUB";
	SUB.Null();//- в параметрах
	//CSV = 0.;
	dMVn = 0.;
	Alarm.SetBlk( W );
	Alarm.pAlarmLevel = &W->AlarmLevel;
	}

void CY_VELLIM::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_VELLIM,CVELLIM,30,50)

void CY_VELLIM::InitParm()
  {
#include "Yokogawa\VELLIM_P.hpp" 
#include "Yokogawa\VELLIM.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_VELLIM::GetOUT ( char * Name )
	{
  GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_VELLIM::GetIN ( char * Name )
	{
	GET_IN(SET)
	GET_IN(INT)
	return NULL;
	}
	
void CY_VELLIM::Work ( double dt )
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
	if ( W->mode.M == SMode::CAS )
  {
    if( SET )
      *SET >> W->SV;
  }
	/*else
		CSV = W->SV;*/
  if ( W->mode.M == SMode::RCAS )
    W->SV = W->RSV;
  else
    W->RSV = W->SV;
	//W->SV = CSV;

  if ( W->mode.M == SMode::ROUT )
    W->MV = W->RMV;
  else 
    W->RMV = W->MV;

	if ( W->mode.M == SMode::MAN )
		return;//MV задается
	double Scale = ( W->MSH - W->MSL ) / ( W->SSH - W->SSL );
	double MV_new = Scale *	( W->SV - W->SSL ) + W->MSL;
	if ( W->BPSW )
		{
		W->MV = MV_new;
		return;
		}
	double Dmp = W->DMVP * Scale;
	dMVn = MV_new - W->MV;
	if ( dMVn > Dmp )
		{
		dMVn = Dmp;
		Alarm.SEND_ON( A_DVP, Dmp );
		}
	else
		{
		Alarm.SEND_OFF( A_DVP );
		double Dmm = W->DMVM * Scale;
		if ( dMVn < -Dmm )
			{
			dMVn = -Dmm;
			Alarm.SEND_ON( A_DVM, Dmm );
			}
		else
			Alarm.SEND_OFF( A_DVM );
		}
	W->MV = W->MV +	dMVn;
  
  if( W->MV < (W->MSL+0.000001) )
    Alarm.SEND_ON( A_MLO, W->MV );
  else
    Alarm.SEND_OFF( A_MLO );

  if( W->MV > (W->MSH-0.000001) )
    Alarm.SEND_ON( A_MHI, W->MV );
  else
    Alarm.SEND_OFF( A_MHI );
//
	}

void CY_VELLIM::Init( )
{
  Alarm.BlockName = Name;
	switch ( W->Aux )//PV, dPV, MV, DMV
		{
		case 2:
			SUB << W->MV;
		break;
		case 3:
			SUB << dMVn;
		break;
		}
	ASS(SUB.pDbl!=NULL)
}

int CY_VELLIM::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\VELLIM_P.hpp" 
	return 0;
	}

#undef VAR_DBL 
#define VAR_DBL(Var) W->Var = Var;
int CY_VELLIM::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_VELLIM::SaveState( )
	{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_VELLIM_W)
	return 0;
	}

int CY_VELLIM::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_VELLIM_W)
	return 1;
	}

int CY_VELLIM::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->SV, "#SV" );
	PARM ( W->MV, "#MV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\VELLIM_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\VELLIM.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(SET)
	PARM_IN(INT)
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_VELLIM::SetLimits( )
  {
  if ( SET )
    {
    SET ->Min = W->SSL;
    SET ->Max = W->SSH;
    SET ->IsLimits = true;
    }
  if ( !OUT.IsLimits )
    return false;
  W->MSH = W->MH = OUT.Max;
  W->MSL = W->ML = OUT.Min;
  return true;
  }

