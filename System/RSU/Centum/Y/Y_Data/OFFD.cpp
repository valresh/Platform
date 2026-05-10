#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create OFFD ( "OFFD", CY_OFFD::Create );

CY_OFFD_W::CY_OFFD_W()
	{
	memset ( this, 0, sizeof ( CY_OFFD_W ));
	}

CY_OFFD::CY_OFFD( COFFD * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	IN = NULL;
	OUT.Name = "OUT";
	OUT << W->CPV;
	State = 0;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_OFFD,COFFD,30,50)

void CY_OFFD::InitParm()
  {
#include "Yokogawa\OFFD_P.hpp" 
#include "Yokogawa\OFFD.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_OFFD::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_OFFD::GetIN ( char * Name )
	{
	GET_IN(IN)
	return NULL;
	}

void CY_OFFD::Work ( double dt )
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
  if( !CY_Base::s_flagBasicScanControl )
    return;
  double deltaT = CY_Base::s_ControlPeriod * m_nBasicControlPeriodCounter;
  m_nBasicControlPeriodCounter = 0;

	if ( IN )
		*IN >> W->RV;
	if ( W->RV != 0 )
		{
		W->CPV = 1;
    W->PTM = 0.;
		State = 0;
		return ;
		}
// RV != 0
	switch ( State )
		{
		case 0:
			{
			W->CPV = 1;
			W->PTM = 0.;
			State = 1;
			}
		case 1:
			{
			W->CPV = 1;
			if ( W->Sec_Min )
				deltaT /= 60.;
			W->PTM += deltaT;
			if ( W->PTM < W->STM )
				break;
			State = 2;
			}
		case 2:
			{
// Задержка кончилась
			W->CPV = 0;
      W->PTM = 0.;
			}
		}
  }

void CY_OFFD::Init( )
	{
	}

int CY_OFFD::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\OFFD_P.hpp" 
	return 0;
	}


int CY_OFFD::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_OFFD::SaveState( )
{
	OUT.SaveState();
	SAVE_CLASS("W",CY_OFFD_W)
	return 0;
}

int CY_OFFD::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_OFFD_W)
	return 1;
}

int CY_OFFD::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "State", State );

  return 0;
}

int CY_OFFD::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
 
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "State", State );
  return 0;
}

int CY_OFFD::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->RV, "#RV" );
	PARM ( W->CPV,"#CPV" );
	PARM ( W->STM,"#STM" );
	PARM ( W->PTM,"#PTM" );
	PARM ( State, "#Состояние" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\OFFD_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\OFFD.hpp"
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

bool CY_OFFD::SetLimits( )
  {
  return true;
  }

