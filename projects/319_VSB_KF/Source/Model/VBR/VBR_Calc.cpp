#include "stdafx.h"
#include "VBR.h"
#include "Err.h"
#include "CommProc.h"

#define TK 273.15
#define RG 8.314471	// kJ/kMol K
#define RP 0.084784873375448 // Для расчета P в атм
#define MPa_atm 10.197266112955116733203828053699


double Ro_T ( double Ro_0, double T_0, double TC )
  {
  double A15 = 613.97226 / ( Ro_0 * Ro_0 );
  double Kt = exp ( -A15 * ( TC - T_0 ) * ( 1. + 0.8 * A15 * ( TC - T_0 )));
  return Ro_0 * Kt;
  }


double Calc_k ( double TC, double T0_C, double k0, double Max, bool Порог )
  {
  if ( Порог && TC < T0_C )
    return 0.;
  double k = k0 * exp ( log ( Max ) * ( 1. - ( T0_C + TK ) / ( TC + TK )));
  return k;
  }

void CVBR::CalcK ( double TC )
  {
  CLEAR(K)
  double kS = k_all * k_Flow;
  K[2][0] = kS * Calc_k ( TC, T0_C, k_20 * k_Gas, Max_20, Порог );
  K[2][1] = kS * Calc_k ( TC, T0_C, k_21 * k_P_1, Max_21, Порог );
//
  K[3][0] = kS * Calc_k ( TC, T0_C, k_30 * k_Gas, Max_30, Порог );
  K[3][1] = kS * Calc_k ( TC, T0_C, k_31 * k_P_1, Max_31, Порог );
  K[3][2] = kS * Calc_k ( TC, T0_C, k_32 * k_P_2, Max_32, Порог );
//
  K[4][0] = kS * Calc_k ( TC, T0_C, k_40 * k_Gas, Max_40, Порог );
  K[4][1] = kS * Calc_k ( TC, T0_C, k_41 * k_P_1, Max_41, Порог );
  K[4][2] = kS * Calc_k ( TC, T0_C, k_42 * k_P_2, Max_42, Порог );
  K[4][3] = kS * Calc_k ( TC, T0_C, k_43, Max_43, Порог );
//
  K[5][0] = kS * Calc_k ( TC, T0_C, k_50 * k_Gas, Max_50, Порог );
  K[5][1] = kS * Calc_k ( TC, T0_C, k_51 * k_P_1, Max_51, Порог );
  K[5][2] = kS * Calc_k ( TC, T0_C, k_52 * k_P_2, Max_52, Порог );
  K[5][3] = kS * Calc_k ( TC, T0_C, k_53, Max_53, Порог );
  K[5][4] = kS * Calc_k ( TC, T0_C, k_54, Max_54, Порог );
  K[5][6] = kS * Calc_k ( TC, T0_C, k_56, Max_56, Порог );
//
  for ( int i = 0; i < K_COMP; i++ )
    {
    double S = 0.;
    for ( int j = 0; j < K_COMP; j++ )
      {
      if ( i != j )
        S += K[i][j];
      }
    K[i][i] = -S;
    }
  }



void CVBR::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( Change_C && kStep > 2 )
    {
    Change_C = false;
    Norm( IsComp_React, CmolOpt );
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp_React[c] )
        {
        double Copt = CmolOpt[c];
        double C = Cmol_React[c];
        if ( C * kCompMax >= Copt && C / kCompMax <= Copt ) 
          kComp[c] = Copt / C;
        else
          {
          if ( C * kCompMax < Copt ) 
            kComp[c] = kCompMax;
          else
            kComp[c] = 1. / kCompMax;
          }
        }
      else
        kComp[c] = 1.;
      }
    }
  Change_k = false;
