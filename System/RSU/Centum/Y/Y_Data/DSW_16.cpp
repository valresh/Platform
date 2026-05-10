#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"
#include "GlobalSettings.h"

// D2.36
static CY_Create DSW_16 ( "DSW-16", CY_DSW_16::Create );

CY_DSW_16_W::CY_DSW_16_W()
	{
	memset ( this, 0, sizeof ( CY_DSW_16_W ));
	}

CY_DSW_16::CY_DSW_16( CDSW_16 * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	OUT.Name = "OUT";
  OUT << W->CPV;
	_State = 0;
  int n = 0;
#undef GET_IN
#define GET_IN(SD) \
		pSD[n++] = &(W->SD);
	GET_IN(SD01)
	GET_IN(SD02)
	GET_IN(SD03)
	GET_IN(SD04)
	GET_IN(SD05)
	GET_IN(SD06)
	GET_IN(SD07)
	GET_IN(SD08)
	GET_IN(SD09)
	GET_IN(SD10)
	GET_IN(SD11)
	GET_IN(SD12)
	GET_IN(SD13)
	GET_IN(SD14)
	GET_IN(SD15)
	GET_IN(SD16)
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_DSW_16,CDSW_16,30,50)

void CY_DSW_16::InitParm()
  {
#include "Yokogawa\DSW_16_P.hpp" 
#include "Yokogawa\DSW_16.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_DSW_16::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_DSW_16::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_DSW_16::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
//  SetActions( );
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    return;
  }
	if ( W->SW > 0 )
		{
		int n = W -> SW - 1;
		if ( n < 0 )
			n = 0;
		if ( n > 15 )
			n = 15;
		W -> CPV = *pSD[n];
		}
	if ( W -> CPV < W -> SL )
		W -> CPV = W -> SL;
	if ( W -> CPV > W -> SH )
		W -> CPV = W -> SH;
  }

void CY_DSW_16::Init( )
	{
	}

int CY_DSW_16::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\DSW_16_P.hpp" 
	return 0;
	}


int CY_DSW_16::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_DSW_16::SaveState( )
{
	OUT.SaveState();
	SAVE_CLASS("W",CY_DSW_16_W)
  if( 1==KGlobalSettings::Instance().ExtendState() )
  {
    SAVE_FIELD( SD01 );
    SAVE_FIELD( SD02 );
    SAVE_FIELD( SD03 );
    SAVE_FIELD( SD04 );
    SAVE_FIELD( SD05 );
    SAVE_FIELD( SD06 );
    SAVE_FIELD( SD07 );
    SAVE_FIELD( SD08 );
    SAVE_FIELD( SD09 );
    SAVE_FIELD( SD10 );
    SAVE_FIELD( SD11 );
    SAVE_FIELD( SD12 );
    SAVE_FIELD( SD13 );
    SAVE_FIELD( SD14 );
    SAVE_FIELD( SD15 );
    SAVE_FIELD( SD16 );
  }
	return 0;
}

int CY_DSW_16::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_DSW_16_W)
  REST_FIELD( SD01 );
  REST_FIELD( SD02 );
  REST_FIELD( SD03 );
  REST_FIELD( SD04 );
  REST_FIELD( SD05 );
  REST_FIELD( SD06 );
  REST_FIELD( SD07 );
  REST_FIELD( SD08 );
  REST_FIELD( SD09 );
  REST_FIELD( SD10 );
  REST_FIELD( SD11 );
  REST_FIELD( SD12 );
  REST_FIELD( SD13 );
  REST_FIELD( SD14 );
  REST_FIELD( SD15 );
  REST_FIELD( SD16 );
	return 1;
}

int CY_DSW_16::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->SW, "#SW" );
	PARM ( W->CPV,"#CPV" );
	PARM ( W->SD01, "#SD01" );
	PARM ( W->SD02, "#SD02" );
	PARM ( W->SD03, "#SD03" );
	PARM ( W->SD04, "#SD04" );
	PARM ( W->SD05, "#SD05" );
	PARM ( W->SD06, "#SD06" );
	PARM ( W->SD07, "#SD07" );
	PARM ( W->SD08, "#SD08" );
	PARM ( W->SD09, "#SD09" );
	PARM ( W->SD10, "#SD10" );
	PARM ( W->SD11, "#SD11" );
	PARM ( W->SD12, "#SD12" );
	PARM ( W->SD13, "#SD13" );
	PARM ( W->SD14, "#SD14" );
	PARM ( W->SD15, "#SD15" );
	PARM ( W->SD16, "#SD16" );
  if ( pDR && pDR->FCS )
  {
    PARM_CALLBACK( pDR->FCS, "#FCS" );
  } 
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\DSW_16_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\DSW_16.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
		PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_DSW_16::SetLimits( )
  {
  return true;
  }

