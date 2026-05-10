#include "stdafx.h"
#include "SepLiq.h"
#include "Err.h"
#include "CommProc.h"
#include "SysDataTypes.h"

const double M_liq_min = 0.1;


void CSepLiq::Calc( double dt )
  {
  SET_BP BreakPoint;
  for ( int n = 0; n < 3; n++ )
    {
    if ( pFlow[n] == NULL )
      return;
    if ( pFlow[n]->Flow_mol > 0. && pFlow[n]->ID_COMP == 0 )
      return;
    }
  if ( Разгерметизация )
    {
    Razgerm( );
    return;
    }
  double Flow_in = 0.;
  double Flow_out = 0.;
  if ( pFlow[0]->Flow_mol > 0.1 )
    _Eps_inv_gas = State[0]->E;
  for ( int n = 0; n < 3; n++ )
    {
    CFlow * pF = pFlow[n];
    double F_mol = pF ->Flow_mol;
    if ( F_mol > 0. )
      {
      // Вход 
      CComp * pComp = pF->Get_Comp();
      State[n]->T_min = T_min;
      State[n]->T_max = T_max;
      State[n]->NoInLiq[G_N2] = NoInLiq_N2;
      State[n]->PH ( pF );
      State[n]->Calc_Mw();
      pF->To_kg = State[n]->Mw_summ;
      Flow_in += F_mol;
      }
    if ( F_mol < 0. )
      {
      // Выход
      Flow_out -= F_mol;
      }
    }
///////////////////////////////////////////////////
  // Массы и энтальпии
  double H_summ_in_gas = 0.;
  double H_summ_in_liq = 0.;
  double dH_in = 0.;
  //=========================================================
  double dF_Gas_in = 0.;
  double dF_Liq_in = 0.;
  double dF_Liq_in_kg = 0.;
  double dF_Gas_out = 0.;
  double dF_Liq_out = 0.;
  double Summ_FS_Gas[K_GAS];
  double Summ_FS_Liq[K_GAS];
  CLEAR(Summ_FS_Gas)
  CLEAR(Summ_FS_Liq)
  double F_Summ = 0.;
  CalcEps( );
  Mw_gas = Mw( IsComp, Y );
  Mw_liq = Mw( IsComp, X );
  M3_liq = To_m3_liq( T_liq, IsComp, X );
  _M_liq = M_Liq * Mw_liq;
  bool IsBypass = false;
  double F_bypass = 0.;
  if ( Eps_Sep < 100. && pFlow[0]->Flow_mol > 0 && pFlow[1]->Flow_mol < 0 )
    {
    IsBypass = true;
    double F_in = pFlow[0]->Flow_mol;
    double F_out = -pFlow[1]->Flow_mol;
    F_bypass = 0.01 * ( 100. - Eps_Sep ) * F_in; 
    if ( F_bypass > F_out )
       F_bypass = F_out; 
    }
  CComp * pComp_0 =  pFlow[0]->Get_Comp();
  double h_0 = pFlow[0]->h_Flow;
  for ( int n = 0; n < 3; n++ )
    {
    CFlow * pF = pFlow[n];
    double F_mol = pF ->Flow_mol;
    if ( F_mol > 0. )
      {
      // Вход
      CSEps * pState = State[n];
      if ( IsBypass && n == 0 )
        {
        Eps[n] = pState->E * ( 1. - F_bypass / F_mol );
        F_mol -= F_bypass;
        }
      else
        Eps[n] = pState->E;
      F_Summ += F_mol;
      if ( pState->E > 0. )
        {
        double F = F_mol * pState->E;
        dF_Gas_in += F;
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( pState->IsComp[n] )
            {
            IsComp[n] = true;
            double FS = F * pState->Y_W[n];
            Summ_FS_Gas[n] += FS;
            H_summ_in_gas += FS * ( pState->h_Gas[n] - h_Gas[n] );
            }
          }
        }
      if ( pState->E < 1. )
        {
        double F = F_mol * ( 1. - pState->E );
        dF_Liq_in += F;
        FINITE(F)
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( pState->IsComp[n] )
            {
            IsComp[n] = true;
            double FS = F * pState->X_W[n];
            Summ_FS_Liq[n] += FS;
            H_summ_in_liq += FS * ( pState->h_Liq[n] - h_Liq[n] );
            dF_Liq_in_kg += FS * pGas[n]->Mw;
            }
          }
        }
      }
