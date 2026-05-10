#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create MOB_RS( "S_MOB_RS", CY_MOB_RS::Create );

CY_MOB_RS::CY_MOB_RS( CS_MOB_RS * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  InitData( );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_MOB_RS,CS_MOB_RS,20,25)

void CY_MOB_RS::InitParm()
{
#include <Yokogawa\S_MOB_RS.hpp>
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_MOB_RS::InitData( )
	{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;
	}

CSignal * CY_MOB_RS::GetOUT ( char * Name )
	{
  return NULL;
	}

CSignal ** CY_MOB_RS::GetIN ( char * Name )
	{
	ASS( 0 );
  return NULL;
	}

void CY_MOB_RS::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	}

bool CY_MOB_RS::Draw ( CDrawData * pDD, int x0, int y0 )
  {
  ASS( 0 );
	return false;
  }

void CY_MOB_RS::Init( )
	{
	}

int CY_MOB_RS::Prepare0()
{
  return 0;
}

bool CY_MOB_RS::SetLimits( )
	{
  return true;
	}

int CY_MOB_RS::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::AUT;
	return 0;
}

int CY_MOB_RS::SaveState( )
	{
	return 0;
	}

int CY_MOB_RS::RestoreState ( char * StrName )
	{
	return 1;
	}

int CY_MOB_RS::SetData( int TypeData, void * pData )
  {
  return 0;
  }

int CY_MOB_RS::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\S_MOB_RS.hpp"
	return 0;
	}

