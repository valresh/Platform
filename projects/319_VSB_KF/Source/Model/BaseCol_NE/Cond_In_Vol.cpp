#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "Extensions.h"


CStage_Static::CStage_Static()
  {

  }


bool CStage_NE::Cond_Static_F_dF( double S, double Eps, double Flow_L[], double Flow_V[], double & FM, double & dFdE )
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
    double Ai = Alfa_Comp[c] * Coef_A * Alfa_Stage * Alfa_Level;
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

//bool Test = false;

void CStage_NE::SetLevel_Static( double dt )
  {
//  if ( M_liq < 1e-3 )
//    return;
  // Уровень на тарелке - фиксирован
  if ( To_m3 <= 0. || To_m3 == 0.2 || *pCalcTom3 )
    {
    if ( M_liq > 0.1 )
      To_m3 = To_m3_liq ( T_liq, IsComp, X );
    else
      To_m3 = 0.2;
    if ( To_m3 <= 0. )
      To_m3 = 0.2;
    }
  if ( ЭтоКуб )
    {
    Level = 0.5;
    double h_liq = Level * h_st;
    double kH =  To_m3 / S_Stage;
    M_liq = h_liq / kH;
    return;
    }
  //===================================
  if ( Numb == 35 )
    KKK();
// L_Summ_out - в статике считается исходя из баланса
// L_Down - L_Summ_out - L_Karman
// M_liq по L_Down

  L_Down = L_Summ_out;
  for ( int s = 0; s < kSrc; s++ )
    {
    if ( Src[s]->pKarman )
      {
      L_Karman = Src[s]->pKarman->Static_Level ( L_Summ_out );
      L_Down -= L_Karman;
      }
    }
  if ( L_Down < 0. )
    L_Down = 0.;
  //===================================
  double h_liq = 0.;
  double kH = 0.;
  if ( h_st > 0. )
    {
    double kH =  To_m3 / S_Stage;
    double M0 = h_st / kH;
    M_liq = M0 + L_Down / k_L;
    h_liq = M_liq * kH;
    Level = h_liq / h_st;
    FINITE(M_liq)
    }
  else
    {
    M_liq = L_Down / k_L;
    Level = 0.;
    }
  FINITE(M_liq)
//  if ( Test )
//    CalcLevel( dt );
  }

double Flow_in_s;
void CStage_NE::CalcCond_Static( double dt )
  {
  double * Flow_L = Summ_In_Gas;
  double * Flow_V = Summ_In_Liq;
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
  Flow_in_s = Flow_in;
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
    double Flow_in = Norm(IsComp,Y);
    M_liq = 0.;
    Level = 0.;
    Vout = Flow_in;// - Flow_Gas_Out_Src;
    L_Summ_out = L_Down = 0.;
    return;
    }
  double PzG = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      P_z[c] = pGas[c]->Pz( T_liq ) * Henry[c] * Gamma[c];
      P_PzX[c] = P_z[c] * X[c];
      PzG += P_PzX[c];
      P_Y[c] = P * Y[c];
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
    if ( !Cond_Static_F_dF( Flow_in, Eps, Flow_L, Flow_V, F, dFdE ))
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
    Cond_XY_Static( Flow_in, Eps, Flow_L, Flow_V );
    return;
    }
  // Деление пополам
  double Eb = 0.;
  double Ee = 1.;
  while ( Ee - Eb > 1e-8 )
    {
    Eps = 0.5 * ( Ee + Eb );
    dFdE = -1.;
    ASS(Cond_Static_F_dF( Flow_in, Eps, Flow_L, Flow_V, F, dFdE ))
    if ( F > 0. )
      Ee = Eps;
    else
      Eb = Eps;
    }
  Static_Eps = Eps;
  Cond_XY_Static( Flow_in, Eps, Flow_L, Flow_V );
  }
