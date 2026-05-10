#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

/*************
	if ( pAddComp && ( pAddComp->AddFlow || pAddComp->AddMass || pAddComp->AddPress ) && pFlow[kFlow] == NULL )
		{
    pAddComp->ChangeComp ( &AddFlow );
    pFlow[kFlow] = &AddFlow;
    IsFlow[kFlow] = true;
    ID_Out[kFlow] = pAddComp->ID_Comp;
    pComp_Out[kFlow] = pAddComp->pComp;
		kFlow++;
    k_Node++;
		}
  if ( pAddComp && pAddComp->Change )
    {
    double F = 0.;
    if ( pAddComp->AddMass )
      {
      F = pAddComp->Flow / dt;
      pAddComp->AddMass = false;
      }
    else
      {
      if ( pAddComp->AddFlow )
        F = pAddComp->Flow;
      else
        {
        if ( pAddComp->AddPress )
          F = pAddComp->Flow * ( pAddComp->P - P );
        }
      }
    AddFlow.Flow_mol = F;
    AddFlow.h_Flow = pAddComp->h_Flow;
    AddFlow.ID_COMP = pAddComp->ID_Comp;
    AddFlow.pComp = pAddComp->pComp;
    }
************/
/************
  if ( Defect == 1 )
		{
		double F_mol = Def_Omega_top * ( 1. - P ) * 10000.;
		_Def_Flow_top = F_mol * M3;
		if ( F_mol > 0. )
			{
// Вход
			S.dF_Gas_in += F_mol;
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( pAir->IsComp[n] )
					{
					IsComp[n] = true;
					double FS = F_mol * pAir->Cmol[n];
					S.Summ_FS_Gas[n] += FS;
					S.H_summ_in_gas += FS * pGas[n]->H_IG( T_air );
					}
				}
			}
		if ( F_mol < 0. )
			{
// Выход
			double Eg = ( 1. - Lev_liq ) / D_trub;
      FINITE(Eg)
			if ( Eg <= 0. )
				{
				S.dF_Liq_out -= F_mol;
        H_summ_out_liq -= F_mol * h_liq_out;
        FINITE(S.dF_Liq_out)
				}
			else
				{
				if ( Eg >= 1. )
					{
					S.dF_Gas_out -= F_mol;
          H_summ_out_gas -= F_mol * h_gas;
					}
				else
					{
					double El = 1. - Eg;
					S.dF_Liq_out -= F_mol * El;
					S.dF_Gas_out -= F_mol * Eg;
          H_summ_out_gas -= F_mol * Eg * h_gas;
          H_summ_out_liq -= F_mol * El * h_liq_out;
          FINITE(S.dF_Liq_out)
					}
				}
			}
		}
	if ( Defect == 2 )
		{
		double F_mol = Def_Omega_bott * ( 1. - ( P + Def_Hydro_bott * Lev_liq ));
		if ( F_mol > 0. )
			{
// Вход
			_Def_Flow_bott = F_mol * pAirFlow->To_kg;
			S.dF_Gas_in += F_mol;
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( pAir->IsComp[n] )
					{
					IsComp[n] = true;
					double FS = F_mol * pAir->Cmol[n];
					S.Summ_FS_Gas[n] += FS;
					S.H_summ_in_gas += FS * pAirFlow->h_Flow;
					}
				}
			}
		if ( F_mol < 0. )   // Энтальпии !!!!!!!!!!!!!!!!
			{
// Выход
			double El = Lev_liq / D_trub;
			if ( El >= 1. )
				{
				S.dF_Liq_out -= F_mol;
        FINITE(S.dF_Liq_out)
				_Def_Flow_bott = F_mol * Mw_liq;
				}
			else
				{
				if ( El <= 0. )
					{
					S.dF_Gas_out -= F_mol;
					_Def_Flow_bott = F_mol * Mw_gas;
					}
				else
					{
					double Eg = 1. - El;
					S.dF_Liq_out -= F_mol * El;
					S.dF_Gas_out -= F_mol * Eg;
          FINITE(S.dF_Liq_out)
					_Def_Flow_bott = F_mol * ( Eg * Mw_gas + El * Mw_liq );
					}
				}
			}
		}
**********/
//if ( dM_gas > dM_gas_max && M_Gas > 0.1 )
//  {
//  double K = dM_gas_max / dM_gas; ;
//  if ( K < 0.1 )
//    K = 0.1;
//  //    dt *= K; 
//  //    dM_gas *= K;
//  }
/*
	double Eg = M_Gas / M;
	double El = 1. - Eg;
	double Cmol_mix[K_GAS];
	for ( int c = 0; c < LAST_GAS; c++ )
		{
		if ( IsComp[c] )
			{
      if ( !_finite(Cmol_Liq[c]))
        Cmol_Liq[c] = 0.;
      Cmol_mix[c] = 0.;
      if ( Eg > 0. )
			  Cmol_mix[c] += Eg * Cmol_Gas[c]; 
      if ( El > 0. )
			  Cmol_mix[c] += El * Cmol_Liq[c];
			}
		else
			{
			Cmol_mix[c] = 0.;
			}
    FINITE(Cmol_mix[c])
		}
	CompVol.Set ( IsComp, Cmol_mix );
*/
/**************************************************
	double k_Heat = 0., T_Heat = 0., Q_Heat = 0., Eps1 = 0.;
	if ( pVolHeat )
		pVolHeat -> GetHeat( dt, k_Heat, T_Heat, Eps1 );
	StateVol.T_min = T_min;
	StateVol.T_max = T_max;
	StateVol.T = T;
	StateVol.Vol	= Vol * Vol_Gas;
//	StateVol.T_max = 400.;
	if ( SetOmegaL )
		StateVol.Omega_L = Mw_liq / Ro_liq;
	else
		StateVol.Omega_L = 0.;
  double M_liq_old = StateVol.L;
  double M_gas_old = StateVol.V;
	if ( kStepTz > 0 )
		{
		kStepTz--;
		if ( kStepTz == 0 )
			T_Z = 0.;
		}
  StateVol.Vol = Vol * KSi_P;
  StateVol.LevelLiq = Lev_liq;
*************************************************/
/**************
		StateVol.MT( M, T_Z, &CompVol );
    double dHZ = StateVol.H - H + kToAir * ( T_Z - T_air ) * dt;
	  if ( pVolHeat )
      dHZ += k_Heat * ( T_Z - T_Heat );
    dH_Z = dHZ;
		//double R = pGas[G_H2O].Hv ( T_Z, P ) - pGas[G_H2O].Hl ( T_Z, P );
		double R = 1.0;	//#
		dSteam_kg = dH_Z * Mw_H2O / ( R * dt );
		if ( Calc_kToAir && fabs ( T_Z - T_air ) > 0.1 )									 
			{
//			dH_Z -= kToAir * ( T_Z - T_air );
			double K = -dH_Z	/ (( T_Z - T_air ) * dt );
			kToAir += K;
			if ( kToAir < 0. )
				kToAir = 0.;
			}
*******************/
