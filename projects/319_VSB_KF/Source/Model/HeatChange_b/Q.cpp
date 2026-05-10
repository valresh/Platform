#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"


bool CUniHeatChange::Calc_QQ( double Q, double & Q_Cp )
  {
  SET_BP BreakPoint;
  double F_Vol = fabs ( pIn_Vol->Flow_mol );
  double F_Trub = fabs ( pIn_Trub->Flow_mol );
  double h_in_Trub = pIn_Trub->h_Flow;
  double h_in_Vol = pIn_Vol->h_Flow;
  double D0 = T_in_Trub - T_in_Vol;
  if ( D0 > 0 )
    {
// От трубы к объему
    h_out_Trub = S_Trub_In.h - Q / F_Trub;
    S_Trub_Out.PH( pIn_Trub->P, h_out_Trub, pComp_Trub );
    if ( S_Trub_Out.T < T_in_Vol - dTdop )
      return false;
    h_out_Vol = S_Vol_In.h + Q / F_Vol;
    S_Vol_Out.PH( pIn_Vol->P, h_out_Vol, pComp_Vol );
    if ( S_Vol_Out.T > T_in_Trub + dTdop )
      return false;
// Мы в пределах
// Пересчет Q через Cp
    double dT_Trub = S_Trub_Out.T - T_in_Trub; 
	  double Cp_Trub_new;
	  if( fabs ( dT_Trub ) > 0.001 )
		  Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
	  else
		  Cp_Trub_new = 100.;
    Cp_Trub += 0.1 * ( Cp_Trub_new - Cp_Trub );
    double dT_Vol = S_Vol_Out.T - T_in_Vol; 
    if( fabs ( dT_Vol ) > 0.001 )
      {
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      Cp_Vol += 0.1 * ( Cp_Vol_new - Cp_Vol );
      }
    else
      Cp_Vol += 0.1 * ( 100. - Cp_Vol );
    if ( Cp_Vol > Cp_Vol_max )
      Cp_Vol = Cp_Vol_max;
    if ( Cp_Vol < Cp_Vol_min )
      Cp_Vol = Cp_Vol_min;
    if ( !_finite ( Cp_Vol ))
      Cp_Vol = 100.;
    if ( Cp_Trub > Cp_Trub_max )
      Cp_Trub = Cp_Trub_max;
    if ( Cp_Trub < Cp_Trub_min )
      Cp_Trub = Cp_Trub_min;
    if ( !_finite ( Cp_Trub ))
      Cp_Trub = 100.;
    Q_Cp = Calc_Q( );
    }
  else
    {
// От объема к трубе
    h_out_Trub = S_Trub_In.h + Q / F_Trub;
    S_Trub_Out.PH( pIn_Trub->P, h_out_Trub, pComp_Trub );
    if ( S_Trub_Out.T > T_in_Vol + dTdop )
      return false;
    h_out_Vol = S_Vol_In.h - Q / F_Vol;
    S_Vol_Out.PH( pIn_Vol->P, h_out_Vol, pComp_Vol );
    if ( S_Vol_Out.T < T_in_Trub - dTdop )
      return false;
// Мы в пределах
// Пересчет Q через Cp
    double dT_Trub = S_Trub_Out.T - T_in_Trub; 
    if ( fabs ( dT_Trub ) > 0.001 )
      {
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      Cp_Trub += 0.1 * ( Cp_Trub_new - Cp_Trub );
      }
 	  else
      {
      Cp_Trub += 0.1 * ( 100. - Cp_Trub );
      }
    double dT_Vol = S_Vol_Out.T - T_in_Vol;    
	  if( fabs ( dT_Vol ) > 0.001 )
	    {
		  double Cp_Vol_new;
		  Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
		  Cp_Vol += 0.1 * ( Cp_Vol_new - Cp_Vol );
	    }
 	  else
		  Cp_Vol += 0.1 * ( 100. - Cp_Vol );
    if ( Cp_Vol > Cp_Vol_max )
      Cp_Vol = Cp_Vol_max;
    if ( Cp_Vol < Cp_Vol_min )
      Cp_Vol = Cp_Vol_min;
    if ( !_finite ( Cp_Vol ))
      Cp_Vol = 100.;
    if ( Cp_Trub > Cp_Trub_max )
      Cp_Trub = Cp_Trub_max;
    if ( Cp_Trub < Cp_Trub_min )
      Cp_Trub = Cp_Trub_min;
    if ( !_finite ( Cp_Trub ))
      Cp_Trub = 100.;
   Q_Cp = -Calc_Q( );
    }
  if ( EpsQ < 0.001 )
    KS_Large = true;
  else
    KS_Large = false;
//  if ( strstr ( ObjName, "T-31" ))
//    KKK();
  return true;
  }

