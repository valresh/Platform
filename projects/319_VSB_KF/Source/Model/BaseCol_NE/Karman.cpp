#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "Extensions.h"


Q_DECL_EXPORT CKarman::CKarman()
  {
  Vol = 5.;
  Alfa_Karman = 0.5;
  F_Karman_In = 0.;
  F_Karman_Out = 0.;
  Eps_Press = 1.;
  To_m3 = 0.018;
  To_kg = 18.;
  dF_kg = 0.;
  M_liq = 0.;
  Level = 0.;
  nStage = 0;
  pSrc = 0;
  pP = NULL;
  //
  F_Karman_In_kg = 0.;
  F_Karman_Out_kg = 0.;
  P_out = 0.;
  LevZ = 0.;
  HydroPress = 0.;
  }

void CKarman::SetPress( CExtIO * pSrc )
  {
  if ( pSrc->dP_Hydro_Level != 0. || pSrc->dP_Hydro_Static != 0. )
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
    HydroPress = ( dP_h + pSrc->dP_Hydro_Static ) * Eliq + *pP;
    }
  else
     HydroPress = *pP;
  *(pSrc->pPress) += Eps_Press * ( HydroPress - *(pSrc->pPress));
  }

void CKarman::OutLiq( CExtIO * pSrc, bool * IsComp, double * X, double * Y, double & FlowGasOut, double ** ppMixOut )
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
  T_liq = pSrc->Src.T;
  if ( Eliq == 1. )
    {
    pComp->Set( IsComp, X );
    To_m3 = To_m3;
    To_kg = ::Mw ( pComp );
    F_Karman_Out_kg = F_Karman_Out * To_kg;
    F_Karman_In_kg = F_Karman_In * To_kg;
    F_Liq_out -> T = T_liq;
    F_Liq_out -> To_kg = To_kg;
    F_Liq_out -> To_m3 = To_m3;
    F_Liq_out -> h_Flow = ::H_Liq ( T_liq, IsComp, X );
    *ppMixOut = X;
    return;
    }
  static double Mix[K_GAS];
  CLEAR( Mix );
  *ppMixOut = Mix;
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
  if ( Egas == 0. )
    F_Liq_out -> h_Flow = H_Liq ( T_liq, pComp ); 
  else
    F_Liq_out -> h_Flow = Eliq * H_Liq ( T_liq, IsComp, X ) + Egas * H_IG ( T_liq, IsComp, Y ); 
  FINITE(F_Liq_out -> h_Flow)
  F_Liq_out -> T = T_liq;
  To_kg = ::Mw ( pComp );
  F_Karman_Out_kg = F_Karman_Out * To_kg;
  F_Karman_In_kg = F_Karman_In * To_kg;
  F_Liq_out -> To_kg = To_kg;
  F_Liq_out -> To_m3 = To_m3;
  }

void CKarman::Sliv( CExtIO * pSrc, double dt )
  {
  CFlow * F_Liq_out = pSrc->pFlow;
  F_Karman_Out = F_Liq_out->Flow_mol;
  if ( F_Karman_Out > 0. )
    F_Karman_Out = 0.;
  F_Karman_Out_kg = F_Karman_Out * To_kg;
  //double dM = -F_Karman_Out * dt;
  //if ( dM > 0.2 * M_liq )
  //  {
  //  dM = 0.2 * M_liq;
  //  }
  //M_liq -= dM;
  }

double CKarman::Static_Level ( double Lout )
  {
  F_Karman_In = -F_Karman_Out;// В статике
  F_Karman_Out_kg = F_Karman_Out * To_kg;
  F_Karman_In_kg = F_Karman_In * To_kg;
  dF_kg = 0.;
  Level = 1.;
  if ( To_m3 <= 0. )
    To_m3 = 0.018;
  M_liq = Level * Vol / To_m3;
  FINITE(M_liq)
  return F_Karman_In;
  }

double CKarman::Flow_In ( double Flow_Summ, double dt )
  {
  To_m3 = 0.2;//18e-3;
  //if ( To_m3 == 0. )
  //  {
  //  To_m3 = To_m3_liq( pSrc->Src.T, 	pSrc->pFlow->Get_Comp());
  //  if ( To_m3 == 0. )
  //  if ( M_liq > 0. )
  //    To_m3 = Vol / M_liq;
  //  else
  //    To_m3 = 18e-3;
  //  }
  //else
  //  {
  //  if ( fabs ( pSrc->pFlow->Flow_mol ) > 1. )
  //    To_m3 = To_m3_liq( pSrc->Src.T, 	pSrc->pFlow->Get_Comp());
  //  }
  if ( LevZ > 0. )
    {
    Level = LevZ * 0.01;
    double V = Level * Vol;
    M_liq = V / To_m3;
    LevZ = 0.;
    }
  if ( LevZ < 0. )
    {
    Level = -LevZ * 0.01;
    double V = Level * Vol;
    M_liq = V / To_m3;
    }
  double V = M_liq * To_m3;
  Level = V / Vol;
  double Flow_max = Flow_Summ * Alfa_Karman;
  if ( F_Karman_Out > 0. )
    F_Karman_Out = 0.;
  if ( Level >= 0.99999 )
    {
    Level = 1.;
    double F_out = -F_Karman_Out;
    if ( F_out > Flow_max )
      F_out = Flow_max;
    F_Karman_In = 0.9 * F_Karman_In + 0.1 * F_out;
    }
  else
    F_Karman_In = 0.9 * F_Karman_In + 0.1 * Flow_max;
  double dM = ( F_Karman_In + F_Karman_Out ) * dt;
  M_liq += dM;
  if ( M_liq < 0. )
    M_liq = 0.;
  FINITE(M_liq)
  F_Karman_Out_kg = F_Karman_Out * To_kg;
  F_Karman_In_kg = F_Karman_In * To_kg;
  dF_kg = F_Karman_In_kg + F_Karman_Out_kg;
  Level = M_liq * To_m3 / Vol;
  if ( Level >= 1.01 )
    {
    Level = 1.01;
    V = Level * Vol;
    M_liq = V / To_m3;
    }
  return F_Karman_In;
  }
