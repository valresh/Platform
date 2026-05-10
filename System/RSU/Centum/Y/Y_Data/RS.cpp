#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create RS( "RS", CY_RS::Create );

CY_RS::CY_RS( CRS * _W ) : CY_Base()
	{
	W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  //CY_Base::pComm = W->Comment;
	}
#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_RS,CRS,200,250)

void CY_RS::InitParm()
  {
#include "Yokogawa\RS_P.hpp" 
#include "Yokogawa\RS.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_RS::GetOUT ( char * Name )
	{
  //ASS(FALSE)
	return NULL;
	}

CSignal ** CY_RS::GetIN ( char * Name )
{
	return NULL;
}

void CY_RS::Work ( double dt )
{
  if( SMode::AUT != W->mode.M )
    return;
  BYTE *pRQ = &W->RQ01;
  BYTE *pPM = &W->PM01;
  const int PMH = W->PMH;
  int nPMH = 0;
  for( int i=0; i<32; ++ i )
  {
    pPM[i] = pRQ[i];
    if( pRQ[i] )
      ++nPMH;
    if( PMH==nPMH )
      break;
  }
}

void CY_RS::Init( )
{
}

int CY_RS::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\RS_P.hpp" 
	return 0;
	}

int CY_RS::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
  /*W->mode.M &= ~SMode::OS;
	if ( W->mode.M < SMode::ROUT )*/
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_RS::SaveState( )
{
	return 0;
}

int CY_RS::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;

	return 1;
}

int CY_RS::GetParams( KYBridge2SysParam &params )
{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\RS_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\RS.hpp"
	E_MENU("Все состояние")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
}

bool CY_RS::SetLimits( )
 {
  return true;
 }
