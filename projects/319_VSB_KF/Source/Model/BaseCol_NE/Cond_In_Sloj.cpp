#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"

// Конденсация в слое 

bool CCond_Sloj::Cond_Press_F_dF( double S, double Eps, double Flow_L[], double Flow_V[], double & FM, double & dFdE )
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
    double Ai = Alfa_Comp[c] * Coef_A * *pAlfa_Stage;
    if ( Ai == 0. )
      Ai = 1e-3;
/*
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
*/
    double AiPPz = Ai * ( P - P_z[c] );
    double Chi =  ( S * ( Eps - 1.) + AiPPz ) * Flow_V[c] + ( S * Eps + AiPPz ) * Flow_L[c];
    double Zni =  (( -S * Eps + S - AiPPz ) * Eps + P * Ai );
    F += Chi / Zni;
    if ( CalcdF )
      {
      double Si = Flow_V[c] + Flow_L[c];
      double dChi =  Si * S;
      double dZni =  -2. * S * Eps + S - AiPPz;
      double dFc = ( dChi * Zni - dZni * Chi ) / ( Zni * Zni );
      dF += dFc;
      }
    }
  FM = F;
  dFdE = dF;
  return true;
  }

void CCond_Sloj::Cond_XY_Sloj( double Flow_Sloj_io, double Eps, double Summ_V_in, double Flow_L[], double Flow_D[], double Flow_V[] )
  {
  double Vout_From_Sloj = Flow_Sloj_io * Eps;
  Vout = Vout_From_Sloj;// + Summ_V_in;
  Lout = ( Flow_Sloj_io - Vout_From_Sloj );
  double Coef_A = 1.;
  // Слой
  if ( StopSloj )
    {
    BREAK
    }
  double Sx = 0;
  double Sy = 0;
  double Yeq[K_GAS];
  double Yin[K_GAS];
  double Px = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Ai = Alfa_Comp[c] * Coef_A * *pAlfa_Stage;
    if ( Ai == 0. )
      Ai = 1e-3;
    double Si = Flow_D[c] + Flow_L[c];
    double AiP = Ai * P;
    double AiPz = Ai * P_z[c];
    double Xi = ( AiP * Flow_D[c] + Flow_L[c] * ( AiP + Vout )) / ( Vout * Lout + Lout * AiP + Vout * AiPz );
    double Yi = ( AiPz * Xi + Flow_D[c]) / ( Vout + AiP );
    FINITE(Yi)
    X[c] = Xi;
    Ysloj[c] = Yi;
    Sx += Xi;
    Sy += Yi;
    Yeq[c] = P_z[c] * X[c];
    Px += Yeq[c];
    Yin[c] = Flow_D[c];
    }
  if ( fabs ( Sx - 1 ) > 1e-5 )
    KKK();
  double PP = P / Px;
  Norm ( IsComp, Yeq );
  Norm ( IsComp, Yin );
  if ( *SpecStop1 )
    KKK();
  if ( StopSloj )
    {
    BREAK
    }
  double Vc = 0.;
  double dH = 0.;
//  double Test[K_GAS];
//  double MM[K_GAS];
//  double Syy = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double Flow_Cond = Flow_D[c] - Vout_From_Sloj * Ysloj[c];
      V_Cond[c] = Flow_Cond;
      Vc += Flow_Cond;
      double hc = Flow_Cond * pGas[c]->H_vap( T_liq ) * k_h_cond[c];
      h_cond[c] = hc;
      dH += hc; 
//      Test[c] = P * Ysloj[c] - P_z[c] * X[c];
//      Syy += Ysloj[c];
//      MM[c] = ( Ysloj[c] - Yin[c] ) / ( Yeq[c] - Yin[c] );
      }
    }
  V_cond = Vc;
  dH_cond = dH;
  if ( *SpecStop1 )
    KKK();
  if ( StopSloj )
    {
    BREAK
    }
  // Добавление газа от источников
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    Y[c] = Vout_From_Sloj * Ysloj[c] + Flow_V[c];
    FINITE(Y[c])
    }
//  double T = Norm( IsComp, Y ) - Vout;
  Vout = Norm(IsComp,Y);
//  Vout += Summ_V_in;
  }


