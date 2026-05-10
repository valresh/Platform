#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include "Tunning.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create SS_H ( "SS-H", CY_SS::Create, CY_SS::H );
static CY_Create SS_M ( "SS-M", CY_SS::Create, CY_SS::M );
static CY_Create SS_L ( "SS-L", CY_SS::Create, CY_SS::L );

CY_SS::CY_SS( char * Name, int Type, int number, int domain ) : CY_Base()
, m_Type( (eType)Type )
	{
	W = NULL;
  Init( Name, number, domain );

  CY_Base::pComm = W->Comment;
//
	IN1 = NULL;
	IN2 = NULL;
	IN3 = NULL;
	M_LIM = 1.;
  Bad1 = Bad2 = Bad3 = false;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SS,CSS_COMM,30,50)

void CY_SS::InitParm()
  {
#include "Yokogawa\SS_P.hpp" 
#include "Yokogawa\SS.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_SS::GetOUT ( char * Name )
	{
	GETOUT
	return NULL;
	}

CSignal ** CY_SS::GetIN ( char * Name )
	{
	GET_IN(IN1)
	GET_IN(IN2)
	GET_IN(IN3)
	return NULL;
	}

void CY_SS::Work ( double dt )
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
	if ( W->SW == 0 )
		SEL = 0;
	Signal_selection( );
  if ( SEL < 0 )
    SEL = 0;
  if ( SEL > 3 )
    SEL = 3;
  if ( W ->SW == 0 )
    SEL = 0;
  W->SEL = SEL;
	if ( SEL == 1 )
		W->PV = W->RV1;
	if ( SEL == 2 )
		W->PV = W->RV2;
	if ( SEL == 3 )
		W->PV = W->RV3;
	}

void CY_SS::Init( char * Name, int number, int domain )
	{
    if( W )
      return;
	switch ( m_Type )
		{
		case H:
			{
      CSS_H * W_H = (CSS_H*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CSS_H::TypeID, number, domain );
      W = W_H;
			}
		break;
		case M:
			{
      CSS_M * W_M = (CSS_M*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CSS_M::TypeID, number, domain );
      W = W_M;
			}
		break;
		case L:
			{
      CSS_L * W_L = (CSS_L*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, CSS_L::TypeID, number, domain );
      W = W_L;
			}
		break;
    default:
      ASS( 0 );
		}
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pMode = &W->mode;
	INFO(W->PV,"PV")
	INFO(SEL,"SEL")
	OUT.Name = "OUT";
	OUT << W->PV;
//
  ObrIn.pSH = &W->SH;
  ObrIn.pSL = &W->SL;
  ObrIn.pPV_Overshoot = &W->PV_Overshoot;
  ObrIn.pPV_Limit = &W->PV_Limit;
	}

int CY_SS::Prepare0()
	{
	return 0;
	}

int CY_SS::Prepare1()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\SS_P.hpp" 
	if (W->SEL == 0)
		W->SW = 4;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  Bad1 = Bad2 = Bad3 = false;
	return 0;
}

int CY_SS::SaveState( )
{
	OUT.SaveState();
	SAVE_CLASS("W",CY_SS_W)
	return 0;
}

int CY_SS::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_SS_W)
	return 1;
}

int CY_SS::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "SEL", SEL );

  return 0;
}

int CY_SS::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "SEL", SEL );

  return 0;
}

int CY_SS::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->SW, "#SW" );
	PARM ( W->PV, "#PV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SS_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\SS.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN1)
	PARM_IN(IN2)
	PARM_IN(IN3)
	PARM_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
  PARM(SEL, "#SEL")
  PARM(Bad1, "#InBad1")
  PARM(Bad2, "#InBad2")
  PARM(Bad3, "#InBad3")
	return 0;
	}

void CY_SS::Signal_selection( )
	{
///////////////////////////////////////////////
	W->mode.TRK = false;
///////////////////////////////////////////////
	ObrIn.PV_Overshoot ( IN1, W->RV1, &Bad1, false );
	ObrIn.PV_Overshoot ( IN2, W->RV2, &Bad2, false );
	ObrIn.PV_Overshoot ( IN3, W->RV3, &Bad3, false );
	//W->RV1 *= 0.999999;
	//W->RV3 *= 1.0000001;
//
	if ( W->SW <= 0 )
		return;
	if ( W->SW >= 1 && W->SW <= 3	)
		{
		SEL = W->SW;
		}
	else
		{
		bool Bad = Bad1 && Bad2 && Bad3;
		if ( Bad )
			{
			W->SW = 0;
			SEL = 0;
			}
		else
			{
			switch ( m_Type )
				{
				case H:
					SS_H( );
				break;
				case M:
					SS_M( );
				break;
				case L:
					SS_L( );
				break;
				}
			}
		}
//
	if ( SEL == 1 )
		{
		W->PV = W->RV1;
		return ;
		}
	if ( SEL == 2 )
		{
		W->PV = W->RV2;
		return ;
		}
	if ( SEL == 3 )
		{
		W->PV = W->RV3;
		return ;
		}
	}

void CY_SS::SS_H( )
	{
	double V = -DBL_MAX;
	if ( !Bad1 ) 
		{
		V = W->RV1;
		SEL = 1;
		}
	if ( !Bad2 && W->RV2 > V ) 
		{
		V = W->RV2;
		SEL = 2;
		}
	if ( !Bad3 && W->RV3 > V ) 
		{
		SEL = 3;
		}
	}

void CY_SS::SS_M( )
	{
// Анализ BAD
	int kBAD = 0;
	if ( Bad1 )
		kBAD++;
	if ( Bad2 )
		kBAD++;
	if ( Bad3 )
		kBAD++;
// kBAD == 0
	if ( kBAD == 0 )
		{
		double V = W->RV1;
		int K1 = 0;
		int K2 = 0;
		int K3 = 0;
		if ( W->RV1 >= W->RV2 )
			K1++;
		else
			K2++;
		if ( W->RV1 >= W->RV3 )
			K1++;
		else
			K3++;
		if ( K1 == 1 )
			{
			SEL = 1;
			return;
			}
		if ( W->RV2 >= W->RV3 )
			K2++;
		else
			K3++;
		if ( K2 == 1 )
			{
			SEL = 2;
			return;
			}
		SEL = 3;
		return;
		}
// kBAD == 1
	if ( kBAD == 1 )
		{
		if ( Bad1 )
			{
			SEL = 2;
			return;
			}
		if ( Bad2 )
			{
			SEL = 1;
			return;
			}
		SEL = 2;
		return;
		}
// kBAD == 2
	if ( !Bad1 )
		{
		SEL = 1;
		return;
		}
	if ( !Bad2 )
		{
		SEL = 2;
		return;
		}
	SEL = 3;
	}

void CY_SS::SS_L( )
	{
	double V = DBL_MAX;
	if ( !Bad1 ) 
		{
		V = W->RV1;
		SEL = 1;
		}
	if ( !Bad2 && W->RV2 < V ) 
		{
		V = W->RV2;
		SEL = 2;
		}
	if ( !Bad3 && W->RV3 < V ) 
		{
		SEL = 3;
		}
	}

#undef LIM
#define LIM(IN) \
  if ( IN )\
    {\
    IN ->Min = W->SL;\
    IN ->Max = W->SH;\
    IN ->IsLimits = true;\
    }

bool CY_SS::SetLimits( )
  {
  LIM(IN1)
  LIM(IN2)
  LIM(IN3)
  return true;
  }

