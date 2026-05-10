#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create FF_DI ( "FF-DI", CY_FF_DI::Create );

CY_FF_DI::CY_FF_DI( CFF_DI * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	OUT.Name = "OUT";
  OUT << W->OUT_V;
	}

void CY_FF_DI::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FF_DI,CFF_DI,20,40)

void CY_FF_DI::InitParm()
  {
#include "Yokogawa\FF_DI_P.hpp" 
#include "Yokogawa\FF_DI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_FF_DI::GetOUT ( char * Name )
	{
  GET_IN_NAME( OUT, "PV" )
  GETOUT
	return NULL;
	}

CSignal ** CY_FF_DI::GetIN ( char * Name )
	{
	ASS(0)
	return NULL;
	}

void CY_FF_DI::Work ( double dt )
{
  if( W && (W->FlagUse & CAlarmBase::fuY_Mask) )
  {
    CSignal *outs[] = { &OUT };
    for( int k=0;k<_countof(outs); k++ )
      outs[k]->On( Alarm2SignalFlag(W) );
  }

  W->OUT_V = W->PV;
  //Alarm.TestIn( NULL, dt, OUT.Status );
}

void CY_FF_DI::Init( )
	{
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  Alarm.Possible = A_HHLL|A_HL;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;
  //
  Alarm.pObj = this;
  //
  Alarm.AF = &W->AF;
	}

int CY_FF_DI::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FF_DI_P.hpp" 
	return 0;
	}


int CY_FF_DI::Prepare1()
{
	return 0;
}

int CY_FF_DI::SaveState( )
	{
	OUT.SaveState();
	return 0;
	}

int CY_FF_DI::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
	}

int CY_FF_DI::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->PV, "#PV" );
  if ( pDR && pDR->FCS )
    {
    PARM_CALLBACK( pDR->FCS, "#FCS" );
    } 
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FF_DI_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\FF_DI.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_FF_DI::SetLimits( )
  {
  return true;
  }

