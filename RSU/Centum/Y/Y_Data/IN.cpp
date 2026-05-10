#include "stdafx.h"
#include "Y_Data.h"
#include "math.h"
#include <rsuErr.h>
#include "StdDef.h"
#include "GlobalSettings.h"

CY_IN::CY_IN()
{
  memset ( this, 0, sizeof ( CY_IN ));
  GAIN = 1.;
}

void CY_IN::PV_Overshoot ( CSignal * IN, double & PV, bool * pBad, BYTE cali )
	{
    if( cali )
      return;
	if ( IN	== NULL )
		{
		if ( pBad )
			*pBad = true;
		return ;
		}
  double PV_new = 0;
  if ( IN->IsBAD() )
    {
		if ( pBad )
			*pBad = true;
    if ( *pPV_Overshoot )
      {
      if ( IN->IsIOPp() )
        PV_new = *pSH;
			else
        PV_new = *pSL;
      }
    else
      PV_new = PV;
    }
  else
		{
		if ( pBad )
			*pBad = false;
		if ( IN->Type == CSignal::NoType )
			PV_new = 0.;
		else
			{
			if ( IN->Type == CSignal::Double )
				*IN >> PV_new;
			else
				{
				if ( IN->Type == CSignal::Int )
					{
					int I;
					*IN >> I;
					PV_new = I;
					}
				}
			}
		}
  PV_new *= GAIN;
  PV_new += BIAS;
  PV = PV_new;
}

void CY_IN::Work( CSignal * IN, double & PV, double & dPV, BYTE cali )
	{
  if( cali )
    return;
  double PV_new;
  if( !IN || !pSH || !pSL )
    return;
  if ( IN->IsBAD() )
    {
    if ( pPV_Overshoot && *pPV_Overshoot )
      {
      if ( IN->IsIOPp() )
        PV_new = *pSH;
			else
        PV_new = *pSL;
      }
    else
      PV_new = PV;
    }
  else
		{
		if ( IN->Type == CSignal::NoType )
			PV_new = 0.;
		else
      {
			*IN >> PV_new;
      PV_new *= GAIN;
      PV_new += BIAS;
      }
		}
  if( !(IN->Status & CSignal::NONE_LIMITED) && (*pSH > *pSL) )
  {
    if( 1==KGlobalSettings::Instance().PV_LIMIT() || (pPV_Limit && *pPV_Limit) )
      {
      if ( PV_new > *pSH )
        PV_new = *pSH;
      if ( PV_new < *pSL )
        PV_new = *pSL;
      }
    else
    {
      double l = (*pSH - *pSL) * 0.25;
      if ( PV_new > (*pSH+l) )
      {
        double s = (*pSH - *pSL);
        if( ((PV_new-*pSH)/10.) < s )
          PV_new = *pSH+l;
        else
          KKK();
      }
      if ( PV_new < (*pSL-l) )
      {
        PV_new = *pSL-l;
      }
    }
  }
	dPV = PV_new - PV;
  PV = PV_new;
//  if (( IN->Status & CSignal::IOPp ) == 0 && ( IN->Status & CSignal::IOPm ) == 0 )
    IN->BackVar = PV_new;
	}

void CY_IN::WorkScaled( CSignal * IN, double & PV, double & dPV, double A, BYTE cali )
{
  if( 0==A )
    return Work( IN, PV, dPV, cali );

  if( cali )
    return;

  double PV_new;
  if( !IN || !pSH || !pSL )
    return;
  if ( IN->IsBAD() )
  {
    if ( pPV_Overshoot && *pPV_Overshoot )
    {
      if ( IN->IsIOPp() )
        PV_new = *pSH;
      else
        PV_new = *pSL;
    }
    else
      PV_new = PV;
  }
  else
  {
    if ( IN->Type == CSignal::NoType )
      PV_new = 0.;
    else
    {
      *IN >> PV_new;
      PV_new *= A;
      PV_new *= GAIN;
      PV_new += BIAS;
    }
  }

  if( !(IN->Status & CSignal::NONE_LIMITED) && *pSH > *pSL )
  {
    if( 1==KGlobalSettings::Instance().PV_LIMIT() || (pPV_Limit && *pPV_Limit) )
    {
      if ( PV_new > *pSH )
        PV_new = *pSH;
      if ( PV_new < *pSL )
        PV_new = *pSL;
    }
    else
    {
      double l = (*pSH - *pSL) * 0.25;
      if ( PV_new > (*pSH+l) )
      {
        double s = (*pSH - *pSL);
        if( ((PV_new-*pSH)/10.) < s )
          PV_new = *pSH+l;
        else
          KKK();
      }
      if ( PV_new < (*pSL-l) )
      {
        PV_new = *pSL-l;
      }
    }
  }
  dPV = PV_new - PV;
  PV = PV_new;
  //  if (( IN->Status & CSignal::IOPp ) == 0 && ( IN->Status & CSignal::IOPm ) == 0 )
  IN->BackVar = PV_new;
}
