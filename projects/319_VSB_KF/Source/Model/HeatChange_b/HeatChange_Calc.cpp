#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"
#include "CommProc.h"
#include "SysDataTypes.h"


void CHeatChange::Calc( double dt )
  {
	SET_BP BreakPoint;
  Heat.BreakPoint = BreakPoint;
	//.....................................
	Set_1F( );
	//.....................................
	SET_BP BreakPoint;
	Mixture( );
	CalcCoef( );
	//.....................................
	SET_BP BreakPoint;
	Calc_1F( dt );
	//.....................................
	T_in_trub = T_in_Trub;
	T_out_trub = T_out_Trub;
	T_in_vol = T_in_Vol;
	T_out_vol = T_out_Vol;
	FINITE(_h_Trub_in)
	FINITE(_h_Trub_out)
	FINITE(_h_Vol_in)
	FINITE(_h_Vol_out)
	bRegTask = Reg_T_Z_Trub.Use || Reg_T_Z_Vol.Use || Reg_dP_trub_Z.Use || 
		Reg_dP_vol_Z.Use || Reg_F_trub_Z.Use || Reg_F_vol_Z.Use;
  }

void CEtalon::CalcH2OCoef ( )
  {
// Базовая точка 
  double TC = 20.;
  double Q = 10000.;
  double Mw = Mw_H2O;
  double D = 0.04;
  double S = 0.01;
  double Ro = 1000.;
//
  double Mu = pGas[G_H2O]->Vis_liq( TC );
  double Lam = pGas[G_H2O]->Lam_liq( TC );
  double Cp = pGas[G_H2O]->C_Liq( TC );
// Re
  Re_Etalon = D * Q / ( 3600. * Mu * S );  // Переменная часть Re
  double U = Q / Ro / S / 3600.;
//
  Pr_Etalon = Cp * Mu / ( Mw * Lam ) * 1000.;
//  Nu_Etalon = 0.021 * pow ( Re, 0.8 ) * pow ( Pr_Etalon, 0.3 );
  Nu_Etalon = 0.0021 * pow ( Re_Etalon, 0.8 ) * pow ( Pr_Etalon, 0.3 ); // Относительный
  Alfa_Etalon = Nu_Etalon * Lam / D; 
//
  KKK();
  }


void CHeatChange::CalcCoef ( )
  {
// Трубы
  double Xi_Rel_Trub = 1.;
  if ( Calc_K_trub == 1 )
    {
    bool Mix = true;
    if ( Heat.S_Trub_In.E <= 0. && Heat.S_Trub_Out.E <= 0. )
      {
// В трубах - жидкость
      Etalon_Trub.CalcLiqCoef ( In_Trub, &Heat.S_Trub_In, Alfa_Rel_Trub, Xi_Rel_Trub );
      Mix = false;
      }
    if ( Heat.S_Trub_In.E >= 1. && Heat.S_Trub_Out.E >= 1. )
      {
// В трубах - газ
      Etalon_Trub.CalcGasCoef ( In_Trub, &Heat.S_Trub_In, Alfa_Rel_Trub, Xi_Rel_Trub );
      Mix = false;
      }
// При конденсации пока ничего не меняем
    if ( Mix )
      Etalon_Trub.CalcMixCoef ( In_Trub, &Heat.S_Trub_In, Alfa_Rel_Trub, Xi_Rel_Trub );
    }
  else
    {
    Alfa_Rel_Trub = 1.;
    Xi_Trub = 1.;
    }
// Объем
  double Xi_Rel_Vol = 1.;
  if ( Calc_K_vol == 1 )
    {
    bool Mix = true;
    if ( Heat.S_Vol_In.E <= 0. && Heat.S_Vol_Out.E <= 0. )
      {
  // В объеме - жидкость
      Etalon_Vol.CalcLiqCoef ( In_Vol, &Heat.S_Vol_In, Alfa_Rel_Vol, Xi_Rel_Vol );
//      Mix = false;
      }
    if ( Heat.S_Vol_In.E >= 1. && Heat.S_Vol_Out.E >= 1. )
      {
  // В объеме - газ
      Etalon_Vol.CalcGasCoef ( In_Vol, &Heat.S_Vol_In, Alfa_Rel_Vol, Xi_Rel_Vol );
//      Mix = false;
      }
    // При конденсации пока ничего не меняем
    if ( Mix )
      Etalon_Vol.CalcMixCoef ( In_Vol, &Heat.S_Vol_In, Alfa_Rel_Vol, Xi_Rel_Vol );
    }
  else
    {
    Alfa_Rel_Vol = 1.;
    Xi_Vol = 1.;
    }
//
  Xi_Trub = 0.9 * Xi_Trub + 0.1 * Xi_Rel_Trub;
  Xi_Vol = 0.9 * Xi_Vol + 0.1 * Xi_Rel_Vol;
  double K_Real_trub = Alfa_Rel_Trub * K_trub;
  double K_Real_vol = Alfa_Rel_Vol * K_vol;
//  double KK = K_Real_trub * K_Real_vol * K_Rel;
//  double K_heat0 = KK / ( K_Real_trub + K_vol + KK * R_trub );
  K_heat = K_Real_trub * K_Real_vol / ( K_Real_trub + K_Real_vol );
//  double D = K_heat / K_heat0;
//  KKK();
  }

