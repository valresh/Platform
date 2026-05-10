#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"


static CY_Create INDST3 ( "INDST3", CY_INDST3::Create );

CY_INDST3::CY_INDST3(CINDST3 * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
//
	JMOD = NULL;
	INT = NULL;
	JBST = NULL;
	JALM = NULL;
	JPV = NULL;
	JMV = NULL;
	JSV = NULL;
	J01 = NULL;
	J02 = NULL;
	J03 = NULL;
	J04 = NULL;	

  JSVout << W->SV, JSVout.Name = "JSV";
  JMVout << W->MV, JMVout.Name = "JMV";
  J01out << W->SVH,J01out.Name = "J01";
  J02out << W->SVL,J02out.Name = "J02";
  J03out << W->MH, J03out.Name = "J03";
  J04out << W->ML, J04out.Name = "J04";
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_INDST3,CINDST3,20,30)

void CY_INDST3::InitParm()
  {
#include "Yokogawa\INDST3_P.hpp" 
#include "Yokogawa\INDST3.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_INDST3::GetOUT ( char * Name )
{
  GET_IN_NAME( JSVout, "JSV");
  GET_IN_NAME( JMVout, "JMV");
  GET_IN_NAME( J01out, "J01");
  GET_IN_NAME( J02out, "J02");
  GET_IN_NAME( J03out, "J03");
  GET_IN_NAME( J04out, "J04");
	return NULL;
}

CSignal ** CY_INDST3::GetIN ( char * Name )
	{
	GET_IN(JMOD)
	GET_IN(INT)
	GET_IN(JBST)
	GET_IN(JALM)
	GET_IN(JPV)
	GET_IN(JMV)
	GET_IN(JSV)
	GET_IN(J01)
	GET_IN(J02)
	GET_IN(J03)
	GET_IN(J04)	
	return NULL;
	}

void CY_INDST3::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
		return ;
    }
	if ( JMOD )
		{
		int M;
		*JMOD >> M;
		W->mode.M = M;
		}
	if ( JBST )
		{
		int I;
		*JBST >> I;
		W->BSTS = I;
		}
	if ( JALM )
		{
		int A;
		*JALM >> A;
		W->ALRM = A;
		}

	if ( JPV )
		*JPV >> W->PV;

	if( JSV )
  {
    if( JSV->Status & CSignal::_ReverseExchange )
      *JSV = W->SV;
    else
    {
      if( tSV != W->SV )
        *JSV = W->SV;
      *JSV >> W->SV;
    }
  }
  tSV = W->SV;

	if( JMV )
  {
    if( tMV != W->MV )
      *JMV = W->MV;
    *JMV >> W->MV;
  }
  tMV = W->MV;

	if( J01 )
  {
    if( tSVH != W->SVH )
      *J01 = W->SVH;
    *J01 >> W->SVH;
  }
  tSVH = W->SVH;

	if( J02 )
  {
    if( tSVL != W->SVL )
      *J02 = W->SVL;
    *J02 >> W->SVL;
  }
  tSVL = W->SVL;
	
  if( J03 )
  {
    if( tMH != W->MH )
      *J03 = W->MH;
    *J03 >> W->MH;
  }
  tMH = W->MH;
	
  if( J04 )
  {
    if( tML != W->ML )
      *J04 = W->ML;
    *J04 >> W->ML;
  }
  tML = W->ML;

  if( W->SH > W->SL )
  {
    if( W->SV > W->SH )
      W->SV = W->SH;
    if( W->SV < W->SL )
      W->SV = W->SL;
  }
}

void CY_INDST3::Init( )
	{
	}

int CY_INDST3::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\INDST3_P.hpp" 
	return 0;
	}

int CY_INDST3::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  tSV = W->SV;
  tMV = W->MV;
  tSVH = W->SVH;
  tSVL = W->SVL;
  tMH = W->MH;
  tML = W->ML;
	return 0;
}

int CY_INDST3::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->PV, "#PV" );
	PARM ( W->MV, "#MV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\INDST3_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\INDST3.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(JMOD)
	PARM_IN(INT)
	PARM_IN(JBST)
	PARM_IN(JALM)
	PARM_IN(JPV)
	PARM_IN(JMV)
	PARM_IN(JSV)
	PARM_IN(J01)
	PARM_IN(J02)
	PARM_IN(J03)
	PARM_IN(J04)	
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_INDST3::SetLimits( )
  {
  return true;
  }
