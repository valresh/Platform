#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 


/// Статический режим

const double Delta_F = 1.;
// Статический выход газовый и жидкой фаз с учётом конденсации 
void CVolBase::Calc_Mix( double & Fout, double & Lout, double Summ_FS_Gas[K_GAS], double Summ_FS_Liq[K_GAS] )
  {
// Давление паров 
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double Ps;
      if ( Is_2F )
        Ps = StateVol.Henry[c] * Gamma_mix[c] * pGas[c]->Pz( T );
      else
        Ps = StateVol.Henry[c] * pGas[c]->Pz( T );
      if ( T + TK > pGas[c]->Tcr && Ps < 1.5 * P )
        Ps = 1.5 * P;
      P_sat[c] = Ps;
      }
    }
// Расчет входных потоков 
  double Gin = 0.;
  double Lin = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      Gin += Summ_FS_Gas[c];
      Lin += Summ_FS_Liq[c];
      }
    }
  double S = Gin + Lin;
  double Fo = Fout;
//  if ( Fo < 0. || Fo > S )
//    Fo = 0.5 * S;
  if ( S <= 0. )
    {
// Нет потоков
    Fout = 0.;
    Lout = 0.;
    return;
    }
//////////////////////////
  double dFout = 0.;
  double Fout_b = Fo;
  double Fout_e = Fo;
  double XY = 0.;
  Calc_dXY( Fo, S - Fo, Summ_FS_Gas, Summ_FS_Liq, &XY, NULL );
  double XYb = XY;
  double XYe = XY;
/// Сжатие интервала для Ньютона
  if ( XY > 0. )
    {
    while ( XY > 0. )
      {
      Fout_b = Fout_e;
      XYb = XYe;
      Fout_e += Delta_F;
      if ( Fout_e > 1e4 )//S )
        {
        Fout = Fout_b;
        Lout = S - Fout;
        Fout_e = S;
        Fout_b = Fout_e - Delta_F;
        break;
        }
      Calc_dXY( Fout_e, S - Fout_e, Summ_FS_Gas, Summ_FS_Liq, &XY, NULL );
      XYe = XY;
      }
    }
  else
    {
    while ( XY < 0. )
      {
      Fout_e = Fout_b;
      XYe = XYb;
      Fout_b -= Delta_F;
      if ( Fout_b < -1e4 ) //0. )
        {
        Fout = Fout_e;
        Lout = S - Fout;
        Fout_b = 0.;
        Fout_e = Delta_F;
        break;
        }
      Calc_dXY( Fout_b, S - Fout_b, Summ_FS_Gas, Summ_FS_Liq, &XY, NULL );
      XYb = XY;
      }
    }
  bool OK = true;
  double deltaFout = Fout_e - Fout_b;
  Fo = 0.5 * ( Fout_b + Fout_e );
  double dXY_dFo = 0.;
  int kIter = 0;
/// Итерации Ньютона
  while ( fabs ( deltaFout ) > 1e-6 )
    {
    Calc_dXY( Fo, S - Fo, Summ_FS_Gas, Summ_FS_Liq, &XY, &dXY_dFo );
    deltaFout = -XY / dXY_dFo;
    Fo += deltaFout;
    if ( ++kIter >= 10 )
      {
      OK = false;
      break;
      }
    }
  if ( OK )
    {
/// Ньютон сошелся
    Fout = Fo;
    Lout = S - Fo;
    return;
    }
/// Ньютон не сошелся - деление пополам
  double A, A1;
  deltaFout = Fout_e - Fout_b;
  Fo = 0.5 * ( Fout_b + Fout_e );
  void * F = Fopen ( "Trace.csv", "wt" );
  for ( int n = 0; n <= 100; n++ )
    {
    Fo = n * S * 0.01;
    Calc_dXY( Fo, S - Fo, Summ_FS_Gas, Summ_FS_Liq, &XY, NULL );
    Fprintf ( F, "%7.5lf;%7.5lf;\n", Fo, XY );
    }
  Fclose ( F );
  while ( fabs ( deltaFout ) > 1e-2 )
    {
    kIter++;
    if ( kIter > 5 )
      KKK();
    if ( fabs ( XYe - XYb ) > 1e-6 )
      {
      double idH = 1. / ( XYe - XYb );
      A = -XYb * idH;
      if ( A < 0. )
        A = 0.;
      if ( A > 1. )
        A = 1.;
      A1 = 1. - A;
      }
    else
      {
      A = 1.;
      A1 = 0.;
      }
    double Fout_new = A * Fout_e + A1 * Fout_b;
    if ( Fout_new < 0 || Fout_new > S )
      {
      Fout = Fo;
      Lout = S - Fo;
      return;
      }
    deltaFout = Fout_new - Fo;
    Fo = Fout_new;
    Calc_dXY( Fo, S - Fo, Summ_FS_Gas, Summ_FS_Liq, &XY, NULL );
    if ( XY < 0. )
      {
      Fout_e = Fo;
      XYe = XY;
      }
    else
      {
      Fout_b = Fo;
      XYb = XY;
      }
    }
  Fout = Fo;
  Lout = S - Fo;
  }

// Сумма X, Y и их производные
void CVolBase::Calc_dXY( double Fo, double Lo, double Summ_FS_Gas[K_GAS], double Summ_FS_Liq[K_GAS], double * F, double * dFdFo )
  {
  double Alfa = k_Cond * 1e3;
  double Sx = 0.;
  double Sy = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double N = 1. / ( Fo * Lo + ( Lo * P + Fo * P_sat[c] ) * Alfa );
      double Y = ( Summ_FS_Gas[c] * Lo + Alfa * P_sat[c] * ( Summ_FS_Liq[c] + Summ_FS_Gas[c] )) * N;
      double X = ( Summ_FS_Liq[c] * Fo + Alfa * P * ( Summ_FS_Liq[c] + Summ_FS_Gas[c] )) * N;
      Cmol_Gas[c] = Y;
      Cmol_Liq[c] = X;
      Sx += X;
      Sy += Y;
      }
    }
  *F = Sy - Sx;
  if ( dFdFo == NULL )
    return;
  double dY_Fo = 0.;
  double dY_Lo = 0.;
  double dX_Fo = 0.;
  double dX_Lo = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      double Zn = Fo * Lo + ( Lo * P + Fo * P_sat[c] ) * Alfa;
      double Zn_Fo = Lo + P_sat[c] * Alfa;
      double Zn_Lo = Fo + P * Alfa;
      double Ch_Y = ( Summ_FS_Gas[c] * Lo + Alfa * P_sat[c] * ( Summ_FS_Liq[c] + Summ_FS_Gas[c] ));
      double Ch_Y_Fo = 0.;
      double Ch_Y_Lo = Summ_FS_Gas[c];
      double Ch_X = ( Summ_FS_Liq[c] * Fo + Alfa * P * ( Summ_FS_Liq[c] + Summ_FS_Gas[c] ));
      double Ch_X_Fo = Summ_FS_Liq[c];
      double Ch_X_Lo = 0.;
      double N = 1. / ( Zn * Zn );
      dY_Fo += ( Ch_Y_Fo * Zn - Zn_Fo * Ch_Y ) * N;
      dY_Lo += ( Ch_Y_Lo * Zn - Zn_Lo * Ch_Y ) * N;
      dX_Fo += ( Ch_X_Fo * Zn - Zn_Fo * Ch_X ) * N;
      dX_Lo += ( Ch_X_Lo * Zn - Zn_Lo * Ch_X ) * N;
      }
    }
  *dFdFo = dY_Fo - dY_Lo - dX_Fo + dX_Lo;
  }