void CUniHeatChange::Calc_Q_Trub_Z( )
  {
  SET_BP BreakPoint;
  double F_Vol = fabs ( pIn_Vol->Flow_mol );
  double F_Trub = fabs ( pIn_Trub->Flow_mol );
  double h_in_Trub = pIn_Trub->h_Flow;
  double h_in_Vol = pIn_Vol->h_Flow;
  double D0 = T_in_Trub - T_in_Vol;
  if ( T_Trub_Z > T_in_Trub )
    {
// Нагрев от vol
    if ( T_Trub_Z > T_in_Vol )
      T_Trub_Z = T_in_Vol;
    S_Trub_Out.PT ( pOut_Trub->P, T_Trub_Z, pOut_Trub->Get_Comp());
    h_out_Trub = S_Trub_Out.h;
    Q = F_Trub * ( h_out_Trub - h_in_Trub );
    h_out_Vol = h_in_Vol - Q / F_Vol;
    S_Vol_Out.PH ( pOut_Vol->P, h_out_Vol, pOut_Vol->Get_Comp());
    if ( S_Vol_Out.T < T_in_Trub + 0.01 )
      {
      Err_Tz = true;
      S_Vol_Out.PT ( pOut_Vol->P, T_in_Trub, pOut_Vol->Get_Comp());
      h_out_Vol = S_Vol_Out.h;
      }
    double dT_Trub = S_Trub_Out.T - T_in_Trub; 
    if ( fabs ( dT_Trub ) > 1. )
      {
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      Cp_Trub = Cp_Trub_new;
      }
    double dT_Vol = S_Vol_Out.T - T_in_Vol;
    if ( fabs ( dT_Vol ) > 1. )
      {
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      Cp_Vol = Cp_Vol_new;
      }
    if ( Cp_Vol > Cp_Vol_max )
      Cp_Vol = Cp_Vol_max;
    if ( Cp_Vol < Cp_Vol_min )
      Cp_Vol = Cp_Vol_min;
    if ( !_finite ( Cp_Vol ))
      Cp_Vol = 100.;
    if ( Cp_Trub > Cp_Trub_max )
      Cp_Trub = Cp_Trub_max;
    if ( Cp_Trub < Cp_Trub_min )
      Cp_Trub = Cp_Trub_min;
    if ( !_finite ( Cp_Trub ))
      Cp_Trub = 100.;
    if ( !Err_Tz )
      {
      double Q_Cp = Calc_Q( );
      if ( Q_Cp > 0.1 )
        {
        double K = Q / Q_Cp;
        KS *= LimdX ( K - 1., 0.1 ) + 1.;
        }
      }
    }
  else
    {
// Охлаждение от vol
    if ( T_Trub_Z < T_in_Vol )
      T_Trub_Z = T_in_Vol;
    S_Trub_Out.PT ( pOut_Trub->P, T_Trub_Z, pOut_Trub->Get_Comp());
    h_out_Trub = S_Trub_Out.h;
    Q = F_Trub * ( h_in_Trub - h_out_Trub );
    h_out_Vol = h_in_Vol + Q / F_Vol;
    S_Vol_Out.PH ( pOut_Vol->P, h_out_Vol, pOut_Vol->Get_Comp());
    if ( S_Vol_Out.T > T_in_Trub + 0.01 )
      {
      Err_Tz = true;
      S_Vol_Out.PT ( pOut_Vol->P, T_in_Trub, pOut_Vol->Get_Comp());
      h_out_Vol = S_Vol_Out.h;
      }
    double dT_Trub = S_Trub_Out.T - T_in_Trub; 
    if ( fabs ( dT_Trub ) > 1. )
      {
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      Cp_Trub = Cp_Trub_new;
      }
    double dT_Vol = S_Vol_Out.T - T_in_Vol; 
    if ( fabs ( dT_Vol ) > 1. )
      {
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      Cp_Vol = Cp_Vol_new;
      }
    if ( Cp_Vol > Cp_Vol_max )
      Cp_Vol = Cp_Vol_max;
    if ( Cp_Vol < Cp_Vol_min )
      Cp_Vol = Cp_Vol_min;
    if ( !_finite ( Cp_Vol ))
      Cp_Vol = 100.;
    if ( Cp_Trub > Cp_Trub_max )
      Cp_Trub = Cp_Trub_max;
    if ( Cp_Trub < Cp_Trub_min )
      Cp_Trub = Cp_Trub_min;
    if ( !_finite ( Cp_Trub ))
      Cp_Trub = 100.;
    if ( !Err_Tz )
      {
      double Q_Cp = Calc_Q( );
      if ( Q_Cp > 0.1 )
        {
        double K = Q / Q_Cp;
        KS *= LimdX ( K - 1., 0.1 ) + 1.;
        }
      }
    }
//  double dQ_trub = (( S_Trub_Out.h - S_Trub_In.h ) + ( Cp_Trub * ( T_in_Trub - S_Trub_Out.T ))) / ( S_Trub_Out.h - S_Trub_In.h ); 
//  double dQ_vol = (( S_Vol_Out.h - S_Vol_In.h ) + ( Cp_Vol * ( T_in_Vol - S_Vol_Out.T ))) / ( S_Vol_Out.h - S_Vol_In.h ); 
  if((S_Vol_In.h - S_Vol_Out.h))
	  _dH = ( F_Trub * ( S_Trub_In.h - S_Trub_Out.h ) + F_Vol * ( S_Vol_In.h - S_Vol_Out.h )) / (F_Vol * ( S_Vol_In.h - S_Vol_Out.h ));
  if ( EpsQ < 0.001 )
    KS_Large = true;
  else
    KS_Large = false;
  }

