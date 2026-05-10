#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create BOPCF64( "BOPCF64", CY_BOPCF64::Create );

CY_BOPCF64::CY_BOPCF64( CBOPCF64  * _W ) : CY_Base()
{
  W = _W;
  pComm = "";
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BOPCF64,CBOPCF64,5,5)

void CY_BOPCF64::InitParm()
{
#include "Yokogawa\BOPCF64.hpp"
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_BOPCF64::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_BOPCF64::GetIN ( char * Name )
{
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_BOPCF64::SaveState( )
{
  SAVE_CLASS(s_Name,CBOPCF64_W)
  return 0;
}

int CY_BOPCF64::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CBOPCF64_W)
  return 1;
}

void CY_BOPCF64::Work( double dt )
{
}

int CY_BOPCF64::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
	MENU("Все состояние")
#include "Yokogawa\BOPCF64.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
