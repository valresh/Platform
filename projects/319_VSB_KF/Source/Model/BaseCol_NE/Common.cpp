#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"

// Суммирование входных потоков 
double CStage_NE::SummFlow()
  {
  if ( StopSrc )
    {
    BREAK
    }
  double Flow_Summ = 0.;
  Flow_Summ -= Vout + L_Summ_out;
  for ( int s = 0; s < kSrc; s++ )
    {
    CExtIO	* pSrc = Src[s];
    CFlow * pF = pSrc->pFlow;
    if ( pF == NULL )
      continue;
    double F = pF->Flow_mol;
    if ( F > 0. )
      Flow_Summ += F;
    }
  if ( pUp && pUp->L_Down > 0. )
    {
    double F = pUp->L_Down;
    Flow_Summ += F;
    }
  if ( pDown )
    {
    double F = pDown->Vout;
    Flow_Summ -= F;
    }
  return Flow_Summ;
  }

void CStage_NE::Summ_Flow_In( double dt )
  {
  if ( StopSrc )
    {
    BREAK
    if ( StopSrc > 0 )
    StopSrc--;
    }
  CLEAR(H_Summ_Gas_In_A);
  CLEAR(H_Summ_Liq_In_A);
  CLEAR(H_Summ_Gas_In_B);
  CLEAR(H_Summ_Liq_In_B);
	Flow_Summ_In = 0.;
  if ( StopSrc )
    {
    BREAK
    }
	CLEAR(Summ_In_Gas)
  CLEAR(Summ_In_Liq)
  Flow_Gas_Out_Src = 0.;
  Flow_Liq_Out_Src = 0.;
///////////////////////////////////////////////////
// Источники
  double GasOut = 0.;
/////////////////
  int c_Del = pCol->n_Comp_Delete;
	for ( int s = 0; s < kSrc; s++ )
		{
    CExtIO	* pSrc = Src[s];
    CFlow * pF = pSrc->pFlow;
		if ( pF == NULL )
			continue;
    double F = pF->Flow_mol;
    if ( F <= 0. )
      {
//--      if ( Numb != 1 && pSrc->Gas )
      if ( pSrc->Gas )
        GasOut -= F;
      if ( pSrc->pKarman )
        {
        double Flow_out_Karman = pSrc->pKarman->F_Karman_In;//Flow_In ( Lout_max, dt );
        Flow_Liq_Out_Src += Flow_out_Karman;
        }
      else
        {
        Flow_Gas_Out_Src -= F * pSrc->Eps;
        Flow_Liq_Out_Src -= F * ( 1. - pSrc->Eps );
        }
      continue;
      }
    Flow_Summ_In += F;
    if ( fabs ( Flow_Summ_In ) > 100000.)
      KKK();
    double F_gas = F * pSrc->Eps;
    double F_liq = F - F_gas;
    bool * IsCompF = pSrc->Src_IsComp;
    double * CmolLiq = pSrc->Src_CmolLiq;
    double * CmolGas = pSrc->Src_CmolGas;
		for ( int c = 0; c < LAST_GAS; c++ )
		  {
      if ( IsCompF[c] && c != c_Del )
			  {
        IsComp[c] = true;
				Summ_In_Gas[c] += F_gas * CmolGas[c];
        if ( fabs ( Summ_In_Gas[c] ) > 10000 )
          KKK();
        Summ_In_Liq[c] += F_liq * CmolLiq[c];
        if ( fabs ( Summ_In_Liq[c] ) > 10000.)
          KKK();
        H_Summ_Gas_In_A[c] -= F_gas * CmolGas[c];
        H_Summ_Liq_In_A[c] -= F_liq * CmolLiq[c];
        H_Summ_Gas_In_B[c] += F_gas * CmolGas[c] * pSrc->h_Src_Gas[c];
        H_Summ_Liq_In_B[c] += F_liq * CmolLiq[c] * pSrc->h_Src_Liq[c];
				}
			}
		}
  GasOutFromStage = GasOut;
///////////////////////////////////////////////////
  if ( StopSrc )
    {
    BREAK
    }
// Сверху
	if ( pUp && pUp->L_Down > 0. )
		{
		double F = pUp->L_Down;
//    Lin = F;
		double * uX = pUp -> X;
		double * uh_Liq = pUp -> h_liq;
    Flow_Summ_In += F;
    if ( fabs ( Flow_Summ_In ) > 100000.)
      KKK();
		for ( int c = 0; c < LAST_GAS; c++ )
			{
			if ( IsComp[c] )
				{
				Summ_In_Liq[c] += F * uX[c];
        if ( fabs ( Summ_In_Liq[c] ) > 10000.)
          KKK();
        H_Summ_Liq_In_A[c] -= F * uX[c];
        H_Summ_Liq_In_B[c] += F * uX[c] * uh_Liq[c];
				}
			}
		}
//  else
//    Lin = 0.;
/*
  if ( pUp && pUp->Vin < 0. )
    {
    double F = -pUp->Vin;
    double * uY = pUp -> Y;
    double * uh_Gas = pUp -> h_gas;
    Flow_Summ_In += F;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        Summ_In_Gas[c] += F * uY[c];
        H_Summ_Gas_In += F * uY[c] * ( uh_Gas[c] - h_gas[c] );
        }
      }
    }
*/
  if ( StopSrc )
    {
    BREAK
    }
///////////////////////////////////////////////////
// Снизу
	if ( pDown )
		{
		double F = pDown->Vout;
    if ( F < 0. )
      F = 0.;
//    Vin = F;
		double * dY = pDown -> Y;
		double * dh_Gas = pDown -> h_gas;
    Flow_Summ_In += F;
		for ( int c = 0; c < LAST_GAS; c++ )
			{
			if ( IsComp[c] )
        {
        if ( pCol->ModelCond != 1 )
				  Summ_In_Gas[c] += F * dY[c];
        if ( fabs ( Summ_In_Gas[c] ) > 100000 )
          KKK();
        FINITE(Summ_In[c])
        H_Summ_Gas_In_A[c] -= F * dY[c];
        H_Summ_Gas_In_B[c] += F * dY[c] * dh_Gas[c];
        }
			}
 		}
//  else
//    Vin = 0.;
  //if ( fabs ( Flow_Summ_In ) > 100000.)
  //  KKK();
  //if ( fabs ( Flow_Summ_In ) < 0.1 )
  //  KKK();
  if ( StopSrc )
    {
    BREAK
    }
  }
