#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 


void CVolBase::DefectFlow( )
  {
  if ( Defect != 1 && Defect != 2 )
    return;
  if ( P <= 1. )
    return;
  double F_mol = Def_Omega_top * ( P - 1. );
  if ( Is_2F )
    {
    // газ + 2 фазы жидкости
    if ( Defect == 1 )
      CalcEps_0( 0, 1. );
    else
      CalcEps_0( 0, 0. );
    double E_0 = Eps[0];
    double E_1 = Eps[1];
    double E_2 = Eps[2];
    if ( E_0 >= 1. )
      {
      S.dF_Gas_out -= F_mol;
      return;
      }
    if ( E_1 >= 1. )
      {
      S.dF_Liq_out_1 -= F_mol;
      FINITE(S.dF_Liq_out_1)
      return;
      }
    if ( E_2 >= 1. )
      {
      S.dF_Liq_out_2 -= F_mol;
      return;
      }
    // Смеси
    double F_liq_1 = F_mol * E_1;
    double F_liq_2 = F_mol * E_2;
    double F_gas = F_mol * E_0;
    S.dF_Liq_out_1 -= F_liq_1;
    S.dF_Liq_out_2 -= F_liq_2;
    S.dF_Gas_out -= F_gas;
    FINITE(S.dF_Liq_out_1)
    FINITE(S.dF_Liq_out_2)
    return;
    }
  else
    {
    // газ + жидкость
    if ( Defect == 1 )
      CalcEps_1F( 0, 1. );
    else
      CalcEps_1F( 0, 0. );
    double Eg = Eps[0];
    double El = Eps[1];
    if ( OverFlow )
      {
      Eg = 0.;
      El = 1.;
      }
    if ( Eg >= 1. )
      {
      S.dF_Gas_out -= F_mol;
      FINITE(S.dF_Gas_out)
      return;
      }
    if ( El >= 1. )
      {
      S.dF_Liq_out_1 -= F_mol;
      FINITE(S.dF_Liq_out_1)
        return;
      }
    S.dF_Liq_out_1 -= F_mol * El;
    S.dF_Gas_out -= F_mol * Eg;
    return;
    }
  }