//
double Xs[K_GAS];
double Ys[K_GAS];
double Vout_s, Lout_s;
void CStage_NE::Cond_XY_Static( double Flow_in, double Eps, double Flow_L[], double Flow_V[] )
  {
  Vout_s = Flow_in * Eps;
  Lout_s = Flow_in - Vout_s;
  //Vout -= Flow_Gas_Out_Src;
  //Lout -= Flow_Liq_Out_Src;
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
    double Ai = Alfa_Comp[c] * Coef_A * Alfa_Stage * Alfa_Level;
//    double Si = Flow_V[c] + Flow_L[c];
    double AiP = Ai * P;
    double AiPz = Ai * P_z[c];
    double Xi = ( AiP * Flow_V[c] + Flow_L[c] * ( AiP + Vout_s )) /
                ( Vout_s * Lout_s + Lout_s * AiP + Vout_s * AiPz );
    double Yi = ( AiPz * Xi + Flow_V[c]) /
                ( Vout_s + AiP );
    Xs[c] = Xi;
    Ys[c] = Yi;
    Sx += Xi;
    Sy += Yi;
    }
  if ( Numb != 1 && Numb != pCol->nCub )
    {
    Vout_s -= Flow_Gas_Out_Src;
    Lout_s -= Flow_Liq_Out_Src;
    }
  KKK();
  }
////////////////////////////////////////////////////////////////////////////////////
void CStage_NE::CalcCond_Static_Vol_XY( double V_out, double L_out, double * Flow_V, double * Flow_L )
  {
  Vout = V_out;
  double Lout = L_out;
  double Sx = 0., Sy = 0.;
  double dH = 0.;
  bool Print = false;
  if ( Numb == 1 )
    KKK();
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Ai = Alfa_Comp[c] * Alfa_Stage * Alfa_Level;
    double Zni_X = ( L_out + Ai * P_z[c] ) * V_out + L_out * Ai * P;
    double Chi_X = ( V_out + Ai * P ) * Flow_L[c] + Ai * P * Flow_V[c];
    double Xi = Chi_X / Zni_X;
    X[c] = Xi;
    FINITE(X[c])
    Sx += Xi;
//
    double Zni_Y = V_out + Ai * P;
    double Chi_Y = Flow_V[c] + Ai * P_z[c] * Xi;
    double Yi = Chi_Y / Zni_Y;
    Y[c] = Yi;
    FINITE(Y[c])
    Sy += Yi;
    double Vc_i = Ai * ( P * Yi - P_z[c] * Xi );
    dH += Vc_i * ( h_gas[c] - h_liq[c] );
    if ( Print )
      Fsend ( "T1.csv", "%7.5lf;%7.5lf;%7.5lf;%7.5lf;%7.5lf;\n",
        Xi,Yi,Vc_i,P_z[c],dH );
    }
  //double Tst_X[K_GAS];
  //double Tst_Y[K_GAS];
  //for ( int c = 0; c < LAST_GAS; c++ )
  //  {
  //  if ( !IsComp[c] )
  //    continue;
  //  Tst_X[c] = X[c] / Xs[c];
  //  Tst_Y[c] = Y[c] / Ys[c];
  //  }
  NoCond = false;
  if ( Sx < 0.95 )
    NoCond = true;
  if ( Sy < 0.95 )
    NoCond = true;
  if ( fabs ( Sx - 1. ) > 1e-7 && Sx > 1e-7 )
    {
    double N = 1. / Sx; 
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        X[c] *= N;
      }
    }
  if ( fabs ( Sy - 1. ) > 1e-7 && Sy > 1e-7 )
    {
    double N = 1. / Sy; 
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        Y[c] *= N;
      }
    }
  L_Summ_out = Lout;
//  L_Down = L_Summ_out;
  if ( Numb != 1 && Numb != pCol->nCub )
    {
    Vout -= Flow_Gas_Out_Src;
//    L_Down = L_Summ_out - Flow_Liq_Out_Src;
    }
  KKK();
  }

