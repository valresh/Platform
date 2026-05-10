#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

double To_m3 ( double T, bool * IsComp, double * Cmol_Liq	)
  {
  double To = To_m3_liq ( T, IsComp, Cmol_Liq	);
  if ( To == 0. )
    To = 0.018;
  return To;
  }

double CVolBase::CalcFlowIn ( int nComp )
  {
  double F = 0.;
  for ( int n = 0; n < k_Node; n++ )
    {
    CFlow * pF = pFlow[n];
    if ( pF == NULL )
      continue;
    double F_mol = pF ->Flow_mol;
    if ( F_mol <= 0. )
      continue;
    if ( nComp < 0 || nComp >= LAST_GAS )
      {
      F += F_mol;
      continue;  
      }
    CComp * pComp = pF -> Get_Comp();
    if ( pComp == NULL )
      continue;
    if ( pComp->IsComp[nComp] )
      {
      F += F_mol * pComp->Cmol[nComp];
      continue;  
      }
    }
  return F;
  }

void CVolBase::CalcShowComp( )
  {
  }

static double EpsCmol = 1e-9;
void CVolBase::Compress (  )
{
	ЕстьЗадания = false;
	double Sx = 0.;
	double Sy = 0.;
	for ( int c = 0; c < LAST_GAS; c++ )
	{
		if ( IsComp[c] )
		{
			if ( !_finite ( Cmol_Gas[c] ))
				Cmol_Gas[c] = 0.;
			if ( Cmol_Gas[c] < EpsCmol && Cmol_Liq[c] < EpsCmol )
			{
				IsComp[c] = false;
				Cmol_Liq[c] = 0.;
				Cmol_Gas[c] = 0.;
				h_Gas[c] = 0.;
				h_Liq[c] = 0.;
			}
			Sx += Cmol_Liq[c];
			Sy += Cmol_Gas[c];
		}
	}
	if ( Sy < 1e-6 )
	{
		IsComp[G_N2] = true;
		Cmol_Gas[G_N2] = 1.;
	}
}
                                 
void CVolBase::SetComp( CComp * pComp, double T, double Lev )
  {
  double To_m3 = To_m3_liq( T, pComp );
  if ( To_m3 )
    {
    double Msumm = Vol * Lev * 0.01 / To_m3;
    StateVol.MT( Msumm, T, pComp );
    StateVol.SetCompData ( IsComp, Cmol_Liq, Cmol_Gas, h_Liq, h_Gas );
    StateVol.Calc_Mw();
    P = StateVol.P;
    T = StateVol.T;
    M = Msumm; 
    M_Gas = StateVol.V;
    M_Liq = StateVol.L;
    Lev_liq =Lev * 0.01;
    H = StateVol.H;
    Mw_liq = StateVol.Mw_liq;
    Mw_gas = StateVol.Mw_gas;
    CompVol.Set ( pComp );
    }
  }

void CVolBase::CalcHliq( )
  {
  double h_liq = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      h_Liq[c] = pGas[c]->H_l( T );
      h_liq += Cmol_Liq[c] * h_Liq[c];
      }
    }
  H = M_Gas * h_gas + M_Liq * h_liq;
  }

