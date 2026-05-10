#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol.h"
#include "Chem.h"
#include "Water.h"

//#define BREAK __asm __emit 0xF1;

// P_z включает Gamma, Henry ....
//??CGas::eTypeComp Type[K_GAS];
double CStage_In::Summ_In[K_GAS];
int CStage::HowShowParams = 0;


void CStage::CalcH_Eps( )
	{
  if ( Eps_In.NH3Col )
    {
    ASS(G_NH3>=0)
    ASS(G_H2O>=0)
    double X_H2O = X[G_H2O];
    double X_NH3 = X[G_NH3];
    bool Calc_H2O_NH3 = true;
    double Sx = X_H2O + X_NH3;
    if ( Sx < 1e-4 )
      Calc_H2O_NH3 = false;
    double hls = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( !IsComp[n] )
        continue;
      double hl;
      hl = pGas[n]-> H_l ( T_liq );
      h_liq[n] = hl - pCol->H_ads[n];
      hls += X[n] * hl;
      }
    if ( !Calc_H2O_NH3 )
      {
      h_liq_mix = hls;
      return;
      }
    //double x = X_NH3 / Sx;
    //double Hl = CNH3_H2O_Appr::H_l_sat( x, P * Sx, T_liq );
    //double Hl_id = 
    //  ( 1. - x ) * h_liq[G_H2O] + x * h_liq[G_NH3];
    //double dH_liq = Sx * ( Hl - Hl_id );
    h_liq_mix = hls + dH_liq;
    return;
    }
  if ( Eps_In.WaterCol )
    {
    double hls = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( IsComp[n] )
        {
        if ( n == G_H2O )
          {
          if ( pSys->WaterIAPWS )
            h_liq[n] = Water.Hl( T_liq, P ) - pCol->H_ads[n];
          else
            h_liq[n] = pGas[n]-> H_l ( T_liq ) - pCol->H_ads[n];
          }
        else
          {
          if ( pGas[n]->AsWater )
            h_liq[n] = pGas[n]-> H_l ( T_liq ) - pCol->H_ads[n];
          else
            h_liq[n] = pGas[n]-> H_IG ( T_liq ) - pCol->H_ads[n];
          }
        hls += X[n] * h_liq[n];
        }
      }
    h_liq_mix = hls;
    return;
    }
	double hls = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
    {
		if ( !IsComp[n] )
			continue;
		double hl;
		hl = pGas[n]-> H_l ( T_liq ) - pCol->H_ads[n];
		h_liq[n] = hl;
		hls += X[n] * hl;
    FINITE(hl)
		}
	h_liq_mix = hls;
  FINITE(h_liq_mix)
	}

void CStage::CalcHeat_Eps( double dt )
	{
  if ( StopHeat )
    {
    BREAK
    if ( StopHeat > 0 )
      StopHeat--;
    }
  double dH_liq = H_SS + ExtHeat;
  if ( L_In > 0. )
    {
	  for ( int n = 0; n < LAST_GAS; n++ )
		  {
		  if ( IsComp[n] )
		    {
			  dH_liq += L_In * X_In[n] * ( h_Liq_In[n] - h_liq[n] );
			  }
      }
    }
  if ( V_Cond > 0. && Hvap_Stage != 1. )
    dH_liq += V_Cond * ( Hvap_Stage - 1. ) * 1000.;
  double H_0 = H_Liq ( T_liq, IsComp, X );
  double H_1 = H_Liq ( T_liq+1, IsComp, X );
//
  Cp_liq = H_1 - H_0;
  if ( Cp_liq < 10. )
    Cp_liq = 10.;
  double C_l = Cp_korp + Cp_liq * M_liq;
  if ( C_l > 300000. )
    C_l = 300000.;
  if ( k_Cool != 0. )
    dH_liq -= k_Cool * ( T_liq - T_air );
  if ( V_Cond > 0. && Hvap_Stage != 1. )
    C_l *= 0.01;
  if ( pStageHeat )
    {
    pStageHeat->Lev = Level;
    pStageHeat->T_ext = T_liq;
    pStageHeat->Calc( dt );
    dH_liq += pStageHeat->Q_Heat;
    }
  dT_liq = LimdX ( dH_liq * dt / C_l, 5. );
  //double dT_simple = 0.;
  //if ( pUp )
  //  {
  //  dT_simple = L_In * dt * ( pUp->T_liq  - T_liq ) / M_liq;
  //  double H_Up = H_Liq ( pUp->T_liq, IsComp, pUp->X );
  //  double dH_Up = L_In * ( H_Up - H_0 );
  //  if ( strstr ( ObjName, "Куб" ))
  //    KKK();
  //  }
  if ( pCol && !pCol->Work )
    {
    double Ksi = dt * 60. / pCol->Tau_cool;
    double T_new = ( T_liq + Ksi * T_air ) / ( 1. +  Ksi );
    double dT_liq_cool = T_new - T_liq;
    dT_liq += dT_liq_cool;
    }
//
  if ( T_liq_Z <= 0. )
    {
    T_liq += dT_liq;
    }
  else
    {
		if ( pSys -> bWorkReg )
			{
		  T_liq = T_liq_Z;
		  if(pCol)
			  pCol->Проблемы |= PROB_FIX_T;
      Eps_In.GammaPz *= P_Pz;
			}
    }
//
 // double T_min;
 // double T_max;
	//if ( pCol )
	//	{
	//	T_min = pCol->T_min;
	//	T_max = pCol->T_max;
	//	}
	//else
	//	{
	//	T_min = 0.1;
	//	T_max = 500.;
	//	}
	if ( T_liq < T_min )
		T_liq = T_min;
	if ( T_liq > T_max )
		T_liq = T_max;
	if ( T_gas < T_min )
		T_gas = T_min;
	if ( T_gas > T_max )
		T_gas = T_max;
	FINITE(T_gas)
	FINITE(T_liq)
//
  }