void CStage_NE::CalcCond_Static_Vol_Func( double V_out, double L_out, double * Flow_V, double * Flow_L, double * pSumm, double * pdSumm_dV )
  {
  double F = 0;
  double dF = 0;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Ai = Alfa_Comp[c] * Alfa_Stage * Alfa_Level;
    double Zni_X = ( L_out + Ai * P_z[c] ) * V_out + L_out * Ai * P;
    double Chi_X = ( V_out + Ai * P ) * Flow_L[c] + Ai * P * Flow_V[c];
    double Xi = Chi_X / Zni_X;
//
    double Zni_Y = V_out + Ai * P;
    double Chi_Y = Flow_V[c] + Ai * P_z[c] * Xi;
    double Yi = Chi_Y / Zni_Y;
    F += Xi - Yi;
    if ( pdSumm_dV )   // d / dV_out =  - d/dL_out
      {
      double dZni_X = -V_out + ( L_out + Ai * P_z[c] ) - Ai * P;
      double dChi_X = Flow_L[c];
      double dXi = ( dChi_X  * Zni_X - dZni_X * Chi_X )/ ( Zni_X * Zni_X );
      double dZni_Y = 1.;
      double dChi_Y = Ai * P_z[c] * dXi;
      double dYi = ( dChi_Y  * Zni_Y - dZni_Y * Chi_Y )/ ( Zni_Y * Zni_Y );
      dF += dXi - dYi;
      }
    }
  *pSumm = F;
  if ( pdSumm_dV )
    *pdSumm_dV = dF;
  }

void CStage_NE::CalcCond_Static_Vol( )
  {
// Конденсация в объеме
// Статика
// Вход:
// Flow_L = X_In
// Flow_V = Y_In
// Выход:
// Lout, Vout
// X, Y
//
  if ( Numb == 1 )
    KKK();
//  CalcCond_Static( 1e-4 );
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
    double Flow_gas_in = Norm(IsComp,Y);
    M_liq = 0.;
    Level = 0.;
    Vout = Flow_gas_in;//
    L_Summ_out = L_Down = 0.;
    return;
    }
// Инерации по Vout
  double PzG = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      P_z[c] = pGas[c]->Pz( T_liq ) * Henry[c] * Gamma[c];
      P_PzX[c] = P_z[c] * X[c];
      PzG += P_PzX[c];
      P_Y[c] = P * Y[c];
      }
    }
  if ( PzG > 1e-8 )
    P_Pz = P / PzG;
  int kIter = 0;
  bool OK = true;
  double F, dFdV;
  double V_out = Vout;
  while ( 1 )
    {
    if ( kIter++ > 20 )
      {
      OK = false;
      break;
      }
    CalcCond_Static_Vol_Func( V_out, Flow_in - V_out, Flow_V, Flow_L, &F, &dFdV );
    double dV = -F / dFdV;
    V_out += dV;
    if ( V_out < 0. || V_out > Flow_in )
      {
      OK = false;
      break;
      }
    if ( fabs ( F ) < 1e-8 )
      break;
    }
  if ( OK )
    {
    Vout = V_out;
    CalcCond_Static_Vol_XY ( V_out, Flow_in - V_out, Flow_V, Flow_L );
    if ( Vout < 0. )
      {
      L_Summ_out += Vout;
//      L_Down = L_Summ_out;
      Vout = 0.;
      L_Down = 0.;
      }
    return;
    }
  // Деление пополам
  double Vb = 0.;
  double Ve = Flow_in;
  F = 1.;
  kIter = 0;
  while ( fabs ( F ) > 1e-5 || Ve - Vb > 1e-5 )
    {
    V_out = 0.5 * ( Ve + Vb );
    CalcCond_Static_Vol_Func( V_out, Flow_in - V_out, Flow_V, Flow_L, &F, NULL );
    if ( F > 0. )
      Ve = V_out;
    else
      Vb = V_out;
    if ( kIter++ > 30 )
      break;
    }
  Vout = V_out;
  CalcCond_Static_Vol_XY ( V_out, Flow_in - V_out, Flow_V, Flow_L );
  if ( Vout < 0. )
    {
    L_Summ_out += Vout;
//    L_Down = L_Summ_out;
    Vout = 0.;
    }
