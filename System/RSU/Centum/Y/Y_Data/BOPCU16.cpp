#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create BOPCU16( "BOPCU16", CY_BOPCU16::Create );

CY_BOPCU16::CY_BOPCU16( CBOPCU16  * _W ) : CY_Base()
{
  W = _W;
  pComm = "";
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BOPCU16,CBOPCU16,5,5)

void CY_BOPCU16::InitParm()
{
#include "Yokogawa\BOPCU16.hpp"
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_BOPCU16::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_BOPCU16::GetIN ( char * Name )
{
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_BOPCU16::SaveState( )
{
  SAVE_CLASS(s_Name,CBOPCU16_W)
  return 0;
}

int CY_BOPCU16::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CBOPCU16_W)
  return 1;
}

void CY_BOPCU16::Work( double dt )
{
}

int CY_BOPCU16::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
	MENU("Все состояние")
#include "Yokogawa\BOPCU16.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
