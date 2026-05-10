#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create AVE ( "AVE", CY_AVE::Create );

CY_AVE::CY_AVE( CAVE  * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	Q01 = NULL;
	Q02 = NULL;
	Q03 = NULL;
	Q04 = NULL;
	Q05 = NULL;
	Q06 = NULL;
	Q07 = NULL;
	Q08 = NULL;
	OUT.Name = "OUT";
	OUT << W->CPV;
	SUB.Name = "SUB";
	SUB.Null();
	}

//YOKO   1
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_AVE,CAVE,30,50)

void CY_AVE::InitParm()
  {
#include "Yokogawa\AVE_P.hpp" 
#include "Yokogawa\AVE.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_AVE::GetOUT ( char * Name )
	{
	GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_AVE::GetIN ( char * Name )
	{
	GET_IN(Q01)
	GET_IN(Q02)
	GET_IN(Q03)
	GET_IN(Q04)
	GET_IN(Q05)
	GET_IN(Q06)
	GET_IN(Q07)
	GET_IN(Q08)
	return NULL;
	}

void CY_AVE::Work ( double dt )
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
	double S = 0.;
	int K = 0;
	if ( W->Sample_All )
		{
		if ( Q01 )
			{
      *Q01 >> W->RV1;
			S += W->RV1;
			K++;
			}
		if ( Q02 )
			{
      *Q02 >> W->RV2;
			S += W->RV2;
			K++;
			}
		if ( Q03 )
			{
      *Q03 >> W->RV3;
			S += W->RV3;
			K++;
			}
		if ( Q04 )
			{
      *Q04 >> W->RV4;
			S += W->RV4;
			K++;
			}
		if ( Q05 )
			{
      *Q05 >> W->RV5;
			S += W->RV5;
			K++;
			}
		if ( Q06 )
			{
      *Q06 >> W->RV6;
			S += W->RV6;
			K++;
			}
		if ( Q07 )
			{
      *Q07 >> W->RV7;
			S += W->RV7;
			K++;
			}
		if ( Q08 )
			{
      *Q08 >> W->RV8;
			S += W->RV8;
			K++;
			}
		}
	else
		{
		if ( Q01 && ( Q01 ->Status & CSignal::BAD )	== 0 )
			{
      *Q01 >> W->RV1;
			S += W->RV1;
			K++;
			}
		if ( Q02 && ( Q02 ->Status & CSignal::BAD )	== 0 )
			{
      *Q02 >> W->RV2;
			S += W->RV2;
			K++;
			}
		if ( Q03 && ( Q03 ->Status & CSignal::BAD )	== 0 )
			{
      *Q03 >> W->RV3;
			S += W->RV3;
			K++;
			}
		if ( Q04 && ( Q04 ->Status & CSignal::BAD )	== 0 )
			{
      *Q04 >> W->RV4;
			S += W->RV4;
			K++;
			}
		if ( Q05 && ( Q05 ->Status & CSignal::BAD )	== 0 )
			{
      *Q05 >> W->RV5;
			S += W->RV5;
			K++;
			}
		if ( Q06 && ( Q06 ->Status & CSignal::BAD )	== 0 )
			{
      *Q06 >> W->RV6;
			S += W->RV6;
			K++;
			}
		if ( Q07 && ( Q07 ->Status & CSignal::BAD )	== 0 )
			{
      *Q07 >> W->RV7;
			S += W->RV7;
			K++;
			}
		if ( Q08 && ( Q08 ->Status & CSignal::BAD )	== 0 )
			{
      *Q08 >> W->RV8;
			S += W->RV8;
			K++;
			}
		}
//	if ( Output_trk )
//		W->CPV = CPV_new;
	if ( K > 0 )
		{
		double CPV_new = W->GAIN * S / K;
		dCPVn = CPV_new - W->CPV;
		W->CPV = CPV_new;
		OUT.Status &= ~CSignal::BAD;
		}
	else
		{
		OUT.Status |= CSignal::BAD;
		}
	}

void CY_AVE::Init( )
	{
	switch ( W->Aux )//PV, dPV, MV, DMV
		{
		case 0:
			SUB << W -> CPV;
		break;
		case 1:
			SUB << dCPVn;
		break;
		}
	}

int CY_AVE::Prepare0()
	{
//YOKO 3
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\AVE_P.hpp" 
	return 0;
	}

int CY_AVE::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

static LPCSTR s_Name = "W";
int CY_AVE::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
  SAVE_CLASS(s_Name,CY_AVE_W)
	return 0;
}

int CY_AVE::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS(s_Name,CY_AVE_W)
	return 1;
}

int CY_AVE::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  //OUT.StateSave( pStateSer );
  //SUB.StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );
  return 0;
}

int CY_AVE::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  //OUT.StateRestore( pStateSer );
  //SUB.StateRestore( pStateSer );
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dCPVn", dCPVn );
  return 0;
}

int CY_AVE::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->CPV, "#CPV" );
	PARM ( W->GAIN, "GAIN" ); //???? - из параметров
//YOKO 4
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\AVE_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\AVE.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(Q01)
	PARM_IN(Q02)
	PARM_IN(Q03)
	PARM_IN(Q04)
	PARM_IN(Q05)
	PARM_IN(Q06)
	PARM_IN(Q07)
	PARM_IN(Q08)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

#undef LIM
#define LIM(IN) \
  if ( IN )\
    {\
    IN ->Min = W->SL;\
    IN ->Max = W->SH;\
    IN ->IsLimits = true;\
    }

bool CY_AVE::SetLimits( )
  {
  LIM(Q01)
  LIM(Q02)
  LIM(Q03)
  LIM(Q04)
  LIM(Q05)
  LIM(Q06)
  LIM(Q07)
  LIM(Q08)
  if ( !OUT.IsLimits )
    return false;
  return true;
  }

