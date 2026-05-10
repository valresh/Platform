#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

// D2.11

static CY_Create INTEG ( "INTEG", CY_INTEG::Create );

CY_INTEG::CY_INTEG( CINTEG * _W ) : CY_Base()
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

LIST_PARM(CY_INTEG,CINTEG,10,20)

void CY_INTEG::InitParm()
  {
#include "Yokogawa\INTEG_P.hpp" 
#include "Yokogawa\INTEG.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_INTEG::GetOUT ( char * Name )
	{
  GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_INTEG::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_INTEG::Work ( double dt )
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

  switch( W->SW )
  {
  case 0:
    W->SW = 1;
    W->CPV = 0;
  case 1:
    W->BSTS = W->_BSTS::RUN;
    break;
  default:
    W->SW = 2;
  case 2:
    W->BSTS = W->_BSTS::STOP;
    return;
  }

	double Ksi = W->I / dt;
	double dRV = ( W->RV * W->GAIN ) / Ksi;

  if( dRV < W -> SL )
		dRV = W -> SL;
	if( dRV > W -> SH )
		dRV = W -> SH;
  W->CPV += dRV;
}

void CY_INTEG::Init( )
	{
	}

int CY_INTEG::Prepare0()
	{
  W->RV = 0;
  W->CPV = 0;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\INTEG_P.hpp" 
	return 0;
	}


int CY_INTEG::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_INTEG::SaveState( )
	{
	OUT.SaveState();
	SUB.SaveState();
	return 0;
	}

int CY_INTEG::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;

  /*DWORD skip[5] = { 0 };
  if ( strcmp ( StrName, "W" ) == 0 )
  {
    ASS(pSRFile->RStruct( 1, skip));
    return 0;
  }*/
	return 1;
}

int CY_INTEG::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
  
  static SYParam::SYParamStrVal vals[] = {{"STOP", W->BSTS.STOP}, {"RUN", W->BSTS.RUN} };
  params.Add( W->BSTS.V, "#BSTS", _countof(vals), vals );

	PARM ( W->RV, "#RV" );
	PARM ( W->CPV,"#CPV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\INTEG_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\INTEG.hpp"
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

bool CY_INTEG::SetLimits( )
  {
  return true;
  }
