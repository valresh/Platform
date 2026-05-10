#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create SPLIT ( "SPLIT", CY_SPLIT::Create );

CY_SPLIT::CY_SPLIT( CSPLIT * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  CY_Base::pMode = &W->mode;
	SET = NULL;
	kOut = 0;
	SET_OUT("OUT1",W->MV1)
	SET_OUT("OUT2",W->MV2)
  BL1 = BL2 = 0.;
  oldMODE = ' ';
  INFO(W->SV,"SV")
  INFO(W->MV1,"MV1")
  INFO(W->MV2,"MV2")
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_SPLIT,CSPLIT,30,50)

void CY_SPLIT::InitParm()
  {
#include "Yokogawa\SPLIT_P.hpp" 
#include "Yokogawa\SPLIT.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_SPLIT::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_SPLIT::GetIN ( char * Name )
	{
	GET_IN(SET)
  return NULL;
	}

inline double Direct ( double SV, double SV_min, double SV_max,
                       double MV_min, double MV_max )
  {
  if ( SV < SV_min )
    return MV_min;
  if ( SV > SV_max )
    return MV_max;
  return MV_min + ( MV_max - MV_min ) / ( SV_max - SV_min ) * ( SV - SV_min ); 
  }

inline double Reverse ( double SV, double SV_min, double SV_max,
                        double MV_min, double MV_max )
  {
  if ( SV < SV_min )
    return MV_max;
  if ( SV > SV_max )
    return MV_min;
  return MV_max - ( MV_max - MV_min ) / ( SV_max - SV_min ) * ( SV - SV_min ); 
  }

void CY_SPLIT::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	if ( W->mode.M == SMode::OS || W->mode.M < SMode::ROUT )
    {
    W->mode.M = SMode::OS;
    Out[0].Status |= CSignal::BAD;
		return ;
    }
  else
    Out[0].Status &= ~CSignal::BAD;
	if ( SET )
		*SET >> W->CSV;
  bool Calc_BL = false;
  if ( W->mode.M == SMode::CAS )
    {
    if ( oldMODE == ' ' )
      oldMODE = 'C';
    if ( oldMODE != 'C' )
      Calc_BL = true;
    oldMODE = 'C';
    if ( W->CSV > W->SSH )
      W->CSV = W->SSH;
    if ( W->CSV < W->SSL )
      W->CSV = W->SSL;
    W->SV = W->CSV;
    }
  else
    {
    W->mode.M = SMode::AUT;
    oldMODE = 'A';
    W->CSV = W->SV;
    }
	
	int SW = W -> SW;
  bool IMAN = false;
  Out[0].Off( CSignal::USED );
  Out[1].Off( CSignal::USED );
  double bv1 = DBL_MAX, bv2 = DBL_MAX;

  switch( SW )
  {
  case 1:
    if ( Out[0].Status & CSignal::CND )
      IMAN = true;
    Out[0].On( CSignal::USED );
    break;
  case 2:
    if ( Out[1].Status & CSignal::CND )
      IMAN = true;
    Out[1].On( CSignal::USED );
    break;
  case 3:
    if (( Out[0].Status & CSignal::CND )&&( Out[1].Status & CSignal::CND ))
      IMAN = true;
    Out[0].On( CSignal::USED );
    Out[1].On( CSignal::USED );
    break;
  }

  W->mode.IMAN = IMAN;
	if ( SET )
	{
		bool CAS = W->mode.M == SMode::CAS || W->mode.M == SMode::AUT;
		if ( IMAN || !CAS || SW == 0 )
			SET->On ( CSignal::CND );
		else
			SET->Off ( CSignal::CND );
	}

	if ( SW == 1 || SW == 3	)
		{
		double CAL1;
		if ( W->MV1_direct )
      CAL1 = Direct ( W->SV, W->SRL1, W->SRH1, W->MSL1, W->MSH1 );
		else
      CAL1 = Reverse ( W->SV, W->SRL1, W->SRH1, W->MSL1, W->MSH1 );
    if ( Calc_BL )
      BL1 = W ->MV1 - CAL1;
		W ->MV1 = CAL1 + BL1;
    if( Out[0].pTo[0] && Out[0].IsLimits )
      bv1 = Out[0].BackVar;
		}
	if ( SW == 2 || SW == 3	)
	{
		double CAL2;
		if ( W->MV2_direct )
      CAL2 = Direct ( W->SV, W->SRL2, W->SRH2, W->MSL2, W->MSH2 );
		else
      CAL2 = Reverse ( W->SV, W->SRL2, W->SRH2, W->MSL2, W->MSH2 );
    if ( Calc_BL )
      BL2 = W ->MV2 - CAL2;
		W ->MV2 = CAL2 + BL2;
    if( Out[1].pTo[0] && Out[1].IsLimits )
      bv2 = Out[1].BackVar;
	}
  if ( SET )
  {
    if( DBL_MAX==bv1 && DBL_MAX==bv2 )
      SET->BackVar = W->CSV;
    else if( DBL_MAX!=bv1 )
      SET->BackVar = bv1;
    else if( DBL_MAX!=bv2 )
      SET->BackVar = bv2;
  }
  if ( BL1 > 0. )
    {
    BL1 -= W->RP1;
    if ( BL1 < 0. )
      BL1 = 0.;
    }
  else
    {
    if ( BL1 < 0. )
      {
      BL1 += W->RP1;
      if ( BL1 > 0. )
        BL1 = 0.;
      }
    }
  if ( BL2 > 0. )
    {
    BL2 -= W->RP2;
    if ( BL2 < 0. )
      BL2 = 0.;
    }
  else
    {
    if ( BL2 < 0. )
      {
      BL2 += W->RP2;
      if ( BL2 > 0. )
        BL2 = 0.;
      }
    }
  int CLP = Clamp();
	if ( SET )
		SET->Set( CSignal::CLPp | CSignal::CLPm, CLP );
	}

