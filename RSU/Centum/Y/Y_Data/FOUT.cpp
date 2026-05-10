#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include <ConnectY.h>
#include "ReadParms.h"

static CY_Create FOUT ( "FOUT", CY_FOUT::Create );

CY_FOUT::CY_FOUT( CFOUT * _W ) : CY_Base()
	{
  W = _W;
  CY_Base::InitFlagUse( W->FlagUse );
  CY_Base::pComm = W->Comment;
  //Sergej CY_Base::pMode = &W->mode;
	SET = NULL;
	kOut = 0;
	SET_OUT("J01",W->MV1)
	SET_OUT("J02",W->MV2)
	SET_OUT("J03",W->MV3)
	SET_OUT("J04",W->MV4)
	SET_OUT("J05",W->MV5)
	SET_OUT("J06",W->MV6)
	SET_OUT("J07",W->MV7)
	SET_OUT("J08",W->MV8)
	double * MSH = &MSH1;
	double * MSL = &MSL1;
	for ( int n = 0; n < 8; n++ )
		{
		MSH[n] = 100.;
		MSL[n] = 0.;
		}
	}

#include "Yokogawa\YPARM_INIT.h" 

LIST_PARM(CY_FOUT,CFOUT,30,50)

void CY_FOUT::InitParm()
  {
#include "Yokogawa\FOUT_P.hpp" 
#include "Yokogawa\FOUT.hpp"
  qsort ( ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  qsort ( VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }

CSignal * CY_FOUT::GetOUT ( char * Name )
	{
	GET_STD_OUT
	return NULL;
	}

CSignal ** CY_FOUT::GetIN ( char * Name )
	{
	GET_IN(SET)
  return NULL;
	}

/*inline double Direct ( double SV, double SV_min, double SV_max,
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
  }*/

void CY_FOUT::Work ( double dt )
	{
	SET_HBP
		{
		KKK();
		}
  //
  Out[0].Status &= ~CSignal::BAD;
	if( SET )
  {
    *SET >> W->CSV;
    W->SV = W->CSV;
  }

  bool IMAN = true;

  double MV = DBL_MAX;

	double * MSH = &MSH1;
	double * MSL = &MSL1;
	double * pMV = &W->MV1;
	const double M = ( W->SV - W->SSL ) / ( W->SSH - W->SSL );
	for( int n = 0; n < _countof(Out); n++ )
	{
    if( !Out[n].pTo[0] )
      continue;
    if( Out[n].IsLimits )
    {
      MSH[n] = Out[n].Max;
      MSL[n] = Out[n].Min;
    }
		double V = ( MSH[n] - MSL[n] ) * M + MSL[n];
		pMV[n] = V;
    if( !(Out[n].Status & CSignal::CND) )
      IMAN = false;
    if( DBL_MAX==MV )
    {
      if( Out[n].IsLimits )
        MV = Out[n].BackVar;//(Out[n].BackVar - MSL[n]) / ( MSH[n] - MSL[n] );
    }
  }

  if( SET )
  {
    if( DBL_MAX!=MV )
      SET->BackVar = MV;
    else
      SET->BackVar = W->SV;

    if( IMAN)
      SET->On( CSignal::CND );
    else
      SET->Off( CSignal::CND );
  }
  W->mode.IMAN = IMAN;
}

void CY_FOUT::Init( )
	{
	}

int CY_FOUT::Prepare0()
	{
#include "Yokogawa\YPARM_TEST.h" 
#include "Yokogawa\FOUT_P.hpp" 
	return 0;
	}

int CY_FOUT::Prepare1()
{
  W->mode.M = W->mode.M & SMode::_MODE_MASK;
	double * MSH = &MSH1;
	double * MSL = &MSL1;
	for ( int n = 0; n < 8; n++ )
		{
		MSH[n] = 100.;
		MSL[n] = 0.;
		}
	return 0;
}

int CY_FOUT::SaveState( )
	{
	SAVE_OUT
	SAVE_CLASS("W",CY_FOUT_W)
	return 0;
	}

int CY_FOUT::RestoreState ( char * StrName )
	{
	REST_OUT
//  IGNORE
	REST_CLASS("W",CY_FOUT_W)
	return 1;
	}

int CY_FOUT::GetParams( KYBridge2SysParam &params )
	{
#include "IO_Parms.h"
	PARM ( W->SV, "#SV" );
	MENU("Все параметры")
#include "Yokogawa\YPARM_PARM.h" 
#include "Yokogawa\FOUT_P.hpp" 
	E_MENU("Все параметры")
	MENU("Все состояние")
#include "Yokogawa\FOUT.hpp"
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

bool CY_FOUT::SetLimits( )
  {
  if ( SET )
    {
    SET->Min = W->SSL;
    SET->Max = W->SSH;
    SET->IsLimits = true;
    }
	double * MSH = &MSH1;
	double * MSL = &MSL1;
  for ( int n = 0; n < _countof(Out); n++ )
    {
    if ( Out[n].pTo[0] == NULL )
      continue;
    if ( !Out[n].IsLimits )
      return false;
		MSH[n] = Out[n].Max;
		MSL[n] = Out[n].Min;
    }
  return true;
  }