void CVolBase::SummFlow( double dt )
  {
  SET_BP_SUB
  Flow_in = 0.;
  Flow_out = 0.;
  int kFlow = k_Node;
  for ( int n = 0; n < kFlow; n++ )
    {
    CFlow * pF = pFlow[n];
    if ( pF == NULL )
      continue;
    double F_mol = pF ->Flow_mol;
    CComp * pComp = pF -> Get_Comp();
    IsFlow[n] = true;
    if ( F_mol > 0 && pComp == NULL )
      {
      IsFlow[n] = false;
      continue;
      }
    pF->To_kg = Mw( pComp );
    Flow_kg[n] = F_mol * pF->To_kg;
    if ( F_mol > 0. )
      {
      // Вход 
      IsFlow[n] = true;
      State_in[n]->T_min = T_min;
      State_in[n]->T_max = T_max;
      if ( n == nP_in )
        State_in[n]->	PH( P_in, pF->h_Flow, pF->Get_Comp(), 0. );
      else
        {
        CComp * pComp = pF->Get_Comp();
        double h = 0;
        for ( int c = 0; c < LAST_GAS; c++ )
          {
          if ( pComp->IsComp[c] )
            h += pComp->Cmol[c] * pGas[c]->H_l( pF->T );
          }
        double dh = h - pF->h_Flow;
        State_in[n]->PH ( pF );
        }
      State_in[n]->Calc_Mw();
      pF->To_kg = State_in[n]->Mw_summ;
      Flow_in += F_mol;
      }
    if ( F_mol < 0. )
      {
      // Выход
      Flow_out -= F_mol;
      IsFlow[n] = true;
      }
    }
  // Массы и энтальпии
  double h_mix = StateVol.dH_liq;
  //=========================================================
  S.Clear();
  F_Summ = 0.;
  Flow_in_liq_kg = 0.;
  //
  double h_liq_out = h_liq;
  double T_bott = T;
  if ( StateVol.UseTs )
    {
    int n_bott = StateVol.kTs - 1;
    T_bott = StateVol.Ts[n_bott];
    h_liq_out = H_Liq ( T_bott, IsComp, Cmol_Liq );
    }
  //
  Mw_gas = ::Mw( IsComp, Cmol_Gas );
  Mw_liq = ::Mw( IsComp, Cmol_Liq );
//  static CTraceControl Trc[100];
  //  int n_Trace = 0;
  for ( int n = 0; n < kFlow; n++ )
    {
    if ( !IsFlow[n] )
      continue;
    CFlow * pF = pFlow[n];
    double F_mol = pF ->Flow_mol;
    FINITE(F_mol)
    F_Summ += F_mol;
    if ( pAddComp && pAddComp->AddMass && n == kFlow -1 )
      {
      F_mol = pAddComp->Flow / dt;
      }
    if ( F_mol > 0. )
      {
      // Вход
      CSEps * pState = State_in[n];
      Flow_kg[n] = F_mol * pState->Mw_summ;
      if ( pState->E > 0. )
        {
        double F = F_mol * pState->E;
        S.dF_Gas_in += F;
        Flow_Gas_mol[n] = F;
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( pState->IsComp[n] )
            {
            IsComp[n] = true;
            double FS = F * pState->Y_W[n];
            S.Summ_FS_Gas[n] += FS;
            S.H_summ_in_gas_A[n] -= FS;
            S.H_summ_in_gas_B[n] += FS * pState->h_Gas[n];
            }
          }
        }
      if ( pState->E < 1. )
        {
        double F = F_mol * ( 1. - pState->E );
        Flow_in_liq_kg += F * pState->Mw_liq;
        S.dF_Liq_in += F;
        FINITE(F)
          //        TRACE_T(Trc[n_Trace++],"101-F",F, "S.dF_Liq_in-%s", pF->PipeName)
        S.dQ_mix += F * ( pState -> dH_liq - h_mix );
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( pState->IsComp[n] )
            {
            IsComp[n] = true;
            double FS = F * pState->X_W[n];
            S.Summ_FS_Liq[n] += FS;
            S.H_summ_in_liq_A[n] -= FS;
            S.H_summ_in_liq_B[n] += FS * pState->h_Liq[n];
//            ASS ( fabs ( S.H_summ_in_liq_A[n] ) < 1e10 )
//            ASS ( fabs ( S.H_summ_in_liq_B[n] ) < 1e10 )
            }
          }
        }
      }
    if ( F_mol < 0. )
      {
      // Выход
      pF -> ID_COMP = ID_Out[n];
      CComp * pComp = pComp_Out[n];
      pF -> pComp = pComp;
      pF -> T = T;
      State_in[n]->T = T;
      if ( Static_Fix_P )
        {
        if ( n == Static_Out_gas )
          {
          S.dF_Gas_out -= F_mol;
          FINITE(S.dF_Gas_out)
          Flow_Gas_mol[n] = F_mol;
          pComp->Set ( IsComp, Cmol_Gas );
          pF -> h_Flow = h_gas;
          pF -> To_kg = Mw_gas;
          pF -> To_m3 = M3;
          // Состав газа на выходе = составу газа в объеме
          Flow_kg[n] = F_mol * Mw_gas;
          continue;
          }
        }
      if ( Is_2F )
        {
        if ( Static_Fix_L )
          {
          if ( n == Static_Out_Liq_1 )
            {
            S.dF_Liq_out_1 -= F_mol;
            FINITE(S.dF_Liq_out_1)
            pComp->Set ( IsComp, Cmol_1F );
            pF -> h_Flow = H_Liq( T_bott, IsComp, Cmol_1F );
            pF -> T = T_bott;
            pF -> To_kg = Mw_1F = ::Mw( pComp );
            pF -> To_m3 = To_m3( T_bott, IsComp, Cmol_1F );
            Flow_kg[n] = F_mol * pF -> To_kg;
            continue;
            }
          if ( n == Static_Out_Liq_2 )
            {
            S.dF_Liq_out_2 -= F_mol;
            FINITE(S.dF_Liq_out_2)
            pComp->Set ( IsComp, Cmol_2F );
            pF -> h_Flow = H_Liq( T_bott, IsComp, Cmol_2F );
            pF -> T = T_bott;
            pF -> To_kg = Mw_2F = ::Mw( pComp );
            pF -> To_m3 = To_m3( T_bott, IsComp, Cmol_2F );
            Flow_kg[n] = F_mol * pF -> To_kg;
            continue;
            }
          }
        // газ + 2 фазы жидкости
        //???
        if ( Vol_Conn[n] && Vol2F > 1e-2 )
          CalcEps_1( Vol_Conn[n], H_node[n] );
        else
          CalcEps_0( Vol_Conn[n], H_node[n] );
        double E_0 = Eps[0];
        double E_1 = Eps[1];
        double E_2 = Eps[2];
        if ( E_0 >= 1. )
          {
          S.dF_Gas_out -= F_mol;
          FINITE(S.dF_Gas_out)
          Flow_Gas_mol[n] = F_mol;
          pComp->Set ( IsComp, Cmol_Gas );
          pF -> h_Flow = h_gas;
          pF -> To_kg = Mw_gas;
          pF -> To_m3 = M3;
          // Состав газа на выходе = составу газа в объеме
          Flow_kg[n] = F_mol * Mw_gas;
          continue;
          }
        if ( E_1 >= 1. )
          {
          S.dF_Liq_out_1 -= F_mol;
          FINITE(S.dF_Liq_out_1)
          pComp->Set ( IsComp, Cmol_1F );
          pF -> h_Flow = H_Liq( T_bott, IsComp, Cmol_1F );
          pF -> T = T_bott;
          pF -> To_kg = Mw_1F = ::Mw( pComp );
          pF -> To_m3 = To_m3( T_bott, IsComp, Cmol_1F );
          Flow_kg[n] = F_mol * pF -> To_kg;
          if ( !Static_Calc_Mix )
            {
            for ( int n = 0; n < LAST_GAS; n++ )
              {
              if ( IsComp[n] )
                {
                double FS = F_mol * Cmol_1F[n]; //
                S.Summ_FS_Liq[n] += FS;
                }
              }
            }
          continue;
          }
        if ( E_2 >= 1. )
          {
          S.dF_Liq_out_2 -= F_mol;
          FINITE(S.dF_Liq_out_2)
          pComp->Set ( IsComp, Cmol_2F );
          pF -> h_Flow = H_Liq( T_bott, IsComp, Cmol_2F );
          pF -> T = T_bott;
          pF -> To_kg = Mw_2F = ::Mw( pComp );
          pF -> To_m3 = To_m3( T_bott, IsComp, Cmol_2F );
          Flow_kg[n] = F_mol * pF -> To_kg;
          if ( !Static_Calc_Mix )
            {
            for ( int n = 0; n < LAST_GAS; n++ )
              {
              if ( IsComp[n] )
                {
                double FS = F_mol * Cmol_2F[n];
                S.Summ_FS_Liq[n] += FS;
                }
              }
            }
          continue;
          }
        // Смеси
        double F_liq_1 = F_mol * E_1;
        double F_liq_2 = F_mol * E_2;
        double F_gas = F_mol * E_0;
        S.dF_Liq_out_1 -= F_liq_1;
        S.dF_Liq_out_2 -= F_liq_2;
        S.dF_Gas_out -= F_gas;
        FINITE(S.dF_Liq_out_1)
        FINITE(S.dF_Liq_out_2)
        Flow_Gas_mol[n] = F_mol * E_0;
        double Cmol_mix[K_GAS];
        for ( int c = 0; c < LAST_GAS; c++ )
          {
          if ( IsComp[c] )
            Cmol_mix[c] = E_0 * Cmol_Gas[c] + E_1 * Cmol_1F[c] + E_2 * Cmol_2F[c];
          else
            Cmol_mix[c] = 0.;
          }
        pComp->Set ( IsComp, Cmol_mix );
        double h_0 = E_0 * h_gas;
        double h_12 = E_1 * H_Liq( T_bott, IsComp, Cmol_1F ) + E_2 * H_Liq( T_bott, IsComp, Cmol_2F );
        pF -> T = T_bott;
        pF -> h_Flow = h_0 + h_12;
        pF -> To_kg = E_0 * Mw_gas + E_1 * Mw_1F + E_2 * Mw_2F;
        pF -> To_m3 = E_1 * To_m3_liq ( T_bott, IsComp, Cmol_1F ) + E_2 * To_m3_liq ( T_bott, IsComp, Cmol_2F ) + E_0 * M3;
        Flow_kg[n] = F_mol * pF -> To_kg;
        if ( !Static_Calc_Mix )
          {
          for ( int n = 0; n < LAST_GAS; n++ )
            {
            if ( IsComp[n] )
              {
              double FS = F_liq_1 * Cmol_1F[n];
              S.Summ_FS_Liq[n] += FS;
              FS = F_liq_2 * Cmol_2F[n];
              S.Summ_FS_Liq[n] += FS;
              }
            }
          }
        continue;
        }
      else
        {
        // газ + жидкость
        CalcEps_1F( Vol_Conn[n], H_node[n] );
        double Eg = Eps[0];
        double El = Eps[1];
        if ( OverFlow )
          {
          Eg = 0.;
          El = 1.;
          }
        if ( Eg >= 1. )
          {
          S.dF_Gas_out -= F_mol;
          FINITE(S.dF_Gas_out)
          Flow_Gas_mol[n] = F_mol;
          pComp->Set ( IsComp, Cmol_Gas );
          pF -> h_Flow = h_gas;
          pF -> To_kg = Mw_gas;
          pF -> To_m3 = M3;
          Flow_kg[n] = F_mol * pF -> To_kg;
          continue;
          }
        if ( El >= 1. )
          {
          S.dF_Liq_out_1 -= F_mol;
          FINITE(S.dF_Liq_out_1)
          pComp->Set ( IsComp, Cmol_Liq );
          pF -> h_Flow = h_liq_out;
          pF -> T = T_bott;
          pF -> To_kg = Mw_liq;
          pF -> To_m3 = M3_liq;
          Flow_kg[n] = F_mol * pF -> To_kg;
          continue;
          }
        S.dF_Liq_out_1 -= F_mol * El;
        S.dF_Gas_out -= F_mol * Eg;
        FINITE(S.dF_Liq_out_1)
        Flow_Gas_mol[n] = F_mol * Eg;
        double Cmol_mix[K_GAS];
        for ( int c = 0; c < LAST_GAS; c++ )
          {
          if ( IsComp[c] )
            Cmol_mix[c] = Eg * Cmol_Gas[c] + El * Cmol_Liq[c];
          else
            Cmol_mix[c] = 0.;
          }
        pComp->Set ( IsComp, Cmol_mix );
        double h_mix = Eg * h_gas + El * h_liq_out;
        pF -> T = T_bott;
        pF -> h_Flow = h_mix;
        pF -> To_kg = Eg * Mw_gas + El * Mw_liq;
        pF -> To_m3 = El * To_m3_liq ( T, pComp ) + Eg * M3;
        Flow_kg[n] = F_mol * pF -> To_kg;
        continue;
        }
      }
    }
