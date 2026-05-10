#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "Extensions.h"

//#define BREAK __asm __emit 0xF1;

// P_z включает Gamma, Henry ....
double CStage_In::Summ_In[K_GAS];
int CStage_NE::HowShowParams = 0;
//double CStage_In::Summ_In_Gas[K_GAS];
//double CStage_In::Summ_In_Liq[K_GAS];

//double CStage_P::SlowCond = 1.;


void CStage_NE::CalcLevel( double dt )
{
  if ( StopLiq )
  {
    BREAK
      if ( StopLiq > 0 )
      StopLiq--;
    KKK();
  }
  // Поступление
  //  if ( pCol->Static_Calc_Mix )
  //    {
  //// Поступление - Lout - от разделения потоков на Lout и Vout
  //// Потом Lout уточнится
  //    double dM = Lout * dt;
  //    M_liq += dM;
  //    }
  //  else
  //    {
  //    KKK();
  //    }
  //
  L_Karman = 0.;
  for ( int o = 0; o < kSrc; o++ )
  {
    if ( Src[o]->Gas )
      continue;
    if ( Src[o]->pFlow == NULL )
      continue;
    /////
    double dM = 0.;
    if ( Src[o]->pKarman )
    {
      L_Karman += Src[o]->pKarman->Flow_In ( Flow_Liq_Stage, dt );
      //      Src[o]->pKarman->To_m3 = To_m3_liq( Src[o]->Src.T, 	Src[o]->Src_IsComp, Src[o]->Src_CmolLiq );
      //     Src[o]->pKarman->Sliv( Src[o], dt );
    }
    else
    {
      CFlow * F_Liq_out = Src[o]->pFlow;
      double F = F_Liq_out->Flow_mol;
      if ( F < 0. )
        L_Karman += -F;
      L_Down = 0.;
    }
  }
  if ( Razgerm_Liq )
  {
    double F = Omega_Razgerm * ( P - 1. );
    double dM = F * dt;
    if ( dM > 0.2 * M_liq )
      dM = 0.2 * M_liq;
    M_liq -= dM;
  }
  if ( !_finite (M_liq))
    M_liq = 0.;
  ////////////////////////////////////////////////
  double Lev_old = Level;
  // Слив
  dLdM = 0.;
  ///////////////////////////////////////////////////
  if ( h_st > 0.  && StageType != Насадка )
  {
    double h_st_e = h_st / pCol->k_h_air;
    if ( To_m3 <= 0. || *pCalcTom3 )
    {
      if ( M_liq > 0.1 )
        To_m3 = To_m3_liq ( T_liq, IsComp, X );
      else
        To_m3 = 0.2;
    }
    if ( To_m3 <= 0. )
      To_m3 = 0.2;
    double kH =  To_m3 / S_Stage;
    double h_liq = M_liq * kH;
    Level = h_liq / h_st_e;
    if ( ЭтоКуб )
      Alfa_Level = 0.1;
    else
    {
      if ( pCol->Cond_Level )
        Alfa_Level = Level;
      else
        Alfa_Level = 1.;
    }
    dLdM = kH / h_st_e;
    double M0 = h_st_e / kH;
    double Mo = M_liq;
    dh_hydro = h_liq * 0.1;
    double kSliv = 0;
    if ( StageType == Ситчатая )
    {
      if ( Vin * M3 < FlowStopSlivm3 )
      {
        double A = Vin * M3 / FlowStopSlivm3;
        kSliv += ( 1. - A ) * kExtLout;
      }
    }
    if ( !pCol->Work )
      kSliv += 60. / pCol->Tau_sliv;
    if ( !ЭтоКуб )//&& StageType != Глухая )
    {
      if ( h_liq > h_st_e )
      {
        double k_L_eff = k_L / ( 1. + k_LV * Vout * 1e-3 ); 
        //        double M_liq_new = ( M_liq + dt * k_L_eff * M0 ) / ( 1. + dt * ( k_L_eff + kSliv ));
        //        FINITE(M_liq_new)
        //        double dM = M_liq - M_liq_new;
        //M_liq = M_liq_new;
        //        M_liq = M_liq_new;
        double ExtLout = kSliv * M_liq;
        L_Down = k_L_eff * ( M_liq - M0 ) + ExtLout;
        if ( StopLiq )
          KKK();
      }
      else
      {
        L_Down = kSliv * M_liq; 
      }
    }
    ///////////////////////////////////////////////
    //    if ( !ЭтоКуб )
    L_Summ_out = L_Down + L_Karman;
    //  double Mn = M_liq;
    //  double dM = Mn - Mo;
    //  double Lo = Level * 100;
    //h_liq = M_liq * kH;
    //Level = h_liq / h_st;
    //  if ( dLev_dt_max > 0. && Lev_old > 0. )
    //    {
    //    double dL = ( Level - Lev_old ) * 100. / dt;
    //    bool Recalc = false;
    //    if ( dL > dLev_dt_max )
    //      {
    //      Recalc = true;
    //      dL = dLev_dt_max;
    //      }
    //    if ( dL < -dLev_dt_max )
    //      {
    //      Recalc = true;
    //      dL = -dLev_dt_max;
    //      }
    //    Sliv = dL * 0.01 * h_st / kH;
    //    if ( Recalc )
    //      {
    //      Level = Lev_old + 0.01 * dL * dt;
    //      h_liq = Level * h_st;
    //      double M_liq_new = h_liq / kH;
    //      Doliv = ( M_liq_new - M_liq ) / dt;
    //      M_liq = M_liq_new;
    //      }
    //    }
    /////////////
    double Ln = Level * 100;
    if ( Lev_Z > 0. )
    {
      Level = Lev_Z * 0.01;
      h_liq = Level * h_st;
      double M_liq_new = h_liq / kH;
      Doliv = ( M_liq_new - M_liq ) / dt;
      M_liq = M_liq_new;
      Lev_Z = 0.;
    }
    if ( Lev_Z < 0. )
    {
      Level = -Lev_Z * 0.01;
      h_liq = Level * h_st;
      double M_liq_new = h_liq / kH;
      Doliv = ( M_liq_new - M_liq ) / dt;
      M_liq = M_liq_new;
    }
    if ( Level < Lev_min )
    {
      pCol->Проблемы |= PROB_LOW_LEV;
      Level = Lev_min;
      h_liq = Level * h_st;
      double M_liq_new = h_liq / kH;
      Doliv = ( M_liq_new - M_liq ) / dt;
      M_liq = M_liq_new;
    }
    else
    {
      if ( Level > Lev_max )
      {
        pCol->Проблемы |= PROB_HI_LEV;
        Level = Lev_max;
        h_liq = Level * h_st;
        double M_liq_new = h_liq / kH;
        Doliv = ( M_liq_new - M_liq ) / dt;
        M_liq = M_liq_new;
      }
      else
      {
        if ( Lev_Opt > 0. )
        {
          double dL = Lev_Opt * 0.01 - Level;
          double K = 0.;
          if ( dL > 0. && dL < 0.1 )
          {
            if ( dL < 0.05 )
              K = 20. * dL;
            else
              K = 20. * ( 0.1 - dL );
          }
          if ( dL < 0. && dL > -0.1 )
          {
            if ( dL > -0.05 )
              K = 20. * dL;
            else
              K = 20. * ( -0.1 - dL );
          }
          double To_kg = ::Mw ( IsComp, X );
          double Doliv = K * Max_Flow_Add / To_kg;
          M_liq += dt * Doliv;
        }
      }
    }
  }
  else
  {
    double K = 1. / ( 1. + dt * k_L );
    //		double M_liq_new = M_liq * K;
    L_Down = k_L * M_liq * pCol->k_l_air;
    Level = 0.;
    L_Summ_out = L_Down + L_Karman;
  }
  if ( M_liq < 1e-4 )
    M_liq = 0.;
  if ( !_finite(M_liq))
    M_liq = 0.;
  FINITE(M_liq)
  if ( pCol->Cond_Level )
    Alfa_Level = Level;
  else
    Alfa_Level = 1.;
  //if ( pRegLev )
  //  {
  // double M_liq_dt = M_liq;
  // double To_kg = ::Mw ( IsComp, X );
  // double To_m3 = To_m3_liq ( T_liq, IsComp, X );
  // pRegLev->Control ( dt, Level, ( M_liq_dt - M_liq_0 ) / dt, dLdM, To_kg, To_m3 );
  //  }
}


void CStage_NE::Calc_dP( double dt )
{
  if ( dP_nom == 0. )
  {
    dP_Stage = 0.;
    return;
  }
  double V_In_rel = 0.;
  if ( pDown )
  {
    V_In_rel = LimdX ( fabs ( pDown -> Vout ) * M3 / V_nom, 5. );
  }
  double dP_S = Level * h_st * 0.02;
  double dP_V = Level * dP_nom * V_In_rel * V_In_rel;
  double dP = dP_V + dP_S;
  dP_Stage += 0.1 * ( dP - dP_Stage );
}