void CUniHeatChange::Calc_Q_Vol_Z( )
  {
  SET_BP BreakPoint;
  double F_Trub = fabs ( pIn_Trub->Flow_mol );
  double F_Vol = fabs ( pIn_Vol->Flow_mol );
  double h_in_Vol = pIn_Vol->h_Flow;
  double h_in_Trub = pIn_Trub->h_Flow;
  double D0 = T_in_Vol - T_in_Trub;
  if ( T_Vol_Z > T_in_Vol )
    {
// Нагрев от труб
    if ( T_Vol_Z > T_in_Trub )
      T_Vol_Z = T_in_Trub;
    S_Vol_Out.PT ( pOut_Vol->P, T_Vol_Z, pOut_Vol->Get_Comp());
    h_out_Vol = S_Vol_Out.h;
    Q = F_Vol * ( h_out_Vol - h_in_Vol );
    h_out_Trub = h_in_Trub - Q / F_Trub;
    S_Trub_Out.PH ( pOut_Trub->P, h_out_Trub, pOut_Trub->Get_Comp());
    if ( S_Trub_Out.T < T_in_Vol + 0.01 )
      {
      Err_Tz = true;
      S_Trub_Out.PT ( pOut_Trub->P, T_in_Vol, pOut_Trub->Get_Comp());
      h_out_Trub = S_Trub_Out.h;
      }
    double dT_Vol = S_Vol_Out.T - T_in_Vol; 
    if ( fabs ( dT_Vol ) > 1. )
      {
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      Cp_Vol = Cp_Vol_new;
      }
    double dT_Trub = S_Trub_Out.T - T_in_Trub; 
    if ( fabs ( dT_Trub ) > 1. )
      {
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      Cp_Trub = Cp_Trub_new;
      }
    if ( Cp_Vol > Cp_Vol_max )
      Cp_Vol = Cp_Vol_max;
    if ( Cp_Vol < Cp_Vol_min )
      Cp_Vol = Cp_Vol_min;
    if ( !_finite ( Cp_Vol ))
      Cp_Vol = 100.;
    if ( Cp_Trub > Cp_Trub_max )
      Cp_Trub = Cp_Trub_max;
    if ( Cp_Trub < Cp_Trub_min )
      Cp_Trub = Cp_Trub_min;
    if ( !_finite ( Cp_Trub ))
      Cp_Trub = 100.;
    if ( !Err_Tz )
      {
      double Q_Cp = Calc_Q( );
      if ( Q_Cp > 0.1 )
        {
        double K = Q / Q_Cp;
        KS *= LimdX ( K - 1., 0.1 ) + 1.;
        }
      }
    }
  else
    {
// Охлаждение от труб
    if ( T_Vol_Z < T_in_Trub )
      T_Vol_Z = T_in_Trub;
    S_Vol_Out.PT ( pOut_Vol->P, T_Vol_Z, pOut_Vol->Get_Comp());
    h_out_Vol = S_Vol_Out.h;
    Q = F_Vol * ( h_in_Vol - h_out_Vol );
    h_out_Trub = h_in_Trub + Q / F_Trub;
    S_Trub_Out.PH ( pOut_Trub->P, h_out_Trub, pOut_Trub->Get_Comp());
    if ( S_Trub_Out.T > T_in_Vol + 0.01 )
      {
      Err_Tz = true;
      S_Trub_Out.PT ( pOut_Trub->P, T_in_Vol, pOut_Trub->Get_Comp());
      h_out_Trub = S_Trub_Out.h;
      }
    double dT_Vol = S_Vol_Out.T - T_in_Vol; 
    if ( fabs ( dT_Vol ) > 1. )
      {
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      Cp_Vol = Cp_Vol_new;
      }
    double dT_Trub = S_Trub_Out.T - T_in_Trub; 
    if ( fabs ( dT_Trub ) > 1. )
      {
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      Cp_Trub = Cp_Trub_new;
      }
    if ( Cp_Vol > Cp_Vol_max )
      Cp_Vol = Cp_Vol_max;
    if ( Cp_Vol < Cp_Vol_min )
      Cp_Vol = Cp_Vol_min;
    if ( !_finite ( Cp_Vol ))
      Cp_Vol = 100.;
    if ( Cp_Trub > Cp_Trub_max )
      Cp_Trub = Cp_Trub_max;
    if ( Cp_Trub < Cp_Trub_min )
      Cp_Trub = Cp_Trub_min;
    if ( !_finite ( Cp_Trub ))
      Cp_Trub = 100.;
    if ( !Err_Tz )
      {
      double Q_Cp = Calc_Q( );
      if ( Q_Cp != 0.0 )
        {
        double K = Q / fabs(Q_Cp);
        KS *= LimdX ( K - 1., 0.1 ) + 1.;
        }
      }
    }
//  double dQ_trub = (( S_Vol_Out.h - S_Vol_In.h ) + ( Cp_Vol * ( T_in_Vol - S_Vol_Out.T ))) / ( S_Vol_Out.h - S_Vol_In.h ); 
//  double dQ_vol = (( S_Trub_Out.h - S_Trub_In.h ) + ( Cp_Trub * ( T_in_Trub - S_Trub_Out.T ))) / ( S_Trub_Out.h - S_Trub_In.h ); 
  _dH = ( F_Vol * ( S_Vol_In.h - S_Vol_Out.h ) + F_Trub * ( S_Trub_In.h - S_Trub_Out.h )) / (F_Trub * ( S_Trub_In.h - S_Trub_Out.h ));
  if ( EpsQ < 0.001 )
    KS_Large = true;
  else
    KS_Large = false;
  }

