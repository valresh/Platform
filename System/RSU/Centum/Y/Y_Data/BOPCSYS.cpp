#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create BOPCSYS( "BOPCSYS", CY_BOPCSYS::Create );

CY_BOPCSYS::CY_BOPCSYS( CBOPCSYS  * _W ) : CY_Base()
{
  W = _W;
  pComm = "";
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BOPCSYS,CBOPCSYS,5,5)

void CY_BOPCSYS::InitParm()
{
#include "Yokogawa\BOPCSYS.hpp"
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_BOPCSYS::GetOUT ( char * Name )
{
  ASSD(0);
  return NULL;
}

CSignal ** CY_BOPCSYS::GetIN ( char * Name )
{
  ASSD(0);
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_BOPCSYS::SaveState( )
{
  SAVE_CLASS(s_Name,CBOPCSYS_W)
  return 0;
}

int CY_BOPCSYS::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CBOPCSYS_W)
  return 1;
}

void CY_BOPCSYS::Work( double dt )
{
}

int CY_BOPCSYS::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h"
	MENU("Все состояние")
#include "Yokogawa\BOPCSYS.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