//  L_Down = L_Summ_out;
// 
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Dynamic_Vol_Func_Params
  {
  double dt, Gout; 
  double V, L, dL;
  double * Vo_i, * Gin_i;
  double * Lo_i, * Lin_i;
//
  double * F;
  double * dFdG;
  };

void CStage_NE::CalcCond_Dynamic_Vol_XY( struct Dynamic_Vol_Func_Params * q )
  {
  double Sx = 0., Sy = 0.;
  double Vc = 0.;
  double dH = 0.;
  bool Print = false;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Xi, Yi;
    double dt = q->dt;
    double Ai; 
    if ( Alfa_Level < 1e-6 )
      {
      Ai = 0.;
      double a00 = q->L + dt * L_Summ_out; //Xi
      if ( fabs ( a00 ) > 1e-6 )
        {
        double b0 = q->Lo_i[c] + dt * q->Lin_i[c];
        Xi = b0 / a00;
        }
      else
        Xi = 0.;
      double a11 = q->V + dt * q->Gout; //Yi
      double b1 = q->Vo_i[c] + dt * q->Gin_i[c];
      if ( fabs ( a11 ) > 1e-6 )
        Yi = b1 / a11;
      else
        Yi = 0.;
      }
    else
      {
      Ai = Alfa_Comp[c] * Alfa_Stage * Alfa_Level;
      double a00 = q->L + dt * ( L_Summ_out + Ai * P_z[c] ); //Xi
      double a01 = -dt * Ai * P; // Yi
      double b0 = q->Lo_i[c] + dt * q->Lin_i[c];
      double a10 = -dt * Ai * P_z[c]; // Xi
      double a11 = q->V + dt * ( q->Gout + Ai * P ); //Yi
      double b1 = q->Vo_i[c] + dt * q->Gin_i[c];
      double det = a00 * a11 - a10 * a01;
      det = 1. / det;
      Xi = ( b0 * a11 - b1 * a01 ) * det;
      Yi = ( b1 * a00 - b0 * a10 ) * det;
      }
    if ( M_liq > 1e-5 || L_Summ_out > 1e-3 )
      {
      if ( Xi > 1. )
        Xi = 1.;
      if ( Xi < 0. )
        Xi = 0.;
      }
    else
      Xi = 0.;
    if ( Yi > 1. )
      Yi = 1.;
    if ( Yi < 0. )
      Yi = 0.;
    X[c] = Xi;
    Sx += Xi;
    Y[c] = Yi;
    Sy += Yi;
    double Vc_i = Ai * ( P * Yi - P_z[c] * Xi );
    Vc_Comp[c] += 0.1 * ( Vc_i - Vc_Comp[c] );
    Vc += Vc_Comp[c];
    double hc = Vc_Comp[c] * ( h_gas[c] - h_liq[c] ) * k_h_cond[c];
    h_cond[c] = hc;
    dH += hc;
//    if ( Print )
//      Fsend ( "T1.csv", "%7.5lf;%7.5lf;%7.5lf;%7.5lf;%7.5lf;\n",
//    Xi,Yi,Vc_i,P_z[c],dH );
    }
  V_Cond = Vc;
  //if ( Numb != 1 )
  //  Vout = q->Gout - FlowGasOut;
  //else
    Vout = q->Gout;
//  if ( strstr ( ObjName, "К-41") && Numb == 15 && Vout > 1. )
//    KKK();
  M_liq = q->L;
  if ( M_liq < 0. )
    M_liq = 0.;
  NoCond = false;
  if ( Sx < 0.95 )
    NoCond = true;
  if ( Sy < 0.95 )
    NoCond = true;
  if ( NoCond )
    dH_cond = 0.;
  else
    dH_cond = dH;
  if ( fabs ( Sx - 1. ) > 1e-7 && Sx > 1e-7 )
    {
    double N = 1. / Sx; 
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        X[c] *= N;
      }
    }
  if ( fabs ( Sy - 1. ) > 1e-7 && Sy > 1e-7 )
    {
    double N = 1. / Sy; 
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( IsComp[c] )
        Y[c] *= N;
      }
    }
  }

