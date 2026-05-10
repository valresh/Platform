#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create VEL( "S_VEL", CY_VEL::Create );

CY_VEL::CY_VEL( CS_VEL * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  InitData( );
}

#include "Yokogawa\YPARM_INIT.h" 
LIST_PARM(CY_VEL,CS_VEL,4,17)

void CY_VEL::InitParm()
{
#include <Yokogawa\S_VEL.hpp>
#include <Yokogawa\S_VEL_P.hpp>
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_VEL::InitData( )
{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;

  Alarm.Possible = A_VEL;
  Alarm.SetBlk( W );
  Alarm.BlockName = Name;
  Alarm.pAlarmLevel = &W->AlarmLevel;

  Alarm.pObj = this;
}

void CY_VEL::ResetAlarms()
{
  Alarm.SetBlk( W );
}

CSignal * CY_VEL::GetOUT ( char * Name )
	{
  return NULL;
	}

CSignal ** CY_VEL::GetIN ( char * Name )
	{
	ASS( 0 );
  return NULL;
	}

void CY_VEL::Work ( double dt )
{
  if( !W->VELP )
    Alarm.SEND_OFF( A_VELP );
  if( W->VELP )
    Alarm.SEND_ON( A_VELP, W->VL );

  if( !W->VELM )
    Alarm.SEND_OFF( A_VELM );
  if( W->VELM )
    Alarm.SEND_ON( A_VELM, W->VL );
}

bool CY_VEL::Draw ( CDrawData * pDD, int x0, int y0 )
  {
  ASS( 0 );
	return false;
  }

void CY_VEL::Init( )
	{
	}

int CY_VEL::Prepare0()
{
  return 0;
}

bool CY_VEL::SetLimits( )
	{
  return true;
	}

int CY_VEL::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	return 0;
}

int CY_VEL::SaveState( )
	{
	return 0;
	}

int CY_VEL::RestoreState ( char * StrName )
	{
	return 1;
	}

int CY_VEL::SetData( int TypeData, void * pData )
  {
  return 0;
  }

int CY_VEL::GetParams( KYBridge2SysParam &params )
{
  #include "IO_Parms.h"
  #include "Yokogawa\YPARM_PARM.h" 
  #include "Yokogawa\S_VEL.hpp"
  MENU("Параметры")
  #include "Yokogawa\S_VEL_P.hpp"
  E_MENU("Параметры")
	return 0;
}

