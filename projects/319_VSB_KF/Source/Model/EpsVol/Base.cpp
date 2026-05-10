#include "stdafx.h"
#define DLL_EpsVol
#include "EpsVol.h"
#include "math.h"
#include "Err.h"
#include "Data.h"
#include "SysDataTypes.h"
#include "Connect.h"
#include "Water.h"

int CSBase::ID_NH3 = -1;


CSBase::CSBase()
	{
	memset ( this, 0, sizeof ( CSBase ));
	for ( int n = 0; n < K_GAS; n++ )
		{
		Gamma[n] = 1.;
		Henry[n] = 1.;
    NoInLiq[n] = false;
    WorkComp[n] = false;
		}
	Henry[G_N2] = 10000.0;
//	Alfa_Gas_liq = 0.;
	T_old = -100.;
	Step_T = 1.;
	k_Gamma = 0.5;
	GAS = false;
	LIQ = false;
	pExtGamma = NULL;
  OW = 0;
  OW_Z = 0;
  HenryPrim = 100.;
  H2S_MEA = true;
  MixH2ONH3 = true;
  UseComp = 0xFFFFFFFF;
  Use_H_liq = false;
	}

void CSBase::SetComp ( bool _IsComp[], double _Cmol[] )
	{
	pComp = NULL;
  const double EpsCmol = 1e-9;
	TypeMix = 0;
	memset ( static_cast<CSBase_Data*>(this), 0, sizeof ( CSBase_Data ));
	double S = 0.;
	if ( !_finite ( dH_liq ))
		dH_liq = 0.;
  SummUse = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( !_finite ( Gamma[n] ))
			Gamma[n] = 1.;
		if ( !_IsComp[n] || _Cmol[n] <= EpsCmol )
			continue;
    IsComp[n] = true;
		Z_W[n] = _Cmol[n];
    S += Z_W[n];
    WorkComp[n] = true;
    FINITE(Z_W[n])
		}
  OW = OW_Z;
	if ( S < 1e-4 )
    {
//  	memset ( static_cast<CSBase_Data*>(this), 0, sizeof ( CSBase_Data ));
//    memcpy ( WorkComp, pN2->IsComp, ISCOMP_GAS );
//    memcpy ( Z_W, pN2->Cmol, CMOL_GAS );
    Z_W[G_N2] = 1.;
    IsComp[G_N2] = true;
    WorkComp[G_N2] = true;
    }
  else
    {
    if ( fabs ( S - 1. ) > 1e-6 )
      {
	    double N = 1. / S;
	    for ( int n = 0; n < LAST_GAS; n++ )
		    {
		    if ( WorkComp[n] )
			    Z_W[n] *= N;
		    }
      }
    }
  SummUse = 1.;
	}

void CSBase::SetComp ( CComp * _pComp )
	{
	pComp = _pComp;
	bool * _IsComp = pComp ->IsComp;
	double * _Cmol = pComp ->Cmol;
  const double EpsCmol = 1e-9;
	TypeMix = 0;
	memset ( static_cast<CSBase_Data*>(this), 0, sizeof ( CSBase_Data ));
	double S = 0.;
	if ( !_finite ( dH_liq ))
		dH_liq = 0.;
  SummUse = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( !_finite ( Gamma[n] ))
			Gamma[n] = 1.;
    if ( !_finite(_Cmol[n]))
      _Cmol[n] = 0.;
		if ( !_IsComp[n] || _Cmol[n] <= EpsCmol )
      {
      WorkComp[n] = false;
			continue;
      }
    IsComp[n] = true;
		Z_W[n] = _Cmol[n];
    S += Z_W[n];
    WorkComp[n] = true;
    FINITE(Z_W[n])
		}
  OW = OW_Z;
	if ( S < 1e-4 )
    {
    Z_W[G_N2] = 1.;
    IsComp[G_N2] = true;
    WorkComp[G_N2] = true;
    }
  else
    {
    if ( fabs ( S - 1. ) > 1e-6 )
      {
	    double N = 1. / S;
	    for ( int n = 0; n < LAST_GAS; n++ )
		    {
		    if ( WorkComp[n] )
			    Z_W[n] *= N;
		    }
      }
    }
  SummUse = 1.;
	}

const double log_10 = log ( 10. );

