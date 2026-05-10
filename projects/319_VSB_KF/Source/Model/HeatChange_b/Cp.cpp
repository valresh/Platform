#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"

#if 0
void CUniHeatChange::CalcTemp_Cp( double dt )
  {
  double h_in_Trub = pIn_Trub->h_Flow;
	S_Trub_In.PH( pIn_Trub->P, h_in_Trub, pComp_Trub );
	T_in_Trub = S_Trub_In.T;
//	double ks_Trub = kGas_Trub * S_Trub_In.E + 1. - S_Trub_In.E;
  double h_in_Vol = pIn_Vol->h_Flow;
  S_Vol_In.PH( pIn_Vol->P, h_in_Vol, pComp_Vol );
	T_in_Vol = S_Vol_In.T;
//
	if ( Cp_Trub == 0. )
		Cp_Trub = 10.;
	if ( Cp_Vol == 0. )
		Cp_Vol = 10.;
//
  int kIter = 0;
  double F_Vol = fabs ( pIn_Vol->Flow_mol );
  double F_Trub = fabs ( pIn_Trub->Flow_mol );
  double D0 = T_in_Trub - T_in_Vol;
  bool LimitQ = false;
  double Q_limit = 0.;
  while ( kIter++ < 20 )
    {
  //
    if ( LimitQ )
      {
      Q = Q_limit;
      h_out_Trub = S_Trub_In.h - Q / F_Trub;
      S_Trub_Out.PH( pOut_Trub->P, h_out_Trub, pComp_Trub );
      double dT_Trub = S_Trub_Out.T - T_in_Trub; 
      Cp_Trub = ( h_out_Trub - h_in_Trub ) / dT_Trub;
//
      h_out_Vol = S_Vol_In.h + Q / F_Vol;
      S_Vol_Out.PH( pOut_Vol->P, h_out_Vol, pComp_Vol );
      double dT_Vol = S_Vol_Out.T - T_in_Vol; 
      Cp_Vol = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      break;
      }
    else
      Q = Calc_Q( );
    h_out_Trub = S_Trub_In.h - Q / F_Trub;
    S_Trub_Out.PH( pOut_Trub->P, h_out_Trub, pComp_Trub );
    bool OK_Trub = true;
    if ( D0 > 0. )
      {
//труба охлаждается
      if ( S_Trub_Out.T < T_in_Vol )
        {
        double Qb = Q_prev;
        double Qe = Q;
        while ( Qe - Qb > 1e-1 )
          {
          kIter++;
          Q_limit = 0.5 * ( Qe + Qb );
          h_out_Trub = S_Trub_In.h - Q_limit / F_Trub;
          S_Trub_Out.PH( pOut_Trub->P, h_out_Trub, pComp_Trub );
          if ( S_Trub_Out.T < T_in_Vol + 1. )
            Qe = Q_limit;
          else
            Qb = Q_limit;
          }
        LimitQ = true;
        continue;
        }
      }
    else
      {
//труба нагревается
      if ( S_Trub_Out.T > T_in_Vol )
        {
        double Qb = Q;
        double Qe = Q_prev;
        while ( Qe - Qb > 1e-1 )
          {
          kIter++;
          Q_limit = 0.5 * ( Qe + Qb );
          h_out_Trub = S_Trub_In.h - Q_limit / F_Trub;
          S_Trub_Out.PH( pOut_Trub->P, h_out_Trub, pComp_Trub );
          if ( S_Trub_Out.T < T_in_Vol - 1. )
            Qe = Q_limit;
          else
            Qb = Q_limit;
          }
        LimitQ = true;
        continue;
        }
      }
    double D_Trub = 2.; 
    if ( OK_Trub )
      {
      double dT_Trub = S_Trub_Out.T - T_in_Trub; 
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      if ( Cp_Trub_new > Cp_Trub_max )
        Cp_Trub_new = Cp_Trub_max;
      D_Trub = Cp_Trub_new / Cp_Trub - 1.;
//      Cp_Trub = sqrt ( Cp_Trub * Cp_Trub_new );
      Cp_Trub = Cp_Trub_new;
      }
    else
      {
      Cp_Trub *= 2;
      if ( Cp_Trub > Cp_Vol_max )
        Cp_Trub = Cp_Vol_max;
      }
/////////////////////////////////////////////////////////
    h_out_Vol = S_Vol_In.h + Q / F_Vol;
    S_Vol_Out.PH( pOut_Vol->P, h_out_Vol, pComp_Vol );
    bool OK_Vol = true;
    double D_Vol = 2.; 
    if ( D0 > 0. )
      {
//Объем нагревается
      if ( S_Vol_Out.T > T_in_Trub )
        {
        double Qb = Q;
        double Qe = Q_prev;
        while ( Qe - Qb > 1e-1 )
          {
          kIter++;
          Q_limit = 0.5 * ( Qe + Qb );
          h_out_Vol = S_Vol_In.h + Q_limit / F_Vol;
          S_Vol_Out.PH( pOut_Vol->P, h_out_Vol, pComp_Vol );
          if ( S_Vol_Out.T < T_in_Trub - 1. )
            Qb = Q_limit;
          else
            Qe = Q_limit;
          }
        LimitQ = true;
        continue;
        }
      }
    else
      {
//Объем охлаждается
      if ( S_Vol_Out.T < T_in_Trub )
        {
        double Qb = Q;
        double Qe = Q_prev;
        while ( Qe - Qb > 1e-1 )
          {
          kIter++;
          Q_limit = 0.5 * ( Qe + Qb );
          h_out_Vol = S_Vol_In.h + Q_limit / F_Vol;
          S_Vol_Out.PH( pOut_Vol->P, h_out_Vol, pComp_Vol );
          if ( S_Vol_Out.T < T_in_Trub + 1. )
            Qb = Q_limit;
          else
            Qe = Q_limit;
          }
        LimitQ = true;
        continue;
        }
      }
    if ( OK_Vol )
      {
      double dT_Vol = S_Vol_Out.T - T_in_Vol; 
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      if ( Cp_Vol_new > Cp_Vol_max )
        Cp_Vol_new = Cp_Vol_max;
      D_Vol = Cp_Vol_new / Cp_Vol - 1.; 
//      Cp_Vol = sqrt ( Cp_Vol * Cp_Vol_new );
      Cp_Vol = Cp_Vol_new;
      }
    else
      {
      Cp_Vol *= 2.;
      if ( Cp_Vol > Cp_Vol_max )
        Cp_Vol = Cp_Vol_max;
      }
    Q_prev = Q;
/////////////////////////////////////////////////////////
    if ( fabs ( D_Trub ) < 1e-5 && fabs ( D_Vol ) < 1e-5 )
      break;
    }
  WasLimit = LimitQ;
  EpsQfin = EpsQ;
  double T1_Trub = ( S_Trub_Out.h - S_Trub_In.h ) + ( Cp_Trub * ( T_in_Trub - S_Trub_Out.T )); 
  double T1_Vol = ( S_Vol_Out.h - S_Vol_In.h ) + ( Cp_Vol * ( T_in_Vol - S_Vol_Out.T )); 
  double T = F_Trub * ( S_Trub_In.h - S_Trub_Out.h ) + F_Vol * ( S_Vol_In.h - S_Vol_Out.h );
  dT_Trub = S_Trub_Out.T - T_out_Trub;
  dT_Vol = S_Vol_Out.T - T_out_Vol;
  kIterQ = kIter;
  KKK();
//	Q_dg = fabs ( Q ) * 1e-6;
//	Q_cal = Q_dg * M_cal;
  }
