#include "stdafx.h"
#include "Y_Data.h"
#include "math.h"
#include <rsuErr.h>

#include "StdDef.h"


CY_OUT::CY_OUT()
	{
	OutputLimiter = true;
	IsMLe = false;
	IsMHe = false;
	MLe = 0.;
	MHe = 100.;
	DataStatus = 0;
	OutputStatus = 0;
//-----------------------------------
	OUT = NULL;
  minusClampVal = -DBL_MAX;
  plusClampVal = DBL_MAX;
	}

void CY_OUT::OutputProcessing( bool Reverse, bool TRK )
	{
// Output Range Tracking
  if ( OUT->IsLimits )
  {
    if( *pMH > OUT->Max )
      *pMH = OUT->Max;
    if( *pMSH == DBL_MAX )
      *pMSH = OUT->Max;
    if( *pML < OUT->Min )
      *pML = OUT->Min;
    if( *pMSL == DBL_MAX )
      *pMSL = OUT->Min;
  }
// Output Tracking
	if ( *pIMAN )
		{
		if ( pOutTrk )
			{
// MLD, .. мб задан
			if ( *pOutTrk )
				*pMV = OUT->BackVar;
			}
		else
			*pMV = OUT->BackVar;
		}
	else
		{
		if ( *pMode == SMode::MAN )
      {
      ModeM( Reverse );
      }
    else
      {
        //C6.1.2 Compound Block Mode описана ситуация AUT с TRK
        if( !TRK )
          ModeAC( Reverse );
      }
		}
	}

void CY_OUT::ModeM( bool Reverse )
{
  DataStatus = 0;
	if ( !*pMAN_bypass )
		{
		double dV = *pOut_VL * 0.01 * ( *pMSH - *pMSL );
		if ( *pdMV > dV )
			*pdMV = dV;
		if ( *pdMV < -dV )
			*pdMV = -dV;
		}
  *pMV += *pdMV;
  if( !pMH || !pML )
    return;
  if ( OutputLimiter && *pMH > *pML )// только у регуляторов
  {
// В 'M' MV не обрезется
    if ( *pMV < *pML )
      {
      IsMLe = true;
      MLe = *pMV;
      }
    else
      IsMLe = false;
    if ( *pMV > *pMH )
      {
      IsMHe = true;
      MHe = *pMV;
      }
    else
      IsMHe = false;
  }
}

void CY_OUT::ModeAC( bool Reverse )
  {
// Preset не реализован
// Ограничение dM
//  По скорости
	double dV = *pOut_VL * 0.01 * ( *pMSH - *pMSL );
	if ( *pdMV > dV )
		*pdMV = dV;
	if ( *pdMV < -dV )
		*pdMV = -dV;
//
	OutputStatus = 0;
  if ( *pLimit_On_CLP )
    {
	  if ( OUT->IsCLPm() )
      {
	    if ( *pdMV < 0. )
      {
        if( -DBL_MAX==minusClampVal )
          minusClampVal = *pMV;
        *pdMV = 0.;
      }
			if ( Reverse )
				OutputStatus = CSignal::CLPm;
			else
				OutputStatus = CSignal::CLPp;
      }
    else
      minusClampVal = -DBL_MAX;
	  if ( OUT->IsCLPp() )
      {
	    if ( *pdMV > 0. )
      {
        if( DBL_MAX==plusClampVal )
          plusClampVal = *pMV;
        *pdMV = 0.;
      }
			if ( Reverse )
				OutputStatus = CSignal::CLPp;
			else
				OutputStatus = CSignal::CLPm;
      }
    else
      plusClampVal = DBL_MAX;
    }
  
  CorrectMVbyNullDMV();

  if ( *pPositional )
    *pMV += *pdMV;
  else
  {
    if( OUT->pTo[0] )
      *pMV = OUT->BackVar + *pdMV;
    else
      *pMV += *pdMV;
  }
  if ( OutputLimiter && *pMH > *pML )// только у регуляторов
  {
    Clamp( );
    if ( IsMLe )
      {
      if ( *pMV >= *pML )
        IsMLe = false;
      else
        MLe = 0.9 * MLe + 0.1 * *pML;
      }
    if ( IsMHe )
      {
      if ( *pMV <= *pMH )
        IsMHe = false;
      else
        MHe = 0.9 * MHe + 0.1 * *pMH;
      }
  }
  else
  {
    DataStatus = 0;
  }
  }

// OutputStatus OUT выставляет приемник
void CY_OUT::Clamp( )
	{
	if ( *pMV < *pML )
		{
    DataStatus = CSignal::CLPm;
    if ( IsMLe )
      {
      if ( *pMV < MLe )
        *pMV = MLe;
      }
    else
		  *pMV = *pML;
		return;
		}
	if ( DataStatus & CSignal::CLPm )
		{
		double dHYS = 0.02 * ( *pMH - *pML );
		if ( *pMV < *pML + dHYS )
			return;
		}
	if ( *pMV > *pMH )
		{
    DataStatus = CSignal::CLPp;
    if ( IsMHe )
      {
      if ( *pMV > MHe )
        *pMV = MHe;
      }
    else
		  *pMV = *pMH;
		return;
		}
	if ( DataStatus & CSignal::CLPp )
		{
		double dHYS = 0.02 * ( *pMH - *pML );
		if ( *pMV > *pMH - dHYS )
			return;
		}
  DataStatus = 0;
	}

static LPCSTR s_Name = "CY_OUT";

int CY_OUT::SaveState( )
  {
  SAVE_CLASS ( s_Name, CY_OUT_W );
  return 0;
  }

int CY_OUT::RestoreState ( char * StrName )
  {
  REST_CLASS ( s_Name, CY_OUT_W );
  return 1;
  }

int CY_OUT::StateSave( IYokoStateSer* pStateSer )
{
  WriteFullStruct( pStateSer, eKeyLocalStruct, s_Name, *(CY_OUT_W*)this );
  return 0;
}

int CY_OUT::StateRestore( IYokoStateSer* pStateSer )
{
  ReadNamedFullStruct( pStateSer, eKeyLocalStruct, s_Name, *(CY_OUT_W*)this );
  return 0;
}

void CY_OUT::CorrectMVbyNullDMV()
{
  if( !pdMV || !pMV )
    return;
  if( 0==(*pdMV) && 0!=*pMV )
  {
    double intptr = 0;
    double frac = modf( *pMV, &intptr );
    if( frac>0.999)
    {
      if( *pMV > 0 )
        *pMV = intptr + 1.;
      else
        *pMV = intptr - 1.;
    }
    else if( frac < 0.001 )
      *pMV = intptr;
  }
}
