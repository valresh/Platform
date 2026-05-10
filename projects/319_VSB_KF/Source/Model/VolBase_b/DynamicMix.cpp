#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 
const double dtf = 1e-5;

bool CVolBase::Dynamic_Func_VC( double dt, double Vcond, double & Vc, double & dVcdVc, double Gas[], double Liq[] )
  {
  double L = M_Liq + dt * ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost + Vcond );
  if ( L < 0. )
    {
    Vc = - ( M_Liq / dt + dt * ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost ));
    dVcdVc = 0.;
    return false;
    }
  double V = M_Gas + dt * ( S.dF_Gas_in - S.dF_Gas_out - Vcond );
  if ( V < 0.001 )
    {
    Vc = M_Gas / dt + dt * ( S.dF_Gas_in - S.dF_Gas_out );
    dVcdVc = 0.;
    return false;
    }
  double Alfa = k_Cond * 1000.;
  double Tabs = T + TK;
  double F = 0.;
  double dF = 0.;
  double dLdVc = dt;
  double dVdVc = -dt;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Ps = P_sat[c];
    double a00 = L + Alfa * dt * Ps;
    double a01 = -Alfa * dt * P;
    double a10 = -Alfa * dt * Ps;
    double a11 = V + Alfa * dt * P;
    double da00 = dLdVc;
    double da11 = dVdVc;
    double b0 = M_Liq * Cmol_Liq[c] + dt * S.Summ_FS_Liq[c];
    double b1 = M_Gas * Cmol_Gas[c] + dt * S.Summ_FS_Gas[c];
    double det = a00 * a11 - a10 * a01;
    det = 1. / det;
    double Xi = ( b0 * a11 - b1 * a01 ) * det;
    if ( Xi < 0. )
      Xi = 0.;
    if ( Xi > 1. )
      Xi = 1.;
    double Yi = ( b1 * a00 - b0 * a10 ) * det;
    if ( Yi < 0. )
      Yi = 0.;
    if ( Yi > 1. )
      Yi = 1.;
    Gas[c] = Yi;
    Liq[c] = Xi;
    double Vci = Alfa * ( P * Yi - Ps * Xi );
    double db0 = -da00 * Xi;
    double db1 = -da11 * Yi;
    double dXi = ( db0 * a11 - db1 * a01 ) * det;
    double dYi = ( db1 * a00 - db0 * a10 ) * det;
    F += Vci;
    double dVci = Alfa * ( P * dYi - Ps * dXi );
    dF += dVci;
    }
  Vc = F;
  dVcdVc = dF;
  return true;
  }

void CVolBase::Dynamic_Iter_VC( double dt )
  {
//  dt = dtf;
  if ( M_Gas < 0.01 )
    M_Gas = 0.01;
  const double D_max = 0.05;
  double dM_gas = dt * ( S.dF_Gas_in - S.dF_Gas_out );
  double dM_gas_max = M_Gas * D_max;
  if ( dM_gas < -dM_gas_max )
    {
    double K = -dM_gas_max / dM_gas;
    dt *= K; 
    }
  double L_new = M_Liq + dt * ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost );
  if ( L_new < 0. )
    M_Liq = 0.;
//////
  double Tabs = T + TK;
  double Ksi;
  if ( SetOmegaL )
    {
    double V_liq = V_Liq_1 + V_Liq_2;
    if ( V_liq > 0.9 * Vol )
      V_liq = 0.9 *  Vol;
    Ksi = ( Vol - V_liq ) * KSi_P * Gas_Vol / ( RP * Tabs );
    }
  else
    Ksi = Vol * KSi_P * Gas_Vol / ( RP * Tabs );
  P = M_Gas / Ksi;