void CStage_NE::CalcCond_Dynamic_Vol_Func(  struct Dynamic_Vol_Func_Params * q )
  {
  double F = 0;
  double dF = 0;
  double Sx = 0.;
  double Sy = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    double Xi, Yi;
    double dt = q->dt;
    double Ai; 
    double a00, a01, b0;
    double a10, a11, b1; 
    double det;
    if ( Alfa_Level < 1e-6 )
      {
      Ai = 0.;
      double a00 = q->L + dt * L_Summ_out; //Xi
      if ( fabs ( a00 ) > 1e-9 )
        {
        double b0 = q->Lo_i[c] + dt * q->Lin_i[c];
        Xi = b0 / a00;
        }
      else
        Xi = 0.;
      double a11 = q->V + dt * q->Gout; //Yi
      double b1 = q->Vo_i[c] + dt * q->Gin_i[c];
      if ( fabs ( a11 ) > 1e-9 )
        Yi = b1 / a11;
      else
        Yi = 0.;
      }
    else
      {
      Ai = Alfa_Comp[c] * Alfa_Stage * Alfa_Level;
      a00 = q->L + dt * ( L_Summ_out + Ai * P_z[c] ); //Xi
      a01 = -dt * Ai * P; // Yi
      b0 = q->Lo_i[c] + dt * q->Lin_i[c];
      a10 = -dt * Ai * P_z[c]; // Xi
      a11 = q->V + dt * ( q->Gout + Ai * P ); //Yi
      b1 = q->Vo_i[c] + dt * q->Gin_i[c];
      det = a00 * a11 - a10 * a01;
      det = 1. / det;
      Xi = ( b0 * a11 - b1 * a01 ) * det;
      Yi = ( b1 * a00 - b0 * a10 ) * det;
      }
    if ( M_liq > 1e-5 )
      {
      if ( Xi > 1. )
        Xi = 1.;
      if ( Xi < 0. )
        Xi = 0.;
      }
    else
      Xi = 0.;
    if ( Yi > 1. )
      Yi = 1.;
    if ( Yi < 0. )
      Yi = 0.;
    X[c] = Xi;
    Sx += Xi;
    Y[c] = Yi;
    Sy += Yi;
    FINITE(Yi)
    double Vc_i = Ai * ( P * Yi - P_z[c] * Xi );
    F += Vc_i;
    if ( q->dFdG )
      {
      if ( Alfa_Level >= 1e-6 )
        {
        double da00 = q->dL;
        double da01 = 0.; 
        double da10 = 0.; 
        double da11 = dt;
        b0 = -da00 * Xi;
        b1 = -da11 * Yi;
        double dXi = ( b0 * a11 - b1 * a01 ) * det;
        double dYi = ( b1 * a00 - b0 * a10 ) * det;
        double dVc_i = Ai * ( P * dYi - P_z[c] * dXi );
        dF += dVc_i;
        }
      }
    }
  Sx;Sy;
  *(q->F) = F;
  if ( q->dFdG )
    *(q->dFdG) = dF;
  }

