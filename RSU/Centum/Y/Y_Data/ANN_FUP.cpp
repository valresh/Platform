#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create ANN_FUP( "ANN_FUP", CY_ANN_FUP::Create );

CY_ANN_FUP::CY_ANN_FUP( CANN_FUP * _W ) : CY_Base()
{
  W = _W;
  ASS( W );
  if ( W )
  {
    InitData();
    CY_Base::InitFlagUse( W->FlagUse );
  }
}


void CY_ANN_FUP::InitData( )
	{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_ANN_FUP,CANN_FUP,20,25)

void CY_ANN_FUP::InitParm()
{
#include <Yokogawa\ANN_FUP.hpp>
#include <Yokogawa\ANN_FUP_P.hpp>
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_ANN_FUP::GetOUT ( char * Name )
	{
  return NULL;
	}

CSignal ** CY_ANN_FUP::GetIN ( char * Name )
	{
	ASS( 0 );
  return NULL;
	}

void CY_ANN_FUP::Work ( double dt )
{
  SET_HBP
	{
	  KKK();
	}
}

bool CY_ANN_FUP::Draw ( CDrawData * pDD, int x0, int y0 )
  {
  ASS( 0 );
	return false;
  }

void CY_ANN_FUP::Init( )
	{
	}

int CY_ANN_FUP::Prepare0()
{
  return 0;
}

bool CY_ANN_FUP::SetLimits( )
	{
  return true;
	}

int CY_ANN_FUP::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	return 0;
}

int CY_ANN_FUP::SaveState( )
	{
	return 0;
	}

int CY_ANN_FUP::RestoreState ( char * StrName )
	{
	return 1;
	}

int CY_ANN_FUP::SetData( int TypeData, void * pData )
  {
  return 0;
  }

int CY_ANN_FUP::GetParams( KYBridge2SysParam &params )
{
  #include "IO_Parms.h"
  W_PARM(PV)
  #include "Yokogawa\YPARM_PARM.h" 
  #include "Yokogawa\ANN_FUP.hpp"
  MENU("Параметры")
  #include "Yokogawa\ANN_FUP_P.hpp"
  E_MENU("Параметры")
	return 0;
}

