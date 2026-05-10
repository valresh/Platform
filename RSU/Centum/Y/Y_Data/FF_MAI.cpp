#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.10

static CY_Create FF_MAI ( "FF-MAI", CY_FF_MAI::Create );

CY_FF_MAI::CY_FF_MAI( CFF_MAI * _W ) : CY_Base()
, kOut( 8 )
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	Out[0].Name = "OUT1";
  Out[0] << W->OUT1_V;
  Out[1].Name = "OUT2";
  Out[1] << W->OUT2_V;
  Out[2].Name = "OUT3";
  Out[2] << W->OUT3_V;
  Out[3].Name = "OUT4";
  Out[3] << W->OUT4_V;
  Out[4].Name = "OUT5";
  Out[4] << W->OUT5_V;
  Out[5].Name = "OUT6";
  Out[5] << W->OUT6_V;
  Out[6].Name = "OUT7";
  Out[6] << W->OUT7_V;
  Out[7].Name = "OUT8";
  Out[7] << W->OUT8_V;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FF_MAI,CFF_MAI,20,40)

void CY_FF_MAI::InitParm()
{
#include "Yokogawa\FF_MAI_P.hpp" 
#include "Yokogawa\FF_MAI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_FF_MAI::GetOUT ( char * Name )
{
  GET_STD_OUT
	return NULL;
}

CSignal ** CY_FF_MAI::GetIN ( char * Name )
{
	ASS(0)
	return NULL;
}

void CY_FF_MAI::Work ( double dt )
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
}

void CY_FF_MAI::Init( )
{
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
}

int CY_FF_MAI::Prepare0()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FF_MAI_P.hpp" 
	return 0;
}


int CY_FF_MAI::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  W->mode.M = SMode::AUT;
	return 0;
}

int CY_FF_MAI::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	//PARM ( W->PV, "#PV" );
  if ( pDR && pDR->FCS )
  {
    PARM_CALLBACK( pDR->FCS, "#FCS" );
  } 
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FF_MAI_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\FF_MAI.hpp"
	E_MENU("Все состояние")
	MENU("Выходы")
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}