#endif

double CUniHeatChange::Calc_Q( )
  {
  double F1 = fabs ( pIn_Trub->Flow_mol ) * Cp_Trub;
  double F2 = fabs ( pIn_Vol->Flow_mol ) * Cp_Vol;
  if ( Прямоток )
    {
    double Eps = exp ( -KS * ( 1. / F1 + 1. / F2 ));
    double Q = F1 * F2 * ( T_in_Trub - T_in_Vol ) * ( 1. - Eps ) / ( F1 + F2 );
    EpsQ = Eps;
    return Q;
    }
  if ( fabs ( F1 - F2 ) < 10. )
    {
    F1 = F2 + 10.;
    }
  if ( F1 > F2 )
    {
    double Ksi = exp ( KS * ( 1. / F1 - 1. / F2 ));
    double Q = F1 * F2 * ( T_in_Trub - T_in_Vol ) * ( 1. - Ksi ) / ( F1 - F2 * Ksi );
    EpsQ = Ksi;
    return Q;
    }
  else
    {
    double Eps = exp ( -KS * ( 1. / F1 - 1. / F2 ));
    EpsQ = Eps;
    double Q = F1 * F2 * ( T_in_Trub - T_in_Vol ) * ( 1. - Eps ) / ( F2 - F1 * Eps );
    return Q;
    }
  }

