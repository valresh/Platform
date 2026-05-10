#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create TON ( "TON", CY_TON::Create );

CY_TON::CY_TON( CTON * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
	OUT.Name = "OUT";
  OUT << W->CPV;
  State = 0;
	IN = NULL;
  nBetween1IN = 0;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_TON,CTON,31,51)

void CY_TON::InitParm()
  {
#include "Yokogawa\TON_P.hpp" 
#include "Yokogawa\TON.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_TON::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_TON::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_TON::Work ( double dt )
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
  if( !CY_Base::s_flagBasicScanControl )
  {
    if( IN )
    {
      int rvt = 0;
      *IN >> rvt;
      if( rvt )
      {
        ++nBetween1IN;
        W->RV = rvt;
      }
    }
    return;
  }
  m_nBasicControlPeriodCounter = 0;

  if( IN )
    *IN >> W->RV;
  if( 0==State && (W->RV || nBetween1IN) )
    W->CPV = 1;
  else
    W->CPV = 0;
  State = (W->RV || nBetween1IN);
  nBetween1IN = 0;
 }

void CY_TON::Init( )
	{
	}

int CY_TON::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\TON_P.hpp" 
	return 0;
	}

int CY_TON::Prepare1()
{
  State = W->RV;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_TON::SaveState( )
{
	OUT.SaveState();
	return 0;
}

int CY_TON::RestoreState ( char * StrName )
{
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	return 1;
}

int CY_TON::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\TON_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\TON.hpp"
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

bool CY_TON::SetLimits( )
  {
  return true;
  }

