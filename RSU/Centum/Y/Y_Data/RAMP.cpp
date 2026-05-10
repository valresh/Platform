#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "ReadParms.h"

// D2.13

static CY_Create RAMP ( "RAMP", CY_RAMP::Create );

CY_RAMP_W::CY_RAMP_W()
{
  memset ( this, 0, sizeof ( *this ));
}

CY_RAMP::CY_RAMP( CRAMP * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  IN = NULL;
  OUT.Name = "OUT";
  OUT << W->CPV;
  SUB.Name = "SUB";
  SUB.Null();
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_RAMP,CRAMP,10,20)

void CY_RAMP::InitParm()
{
#include "Yokogawa\RAMP_P.hpp" 
#include "Yokogawa\RAMP.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_RAMP::GetOUT ( char * Name )
{
  GETOUT
  GETSUB
  return NULL;
}

CSignal ** CY_RAMP::GetIN ( char * Name )
{
  GET_IN(IN)
  return NULL;
}

void CY_RAMP::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return;
  }
  if ( IN )
    *IN >> W->RV;

  double CPV_new = W->RV;

  if( CPV_new < W -> SL )
    CPV_new = W -> SL;
  if( CPV_new > W -> SH )
    CPV_new = W -> SH;

  dCPVn = CPV_new - W->CPV;

  CPV_new *= W->GAIN;

  W->CPV = CPV_new;
}

void CY_RAMP::Init( )
{
  switch ( W->Aux )//CPV dCPV
  {
  case 0:
    SUB << W->CPV;
    break;
  case 1:
    SUB << dCPVn;
    break;
  }
}

int CY_RAMP::Prepare0()
{
  W->RV = 0;
  W->CPV = 0;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\RAMP_P.hpp" 
  return 0;
}


int CY_RAMP::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

int CY_RAMP::SaveState( )
{
  OUT.SaveState();
  SUB.SaveState();
  SAVE_CLASS("W",CY_RAMP_W)
  return 0;
}

int CY_RAMP::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  if ( OUT.RestoreState ( StrName ) == 0 )
    return 0;
  if ( SUB.RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS("W",CY_RAMP_W)
  return 1;
}

int CY_RAMP::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );

  return 0;
}

int CY_RAMP::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );

  return 0;
}

int CY_RAMP::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->RV, "#RV" );
  PARM ( W->CPV,"#CPV" );
  PARM ( dCPVn,"#dCPV" );
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\RAMP_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\RAMP.hpp"
    E_MENU("Все состояние")
    MENU("Входы")
    PARM_IN(IN)
    PARM_OUT
    PARM_SUB
    E_MENU("Входы")
    MENU("Base")
    CY_Base::GetParams( params );
  E_MENU("Base")
    return 0;
}

bool CY_RAMP::SetLimits( )
{
  return true;
}

