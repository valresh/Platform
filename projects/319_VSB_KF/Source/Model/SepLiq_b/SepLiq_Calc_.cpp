#include "stdafx.h"
#include "SepLiq.h"
#include "Err.h"
#include "CommProc.h"
#include "SysDataTypes.h"


void CSepLiq::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( pFlow[0] == NULL || pFlow[1] == NULL )
    return;
  if ( pFlow[0]->ID_COMP == 0 )
    return;
  if ( Reg_L.On() )
    Level_Z = Reg_L;
  else
    Level_Z = 0.;
  //if ( Add_M_Water > 0. )
  //  {
  //  M_liq = Add_M_Water / Mw_H2O;
  //  Mw_Liq = Mw_H2O;
  //  _M_liq = M_liq * Mw_Liq;
  //  T_liq = T_air;
  //  IsComp[G_H2O] = true;
  //  CLEAR(X)
  //  X[G_H2O] = 1.;
  //  CLEAR(h_Liq)
  //  h_liq_mix = h_Liq[G_H2O] = pGas[G_H2O]->H_l( T_air );
  //  To_m3 = 18e-3;
  //  }
// Смесь на входе
  if(!_finite ( pLIQ->Cmol[1] ))
	  pLIQ->Set ( pWater );
//
//
  if ( Eps_Sep < 0. )
    Eps_Sep = 0.;
  if ( Eps_Sep > 100. )
    Eps_Sep = 100.;
  double EpsSep = Eps_Sep * 0.01;
  double EpsSep1 = 1. - EpsSep;

  CComp * pCompIn = NULL;
  double h_in = 0.;
  CFlow * pF_in = NULL;
  CFlow * pF_out = NULL;
  EpsIn.T_min = T_min;
  EpsIn.T_max = T_max;
  EpsIn.NoInLiq[G_N2] = NoInLiq_N2;
  if ( pFlow[0]->Flow_mol > 0. )
    {
    pF_in = pFlow[0];
    pCompIn = pF_in->Get_Comp();
    h_in = pF_in->h_Flow;
    EpsIn.PH ( pF_in, T_gas );
    pF_out = pFlow[1];
    }
  else
    {
    pF_in = pFlow[1];
    pCompIn = pF_in->Get_Comp();
    h_in = pF_in->h_Flow;
    EpsIn.PH ( pF_in, T_gas );
    pF_out = pFlow[0];
    }
///////////////////////////////////
  double Flow_in = fabs ( pF_in->Flow_mol ) * EpsSep;
  double E = EpsIn.E;
  Eps_in = E;
  k_Flow = 0.9 * k_Flow + 0.1 * ( EpsSep * E + EpsSep1 );
//  
  double F_gas = Flow_in * E;
  double F_liq = Flow_in - F_gas;
  EpsIn.Calc_Mw();
  _F_liq = F_liq * EpsIn.Mw_liq;
  double _F_out_liq = Flow_out_liq * Mw_Liq;
 _dM_liq = _F_liq - _F_out_liq;
//
  double dMl = F_liq * dt;
  double dMg = F_gas * dt;
////////////////////////////////////////////////////////////////
  bool IsCompIn[K_GAS];
  double X_in[K_GAS];
  double h_L_in[K_GAS];
  double h_G_in[K_GAS];
  EpsIn.SetCompData ( IsCompIn, X_in, Y, h_L_in, h_G_in );
  double dH = 0.;
  double Sx = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsCompIn[c] || IsComp[c] )
      {
      IsComp[c] = true;
      X[c] = M_liq * X[c] + dMl * X_in[c];
      if( X[c] < 1e-7)
        X[c] = 0;
      Sx += X[c];
      dH += dMl * X_in[c] * ( h_L_in[c] - h_Liq[c] ); 
      }
    }
  if ( Sx < 1e-8 )
    {
    IsComp[G_N2] = true;
    X[G_N2] = 1.;
    }
  Norm ( IsComp, X );
//
  double dMlio = ( F_liq - Flow_out_liq ) * dt;
  M_liq += dMlio;
  if ( M_liq < 0. )
    M_liq = 0.;
//
  Heat.Lev = Level;
  Heat.T_ext = T_liq;                                                                                                                                                         
  Heat.Lev_Zm = H_heat;
  Heat.KS = K_heat * S_heat * 3.6;
  Heat.Calc( dt );
  dH += Heat.Q_Heat * dt;
//
  T_gas = 0.5 * T_gas + 0.5 * EpsIn.T;
  _T_gas = T_gas;
//
  double dM_in = F_liq * dt;
  double Cp_Liq = ::Cp_Liq ( T_liq, IsComp, X ) * M_liq;
  T_liq += LimdX ( dH / ( Cp_Corp + Cp_Liq ), 1. );
  if ( T_liq < T_min )
    T_liq = T_min;
  _T_liq = T_liq;
  CalcH( );
