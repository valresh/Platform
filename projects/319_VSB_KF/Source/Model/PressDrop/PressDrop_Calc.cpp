#include "stdafx.h"
#define DLL_PressDrop
#include "PressDrop.h"
#include "Err.h"
#include "CommProc.h"


void CPressDrop::CalcCoef ( CFlow * pFlow )
  {
  SET_BP BreakPoint;
  double Xi = 1.;
  pEps->PH ( pFlow ); 
  bool Mix = true;
  if ( pEps->E <= 0. )
    {
    // жидкость
    CalcLiqCoef ( pFlow, Xi );
    Mix = false;
    }
  if ( pEps->E >= 1. )
    {
    //  газ
    CalcGasCoef ( pFlow, Xi );
    Mix = false;
    }
  // смесь
  if ( Mix )
    CalcMixCoef ( pFlow, Xi );
  //
  if ( !_finite ( Xi_Flow ))
    Xi_Flow = Xi;
  Xi_Flow = 0.9 * Xi_Flow + 0.1 * Xi;
  Omega = k_Omega / Xi_Flow;
  }

void CPressDrop::SetGasEtalon( CComp * pComp, double Flow, double P, double dP, double T )
  {
  SET_BP BreakPoint;
  double F = Flow;
  if ( F < 0.01 )
    F = 0.01;
  double Mw = ::Mw ( pComp );
  double Q = F * Mw;
  double Mu = Vis_Gas( P, T, pComp->IsComp, pComp->Cmol  );
  double Ro = P * Mw / ( RP * ( pEps->T + TK )); // Газ
  //
  Re = D * Q / ( 3600. * Mu * S ); 
  //
  double Ksi;
  if ( Re < 2300 )
    Ksi = 105.083 / Re;
  else
    Ksi = 0.3164 / pow ( Re, 0.25 );
  double Xi = F * Mw * Mw  * Ksi / Ro;
//  Omega * dP = F
//  Omega = k_Omega / Xi;
//  k_Omega * dP = Xi * F
  k_Omega = Xi * F / dP;
  }

void CPressDrop::SetLiqEtalon( CComp * pComp, double Flow, double dP, double T )
  {
  double F = Flow;
  if ( F < 0.01 )
    F = 0.01;
  double Mw = ::Mw ( pComp );
  double Q = F * Mw;
  double Mu = Vis_Liq( T, pComp->IsComp, pComp->Cmol  );
  double Ro = Ro_liq ( T, pComp );
  //
  Re = D * Q / ( 3600. * Mu * S ); 
  //
  double Ksi;
  if ( Re < 2300 )
    Ksi = 105.083 / Re;
  else
    Ksi = 0.3164 / pow ( Re, 0.25 );
  double Xi = F * Mw * Mw  * Ksi / Ro;
  //  Omega * dP = F
  //  Omega = k_Omega / Xi;
  //  k_Omega * dP = Xi * F
  k_Omega = Xi * F / dP;
  }

void CPressDrop::CalcLiqCoef ( CFlow * pFlow, double & Xi )
  {
  SET_BP BreakPoint;
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 0.01 )
    F = 0.01;
  CComp * pComp = pFlow->Get_Comp();
  double Mw = ::Mw ( pComp );
  double Q = F * Mw;
  double Mu = Vis_Liq( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Ro = Ro_liq ( pEps->T, pComp );
  //
  Re = D * Q / ( 3600. * Mu * S ); 
  //
  double Ksi;
  if ( Re < 2300 )
    Ksi = 105.083 / Re;
  else
    Ksi = 0.3164 / pow ( Re, 0.25 );
  Xi = F * Mw * Mw  * Ksi / Ro;
  }

void CPressDrop::CalcGasCoef ( CFlow * pFlow, double & Xi )
  {
  SET_BP BreakPoint;
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 0.01 )
    F = 0.01;
  CComp * pComp = pFlow->Get_Comp();
  double Mw = ::Mw ( pComp );
  double Q = F * Mw;
  double Mu = Vis_Gas( pFlow->P, pEps->T, pComp->IsComp, pComp->Cmol  );
  double Ro = pFlow->P * Mw / ( RP * ( pEps->T + TK )); // Газ
  //
  Re = D * Q / ( 3600. * Mu * S ); 
  //
  double Ksi;
  if ( Re < 2300 )
    Ksi = 105.083 / Re;
  else
    Ksi = 0.3164 / pow ( Re, 0.25 );
  Xi = F * Mw * Mw  * Ksi / Ro;
  }

void CPressDrop::CalcMixCoef ( CFlow * pFlow, double & Xi )
  {
  SET_BP BreakPoint;
  double D = 0.04;
  double S = 0.01;
  bool IsComp[K_GAS];
  double X[K_GAS]; 
  double Y[K_GAS];
  pEps->SetCompData ( IsComp, X, Y, NULL, NULL );
  pEps->Calc_Mw();
  CComp * pComp = pFlow->Get_Comp();
  double Mw = ::Mw ( pComp );
  double Q = fabs ( pFlow->Flow_mol ) * Mw;
  double Mu_gas = Vis_Gas( pFlow->P, pEps->T, IsComp, Y  );
  double Mu_liq = Vis_Liq( pEps->T, IsComp, X  );
  //
  double Rogas = pFlow->P * Mw / ( RP * ( pEps->T + TK ));
  double Roliq = Ro_liq ( pEps->T, IsComp, X ); // жидкость
  double Mg = pEps->E * pEps->Mw_gas;
  double Ml = ( 1. - pEps->E ) * pEps->Mw_liq;
  double N = 1. / ( Mg + Ml );
  double Mu = ( Mg * Mu_gas + Ml * Mu_liq ) * N;
  double Ro = 1. / ( ( Mg / Rogas + Ml / Roliq ) * N );
  double Re = D * Q / ( 3600. * Mu * S ); 
  //
  double Ksi;
  if ( Re < 2300 )
    {
    if ( Re < 100. )
      Re = 100.;
    Ksi = 105.083 /  Re;
    }
  else
    Ksi = 0.3164 / pow (  Re, 0.25 );
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 0.01 )
    F = 0.01;
  Xi = F * Mw * Mw  * Ksi / Ro;
  }
