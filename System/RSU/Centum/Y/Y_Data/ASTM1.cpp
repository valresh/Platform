#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create ASTM1( "ASTM1", CY_ASTM1::Create );

CY_ASTM1::CY_ASTM1( CASTM1  * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
  IN  = NULL;
  Q01 = NULL;
  OUT.Name = "OUT";
  OUT << W->CPV;
  SUB.Name = "SUB";
  SUB.Null();
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_ASTM1,CASTM1,30,50)

void CY_ASTM1::InitParm()
{
#include "Yokogawa\ASTM1_P.hpp" 
#include "Yokogawa\ASTM1.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_ASTM1::GetOUT ( char * Name )
{
  GETOUT
  GETSUB
  return NULL;
}

CSignal ** CY_ASTM1::GetIN ( char * Name )
{
  GET_IN(Q01)
  GET_IN(IN)
  return NULL;
}

void CY_ASTM1::Init( )
{
  SUB << W->CPV;
}

int CY_ASTM1::Prepare0()
{
  //YOKO 3
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\AVE_P.hpp" 
  return 0;
}

int CY_ASTM1::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
  return 0;
}

static LPCSTR s_Name = "W";
int CY_ASTM1::SaveState( )
{
  OUT.SaveState();
  SUB.SaveState();
  SAVE_CLASS(s_Name,CASTM1_W)
  return 0;
}

int CY_ASTM1::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  if ( OUT.RestoreState ( StrName ) == 0 )
    return 0;
  if ( SUB.RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CASTM1_W)
  return 1;
}

double CY_ASTM1::Alpha(double T, double C)
{
	if(C > 0.0)
	{
		const double A = -6.55e-4;
		double P1 = 5.0;
		double P2 = 5.0*exp(1-C)*A;
		return T*(P2 - P1/C);
	}
	return 0.0;
}

double CY_ASTM1::Beta(double T, double C)
{
	if(C > 0.0)
	{
		const double B = 5.90e-4;
		double P3 = 0.0;
		double P4 = B;
		return T*(P4 - P3/C);
	}
	return 0.0;
}

void CY_ASTM1::Work( double dt )
{
  if( Q01 )
    *Q01 >> W->TMP;
  if( IN )
    *IN >> W->RV;
  //-------------------
  double Cf = 1.0 + Alpha(W->TMP-15.0, W->DEN) + Beta(W->TMP-15.0, W->DEN)*Beta(W->TMP-15.0, W->DEN);
  // Измеренный расход газа
  double Fi = W->RV;
  // Скорректируемый расход газа
  double F0 = Cf*Fi;
  W->CPV = W->GAIN*F0;
  if(W->CPV > W->SH)
	W->CPV = W->SH;
  if(W->CPV < W->SL)
	W->CPV = W->SL;
}

int CY_ASTM1::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->CPV, "#CPV" );
	PARM ( W->GAIN, "GAIN" ); 
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\ASTM1_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\ASTM1.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(Q01)
	PARM_IN(IN)
	PARM_OUT
  PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