//////////
  if ( M_Liq < 0.01 )
    {
    double Summ = 0.;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        double Ps = StateVol.Henry[c] * pGas[c]->Pz( T );
        if ( Tabs > pGas[c]->Tcr && Ps < 1.5 * P )
          Ps = 1.5 * P;
        P_sat[c] = Ps;
        Summ += Cmol_Gas[c] / Ps;
        }
      }
    if ( P * Summ > 1. )
      {
      // Возможна конденсация
      M_Liq = 0.02;
      }
    else
      {
// Конденсации нет
      double L = M_Liq + dt * ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost );
      if ( L < 0. )
        L = 0.;
      double V = M_Gas + dt * ( S.dF_Gas_in - S.dF_Gas_out );
      ASS( V > 0 )
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( !IsComp[c] )
          continue;
        double b0 = M_Liq * Cmol_Liq[c] + dt * S.Summ_FS_Liq[c];
        double b1 = M_Gas * Cmol_Gas[c] + dt * S.Summ_FS_Gas[c];
        double Xi = 0.;
        if ( L > 0.01 )
          Xi = b0 / L;
        double Yi = b1 / V;
        Cmol_Liq[c] = Xi;
        Cmol_Gas[c] = Yi;
        }
      M_Liq = L;
      M_Gas = V;
      ::Norm( IsComp, Cmol_Liq );
      ::Norm( IsComp, Cmol_Gas );  
      Mw_liq = ::Mw ( IsComp, Cmol_Liq );
      Mw_gas = ::Mw ( IsComp, Cmol_Gas );  
      }
    return;
    }

  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Ps;
//    if ( Is_2F )
//      Ps = StateVol.Henry[c] * Gamma_mix[c] * pGas[c]->Pz( T );
//    else
      Ps = StateVol.Henry[c] * pGas[c]->Pz( T );
    if ( Tabs > pGas[c]->Tcr && Ps < 1.5 * P )
      Ps = 1.5 * P;
    P_sat[c] = Ps;
    }
// Итерации по Vcond
// Ньютон
  bool OK = false;
  int kIter = 0;
  //double Vc0, Vc1, dVcdVc;
  //double Eps = 1e-6;
  //Dynamic_Func_VC( dt, 80, Vc0, dVcdVc );
  //Dynamic_Func_VC( dt, 80 + Eps, Vc1, dVcdVc );
  //double dTest = ( Vc1 - Vc0 ) / Eps;
  //KKK();
  double Gas[K_GAS], Liq[K_GAS];
  while ( kIter++ < 10 )
    {
    double Vc, dVcdVc;
    if ( !Dynamic_Func_VC( dt, Vcond, Vc, dVcdVc, Gas, Liq ))
      {
      Vcond = Vc;
      OK = false;
      break;
      }
    double F = Vc - Vcond;
    double dVc = F / ( 1. - dVcdVc );
    Vcond += dVc;
    if ( Vcond < -30000 || Vcond > 30000 )
      break;
    if ( fabs ( dVc ) < 1e-8 )
      {
      OK = true;
      break;
      }
    }
  if ( !OK )
    {
    double Vb =  -( M_Liq / dt + ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost ));
    if ( Vb < -30000. )
      Vb = -30000.;
    double Ve = M_Gas / dt + ( S.dF_Gas_in - S.dF_Gas_out );
    if ( Ve > 30000. )
      Ve = 30000.;
    int kIter = 0;
    double Vc, dVcdVc;
    while ( Ve - Vb > 1e-6 )
      {
      kIter++;
      Vcond = 0.5 * ( Vb + Ve );
      if( !Dynamic_Func_VC( dt, Vcond, Vc, dVcdVc, Gas, Liq ))
        {
        Vcond = Vc;
        break;
        }
      if ( Vc > Vcond )
        Vb = Vcond;
      else
        Ve = Vcond;
      }
    if ( fabs ( Vcond - Vc ) > 1. )
      KKK();
    KKK();
    }