///
    if ( F_mol < 0. )
      {
      // Выход
      double F_out = -F_mol;
      pF -> ID_COMP = ID_Comp_Out[n];
      CComp * pComp = pComp_Out[n];
      pF -> pComp = pComp;
      pF -> T = T_liq;
      double Eg = Eps_Gas[n];
      double El = 1. - Eg;
      //if ( OverFlow )
      //  {
      //  Eg = 0.;
      //  El = 1.;
      //  }
      bool UseByPass = false;
      if ( IsBypass && n == 1 && F_out > 0.1 )
        {
        UseByPass = true;
        double F_b = F_out - F_bypass;
        Eps[n] = Eg * ( 1. - F_bypass / F_out );
        F_mol = -F_b;
        }
      else
        Eps[n] = Eg;
      if ( Eg >= 1. )
        {
        dF_Gas_out -= F_mol;
        FINITE(dF_Gas_out)
//        Flow_Gas_mol[n] = F_mol;
        pComp->Set ( IsComp, Y );
        if ( UseByPass )
          {
          SummComp ( -F_mol, pComp, F_bypass, pComp_0 );
          pF -> h_Flow = ( -F_mol * h_gas_mix + F_bypass * h_0 ) / F_out;
          }
        else
          pF -> h_Flow = h_gas_mix;
        pF -> To_kg = Mw_gas;
        pF -> To_m3 = M3;
        pF -> T = T_gas;
//        Flow_kg[n] = F_mol * pF -> To_kg;
       continue;
        }
      if ( El >= 1. )
        {
        dF_Liq_out -= F_mol;
        FINITE(dF_Liq_out)
        pComp->Set ( IsComp, X );
        if ( UseByPass )
          {
          SummComp ( F_out, pComp, F_bypass, pComp_0 );
          pF -> h_Flow = ( -F_mol * h_liq_mix + F_bypass * h_0 ) / F_out;
          }
        else
          pF -> h_Flow = h_liq_mix;
        pF -> T = T_liq;
        pF -> To_kg = Mw_liq;
        pF -> To_m3 = M3_liq;
//        Flow_kg[n] = F_mol * pF -> To_kg;
        continue;
        }
      dF_Liq_out -= F_mol * El;
      dF_Gas_out -= F_mol * Eg;
      FINITE(dF_Liq_out)
//      Flow_Gas_mol[n] = F_mol * Eg;
      double Cmol_mix[K_GAS];
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( IsComp[c] )
          Cmol_mix[c] = Eg * Y[c] + El * X[c];
        else
          Cmol_mix[c] = 0.;
        }
      pComp->Set ( IsComp, Cmol_mix );
      double h_mix = Eg * h_gas_mix + El * h_liq_mix;
      if ( UseByPass )
        {
        SummComp ( F_out, pComp, F_bypass, pComp_0 );
        pF -> h_Flow = ( -F_mol * h_mix + F_bypass * h_0 ) / F_out;
        }
      else
        pF -> h_Flow = h_mix;
      pF -> T = T_liq;
      pF -> To_kg = Eg * Mw_gas + El * Mw_liq;
      pF -> To_m3 = El * To_m3_liq ( T_liq, pComp ) + Eg * M3;
//      Flow_kg[n] = F_mol * pF -> To_kg;
      continue;
      }
    }