void CUniHeatChange::CalcTrubZ_Cp( double dt )
  {
  double F_vol = fabs ( pIn_Vol->Flow_mol );
  double F_trub = fabs ( pIn_Trub->Flow_mol );
  double h_vol_in = S_Vol_In.h;
  double h_vol_out = S_Vol_Out.h;
  double h_trub_in = S_Trub_In.h;
  double h_trub_out = S_Trub_Out.h;
  double T_vol_in = S_Vol_In.T;
  double T_vol_out = S_Vol_Out.T;
  double T_trub_in = S_Trub_In.T;
  double T_trub_out = S_Trub_Out.T;
  if ( !TestTemp ( T_vol_in, T_trub_in, T_vol_out, T_trub_out ))
    KKK();
  if ( T_Trub_Z > T_trub_in )
    {
// Нагрев от vol
    if ( T_Trub_Z > T_vol_in - 1. )
      {
      WasLimit = true;
      T_Trub_Z = T_vol_in - 1.;
      }
    }
  else
    {
// Охлаждение от vol
    if ( T_Trub_Z < T_vol_in + 1. )
      {
      WasLimit = true;
      T_Trub_Z = T_vol_in + 1.;
      }
    }
  if ( S_Trub_In.T < T_Trub_Z )
    {
// Труба нагревается
    S_Trub_Out.PT ( pOut_Trub->P, T_Trub_Z, pOut_Trub->Get_Comp());
    h_trub_out = S_Trub_Out.h;
    Q = F_trub * ( h_trub_out - h_trub_in );
    h_vol_out = h_vol_in - Q / F_vol;
    S_Vol_Out.PH ( pOut_Vol->P, h_vol_out, pOut_Vol->Get_Comp());
    if ( S_Vol_Out.T < T_trub_in + 0.01 )
      {
      Err_Tz = true;
      WasLimit = true;
      }
    else
      {
      WasLimit = false;
      Err_Tz = false;
      double dh_Trub = S_Trub_Out.h - S_Trub_In.h;
      double dT_Trub = S_Trub_Out.T - S_Trub_In.T;
      Cp_Trub = dh_Trub / dT_Trub;
      if ( Cp_Trub > Cp_Trub_max )
        Cp_Trub = Cp_Trub_max;
      if ( Cp_Trub < Cp_Trub_min )
        Cp_Trub = Cp_Trub_min;
      if ( !_finite ( Cp_Trub ))
        Cp_Trub = 100.;
      double dh_Vol = S_Vol_Out.h - S_Vol_In.h;
      double dT_Vol = S_Vol_Out.T - S_Vol_In.T;
      Cp_Vol = dh_Vol / dT_Vol;
      if ( Cp_Vol > Cp_Vol_max )
        Cp_Vol = Cp_Vol_max;
      if ( Cp_Vol < Cp_Vol_min )
        Cp_Vol = Cp_Vol_min;
      if ( !_finite ( Cp_Vol ))
        Cp_Vol = 100.;
      if ( Прямоток )
        {
        double F1 = F_trub  * Cp_Trub;
        double F2 = F_vol  * Cp_Vol;
        double QT = -Calc_Q( );
        Q_Q = QT / Q;
        double Eps = 1. + (( F1 + F2 ) * Q ) /( F1 * F2 * ( T_trub_in - T_vol_in ));
        if ( Eps < 0.01 )
          Eps = 0.01;
        EpsQ = Eps;
        double KS_Cp = -log( Eps ) / ( 1. / F1 + 1. / F2 );
        KS_KS = KS_Cp / KS;
        }
      }
    }
  else
    {
// Труба охлаждается
    S_Trub_Out.PT ( pOut_Trub->P, T_Trub_Z, pOut_Trub->Get_Comp());
    h_trub_out = S_Trub_Out.h;
    Q = F_trub * ( h_trub_in - h_trub_out );
    double QT = Calc_Q( );
    Q_Q = QT / Q;
    h_vol_out = h_vol_in + Q / F_vol;
    S_Vol_Out.PH ( pOut_Vol->P, h_vol_out, pOut_Vol->Get_Comp());
    if ( S_Vol_Out.T > T_trub_in - 0.01 )
      {
      Err_Tz = true;
      WasLimit = true;
      }
    else
      {
      WasLimit = false;
      Err_Tz = false;
      double dh_Trub = S_Trub_Out.h - S_Trub_In.h;
      double dT_Trub = S_Trub_Out.T - S_Trub_In.T;
      Cp_Trub = dh_Trub / dT_Trub;
      if ( Cp_Trub > Cp_Trub_max )
        Cp_Trub = Cp_Trub_max;
      if ( Cp_Trub < Cp_Trub_min )
        Cp_Trub = Cp_Trub_min;
      if ( !_finite ( Cp_Trub ))
        Cp_Trub = 100.;
      double dh_Vol = S_Vol_Out.h - S_Vol_In.h;
      double dT_Vol = S_Vol_Out.T - S_Vol_In.T;
      Cp_Vol = dh_Vol / dT_Vol;
      if ( Cp_Vol > Cp_Vol_max )
        Cp_Vol = Cp_Vol_max;
      if ( Cp_Vol < Cp_Vol_min )
        Cp_Vol = Cp_Vol_min;
      if ( !_finite ( Cp_Vol ))
        Cp_Vol = 100.;
      if ( Прямоток )
        {
        double F1 = F_trub  * Cp_Trub;
        double F2 = F_vol  * Cp_Vol;
        double Eps = 1. - (( F1 + F2 ) * Q ) /( F1 * F2 * ( T_trub_in - T_vol_in ));
        if ( Eps < 0.01 )
          Eps = 0.01;
        EpsQ = Eps;
        double KS_Cp = -log( Eps ) / ( 1. / F1 + 1. / F2 );
        KS_KS = KS_Cp / KS;
        }
      }
    }
  EpsQfin = EpsQ;
  dT_Trub = S_Trub_Out.T - T_out_Trub;
  dT_Vol = S_Vol_Out.T - T_out_Vol;
  kIterQ = 1;
  }

