#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create BOPCI16( "BOPCI16", CY_BOPCI16::Create );

CY_BOPCI16::CY_BOPCI16( CBOPCI16  * _W ) : CY_Base()
{
  W = _W;
  pComm = "";
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BOPCI16,CBOPCI16,5,5)

void CY_BOPCI16::InitParm()
{
#include "Yokogawa\BOPCI16.hpp"
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_BOPCI16::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_BOPCI16::GetIN ( char * Name )
{
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_BOPCI16::SaveState( )
{
  SAVE_CLASS(s_Name,CBOPCI16_W)
  return 0;
}

int CY_BOPCI16::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CBOPCI16_W)
  return 1;
}

void CY_BOPCI16::Work( double dt )
{
}

int CY_BOPCI16::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
	MENU("Все состояние")
#include "Yokogawa\BOPCI16.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
