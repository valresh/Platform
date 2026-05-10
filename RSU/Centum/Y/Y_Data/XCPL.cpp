#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create XCPL ( "XCPL", CY_XCPL::Create );

CY_XCPL::CY_XCPL( CXCPL  * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	IN = NULL;
  BIN = NULL;
  TIN = NULL;
  TSI = NULL;
  INT = NULL;
	OUT.Name = "OUT";
  OUT << W->MV;
  SUB.Name = "SUB";
  SUB << W->MV;

  ObrOut.pMH = &W->MH;
  ObrOut.pML = &W->ML;
  ObrOut.pMSH = &W->MSH;
  ObrOut.pMSL = &W->MSL;
  ObrOut.pMV = &W->MV;
  ObrOut.pdMV = &dMV;
  //-----------------------------------
  ObrOut.OUT = &OUT;
  ObrOut.pIMAN = &W->mode.IMAN;
  ObrOut.pMode = &W->mode.M;
  ObrOut.pMAN_bypass = &W->MAN_bypass;
  ObrOut.pOut_VL = &W->Out_VL;
  ObrOut.pPositional = &W->Positional;
  ObrOut.OutputLimiter = true;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_XCPL,CXCPL,50,80)

void CY_XCPL::InitParm()
  {
#include "Yokogawa\XCPL_P.hpp" 
#include "Yokogawa\XCPL.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

void CY_XCPL::ResetAlarms()
{
  Alarm.SetBlk( W );
}

CSignal * CY_XCPL::GetOUT ( char * Name )
{
	GETOUT
  GETSUB
	return NULL;
}

CSignal ** CY_XCPL::GetIN ( char * Name )
{
	GET_IN(IN);
	GET_IN(BIN)
  GET_IN(TIN)
  GET_IN(TSI)
  GET_IN(INT)
	return NULL;
}

void CY_XCPL::Work ( double dt )
{
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;

	if( IN )
		*IN >> W->PV;
  if( BIN )
    *BIN >> W->VN;
  if( TSI )
    *TSI >> W->TSW;
  if( INT )
  {
    if (*( INT->pInt ))
    {
      W->INTERLOCK = 1;
      W->mode.M = SMode::MAN;
    }
    else
      W->INTERLOCK = 0;
  }
  else 
    W->INTERLOCK = 0;

  if( W->CALI  )
  {
    W->INTERLOCK = 0;
    W->mode.M = SMode::MAN;
    OUT.Status |= CSignal::CAL;
  }
  else
    OUT.Status &= ~CSignal::CAL;

  if( TIN && W->TSW )
  {
    *TIN >> W->MV;
    W->mode.TRK = true;
  }
  else
    W->mode.TRK = false;

  if( SMode::AUT == W->mode.M && !W->mode.TRK )
    W->MV = W->PV + W->CK*(W->VN+W->CB);
}

void CY_XCPL::Init( )
{
  switch ( W->AuxDataType )
  {
  case 0:
    SUB << W->MV;
    break;
  case 1:
    SUB << dMV;
    break;
  }
  ASS(SUB.pDbl!=NULL)
}

int CY_XCPL::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\XCPL_P.hpp" 
	return 0;
	}

int CY_XCPL::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_XCPL::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  return 0;
}

int CY_XCPL::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  return 0;
}

int CY_XCPL::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->Comment, "#Comment" );
  PARM ( W->PV, "#PV" );
#include "IO_Parms.h"
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\XCPL_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\XCPL.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_XCPL::SetLimits( )
{
  return true;
}
