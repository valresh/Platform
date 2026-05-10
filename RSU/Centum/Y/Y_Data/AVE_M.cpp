#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create AVE ( "AVE-M", CY_AVE_M::Create );

CY_AVE_M::CY_AVE_M( CAVE_M  * _W ) : CY_Base()
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

LIST_PARM(CY_AVE_M,CAVE_M,30,50)

void CY_AVE_M::InitParm()
{
#include "Yokogawa\AVE_M_P.hpp" 
#include "Yokogawa\AVE_M.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_AVE_M::GetOUT ( char * Name )
{
  GETOUT
  GETSUB
  return NULL;
}

CSignal ** CY_AVE_M::GetIN ( char * Name )
{
  GET_IN(IN)
  return NULL;
}

void CY_AVE_M::Work ( double dt )
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

  dtWait += dt;

  if( W->RST )
  {
    nSamplesReady = 0;
    W->RST = 0;
  }

  if( !nSamplesReady )
  {
    W->PREV = W->CPV;
    W->CPV = W->GAIN * W->RV;
    samplesIn[0] = W->RV;
    ++nSamplesReady;
    dtWait = 0;
    dCPVn = W->CPV - W->PREV;
  }
  else if( dtWait >= W->SMPL )
  {
    dtWait -= W->SMPL;
    MoveMemory( &samplesIn[1], &samplesIn[0], sizeof(samplesIn[0])*(_countof(samplesIn)-1) );
    samplesIn[0] = W->RV;
    ++nSamplesReady;
    if( nSamplesReady > W->NUM )
      nSamplesReady = W->NUM;

    double CPV_new = 0;
    for ( int n=0; n<nSamplesReady; ++n )
    {
      CPV_new += samplesIn[n];
    }
    W->PREV = W->CPV;
    W->CPV = CPV_new * W->GAIN / nSamplesReady;
    dCPVn = W->CPV - W->PREV;
  }

  if( W->CPV < W->SL )
    W->CPV = W->SL;
  if( W->CPV > W->SH )
    W->CPV = W->SH;
}

void CY_AVE_M::Init( )
{
  switch( W->Aux )//PV, dPV, MV, DMV
  {
  case 0:
    SUB << W->CPV;
    break;
  case 1:
    SUB << dCPVn;
    break;
  }
}

int CY_AVE_M::Prepare0()
{
  //YOKO 3
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\AVE_P.hpp" 
  return 0;
}

int CY_AVE_M::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

static LPCSTR s_Name = "W";
int CY_AVE_M::SaveState( )
{
  OUT.SaveState();
  SUB.SaveState();
  SAVE_CLASS(s_Name,CY_AVE_M_W)
  return 0;
}

int CY_AVE_M::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  if ( OUT.RestoreState ( StrName ) == 0 )
    return 0;
  if ( SUB.RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CY_AVE_M_W)
  return 1;
}

int CY_AVE_M::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  //OUT.StateSave( pStateSer );
  //SUB.StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 4 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "samplesIn", samplesIn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dtWait", dtWait );
  WriteFullStruct( pStateSer, eKeyAnyVar, "nSamplesReady", nSamplesReady );
  return 0;
}

int CY_AVE_M::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  //OUT.StateRestore( pStateSer );
  //SUB.StateRestore( pStateSer );
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 4 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "samplesIn", samplesIn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dtWait", dtWait );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "nSamplesReady", nSamplesReady );
  return 0;
}

int CY_AVE_M::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  PARM ( W->CPV, "#CPV" );
  PARM ( W->GAIN, "GAIN" ); //???? - из параметров
  //YOKO 4
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\AVE_M_P.hpp" 
    E_MENU("Все параметры")
    MENU("Все состояние")
#include "Yokogawa\AVE_M.hpp"
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

bool CY_AVE_M::SetLimits( )
{
  LIM(IN)
  if ( !OUT.IsLimits )
    return false;
  return true;
}

