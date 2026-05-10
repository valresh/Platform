#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create AVE ( "AVE-C", CY_AVE_C::Create );

CY_AVE_C::CY_AVE_C( CAVE_C  * _W ) : CY_Base()
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

LIST_PARM(CY_AVE_C,CAVE_C,30,50)

void CY_AVE_C::InitParm()
{
#include "Yokogawa\AVE_C_P.hpp" 
#include "Yokogawa\AVE_C.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_AVE_C::GetOUT ( char * Name )
{
  GETOUT
  GETSUB
  return NULL;
}

CSignal ** CY_AVE_C::GetIN ( char * Name )
{
  GET_IN(IN)
  return NULL;
}

void CY_AVE_C::Work ( double dt )
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
    switch( W->SW )
    {
    case 0:
      W->CPV = 0;
      W->PREV = 0;
      W->Counter = 0;
      break;
    case 1:
      W->Counter++;
      if( W->Counter )
        W->CPV = (W->PREV / W->Counter) * W->GAIN;
      else
        W->CPV = (W->RV) * W->GAIN;
    case 2:
      W->PREV += W->RV;
      break;
    }
    if( W->Counter == 2147483647 )
    {
      W->CPV = 0;
      W->PREV = 0;
      W->Counter = 0;
    }
  }
  else
    OUT.Status |= CSignal::BAD;
  /*if ( K > 0 )
  {
    double CPV_new = W->GAIN * S / K;
    dCPVn = CPV_new - W->CPV;
    W->CPV = CPV_new;
    OUT.Status &= ~CSignal::BAD;
  }
  else
  {
    OUT.Status |= CSignal::BAD;
  }*/
}

void CY_AVE_C::Init( )
{
  switch ( W->AuxDataType )//PV, dPV, MV, DMV
  {
  case 0:
    SUB << W -> CPV;
    break;
  case 1:
    SUB << dCPVn;
    break;
  }
}

int CY_AVE_C::Prepare0()
{
  //YOKO 3
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\AVE_P.hpp" 
  return 0;
}

int CY_AVE_C::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

static LPCSTR s_Name = "W";
int CY_AVE_C::SaveState( )
{
  OUT.SaveState();
  SUB.SaveState();
  SAVE_CLASS(s_Name,CY_AVE_C_W)
  return 0;
}

int CY_AVE_C::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  if ( OUT.RestoreState ( StrName ) == 0 )
    return 0;
  if ( SUB.RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CY_AVE_C_W)
  return 1;
}

int CY_AVE_C::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  //OUT.StateSave( pStateSer );
  //SUB.StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );
  return 0;
}

int CY_AVE_C::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  //OUT.StateRestore( pStateSer );
  //SUB.StateRestore( pStateSer );
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );
  return 0;
}

int CY_AVE_C::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->CPV, "#CPV" );
  PARM ( W->GAIN, "GAIN" ); //???? - из параметров
  //YOKO 4
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\AVE_C_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\AVE_C.hpp"
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

bool CY_AVE_C::SetLimits( )
{
  if ( !OUT.IsLimits )
    return false;
  return true;
}

