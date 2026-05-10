#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create BOPCCHR( "BOPCCHR", CY_BOPCCHR::Create );

CY_BOPCCHR::CY_BOPCCHR( CBOPCCHR  * _W ) : CY_Base()
{
  W = _W;
  pComm = "";
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BOPCCHR,CBOPCCHR,5,5)

void CY_BOPCCHR::InitParm()
{
#include "Yokogawa\BOPCCHR.hpp"
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_BOPCCHR::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_BOPCCHR::GetIN ( char * Name )
{
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_BOPCCHR::SaveState( )
{
  SAVE_CLASS(s_Name,CBOPCCHR_W)
  return 0;
}

int CY_BOPCCHR::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CBOPCCHR_W)
  return 1;
}

void CY_BOPCCHR::Work( double dt )
{
}

int CY_BOPCCHR::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
	MENU("Все состояние")
#include "Yokogawa\BOPCCHR.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
