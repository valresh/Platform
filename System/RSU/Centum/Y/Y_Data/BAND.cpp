#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.31
static CY_Create BAND ( "BAND", CY_BAND::Create );

CY_BAND::CY_BAND( CBAND * _W ) : CY_Base()
	{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	Q01 = NULL;
	Q02 = NULL;
	OUT.Name = "OUT";
  OUT << W->CPV;
	}

//YOKO   1
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BAND,CBAND,30,50)

void CY_BAND::InitParm()
  {
#include "Yokogawa\BAND_P.hpp" 
#include "Yokogawa\BAND.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_BAND::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_BAND::GetIN ( char * Name )
	{
	GET_IN(Q01)
  GET_IN(Q02)
	return NULL;
	}

void CY_BAND::Work ( double dt )
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
  iGetIN();
  OUT.Set( CSignal::BAD | CSignal::QST, OutStatus );
  W->RV1 = iRV1;
  W->RV2 = iRV2;
  if ( OutStatus == CSignal::BAD )
    return ;
  W->CPV = iRV1 & iRV2;
	}

void CY_BAND::Init( )
	{
	}

int CY_BAND::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\BAND_P.hpp" 
	return 0;
	}

int CY_BAND::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

static LPCSTR s_Name = "W";
int CY_BAND::SaveState( )
{
	OUT.SaveState();
  SAVE_CLASS(s_Name,CY_BAND_W)
	return 0;
}

int CY_BAND::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS(s_Name,CY_BAND_W)
	return 1;
}

int CY_BAND::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  //OUT.StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "iRV1", iRV1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "iRV2", iRV2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OutStatus", OutStatus );
  return 0;
}

int CY_BAND::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  //OUT.StateRestore( pStateSer );
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "iRV1", iRV1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "iRV2", iRV2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OutStatus", OutStatus );
  return 0;
}

int CY_BAND::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
  W_PARM(RV1)
  W_PARM(RV2)
	W_PARM(CPV)
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\BAND_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\BAND.hpp"
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

bool CY_BAND::SetLimits( )
  {
  return true;
  }

void CY_BAND::iGetIN()
  {
	if ( Q02 == NULL )
		{
		if ( Q01->Status & CSignal::BAD )
			{
			OutStatus = CSignal::BAD;
			return ;
			}
    iRV1 = int(*Q01);
		iRV2 = 0;
		return;
		}
  bool BadRV1 = false;
  bool BadRV2 = false;
  if ( Q01->Status & CSignal::BAD )
    BadRV1 = true;
  if ( Q02->Status & CSignal::BAD )
    BadRV2 = true;
  if ( !BadRV1 && !BadRV2 )
    {
    *Q01 >> iRV1;
    if ( Q02 )
      *Q02>>iRV2;
    OutStatus = 0;
    return ;
    }
  OutStatus = 0;
  if ( !BadRV1 )
    iRV1 = int(*Q01);
  if ( !BadRV2 && Q02 )
    iRV2 = int(*Q02);
  switch ( W->Calc_Val_Err )
    {
    case 0:
    case 1:
      {
      if ( BadRV1 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 2:
      {
      if ( BadRV2 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 3:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV1 )
          OutStatus = CSignal::QST;
      }
    break;
    case 4:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        if ( BadRV2 )
          OutStatus = CSignal::QST;
      }
    break;
    case 5:
      {
      if ( BadRV1 && BadRV2 )
        OutStatus = CSignal::BAD;
      else
        OutStatus = CSignal::QST;
      }
    break;
    case 6:
      {
      if ( BadRV1 || BadRV2 )
        OutStatus = CSignal::BAD;
      }
    break;
    }
  }