///////////////////////////////////////////////////////////////////
//  double dF_gas = 0.;
//  double dF_liq = 0.;
//  for ( int c = 0; c < LAST_GAS; c++ )
//    {
//    if ( IsComp[c] )
//      {
//      dF_gas += Summ_FS_Gas[c];
//      dF_liq += Summ_FS_Liq[c];
//      }
//    }
//  double Tst_gas = dF_gas - dF_Gas_in; // dF_Gas_out не входит в Summ_FS_Gas
//  double Tst_liq = dF_liq - dF_Liq_in; // dF_Liq_out не входит в Summ_FS_Liq
///////////////////////////////////////////////////////////////////////
  const double D_max = 0.05;
  double dM_gas = dt * ( dF_Gas_in - dF_Gas_out );
  double dM_gas_max = M_Gas * D_max;
  if ( dM_gas > dM_gas_max && M_Gas > 0.1 )
    {
    double K = dM_gas_max / dM_gas; ;
    if ( K < 0.1 )
      K = 0.1;
    dt *= K; 
    dM_gas *= K;
    }
  if ( dM_gas < -dM_gas_max && M_Gas > 0.1 )
    {
    double K = -dM_gas_max / dM_gas;
    if ( K < 0.1 )
      K = 0.1;
    dt *= K; 
    dM_gas *= K;
    }
  double dM_liq = dt * ( dF_Liq_in - dF_Liq_out );
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
//////////////////////////////////////////////////
  // Новые составы
  double M_Gas_new = M_Gas + dt * dF_Gas_in;
  //
  if ( M_Gas_new > 0. )
    {
    double N_Gas = 1. / M_Gas_new;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        double MC = M_Gas * Y[c];
        double MC_new = MC + dt * Summ_FS_Gas[c];
        if ( MC_new < 0. )
          MC_new = 0.;
        Y[c] = MC_new * N_Gas;
        }
      }
    }
//
  double M_Liq_new = M_Liq + dt * dF_Liq_in;
  if ( M_Liq_new > 0. )
    {
    double N_Liq = 1. / M_Liq_new;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        double MC = M_Liq * X[c];
        double MC_new = MC + dt * Summ_FS_Liq[c];
        X[c] = MC_new * N_Liq;
        }
      }
    }
////////////////////////////////////////////////////////
  M_Gas += dM_gas;
  if ( M_Gas < 0.0001 )
    M_Gas = 0.0001;
  M_Liq += dM_liq;
  if ( M_Liq < 0. )
    M_Liq = 0.;
  FINITE(M_Gas)
  FINITE(M_Liq)
/////////
  if ( Add_Comp )
    {
	  CComp CompAdd;
    if ( CompAdd.Read ( Add_Comp_Name ))
      {
      double M_add = Add_Comp / Mw ( &CompAdd );
      CSEps State ( Add_Comp_Name );
      State.PT ( P, Add_Comp_T, &CompAdd );
	    State.Calc_Mw();
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( CompAdd.IsComp[c] )
          IsComp[c] = true;
        }
	    State.SetCompData( NULL, X, NULL, h_Liq, h_Gas );
      h_liq_mix = State.h_liq;
      M_Liq = M_add;
      T_gas = T_liq = Add_Comp_T;
      if ( T_liq < T_min )
        T_liq = T_min;
      if ( T_liq > T_max )
        T_liq = T_max;
      H_summ_in_liq = 0.;
      H_summ_in_gas = 0.;
      }
    Add_Comp = 0.;
    }
//
///////////////////////////////////////////////////////////////////
  Heat.Lev = Level;
  Heat.T_ext = T_liq;                                                                                                                                                         
  Heat.Lev_Zm = H_heat;
  Heat.KS = K_heat * S_heat * 3.6;
  Heat.Calc( dt );
  Q_heat = Heat.Q_Heat;
  double C_l = Cp_liq * M_Liq + Cp_сorp;
  double C_g = Cp_gas * M_Gas;
  double Kg = K_GL * dt / ( C_g + K_GL * dt );