void CVolBase::DeleteComp( double dt )
  {
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( !IsDelete[n] || !IsComp[n]	)
      continue;
    double F_Delete_Gas = 0., F_Delete_Liq = 0.;
    if( M_Liq != 0.0 )
      {
      if ( k_Delete < 0. )
        {
        Cmol_Liq[n] = 0.;
        //          H -= M_Liq * Cmol_Liq[n] * h_Liq[n];
        IsComp[n] = false;
        }
      else
        {
        F_Delete_Liq = M_Liq * Cmol_Liq[n] * k_Delete;
        F_Del[n] = F_Delete_Liq;
        Cmol_Liq[n] -= dt * F_Delete_Liq / M_Liq;
        //				  H -= dt * F_Delete_Liq * h_Liq[n];
        }
      }
    if( M_Gas != 0.0 )
      {
      if ( k_Delete < 0. )
        {
        Cmol_Gas[n] = 0.;
        //          H -= M_Gas * Cmol_Gas[n] * h_Gas[n];
        IsComp[n] = false;
        }
      else
        {
        F_Delete_Gas = M_Gas * Cmol_Gas[n] * k_Delete;
        Cmol_Gas[n] -= dt * F_Delete_Gas / M_Gas;
        //		  		H -= dt * F_Delete_Gas * h_Gas[n];
        }
      }
    F_Del[n] = F_Delete_Liq + F_Delete_Gas;
    }
  Norm ( IsComp, Cmol_Liq );
  Norm ( IsComp, Cmol_Gas );
  }

void CVolBase::CalcBalance( double dt )
  {
  double B_in = 0.;
  double B_out = 0.;
  double B_dM = 0.;
  double B_in_S = 0.;
  double B_out_S = 0.;
  double B_dM_S = 0.;
  double H_in_S = 0.;
  double H_out_S = 0.;
  double H_dM_S = 0.;
  if ( n_Balance >= 0 && n_Balance < LAST_GAS )
    {
    for ( int n = 0; n < k_Node; n++ )
      {
      if ( pFlow[n] == NULL )
        continue;
      CFlow * pF = pFlow[n];
	  if(!pF)
		  continue;	  
      double F_mol = pF ->Flow_mol;
      // Вход 
      CComp * pComp = pF -> Get_Comp();
      if ( pComp == NULL )
        continue;
      if ( F_mol > 0. )
        {
        B_in += F_mol * pComp ->Cmol[n_Balance];
        B_in_S += F_mol; 
        H_in_S += F_mol * pF->h_Flow; 
        }
      else
        {
        B_out -= F_mol * pComp ->Cmol[n_Balance];
        B_out_S -= F_mol; 
        H_out_S -= F_mol * pF->h_Flow; 
        }
      }
    B_dM = dMdt_mol * Cmol_Liq[n_Balance];
    double Mw = pGas[n_Balance]->Mw;
    Balance_in = B_in * Mw;
    Balance_out = B_out * Mw;
    Balance_dM = B_dM * Mw;
	if(Balance_in != 0.0)
		Balance = ( Balance_out - Balance_dM ) / Balance_in;
	else
		Balance = 0.0;

    }
  else
    {
    for ( int n = 0; n < k_Node; n++ )
      {
      CFlow * pF = pFlow[n];
      if ( pF == NULL )
        continue;
      double F_mol = pF ->Flow_mol;
      double F_kg = pF ->Flow_mol * pF->To_kg;
      // Вход 
      CComp * pComp = pF -> Get_Comp();
      if ( pComp == NULL )
        continue;
      if ( F_mol > 0. )
        {
        B_in_S += F_kg; 
        H_in_S += F_mol * pF->h_Flow; 
        }
      else
        {
        B_out_S -= F_kg; 
        H_out_S -= F_mol * pF->h_Flow; 
        }
      }
    Balance_in = 0.;
    Balance_out = 0.;
    Balance_dM = 0.;
    Balance = 0.;
    }
  H_in_S += Q_ext;
  B_dM_S = dMdt_mol;
  H_dM_S = dMdt_mol * h_liq;
//
  Balance_in_S = B_in_S;
  Balance_out_S = B_out_S;
  Balance_dM_S = B_dM_S;
  if(Balance_out_S != 0.0)
	  Balance_S = (  Balance_in_S - Balance_dM_S ) / Balance_out_S;
  else 
	  Balance_S = -1.0;
  //
  Balance_H_in_S = H_in_S;
  Balance_H_out_S = H_out_S;
  Balance_H_dM_S = H_dM_S;
  if(Balance_H_out_S)
	  Balance_H_S = ( Balance_H_in_S - Balance_H_dM_S ) / Balance_H_out_S;
  else
	  Balance_H_S = -1.0;
  }