///////////////////////////////////////////////////////////////////
void CStage_NE::Model_Cond( double dt )
  {
  StageBalance = SummStageBalance;
  SummStageBalance = 0.;
//........................
  Summ_Flow_In( dt );
//........................
  if ( pCol->CalcStageBalance )
    {
    if ( pCol->n_Comp_Balance >= 0 )
      SummStageBalance = Summ_In[pCol->n_Comp_Balance];
    else
      SummStageBalance = Flow_Summ_In;
    }
  if ( Flow_Summ_In < 0.01 )
    {
// Нет подачи
    V_Cond = 0.;
    Vout = 0.;
    M_liq -= dt * L_Summ_out;
    if ( M_liq < 0. )
      M_liq = 0.;
    L_Summ_out = 0.;
    L_Karman = 0;
    L_Down = 0;
    ExtHeat = 0.;
    dH_cond = 0.;
    CLEAR(X_In)
    CLEAR(Y_In)
    return;
    }
  Norm ( IsComp, Summ_In );
  if ( StopSrc )
    {
    BREAK
    }
///////////////////////////////////////////////////
  double dH_liq = 0.;
//  if ( pGammaCommon || pGammaAbs || pColumnGamma )
  if ( pColumnGamma )
    {
    IGammaParam GP;
    GP.Param = (DWORD*)&Numb;
    GP.P = &P;
    GP.T = &T_liq;
    GP.IsComp = IsComp;
    GP.X = X;
    GP.P_sat = P_z;
    GP.Gamma = Gamma;
    GP.dH_liq = &dH_liq;
//    if ( pGammaCommon )
//      pGammaCommon->Gamma( &GP );
//    if ( pGammaAbs )
//      pGammaAbs->Gamma( &GP );
    if ( pColumnGamma )
      pColumnGamma->Gamma( &GP );
    }
  double Q_React = 0;
  if ( pReactions )
    {
    IReactionsParam Parm;
    Parm.Param = (DWORD*)&Numb;
    Parm.dt = dt;
    Parm.P = &P;
    Parm.T = &T_liq;
    Norm( IsComp, X );
    Norm( IsComp, Y );
    Parm.IsComp = IsComp;
    Parm.X = X;
    Parm.Y = Y;
    Parm.Q = &Q_React;
    Parm.M_Liq = &M_liq;
    double R = 0.;
    Parm.R = &R;
    double Mw_in = X[1];
    pReactions->Reactions( dt, &Parm );
    double Mw_out = X[1];
    M_liq = M_liq * Mw_in / Mw_out;
    ExtHeat = Q_React;
    }
  double RT = RP * ( T_gas + TK );
  M_gas = P * Vol_Stage / RT;
//
/*****
  if ( pCalcChemEx )
    {
    double k_Flow, dh;
    (*pCalcChemEx)( dt, *Eps_In.T, P, IsComp, Summ_In, pCalcChemParam, k_Flow, dh );
    Flow_Summ_In *= k_Flow;
    h += dh;
    }
  if ( Eps_In.WaterCol )
    {
    switch ( pCol->AdsType )
      {
      case AdsH2S_MEA:
        {
        pCol->pAdsGas_1->CalcGamma( T_liq, IsComp, X, Gamma );
        break;
        }
      case AdsCO2_MEA:
        {
        if ( pCol->UseCO2Appr )
          {
          int g1 = G_CO2;
          int g2 = G_MDEA;
          if ( g1 > 0 && g2 > 0 )
            Gamma[G_CO2] = CMDEA_CO2::Gamma_CO2 ( T_liq, X[G_CO2], X[G_MDEA] );// * pCol->HenryH2S;
          }
        else
          pCol->pAdsGas_1->CalcGamma( T_liq, IsComp, X, Gamma );
        break;
        }
      case AdsCO2_K2CO3:
        {
        int g1 = G_CO2;
        int g2 = G_K2CO3;
        pCol->pAdsGas_1->CalcGamma( T_liq, IsComp, X, Gamma );
        break;
        }
      }
    }
*/
  double L = 0.;
  double V = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      L += Summ_In_Liq[c];
      V += Summ_In_Gas[c];
      if ( pCol->Static_Calc_Mix )
        {
        X_In[c] = Summ_In_Liq[c];
        Y_In[c] = Summ_In_Gas[c];
        }
      else
        {
        Y_In[c] = M_gas * Y[c] + dt * Summ_In_Gas[c];
        X_In[c] = M_liq * X[c] + dt * Summ_In_Liq[c];
        }
      }
    }
  Lin = L;
  Vin = V;
  if ( StopSrc )
    {
    BREAK
    }