//
  double Alfa = k_Cond * 1000.;
  double Vsumm = 0.;
  double Sx = 0.;
  double Sy = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    Cmol_Gas[c] = Gas[c];
    Cmol_Liq[c] = Liq[c];
    Sx += Liq[c];
    Sy += Gas[c];
    V_Cond[c] = Alfa * ( P * Cmol_Gas[c] - P_sat[c] * Cmol_Liq[c] );
    Vsumm += V_Cond[c];
    S.Q += V_Cond[c] * pGas[c]->H_vap( T );
    }
  Norm ( IsComp, Cmol_Liq );
  Norm ( IsComp, Cmol_Gas );
  double dM_Liq = dt * ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost + Vcond );
  if ( M_Liq > 1. )
    {
    if ( dM_Liq < -0.1 * M_Liq )
      dM_Liq = -0.1 * M_Liq;
    if ( dM_Liq > 0.1 * M_Liq )
      dM_Liq = 0.1 * M_Liq;
    M_Liq += dM_Liq;
    }
  else
    {
    if ( dM_Liq < -0.1 )
      dM_Liq = -0.1;
    if ( dM_Liq > 0.1 )
      dM_Liq = 0.1;
    M_Liq += dM_Liq;
    if ( M_Liq < 0. )
      M_Liq = 0.;
    }
  double dM_Gas = dt * ( S.dF_Gas_in - S.dF_Gas_out - Vcond );
  if ( dM_Gas < -0.1 * M_Gas )
    dM_Gas = -0.1 * M_Gas;
  if ( dM_Gas > 0.1 * M_Gas )
    dM_Gas = 0.1 * M_Gas;
  M_Gas += dM_Gas;
  if ( M_Gas < 0.01 )
    M_Gas = 0.01;
  Mw_liq = ::Mw ( IsComp, Cmol_Liq );
  Mw_gas = ::Mw ( IsComp, Cmol_Gas );  
  FINITE(M_Gas)
  KKK();
  }



////////////////////////////////////////////////////////////////


// Динамический режим - газовая фаза 
void CVolBase::DynamicMix_Gas( double dt )
  {
  const double D_max = 0.05;
  double dM_gas = dt * ( S.dF_Gas_in - S.dF_Gas_out );
  double dM_gas_max = M_Gas * D_max;
  if ( dM_gas < -dM_gas_max && M_Gas > 0.1 )
    {
    double K = -dM_gas_max / dM_gas;
    if ( K < 0.1 )
      K = 0.1;
    dt *= K; 
    dM_gas *= K;
    }
  // Новые составы
  double M_Gas_new = M_Gas + dt * S.dF_Gas_in;
  //
  double N_Gas = 1. / M_Gas_new;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double MC = M_Gas * Cmol_Gas[c];
      double MC_new = MC + dt * S.Summ_FS_Gas[c];
      if ( MC_new < 0. )
        MC_new = 0.;
      Cmol_Gas[c] = MC_new * N_Gas;
      }
    }
  M_Gas += dM_gas;
  if ( M_Gas < 0.0001 )
    M_Gas = 0.0001;
  }

// Динамический режим - жидкая фаза 
void CVolBase::DynamicMix_Liq( double dt )
  {
  const double D_max = 0.05;
  /////////////////////////////////////////////
  double dM_liq = dt * ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 - F_lost );
  double dM_liq_max = M_Liq * D_max;
  if ( dM_liq > dM_liq_max && M_Liq > 1. )
    {
    dt *= dM_liq_max / dM_liq; 
    dM_liq = dM_liq_max;
    }
  if ( dM_liq < -dM_liq_max && M_Liq > 1. )
    {
    dt *= -dM_liq_max / dM_liq; 
    dM_liq = -dM_liq_max;
    }
  ///////////////////////////////////////////////////////
  //.................................
  if ( Is_2F )
    {
    double M_Liq_new = M_Liq + dt * ( S.dF_Liq_in - S.dF_Liq_out_1 - S.dF_Liq_out_2 );
    if ( M_Liq_new > 0. )
      {
      double N_Liq = 1. / M_Liq_new;
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( IsComp[c] )
          {
          double MC = M_Liq * Cmol_Liq[c];
          double MC_new = MC + dt * ( S.Summ_FS_Liq[c] );
          Cmol_Liq[c] = MC_new * N_Liq;
          }
        }
      }
    }
  else
    {
    double M_Liq_new = M_Liq + dt * ( S.dF_Liq_in );
    if ( M_Liq_new > 0. )
      {
      double N_Liq = 1. / M_Liq_new;
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( IsComp[c] )
          {
          double MC = M_Liq * Cmol_Liq[c];
          double MC_new = MC + dt * ( S.Summ_FS_Liq[c] );
          Cmol_Liq[c] = MC_new * N_Liq;
          }
        }
      }
    }
  M_Liq += dM_liq;
  if ( M_Liq < 0. )
    M_Liq = 0.;
  double F1 = dM_liq / dt;
  FINITE(M_Gas)
  FINITE(M_Liq)
  M = M_Gas + M_Liq;
  double D = Norm ( IsComp, Cmol_Liq );
  KKK();
  }

