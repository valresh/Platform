#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create SR_FNCVR( "SR-FNCVR", CY_SR_FNCVR::Create );

CY_SR_FNCVR::CY_SR_FNCVR( CSR_FNCVR  * _W ) : CY_Base()
{
  W = _W;
  W->mode.M = SMode::AUT;
  CY_Base::pMode = &W->mode;
  pComm = W->Comment;
  CY_Base::InitFlagUse( W->FlagUse );
}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SR_FNCVR,CSR_FNCVR,20,20)

void CY_SR_FNCVR::InitParm()
{
#include "Yokogawa\SR_FNCVR.hpp"
#include "Yokogawa\SR_FNCVR_P.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
}

CSignal * CY_SR_FNCVR::GetOUT ( char * Name )
{
  ASSD(0);
  return NULL;
}

CSignal ** CY_SR_FNCVR::GetIN ( char * Name )
{
  ASSD(0);
  return NULL;
}


static LPCSTR s_Name = "W";
int CY_SR_FNCVR::SaveState( )
{
  SAVE_CLASS(s_Name,CSR_FNCVR_W)
  return 0;
}

int CY_SR_FNCVR::RestoreState ( char * StrName )
{
  if ( CY_Base::RestoreState ( StrName ) == 0 )
    return 0;
  REST_CLASS(s_Name,CSR_FNCVR_W)
  return 1;
}

void CY_SR_FNCVR::Work( double dt )
{
}

int CY_SR_FNCVR::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
  PARM_MODEFULL( W->mode, "#Mode" );
#include "Yokogawa\YPARM_PARM.h"
	MENU("Все состояние")
#include "Yokogawa\SR_FNCVR.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}