//
  In.PH ( pFlow_in );
  double P_MPa = pFlow_out->P  * iMPa_atm;
  k_Gas = ( 1. + kPress_gas ) / ( 1. + kPress_gas * P_MPa );
  k_P_1 = ( 1. + kPress_1 ) / ( 1. + kPress_1 * P_MPa );
  k_P_2 = ( 1. + kPress_2 ) / ( 1. + kPress_2 * P_MPa );
  double Flow_kg = pFlow_in->Flow_kg();
  double Flow_t = Flow_kg * 1e-3;
  if ( Flow_t * 10. > Flow_nom_t )
    k_Flow = Flow_nom_t / Flow_t;
  else 
    k_Flow = 10.;
  Ro_From_Pech = Ro_T ( Ro_Pech, TRo_Pech, In.T );
//
  Prepare( );
  if ( Flow_Conv < 1. )
    {
    *pFlow_out = *pFlow_in;
    return;
    }
  if ( !On || ( Порог && In.T < T0_C ))
    {
    dQ_React = 0.;
    CalcT ( 1e6 );
    *pFlow_out = *pFlow_in;
    return;
    }
  React( dt );
  Convert( );
  CalcT( dt );
  pFlow_out->ID_COMP = ID_CompSumm;
  pFlow_out->pComp = pCompSumm;
  pFlow_out->T = Tout;
  pFlow_out->h_Flow = Out_Реактор.h;
  pFlow_out->To_kg = Mw_summ;
  pFlow_out->To_m3 = To_m3_liq ( Tout, pCompSumm );
//
  _Давление_на_выходе = pFlow_out->P - 1.; 
  _Температура_на_входе = In.T;
  _Температура_на_выходе = Out_Реактор.T;
  _Коксуемость = 0.;
  _Вязкость = 0.;

	}
/*
int Comp[K_COMP] = { 5, 4, 3, 2, 1, 0, 6 };
void CVBR::React( double dt )
  {
  SET_BP BreakPoint;
  CalcK ( Tout );
  CLEAR(Cout)
  CLEAR(S)
  dQ_React = 0.;
	for ( int i = 0; i < K_COMP-1; i++ )
		{
		int From = Comp[i];
		double k = 0.;
// Суммарный k
    for ( int j = i + 1; j < K_COMP; j++ )
      {
      int To = Comp[j];
      k += K[From][To];
      }
    if ( k < 1e-6 )
      {
      Cout[From] = Cin[From] + S[From];
      continue;
      }
    double C_new = ( Cin[From] + S[From] ) /( 1 + k );
    if ( !_finite ( C_new ))
      C_new = Cin[From];
    double dC = Cin[From] - C_new;
    if ( i == 0 )
      dQ_React = dC * Q_React * 1e6;
    Cout[From] = C_new;
    double N = dC / k;
    double SummN = 0.; 
    for ( int j = i + 1; j < K_COMP; j++ )
      {
      int To = Comp[j];
      double D = N * K[From][To]; 
      S[To] += D;
      SummN += D;
      }
    KKK();
    }
  }
*/

void CVBR::React( double dt )
  {
  SET_BP BreakPoint;
  CalcK ( Tout );
  CLEAR(Cout)
  dQ_React = 0.;
  double M[K_COMP][K_COMP];
  double V[K_COMP];
  CLEAR(M)
	for ( int i = 0; i < K_COMP; i++ )
		{
    M[i][i] = -1.;
    V[i] = -Cin[i];
    }
  for ( int i = 0; i < K_COMP; i++ )
    {
    for ( int j = 0; j < K_COMP; j++ )
      {
      M[i][j] += K[j][i];
      }
    }
  ASS( LinSolv ( K_COMP, (double*)M, V, Cout ));
  double T = 0.;
  double Q = 0.;
  for ( int i = 0; i < K_COMP; i++ )
    {
    double dC = Cout[i] - Cin[i];
    T += dC;
    if ( dC < 0. )
      Q += dC;
    }
  dQ_React = Q * 1e6;
  }