void CHeatChange::CalcEtalonTrub( )
  {
  // Трубы
  if ( Heat.S_Trub_In.E <= 0. && Heat.S_Trub_Out.E <= 0. )
    {
    // В трубах - жидкость
    Etalon_Trub.CalcLiqEtalon ( In_Trub, &Heat.S_Trub_In );
    return;
    }
  if ( Heat.S_Trub_In.E >= 1. && Heat.S_Trub_Out.E >= 1. )
    {
    // В трубах - газ
    Etalon_Trub.CalcGasEtalon ( In_Trub, &Heat.S_Trub_In );
    return;
    }
  Etalon_Trub.CalcMixEtalon ( In_Trub, &Heat.S_Trub_In );
  }

void CHeatChange::CalcEtalonVol( )
  {
  // Объем
  if ( Heat.S_Vol_In.E <= 0. && Heat.S_Vol_Out.E <= 0. )
    {
    // В объеме - жидкость
    Etalon_Vol.CalcLiqEtalon ( In_Vol, &Heat.S_Vol_In );
    return;
    }
  if ( Heat.S_Vol_In.E >= 1. && Heat.S_Vol_Out.E >= 1. )
    {
    // В объеме - газ
    Etalon_Vol.CalcGasEtalon ( In_Vol, &Heat.S_Vol_In );
    return;
    }
  Etalon_Vol.CalcMixEtalon( In_Vol, &Heat.S_Vol_In );
  }

