#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create BOPCBOL( "BOPCBOL", CY_BOPCBOL::Create );

CY_BOPCBOL::CY_BOPCBOL( CBOPCBOL  * _W ) : CY_Base()
{
  W = _W;
  pComm = "";
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_BOPCBOL,CBOPCBOL,5,5)

void CY_BOPCBOL::InitParm()
{
#include "Yokogawa\BOPCBOL.hpp"
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_BOPCBOL::GetOUT ( char * Name )
{
  return NULL;
}

CSignal ** CY_BOPCBOL::GetIN ( char * Name )
{
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_BOPCBOL::SaveState( )
{
  SAVE_CLASS(s_Name,CBOPCBOL_W)
  return 0;
}

int CY_BOPCBOL::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CBOPCBOL_W)
  return 1;
}

void CY_BOPCBOL::Work( double dt )
{
}

int CY_BOPCBOL::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
#include "Yokogawa\YPARM_PARM.h" 
	MENU("Все состояние")
#include "Yokogawa\BOPCBOL.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
