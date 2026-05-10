#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create MOA( "S_MOA", CY_S_MOA::Create );

CY_S_MOA::CY_S_MOA( CS_MOA * _W ) : CY_Base()
{
  W = _W;
  if( W )
  {
    CY_Base::InitFlagUse( W->FlagUse );
    InitData( );
  }
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_S_MOA,CS_MOA,20,25)

void CY_S_MOA::InitParm()
{
#include <Yokogawa\S_MOA.hpp>
#include <Yokogawa\S_MOA_P.hpp>
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

void CY_S_MOA::InitData( )
{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;
}

CSignal * CY_S_MOA::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_S_MOA::GetIN ( char * Name )
{
  ASS( 0 );
  return NULL;
}

void CY_S_MOA::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
}

bool CY_S_MOA::Draw ( CDrawData * pDD, int x0, int y0 )
{
  ASS( 0 );
  return false;
}

void CY_S_MOA::Init( )
{
}

int CY_S_MOA::Prepare0()
{
  return 0;
}

bool CY_S_MOA::SetLimits( )
{
  return true;
}

int CY_S_MOA::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  W->notACK = 0;
  return 0;
}

int CY_S_MOA::SaveState( )
{
  return 0;
}

int CY_S_MOA::RestoreState ( char * StrName )
{
  return 1;
}

int CY_S_MOA::SetData( int TypeData, void * pData )
{
  return 0;
}

int CY_S_MOA::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  W_PARM(MV)
  W_PARM(SW)
  #include "Yokogawa\YPARM_PARM.h" 
  #include "Yokogawa\S_MOA.hpp"
  MENU("Параметры")
  #include "Yokogawa\S_MOA_P.hpp"
  E_MENU("Параметры")
  return 0;
}
