#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include "Y_Draw.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include "GetObj.h"
#include <vector>
#include <macros/StrHelps.h>

FILE * fopen_err ( char * File, char * Mode );


static CY_Create LC64 ( "LC64", CY_LC64::Create );

CY_LC64::CY_LC64(CLC64 * _W ) : CY_LC64_E_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	kOut = 0;
  pSTs64 = W->ST;
  pPTs64 = W->PT;
}

//YOKO   1
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_LC64,CLC64,75,150)
void SortLC64Cond();
void SortLC64Act();

void CY_LC64::InitParm()
{
#include "Yokogawa\LC64_P.hpp" 
#include "Yokogawa\LC64.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  SortLC64Cond();
  SortLC64Act();
}

bool CY_LC64::ReadParm( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( !strncmp(ParmName, "TMNL", 4) )
  {
    char Fld[4][8] = { 0 };
    int K = SetFld_X( ParmValue, ':', Fld );
    ASS( K>=2 );
    W->TMNL_StartTiming = Fld[1][0];
    W->TMNL_hz = atoi( Fld[0] );
    switch( W->TMNL_StartTiming )
    {
    case eTMNL_StartupAtInitialColdStart_Restart:
      ASS( K==2 );
      break;
    case eTMNL_PeriodicExecution:
      if( K==4 )
      {
        W->TMNL_ControlPeriod = atoi( Fld[2] );
        W->TMNL_ControlPhase = atoi( Fld[3] );
      }
      else
      {
        W->TMNL_ControlPeriod = 0;
        W->TMNL_ControlPhase = 0;
      }
      break;
    default:
      ASS( !"надо думать что делать" );
    }
    KKK();
  }
  return __super::ReadParm( ParmName, ParmValue, pData );
}


CSignal * CY_LC64::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_LC64::GetIN ( char * Name )
{
	return NULL;
}

void CY_LC64::Work ( double dt )
{
  bool bExecAnyThing = false;
  if( -1==dt )
  {
    bExecAnyThing = true;
  }
  else
  {
    if( W->TMNL_StartTiming!=eTMNL_PeriodicExecution )
    {
      if( W->TMNL_StartTiming==eTMNL_StartupAtInitialColdStart_Restart )
      {
        if( execStartupAtInitialColdStart_Restart )
        {
          execStartupAtInitialColdStart_Restart = false;
          bExecAnyThing = true;
        }
        else
          return;
      }
      else
        return;
    }
    else
    {
      if( 0==dt )
        return;
    }
  }

	if ( !IsWork )
		{
		W -> mode.M = SMode::MAN;
		return ;
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
		return ;
    }
  else
    Out[0].Status &= ~CSignal::BAD;
	if ( W -> mode.M == SMode::MAN )
		return;
//////////////////////////////////////////////
  if( bExecAnyThing )
    Logic( CY_Base::s_ControlPeriod*W->TMNL_ControlPeriod );

  bExecutedAtStep = false;
  if( CY_Base::s_flagBasicScanControl )
  {
    m_timeHolder += CY_Base::s_ControlPeriod;
    if( m_nBasicControlPeriodCounter >= W->TMNL_ControlPeriod )
    {
      Logic( m_timeHolder );
      while( m_timeHolder >= CY_Base::s_ControlPeriod )
      {
        m_timeHolder -= CY_Base::s_ControlPeriod;
      }
      m_nBasicControlPeriodCounter = 0;
      bExecutedAtStep = true;
    }
  }
//////////////////////////////////////////////
	DWORD Mask = 0x80000000;
	DWORD ISS = 0;
	for ( int n = 0; n < kISS; n++ )
		{
		int N = OrdISS[n];
		if ( VarCond[N][0].pVar && VarCond[N][0].Yes )
			ISS |= Mask;
		Mask = Mask >> 1;
		}
	W->ISS = ISS;
	Mask = 0x80000000;
	DWORD COS = 0;
	for ( int n = 0; n < kCOS; n++ )
		{
		int N = OrdCOS[n];
		if ( Node[N].State )
			COS |= Mask;
		Mask = Mask >> 1;
		}
	W->COS = COS;
}

void CY_LC64::Init( )
{
}

int CY_LC64::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\LC64_P.hpp" 
  if( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
	Prepare();
	return 0;
}

int CY_LC64::SaveState()
{
	SAVE_OUT
	SAVE_CLASS("W",CY_LC64_W)
  return __super::SaveState();
}

int CY_LC64::RestoreState( char * StrName )
{
  REST_OUT
  REST_CLASS("W",CY_LC64_W)
  return __super::RestoreState( StrName );
}

int CY_LC64::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	PARM ( IsWork, "Работает" );
	PARM_MODE ( W->mode, "#Mode" );
  MENU("ST")
	char Name[64];
	for ( int n = 0; n < 64; n++ )
		{
		if ( W->ST[n] > 0. )
			{
			sprintf_s ( Name, sizeof(Name), "#ST%02d", n + 1 );
			PARM ( W->ST[n], Name );
			sprintf_s ( Name, sizeof(Name), "#PT%02d", n + 1 );
			PARM ( W->PT[n], Name );
			}
		}
	E_MENU("ST")
	PARM ( StopOnChange, "#Stop при изменениях" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\LC64_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\LC64.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_ALL_OUT
	E_MENU("Входы")
  __super::GetParams( params );
	return 0;
}
