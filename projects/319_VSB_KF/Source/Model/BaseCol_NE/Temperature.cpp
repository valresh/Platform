#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"


void CStage_NE::CalcHeat( double dt )
	{
  if ( StopHeat )
    {
    BREAK
    if ( StopHeat > 0 )
      StopHeat--;
    }
//         
///////////////////////////////////////////
  if ( pStageHeat )
    {
    pStageHeat->Lev = Level;
    pStageHeat->T_ext = T_liq;            
    pStageHeat->Calc( dt );               
    dH_liq += pStageHeat->Q_Heat;
    }
  if ( pCol->Static_Calc_Mix && pCol->ModelCond == 1 )
    {
    CalcH( );
    return;
    }
  double dT_liq;
  double dT_gas;
  double dH_liq = ExtHeat + dH_cond;
  double dH_gas = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double hl = h_liq[c];
      double hg = h_gas[c];
      dH_liq += H_Summ_Liq_In_A[c] * hl + H_Summ_Liq_In_B[c];
      dH_gas += H_Summ_Gas_In_A[c] * hg + H_Summ_Gas_In_B[c];
      }
    }
  if ( pCol->Static_Calc_Mix && pCol->Fast_T )
    {
    double Tm = 0.5 * ( T_gas + T_liq );
    Calc_T( Tm );
    dT_liq = pCol->Slow_Fast_T * ( Tm - T_liq );
    dT_gas = pCol->Slow_Fast_T * ( Tm - T_gas );
    T_gas += dT_gas;
    T_liq += dT_liq;
    double adT_liq = fabs ( dT_liq );
    double adT_gas = fabs ( dT_gas );
    if ( adT_liq > adT_gas )
      {
      pCol->dT_abs_med += adT_liq;
      if ( pCol->dT_abs_max < adT_liq )
        pCol->dT_abs_max = adT_liq;
      }
    else
      {
      pCol->dT_abs_med += adT_gas;
      if ( pCol->dT_abs_max < adT_gas )
        pCol->dT_abs_max = adT_gas;
      }
    }
  else
    {
    double dtK = dt * k_GL * 1e6;
    double Cp_korp_gas = Cp_korp * 0.1 + Cp_gas * M_gas;
    double Cp_korp_liq = Cp_korp * 0.9 + Cp_liq * M_liq;
    double a00 = Cp_korp_gas + dtK;
    double a01 = -dtK;
    double a10 = -dtK;
    double a11 = Cp_korp_liq + dt * ( k_GL * 1e6 + k_Cool );
    double b0 = Cp_korp_gas * T_gas + dt * dH_gas;
    double b1 = Cp_korp_liq * T_liq + dt * ( dH_liq + k_Cool * T_air );
    double det = 1. / ( a00 * a11 - a10 * a01 );
    double T_gas_new = ( b0 * a11 - b1 * a01 ) * det;
    double T_liq_new = ( b1 * a00 - b0 * a10 ) * det;
    FINITE(T_gas_new)
    FINITE(T_liq_new)
    dT_liq = T_liq_new - T_liq;
    dT_gas = T_gas_new - T_gas;
    //T_liq = T_liq_new;
    //T_gas = T_gas_new;
    T_liq += dT_liq;
    T_gas += dT_gas;
    double adT_liq = fabs ( dT_liq );
    double adT_gas = fabs ( dT_gas );
    if ( adT_liq > adT_gas )
      {
      pCol->dT_abs_med += adT_liq;
      if ( pCol->dT_abs_max < adT_liq )
        pCol->dT_abs_max = adT_liq;
      }
    else
      {
      pCol->dT_abs_med += adT_gas;
      if ( pCol->dT_abs_max < adT_gas )
        pCol->dT_abs_max = adT_gas;
      }
    }
////////////////////////////////////////////////////
//
  double _h_gas = h_gas_mix;
  double _h_liq = h_liq_mix;

  //double T_min;
  //double T_max;
  //if ( pCol )
  //  {
  //  T_min = pCol->T_min;
  //  T_max = pCol->T_max;
  //  }
  //else
  //  {
  //  T_min = 0.1;
  //  T_max = 500.;
  //  }
//
  if ( T_gas < T_min - 1. )
    T_gas = T_min;
  if ( T_gas > T_max )
    T_gas = T_max;
  if ( T_liq < T_min - 1. )
	  T_liq = T_min;
  if ( T_liq > T_max )
	  T_liq = T_max;
  CalcH( );