void CCond_Sloj::CalcCond_Sloj( )
  {
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
  double Flow_Down = 0.;
  double * Y_Down = NULL;
  if ( pDown )
    {
    Flow_Down = pDown->Vout;
    Y_Down = pDown->Y;
    }
  if ( StopSloj )
    {
    BREAK
    }
  if ( Flow_Down < 0. )
    Flow_Down = 0.;
  if ( Summ_L_in < 1e-8 )
    {
    // Газ
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        X[c] = 0.;
        Y[c] = Flow_V[c];
        if ( Y_Down )
          Y[c] += Flow_Down * Y_Down[c];
        V_Cond[c] = 0.;
        }
      }
    V_cond = 0.;
    dH_cond = 0.;
    double Flow_out = Norm( IsComp, Y );
    Vout = Flow_out;// - Flow_Gas_Out_Src;
    Lout = 0.;
    if ( StopSloj )
      {
      BREAK
      }
    return;
    }
/////////////////////////
  double Flow_D[K_GAS];
  if ( Y_Down )
    {
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        Flow_D[c] = Flow_Down * Y_Down[c];
      }
    }
  else
    {
    memset ( Flow_D, 0, LAST_GAS << 3 );
    }
  if ( StopSloj )
    {
    BREAK
    }
//////////////////////////////////////////////////
  double PzG = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      P_z[c] = pGas[c]->Pz( T_liq ) * Henry[c] * Gamma[c];
      PzG += P_z[c] * X[c];
      //P_PzX[c] = P_z[c] * X[c];
      //PzG += P_PzX[c];
      //P_Y[c] = P * Y[c];
      }
    }
  int kIter = 0;
  bool OK = true;
  double F, dFdE;
  double Eps = *pEps;
  double Flow_Summ = Flow_Down + Summ_L_in;
  if ( fabs ( Flow_Summ ) > 100000.)
    KKK();
  while ( 1 )
    {
    if ( kIter++ > 20 )
      {
      OK = false;
      break;
      }
    dFdE = 1.;
    if ( !Cond_Press_F_dF( Flow_Summ, Eps, Flow_L, Flow_D, F, dFdE ))
      {
      OK = false;
      break;
      }
    //double F1, dFdE1 = 1.;
    //if ( !Cond_Press_F_dF( Flow_Summ, Eps+0.0001, Flow_L, Flow_D, F1, dFdE1 ))
    //  {
    //  OK = false;
    //  break;
    //  }
    //double dF = ( F1 - F ) * 10000.;
    //double T = 2. * dF / ( dFdE + dFdE1 );
    double dE = -F / dFdE;
    Eps += dE;
    if ( !_finite(Eps))
      {
      OK = false;
      break;
      }
    if ( *SpecStop1 )
      KKK();
    if ( Eps < 0. || Eps > 1. )
      {
      OK = false;
      break;
      }
    if ( fabs ( F ) < 1e-8 )
      break;
    }
  if ( StopSloj )
    {
    BREAK
    }
  if ( *SpecStop1 )
    KKK();
  if ( OK )
    {
    *pEps = Eps;
    Cond_XY_Sloj( Flow_Summ, Eps, Summ_V_in, Flow_L, Flow_D, Flow_V );
    return;
    }
  // Деление пополам
  double Eb = 0.;
  double Ee = Eps * 2.;
  if ( !_finite ( Ee ) || Ee > 1. || Ee < 0. || Ee <= Eb )
    Ee = 1.;
  while ( Ee - Eb > 1e-8 )
    {
    Eps = 0.5 * ( Ee + Eb );
    dFdE = -1.;
    ASS(Cond_Press_F_dF( Flow_Summ, Eps, Flow_L, Flow_D, F, dFdE ))
      if ( F > 0. )
        Ee = Eps;
      else
        Eb = Eps;
    }
  *pEps = Eps;
  Cond_XY_Sloj( Flow_Summ, Eps, Summ_V_in, Flow_L, Flow_D, Flow_V );
  if ( StopSloj )
    {
    BREAK
    }
  }

double CStage_NE::CalcStaticHeat( double T )
  {
  if ( StopSloj )
    {
    BREAK
    }
  Sloj0.h_cond = h_cond;
  Sloj0.k_h_cond = k_h_cond;
  Sloj0.StopSloj = StopSloj;
  Sloj0.P = P;
  Sloj0.T_liq = T;
  Sloj0.Flow_L = X_In;
  Sloj0.Flow_V = Y_In;
  Sloj0.CalcCond_Sloj( );
  double H_out_0 = Sloj0.dH_cond;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double h = pGas[c]->H_IG( Sloj0.T_liq );
      H_out_0 += H_Summ_Gas_In_A[c] * h + H_Summ_Gas_In_B[c];
      h = pGas[c]->H_l( Sloj0.T_liq );
      H_out_0 += H_Summ_Liq_In_A[c] * h + H_Summ_Liq_In_B[c];
      }
    }
  return H_out_0;
  }

const double Delta_T = 1.;