/////////////////////////////////////////////////////////////////////////
  pCol->ModelCond = 0;
  if ( pCol->Static_Calc_Mix )
    {

// Статика
    switch ( pCol->ModelCond )
      {
      case 1:
        Sloj0.Flow_L = X_In;
        Sloj0.Flow_V = Y_In;
        CalcCond_Sloj( );
      break;
      case 0:
      default:
        CalcCond_Static_Vol( );
      break;
      }
    if ( pCol->ModelCond != 1 )
      {
      double Vc = 0.;
      double dH = 0.;
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( IsComp[c] )
          {
          double Alfa = Alfa_Comp[c] * Alfa_Stage * Alfa_Level;
          if ( NoCond )
            Alfa = 0.;
          double Flow_Cond = Alfa * ( P * Y[c] - P_z[c] * X[c] );
          Vc_Comp[c] = Flow_Cond;
          Vc += Flow_Cond;
          double hc = Flow_Cond * ( h_gas[c] - h_liq[c] ) * k_h_cond[c];
          h_cond[c] = hc;
          dH += hc; 
          }
        }
      V_Cond = Vc;
      dH_cond = dH;
      }
    }
  else
    {
// Динамика
    switch ( pCol->ModelCond )
      {
      case 1:
      break;
      case 0:
      default:
        CalcCond_Dynamic_Vol( dt );
      break;
      }
    }
/////////////////////////////////////////////////////////////////////////////
////// Баланс ///////////////////////////////////////////////////////////////////////
  Vin; Vout; V_Cond;
  if ( StopSrc )
    {
    BREAK
    }
//////////////////////////////////////////////////////////
  if ( pCol->CalcStageBalance )
    {
    if ( pCol->n_Comp_Balance >= 0 )
      {
//      if ( pUp )
        SummStageBalance -= Vout * Y[pCol->n_Comp_Balance];
//      if ( pDown )
        SummStageBalance -= L_Down * X[pCol->n_Comp_Balance];
      }
    else
      {
//      if ( pUp )
        SummStageBalance -= Vout;
//      if ( pDown )
        SummStageBalance -= L_Down;
      }
    }
  }

double CBaseCol_NE::Show_Press ( double Pabs )
  {
  double P = Pabs - 1.;
  double Value;
  switch ( MesPress )
    {
    case 0: //атм;
      Value = P;
      break;
    case 1: //Па;
      Value = P * 98066.5;
      break;
    case 2: //КПа;
      Value = P * 98.0665;
      break;
    case 3: //МПа;
      Value = P * 0.0980665;
      break;
    case 4: //мм в.ст.;
      Value = P * 1e4;
      break;
    case 5: //мм рт.ст.;
      Value = P * 760.;
      break;
    case 6: //атм(а);
      Value = Pabs;
      break;
    case 7: //Па(а);
      Value = Pabs * 98066.5;
      break;
    case 8: //мм в.ст.(а);
      Value = Pabs * 1e4;
      break;
    case 9: //мм рт.ст.(а);
      Value = Pabs * 760.;
      break;
    }
  return Value;
  }


