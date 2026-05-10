#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

double CVolBase::Calc_dH( double T,  double H_summ_in_gas_A[], double H_summ_in_liq_A[], double H_summ_in_gas_B[], double H_summ_in_liq_B[], double Q  )
  {
  double dH_in = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      h_Gas[c] = pGas[c]->H_IG( T );
      h_Liq[c] = pGas[c]->H_l( T );
      double hg = S.H_summ_in_gas_A[c] * h_Gas[c] +  S.H_summ_in_gas_B[c];
      double hl = S.H_summ_in_liq_A[c] * h_Liq[c] +  S.H_summ_in_liq_B[c];
      dH_in += hg + hl;
      }
    }
  double Pz = 0.;
  double R = 0.;
  double Alfa = k_Cond * 1e3;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double Ps;
      if ( Is_2F )
        Ps = StateVol.Henry[c] * Gamma_mix[c] * pGas[c]->Pz( T );
      else
        Ps = StateVol.Henry[c] * pGas[c]->Pz( T );
      if ( T + TK > pGas[c]->Tcr && Ps < 1.5 * P )
        Ps = 1.5 * P;
      P_sat[c] = Ps;
      Pz += Ps * Cmol_Liq[c];
      FINITE(Ps)
        //
        double Vc = Alfa * ( P * Cmol_Gas[c] - Ps * Cmol_Liq[c] );
      R += Vc * pGas[c]->H_vap( T );
      }
    }
  return dH_in + R + Q;
  }

const double Delta = 1.;

void CVolBase::Fast_Calc_T( double & Tio, double R )
  {
  double dT = 0.;
  double Tb = Tio;
  double Te = Tio;
  double F = Calc_dH( Tb,  S.H_summ_in_gas_A, S.H_summ_in_liq_A, S.H_summ_in_gas_B, S.H_summ_in_liq_B, R  );
  double Fb = F;
  double Fe = F;
  if ( F > 0. )
    {
    while ( F > 0. )
      {
      Tb = Te;
      Fb = Fe;
      Te += Delta;
      if ( Te > T_max )
        {
        T = Tb;
        return;
        }
      F = Calc_dH( Te,  S.H_summ_in_gas_A, S.H_summ_in_liq_A, S.H_summ_in_gas_B, S.H_summ_in_liq_B, R  );
      Fe = F;
      }
    }
  else
    {
    while ( F < 0. )
      {
      Te = Tb;
      Fe = Fb;
      Tb -= Delta;
      if ( Tb < T_min )
        {
        Tio = Te;
        return;
        }
      F = Calc_dH( Tb,  S.H_summ_in_gas_A, S.H_summ_in_liq_A, S.H_summ_in_gas_B, S.H_summ_in_liq_B, R  );
      Fb = F;
      }
    }
  int kIter = 0;
  double A, A1;
  double deltaT = Te - Tb;
  double T = 0.5 * ( Tb + Te );
  while ( fabs ( deltaT ) > 1e-4 )
    {
    kIter++;
    if ( kIter > 5 )
      KKK();
    double idH = 1. / ( Fe - Fb );
    A = -Fb * idH;
    A1 = 1. - A;
    double T_new = A * Te + A1 * Tb;
    if ( T_new < T_min || T_new > T_max )
      {
      Tio = T;
      return;
      }
    deltaT = T_new - T;
    T = T_new;
    F = Calc_dH( T,  S.H_summ_in_gas_A, S.H_summ_in_liq_A, S.H_summ_in_gas_B, S.H_summ_in_liq_B, R  );
    if ( F < 0. )
      {
      Te = T;
      Fe = F;
      }
    else
      {
      Tb = T;
      Fb = F;
      }
    }
  Tio = T;
  }