void CVBR::Prepare( )
  {
  CComp * pCompIn = pFlow_in->Get_Comp();
  CLEAR(Cmol_No_Conv)
  CLEAR(IsComp_No_Conv)
  CLEAR(Cmol_Conv)
  CLEAR(IsComp_Conv)
  No_React[G_H2O] = true;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !pCompIn->IsComp[c] )
      continue;
    if ( No_React[c] )
      {
      IsComp_No_Conv[c] = true;
      Cmol_No_Conv[c] = pCompIn->Cmol[c];
      }
    else
      {
      IsComp_Conv[c] = true;
      Cmol_Conv[c] = pCompIn->Cmol[c];
      }
    }
  Flow_in = pFlow_in->Flow_mol;
  Flow_No_Conv = Flow_in * Norm ( IsComp_No_Conv, Cmol_No_Conv );
  Flow_Conv = Flow_in * Norm ( IsComp_Conv, Cmol_Conv );
  if ( Flow_Conv < 1. )
    return;
// Переводим в группы
  //( "С1-С4" );
  //( "С5-90" );
  //( "90-170" );
  //( "170-350" );
  //( "350-550" );
  //( "550+" );
  double M[6][6];
  double V[6];
  CLEAR(M)
  CLEAR(V)
  for ( int i = 0; i < 6; i++ )
    {
    for ( int j = i; j < 6; j++ )
      {
      double mij = 0;
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( pComp[i]->IsComp[c] && pComp[j]->IsComp[c] )
          {
          mij += pComp[i]->Cmol[c] * pComp[j]->Cmol[c];
          }
        }
      M[i][j] = mij;
      M[j][i] = mij;
      double vi = 0;
      for ( int c = 0; c < LAST_GAS; c++ )
        {
        if ( pComp[i]->IsComp[c] && IsComp_Conv[c] )
          {
          vi += pComp[i]->Cmol[c] * Cmol_Conv[c];
          }
        }
      V[i] = vi;
      }
    }
// Мольные доли
  ASS( LinSolv ( 6, (double*)M, V, Cin ));
  Cin[6] = 0;
  double S = 0.;
  for ( int i = 0; i < 6; i++ )
    {
    if ( Cin[i] < 0. )
      Cin[i] = 0.;
    S += Cin[i];
    }
  if ( S < 1e-3 )
    {
    Flow_Conv_kg = 0.;
    return;
    }
  S = 1. / S;
  for ( int i = 0; i < 6; i++ )
    Cin[i] *= S;
// Массовые доли
  Mw_in = 0.; 
  for ( int i = 0; i < 6; i++ )
    {
    Cin[i] *= MwComp[i];
    Mw_in += Cin[i];
    }
  S = 1. / Mw_in;
  for ( int i = 0; i < 6; i++ )
    Cin[i] *= S;
  Cin[6] = 0;
  Flow_Conv_kg = Flow_Conv * Mw_in;
  }

void CVBR::Convert( )
  {
//
  _C1_4_на_выходе = Cout[0] * 100.;
  _C5_T90_на_выходе = Cout[1] * 100.;
  _T90_T170_на_выходе = Cout[2] * 100.;
  _T170_T350_на_выходе = Cout[3] * 100.;
  _T350_T550_на_выходе = Cout[4] * 100.;
  _T550_на_выходе = Cout[5] * 100.;
  double Flow_t = Flow_Conv_kg * 1e-3;
  _C1_4_на_выходе_т = _C1_4_на_выходе * Flow_t;
  _C5_T90_на_выходе_т = _C5_T90_на_выходе * Flow_t;
  _T90_T170_на_выходе_т = _T90_T170_на_выходе * Flow_t;
  _T170_T350_на_выходе_т = _T170_T350_на_выходе * Flow_t;
  _T350_T550_на_выходе_т = _T350_T550_на_выходе * Flow_t;
  _T550_на_выходе_т = _T550_на_выходе * Flow_t;
  double S = 0.;
  double iMw_out = 0.; 
  for ( int i = 0; i < 6; i++ )
    {
    S += Cout[i];
    Cout[i] /= MwComp[i];
    iMw_out += Cout[i];
    }
  S += Cout[6];
  Cout[6] /= 12.;
  iMw_out += Cout[6];
// д.б сохранение массы
  ASS( fabs ( S - 1. ) < 1e-5 )
  Mw_react = 1. / iMw_out;
  for ( int i = 0; i < K_COMP; i++ )
    Cout[i] *= Mw_react;
  Flow_React_mol = Flow_Conv_kg / Mw_react;
// Компонуем состав
  CLEAR(IsComp_React)
  CLEAR(Cmol_React)
  for ( int i = 0; i < 6; i++ )
    {
    double C = Cout[i];
    CComp * pC = pComp[i];
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( pC->IsComp[c] )
        {
        IsComp_React[c] = true;
        Cmol_React[c] += C * pC->Cmol[c];
        }
      }
    }
  double D = Norm( IsComp_React, Cmol_React );
  double Mw_cmol = ::Mw( IsComp_React, Cmol_React );
  double T = Mw_cmol / Mw_react;