void CStage_NE::IterStaticHeat( )
  {
  if ( StopSloj )
    {
    BREAK
    }
  if ( SpecStop1 )
    {
    KKK();
    }
  double Tb = T_liq;
  double Te = T_liq;
  double F = CalcStaticHeat( T_liq );
  double Fb = F;
  double Fe = F;
  if ( F > 0. )
    {
    while ( F > 0. )
      {
      Tb = Te;
      Fb = Fe;
      Te += Delta_T;
      if ( Te > T_max )
        {
        T_liq = Tb;
        return;
        }
      F = CalcStaticHeat( Te );
      Fe = F;
      }
    }
  else
    {
    while ( F < 0. )
      {
      Te = Tb;
      Fe = Fb;
      Tb -= Delta_T;
      if ( Tb < T_min )
        {
        T_liq = Te;
        return;
        }
      F = CalcStaticHeat( Tb );;
      Fb = F;
      }
    }
  if ( Fe >= Fb )
    {
    double T = 0.5 * ( Tb + Te );
    T_liq = T;
    return;
    }
	int kIter = 0;
	double A, A1;
  double deltaT = Te - Tb;
  double T = 0.5 * ( Tb + Te );
  FINITE(T)
	while ( fabs ( deltaT ) > 1e-4 && Fb - Fe > 1e-3 )
		{
		kIter++;
		if ( kIter > 5 )
			KKK();
		double idH = 1. / ( Fe - Fb );
		A = -Fb * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
    if ( T_new < T_min || T_new > T_max )
      {
      T_liq = T;
      return;
      }
		deltaT = T_new - T;
		T = T_new;
    F = CalcStaticHeat( T );
		if ( F < 0. )
			{
			Te = T;
			Fe = F;
			}
		else
			{
			Tb = T;
			Fb = F;
			}
		}
  FINITE(T)
  T_liq = T;
  if ( SpecStop1 )
    {
    KKK();
    }
  if ( StopSloj )
    {
    BREAK
    }
  }

void CStage_NE::CalcCond_Sloj( )
  {
  if ( StopSloj )
    {
    BREAK
    if ( StopSloj > 0 )
      StopSloj--;
    }
  if ( pCol->Static_Iter_T )
    {
    double T0 = T_liq;
    IterStaticHeat();
    double T1 = T_liq;
    double dT = T1 - T0;
    double adT = fabs ( dT );
    pCol->dT_abs_med += adT;
    if ( pCol->dT_abs_max < adT )
      pCol->dT_abs_max = adT;
    }
  else
    {
    double H_out_0 = CalcStaticHeat( T_liq );
    double dT = LimdX ( pCol->k_SlowT * H_out_0 * 1e-7, 1. );
    T_liq += dT;
    double adT = fabs ( dT );
    pCol->dT_abs_med += adT;
    if ( pCol->dT_abs_max < adT )
      pCol->dT_abs_max = adT;
    }
  T_gas = T_liq;
  if ( Numb != 1 && !ЭтоКуб )
    {
    Sloj0.Vout -= Flow_Gas_Out_Src;
    Sloj0.Lout -= Flow_Liq_Out_Src;
    if (Sloj0.Vout < 0. )
      Sloj0.Vout = 0.;
    if (Sloj0.Lout < 0. )
      Sloj0.Lout = 0.;
    }
///
////////////////////////////////////
  double Yeq[K_GAS];
  double Px = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      Yeq[c] = Sloj0.P_z[c] * Sloj0.X[c];
      Px += Yeq[c];
      }
    }
  double iPx = 1. / Px;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      Yeq[c] *= iPx;
    }
  P_Pz = P / Px;
  if ( pDown )
    {
    double * Y_Down = pDown->Y;
    double Ms = 0.;
    double MM[K_GAS];
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        {
        double M = ( Sloj0.Ysloj[c] - Y_Down[c] )/( Yeq[c] - Y_Down[c] );
        MM[c] = M;
        Ms += Y_Down[c] * M;
        }
      }
    Мерфри = Ms * 100.;
    }
  else
    Мерфри = 100.;

  V_Cond = Sloj0.V_cond;
  memmove ( X, Sloj0.X, LAST_GAS << 3 );
  memmove ( Y, Sloj0.Y, LAST_GAS << 3 );
  memmove ( Vc_Comp, Sloj0.V_Cond, LAST_GAS << 3 );
  dH_cond = Sloj0.dH_cond;
  L_Summ_out = Sloj0.Lout;
  Vout = Sloj0.Vout;
  if ( StopSloj )
    {
    BREAK
    }
  //////////////////////////////////////////////
  }

