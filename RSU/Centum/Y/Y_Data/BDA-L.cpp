#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.36
static CY_Create BDA_L ( "BDA-L", CY_BDA_L::Create );

CY_BDA_L::CY_BDA_L( CBDA_L * _W ) : CY_Base()
, kOut( 0 )
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  SET_OUT("J01",W->DT01)
  SET_OUT("J02",W->DT02)
  SET_OUT("J03",W->DT03)
  SET_OUT("J04",W->DT04)
  SET_OUT("J05",W->DT05)
  SET_OUT("J06",W->DT06)
  SET_OUT("J07",W->DT07)
  SET_OUT("J08",W->DT08)
  SET_OUT("J09",W->DT09)
  SET_OUT("J10",W->DT10)
  SET_OUT("J11",W->DT11)
  SET_OUT("J12",W->DT12)
  SET_OUT("J13",W->DT13)
  SET_OUT("J14",W->DT14)
  SET_OUT("J15",W->DT15)
  SET_OUT("J16",W->DT16)
  kOut = 16;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BDA_L,CBDA_L,50,60)

void CY_BDA_L::InitParm()
  {
#include "Yokogawa\BDA-L_P.hpp" 
#include "Yokogawa\BDA-L.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_BDA_L::GetOUT ( char * Name )
	{
	GET_STD_OUT;
	return NULL;
	}

CSignal ** CY_BDA_L::GetIN ( char * Name )
	{
	return NULL;
	}

void CY_BDA_L::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
  }

void CY_BDA_L::Init( )
	{
	}

int CY_BDA_L::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\BDA-L_P.hpp" 
	return 0;
}


int CY_BDA_L::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_BDA_L::SaveState( )
{
	return 0;
}

int CY_BDA_L::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
}

int CY_BDA_L::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->SW, "#SW" );
  PARM ( W->DT01, "#DT01" );
  PARM ( W->DT02, "#DT02" );
  PARM ( W->DT03, "#DT03" );
  PARM ( W->DT04, "#DT04" );
  PARM ( W->DT05, "#DT05" );
  PARM ( W->DT06, "#DT06" );
  PARM ( W->DT07, "#DT07" );
  PARM ( W->DT08, "#DT08" );
  PARM ( W->DT09, "#DT09" );
  PARM ( W->DT10, "#DT10" );
  PARM ( W->DT11, "#DT11" );
  PARM ( W->DT12, "#DT12" );
  PARM ( W->DT13, "#DT13" );
  PARM ( W->DT14, "#DT14" );
  PARM ( W->DT15, "#DT15" );
  PARM ( W->DT16, "#DT16" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\BDA-L_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\BDA-L.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
