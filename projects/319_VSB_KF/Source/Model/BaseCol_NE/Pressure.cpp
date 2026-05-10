#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "math.h"
#include "Err.h"
#include "SetData.h"


#if 0
void CStage_NE::Static_Press( )
  {
  if ( StopSrc )
    {
    BREAK
    if ( StopSrc > 0 )
      StopSrc--;
    }
  if ( StopSrc )
    {
    BREAK
    }
	CLEAR(Summ_In_Gas)
  CLEAR(Summ_In_Liq)
  Flow_Summ_In = 0.;
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
      if ( Numb != 1 && pSrc->Gas )
        GasOut -= F;
      Flow_Summ_In += F;
      continue;
      }
    Flow_Summ_In += F;
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
        Summ_In_Liq[c] += F_liq * CmolLiq[c];
        H_Summ_Gas_In_A[c] -= F_gas * CmolGas[c];
        H_Summ_Liq_In_A[c] -= F_liq * CmolLiq[c];
        H_Summ_Gas_In_B[c] += F_gas * CmolGas[c] * pSrc->h_Src_Gas[c];
        H_Summ_Liq_In_B[c] += F_liq * CmolLiq[c] * pSrc->h_Src_Liq[c];
				}
			}
		}
///////////////////////////////////////////////////
  if ( StopSrc )
    {
    BREAK
    }
// Сверху
	if ( pUp && pUp->Lout > 0. )
		{
		double F = pUp->Lout;
		double * uX = pUp -> X;
		double * uh_Liq = pUp -> h_liq;
    Flow_Summ_In += F;
		for ( int c = 0; c < LAST_GAS; c++ )
			{
			if ( IsComp[c] )
				{
				Summ_In_Liq[c] += F * uX[c];
        H_Summ_Liq_In_A[c] -= F * uX[c];
        H_Summ_Liq_In_B[c] += F * uX[c] * uh_Liq[c];
				}
			}
		}
  if ( StopSrc )
    {
    BREAK
    }
///////////////////////////////////////////////////
// Снизу
	if ( pDown )
		{
		double F = pDown->Vout;
		double * dY = pDown -> Y;
		double * dh_Gas = pDown -> h_gas;
    Flow_Summ_In += F;
		for ( int c = 0; c < LAST_GAS; c++ )
			{
			if ( IsComp[c] )
        {
				Summ_In_Gas[c] += F * dY[c];
        FINITE(Summ_In[c])
        H_Summ_Gas_In_A[c] -= F * dY[c] * h_gas[c];
        H_Summ_Gas_In_B[c] += F * dY[c] * dh_Gas[c];
        }
			}
 		}
  if ( StopSrc )
    {
    BREAK
    }
  if ( pCol->CalcStageBalance )
    {
    if ( pCol->n_Comp_Balance >= 0 )
      ConstStageBalance = SummStageBalance = Summ_In[pCol->n_Comp_Balance];
    else
      ConstStageBalance = SummStageBalance = Flow_Summ_In;
    }
  if ( Flow_Summ_In < 0.01 )
    {
// Нет подачи
    L_In = 0.;
    V_Cond = 0.;
    Vout = 0.;
    CLEAR(X_In)
    CLEAR(Y_In)
    return;
    }
  Norm ( IsComp, Summ_In );
  if ( StopSrc )
    {
    BREAK
    }
//
  double RT = RP * ( T_gas + TK );
  M_gas = P * Vol_Stage / RT;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      X_In[c] = Summ_In_Liq[c];
      Y_In[c] = Summ_In_Gas[c];
      }
    }
///
  switch ( pCol->ModelCond )
    {
    case 1:
      CalcCond_Sloj( );
    break;
    case 0:
    default:
      CalcCond_Press( );
    break;
    }

///
  if ( StopSrc )
    BREAK
  double Vc = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double Vco = Vc_Comp[c];
      double Alfa = Alfa_Comp[c] * Alfa_Stage;
      Vc_Comp_C[c] = Alfa * P * Y[c];
      Vc_Comp_Pz[c] = Alfa * X[c];
      Vc_Comp[c] = Vc_Comp_C[c] - Vc_Comp_Pz[c] * P_z[c];
      double dVc = Vc_Comp[c] - Vco;
      Vc += Vc_Comp[c];
      }
    }
  }