void CStage_NE::CalcCond_Dynamic_Vol( double dt )
  {
// Конденсация в объеме
// На входе Lin_i, Gin_i, M_liq, M_gas, L_Summ_out
// На входе Gout, M_liq, M_gas, X, Y
// Динамика
  double * Lin_i = Summ_In_Liq;
  double * Gin_i = Summ_In_Gas;
  double Lo_i[K_GAS];
  double Vo_i[K_GAS];
  double Lo = M_liq;
  double Vo = M_gas;
  double F;
  double dFdG;
  Dynamic_Vol_Func_Params Q;
  Q.dt = dt;
  Q.Lin_i = Lin_i;
  Q.Gin_i = Gin_i;
  Q.Lo_i = Lo_i;
  Q.Vo_i = Vo_i;
  Q.F = &F;
  Q.dFdG = &dFdG;
  double Lin = 0.;
  double Gin = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      Lin += Lin_i[c];
      Gin += Gin_i[c];
      Lo_i[c] = Lo * X[c];
      Vo_i[c] = Vo * Y[c];
      if ( fabs ( Y[c] ) > 1e5 )
        KKK();
      }
    }
  if ( Numb != 1 )
    Gin -= GasOutFromStage;
  //if ( fabs ( Gin ) < 1e-5 )
  //  KKK();
  //if ( fabs ( Lin ) < 1e-5 )
  //  KKK();
// Итерации по Gout
  double PzG = 0.;
  double Vc0 = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double Ai = Alfa_Comp[c] * Alfa_Stage * Alfa_Level;
      P_z[c] = pGas[c]->Pz( T_liq ) * Henry[c] * Gamma[c];
      P_PzX[c] = P_z[c] * X[c];
      PzG += P_PzX[c];
      P_Y[c] = P * Y[c];
      Vc0 += Ai * ( P_Y[c] - P_PzX[c] );
      }
    }
  if ( PzG > 0. )
    P_Pz = P / PzG;
//////////////////////////////////////////
  double Gout = Vout;
  //if ( fabs ( Gout ) > 1e5 )
  //  KKK();
//  double L_out_summ = Lout + L_Karman;
//  Lout_max = Lin;
  double dG = 1e10;
  bool OK = true;
  int kIter = 0;
  while ( fabs ( dG ) > 1e-8 )
    {
    Q.Gout = Gout;
    double Vc = Gin - Gout;
    double dVc = -1.;
    Q.L = Lo + dt * ( Lin - L_Summ_out + Vc );
    Q.dL = dt * dVc;
    Q.V = Vo;
    CalcCond_Dynamic_Vol_Func( &Q );
    double Func = F - Vc;
    dG = -Func / ( dFdG + 1. );
    Gout += dG;
    kIter++;
    if ( Gout < 0. || kIter > 10 )
      {
      OK = false;
      break;
      }
    }
  //if ( fabs ( Gout ) > 1e5 )
  //  KKK();
  if ( OK )
    {
    Flow_Liq_Stage = Lin + Gin - Gout;
    CalcCond_Dynamic_Vol_XY( &Q );
    return;
    }
  // Деление пополам
  double Gb = 0.;
  double Ge = Lin + Gin;//2. * Gin;  //10000.;//
  //if ( fabs ( Ge ) > 1e5 )
  //  KKK();
  Q.dFdG = NULL;
  kIter = 0;
  double Func, Vc;
  while ( Ge - Gb > 1e-4 && kIter++ < 40 )
    {
    Gout = 0.5 * ( Gb + Ge );
    Q.Gout = Gout;
    Vc = Gin - Gout;
    double dVc = -1.;
    Q.L = Lo + dt * ( Lin - L_Summ_out + Vc );
    Q.dL = dt * dVc;
    Q.V = Vo;
    CalcCond_Dynamic_Vol_Func( &Q );
    Func = F - Vc;
    if ( Func == 0. )
      break;
    if ( Func > 0. )
      Ge = Gout;
    else
      Gb = Gout;
    }
  if ( kIter >= 35 )
    KKK();
//--  ASS(kIter<35)
  Gout = 0.5 * ( Gb + Ge );
  //if ( fabs ( Gout ) > 1e5 )
  //  KKK();
  Q.Gout = Gout;
  Vc = Gin - Gout;
  Q.L = Lo + dt * ( Lin - L_Summ_out + Vc );
  Q.V = Vo;
  Vout = Gout;
  if ( Numb == 35 )
    KKK();
  Flow_Liq_Stage = Lin + Gin - Gout;
  CalcCond_Dynamic_Vol_XY( &Q );
  }
