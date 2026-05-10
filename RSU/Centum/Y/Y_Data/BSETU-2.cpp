#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create GOV_B( "BSETU-2", CY_BSETU_2::Create );

CY_BSETU_2::CY_BSETU_2( CBSETU_2 * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  InitData( );
}

void CY_BSETU_2::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BSETU_2,CBSETU_2,71,91)

void CY_BSETU_2::InitParm()
{
#include <Yokogawa\BSETU-2.hpp>
#include <Yokogawa\BSETU-2_P.hpp>
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

#include <sbsts.h>
void CY_BSETU_2::OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue )
{
  if( !strcmp(pszParm, "BSTS") )
  {
    LPSTR pEnd = NULL;
    DWORD v = strtoul(pszValue, &pEnd, 10 );
    switch( v )
    {
    case BS_STRT:
      W->BSTS.V = CBSETU_2::_BSTS::STRT;
      break;
    case BS_IBCH:
      W->BSTS.V = CBSETU_2::_BSTS::IBCH;
      break;
    case BS_STUP:
      W->BSTS.V = CBSETU_2::_BSTS::STUP;
      break;
    case BS_STDY:
      W->BSTS.V = CBSETU_2::_BSTS::STDY;
      break;
    case BS_ERLY:
      W->BSTS.V = CBSETU_2::_BSTS::ERLY;
      break;
    case BS_END:
      W->BSTS.V = CBSETU_2::_BSTS::END;
      break;
    case BS_NCNT:
      W->BSTS.V = CBSETU_2::_BSTS::NCNT;
      break;
    case BS_RSET:
      W->BSTS.V = CBSETU_2::_BSTS::RSET;
      break;
    case BS_EMST:
      W->BSTS.V = CBSETU_2::_BSTS::EMST;
      break;
    case BS_EEMS:
      W->BSTS.V = CBSETU_2::_BSTS::EEMS;
      break;
    default:
      ASSD(!"подобрать");
    }
  }
}

void CY_BSETU_2::InitData( )
{
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;

  Alarm.pAlarmLevel = &W->AlarmLevel;
  Alarm.SetBlk( W );
  Alarm.pObj = this;
  Alarm.BlockName = Name;

  IN = NULL;
  OUT.Name = "OUT";
  OUT << W->MV;

  OUT2.Name = "OUT2";
  OUT2 << W->MV;
}

CSignal * CY_BSETU_2::GetOUT( char * Name )
{
  GET_IN(OUT)
  GET_IN(OUT2)
  return NULL;
}

CSignal ** CY_BSETU_2::GetIN( char * Name )
{
  GET_IN(IN)
  GET_IN(INT)
  return NULL;
}

static const double To_h = 1. / 3600.;

void CY_BSETU_2::Work ( double dt )
{
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
    return ;
  }
  else
    OUT.Status &= ~CSignal::BAD;

  if( IN )
    *IN >> W->PV;


  /*if( W->PV )
    Alarm.SendAlarm( A_ON, true );
  else
    Alarm.SendAlarm( A_ON, false );*/

  W->SUM += W->PV * dt * To_h;
}

void CY_BSETU_2::Init( )
{
}

int CY_BSETU_2::Prepare0()
{
  return 0;
}

int CY_BSETU_2::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

int CY_BSETU_2::SaveState( )
{
  return 0;
}

int CY_BSETU_2::RestoreState ( char * StrName )
{
  return 1;
}

int CY_BSETU_2::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
#include <Yokogawa\BSETU-2.hpp>
#include <Yokogawa\BSETU-2_P.hpp>
 return 0;
}
