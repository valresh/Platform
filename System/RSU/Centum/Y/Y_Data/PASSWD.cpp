#include "stdafx.h"
#include "Y_Data.h"
#include <ConnectY.h>
#include "StdDef.h"
#include "ReadParms.h"

static CY_Create PASSWD( "S_PASSWD", CYS_PASSWD::Create );

CYS_PASSWD::CYS_PASSWD( CS_PASSWD * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  InitData( );
}

void CYS_PASSWD::InitData( )
{
  CY_Base::pComm = "";//W->Comment;
  CY_Base::pMode = NULL;//&W->mode;
}

CSignal * CYS_PASSWD::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CYS_PASSWD::GetIN ( char * Name )
{
  ASS( 0 );
  return NULL;
}

void CYS_PASSWD::Work ( double dt )
{
  SET_HBP
  {
    KKK();
  }
}

bool CYS_PASSWD::Draw ( CDrawData * pDD, int x0, int y0 )
{
  ASS( 0 );
  return false;
}

void CYS_PASSWD::Init( )
{
}

int CYS_PASSWD::Prepare0()
{
  return 0;
}

bool CYS_PASSWD::SetLimits( )
{
  return true;
}

int CYS_PASSWD::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  return 0;
}

int CYS_PASSWD::SaveState( )
{
  return 0;
}

int CYS_PASSWD::RestoreState ( char * StrName )
{
  return 1;
}

int CYS_PASSWD::SetData( int TypeData, void * pData )
{
  return 0;
}

int CYS_PASSWD::GetParams( KYBridge2SysParam &params )
{
  #include "IO_Parms.h"
  W_PARM(PV)
  W_PARM(MV)
  #include "Yokogawa\YPARM_PARM.h" 
  #include "Yokogawa\S_PASSWD.hpp"
  MENU("Параметры")
  #include "Yokogawa\S_PASSWD_P.hpp"
  E_MENU("Параметры")
  return 0;
}