void CSBase::Pz( double T )
	{
  if ( T > 1000. )
    T = 1000.;
  FINITE(T)
  double Tabs = T + TK;
  for ( int n = 0; n < LAST_GAS; n++ )
	  {
	  if ( WorkComp[n] )
      {
      if ( NoInLiq[n] )
        P_sat[n] = 200.;
      else
        {
	      P_sat[n] = Henry[n] * pGas[n]->Pz ( T ) * Gamma[n];
        if ( pExtGamma )
          P_sat[n] *= pExtGamma[n];
        if ( T > pGas[n]-> Tcr && P_sat[n] < 1.5 * P )
          P_sat[n] = 1.5 * P;
        }
      }
	  }
	}

void CSBase::H_liq ( double Ts )
  {
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
//      if ( n == G_H2O && pSys->WaterIAPWS )
//        h_Liq[n] = Water.Hl( Ts, P );
//      else
        h_Liq[n] = pGas[n]-> H_l ( Ts );
      }
    }
  }

void CSBase::H_liq( )
	{
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
      {
//      if ( n == G_H2O && pSys->WaterIAPWS )
//        h_Liq[n] = Water.Hl( T, P );
//      else
			  h_Liq[n] = pGas[n]-> H_l ( T );
      }
		}
	}

void CSBase::H_gas( )
	{
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
      {
//      if ( n == G_H2O && pSys->WaterIAPWS )
//        h_Gas[n] = Water.Hv( T, P );
//      else
	    h_Gas[n] = pGas[n]-> H_IG ( T );
      }
		}
	}


void CSBase::SetGamma()
	{
	double k_Gamma1 = 1. - k_Gamma;
	if ( T_old == -100. )
		{
		Step_T = 1.;
		T_old = T;
		}
	else
		{
		Step_T = fabs ( T_old - T );
		if ( Step_T < 0.01 )
			Step_T = 0.01;
		if ( Step_T > 5. )
			Step_T = 5.;
		T_old = T;
		}
	/**/
//=========================================================
//  if ( MixH2ONH3 )
//    pChemAdditions->Gamma( NULL, P, T, WorkComp, X_W, P_sat, Gamma, dH_liq );
  //for ( int n = 0; n < MAX_GAMMA; n++ )
  //  {
  //  if ( pGamma[n] )
  if ( pGamma )
    {
    IGammaParam GP;
    GP.Param = (DWORD*)&BP;
    GP.P = &P;
    GP.T = &T;
    GP.IsComp = WorkComp;
    GP.X = X_W;
    GP.P_sat = P_sat;
    GP.Gamma = Gamma;
    double h;
    GP.dH_liq = &h;
    pGamma -> Gamma( &GP );
    if ( Use_H_liq )
      dH_liq = h;
    else
      dH_liq = 0.;
    }

#if 0
  if ( G_H2O >= 0 && G_NH3 >= 0 )
    {
    if ( WorkComp[G_H2O] && WorkComp[G_NH3] )
      {
      double x_H2O = X_W[G_H2O];
      double x_NH3 = X_W[G_NH3];
      double Sx = x_H2O + x_NH3;
      if ( x_H2O < 1e-4 || x_NH3 < 1e-4 )
        {
        Gamma[G_H2O] = k_Gamma * Gamma[G_H2O] + k_Gamma1;
        Gamma[G_NH3] = k_Gamma * Gamma[G_NH3] + k_Gamma1;
        dH_liq = k_Gamma * dH_liq;
        }
      else
        {
        double N = 1. / Sx;
        x_H2O *= N;
        x_NH3 *= N;
        double Psat = CNH3_H2O_Appr::P_sat( x_NH3, T );
        double y_HN3 = CNH3_H2O_Appr::Y_sat( x_NH3, T );
        double P_NH3 = Psat * y_HN3;
        double P_H2O = Psat - P_NH3;
        double Pz_NH3;
        if ( x_NH3 > 1e-6 )
          Pz_NH3 = P_NH3 / x_NH3;
        else
          Pz_NH3 = 0.;
        double Pz_H2O;
        double x1 = 1. - x_NH3;
        if ( x1 > 1e-6 )
          Pz_H2O = P_H2O / x1;
        else
          Pz_H2O = 0.;
        double k_NH3 = Pz_NH3 / P_sat[G_NH3];
        double k_H2O = Pz_H2O / P_sat[G_H2O];
        if ( Gamma[G_NH3] < 1e-5 )
          Gamma[G_NH3] = 1.;
        if ( Gamma[G_H2O] < 1e-5 )
          Gamma[G_H2O] = 1.;
        Gamma[G_NH3] = ( k_Gamma + k_Gamma1 * k_NH3 ) * Gamma[G_NH3];
        Gamma[G_H2O] = ( k_Gamma + k_Gamma1 * k_H2O ) * Gamma[G_H2O];
        double Hl = CNH3_H2O_Appr::H_l_sat( x_NH3, P * Sx, T );
        double Hl_id = 
          x_H2O * CNH3_H2O_Appr::H_l_sat( 0., P, T ) + x_NH3 * CNH3_H2O_Appr::H_l_sat( 1., P, T );
        dH_liq = k_Gamma * dH_liq + k_Gamma1 * Sx * ( Hl - Hl_id );
        if ( !MixH2ONH3 )
          dH_liq = 0.;
        }
      return;
      }
    else
      {
      Gamma[G_H2O] = k_Gamma * Gamma[G_H2O] + k_Gamma1;
      Gamma[G_NH3] = k_Gamma * Gamma[G_NH3] + k_Gamma1;
      dH_liq = k_Gamma * dH_liq;
      }
    }
  else
    {
    if ( G_H2O >= 0 )
      Gamma[G_H2O] = k_Gamma * Gamma[G_H2O] + k_Gamma1;
    if ( G_NH3 >= 0 )
      Gamma[G_NH3] = k_Gamma * Gamma[G_NH3] + k_Gamma1;
    dH_liq = k_Gamma * dH_liq;
    }
/*
  if ( WorkComp[G_CO2] && WorkComp[G_MDEA] && CO2_MEA )
    {
    Gamma[G_MDEA] = 1.;
    Gamma[G_H2O] = 1.;
    if ( Gamma[G_CO2] < 1e-6 )
      Gamma[G_CO2] = 1.;
    double x_MEA  = X_W[G_MDEA];
    double x_CO2  = X_W[G_CO2];
    if ( x_CO2 < 1e-6 )
      x_CO2 = 1e-6;
    //
    double k_CO2 = CGas_MDEA::Gamma_CO2 ( T, x_CO2, x_MEA );
    Gamma[G_CO2] = k_Gamma * Gamma[G_CO2] + k_Gamma1 * k_CO2;
    dH_liq = 0.;
    return;
    }
  else
    Gamma[G_CO2] = k_Gamma * Gamma[G_CO2] + k_Gamma1;
  dH_liq = 0.;
*/
#endif
	}


