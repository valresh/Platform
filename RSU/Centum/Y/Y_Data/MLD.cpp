#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"


static CY_Create MLD ( "MLD", CY_MLD::Create );

static int C2 = 2;

CY_MLD_W::CY_MLD_W()
{
	memset ( this, 0, sizeof ( *this ));
}

CY_MLD::CY_MLD(CMLD * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	//INIT(MV)// Manipulated output value (*1) MV engineering unit value MSL
	//INIT(MH)// Manipulated variable high-limit setpoint MSL to MSH MSH
	//INIT(ML)// Manipulated variable low-limit setpoint MSL to MSH MSL
	//INIT(TSW)// Tracking switch 0, 1 0
	//INIT(RSW)// Pulse width reset switch 0, 1 0
//
	TIN = NULL;
	TSI = NULL;
	OUT.Name = "OUT";
  OUT << W->MV;
	SUB.Name = "SUB";
  SUB.Null();
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
  ObrOut.pOutTrk = &W->Out_Trk;
  ObrOut.pMode = &W->mode.M;
  ObrOut.pMAN_bypass = &W->MAN_bypass;
  ObrOut.pOut_VL = &W->Out_VL;
  ObrOut.pLimit_On_CLP = &W->Limit_On_CLP;
  ObrOut.pPositional = &W->Positional;
	ObrOut.OutputLimiter = true;
  GetOut = false;
	Start = false;
	}

void CY_MLD::ResetAlarms()
{
  Alarm.SetBlk( W );
}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_MLD,CMLD,30,50)

void CY_MLD::InitParm()
  {
#include "Yokogawa\MLD_P.hpp" 
#include "Yokogawa\MLD.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_MLD::GetOUT ( char * Name )
	{
  GETOUT
	GETSUB
	return NULL;
	}

CSignal ** CY_MLD::GetIN ( char * Name )
	{
	GET_IN(TIN)
	GET_IN(TSI)
	return NULL;
	}

void CY_MLD::Work ( double dt )
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
	if ( OUT.Status & CSignal::OOP )
		Alarm.SEND_ON( A_OOP, -1. );
	else
		Alarm.SEND_OFF( A_OOP );
//  if ( GetOut )
//    {
//    GetOut = false;
//		if ( pKlap )
//			W->MV = pKlap->Vent;
//		else
//			W->MV = OUT.BackVar;
//    }
  if ( OUT.IsLimits )
    {
    W->MH = W->MSH = OUT.Max;
    W->ML = OUT.Min;
    }

  if( TSI )
    *TSI >> W->TSW;

  if( TIN && W->TSW )
  {
    *TIN >> W->MV;
    W->mode.TRK = true;
  }
  else
    W->mode.TRK = false;

  if( W->MVH > W->MVL )
  {
    if( W->MV > W->MVH )
      W->MV = W->MVH;
    if( W->MV < W->MVL )
      W->MV = W->MVL;
  }

  ObrOut.OutputProcessing( false );
  if( 2==W->OutputSignalConvertion && W->Conv_GAIN != 0 )
    subsysMV = ( W->MV - W->Conv_BIAS ) / W->Conv_GAIN;
}

void CY_MLD::Init( )
{
	switch( W->Aux )//MV, DMV
	{
	case 0:
		SUB << W->MV;
		break;
	case 1:
		SUB << dMV;
		break;
  default:
    ASSD(0);
	}
  switch( W->OutputSignalConvertion )
  {
  case 2:
    if( W->Conv_GAIN != 0 )
      OUT << subsysMV;
  case 0:
    break;
  default:
    ASSD(0);
  }
}

int CY_MLD::Prepare0()
{
  if( DBL_MAX!=W->MVH && DBL_MAX==W->MSH )
    W->MSH = W->MVH;
  if( DBL_MAX!=W->MVL && DBL_MAX==W->MSL )
    W->MSL = W->MVL;
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\MLD_P.hpp" 
	Alarm.SetBlk( W );
	Alarm.pAlarmLevel = &C2;
	return 0;
}

int CY_MLD::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
	  W->mode.M = SMode::AUT;
  //W->mode.TRK = false;
	Start = true;
  //if ( Res )
  //  {
  //  W->MV = pKlap->Vent;
  //  }
	return 0;
}

int CY_MLD::SaveState( )
{
	SAVE_CLASS("W",CY_MLD_W)
  ObrOut.SaveState();
	OUT.SaveState();
	SUB.SaveState();
	return 0;
}

int CY_MLD::RestoreState ( char * StrName )
{
	if ( CY_Base::RestoreState ( StrName ) == 0 )
		return 0;
	if ( OUT.RestoreState ( StrName ) == 0 )
		return 0;
	if ( SUB.RestoreState ( StrName ) == 0 )
		return 0;
	REST_CLASS("W",CY_MLD_W)
  if ( ObrOut.RestoreState ( StrName ) == 0 )
    return 0;
	return 1;
}

int CY_MLD::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );
  ObrOut.StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  WriteFullStruct( pStateSer, eKeyAnyVar, "subsysMV", subsysMV );

  return 0;
}

int CY_MLD::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );
  ObrOut.StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "dMV", dMV );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "subsysMV", subsysMV );
  return 0;
}

int CY_MLD::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODEFULL( W->mode, "#Mode" );
	PARM ( W->MV, "#MV" );
  PARM ( GetOut, "#Взять выход" );
	PARM ( W->Comment, "#Что это" );
	if ( pDR && pDR->FCS )
		PARM_CALLBACK( pDR->FCS, "#FCS" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\MLD_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\MLD.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(TIN)
	PARM_IN(TSI)
	PARM_OUT
	PARM_SUB
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	PARM ( W->Tune_MV, "#Тип:MV" )
	return 0;
	}

bool CY_MLD::SetLimits( )
  {
  //IN ->Min = W->SL;
  //IN ->Max = W->SH;
  //IN ->IsLimits = true;
  if ( OUT.pTo == NULL )
    return true;
  if ( !OUT.IsLimits )
    return false;
  W->MSH = W->MH = OUT.Max;
  W->MSL = W->ML = OUT.Min;
  return true;
  }