// Динамический режим - конденсация   

void CVolBase::DynamicCond( double dt )
  {
  double R = 0.;
// Точка росы
  double Tabs = T + TK;
  if ( M_Liq < 0.01 )
    {
    double Summ = 0.;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        double Ps = StateVol.Henry[c] * pGas[c]->Pz( T );
        if ( Tabs > pGas[c]->Tcr && Ps < 1.5 * P )
          Ps = 1.5 * P;
        P_sat[c] = Ps;
        Summ += Cmol_Gas[c] / Ps;
        }
      }
    if ( P * Summ > 1. )
      {
// Возможна конденсация
      M_Liq = 0.02;
      }
    }
  if ( M_Liq > 0.01 )
    {
    double Ksi;
    if ( SetOmegaL )
      {
      double V_liq = V_Liq_1 + V_Liq_2;
      if ( V_liq > 0.99 * Vol )
        V_liq = 0.99 *  Vol;
      Ksi = ( Vol - V_liq ) * KSi_P * Gas_Vol / ( RP * Tabs );
      }
    else
      Ksi = Vol * KSi_P * Gas_Vol / ( RP * Tabs );
    P = M_Gas / Ksi;
    FINITE(M_Gas)
    double Pz = 0.;
    double Alfa = k_Cond * 100.;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        double Ps;
        if ( Is_2F )
          Ps = StateVol.Henry[c] * Gamma_mix[c] * pGas[c]->Pz( T );
        else
          Ps = StateVol.Henry[c] * pGas[c]->Pz( T );
        if ( Tabs > pGas[c]->Tcr && Ps < 1.5 * P )
          Ps = 1.5 * P;
        P_sat[c] = Ps;
        Pz += Ps * Cmol_Liq[c];
        FINITE(Ps)
          //
//        double dM = Alfa * M_Gas * M_Liq * ( P * Cmol_Gas[c] - Ps * Cmol_Liq[c] ) /
//          ( M_Gas * M_Liq + Alfa * ( M_Liq * P + M_Gas * Ps ));
//        double Vc = dM / dt;
        double Vc = Alfa * ( P * Cmol_Gas[c] - Ps * Cmol_Liq[c] );
        V_Cond[c] += 0.1 * ( Vc - V_Cond[c] );
        S.Summ_FS_Gas[c] -= V_Cond[c];
        S.Summ_FS_Liq[c] += V_Cond[c];
        R += V_Cond[c] * pGas[c]->H_vap( T );
/***
        if ( !Static_Fix_P )
          {
          Cmol_Gas[c] = M_Gas * Cmol_Gas[c] - dM;
          if ( Cmol_Gas[c] < 0. )
            Cmol_Gas[c] = 0.;
          }
        Cmol_Liq[c] = M_Liq * Cmol_Liq[c] + dM;
        if ( Cmol_Liq[c] < 0. )
          Cmol_Liq[c] = 0.;
        *****/
        }
      }
    if ( !Static_Fix_P )
      M_Gas = Norm ( IsComp, Cmol_Gas );
    double M0 = M_Liq;
    M_Liq = Norm ( IsComp, Cmol_Liq );
    if ( Pz > 0. )
      P_Pz = P / Pz;    
    double F2 = ( M_Liq - M0 ) / dt;
    //    P = Pz;
    double Tst = P * Ksi / M_Gas;
    //    M_Gas = P * Ksi;
    Mw_liq = ::Mw ( IsComp, Cmol_Liq );
    Mw_gas = ::Mw ( IsComp, Cmol_Gas );  
    if ( !Fast_T )
      {
      S.Q += R;    //????
      //double dT = R * dt / ( Cp_liq * M_Liq + Cp_corp );
      //T += dT;
      //if ( T < T_min )
      //  T = T_min;
      //FINITE(T)
      }
    }
  else
    {
    double Ksi = Vol * KSi_P * Gas_Vol / ( RP * ( T + TK ));
    P = M_Gas / Ksi;
    Mw_liq = ::Mw ( IsComp, Cmol_Liq );
    Mw_gas = ::Mw ( IsComp, Cmol_Gas );  
    FINITE(M_Gas)
    }
  }
