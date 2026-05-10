#include "stdafx.h"
#define DLL_TBP
#include "tbp.h"
#include "math.h"
#include "Err.h"
#include "Chem.h"
#include "Data.h"


CTBP::CTBP() : Src ( "Источник" ), Dst ( "Приемник" ) 
  {
	P = 1.;
	T_cool = 15.;
  }

#define CPY(Dst,Src) 	memcpy ( Dst, Src, sizeof ( Dst ));

bool CTBP::Calc ( CComp * pComp, int kPnt, double _V[], double _T[] )
	{
	double T_min = 1000.;
	double T_max = -1000.;
	CPY( IsComp, pComp->IsComp )
	CPY( SummSrc, pComp->Cmol );
  FINITE(pComp->Cmol[34])
	for ( int n = G_N2; n <= G_H2O; n++ )
		{
		IsComp[n] = false;
		SummSrc[n] = 0.;
		}
  double S = 0;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( !IsComp[n] )
		  SummSrc[n] = 0.;
    if ( SummSrc[n] > 1e-5 )
      S += SummSrc[n];
		}
  if ( S < 0.2 )
    return false;
	Norm( IsComp, SummSrc );
  S = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
			{
      if ( SummSrc[n] < 1e-3 )
        continue;
      S += SummSrc[n];
      if ( S > 0.995 )
        break;
			if ( pGas[n]->Tb < T_min )
				T_min = pGas[n]->Tb;
			if ( pGas[n]->Tb > T_max )
				T_max = pGas[n]->Tb;
			}
		}
	if ( T_min < T_cool )
		T_min = T_cool;
	Src.T_min = T_min;
	Src.T_max = T_max;
	Dst.T_min = T_min;
	Dst.T_max = T_max;
	double dT = ( T_max - T_min ) / ( kPnt - 1 );
	double Msrc = 1.;
	CLEAR ( SummDst );
	double Mdst = 0.;
	double Vol0 = 0.;
  FINITE(SummSrc[10])
	Src.PT( P, T_min, IsComp, SummSrc );
	Eps_0 = Src.E;
  if ( Eps_0 > 0.99 )
    {
    // Газ
    for ( int n = 0; n < kPnt; n++ )
      {
      _V[n] = 100.;
      _T[n] = T_max;
      }
    return false;
    }
	if ( Eps_0 > 0. )
		{
// Отделяем жидкую часть
		Src.SetCompData ( NULL, Liq, Gas, NULL, NULL );
		CPY(SummSrc,Liq)
		}
	Norm( IsComp, SummSrc );
	double ToM3 = To_m3_liq ( T_cool, IsComp, SummSrc );
	Vol0 = Msrc * ToM3;
	for ( int n = 0; n < kPnt; n++ )
		{
		_V[n] = 100.;
		_T[n] = T_max;
		}
	for ( int n = 0; n < kPnt; n++ )
		{
		double T = T_min + n * dT;
		_T[n] = T;
		Src.PT( P, T, IsComp, SummSrc );
		double V = Src.E * Msrc;// Испаренная часть
		if ( V <= 1e-7 )
			{
			_V[n] = 0.;
			continue;
			}
		Msrc -= V;
		if ( Msrc <= 1e-3 )
			break;
		Src.SetCompData ( NULL, Liq, Gas, NULL, NULL );
		CPY( SummSrc, Liq )
		for ( int c = 0; c < LAST_GAS; c++ )
			{
			if ( IsComp[c] )
				{
				SummDst[c] = Mdst * SummDst[c] + V * Gas[c];
				}
			}
		Mdst = Norm ( IsComp, SummDst );
    FINITE(SummDst[10])
//		Dst.PT( P, T_cool, IsComp, SummDst );
		double ToM3 = To_m3_liq ( T_cool, IsComp, SummDst );
		double Vol = Mdst * ToM3;
		_V[n] = Vol / Vol0 * 100.;
		}
  return true;
	}

