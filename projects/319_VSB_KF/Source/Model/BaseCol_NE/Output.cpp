#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"


void CStage_NE::CalcLiq( double dt )
{
  if ( ЭтоКуб )
    KKK();
  // Давление на выводах
  for ( int o = 0; o < kSrc; o++ )
  {
    if ( Src[o]->pFlow == NULL )
      continue;
    if ( Src[o]->Gas )
    {
      *(Src[o]->pPress ) = P;
      continue;
    }
    if ( Src[o]->pKarman )
    {
      Src[o]->pKarman->SetPress( Src[o] );
    }
    else
    {
      if ( ЭтоКуб )
      {
        CExtIO	* pSrc = Src[o];
        if ( pCol->dP_Hydro_Level_Cub != 0. || pCol->dP_Hydro_Static_Cub != 0. )
        {
          double Eliq;
          if ( Level > 0.1 )
            Eliq = 1.;
          else
            Eliq = 10. * Level;
          double dP_h;
          if ( Level > 0. )
            dP_h = Level * pSrc->dP_Hydro_Level;
          else
            dP_h = 0.;
          *(pSrc->pPress) = ( dP_h + pSrc->dP_Hydro_Static ) * Eliq + P;
        }
        else
          *(pSrc->pPress) = P;
      }
      else
        *(Src[o]->pPress ) = P;
    }
  }
  ///////////////////////////////////////////////
  FINITE(M_liq)
  for ( int o = 0; o < kSrc; o++ )
  {
    CExtIO * pSrc = Src[o];
    if ( pSrc->Gas )
      continue;
    CFlow * F_Liq_out = pSrc->pFlow;
    if ( F_Liq_out == NULL )
      continue;
    double F = F_Liq_out->Flow_mol;
    if ( F >= 0. )
      continue;
    // Вывод
    pSrc->Src.T = T_liq;
    //
    double * MixOut;
    double Mix[K_GAS];
    if ( pSrc->pKarman )
    {
      pSrc->Src.PT( pSrc->pFlow->P, T_liq, IsComp, X );
      pSrc->pKarman->OutLiq( pSrc, IsComp, X, Y, FlowGasOut, &MixOut );
    }
    else
    {
      CFlow * F_Liq_out = pSrc->pFlow;
      F_Liq_out->ID_COMP = pSrc->ID_Comp;
      CComp * pComp = pSrc->pComp;
      F_Liq_out->pComp = pComp;
      //
      double Eliq;
      if ( Level > 0.1 )
        Eliq = 1.;
      else
        Eliq = 10. * Level;
      double Egas = 1. - Eliq;
      pSrc -> Eps = Egas;
      double T_liq = pSrc->Src.T;
      if ( Eliq == 1. )
      {
        pComp->Set ( IsComp, X );
        F_Liq_out -> T = T_liq;
        F_Liq_out -> To_kg = ::Mw ( pComp );
        F_Liq_out -> To_m3 = To_m3_liq ( T_liq, pComp );
        F_Liq_out -> h_Flow = ::H_Liq ( T_liq, IsComp, X );
        MixOut = X;
        continue;
      }
      CLEAR( Mix );
      MixOut = Mix;
      if ( Eliq > 0. )
      {
        for ( int n = 0; n < LAST_GAS; n++ )
        {
          if ( IsComp[n] )
          {
            Mix[n] = Eliq * X[n];
          }
        }
      }
      FlowGasOut += -Egas * F_Liq_out->Flow_mol;
      if ( Egas > 0. )
      {
        for ( int n = 0; n < LAST_GAS; n++ )
        {
          if ( IsComp[n] )
            Mix[n] += Egas * Y[n];
        }
      }
      pComp->Set ( IsComp, Mix );
      Norm ( pComp );
      if ( Egas == 0. )
        F_Liq_out -> h_Flow = H_Liq ( T_liq, pComp ); 
      else
        F_Liq_out -> h_Flow = Eliq * H_Liq ( T_liq, IsComp, X ) + Egas * H_IG ( T_liq, IsComp, Y ); 
      FINITE(F_Liq_out -> h_Flow)
      F_Liq_out -> T = T_liq;
      F_Liq_out -> To_kg = ::Mw ( pComp );
      F_Liq_out -> To_m3 = To_m3_liq ( T_liq, pComp );
    }
    if ( pCol->CalcStageBalance )
    {
      if ( pCol->n_Comp_Balance >= 0 )
      {
        SummStageBalance += F * MixOut[pCol->n_Comp_Balance];
      }
      else
      {
        SummStageBalance += F;
      }
    }
  }
}

void CStage_NE::CalcGas( double dt )
{
  if ( StopGas )
  {
    BREAK
    if ( StopGas > 0 )
      StopGas--;
  }
  FlowGasOut = 0.;
  //  Eps_In.PH( P, h_gas_mix, Y, T_In );
  for ( int o = 0; o < kSrc; o++ )
  {
    if ( !Src[o]->Gas )
      continue;
    *(Src[o]->pPress) = P;                            
    CFlow * F_Gas_out = Src[o]->pFlow;
    if ( F_Gas_out == NULL )
      continue;
    double F = F_Gas_out->Flow_mol;
    if ( F > 0. )
      continue;
    double Max_Gas_Flow = Vin - V_Cond;
    if ( -F > Max_Gas_Flow )
      F = -Max_Gas_Flow;
    FlowGasOut -= F;
    Src[o]->SetFlowOut = true;
    Src[o]->Eps = 1.;
    Src[o]->Src.T = T_gas;
    Src[o]->Max_Gas_Flow = Max_Gas_Flow;
    CComp * pCompOut = Src[o]->pComp;//
    pCompOut ->Set ( IsComp, Y );
    F_Gas_out -> T = T_gas;
    if ( pCol->OutPureComp )
    {
      double T_out = T_gas;
      if ( T_liq > T_out )
        T_out = T_liq;
      F_Gas_out -> h_Flow = H_IG ( T_out + 0.01, pCompOut );
    }
    else
      F_Gas_out -> h_Flow = h_gas_mix;
    FINITE(F_Gas_out -> h_Flow)
    //    F_Gas_out -> h_Flow = H_IG( T_gas, pCompOut );
    F_Gas_out -> To_kg = ::Mw ( pCompOut );
    F_Gas_out->pComp = pCompOut;
    F_Gas_out->ID_COMP = Src[o]->ID_Comp;
    if ( pCol->CalcStageBalance )
    {
      if ( pCol->n_Comp_Balance >= 0 )
        SummStageBalance += F * Y[pCol->n_Comp_Balance];
      else
        SummStageBalance += F;
    }
  }
  if ( Razgerm_Gas )
  {
    double F = Omega_Razgerm * ( P - 1. );
    Vout -= F;
    if ( Vout < 0. )
      Vout = 0.;
  }
}