void CEtalon::CalcLiqCoef ( CFlow * pFlow, CSEps * pEps, double & Alfa_Rel, double & Xi_Rel )
  {
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 0.01 )
    F = 0.01;
  CComp * pComp = pFlow->Get_Comp();
  double Mw = ::Mw ( pComp );
  double Q = F * Mw;
  double Mu = Vis_Liq( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Lam = Lam_Liq( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Cp = Cp_Liq( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Ro = Ro_liq ( pEps->T, pComp ); // жидкость
//
  Re = D * Q / ( 3600. * Mu * S ); 
  Pr = Cp * Mu / ( Mw * Lam ) * 1000.;
  Nu = 0.0021 * pow ( Re, 0.8 ) * pow ( Pr, 0.3 );
  Alfa = Nu * Lam / D; 
//
  Alfa_Rel = Alfa / Alfa_Etalon;
  //
  double Ksi;
  if ( Re < 2300 )
    Ksi = 105.083 / Re;
  else
    Ksi = 0.3164 / pow ( Re, 0.25 );
  double Xi = F * Mw * Mw  * Ksi / Ro;
  Xi_Rel = Xi / Xi_Etalon;
  }

void CEtalon::CalcGasCoef ( CFlow * pFlow, CSEps * pEps, double & Alfa_Rel, double & Xi_Rel )
  {
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 0.01 )
    F = 0.01;
  CComp * pComp = pFlow->Get_Comp();
  double Mw = ::Mw ( pComp );
  double Q = F * Mw;
  double Mu = Vis_Gas( pFlow->P, pEps->T, pComp->IsComp, pComp->Cmol  );
  double Lam = Lam_Gas( pFlow->P, pEps->T, pComp->IsComp, pComp->Cmol  );
  double Cp = Cp_IG( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Ro = pFlow->P * Mw / ( RP * ( pEps->T + TK )); // Газ
  //
  Re = D * Q / ( 3600. * Mu * S ); 
  Pr = Cp * Mu / ( Mw * Lam ) * 1000.;
  Nu = 0.0021 * pow ( Re, 0.8 ) * pow ( Pr, 0.3 );
  Alfa = Nu * Lam / D; 
  Alfa_Rel = Alfa / Alfa_Etalon;
  //
  double Ksi;
  if ( Re < 2300 )
    Ksi = 105.083 / Re;
  else
    Ksi = 0.3164 / pow ( Re, 0.25 );
  double Xi = F * Mw * Mw  * Ksi / Ro;
  Xi_Rel = Xi / Xi_Etalon;
  }

void CEtalon::CalcLiqEtalon ( CFlow * pFlow, CSEps * pEps )
  {
  CComp * pComp = pFlow->Get_Comp();
  double Mw = ::Mw ( pComp );
  double Q = fabs ( pFlow->Flow_mol ) * Mw;
  double Mu = Vis_Liq( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Lam = Lam_Liq( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Cp = Cp_Liq( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Ro = Ro_liq ( pEps->T, pComp ); // жидкость
//
  Re_Etalon = D * Q / ( 3600. * Mu * S ); 
  Pr_Etalon = Cp * Mu / ( Mw * Lam ) * 1000.;
  Nu_Etalon = 0.0021 * pow ( Re_Etalon, 0.8 ) * pow ( Pr_Etalon, 0.3 );
  Alfa_Etalon = Nu_Etalon * Lam / D;
//
  double Ksi;
  if ( Re_Etalon < 2300 )
    Ksi = 105.083 /  Re_Etalon;
  else
    Ksi = 0.3164 / pow (  Re_Etalon, 0.25 );
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 10. )
    F = 10.;
  double Xi = F * Mw * Mw  * Ksi / Ro;
  Xi_Etalon = Xi;
  }

void CEtalon::CalcGasEtalon ( CFlow * pFlow, CSEps * pEps )
  {
  double D = 0.04;
  double S = 0.01;
  CComp * pComp = pFlow->Get_Comp();
  double Mw = ::Mw ( pComp );
  double Q = fabs ( pFlow->Flow_mol ) * Mw;
  double Mu = Vis_Gas( pFlow->P, pEps->T, pComp->IsComp, pComp->Cmol  );
  double Lam = Lam_Gas( pFlow->P, pEps->T, pComp->IsComp, pComp->Cmol  );
  double Cp = Cp_IG( pEps->T, pComp->IsComp, pComp->Cmol  );
  double Ro = pFlow->P * Mw / ( RP * ( pEps->T + TK )); // Газь
  //
  Re_Etalon = D * Q / ( 3600. * Mu * S ); 
  Pr_Etalon = Cp * Mu / ( Mw * Lam ) * 1000.;
  Nu_Etalon = 0.0021 * pow ( Re_Etalon, 0.8 ) * pow ( Pr_Etalon, 0.3 );
  Alfa_Etalon = Nu_Etalon * Lam / D; 
//
  double Ksi;
  if ( Re_Etalon < 2300 )
    Ksi = 105.083 /  Re_Etalon;
  else
    Ksi = 0.3164 / pow (  Re_Etalon, 0.25 );
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 10. )
    F = 10.;
  double Xi = F * Mw * Mw  * Ksi / Ro;
  Xi_Etalon = Xi;
  }

void CEtalon::CalcMixEtalon ( CFlow * pFlow, CSEps * pEps )
  {
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
  double Mu_liq = Vis_Gas( pFlow->P, pEps->T, IsComp, X  );
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
    Ksi = 105.083 /  Re;
  else
    Ksi = 0.3164 / pow (  Re, 0.25 );
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 10. )
    F = 10.;
  double Xi = F * Mw * Mw  * Ksi / Ro;
  Xi_Etalon = Xi;
  }

void CEtalon::CalcMixCoef ( CFlow * pFlow, CSEps * pEps, double & Alfa_Rel, double & Xi_Rel )
  {
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
    Ksi = 105.083 /  Re;
  else
    Ksi = 0.3164 / pow (  Re, 0.25 );
  double F = fabs ( pFlow->Flow_mol );
  if ( F < 0.01 )
    F = 0.01;
  double Xi = F * Mw * Mw  * Ksi / Ro;
  Xi_Rel = Xi / Xi_Etalon;
  }


void CHeatChange::Mixture( )
  {
  SET_BP
  if ( !Перемешивание )
    {
    In_Trub->Copy( In_Trub_0 );
    In_Vol->Copy( In_Vol_0 );
    k_Trub = 1.;
    k_Vol = 1.;
    return;
    }
//
  double F_Vol = fabs ( In_Vol_0->Flow_mol );
  double F_Trub = fabs ( In_Trub_0->Flow_mol );
  double F_min = F_Trub;
  if ( F_Vol < F_min )
    F_min = F_Vol;
  double F_mix = F_min * Def_Trub;
  if ( F_mix < 0.1 )
    {
    In_Trub->Copy( In_Trub_0 );
    In_Vol->Copy( In_Vol_0 );
    k_Trub = 1.;
    k_Vol = 1.;
    return;
    }
  CComp * pComp_Trub = In_Trub_0->Get_Comp();
  CComp * pComp_Vol = In_Vol_0->Get_Comp();
  double Mw_Trub =  Mw ( pComp_Trub );
  double Mw_Vol =  Mw ( pComp_Vol );
  if ( In_Trub_0->P > In_Vol_0->P )
    {
// Из труб в объем
    SET_BP
    pCompVol->Summ ( F_Vol, pComp_Vol, F_mix, pComp_Trub );
    double h = ( F_Vol * In_Vol_0->h_Flow + F_mix * In_Trub_0->h_Flow )/
               ( F_Vol + F_mix );
    In_Vol->P = In_Vol_0->P;
    double F_mix_kg = F_mix * Mw_Trub;
    In_Vol->Flow_mol = In_Vol_0->Flow_mol + F_mix_kg / Mw_Vol;
    In_Vol->h_Flow = h;
    In_Vol->ID_COMP = ID_Comp_Vol;
    *In_Trub = *In_Trub_0;
    In_Trub->Flow_mol = In_Trub_0->Flow_mol - F_mix;
    In_Trub->P = In_Trub_0->P;
    k_Trub = In_Trub->Flow_mol / In_Trub_0->Flow_mol;
    k_Vol = In_Vol->Flow_mol / In_Vol_0->Flow_mol;
    return;
    }
// Из объема в трубы
  SET_BP
  pCompTrub->Summ ( F_Trub, pComp_Trub, F_mix, pComp_Vol );
  double h = ( F_Trub * In_Trub_0->h_Flow + F_mix * In_Vol_0->h_Flow )/
    ( F_Trub + F_mix );
  double F_mix_kg = F_mix * Mw_Vol;
  In_Trub->P = In_Trub_0->P;
  In_Trub->Flow_mol = In_Trub_0->Flow_mol + F_mix_kg / Mw_Trub;
  In_Trub->h_Flow = h;
  In_Trub->ID_COMP = ID_Comp_Trub;
  *In_Vol = *In_Vol_0;
  In_Vol->Flow_mol = In_Vol_0->Flow_mol - F_mix;
  In_Vol->P = In_Vol_0->P;
  k_Trub = In_Trub->Flow_mol / In_Trub_0->Flow_mol;
  k_Vol = In_Vol->Flow_mol / In_Vol_0->Flow_mol;
  }