void CUniHeatChange::CalcVolZ_Cp( double dt )
  {
  double F_vol = fabs ( pIn_Vol->Flow_mol );
  double F_trub = fabs ( pIn_Trub->Flow_mol );
  double h_vol_in = S_Vol_In.h;
  double h_vol_out = S_Vol_Out.h;
  double h_trub_in = S_Trub_In.h;
  double h_trub_out = S_Trub_Out.h;
  double T_vol_in = S_Vol_In.T;
  double T_vol_out = S_Vol_Out.T;
  double T_trub_in = S_Trub_In.T;
  double T_trub_out = S_Trub_Out.T;
  if ( !TestTemp ( T_vol_in, T_trub_in, T_vol_out, T_trub_out ))
    KKK();
  if ( T_Vol_Z > T_vol_in )
    {
// Нагрев от trub
    if ( T_Vol_Z > T_trub_in - 1. )
      {
      WasLimit = true;
      T_Vol_Z = T_trub_in - 1.;
      }
    }
  else
    {
// Охлаждение от vol
    if ( T_Vol_Z < T_trub_in + 1. )
      {
      WasLimit = true;
      T_Vol_Z = T_trub_in + 1.;
      }
    }
  if ( S_Vol_In.T < T_Vol_Z )
    {
// Объем нагревается
    S_Vol_Out.PT ( pOut_Vol->P, T_Vol_Z, pOut_Vol->Get_Comp());
    h_vol_out = S_Vol_Out.h;
    Q = F_vol * ( h_vol_out - h_vol_in );
    h_trub_out = h_trub_in - Q / F_trub;
    S_Trub_Out.PH ( pOut_Trub->P, h_trub_out, pOut_Trub->Get_Comp());
    if ( S_Trub_Out.T < T_vol_in + 0.01 )
      {
      Err_Tz = true;
      WasLimit = true;
      }
    else
      {
      WasLimit = false;
      Err_Tz = false;
      double dh_Vol = S_Vol_Out.h - S_Vol_In.h;
      double dT_Vol = S_Vol_Out.T - S_Vol_In.T;
      Cp_Vol = dh_Vol / dT_Vol;
      if ( Cp_Vol > Cp_Vol_max )
        Cp_Vol = Cp_Vol_max;
      if ( Cp_Vol < Cp_Vol_min )
        Cp_Vol = Cp_Vol_min;
      if ( !_finite ( Cp_Vol ))
        Cp_Vol = 100.;
      double dh_Trub = S_Trub_Out.h - S_Trub_In.h;
      double dT_Trub = S_Trub_Out.T - S_Trub_In.T;
      Cp_Trub = dh_Trub / dT_Trub;
      if ( Cp_Trub > Cp_Trub_max )
        Cp_Trub = Cp_Trub_max;
      double F1 = F_trub  * Cp_Vol;
      double F2 = F_vol  * Cp_Trub;
      if ( Прямоток )
        {
        double QT = Calc_Q( );
        Q_Q = QT / Q;
        double Eps = 1. - (( F1 + F2 ) * Q ) /( F1 * F2 * ( T_trub_in - T_vol_in ));
        if ( Eps < 0.01 )
          Eps = 0.01;
        EpsQ = Eps;
        double KS_Cp = -log( Eps ) / ( 1. / F1 + 1. / F2 );
        KS_KS = KS_Cp / KS;
        }
      else
        {
        double FF = F1 * F2 * ( T_trub_in - T_vol_in ); 
        double Eps = ( F2 * Q - FF ) / ( F1 * Q - FF );
        if ( Eps < 0.01 )
          Eps = 0.01;
        EpsQ = Eps;
        double KS_Cp = fabs ( log( Eps ) / ( 1. / F1 - 1. / F2 ));
        KS_KS = KS_Cp / KS;
        }
      }
    }
  else
    {
// Объем охлаждается
    S_Vol_Out.PT ( pOut_Vol->P, T_Vol_Z, pOut_Vol->Get_Comp());
    h_vol_out = S_Vol_Out.h;
    Q = F_vol * ( h_vol_in - h_vol_out );
    double QT = -Calc_Q( );
    Q_Q = QT / Q;
    h_trub_out = h_trub_in + Q / F_vol;
    S_Trub_Out.PH ( pOut_Trub->P, h_trub_out, pOut_Trub->Get_Comp());
    if ( S_Trub_Out.T > T_vol_in - 0.01 )
      {
      Err_Tz = true;
      WasLimit = true;
      }
    else
      {
      WasLimit = false;
      Err_Tz = false;
      double dh_Vol = S_Vol_Out.h - S_Vol_In.h;
      double dT_Vol = S_Vol_Out.T - S_Vol_In.T;
      Cp_Vol = dh_Vol / dT_Vol;
      if ( Cp_Vol > Cp_Vol_max )
        Cp_Vol = Cp_Vol_max;
      if ( Cp_Vol < Cp_Vol_min )
        Cp_Vol = Cp_Vol_min;
      if ( !_finite ( Cp_Vol ))
        Cp_Vol = 100.;
      double dh_Trub = S_Trub_Out.h - S_Trub_In.h;
      double dT_Trub = S_Trub_Out.T - S_Trub_In.T;
      Cp_Trub = dh_Trub / dT_Trub;
      if ( Cp_Trub > Cp_Trub_max )
        Cp_Trub = Cp_Trub_max;
      double F1 = F_trub  * Cp_Vol;
      double F2 = F_vol  * Cp_Trub;
      if ( Прямоток )
        {
        double Eps = 1. + (( F1 + F2 ) * Q ) /( F1 * F2 * ( T_trub_in - T_vol_in ));
        if ( Eps < 0.01 )
          Eps = 0.01;
        EpsQ = Eps;
        double KS_Cp = -log( Eps ) / ( 1. / F1 + 1. / F2 );
        KS_KS = KS_Cp / KS;
        }
      else
        {
        double FF = F1 * F2 * ( T_trub_in - T_vol_in ); 
        double Eps;
        if ( F2 > F1 )
          Eps = ( F1 * Q - FF ) / ( F2 * Q - FF );
        else
          Eps = ( F2 * Q - FF ) / ( F1 * Q - FF );
        if ( Eps < 0.01 )
          Eps = 0.01;
        EpsQ = Eps;
        double KS_Cp = fabs ( log( Eps ) / ( 1. / F1 - 1. / F2 ));
        KS_KS = KS_Cp / KS;
        }
      }
    }
  EpsQfin = EpsQ;
  dT_Vol = S_Vol_Out.T - T_out_Vol;
  dT_Trub = S_Trub_Out.T - T_out_Trub;
  kIterQ = 1;
  }
