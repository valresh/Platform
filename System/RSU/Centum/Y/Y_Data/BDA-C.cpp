#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.192
static CY_Create BDA_C ( "BDA-C", CY_BDA_C::Create );

CY_BDA_C::CY_BDA_C( CBDA_C * _W ) : CY_Base()
, kOut( 0 )
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  Out[0].Name = "J01";
  Out[0].Set( W->DT[0] );
  Out[1].Name = "J02";
  Out[1].Set( W->DT[1] );
  Out[2].Name = "J03";
  Out[2].Set( W->DT[2] );
  Out[3].Name = "J04";
  Out[3].Set( W->DT[3] );
  Out[4].Name = "J05";
  Out[4].Set( W->DT[4] );
  Out[5].Name = "J06";
  Out[5].Set( W->DT[5] );
  Out[6].Name = "J07";
  Out[6].Set( W->DT[6] );
  Out[7].Name = "J08";
  Out[7].Set( W->DT[7] );
  Out[8].Name = "J09";
  Out[8].Set( W->DT[8] );
  Out[9].Name = "J10";
  Out[9].Set( W->DT[9] );
  Out[10].Name = "J11";
  Out[10].Set( W->DT[10] );
  Out[11].Name = "J12";
  Out[11].Set( W->DT[11] );
  Out[12].Name = "J13";
  Out[12].Set( W->DT[12] );
  Out[13].Name = "J14";
  Out[13].Set( W->DT[13] );
  Out[14].Name = "J15";
  Out[14].Set( W->DT[14] );
  Out[15].Name = "J16";
  Out[15].Set( W->DT[15] );
  kOut = _countof(Out);
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BDA_C,CBDA_C,50,60)

void CY_BDA_C::InitParm()
  {
#include "Yokogawa\BDA-C_P.hpp" 
#include "Yokogawa\BDA-C.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_BDA_C::GetOUT ( char * Name )
{
	GET_STD_OUT;
	return NULL;
}

CSignal ** CY_BDA_C::GetIN ( char * Name )
{
	return NULL;
}

void CY_BDA_C::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
}

void CY_BDA_C::Init( )
{
}

int CY_BDA_C::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\BDA-C_P.hpp" 
	return 0;
}


int CY_BDA_C::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_BDA_C::SaveState( )
{
	return 0;
}

int CY_BDA_C::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
}

int CY_BDA_C::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->SW, "#SW" );
  char Name[32];
  for ( int n = 0; n < _countof(W->DT); n++ )
  {
    sprintf_s( Name, "#DT%02d", n + 1 );
    PARMS( W->DT[n], Name );
  }
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\BDA-C_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\BDA-C.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