void CVolBase::CalcRoH( )
  {
  h_gas = 0.;
  h_liq = 0.;
  //.....................................................
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      h_Gas[c] = pGas[c]->H_IG( T );
      h_gas += Cmol_Gas[c] * h_Gas[c];
      h_Liq[c] = pGas[c]->H_l( T );
      h_liq += Cmol_Liq[c] * h_Liq[c];
      }
    }
  if ( M_Liq > 0.1 )
    {
    if ( Is_2F )
      {
      Mw_1F = ::Mw ( IsComp, Cmol_1F );
      Ro_liq = Mw_1F / M3_liq_1;
      }
    else
      {
      Mw_liq = ::Mw ( IsComp, Cmol_Liq );
      Ro_liq = Mw_liq / M3_liq;
      }
    }
  else
    Ro_liq = 1000.;
  FINITE(Ro_liq)
  }

void CVolBase::Set_P_Z( double dt )
  {
  ЕстьЗадания = true;
  double aP_Z = P_Z + 1.;
  double M_Gas_new = aP_Z * Vol / ( RP * ( T + TK ));
  FlowGas_P_Z = ( M_Gas - M_Gas_new ) / dt;
  M_Gas = M_Gas_new;
  M = M_Liq + M_Gas;
  H = M_Liq * h_liq + M_Gas * h_gas;
  }

void CVolBase::Add( )
  {
  if ( Add_Comp )
    {
	  CComp CompAdd;
    if ( !CompAdd.Read ( Add_Comp_Name ))
      {
      Add_Comp = 0.;
      return;
      }
    double M_add = Add_Comp / Mw ( &CompAdd );
    Add_Comp = 0.;
    CSEps State ( Add_Comp_Name );
    State.PT ( P, Add_Comp_T, &CompAdd );
	  State.Calc_Mw();
    double Cmol_Add[K_GAS];
	  State.SetCompData( NULL, Cmol_Add, NULL, h_Liq, NULL );
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( CompAdd.IsComp[c] )
        IsComp[c] = true;
      if ( IsComp[c] )
        {
        Cmol_Liq[c] = M_Liq * Cmol_Liq[c] + M_add * Cmol_Add[c];
        }
      }
    M_Liq = Norm ( IsComp, Cmol_Liq );
    return;
    }
  if ( Add_1F )
    {
    double M_add = Add_1F / Mw_1F;
    Add_1F = 0.;
    M_Liq_1 += M_add;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        Cmol_Liq[c] = M_Liq * Cmol_Liq[c] + M_add * Cmol_1F[c];
        }
      }
    M_Liq = Norm ( IsComp, Cmol_Liq );
    }
  if ( Add_2F )
    {
    double M_add = Add_2F / Mw_2F;
    Add_2F = 0.;
    M_Liq_2 += M_add;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        Cmol_Liq[c] = M_Liq * Cmol_Liq[c] + M_add * Cmol_2F[c];
        }
      }
    M_Liq = Norm ( IsComp, Cmol_Liq );
    }
  }

void CVolBase::CalcT( double dt )
  {
  S.dH_in = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double hg = S.H_summ_in_gas_A[c] * h_Gas[c] +  S.H_summ_in_gas_B[c];
      double hl = S.H_summ_in_liq_A[c] * h_Liq[c] +  S.H_summ_in_liq_B[c];
      S.dH_in += hg + hl;
      }
    }
  _Hs_in = S.dH_in * 1e-6;
  double dT = (	S.dH_in + S.Q + S.dQ_mix ) * dt / ( Cp_liq * M_Liq + Cp_corp );
//  if ( strstr ( ObjName, "Т-41_1_Емк" ))
//    KKK();
  T += dT;
  if ( T < T_min )
    T = T_min;
  if ( T > T_max )
    {
    //ASS(0)
    T = T_max;
    }
  }