/********/
  double h_gas = h_gas_mix;
  double h_liq = h_liq_mix;
  double Q = k_GL * 1e6 * ( T_gas - T_liq );
  double dHg = dH_gas - Q;
  double dHl = dH_liq + Q;
  double dH = dHg - dHl;
  if ( fabs ( dT_gas ) > 0.01 )
    {
    double Cp = ( h_gas - _h_gas ) / dT_gas;
    Cp_gas += pCol->k_Cp * ( Cp - Cp_gas ) * 0.1;
    }
  if ( Cp_gas > 500. )
    Cp_gas = 500.;
  if ( Cp_gas < 30. )
    Cp_gas = 30.;
  if ( fabs ( dT_liq ) > 0.01 )
    {
    double Cp = ( h_liq - _h_liq ) / dT_liq;
    Cp_liq += pCol->k_Cp * ( Cp - Cp_liq ) * 0.1;
    }
  if ( Cp_liq > 500. )
    Cp_liq = 500.;
  if ( Cp_liq < 30. )
    Cp_liq = 30.;
  FINITE(T_liq)
  FINITE(T_gas)
  if ( T_liq_Z > 0. )
  if ( pSys -> bWorkReg )
	  {
		T_gas = T_liq = T_liq_Z;
		pCol->Проблемы |= PROB_FIX_T;
    }
/*******/
//
	if ( T_liq < T_min )
		T_liq = T_min;
	if ( T_liq > T_max )
		T_liq = T_max;
	if ( T_gas < T_min )
		T_gas = T_min;
	if ( T_gas > T_max )
		T_gas = T_max;
//
  }

double CStage_NE::Calc_F( double Tm, double dT )
  {
  double F = F_T_gas( Tm, dT ) + F_T_liq( Tm, dT );
  FINITE(F)
  return -F;
  }

const double Delta = 1.;
void CStage_NE::Calc_T( double & Tres )
  {
  double Tm = 0.5 * ( T_gas + T_liq );
  double dT = 0.;
  double Tb = Tm;
  double Te = Tm;
  double F = Calc_F( Tm, dT );
  double Fb = F;
  double Fe = F;
  if ( F < 0. )
    {
    while ( F < 0. )
      {
      Tb = Te;
      Fb = Fe;
      Te += Delta;
      if ( Te > T_max )
        {
        Tres = Tb;
        return;
        }
      F = Calc_F( Te, dT );
      Fe = F;
      }
    }
  else
    {
    while ( F > 0. )
      {
      Te = Tb;
      Fe = Fb;
      Tb -= Delta;
      if ( Tb < T_min )
        {
        Tres = Te;
        return;
        }
      F = Calc_F( Tb, dT );
      Fb = F;
      }
    }
  if ( Fe <= Fb )
    {
    double T = 0.5 * ( Tb + Te );
    Tres = T;
    return;
    }
	int kIter = 0;
	double A, A1;
  double deltaT = Te - Tb;
  double T = 0.5 * ( Tb + Te );
  FINITE(T)
	while ( fabs ( deltaT ) > 1e-4 && Fe - Fb > 1e-3)
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
      Tres = T;
      return;
      }
		deltaT = T_new - T;
		T = T_new;
    F = Calc_F( T, dT );
		if ( F > 0. )
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
  FINITE(T)
  Tres = T;
  }

double CStage_NE::F_T_liq( double Tm, double dT, bool AddR )
  {
  double F = 0.;
  double Tl = Tm - 0.5 * dT;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double h = pGas[c]->H_l( Tl );
      F += H_Summ_Liq_In_A[c] * h + H_Summ_Liq_In_B[c];
//      F += ( Vc_Comp_C[c] - Vc_Comp_Pz[c] * pGas[c]->Pz( Tl )) * pGas[c]->H_vap( Tl );
//      h_liq[c] = h;
      }
    }
  //if ( AddR )
  //  {
  //  for ( int c = 0; c < LAST_GAS; c++ )
  //    {
  //    if ( IsComp[c] )
  //      {
  //      F += ( Vc_Comp_C[c] - Vc_Comp_Pz[c] * pGas[c]->Pz( Tl )) * pGas[c]->H_vap( Tl );
  //      }
  //    }
  //  }
  return F + k_GL * 1e6 * dT + dH_cond;
  }

double CStage_NE::F_T_gas( double Tm, double dT )
  {
  double F = 0.;
  double Tg = Tm + 0.5 * dT;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double h = pGas[c]->H_IG( Tg );
      F += H_Summ_Gas_In_A[c] * h + H_Summ_Gas_In_B[c];
//      h_gas[c] = h;
      }
    }
  return F - k_GL * 1e6 * dT;
  }

void CStage_NE::CalcH( )
	{
  double hls = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
	  if ( !IsComp[n] )
		  continue;
	  double hl;
	  hl = pGas[n]-> H_l ( T_liq );
	  h_liq[n] = hl;
	  hls += X[n] * hl;
	  }
  h_liq_mix = hls;
////////////////////////////////
  double hgs = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    double hg;
    hg = pGas[n]-> H_IG ( T_gas );
    h_gas[n] = hg;
    hgs += Y[n] * hg;
    FINITE(hg)
    }
  h_gas_mix = hgs;
	}
