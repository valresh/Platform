#include "stdafx.h"
#include "KO.h"
#include "Err.h"
#include "CommProc.h"



/*
void CKO::Calc( double dt )
  {
  SET_BP BreakPoint;
  if ( pFlow_In == NULL )
    return;
  CComp * pComp = pFlow_In->Get_Comp();
  if ( pComp == NULL )
    return;
  double F_mol = pFlow_In->Flow_mol;
  if ( F_mol >= 0. )
    {
    In.PH( pFlow_In );
    _Eps = In.E;
    double C_H2O = 0.;
    if ( pComp->IsComp[G_H2O] )
      C_H2O = pComp->Cmol[G_H2O];
    kFlow = 0.9 * kFlow + 0.1 * C_H2O;
    double K = dt * 60. / Ksi;
    double Old = 1. / ( 1. + K );
    double New = K * Old;
    T = Old * T + New * In.T;
    P = pGas[G_H2O]->Pz( T + 0.5 );
//////////////////////////////////////////
    double dP = LimdX ( P_in - P, 0.1 );
    Omega *= 1. + dP; 
    if ( Omega > Omega_In )
      Omega = Omega_In; 
    if ( Omega < Omega_Out )
      Omega = Omega_Out; 
//////////////////////////////////////////
    _P = P;
    _T = T;
    _Flow_kg = F_mol * Mw_H2O;
    //    if ( P < P_Out + 1. )
    //      P = P_Out + 1.;
   M_H2O += dt * F_mol * C_H2O;
   M_H2O -= dt * pFlow_Out->Flow_mol;
    if (M_H2O < 0. )
     M_H2O = 0.;
    double L =M_H2O * 18. / Vol_Liq;
    _Level = L * 100.;
    if ( _Level > 100. )
      _Level = 100.;
    if ( L < 0.1 )
      P_H = L * 10. * dP_Hydro;
    else
      P_H = dP_Hydro + 2. * ( L - 0.1 );
    *pFlow_Out = *pWaterFlow;
    pFlow_Out->T = T;
    pFlow_Out->h_Flow = pGas[G_H2O]->H_l( T );
    }
  else
    {
    *pFlow_In = *pWaterFlow;
    *pFlow_Out = *pWaterFlow;
    kFlow = 1.;
    Omega = Omega_Out;
    }
  }
*/

void CKO_O::Calc( double dt )
  {
  SET_BP BreakPoint;
  if ( pFlow_In == NULL )
    return;
  double F_mol = pFlow_In->Flow_mol;
  _Flow_kg_in = F_mol * pFlow_In->To_kg; 
  _Flow_kg_out = pFlow_Out->Flow_kg(); 
  double Eps = 1.;
  CComp * pComp;
  if ( F_mol > 0. )
    {
    In.PH( pFlow_In );
    Eps = In.E;
    pComp = pFlow_In->Get_Comp();
    if ( pComp == NULL )
      return;
    }
  else
    {
    pComp = pWater;
    Eps = 1.;
    }
  _Eps = Eps; 
  dP_In = dP_In_max * ( 1. - Eps );
// давление
  double A_W = A_H2O * 100.;
  if ( L < 0.1 && P_H2O < Pz_H2O )
    A_W = 10. * L * A_H2O * 100.;
  double C_H2O = 0.;
  if ( pComp->IsComp[G_H2O] )
    C_H2O = pComp->Cmol[G_H2O];
  double C_Gas = 1. - C_H2O;
  P_Gas = ( P_Gas + F_mol * Eps * C_Gas * Ksi ) / ( 1. + A_Gas * Ksi );  //Pz_Gas = 0 - вывод газов
  P_H2O = ( P_H2O + F_mol * Eps * C_H2O * Ksi + A_W * Pz_H2O * Ksi ) / ( 1. + A_W * Ksi );
  P = P_Gas + P_H2O;
  _P = P - 1.;
  double Tst = P - ( AA * F_mol + BB );
  V_H2O += k_H2O * ( A_W * ( P_H2O - Pz_H2O ) - V_H2O );
// жидкость
  double Flow_in = F_mol * ( 1. - In.E ) + V_H2O;
//  if ( Flow_in < 0. )
//    Flow_in = 0.;
  M_H2O += dt * Flow_in;
  M_H2O -= dt * pFlow_Out->Flow_mol;
  if ( M_H2O < 0. )
   M_H2O = 0.;
  L = M_H2O * 18. / ( Vol_Liq * 10. );
  Omega = Omega_max * 1e3;
  _Level = L * 100.;
  if ( _Level > 100. )
    _Level = 100.;
  Flow_liq = Flow_in * 2. * L;
//
  if ( F_mol > 0. )
    {
    // температура
    const double Cp = 4.18 * 18.;
    double R = pGas[G_H2O]->H_vap( T );
    double T_new = ( Cp * M_H2O * T + dt * ( Cp * Flow_in * In.T + V_H2O * R + kCool * 10. * T_air )) /
                   ( Cp * M_H2O + dt * ( kCool * 10. + Cp * Flow_in ));
    T += LimdX ( T_new - T, 2. );
    if ( T < T_air )
      T = T_air;
    if ( T > 300. )
      T = 300.;
    }
  else
    {
    const double Cp = 4.18 * 18.;
    double R = pGas[G_H2O]->H_vap( T );
    double T_new = ( Cp * M_H2O * T + dt * ( V_H2O * R + kCool * 10. * T_air )) /
                   ( Cp * M_H2O + dt * kCool * 10. );
    T += LimdX ( T_new - T, 2. );
    if ( T < T_air )
      T = T_air;
    if ( T > 300. )
      T = 300.;
    }
 // _T = T;
////////////////////////////////////////
  Pz_H2O = pGas[G_H2O]->Pz( T );
//
  Ksi = dt * RP * ( T + TK ) / ( Vol_Gas * 10. );
  AA = Eps * Ksi * ( 1. /  ( A_W * Ksi + 1. ) + 1. /  ( A_Gas * 10. * Ksi + 1. ) );
  BB = ( P_H2O + A_W * Ksi * Pz_H2O ) / ( A_W * Ksi + 1. ) +
       P_Gas / ( A_Gas * 10. * Ksi + 1. );
  FINITE(BB)
  *pFlow_Out = *pWaterFlow;
  pFlow_Out->T = T;
  pFlow_Out->h_Flow = pGas[G_H2O]->H_l( T );
  }