//???  DefectFlow( );
  if ( pAddComp && pAddComp->AddMass )
    {
    pAddComp->AddMass = false;
    }
  //if ( Tr )
  //	{
  //	fprintf ( Tr, "hs=%7.1lf\n", _Hs_in );
  //	fclose ( Tr );
  //	}
  if ( kToAtm > 0. )
    {
    double F_mol = kToAtm * ( Подпор + 1. - P );
    if ( F_mol > 0. )
      {
      // Вход
      double F = F_mol;
      S.dF_Gas_in += F;
      IsComp[G_N2] = true;
      S.Summ_FS_Gas[G_N2] += F;
      S.H_summ_in_gas_A[G_N2] -= F;
      S.H_summ_in_gas_B[G_N2] += F * pN2Flow->h_Flow;
      }
    else
      {
      // Выход
      S.dF_Gas_out -= F_mol;
      }
    }
  ////////////////////////////////////////////////////////////////////////
  //
  //
  ////////////////////////////////////////////////////////////////////////
  if ( PseudoWater )
    {
    Flow_Water_In = S.Summ_FS_Gas[G_H2O];
    S.Summ_FS_Gas[G_H2O] = 0.;
    Cmol_Gas[G_H2O] = 0.;
    Cmol_Liq[G_H2O] = 0.;
    }
  ////////////////////////////////////////////////////////////////////////
  SET_BP_SUB
  FINITE(M_Gas)
  FINITE(M_Liq)
/////////
  double dF_gas = 0.;
  double dF_liq = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      dF_gas += S.Summ_FS_Gas[c];
      dF_liq += S.Summ_FS_Liq[c];
      }
    }
  double Tst_gas = dF_gas - S.dF_Gas_in; // S.dF_Gas_out не входит в S.Summ_FS_Gas
  if ( Is_2F )
    {
    double Tst_liq = dF_liq - ( S.dF_Liq_in - S.dF_Liq_out_1 );// S.dF_Liq_out входит в S.Summ_FS_Liq
    KKK();
    }
  else
    {
    double Tst_liq = dF_liq - S.dF_Liq_in; // S.dF_Liq_out не входит в S.Summ_FS_Liq
    KKK();
    }
  }
