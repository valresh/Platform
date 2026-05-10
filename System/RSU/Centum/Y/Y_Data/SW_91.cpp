#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.35
static CY_Create SW_91_H ( "SW-91", CY_SW_91::Create );

CY_SW_91_W::CY_SW_91_W()
{
  ZeroMemory( this, sizeof(*this) );
}

CY_SW_91::CY_SW_91( CSW_91 * _W ) : CY_Base(), b19to0( eHZ )
	{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	iS11 = NULL;
	iS12 = NULL;
	iS13 = NULL;
	iS14 = NULL;
	iS15 = NULL;
	iS16 = NULL;
	iS17 = NULL;
	iS18 = NULL;
	iS19 = NULL;
	oS10.Name = "S10";
	oS10 << OUT;

  iS10 = NULL;
  oS1to19[0] << OUT9[0]; oS1to19[0].Name = "S11";
  oS1to19[1] << OUT9[1]; oS1to19[1].Name = "S12";
  oS1to19[2] << OUT9[2]; oS1to19[2].Name = "S13";
  oS1to19[3] << OUT9[3]; oS1to19[3].Name = "S14";
  oS1to19[4] << OUT9[4]; oS1to19[4].Name = "S15";
  oS1to19[5] << OUT9[5]; oS1to19[5].Name = "S16";
  oS1to19[6] << OUT9[6]; oS1to19[6].Name = "S17";
  oS1to19[7] << OUT9[7]; oS1to19[7].Name = "S18";
  oS1to19[8] << OUT9[8]; oS1to19[8].Name = "S19";
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SW_91,CSW_91,30,50)

void CY_SW_91::InitParm()
  {
#include "Yokogawa\SW_91_P.hpp" 
#include "Yokogawa\SW_91.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_SW_91::GetOUT ( char * Name )
{
	if ( strcmp ( Name, "S10" ) == 0 )
	{
    b19to0 = e19to0;
		return &oS10;
  }
  for( int i=0; i<_countof(oS1to19); i++ )
  {
    if( strcmp( oS1to19[i].Name, Name ) == 0 )
    {
      b19to0 = e0to19;
      return &oS1to19[i];
    }
  }
	return NULL;
}

CSignal ** CY_SW_91::GetIN ( char * Name )
	{
  GET_IN_NAME(iS10,"S10")

	GET_IN_NAME(iS11,"S11")
	GET_IN_NAME(iS12,"S12")
	GET_IN_NAME(iS13,"S13")
	GET_IN_NAME(iS14,"S14")
	GET_IN_NAME(iS15,"S15")
	GET_IN_NAME(iS16,"S16")
	GET_IN_NAME(iS17,"S17")
	GET_IN_NAME(iS18,"S18")
	GET_IN_NAME(iS19,"S19")
	return NULL;
	}

void CY_SW_91::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}

  W->FlagUse |= CAlarmBase::fuYConnect;//пусть будет всегда доступным

  if( b19to0==e19to0 )
    Work19to0( dt );
  else if( b19to0==e0to19 )
    Work0to19( dt );
  
	}

void CY_SW_91::Work0to19( double dt )
{
  if( W->mode.M == SMode::OS )
  {
    return;
  }

  W->mode.M = SMode::AUT;
  if ( W->SW < 0 )
    W->SW = 0;
  if ( W->SW > 9 )
    W->SW = 9;
  if ( W->SW == 0 )
    return;
  *iS10 >> OUT9[W->SW - 1];
}

void CY_SW_91::Work19to0( double dt )
{
  if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
  {
    W->mode.M = SMode::OS;
    oS10.Status |= CSignal::BAD;
    return ;
  }
  else
    oS10.Status &= ~CSignal::BAD;
  W->mode.M = SMode::AUT;
  if ( W->SW < 0 )
    W->SW = 0;
  if ( W->SW > 9 )
    W->SW = 9;
  if ( W->SW == 0 )
    return;
  switch ( W->SW )
  {
  case 1:
    if( iS11 )
    *iS11 >> OUT;
    break;
  case 2:
    if( iS12 )
    *iS12 >> OUT;
    break;
  case 3:
    if( iS13 )
    *iS13 >> OUT;
    break;
  case 4:
    if( iS14 )
    *iS14 >> OUT;
    break;
  case 5:
    if( iS15 )
    *iS15 >> OUT;
    break;
  case 6:
    if( iS16 )
    *iS16 >> OUT;
    break;
  case 7:
    if( iS17 )
    *iS17 >> OUT;
    break;
  case 8:
    if( iS18 )
    *iS18 >> OUT;
    break;
  case 9:
    if( iS19 )
    *iS19 >> OUT;
    break;
  }
}

void CY_SW_91::Init( )
	{
	}

int CY_SW_91::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\SW_91_P.hpp" 
	return 0;
	}

int CY_SW_91::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_SW_91::SaveState( )
{
	oS10.SaveState();
  for( int i=0; i<_countof(oS1to19); i++ )
    oS1to19[i].SaveState();
	SAVE_CLASS("W",CY_SW_91_W)
	return 0;
}

int CY_SW_91::RestoreState ( char * StrName )
{
	if ( oS10.RestoreState ( StrName ) == 0 )
		return 0;
  for( int i=0; i<_countof(oS1to19); i++ )
  {
    if( 0==oS1to19[i].RestoreState( StrName ) )
      return 0;
  }
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_SW_91_W)
	return 1;
}

int CY_SW_91::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OUT", OUT );
  WriteFullStruct( pStateSer, eKeyAnyVar, "OUT9", OUT9 );

  return 0;
}

int CY_SW_91::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OUT", OUT );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "OUT9", OUT9 );

  return 0;
}

int CY_SW_91::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->SW, "#SW" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SW_91_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\SW_91.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(iS11)
	PARM_IN(iS12)
	PARM_IN(iS13)
	PARM_IN(iS14)
	PARM_IN(iS15)
	PARM_IN(iS16)
	PARM_IN(iS17)
	PARM_IN(iS18)
	PARM_IN(iS19)
	if ( oS10.pTo[0] )
		{
		char Name[256];
		char Used = ' ';
		if ( oS10.Used() ) Used = '<';
		sprintf_s ( Name, sizeof(Name), "#-%c%s", Used, oS10.Name );
		PARM(oS10.pTo[0], Name )
		}
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}


