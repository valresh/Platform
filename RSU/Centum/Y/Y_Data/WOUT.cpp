#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create WOUT( "WOUT", CY_WOUT::Create );

CY_WOUT::CY_WOUT( CWOUT * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = NULL;
  Q01 = NULL;
  Q02 = NULL;
  OUT.Name = "OUT";
  OUT << W->CPV;
  //State = 0;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_WOUT,CWOUT,1,11)

void CY_WOUT::InitParm()
{
#include "Yokogawa\WOUT.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_WOUT::GetOUT ( char * Name )
{
  GETOUT
  return NULL;
}

CSignal ** CY_WOUT::GetIN ( char * Name )
{
  GET_IN(Q01)
  GET_IN(Q02)
  return NULL;
}

void CY_WOUT::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
#pragma message( "!!!!! должен работать периодически или по пинку\n")
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
    return ;
  }
  else
    OUT.Status &= ~CSignal::BAD;

  if( !Q01 || !Q02 )
  {
    OUT.Status |= CSignal::BAD;
    return;
  }

  bool BadRV2 = false;
  bool BadRV1 = false;
  if( Q01->Status & CSignal::BAD )
    BadRV1 = true;
  if( Q02->Status & CSignal::BAD )
    BadRV2 = true;
  *Q01 >> W->RV1;
  *Q02 >> W->RV2;
  W->CPV = W->RV1 && !W->RV2;
}

void CY_WOUT::Init( )
{
}

int CY_WOUT::Prepare0()
{
//#include "Yokogawa\YPARM_TEST.h" 
//#include "Yokogawa\WOUT_P.hpp" 
  return 0;
}


int CY_WOUT::Prepare1()
{
  //W->RV2 = W->RV1 = W->CPV = 0;//???
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

int CY_WOUT::SaveState( )
{
  OUT.SaveState();
  return 0;
}

int CY_WOUT::RestoreState ( char * StrName )
{
  if ( OUT.RestoreState ( StrName ) == 0 )
    return 0;
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  return 1;
}

int CY_WOUT::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
  /*MENU("Все параметры")
#include "Yokogawa\WOUT_P.hpp" 
    E_MENU("Все параметры")*/
    MENU("Все состояние")
#include "Yokogawa\WOUT.hpp"
    E_MENU("Все состояние")
    MENU("Входы")
    PARM_IN(Q01)
    PARM_IN(Q02)
    PARM_OUT
    E_MENU("Входы")
    MENU("Base")
    CY_Base::GetParams( params );
  E_MENU("Base")
    return 0;
}

bool CY_WOUT::SetLimits( )
{
  return true;
}