void CSBase::SetCompData ( bool * _IsComp, double * Cmol_X,  double * Cmol_Y,
									         double * _h_Liq, double * _h_Gas )
	{
	if ( _IsComp )
		memcpy ( _IsComp, IsComp, K_GAS );
	if ( Cmol_X )
    {
	  for ( int n = 0; n < LAST_GAS; n++ )
		  {
		  if ( IsComp[n] )
        {
        if ( WorkComp[n] ) 
          Cmol_X[n] = X_W[n] * SummUse;
        else
          Cmol_X[n] = Z_W[n];
        }
      else
        Cmol_X[n] = 0.;
      }
//		memcpy ( Cmol_X, X_W, CMOL_GAS );
    }
	if ( Cmol_Y )
		memcpy ( Cmol_Y, Y_W, CMOL_GAS );
	if ( _h_Liq )
		memcpy ( _h_Liq, h_Liq, CMOL_GAS );
	if ( _h_Gas )
		memcpy ( _h_Gas, h_Gas, CMOL_GAS );
	}

void CSBase::Calc_Mw()
	{
	Mw_liq = 0.;
	Mw_gas = 0.;
  Mw_sol = 0.;
  Mw_summ = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
      double Mw = pGas[n]->Mw;
			Mw_gas += Y_W[n] * Mw;
			Mw_liq += X_W[n] * Mw;
      Mw_summ += Z_W[n] * Mw;
			}
    else
      {
      if ( IsComp[n] )
        Mw_sol += Z_W[n] * pGas[n]->Mw;
      }
		}
	}

  CWater __declspec(dllimport) Water;


double CSBase::S_liq ( int N, double P, double T )
	{
	if ( N == G_H2O )
		{
		if ( T >= 372. )
			return Water.Sv ( T, P );
		double Sl = Water.Sl ( T, P );
		return Sl;
		}
	return S_gas ( N, P, T );
	}

double CSBase::S_gas ( int N, double _P, double _T )
	{
	if ( N == G_H2O )
		{
		double Sv = Water.Sv ( _T, _P );
		return Sv;
		}
	if ( N == G_OIL )
		{
		return 0.;
		}
	return pGas[N] ->S_IG ( _P, _T );
	}