void CStage::CalcGas( double dt )
	{
  if ( StopGas )
    {
    BREAK
    if ( StopGas > 0 )
      StopGas--;
    }
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
    Src[o]->SetFlowOut = true;
		CComp * pCompOut = Src[o]->pComp;//
		pCompOut ->Set ( IsComp, Y );
    Norm( pCompOut );
		F_Gas_out -> T = T_gas;
    if ( pCol->OutPureGas )
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
    if ( CalcStageBalance )
      {
      if ( nStageBalance >= 0 )
        SummStageBalance += F * Y[nStageBalance];
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

void CStage::CalcLiq( double dt )
  {
  if ( StopLiq )
    {
    BREAK
    if ( StopLiq > 0 )
      StopLiq--;
    KKK();
    }
	for ( int o = 0; o < kSrc; o++ )
		{
    if ( Src[o]->Gas )
      continue;
    if ( Src[o]->pFlow == NULL )
      continue;
    CFlow * F_Liq_out = Src[o]->pFlow;
		double F = Src[o]->pFlow->Flow_mol;
		if ( F > 0. )
      continue;
		double dM = -F * dt;
		if ( dM > 0.2 * M_liq )
			{
			pCol->Проблемы |= PROB_HI_DM;
			dM = 0.2 * M_liq;
			}
		M_liq -= dM;
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
	FINITE(M_liq)
////////////////////////////////////////////////
// Слив
  Doliv = 0.;
  Sliv = 0.;
  dLdM = 0.;
	if ( h_st > 0. && StageType != Насадка )
		{
		if ( To_m3 <= 0. || CalcTom3 )
      {
      if ( M_liq > 0.1 )
		    To_m3 = To_m3_liq ( T_liq, IsComp, X );
      else
			  To_m3 = 0.2;
      }
    double kH =  To_m3 / S_Stage;
		double h_liq = M_liq * kH;
    Lev_old = Level;
		Level = h_liq / h_st;
		dLdM = kH / h_st;
		double M0 = h_st / kH;
		dh_hydro = h_liq * 0.1;
    double Mo = M_liq;
    double kSliv = 0;
    if ( StageType == Ситчатая )
      {
      if ( Vin * M3 < FlowStopSlivm3 )
        {
        double A = Vin * M3 / FlowStopSlivm3;
        kSliv += A * kExtLout;
        }
      }
    if ( !pCol->Work )
      kSliv += 60. / pCol->Tau_sliv;
    if ( StageType != Куб && !ЭтоКуб && StageType != Глухая )
      {
		  if ( h_liq > h_st )
			  {
		 	  if ( StopLiq )
				  KKK();
        double k_L_eff = k_L / ( 1. + k_LV * Vout * 1e-3 ); 
			  double M_liq_new = ( M_liq + dt * k_L_eff * M0 ) / ( 1. + dt * ( k_L_eff + kSliv ));
        double dM = M_liq - M_liq_new;
        M_liq = M_liq_new;
        double ExtLout = kSliv * M_liq;
        Lout = k_L_eff * ( M_liq - M0 ) + ExtLout;
			  }
		  else
			  {
		 	  if ( StopLiq )
				  KKK();
			  M_liq = M_liq / ( 1. + dt * kSliv );
			  Lout = kSliv * M_liq;
			  }
      }
// Ограничение скорости изменения
    double Mn = M_liq;
/**/
    double dM = Mn - Mo;
    double Lo = Level * 100;
		h_liq = M_liq * kH;
//
		Level = h_liq / h_st;
    if ( dLev_dt_max > 0. && Lev_old > 0. )
      {
      double dL = ( Level - Lev_old ) * 100. / dt;
      bool Recalc = false;
      if ( dL > dLev_dt_max )
        {
        Recalc = true;
        dL = dLev_dt_max;
        }
      if ( dL < -dLev_dt_max )
        {
        Recalc = true;
        dL = -dLev_dt_max;
        }
//      Sliv = dL * 0.01 * h_st / kH;
      if ( Recalc )
        {
        Level = Lev_old + 0.01 * dL * dt;
        h_liq = Level * h_st;
        double M_liq_new = h_liq / kH;
        Doliv = ( M_liq_new - M_liq ) / dt;
        M_liq = M_liq_new;
        }
      }
//......................................................
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
				Sliv = ( M_liq_new - M_liq ) / dt;
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
          Doliv = K * Max_Flow_Add / To_kg;
          M_liq += dt * Doliv;
          }
//        else
//          Doliv = 0.;
        }
			}
		}
	else
		{
// Насадка
    double k_L_eff = k_L / ( 1. + k_LV * Vout * 1e-3 ); 
		double K = 1. / ( 1. + dt * k_L_eff );
		M_liq *= K;
		Lout = k_L * M_liq;
		Level = 0.;
		}
	if ( M_liq < 1e-4 )
		M_liq = 0.;
	if ( !_finite(M_liq))
		M_liq = 0.;
  //if ( pRegLev )
  //  {
	 // double M_liq_dt = M_liq;
	 // double To_kg = ::Mw ( IsComp, X );
	 // double To_m3 = To_m3_liq ( T_liq, IsComp, X );
	 // pRegLev->Control ( dt, Level, ( M_liq_dt - M_liq_0 ) / dt, dLdM, To_kg, To_m3 );
  //  }
// Давление на выводах
	double dP_h;
	for ( int o = 0; o < kSrc; o++ )
		{
    if ( Src[o]->pFlow == NULL )
      continue;
    if ( Src[o]->Gas )
	    {
		  *(Src[o]->pPress ) = P;
		  double dL_h_In = Level - h_In;
		  if(dL_h_In > 0.0)
			  *(Src[o]->pPress ) += dL_h_In * k_Hydro_statatic;
		  continue;
	    }
    if ( pCol->UseHydrostat || dP_Hydro_Level != 0. || dP_Hydro_Static != 0. )
      {
		 
		  double dL = Level - Src[o]->Lev_Feed;
		  double Eliq;
		  if ( dL > 0.1 )
			  Eliq = 1.;
		  else
			  Eliq = 10. * dL;
		  if ( h_st > 0. && dL > 0. )
			  dP_h = dL * ( /*h_st * 0.1 + */ dP_Hydro_Level );
		  else
			  dP_h = 0.;
		  *(Src[o]->pPress ) = ( dP_h + dP_Hydro_Static ) * Eliq + P;
      }
    else
      *(Src[o]->pPress ) = P;
		}
/*
	for ( int s = 0; s < kSrc; s++ )
		{
    if ( Src[s]->pFlow == NULL )
      continue;
		double dL = Level - Src[s]->Lev_Feed;
		if ( h_st > 0. && dL > 0. )
			dP_h = dL * h_st * 0.1;
		else
			dP_h = 0.;
		*(Src[s]->pPress ) = P + dP_h;
		}
*/
///////////////////////////////////////////////
	FINITE(M_liq)
  FlowGasOut = 0.;
	for ( int o = 0; o < kSrc; o++ )
		{
    if ( Src[o]->Gas )
      continue;
    CFlow * F_Liq_out = Src[o]->pFlow;
		if ( F_Liq_out == NULL )
      continue;
    double F = F_Liq_out->Flow_mol;
		if ( F >= 0. )
      continue;
// Вывод
		F_Liq_out->ID_COMP = Src[o]->ID_Comp;
		CComp * pComp = Src[o]->pComp;
		F_Liq_out->pComp = pComp;
//
		double Eliq;
		if ( Level > pCol->Lev_Voronka )
			Eliq = 1.;
		else
      {
      if ( pCol->Lev_Voronka > 0. )
			  Eliq = Level / pCol->Lev_Voronka;
      else
        Eliq = 0.;
      }
		double Egas = 1. - Eliq;
		double Mix[K_GAS];
		CLEAR( Mix );
		if ( Eliq > 0. )
			{
      double Mw = 0.;
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( IsComp[n] )
					{
					Mix[n] = Eliq * X[n];
          Mw += X[n] * pGas[n]->Mw;
					}
				}
      Mw_Liq = Mw;
			}
    FlowGasOut += Egas * F;
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
    if ( Egas == 0. && pCol->OutPureLiq )
      F_Liq_out -> h_Flow = H_Liq ( T_liq, pComp );
    else
		  F_Liq_out -> h_Flow = Eliq * h_liq_mix + Egas * h_gas_mix;
    FINITE(F_Liq_out -> h_Flow)
		F_Liq_out -> T = T_liq;
		F_Liq_out -> To_kg = ::Mw ( pComp );
		F_Liq_out -> To_m3 = To_m3_liq ( T_liq, pComp );
    if ( CalcStageBalance )
      {
      if ( nStageBalance >= 0 )
        SummStageBalance += F * Mix[nStageBalance];
      else
        SummStageBalance += F;
      }
    }
//  if ( CalcStageBalance )
//    StageBalance = SummStageBalance + ConstStageBalance;
  }