//
  if ( Reg_T_liq.IsOn() )
    T_liq = Reg_T_liq;
  else
    {
    T_liq = ( C_l * T_liq + ( Q_heat + H_summ_in_liq ) * dt + Kg * ( C_g * T_gas + dt * H_summ_in_gas ))/
          ( C_l + K_GL * dt * ( 1. - Kg ));
    }
  if ( T_liq < T_min )
    T_liq = T_min;
  if ( T_liq > T_max )
    T_liq = T_max;
  if ( !_finite ( T_liq ))
    T_liq = 15.;
  if ( Reg_T_gas.IsOn() )
    T_gas = Reg_T_gas;
  else
    {
    T_gas = ( C_g * T_gas + dt * ( H_summ_in_gas + K_GL * T_liq ))/
          ( C_g + K_GL * dt );
    }
  if ( T_gas < T_min )
    T_gas = T_min;
  if ( T_gas > T_max )
    T_gas = T_max;
  if ( !_finite ( T_gas ))
    T_gas = 15.;
  _T_gas = T_gas;
  _T_liq = T_liq;
  // Конденсация и испарение
  // P * V = M * R * T
  if ( M_Liq > M_liq_min )
    {
    double Tabs = T_liq + TK;
    double Ksi = Volume / ( RP * Tabs );
//    P = M_Gas / Ksi;
    M_Gas = P * Ksi;
    FINITE(M_Gas)
    double R = 0;
    double Pz = 0.;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        if ( NoInLiq_N2 && c == G_N2 )
          {
          double dM = X[c] * M_Liq;
          Y[c] = M_Gas * Y[c] + dM;
          X[c] = 0.;
          continue;
          }
        double Ps;
        Ps = Henry[c] * pGas[c]->Pz( T_liq );
        if ( Tabs > pGas[c]->Tcr && Ps < 1.5 * P )
          Ps = 1.5 * P;
        P_sat[c] = Ps;
        Pz += Ps * X[c];
        FINITE(Ps)
          //
        double dM = Ksi * M_Gas * M_Liq * ( P * Y[c] - Ps * X[c] ) /
          ( M_Gas * M_Liq + Ksi * ( M_Liq * P + M_Gas * Ps ));
        double Vc = dM * kVC / dt;
        dM = Vc * dt;
        V_cond[c] = Vc;
        R += Vc * pGas[c]->H_vap( T_liq );
        Y[c] = M_Gas * Y[c] - dM;
        if ( Y[c] < 0. )
          Y[c] = 0.;
        X[c] = M_Liq * X[c] + dM;
        if ( X[c] < 0. )
          X[c] = 0.;
        dF_Liq_in_kg += dM * pGas[c]->Mw / dt;        
        }
      }
    M_Gas = Norm ( IsComp, Y );
    M_Liq = Norm ( IsComp, X );
    P_Pz = P / Pz;
//??    P = Pz;
    if ( M_Gas < 0.001 )
      M_Gas = 0.001;
    double Tst = P * Ksi / M_Gas;
    M_Gas = P * Ksi;
    //    Norm ( IsComp, Y );
    //    Norm ( IsComp, X );
    Mw_liq = ::Mw ( IsComp, X );
    Mw_gas = ::Mw ( IsComp, Y );
    if ( !Reg_T_liq.IsOn())
      {
      double dT = R * dt / ( Cp_liq * M_Liq + Cp_сorp );
      T_liq += dT;
      if ( T_liq < T_min )
        T_liq = T_min;
      if ( T_liq > T_max )
        T_liq = T_max;
      FINITE(T_liq)
      }
    }
  else
    {
    double Ksi = Volume / ( RP * ( T_gas + TK ));
    P = M_Gas / Ksi;
    FINITE(M_Gas)
    }
  if ( P > P_max + 1. )
    {
    if ( pSys->IsBlk )
      Разгерметизация = true;
    }
 _F_liq = dF_Liq_in_kg; 
  if ( M_Liq > M_liq_min )
    {
    double M0 = _M_liq;
    _M_liq = Mw_liq * M_Liq;
    _dM_liq = ( _M_liq - M0 ) / dt;
    }
  else
    {
    _M_liq = 0.;
    _dM_liq = 0.;
    }
  h_gas_mix = 0.;
  h_liq_mix = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      h_Gas[c] = pGas[c]->H_IG( T_gas );
      h_gas_mix += Y[c] * h_Gas[c];
      h_Liq[c] = pGas[c]->H_l( T_liq );
      h_liq_mix += X[c] * h_Liq[c];
      }
    }
