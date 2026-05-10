#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create OVR_B( "S_OVR_B", CY_OVR_B::Create );

CY_OVR_B::CY_OVR_B( CS_OVR_B * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  InitData( );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_OVR_B,CS_OVR_B,20,25)

void CY_OVR_B::InitParm()
{
#include <Yokogawa\S_OVR_B.hpp>
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_OVR_B::InitData( )
{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;

  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.SetBlk( W );
  Alarm.pObj = this;
  Alarm.BlockName = Name;
}

void CY_OVR_B::ResetAlarms()
{
  Alarm.SetBlk( W );
}

CSignal * CY_OVR_B::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_OVR_B::GetIN ( char * Name )
{
  ASS( 0 );
  return NULL;
}

void CY_OVR_B::Work ( double dt )
{
  if( W->PV )
    Alarm.SendAlarm( A_ON, true );
  else
    Alarm.SendAlarm( A_ON, false );
}

bool CY_OVR_B::Draw ( CDrawData * pDD, int x0, int y0 )
{
  ASS( 0 );
  return false;
}

void CY_OVR_B::Init( )
{
}

int CY_OVR_B::Prepare0()
{
  return 0;
}

bool CY_OVR_B::SetLimits( )
{
  return true;
}

int CY_OVR_B::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

int CY_OVR_B::SaveState( )
{
  return 0;
}

int CY_OVR_B::RestoreState ( char * StrName )
{
  return 1;
}

int CY_OVR_B::SetData( int TypeData, void * pData )
{
  return 0;
}

int CY_OVR_B::GetParams( KYBridge2SysParam &params )
{
  #include "IO_Parms.h"
  #include "Yokogawa\YPARM_PARM.h" 
  #include "Yokogawa\S_OVR_B.hpp"
  return 0;
}

