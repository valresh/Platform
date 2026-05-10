#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#undef OUT
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create MLD_PVI ( "MLD-PVI", CY_MLD_PVI::Create );

CY_MLD_PVI_W::CY_MLD_PVI_W()
{
  memset ( this, 0, sizeof ( *this ));
}

CY_MLD_PVI::CY_MLD_PVI(CMLD_PVI * _W ) : CY_Base()
{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
//
	OUT.Name = "OUT";
  OUT << W->MV;
	SUB.Name = "SUB";
  SUB.Null();
	IN = NULL;
	TIN = NULL;
	TSI = NULL;
// ObrOut
	ObrOut.pMH = &W->MH;
  ObrOut.pML = &W->ML;
	ObrOut.pMSH = &W->MSH;
  ObrOut.pMSL = &W->MSL;
	ObrOut.pMV = &W->MV;
  ObrOut.pdMV = &dMV;
//-----------------------------------
	ObrOut.OUT = &OUT;
  ObrOut.pIMAN = &W->mode.IMAN;
  ObrOut.pOutTrk = NULL;//&Out_Trk;
  ObrOut.pMode = &W->mode.M;
  ObrOut.pMAN_bypass = &W->MAN_bypass;
  ObrOut.pOut_VL = &W->Out_VL;
  ObrOut.pLimit_On_CLP = &W->Limit_On_CLP;
  ObrOut.pPositional = &W->Positional;
	ObrOut.OutputLimiter = true;
//
  ObrIn.pSH = &W->SH;
  ObrIn.pSL = &W->SL;
  ObrIn.pPV_Overshoot = &W->PV_Overshoot;
  ObrIn.pPV_Limit = &W->PV_Limit;

  UseAlg = 3;
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_MLD_PVI,CMLD_PVI,49,69)

void CY_MLD_PVI::InitParm()
  {
#include "Yokogawa\MLD_PVI_P.hpp" 
#include "Yokogawa\MLD_PVI.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_MLD_PVI::GetOUT ( char * Name )
	{
  GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_MLD_PVI::GetIN ( char * Name )
	{
	GET_IN(IN)
	GET_IN(TIN)
	GET_IN(TSI)
	return NULL;
	}

void CY_MLD_PVI::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}

  if( 3==W->InputSignalConversion )
  {
    if( UseAlg & 1 )
      ObrIn.GAIN = W->Data_GAINin;
    else
      ObrIn.GAIN = 1.;
    if( UseAlg & 2 )
      ObrIn.BIAS = W->Data_BIASin;
    else
      ObrIn.BIAS = 0;
  }

	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
	if ( IN )
		ObrIn.Work ( IN, W->PV, dPVn, W->CALI );
	else
		W->PV = 0;

  if( TSI )
    *TSI >> W->TSW;

  if( TIN && W->TSW )
  {
    *TIN >> W->MV;
    W->mode.TRK = true;
  }
  else
    W->mode.TRK = false;

  ObrOut.OutputProcessing( false );
  if( 2==W->OutputSignalConvertion && W->Conv_GAINout != 0 )
    subsysMV = ( W->MV - W->Conv_BIASout ) / W->Conv_GAINout;
}

void CY_MLD_PVI::Init( )
{
	switch ( W->Aux )//PV, dPV, MV, DMV
	{
  default:
	case 0:
		SUB << W->PV;
	break;
	case 1:
		SUB << dPVn;
	break;
	case 2:
		SUB << W->MV;
	break;
	case 3:
		SUB << dMV;
	break;
	}
  switch( W->OutputSignalConvertion )
  {
  case 2:
    if( W->Conv_GAINout != 0 )
      OUT << subsysMV;
  case 0:
    break;
  default:
    ASSD(0);
  }
	ASS(SUB.pDbl!=NULL)
}

int CY_MLD_PVI::Prepare0()
{
  if( DBL_MAX!=W->MVH && DBL_MAX==W->MSH )
    W->MSH = W->MVH;
  if( DBL_MAX!=W->MVL && DBL_MAX==W->MSL )
    W->MSL = W->MVL;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\MLD_PVI_P.hpp" 
	return 0;
}


int CY_MLD_PVI::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_MLD_PVI::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_MLD_PVI_W)
  ObrOut.SaveState();
	return 0;
}

int CY_MLD_PVI::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_MLD_PVI_W)
  if ( ObrOut.RestoreState ( StrName ) == 0 )
    return 0;
	return 1;
}

int CY_MLD_PVI::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  ObrOut.StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dPVn", dPVn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "subsysMV", subsysMV );

  return 0;
}

int CY_MLD_PVI::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  ObrOut.StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPVn", dPVn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "subsysMV", subsysMV );
  return 0;
}

int CY_MLD_PVI::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODEFULL( W->mode, "#Mode" );
	PARM ( W->PV, "#PV" );
	PARM ( W->MV, "#MV" );
  PARM ( UseAlg, "Способ расчета" )
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\MLD_PVI_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\MLD_PVI.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	PARM_IN(TIN)
	PARM_IN(TSI)
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_MLD_PVI::SetLimits( )
  {
	if ( IN )
		{
		IN ->Min = W->SL;
		IN ->Max = W->SH;
		IN ->IsLimits = true;
		}
  if ( OUT.IsLimits )
  {
    if( W->MH > OUT.Max )
      W->MH = OUT.Max;
    if( W->ML < OUT.Min )
      W->ML = OUT.Min;
  }
  if( W->MSH == DBL_MAX && W->MH != DBL_MAX )
    W->MSH = W->MH;
  if( W->MSL == DBL_MAX && W->ML != DBL_MAX )
    W->MSL = W->ML;
  ASSD( W->MSH!=DBL_MAX );
  ASSD( W->MSL!=DBL_MAX );
  return true;
  }
