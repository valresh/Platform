#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create SRS1_R ( "SRS1-R", CY_SRSX_X::Create, CSRS1_R::TypeID );
static CY_Create SRS1_S ( "SRS1-S", CY_SRSX_X::Create, CSRS1_S::TypeID );
static CY_Create SRS2_R ( "SRS2-R", CY_SRSX_X::Create, CSRS2_R::TypeID );
static CY_Create SRS2_S ( "SRS2-S", CY_SRSX_X::Create, CSRS2_S::TypeID );

CY_SRSX_X::CY_SRSX_X( char * Name, int _Type, int number, int domain ) : CY_Base()
	{
  Type = _Type;
  switch ( Type )
  {
  case CSRS1_S::TypeID:
    W = (CSRSN_X*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CSRS1_S::TypeID, number, domain );
    break;
  case CSRS1_R::TypeID:
    W = (CSRSN_X*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CSRS1_R::TypeID, number, domain );
    break;
  case CSRS2_R::TypeID:
    W = (CSRSN_X*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CSRS2_R::TypeID, number, domain );
    break;
  case CSRS2_S::TypeID:
    W = (CSRSN_X*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CSRS2_S::TypeID, number, domain );
    break;
  default:
    ASS(0);
  }
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
//
	Q01 = NULL;
	Q02 = NULL;
	Out[0].Name = "J01";
	Out[0] << W->CPV1;
	Out[1].Name = "J02";
	Out[1] << W->CPV2;
	kOut = 2;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SRSX_X,CSRSN_X,5,15)

void CY_SRSX_X::InitParm()
  {
#include "Yokogawa\SRSn_X_P.hpp" 
#include "Yokogawa\SRSn_X.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_SRSX_X::GetOUT ( char * Name )
{
	GET_STD_OUT
	return NULL;
}

CSignal ** CY_SRSX_X::GetIN ( char * Name )
	{
	GET_IN(Q01)
	GET_IN(Q02)
	return NULL;
	}

void CY_SRSX_X::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
		return ;
    }
  else
    Out[0].Status &= ~CSignal::BAD;
	W->mode.M = SMode::AUT;
	if ( Q01 )
		*Q01 >> W -> RV1;  //S
	if ( Q02 )
		*Q02 >> W -> RV2;  //R
	if ( W -> RV1 == 0 && W -> RV2 == 0 )
		return;

  switch( Type )
  {
  case CSRS2_R::TypeID:
  case CSRS1_R::TypeID:
    if( W -> RV2 != 0 )
		{
		  W -> CPV1 = 0;
		  W -> CPV2 = 1;
		  break;
		}
	  W -> CPV1 = 1;
	  W -> CPV2 = 0;
    break;
  case CSRS2_S::TypeID:
  case CSRS1_S::TypeID:
    if( W -> RV1 != 0 )
    {
      W -> CPV1 = 1;
      W -> CPV2 = 0;
      break;
    }
    W -> CPV1 = 0;
    W -> CPV2 = 1;
    break;
  }
}

void CY_SRSX_X::Init( )
	{
	}

int CY_SRSX_X::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\SRSn_X_P.hpp" 
	return 0;
	}

int CY_SRSX_X::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_SRSX_X::SaveState( )
	{
	SAVE_OUT
	SAVE_CLASS("W",CY_SRSX_X_W)
	return 0;
	}

int CY_SRSX_X::RestoreState ( char * StrName )
	{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	REST_OUT
	REST_CLASS("W",CY_SRSX_X_W)
	return 1;
	}

int CY_SRSX_X::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	if ( Q01 )
		{
		PARM ( W->RV1, "#RV1" );
		}
	else
		{
		PARM ( W->RV1, "#RV1 - НЕТ Q01" );
		}
	if ( Q02 )
		{
		PARM ( W->RV2, "#RV2" );
		}
	else
		{
		PARM ( W->RV2, "#RV2 - НЕТ Q02" );
		}
	PARM ( W->CPV1, "#CPV1" );
	PARM ( W->CPV2, "#CPV2" );
	MENU("Все состояние")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SRSn_X.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(Q01)
	PARM_IN(Q02)
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