int CY_SPLIT::Clamp()
  {
	int SW = W -> SW;
  if ( SW == 0 )
    return 0;
  if ( SW == 1 )
    {
    if ( Out[0].IsCLPp() )
      {
      if ( W->MV1_direct )
        return CSignal::CLPp;
      else
        return CSignal::CLPm;
      }
    if ( Out[0].IsCLPm() )
      {
      if ( W->MV1_direct )
        return CSignal::CLPm;
      else
        return CSignal::CLPp;
      }
    }
  if ( SW == 2 )
    {
    if ( Out[1].IsCLPp() )
      {
      if ( W->MV2_direct )
        return CSignal::CLPp;
      else
        return CSignal::CLPm;
      }
    if ( Out[1].IsCLPm() )
      {
      if ( W->MV2_direct )
        return CSignal::CLPm;
      else
        return CSignal::CLPp;
      }
    }
  if ( SW == 3 )
    {
    int CLP1 = 0;
    if ( Out[0].IsCLPp() )
      {
      if ( W->MV1_direct )
        CLP1 = CSignal::CLPp;
      else
        CLP1 = CSignal::CLPm;
      }
    if ( Out[0].IsCLPm() )
      {
      if ( W->MV1_direct )
        CLP1 = CSignal::CLPm;
      else
        CLP1 = CSignal::CLPp;
      }
    int CLP2 = 0;
    if ( Out[1].IsCLPp() )
      {
      if ( W->MV2_direct )
        CLP2 = CSignal::CLPp;
      else
        CLP2 = CSignal::CLPm;
      }
    if ( Out[1].IsCLPm() )
      {
      if ( W->MV2_direct )
        CLP2 = CSignal::CLPm;
      else
        CLP2 = CSignal::CLPp;
      }
    if ( CLP1 == CLP2 )
      return CLP1;
    else
      return 0;
    }
  return 0;
  }

void CY_SPLIT::Init( )
	{
	}

int CY_SPLIT::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\SPLIT_P.hpp" 
	return 0;
	}

int CY_SPLIT::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	if ( W->mode.M < SMode::ROUT )
  {
    if( SET )
      W->mode.M = SMode::CAS;
    else
      W->mode.M = SMode::AUT;
    W->SW = 3;
  }
//
  if ( W->RP1 == 0. )
    W->RP1 = W->MV1H;
  if ( W->RP2 == 0. )
    W->RP2 = W->MV2H;
	return 0;
}

int CY_SPLIT::SaveState( )
	{
	SAVE_OUT
	SAVE_CLASS("W",CY_SPLIT_W)
	return 0;
	}

int CY_SPLIT::RestoreState ( char * StrName )
	{
	REST_OUT
	REST_CLASS("W",CY_SPLIT_W)
	return 1;
	}

int CY_SPLIT::StateSave( IYokoStateSer* pStateSer )
{
  __super::StateSave( pStateSer );

  pStateSer->SimpleWrite( eKeyVarCount );
  pStateSer->SimpleWrite( 2 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "BL1", BL1 );
  WriteFullStruct( pStateSer, eKeyAnyVar, "BL2", BL2 );

  return 0;
}

int CY_SPLIT::StateRestore( IYokoStateSer* pStateSer )
{
  __super::StateRestore( pStateSer );

  ReadTest( pYokoStateSer, eKeyVarCount );
  ReadTest( pYokoStateSer, 2 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "BL1", BL1 );
  ReadNamedFullStruct( pStateSer, eKeyAnyVar, "BL2", BL2 );

  return 0;
}

int CY_SPLIT::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM_MODEFULL( W->mode, "#Mode" );
	PARM ( W->SV, "#SV" );
	PARM ( W->SW, "#SW" );
	PARM ( W->MV1, "#MV1" );
	PARM ( W->MV2, "#MV2" );
  PARM(W->MV1_direct, "#MV1_direct" ) 
  PARM(W->SRL1, "#SRL1" )
  PARM(W->SRH1, "#SRH1" ) 
  PARM(W->MSL1, "#MSL1" )
  PARM(W->MSH1, "#MSH1" )
  PARM(W->MV2_direct, "#MV2_direct" ) 
  PARM(W->SRL2, "#SRL2" )
  PARM(W->SRH2, "#SRH2" ) 
  PARM(W->MSL2, "#MSL2" )
  PARM(W->MSH2, "#MSH2" )
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\SPLIT_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\SPLIT.hpp"
	E_MENU("Все состояние")
	MENU("Входы")
	PARM_IN(SET)
	PARM_ALL_OUT
	E_MENU("Входы")
	MENU("Base")
	CY_Base::GetParams( params );
	E_MENU("Base")
	return 0;
	}

bool CY_SPLIT::SetLimits( )
  {
  if ( SET )
    {
    SET->Min = W->SSL;
    SET->Max = W->SSH;
    SET->IsLimits = true;
    }
  bool OK = true;
  if ( Out[0].pTo[0] )
    {
    if ( Out[0].IsLimits )
      {
      W->MSH1 = Out[0].Max;
      W->MSL1 = Out[0].Min;
      }
    else
      OK = false;
    }
  if ( Out[1].pTo[0] )
    {
    if ( Out[1].IsLimits )
      {
      W->MSH2 = Out[1].Max;
      W->MSL2 = Out[1].Min;
      }
    else
      OK = false;
    }
  return OK;
  }