//
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp_React[c] )
      CmolOpt[c] = Cmol_React[c] * kComp[c];
    }
  Norm( IsComp_React, CmolOpt );
  Mw_conv = ::Mw( IsComp_React, CmolOpt );
  Flow_Conv_mol = Flow_Conv_kg / Mw_conv;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp_React[c] )
      {
      if ( CompEtalon.IsComp[c] && CompEtalon.Cmol[c] > 1e-9 )
        OutToEtalon[c] = CmolOpt[c] / CompEtalon.Cmol[c];
      else
        OutToEtalon[c] = -1.;
      }
    else
      OutToEtalon[c] = -1.;
    }
  if ( SetOptKoef )
    {
    SetOptKoef = false;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( OutToEtalon[c] > 0. )
        {
        kComp[c] /= OutToEtalon[c];
        }
      }
    }

// Сумма
  CLEAR(CmolSumm)
  CLEAR(IsCompSumm)
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp_No_Conv[c] )
      {
      IsCompSumm[c] = true;
      CmolSumm[c] = Cmol_No_Conv[c] * Flow_No_Conv; 
      }
    if ( IsComp_React[c] )
      {
      IsCompSumm[c] = true;
      CmolSumm[c] += CmolOpt[c] * Flow_Conv_mol; 
      }
    }
  int n = G_H2S;
  double T_out = Out_Реактор.T;
  double Alfa = 0.;
  if ( T_out > T_H2S_100 )
    Alfa = 1.;
  else
    if ( T_out > T_H2S_0 )
      Alfa = ( T_out - T_H2S_0 ) / ( T_H2S_100 - T_H2S_0 );
  CmolSumm[n] = Alfa * C_H2S * 0.01 * Flow_Conv_mol; 
  IsCompSumm[n] = true;
  Flow_Summ_mol = Norm( IsCompSumm, CmolSumm );
  Mw_summ = ::Mw( IsCompSumm, CmolSumm );
  pCompSumm->Set( IsCompSumm, CmolSumm );
  CComp * pCompIn = pFlow_in->Get_Comp();
  double Mw_in = Mw ( pCompIn );
//
  if ( Flow_in > 0.1 )
    k_Flow_IO = 0.9 * k_Flow_IO + 0.1 * Mw_in / Mw_summ;
  }

void CVBR::CalcT ( double dt )
  {
  Out_Реактор_0.PT( pFlow_out->P, In.T, pCompSumm );
  double H = Flow_Summ_mol * Out_Реактор_0.h;
  double dH = Flow_Conv * dQ_React * Q_React;
  H += dH;
  double h; 
  if ( Flow_Summ_mol > 0.1 )
    h = H / Flow_Summ_mol;
  else
    h = Out_Реактор_0.h;
const double Cp = 1000.;
  double Ksi = Cp * Tau / ( dt * 60. );
  Out_Реактор.PH_corp( pFlow_out->P, h, pCompSumm, Ksi );
  Tout = Out_Реактор.T;
  }
