#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create MOB_21( "S_MOB_21", CY_MOB_21::Create );

CY_MOB_21::CY_MOB_21( CS_MOB_21 * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
	InitData( );
}
       
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_MOB_21,CS_MOB_21,30,50)

void CY_MOB_21::InitParm()
  {
#include "Yokogawa/S_MOB_21.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

void CY_MOB_21::InitData( )
	{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;
	}

CSignal * CY_MOB_21::GetOUT ( char * Name )
	{
  return NULL;
	}

CSignal ** CY_MOB_21::GetIN ( char * Name )
	{
	ASS( 0 );
  return NULL;
	}

void CY_MOB_21::Work ( double dt )
{
  if( !W )
    return;
  if( W->MV == W->PV )
    W->BSTS = 1;
  else
    W->BSTS = 0;
}

bool CY_MOB_21::Draw ( CDrawData * pDD, int x0, int y0 )
  {
  ASS( 0 );
	return false;
  }

void CY_MOB_21::Init( )
	{
	}

int CY_MOB_21::Prepare0()
{
  return 0;
}

bool CY_MOB_21::SetLimits( )
	{
  return true;
	}

int CY_MOB_21::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
	return 0;
}

int CY_MOB_21::SaveState( )
	{
	return 0;
	}

int CY_MOB_21::RestoreState ( char * StrName )
	{
	return 1;
	}

int CY_MOB_21::SetData( int TypeData, void * pData )
  {
  return 0;
  }

int CY_MOB_21::GetParams( KYBridge2SysParam &params )
	{
  #include "IO_Parms.h"
  #include "Yokogawa\YPARM_PARM.h" 
  #include "Yokogawa\S_MOB_21.hpp"
	return 0;
	}