double CStage_NE::CalcCond_Y( double F0, double * A, double * B, double * Y, double * Y_in )
  {
  double F = F0;
  double S, dSdF;
  double dF = 1e10;
  while ( fabs ( dF ) > 1e-8 )
    {
    S = -1.;
    dSdF = 0.;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        if ( fabs ( F + B[c] ) < 1e-8 )
          continue;
        double Z = 1. / ( F + B[c] );
        double W = A[c] * Z;
        S += W;
        dSdF -= W * Z;
        FINITE(Z)
        }
      }
    dF = -S / dSdF;
    F += dF;
    FINITE(F)
    if ( F <= 0. )
      {
      F = 0.;
      break;
      }
    }
  double Sy = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      if ( fabs ( F + B[c] ) < 1e-8 )
        {
        Y[c] = Y_in[c];
        Sy += Y[c];
        continue;
        }
      double Z = 1. / ( F + B[c] );
      Y[c] = A[c] * Z;
      FINITE(Y[c])
      Sy += Y[c];
      }
    }
  if ( fabs ( Sy - 1. ) > 1e-6 )
    {
    double N = 1. / Sy;
    FINITE(N)
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        FINITE(Y[c])
        Y[c] *= N;
        }
      }
    }
  return F;
  }

const int k_X = 50;
const double dx = 1. / k_X;

// Через слой
void CStage_NE::CalcCond_Sloj( double F_in, double Y_Gas[], double T,
                               double & F_out, double & Rcond_summ, double & Vcond_summ, double Y_out[], double V_cond[] )
  {
  double A[K_GAS];
  double B[K_GAS];
  double P_in = P;
  if ( pDown )
    P_in = pDown->P;
  double k_Alfa = 0.01;
  double Ye[K_GAS];
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      P_z[c] = pGas[c]->Pz( T ) * Henry[c] * Gamma[c];;
      Ye[c] = P_z[c] * X[c];
      }
    }
  Norm(IsComp,Ye);
  memmove ( Y_out, Y_Gas, LAST_GAS << 3 );
  double F = F_in;
  double dP = ( P - P_in ) / k_X;
  for ( int n = 0; n <= k_X; n++ )
    {
    double Px = P_in + n * dP;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        double Alfa = k_Alfa * Alfa_Comp[c] * Alfa_Stage;
        A[c] = F_in * Y_out[c] + Alfa * dx * P_z[c] * X[c];
        B[c] = Alfa * dx * Px;
        }
      }
    F = CalcCond_Y( F, A, B, Y_out, Y_Gas );
    if ( F <= 0. )
      {
      F = 0.;
      break;
      }
    }
  F_out = F;
  double V_Cond = 0.;
  double R_Cond = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      V_cond[c] = F_in * Y_Gas[c] - F_out * Y_out[c];
      V_Cond += V_cond[c];
      R_Cond += V_cond[c] * pGas[c]->H_vap( T );
      }
    }
  Vcond_summ = V_Cond;
  double Tst = F_out - ( F_in - V_Cond );
  KKK();
  Rcond_summ = R_Cond;
  double M[K_GAS];
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      M[c] = ( Y_out[c] - Y_Gas[c] )/( Ye[c] - Y_Gas[c] );
      }
    }
  KKK();
/*
  double H = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double h = pGas[c]->H_IG( T );
      H += H_Summ_Gas_In_A[c] * h + H_Summ_Gas_In_B[c];
      h = pGas[c]->H_l( T );
      H += H_Summ_Liq_In_A[c] * h + H_Summ_Liq_In_B[c];
      double V = Y_In[c] - F * Y_out[c];
      H += V * pGas[c]->H_vap( T_liq );
      }
    }
  H_out = H;
*/
  }



