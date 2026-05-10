#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create SR_PID( "SR-PID", CY_SR_PID::Create );

CY_SR_PID::CY_SR_PID( CSR_PID  * _W ) : CY_Base()
{
  W = _W;
  W->mode.M = SMode::AUT;
  CY_Base::pMode = &W->mode;
  pComm = W->Comment;
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SR_PID,CSR_PID,51,51)

void CY_SR_PID::InitParm()
{
#include "Yokogawa\SR_PID.hpp"
#include "Yokogawa\SR_PID_P.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_SR_PID::GetOUT ( char * Name )
{
  ASSD(0);
  return NULL;
}

CSignal ** CY_SR_PID::GetIN ( char * Name )
{
  ASSD(0);
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_SR_PID::SaveState( )
{
  SAVE_CLASS(s_Name,CSR_PID_W)
  return 0;
}

int CY_SR_PID::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CSR_PID_W)
  return 1;
}

void CY_SR_PID::Work( double dt )
{
  if ( W->mode.M < SMode::ROUT )
    W->mode.M = SMode::MAN;
}

int CY_SR_PID::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODEFULL( W->mode, "#Mode" );
  MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SR_PID_P.hpp" 
  E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\SR_PID.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
