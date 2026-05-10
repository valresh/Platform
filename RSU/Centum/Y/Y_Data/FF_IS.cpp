#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create FF_IS ( "FF-IS", CY_FF_IS::Create );

CY_FF_IS::CY_FF_IS( CFF_IS * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  OUT.Name = "OUT";
  OUT << W->CPV;
  SEL.Name = "SEL";
  SEL << W->SEL_V;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FF_IS,CFF_IS,20,40)

void CY_FF_IS::InitParm()
{
#include "Yokogawa\FF_IS_P.hpp" 
#include "Yokogawa\FF_IS.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_FF_IS::GetOUT ( char * Name )
{
  GETOUT
  if( strcmp ( SEL.Name, Name ) == 0 )
    return &SEL;
  return NULL;
}

CSignal ** CY_FF_IS::GetIN ( char * Name )
{
  GET_IN(IN1)
  GET_IN(IN2)
  GET_IN(IN3)
  GET_IN(IN4)
  GET_IN(OSEL)
  GET_IN(DIS1)
  GET_IN(DIS2)
  GET_IN(DIS3)
  GET_IN(DIS4)
  return NULL;
}

static double GetMin( double *INX1, int &indexMin )
{
  double min = *INX1;
  for( int i=1; i<4; ++i )
  {
    if( INX1[i] < min )
    {
      min = INX1[i];
      indexMin = i+1;
    }
  }
  return min;
}

static double GetMax( double *INX1, int &indexMin )
{
  double max = *INX1;
  for( int i=1; i<4; ++i )
  {
    if( INX1[i] > max )
    {
      max = INX1[i];
      indexMin = i+1;
    }
  }
  return max;
}

void CY_FF_IS::Work ( double dt )
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

  if( IN1 )
    *IN1 >> W->IN1_V;
  if( IN2 )
    *IN2 >> W->IN2_V;
  if( IN3 )
    *IN3 >> W->IN3_V;
  if( IN4 )
    *IN4 >> W->IN4_V;

  if( DIS1 )
    *DIS1 >> W->DIS1_V;
  if( DIS2 )
    *DIS2 >> W->DIS2_V;
  if( DIS3 )
    *DIS3 >> W->DIS3_V;
  if( DIS4 )
    *DIS4 >> W->DIS4_V;

  if( OSEL )
    *OSEL >> W->OSEL_V;

  if( SMode::AUT==W->mode.M )
  {
  }
  else if( SMode::MAN==W->mode.M )
  {
  }

  switch( W->OSEL_V )
  {
  case 1:
    W->CPV = W->IN1_V;
    W->SEL_V = W->OSEL_V;
    break;
  case 2:
    W->CPV = W->IN2_V;
    W->SEL_V = W->OSEL_V;
    break;
  case 3:
    W->CPV = W->IN3_V;
    W->SEL_V = W->OSEL_V;
    break;
  case 4:
    W->CPV = W->IN4_V;
    W->SEL_V = W->OSEL_V;
    break;
  case 0:
    switch( W->SEL_TYPE )
    {
    case 1:
      W->SEL_V = 1;
      W->CPV = W->IN1_V;
      break;
    case 2:
      {
        int index = 1;
        W->CPV = GetMin( &W->IN1_V, index );
        W->SEL_V = index;
      }
      break;
    case 3:
      {
        int index = 1;
        W->CPV = GetMax( &W->IN1_V, index );
        W->SEL_V = index;
      }
      break;
    case 4:
      {
        W->SEL_V = 0;
        int indexMin = 1, indexMax = 1;
        GetMin( &W->IN1_V, indexMin );
        GetMax( &W->IN1_V, indexMax );
        double *add = &W->IN1_V;
        double avg = 0;
        for( int i=1; i<5; ++i, add++ )
        {
          if( indexMin==i )
            continue;
          if( indexMax==i )
            continue;
          avg += *add;
        }
        W->CPV = avg;
      }
      break;
    case 5:
      W->SEL_V = 4;
      W->CPV = ( W->IN1_V + W->IN2_V + W->IN3_V + W->IN4_V ) / 4;
      break;
    }
    break;
  }

}

void CY_FF_IS::Init( )
{
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
}

int CY_FF_IS::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FF_IS_P.hpp" 
  return 0;
}


int CY_FF_IS::Prepare1()
{
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::CAS;
  return 0;
}
int CY_FF_IS::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  return 0;
}

int CY_FF_IS::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  return 0;
}

int CY_FF_IS::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODE ( W->mode, "#Mode" );
  if ( pDR && pDR->FCS )
  {
    PARM_CALLBACK( pDR->FCS, "#FCS" );
  } 
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FF_IS_P.hpp" 
  E_MENU("Все параметры")
  MENU("Все состояние")
#include "Yokogawa\FF_IS.hpp"
  E_MENU("Все состояние")
  MENU("Входы")
  PARM_OUT
  PARM_IN(IN1)
  PARM_IN(IN2)
  PARM_IN(IN3)
  PARM_IN(IN4)
  E_MENU("Входы")
  MENU("Base")
  CY_Base::GetParams( params );
  E_MENU("Base")
  return 0;
}

bool CY_FF_IS::SetLimits( )
{
  return true;
}
