#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create LD ( "LD", CY_LD::Create );

CY_LD::CY_LD( CLD  * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  IN = NULL;
  OUT.Name = "OUT";
  OUT << W->CPV;
  SUB.Name = "SUB";
  SUB.Null();
}

//YOKO   1
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_LD,CLD,30,50)

void CY_LD::InitParm()
{
#include "Yokogawa\LD_P.hpp" 
#include "Yokogawa\LD.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_LD::GetOUT ( char * Name )
{
  GETOUT
  GETSUB
  if ( strcmp ( Name, "CPV" ) == 0 )
    return &OUT;
  return NULL;
}

CSignal ** CY_LD::GetIN ( char * Name )
{
  GET_IN(IN)
  return NULL;
}

void CY_LD::Work ( double dt )
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
  W->mode.M = SMode::AUT;
  if( IN )
  {
    *IN >> W->RV;
    OUT.Status &= ~CSignal::BAD;
  }
  else
    OUT.Status |= CSignal::BAD;

//   double prevCPV = W->CPV;
//   W->CPV = ( W->GAIN * W->D * (W->RV-prevRV)/dt  + (W->D * prevCPV)/dt ) / (1 + W->D/dt);
//   prevRV = W->RV;
  double dRV = (W->RV - prevRV);
  double prevCPV = W->CPV;
  W->CPV = (W->D / dt * (W->GAIN * dRV + prevCPV)) / (W->D / dt + 1);
  if(fabs(W->CPV) < 1e-5)
	  W->CPV = 0.0;
  prevRV = W->RV;
}

void CY_LD::Init( )
{
}

int CY_LD::Prepare0()
{
  //YOKO 3
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\LD_P.hpp" 
  return 0;
}

int CY_LD::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  prevRV = W->RV;
  return 0;
}

int CY_LD::SaveState( )
{
  OUT.SaveState();
  SUB.SaveState();
  return 0;
}

int CY_LD::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  if ( OUT.RestoreState ( StrName ) == 0 )
    return 0;
  if ( SUB.RestoreState ( StrName ) == 0 )
    return 0;
  return 1;
}

int CY_LD::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->CPV, "#CPV" );
  PARM ( W->GAIN, "GAIN" ); //???? - из параметров
  //YOKO 4
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\LD_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\LD.hpp"
    E_MENU("Все состояние")
    MENU("Входы")
    PARM_IN(IN)
    PARM_OUT
    E_MENU("Входы")
    MENU("Base")
    CY_Base::GetParams( params );
  E_MENU("Base")
    return 0;
}

#undef LIM
#define LIM(IN) \
  if ( IN )\
{\
  IN ->Min = W->SL;\
  IN ->Max = W->SH;\
  IN ->IsLimits = true;\
}

bool CY_LD::SetLimits( )
{
  //LIM(IN)
  if ( !OUT.IsLimits )
    return false;
  return true;
}