///////////////////////////////////////////////////////
  if ( EpsSep >= 1. )
    {
    pGAS->Set( IsComp, Y );
    h_gas_mix = EpsIn.h_gas;
    Eps_out = 1.;
    }
  else
    {
    double F_from_in = Flow_in * EpsSep1;
    double F_gas_from_in = F_from_in * E;
    double Y_mix[K_GAS];
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        Y_mix[c] = F_from_in * pCompIn->Cmol[c] + F_gas * Y[c];
        }
      else
        Y_mix[c] = 0.;
      }
    h_gas_mix = EpsIn.h_gas;
    if ( F_gas + F_from_in > 1e-3 )
      h_gas_mix = ( F_from_in * h_in + F_gas * EpsIn.h_gas ) / ( F_gas + F_from_in );
    pGAS->Set( IsComp, Y_mix );
    Eps_out = ( F_gas_from_in + F_gas ) / ( F_from_in + F_gas );
    }
//
 pLIQ->Set( IsComp, X );
//
/////////////////////////////////
  pF_out->ID_COMP = ID_GAS;
  pF_out->pComp = pGAS;
  pF_out-> T = T_gas;
  pF_out-> h_Flow = h_gas_mix;
  pF_out-> To_kg = ::Mw ( pGAS );
  pF_out-> To_m3 = M3;
/////////////////////////////////
  CalcLiq( dt );
  Температура = T_gas;
  Давление = P - 1.;
  Давление_абс = P;
  double L1 = Lev_0_100( Level, Lev_1_0, Lev_1_100 );
  Уровень = L1 * Масштаб;
/////////////////////////////////
  if ( Use_mol )
    {
    Use_mol = false;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        X_mol[c] = X[c] * 100.;
        Y_mol[c] = Y[c] * 100.;
        }
      }
    }
  if ( Use_mass )
    {
    Use_mass = false;
    double Sx = 0., Sy = 0.;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        X_mass[c] = X[c] * pGas[c]->Mw;
        Y_mass[c] = Y[c] * pGas[c]->Mw;
        Sx += X_mass[c];
        Sy += Y_mass[c];
        }
      }
    Sx = 100. / Sx;
    Sy = 100. / Sy;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        X_mass[c] *= Sx;
        Y_mass[c] *= Sy;
        }
      }
    }
	}

void CSepLiq::CalcLiq( double dt )
  {
  SET_BP BreakPoint;
  Flow_out_liq = pOutLiq->Flow_mol;
  if ( kStep % 5 == 0 && M_liq > 1. )
    {
    SET_BP BreakPoint;
    Mw_Liq = ::Mw ( IsComp, X );
    To_m3 = To_m3_liq ( T_liq, IsComp, X );
    if ( To_m3 * 1000. > Mw_Liq )
      Ro_Liq = Mw_Liq / To_m3;
//    Get_RoMw = false;
    }
  FINITE(M_liq)
  double To_L = To_m3 / Volume;
  double To_M = 1. / To_L;
  Level = M_liq * To_L;
  _Level = Level * 100.;
  _M_liq = M_liq * Mw_Liq;
  if ( Level > Lev_max )
    {
    Level = Lev_max;
    M_liq = Level * To_M;
    }
  if ( Level_Z > 0. && Level >= 0.01 )
    {
    Level = Level_Z * 0.01;
    M_liq = Level * To_M;
    Level_Z = 0.;
    Reg_L.Off();
    }
  if ( Level_Z < 0. && Level >= 0.01 )
    {
    Level = -0.01 * Level_Z;
    M_liq = Level * To_M;
    }
  _Level = Level * 100.;
  pOutLiq->ID_COMP = ID_LIQ;
  pOutLiq->pComp = pLIQ;
  pOutLiq-> T = T_liq;
  if ( Level > Lev_Funnel * 0.01 )
    {
    pOutLiq->h_Flow = h_liq_mix;
    pOutLiq-> To_kg = ::Mw ( pLIQ );
    pOutLiq-> To_m3 = To_m3;
    Eps_bott = 0.;
    }
  else
    {
    double E1 = 1.;
    if ( Lev_Funnel > 0. )
      E1 = 100. * Level / Lev_Funnel;
    double E = 1. - E1;
    Eps_bott = E;
    double Mix[K_GAS];
    CLEAR( Mix );
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( IsComp[n] )
        Mix[n] = E * Y[n] + E1 * X[n];
      }
    pOutLiq->h_Flow = E1 * h_liq_mix + E * h_gas_mix;
    pLIQ->Set ( IsComp, Mix );
    Norm ( pLIQ );
    pOutLiq -> To_kg = Mw_Liq;
    pOutLiq -> To_m3 = To_m3_liq ( T_liq, IsComp, Mix );
    F_bott = pOutLiq -> Flow_mol;
 //
    double Flow_Gas_Bott = pOutLiq->Flow_mol * E;
    }
  }

void CSepLiq::CalcH( )
  {
  SET_BP BreakPoint;
  double hs = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsComp[n] )
      continue;
    double h;
    h = pGas[n]-> H_l ( T_liq );
    h_Liq[n] = h;
    hs += X[n] * h;
    }
  h_liq_mix = hs;
  }
