#include "stdafx.h"
#include "Fakel.h"
#include "Err.h"
#include "CommProc.h"
#include "Chem.h"


void CFakel::Calc( double dt )
	{
	Fire( dt );
	}

void CFurnace::AddFlow( CFlow * pFlow )
  {
  if ( pFlow == NULL )
    return ;
  double Flow = pFlow ->Flow_mol;
  if ( Flow <= 0. )
    return ;
  FSumm += Flow;
  HSumm += Flow * pFlow->h_Flow;
  CComp * pComp = pFlow ->Get_Comp();
  if ( pComp == NULL )
    return ;
  for ( int n = 0; n < K_GAS; n++ )
    {
    if ( pComp ->IsComp[n] )
      {
      IsComp[n] = true;
      F_in[n] += Flow * pComp ->Cmol[n];
      }
    }
  }

void CFurnace::Calc( double dt )
  {
  if ( SetFire && FSumm > 0.1 )
    IsFire = true;
  if ( !IsFire || FSumm < 0.1 )
    {
    IsFire = false;
    T_fire = 0.99 * T_fire + 0.01 * T_air;
    memmove ( F_smoke, F_in, sizeof ( F_smoke ));
    return ;
    }
  //
  double O2 = NeedO2( );
  if ( O2 < 0.1 )
    {
    IsFire = false;
    T_fire = 0.99 * T_fire + 0.01 * T_air;
    memmove ( F_smoke, F_in, sizeof ( F_smoke ));
    return ;
    }
  Smoke( );
  double F_s = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    F_s += F_smoke[n];
    }
  // Расчет температуры
  double H_in = HSumm;
  //	double Cp_in = Cp_IG ( T_Air, IsComp, F_in );
  double dH_mix_in = dH_Chem( F_in, IsComp );
  double dH_mix_out = dH_Chem( F_smoke, IsComp );
  double dH_chem = dH_mix_out - dH_mix_in;
  H_in -= dH_chem;
  double dT = 1e10;
  double H_fire, Cp_fire;
  int kIter = 0;
  while ( fabs ( dT ) > 1e-3 && kIter++ < 10 )
    {
    H_fire = H_IG ( T_fire, IsComp, F_smoke ) * F_s;
    Cp_fire = Cp_IG ( T_fire, IsComp, F_smoke ) * F_s;
    double T_K = T_fire + TK;
    double T2 = T_K * T_K;
    double T3 = T2 * T_K;
    double T4 = T3 * T_K;
    double dH = H_in - H_fire - F_s * Sigma * T4 * 1e-8;
    double ddH = Cp_fire + 4. * F_s * Sigma * T3 * 1e-8;
    dT = LimdX ( dH / ddH, 10. );
    T_fire += dT;
    }
  if ( T_fire < T_air )
    T_fire = T_air;
//  double dPr = kRazr * F_summ; 
  Norm ( IsComp, F_smoke );
  }

void CFakel::Fire( double dt )
  {
  double T_Air = T_air;
  CLEAR(IsComp)
  CLEAR(F_smoke)
  double TS = 0.;
  Furnace.Init();
  Furnace.AddFlow( pGas_in );
  Furnace.AddFlow( pFuel_in );
  if ( pSteam_in )
    Furnace.AddFlow( pSteam_in );
  if ( Поджиг.On() && pFuel_in->Flow_mol > 0.1 )
    Furnace.SetFire = true;
  else
    Furnace.SetFire = false;
  Furnace.Calc( dt );
  if ( Furnace.IsFire )
    {
    Горит = 1;
    Интенсивность_пламени = 100.;
    }
  else
    {
    Горит = 0;
    Интенсивность_пламени = 0.;
    }
  TS = Furnace.T_fire;
  _T_fire = Furnace.T_fire;
  Температура_пламени = _T_fire;
  }

double CFurnace::NeedO2( )
  {
  double S_C = 0.;
  double S_H = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] && n != G_H2O && n != G_CO  && n != G_CO2 && n != G_N2 && n != G_O2 )
      {
      S_C += F_in[n] * pGas[n]->k_C;
      S_H += F_in[n] * pGas[n]->k_H;
      }
    }
  double S_O2 = S_C + 0.25 * S_H;
  return S_O2;
  }

void CFurnace::Smoke( )
  {
  memset ( F_smoke, 0, sizeof ( F_smoke ));
  double O2 = ( 1. + kkO2 ) * NeedO2( );
  double Air = O2 / 0.21;
  double N2 = Air * 0.79;
  F_in[G_N2] = N2;
  F_in[G_O2] = O2;
  F_smoke[G_O2] = O2 * kkO2;
  F_smoke[G_N2] = N2;
  F_smoke[G_H2O] = F_in[G_H2O];
  F_smoke[G_CO2] = F_in[G_CO2];
//
//
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] && n != G_H2O && n != G_CO  && n != G_CO2 && n != G_N2 && n != G_O2 )
      {
      double CO2 = F_in[n] * pGas[n]->k_C;
      double H2O = 0.5 * F_in[n] * pGas[n]->k_H;
      F_smoke[G_H2O] += H2O;
      F_smoke[G_CO2] += CO2;
      }
    }
  }