/////////////////////////////////////////////////////////////////
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
    if ( Sx > 1e-9 )
      Sx = 100. / Sx;
    else
      Sx = 0.;
    if ( Sy > 1e-9 )
      Sy = 100. / Sy;
    else
      Sy = 0.;
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

void CSepLiq::CalcEps( )
  {
const double d = 0.02;
  if ( Level < 1 - d )
    {
    Eps_Gas[0] = 1.;
    Eps_Gas[1] = 1.;
    }
  else
    {
    double E = ( 1. - Level ) / d;
    if ( E < 0. )
      E = 0.;
    Eps_Gas[0] = E;
    Eps_Gas[1] = E;
    }
  if ( Level > d )
    Eps_Gas[2] = 0.;
  else
    {
    Eps_Gas[2] = 1. - Level / d;
    }
  }


void CSepLiq::CalcLiq( double dt )
  {
  SET_BP BreakPoint;
  Flow_out_liq = pFlow[2]->Flow_mol;
  if ( kStep % 5 == 0 && M_Liq > M_liq_min )
    {
    SET_BP BreakPoint;
    Mw_liq = ::Mw ( IsComp, X );
    To_m3 = To_m3_liq ( T_liq, IsComp, X );
    if ( To_m3 * 1000. > Mw_liq )
      Ro_Liq = Mw_liq / To_m3;
    //    Get_RoMw = false;
    }
  else
    {
    if ( M_Liq <= M_liq_min )
      Ro_Liq = 0.;
    }
  FINITE(M_Liq);
  if(To_m3 == 0.0)
	To_m3 = 0.018;
  double To_L = To_m3 / Volume;
  double To_M = 1. / To_L;
  Level = M_Liq * To_L;
  _Level = Level * 100.;
  if ( Level > Lev_max )
    {
    Level = Lev_max;
    M_Liq = Level * To_M;
    }
  if ( Reg_L.IsOn() )
    Level_Z = Reg_L;
  else
    Level_Z = 0.;
  if ( VolumeZ != Volume )
    {
    double K = VolumeZ / Volume;
    M_Gas *= K;
    Volume = VolumeZ;
    if ( Level > 0.01 )
      Level_Z = Level * 100.;
    }
  if ( Level_Z > 0. && Level >= 0.01 )
    {
//    double L1 = Lev_0_100( Level, Lev_1_0, Lev_1_100 );
// L1 = ( Level - Lev_1_0 ) / ( Lev_1_100 - Lev_1_0 )
//    Уровень = L1 * Масштаб;   
    double LZ_rel = Level_Z / Масштаб;
    double LZ_abs = Lev_1_0 + LZ_rel * ( Lev_1_100 - Lev_1_0 );
    Level = LZ_abs;
    M_Liq = Level * To_M;
    Level_Z = 0.;
    Reg_L.Off();
    }
  if ( Level_Z < 0. && Level >= 0.01 )
    {
    double LZ_rel = -Level_Z / Масштаб;
    double LZ_abs = Lev_1_0 + LZ_rel * ( Lev_1_100 - Lev_1_0 );
    Level = LZ_abs;
    M_Liq = Level * To_M;
    }
  _Level = Level * 100.;
  }


void CSepLiq::Razgerm( )
  {
  _P = 0.;
  _T_gas = T_air;
  _T_liq = T_air;
  _M_liq = 0.;
  M_Liq = 0.;
  _Level = 0.;
  _F_liq = 0.;
  _dM_liq = 0.;
  P_Pz = 0.;
  }

void CSepLiq::SummComp ( double F, CComp * pComp, double F_add, CComp * pComp_add )
  {
  double * Cmol = pComp->Cmol;
  double * Cmol_add = pComp_add->Cmol;
  double N = 1. / ( F + F_add );
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      Cmol[c] = ( F * Cmol[c] + F_add * Cmol_add[c] ) * N;
      }
    }
  }
