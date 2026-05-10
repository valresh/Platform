#include "stdafx.h"
#include "KO.h"
#include "Err.h"
#include "CommProc.h"


void CKO_N::Calc( double dt )
  {
  SET_BP BreakPoint;
  if ( pFlow_In == NULL )
    return;
  double F_mol = pFlow_In->Flow_mol;
  _Flow_kg_in = F_mol * pFlow_In->To_kg; 
  _Flow_kg_out = pFlow_Out->Flow_kg(); 
  double Eps = 1.;
  CComp * pComp;
  if ( F_mol > 0.1 )
    {
    In.PH( pFlow_In );
    Eps = In.E;
    pComp = pFlow_In->Get_Comp();
    if ( pComp == NULL )
      return;
    }
  else
    {
    In.T = 0.99 * In.T + 0.01 * T_air;
    //if ( In.T < 20. )
    //  {
    //  M_H2O *= 0.9;
    //  }
    pComp = pWater;
    Eps = 1.;
    }
  _Eps = Eps; 
// давление
  double C_H2O = 0.;
  if ( pComp->IsComp[G_H2O] )
    C_H2O = pComp->Cmol[G_H2O];
  P_H2O = Pz_H2O;
  P = P_H2O;
  _P = P - 1.;
// жидкость
  double Flow_in = pFlow_In->Flow_mol;// * ( 1. - Eps );
  if ( Flow_in < 0. )
    Flow_in = 0.;
  double Flow_out = pFlow_Out->Flow_mol;
  if ( Flow_out < 0. )
    Flow_out = 0.;
  M_H2O += dt * ( Flow_in - Flow_out );
  if ( M_H2O < 0. )
   M_H2O = 0.;
  L = M_H2O * 18. / ( Vol_Liq * 10. );
  if ( L > 1. )
    {
    L = 1.01;
    M_H2O = Vol_Liq * 10. * L / 18.;
    Full = true;
    }
  if ( L < 0.95 )//|| P_out < Pz_H2O )
    {
    Full = false;
    }
  _Level = L * 100.;
//
  T += 0.1 * ( In.T - T );
  if ( T < T_air )
    T = T_air;
  if ( T > 300. )
    T = 300.;
////////////////////////////////////////
  Pz_H2O = pGas[G_H2O]->Pz( T );
//
  //AA = Eps * Ksi * ( 1. /  ( A_W * Ksi + 1. ) + 1. /  ( A_Gas * 10. * Ksi + 1. ) );
  //BB = ( P_H2O + A_W * Ksi * Pz_H2O ) / ( A_W * Ksi + 1. ) +
  //     P_Gas / ( A_Gas * 10. * Ksi + 1. );
  //FINITE(BB)
  double Pi = Pz_H2O;
  if ( Pi < 1. )
    Pi = 1.;
  B_In = 0.9 * B_In + 0.1 * Pi * 1.05;
  B_Out = 0.9 * B_Out + 0.1 * Pi * 1.01;
  double dP = P_in - 1.02 * Pz_H2O;
  dP_Cond = dP;
  if ( Full )
    {
    double K = 1. + LimdX( kRegOmega * dP, 0.1 );
    Omega_Full *= K;
    if ( Omega_Full > Omega_Full_max )
      Omega_Full = Omega_Full_max;
    if ( Omega_Full < Omega_Full_min )
      Omega_Full = Omega_Full_min;
    }
//
  *pFlow_Out = *pWaterFlow;
  pFlow_Out->T = T;
  pFlow_Out->h_Flow = pGas[G_H2O]->H_l( T );
  }