void CStage_NE::CalcCond_Press( )
  {
  double * Flow_L = X_In;
  double * Flow_V = Y_In;
  double Summ_L_in = 0.;
  double Summ_V_in = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      Summ_L_in += Flow_L[c];
      Summ_V_in += Flow_V[c];
      }
    }
  double Flow_in = Summ_L_in + Summ_V_in; // = const
  if ( Summ_L_in < 1e-8 )
    {
    // Газ
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        X[c] = 0.;
        Y[c] = Flow_V[c];
        Vc_Comp[c] = 0.;
        }
      }
    V_Cond = 0.;
    dH_cond = 0.;
    Flow_in = Norm(IsComp,Y);
    M_liq = 0.;
    return;
    }
  double PzG = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      P_z[c] = pGas[c]->Pz( T_liq ) * Henry[c];
      PzG += P_z[c] * X[c] * Gamma[c];
      }
    }
  P_Pz = P / PzG;
  int kIter = 0;
  bool OK = true;
  double F, dFdE;
  double Eps = Static_Eps;
  while ( 1 )
    {
    if ( kIter++ > 20 )
      {
      OK = false;
      break;
      }
    dFdE = 1.;
    if ( !Cond_Press_F_dF( Flow_in, Eps, Flow_L, Flow_V, F, dFdE ))
      {
      OK = false;
      break;
      }
    double dE = -F / dFdE;
    Eps += dE;
    if ( Eps < 0. || Eps > 1. )
      {
      OK = false;
      break;
      }
    if ( fabs ( F ) < 1e-8 )
      break;
    }
  if ( OK )
    {
    Static_Eps = Eps;
    Cond_XY_Press( Flow_in, Eps, Flow_L, Flow_V );
    return;
    }
  // Деление пополам
  double Eb = 0.;
  double Ee = 1.;
  while ( Ee - Eb > 1e-8 )
    {
    Eps = 0.5 * ( Ee + Eb );
    dFdE = -1.;
    ASS(Cond_Press_F_dF( Flow_in, Eps, Flow_L, Flow_V, F, dFdE ))
    if ( F > 0. )
      Ee = Eps;
    else
      Eb = Eps;
    }
  Static_Eps = Eps;
  Cond_XY_Press( Flow_in, Eps, Flow_L, Flow_V );
  }

void CStage_NE::Cond_XY_Press( double Flow_in, double Eps, double Flow_L[], double Flow_V[] )
  {
  Vout = Flow_in * Eps;
  Lout = Flow_in - Vout;
  double Coef_A = 1.;
// L, V - известны 
// V - из P
// L - исходя из Lout
  double Sx = 0;
  double Sy = 0;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Ai = Alfa_Comp[c] * Coef_A * Alfa_Stage;
    double Si = Flow_V[c] + Flow_L[c];
    double AiP = Ai * P;
    double AiPz = Ai * P_z[c];
    double Xi = ( AiP * Flow_V[c] + Flow_L[c] * ( AiP + Vout )) /
                ( Vout * Lout + Lout * AiP + Vout * AiPz );
    double Yi = ( AiPz * Xi + Flow_V[c]) /
                ( Vout + AiP );
    X[c] = Xi;
    Y[c] = Yi;
    Sx += Xi;
    Sy += Yi;
    }
  KKK();
  }

bool CStage_NE::Cond_Press_F_dF( double S, double Eps, double Flow_L[], double Flow_V[], double & FM, double & dFdE )
  {
  double F = 0;
  double dF = 0;
  double Coef_A = 1.;
  double L = S * ( 1. - Eps );
  bool CalcdF = dFdE > 0.;
//  if ( L < 1. )
//   S = L;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Ai = Alfa_Comp[c] * Coef_A * Alfa_Stage;
    double Si = Flow_V[c] + Flow_L[c];
    double AiPPz = Ai * ( P - P_z[c] );
    double Chi =  Si * S * Eps + Si * AiPPz - Flow_V[c] * S;;
    double Zni =  -S * S * Eps * Eps + ( S * S - S * AiPPz ) * Eps + S * Ai * P;
    F += Chi / Zni;
    if ( CalcdF )
      {
      double dChi =  Si * S;
      double dZni =  -2. * S * S * Eps + S * S - S * AiPPz;
      double dFc = ( dChi * Zni - dZni * Chi ) / ( Zni * Zni );
      dF += dFc;
      }
    }
  FM = F;
  dFdE = dF;
  return true;
  }
#endif