void CUniHeatChange::CalcTemp_Q( double dt )
  {
  SET_BP BreakPoint;
  if ( strstr ( ObjName, "EA-403"))
    KKK();

//  if ( strstr ( ObjName, "ОГ-301(Т/О)"))
//    KKK();
  int kIter = 0;
  if ( Q_prev == 0. || !_finite(Q_prev))
    Q_prev = 100000.;
  double Q_Cp = Q_prev;
  double Q = Q_prev;
//  double dQ = 1000.;
  double M_Qm = 0.99;
  double M_Qp = 1.01;
  bool Res = Calc_QQ( Q, Q_Cp );
  while ( !Res && kIter < 50 )
    {
    kIter++;
//    Q -= dQ;
    Q *= M_Qm;
    Res = Calc_QQ( Q, Q_Cp );
    if ( kIter > 5 )
      M_Qm = 0.9;
    }
  if ( kIter > 5 )
    KKK();
#if 0
  SET_BP BreakPoint;
  double Qe = Q_prev;
  double Qb = Q_prev;
  kIter = 0;
  //if ( strstr ( ObjName, "Т-102" ))
  //  KKK();
  if ( Q > Q_Cp )
    {
    SET_BP BreakPoint;
    while ( Q > Q_Cp )
      {
      kIter++;
      Qe = Q;
//      Q -= dQ;
      Q *= M_Qm;
      Qb = Q;
      if ( Qb < 0 )
        break;
      Res = Calc_QQ( Q, Q_Cp );
      ASS(Res)
      if ( kIter > 5 )
        M_Qm = 0.9;
      //if ( kIter > 5 )
      //  dQ = 1000.;
      //if ( kIter > 10 )
      //  dQ = 10000.;
      //if ( kIter > 15 )
      //  dQ = 100000.;
      }
    SET_BP BreakPoint;
    }
  else
    {
    SET_BP BreakPoint;
    kIter = 0;
    while ( Q < Q_Cp )
      {
      kIter++;
      Qb = Q;
//      Q += dQ;
      Q *= M_Qp;
      Qe = Q;
      Res = Calc_QQ( Q, Q_Cp );
      if ( !Res )
        break;
      if ( kIter > 5 )
        M_Qp = 1.1;
      //if ( kIter > 5 )
      //  dQ = 1000.;
      //if ( kIter > 10 )
      //  dQ = 10000.;
      //if ( kIter > 15 )
      //  dQ = 100000.;
      }
    }
  SET_BP BreakPoint;
  //if ( strstr ( ObjName, "Т-102" ))
  //  KKK();
  Q = 0.5 * ( Qb + Qe );
  while ( Qe - Qb > 1. && kIter++ < 100 )
    {
    Q = 0.5 * ( Qb + Qe );
    bool Res = Calc_QQ( Q, Q_Cp );
    if ( !Res )
      {
      Qe = Q;
      continue;
      }
    if ( Q < Q_Cp )
      Qb = Q;
    else
      Qe = Q;
    }

#endif
  SET_BP BreakPoint;
  Q = Q_Cp;
  Q_prev += kQ * ( Q -  Q_prev );
  WasLimit = false;
  EpsQfin = EpsQ;
  double F_Vol = fabs ( pIn_Vol->Flow_mol );
  double F_Trub = fabs ( pIn_Trub->Flow_mol );
//  dQ_trub = (( S_Trub_Out.h - S_Trub_In.h ) + ( Cp_Trub * ( T_in_Trub - S_Trub_Out.T ))) / ( S_Trub_Out.h - S_Trub_In.h ); 
//  dQ_vol = (( S_Vol_Out.h - S_Vol_In.h ) + ( Cp_Vol * ( T_in_Vol - S_Vol_Out.T ))) / ( S_Vol_Out.h - S_Vol_In.h ); 
  if(S_Vol_In.h - S_Vol_Out.h != 0.0)
	  _dH = ( F_Trub * ( S_Trub_In.h - S_Trub_Out.h ) + F_Vol * ( S_Vol_In.h - S_Vol_Out.h )) / (F_Vol * ( S_Vol_In.h - S_Vol_Out.h ));
  else
	 _dH = 0.0;
  dT_Trub = S_Trub_Out.T - T_out_Trub;
  dT_Vol = S_Vol_Out.T - T_out_Vol;
  kIterQ = kIter;
/*
  double h_in_Trub = pIn_Trub->h_Flow;
	S_Trub_In.PH( pIn_Trub->P, h_in_Trub, pComp_Trub );
	T_in_Trub = S_Trub_In.T;
//	double ks_Trub = kGas_Trub * S_Trub_In.E + 1. - S_Trub_In.E;
  double h_in_Vol = pIn_Vol->h_Flow;
  S_Vol_In.PH( pIn_Vol->P, h_in_Vol, pComp_Vol );
	T_in_Vol = S_Vol_In.T;
//
  double F_Vol = fabs ( pIn_Vol->Flow_mol );
  double F_Trub = fabs ( pIn_Trub->Flow_mol );
  double D0 = T_in_Trub - T_in_Vol;
  double Qb = 0.;
  double Qe = 1e9;    //Q > 0
  int kIter = 0;
  while ( Qe - Qb > 1. && kIter++ < 40 )
    {
    Q = 0.5 * ( Qb + Qe );
    if ( D0 > 0 )
      {
// От трубы к объему
      h_out_Trub = S_Trub_In.h - Q / F_Trub;
      S_Trub_Out.PH( pOut_Trub->P, h_out_Trub, pComp_Trub );
      if ( S_Trub_Out.T < T_in_Vol )
        {
        Qe = Q;
        continue;
        }
      h_out_Vol = S_Vol_In.h + Q / F_Vol;
      S_Vol_Out.PH( pOut_Vol->P, h_out_Vol, pComp_Vol );
      if ( S_Vol_Out.T > T_in_Trub )
        {
        Qe = Q;
        continue;
        }
// Мы в пределах
// Пересчет Q через Cp
      double dT_Trub = S_Trub_Out.T - T_in_Trub; 
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      Cp_Trub = Cp_Trub_new;
      double dT_Vol = S_Vol_Out.T - T_in_Vol; 
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      Cp_Vol = Cp_Vol_new;
      double Q_Cp = Calc_Q( );
      if ( Q < Q_Cp )
        Qb = Q;
      else
        Qe = Q;
      }
    else
      {
// От объема к трубе
      h_out_Trub = S_Trub_In.h + Q / F_Trub;
      S_Trub_Out.PH( pOut_Trub->P, h_out_Trub, pComp_Trub );
      if ( S_Trub_Out.T > T_in_Vol )
        {
        Qe = Q;
        continue;
        }
      h_out_Vol = S_Vol_In.h - Q / F_Vol;
      S_Vol_Out.PH( pOut_Vol->P, h_out_Vol, pComp_Vol );
      if ( S_Vol_Out.T < T_in_Trub )
        {
        Qe = Q;
        continue;
        }
// Мы в пределах
// Пересчет Q через Cp
      double dT_Trub = S_Trub_Out.T - T_in_Trub; 
      double Cp_Trub_new = ( h_out_Trub - h_in_Trub ) / dT_Trub;
      Cp_Trub = Cp_Trub_new;
      double dT_Vol = S_Vol_Out.T - T_in_Vol; 
      double Cp_Vol_new = ( h_out_Vol - h_in_Vol ) / dT_Vol;
      Cp_Vol = Cp_Vol_new;
      double Q_Cp = -Calc_Q( );
      if ( Q < Q_Cp )
        Qb = Q;
      else
        Qe = Q;
      }
    }
*/
  //
  }
