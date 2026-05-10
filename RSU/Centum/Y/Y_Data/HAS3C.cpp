#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create HAS3C ( "HAS3C", CY_HAS3C::Create );

CY_HAS3C::CY_HAS3C( CHAS3C * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	JPV = JMV = JSV = JMOD = JALM = JBST = NULL;
  Q01 = Q02 = Q03 = Q04 = Q05 = NULL;
	Out[0].Name = "B01";
  Out[0] << W->MV01;
	Out[1].Name = "B02";
  Out[1] << W->MV02;
	Out[2].Name = "B03";
  Out[2] << W->MV03;
	Out[3].Name = "B04";
  Out[3] << W->MV04;
	Out[4].Name = "B05";
  Out[4] << W->MV05;
  Out[5].Name = "J01";
  Out[5] << W->SVH;
  Out[6].Name = "J02";
  Out[6] << W->SVL;
  Out[7].Name = "J03";
  Out[7] << W->MH;
  Out[8].Name = "J04";
  Out[8] << W->ML;
	kOut = 9;
  O = testMV = testSV = testPV = 0;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_HAS3C,CHAS3C,30,80)

void CY_HAS3C::InitParm()
  {
#include "Yokogawa\HAS3C_P.hpp" 
#include "Yokogawa\HAS3C.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_HAS3C::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_HAS3C::GetIN ( char * Name )
	{
	GET_IN(JPV)
  GET_IN(Q01)
  GET_IN(Q02)
  GET_IN(Q03)
  GET_IN(Q04)
  GET_IN(Q05)
  GET_IN(JMV)
  GET_IN(JSV)
  GET_IN(JMOD)
  GET_IN(JALM)
  GET_IN(JBST)
	return NULL;
	}

template<class T>
void SynchronizeJ( CSignal *target, T &test, T &V )
{
  if( !target )
    return;
  double t = V;
  *target >> t;
  if( test == V )
    V = t;
  else
    *target = V;

  test = V;
}

void CY_HAS3C::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
  if( Q01 ) *Q01 >> W->PV01;
  if( Q02 ) *Q02 >> W->PV02;
  if( Q03 ) *Q03 >> W->PV03;
  if( Q04 ) *Q04 >> W->PV04;
  if( Q05 ) *Q05 >> W->PV05;
  SynchronizeJ( JMV, testMV, W->MV );
  SynchronizeJ( JSV, testSV, W->SV );
  SynchronizeJ( JPV, testPV, W->PV );
  if( JALM) W->ALRM = *JALM;
  if( JMOD )
  {
    *JMOD >> W->mode.M;
    ASSD( W->mode.M >= SMode::ROUT );
  }
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
		return ;
    }
  else
    Out[0].Status &= ~CSignal::BAD;

  if ( JBST )
  {
    int I;
    *JBST >> I;
    W->BSTS = I;
  }

  switch( W->ButtonAction )
  {
  default:
    return;
  case MOMENT:
    {
      int *pMV0X = &W->MV01;
      for ( int i=0; i<_countof(W->MBOTs); i++ )
      {
        if( !pMV0X[i] )
          continue;
        if( W->MBOTs[i] == 0 )
        {
          W->MBOTs[i] = W->OutputTimeWidthBTA2;
          W->MBOTs[i] += dt/2;
          continue;
        }
        W->MBOTs[i] -= dt;
        if( W->MBOTs[i] < 0 )
          pMV0X[i] = 0, W->MBOTs[i] = 0;
      }
    }
    break;
  }
	}

void CY_HAS3C::Init( )
	{
	}

int CY_HAS3C::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\HAS3C_P.hpp" 
	return 0;
	}

int CY_HAS3C::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  switch( W->ButtonAction )
  {
  case MOMENT:
    for ( int i=0; i<_countof(W->MBOTs); i++ )
    {
      if( W->MBOTs[i] < 0 || W->MBOTs[i] > W->OutputTimeWidthBTA2 )
        W->MBOTs[i] = 0;
    }
    break;
  }
  testMV = W->MV;
	return 0;
}

static LPCSTR s_Name = "W";
int CY_HAS3C::SaveState( )
{
	SAVE_OUT
	SAVE_CLASS(s_Name,CY_HAS3C_W)
	return 0;
}

int CY_HAS3C::RestoreState ( char * StrName )
{
	REST_OUT
	REST_CLASS(s_Name,CY_HAS3C_W)
	return 1;
}

int CY_HAS3C::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "testMV", testMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "testSV", testSV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "testPV", testPV );
  return 0;
}

int CY_HAS3C::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "testMV", testMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "testSV", testSV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "testPV", testPV );
  return 0;
}

int CY_HAS3C::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM ( W->Comment, "#Comment" );
	PARM_MODE ( W->mode, "#Mode" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\HAS3C_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\HAS3C.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(JPV)
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}


bool CY_HAS3C::SetLimits( )
  {
  JPV ->Min = W->SL;
  JPV ->Max = W->SH;
  JPV ->IsLimits = true;
  if ( !Out[0].IsLimits )
    return false;
  W->MSH = W->MH = Out[0].Max;
  W->MSL = W->ML = Out[0].Min;
  return true;
  }