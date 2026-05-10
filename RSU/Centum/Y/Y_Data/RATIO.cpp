#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create RATIO ( "RATIO", CY_RATIO::Create );

CY_RATIO::CY_RATIO( CRATIO * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	IN = NULL;
	SET = NULL;
	TIN = NULL;
	TSI = NULL;
	INT = NULL;
	OUT.Name = "OUT";
	OUT << W->MV;
	SUB.Name = "SUB";
	SUB.Null();//- в параметрах
//
	INFO(W->mode.M,"Mode")
	INFO(W->PV,"PV")
	INFO(W->MV,"MV")
//
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
  BIAS0 = false;
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_RATIO,CRATIO,50,75)

void CY_RATIO::InitParm()
  {
#include "Yokogawa\RATIO_P.hpp" 
#include "Yokogawa\RATIO.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_RATIO::GetOUT ( char * Name )
	{
  GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_RATIO::GetIN ( char * Name )
	{
	GET_IN(IN)
	GET_IN(SET)
	GET_IN(TIN)
	GET_IN(TSI)
	GET_IN(INT)
	return NULL;
	}

void CY_RATIO::Work ( double dt )
{
	SET_HBP
		{
		KKK();
		}

	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    OUT.Status |= CSignal::BAD;
		return ;
    }
  else
    OUT.Status &= ~CSignal::BAD;
  if ( BIAS0 )
    W->BIAS = 0.;
	if ( OUT.Status & CSignal::CND )
		{
		W->mode.IMAN = true;
// Track output
		}
	else
		{
		W->mode.IMAN = false;
		}
//
	if ( IN )
    *IN >> W->PV;
		//ObrIn.Work ( IN, W->PV, dPVn );
//
  double CSV = W->SV;
  if ( W->mode.M != SMode::CAS )
    CSV = W->SV;

  if ( W->mode.M == SMode::RCAS )
    CSV = W->RSV;
  else
    W->RSV = W->SV;

  if ( W->mode.M == SMode::ROUT )
    W->MV = W->RMV;
  else 
    W->RMV = W->MV;

  if ( SET && W->mode.M == SMode::CAS )
		*SET >> CSV;
  /*else
    CSV = W->SV;*/


	bool RatioTRK = false;
	if ( W->mode.M == SMode::MAN && W->Trk_MAN )
		RatioTRK = true;
	if ( W->mode.M == SMode::AUT && W->mode.IMAN && W->Trk_AUT_CND )
		RatioTRK = true;
	if ( W->mode.M == SMode::CAS && W->mode.IMAN && W->Trk_CAS_CND )
		RatioTRK = true;
	SET_HBP
		{
		KKK();
		}
	if ( RatioTRK )
	{
// Ratio Tracking
		W->SV = ( W->MV - W->BIAS ) / ( W->PV * W->GAIN ); 
	}
	else if ( W->mode.M == SMode::CAS || W->mode.M == SMode::RCAS )
	{
	  W->SV = CSV;
	}                        

  if ( W->SV > W->SSH )
    W->SV = W->SSH;
  if ( W->SV < W->SSL )
    W->SV = W->SSL;

	if ( RatioTRK )
		{
		SVe = W->SV;
		}
	else
		{
//SV ramp p 254
		if ( W->mode.M == SMode::MAN )
			{
			SVe = ( W->MV - W->BIAS ) / ( W->PV * W->GAIN );
      if ( !_finite ( SVe ))
        SVe = 0.;
      if ( SVe > W->SH )
        SVe = W->SH;
      if ( SVe < W->SL )
        SVe = W->SL;
			W->SV = SVe;
			}
		else
			{
			double dS = W->SV - SVe;
			if ( dS > W->RP )
				dS = W->RP;
			if ( dS < -W->RP )
				dS = -W->RP;
			SVe += dS;
			}
		}
  if ( !_finite ( SVe ))
    SVe = 0.;
  if ( !_finite ( W->MV ))
    W->MV = 0.;
	//

  double CALC = 0;
  if( W->mode.M == SMode::CAS )
  {
    CALC = W->GAIN * SVe * W->PV + W->BIAS;
    dMV = CALC - W->MV;
  }
  else
  {
    if( abs(SVe) > 0.00000001 )
      CALC = W->GAIN * SVe * W->PV + W->BIAS;
    else
	    CALC = W->GAIN * W->PV + W->BIAS;
	  dMV = CALC - W->MV;
  }
  if( W->mode.M == SMode::CAS || W->mode.M == SMode::AUT )
    W->MV = CALC;
  W->CALC = CALC;
  if( W->mode.IMAN )
	  ObrOut.OutputProcessing( false );
}

void CY_RATIO::Init( )
	{
	switch ( W->Aux )//PV, dPV, MV, DMV
		{
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
	ASS(SUB.pDbl!=NULL)
	}

int CY_RATIO::Prepare0()
	{
	W->RP = W->SVH - W->SVL;
	return 0;
	}

int CY_RATIO::Prepare1()
{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\RATIO_P.hpp" 
	if ( W->RP == 0. )
		W->RP = W->SH - W->SL;
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
	return 0;
}

int CY_RATIO::SaveState( )
{
	OUT.SaveState();
	SUB.SaveState();
	SAVE_CLASS("W",CY_RATIO_W)
	return 0;
}

int CY_RATIO::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_RATIO_W)
	return 1;
}

int CY_RATIO::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 3 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dPVn", dPVn );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "SVe", SVe );

  return 0;
}

int CY_RATIO::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 3 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dPVn", dPVn );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "SVe", SVe );

  return 0;
}

int CY_RATIO::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODE ( W->mode, "#Mode" );
	PARM ( W->PV, "#PV" );
	PARM ( W->SV, "#SV" );
	PARM ( W->MV, "#MV" );
	PARM ( SVe, "#SVe" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\RATIO_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\RATIO.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(IN)
	PARM_IN(SET)
	PARM_IN(TIN)
	PARM_IN(TSI)
	PARM_IN(INT)
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_RATIO::SetLimits( )
  {
	if ( IN )
		{
		IN ->Min = W->SL;
		IN ->Max = W->SH;
		IN ->IsLimits = true;
		}
	if ( SET )
		{
		SET ->Min = W->SVL;
		if (W->SSL > SET ->Min)
			SET ->Min = W->SSL;
		SET ->Max = W->SVH;
		if (W->SSH < SET ->Max)
			SET ->Max = W->SSH;
		SET ->IsLimits = true;
		}
  if ( !OUT.IsLimits )
    return false;
  W->MSH = W->MH = OUT.Max;
  W->MSL = W->ML = OUT.Min;
  return true;
  }

