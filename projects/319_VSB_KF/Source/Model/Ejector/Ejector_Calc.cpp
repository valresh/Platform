#include "stdafx.h"
#include "Ejector.h"
#include "Err.h"
#include "CommProc.h"


void CEjector::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( p_In == NULL )
    return;
  if ( p_Podsos == NULL )
    return;
  if ( p_Out == NULL )
    return;
  if ( SuperSound )
    {
    SimplePhysModel ( dt );
    }
  else
    {
    Podsos( dt );
    }
  // Смешение потоков
  Mix( );
//
  if ( Use_JT )
    Heat( );
	}

void CEjector::Mix( )
  {
  SET_BP BreakPoint;
  int k_In = 0;
  double W[3];
  CComp * pComp[3];
  double F_in = 0.;
  double h_in = 0.;
  //
  if ( p_In->Flow_mol > 0. )
    {
    W[k_In] = p_In->Flow_mol;
    F_in += p_In->Flow_mol;
    h_in += p_In->Flow_mol * p_In->h_Flow;
    pComp[k_In] = p_In->Get_Comp();
    k_In++;
    }
  if ( p_Podsos->Flow_mol > 0. )
    {
    W[k_In] = p_Podsos->Flow_mol;
    F_in += p_Podsos->Flow_mol;
    h_in += p_Podsos->Flow_mol * p_Podsos->h_Flow;
    pComp[k_In] = p_Podsos->Get_Comp();
    k_In++;
    }
  if ( p_Out->Flow_mol < 0. )
    {
    W[k_In] = -p_Out->Flow_mol;
    F_in -= p_Out->Flow_mol;
    h_in -= p_Out->Flow_mol * p_Out->h_Flow;
    pComp[k_In] = p_Out->Get_Comp();
    k_In++;
    }
  if ( k_In == 0 || F_in < 0.01 )
    {
    pCompOut ->Set ( pN2 );
    *p_Out = *pN2Flow;
    p_Out->To_kg = Mw ( pCompOut );
    return ;
    }
  double h_Flow = h_in / F_in;
  if ( k_In == 2 )
    {
    if ( pComp[0] == NULL || pComp[1] == NULL )
      return ;
    pCompOut ->Summ ( W[0], pComp[0], W[1], pComp[1] );
    p_Out->ID_COMP = ID_CompOut;
    p_Out->h_Flow = h_Flow;
    p_Out->To_kg = Mw ( pCompOut );
    return ;
    }
  if ( k_In == 1 )
    {
    if ( pComp[0] == NULL )
      return ;
    pCompOut ->Set ( pComp[0] );
    p_Out->ID_COMP = ID_CompOut;
    p_Out->h_Flow = h_Flow;
    p_Out->To_kg = Mw ( pCompOut );
    return ;
    }
  ASS(FALSE)
  }

double CEjector::Напор( double Расход )
  {
  double A = Расход / Alfa_max;
  if ( A < 0. )
    A = 0.;
  if ( A >= 1 )
    return 0.;
  double _h = pow ( 1. - A, Pow_h );
  double h = h_max * _h;
  if ( h < 0. )
    h = 0.;
  return h;
  }

void CEjector::Podsos( double dt )
  {
  SET_BP BreakPoint;
//
  F_in = p_In ->Flow_mol;
  if ( F_in < 0. )
    F_in = 0.;
  _Flow_main = F_in * p_In -> To_kg;
  P_in = p_In -> P;
  P_out = p_Out -> P;
  //
  double F_podsos = p_Podsos->Flow_mol;
  if ( F_podsos < 0. )
    F_podsos = 0.;
  _Flow_podsos = F_podsos * p_Podsos -> To_kg;
  double Ksi = dt * 3600. / Tau;
///////////////////////
  if ( _Flow_main * Alfa_max > _Flow_podsos )
    Alfa = _Flow_podsos / _Flow_main;
  else
    Alfa = Alfa_max;
//
// Относительный напор ( Относительный расход )
  h = Напор( Alfa );
  if ( Defect )
    h = 0;
  double Pc;
  if(h != 1.0)
	  Pc = ( P_out - P_in * h ) / ( 1. - h );
  else
	  Pc = P_cam_max;
  if ( Pc > P_cam_max )
    Pc = P_cam_max;
  if ( Pc < P_cam_min )
    Pc = P_cam_min;
  P_cam += Ksi * ( Pc - P_cam );
  if ( P_cam > P_cam_max )
    P_cam = P_cam_max;
  _dP_cam = P_cam;
  _dP_cam_mm = P_cam * 760.;
  }


void CEjector::Heat( )
  {
  In.PH( p_Out );
  In.T_min = T_min;
  T_in = In.T;
  State_In.SetComp ( pCompOut );
  State_In.SetPT( p_In->P, T_in );
  State_In.DeltaN = 1.;//Условно ( не считать S!!)
  State_In.CalcUHS();
//
  double h_In = State_In.H;
//
  State_Out.SetComp ( pCompOut );
  if ( T_out == 0.)
    T_out = T_in;
  double Tb = T_out, Te = T_out;
  State_Out.SetPT( p_Out->P, T_out );
  State_Out.DeltaN = 1.;//Условно ( не считать S!!)
  State_Out.CalcUHS();
  double h_Out = State_Out.H;
  if ( h_Out > h_In )
    {
    while ( h_Out > h_In )
      {
      Te = Tb;
      Tb -= 1.;
      T_out = Tb;
      State_Out.SetPT( p_Out->P, T_out );
      State_Out.CalcUHS();
      h_Out = State_Out.H;
      }
    }
  else
    {
    while ( h_Out < h_In )
      {
      Tb = Te;
      Te += 1.;
      T_out = Te;
      State_Out.SetPT( p_Out->P, T_out );
      State_Out.CalcUHS();
      h_Out = State_Out.H;
      }
    }
  while ( Te - Tb > 0.01 )
    {
    T_out = 0.5 * ( Te + Tb );
    State_Out.SetPT( p_Out->P, T_out );
    State_Out.CalcUHS();
    h_Out = State_Out.H;
    if ( h_Out < h_In )
      Tb = T_out;
    else
      Te = T_out;
    }
  dT_JT = T_in - T_out;
  double dT_k = dT_JT * k_JT;
  Out.T_min = T_min;
  _T_out = T_in - dT_k;
  Out.PT ( p_Out->P, _T_out, pCompOut );
  p_Out->h_Flow = Out.h;
  p_Out->T = _T_out;
  }

double CSS::h_H2O( double T )
  {
  return Cp_H2O * ( T - 25 );
  }

double CSS::h_Add( double T )
  {
  return Cp_Add * ( T - 25 );
  }

double CEjector::Calc_dh_0 ( double Tp )
  {
  double h_Tp = h_H2O( Tp ); 
  Pp = PTO / pow ( Tp + TK, Lam );
  RoP = Pp * Mw_Mix / ( RP * ( Tp + TK ));
  Wp = G0 / ( RoP * Sp ); 
  double dh = h_in - ( h_Tp + 0.5 * Loose * Wp * Wp );
  return dh;
  }

double CEjector::Calc_h_mix ( double Tm )
  {
  double h_Tm = h_H2O( Tm ); 
  return h_Tm + 0.5 * W3 * W3;
  }

void CEjector::SimplePhysModel ( double dt )
  {
  F_in = p_In->Flow_mol;
  if ( F_in <= 0.1 )
    return;
// Сопло
// Итерируем по T 
  In.PH( p_In );
//
  Mw_Mix = Mw_H2O;
  Loose = 1. + k_Heat;
  if ( CalcPcam )
    {
    double Ps = P_cam_nomZ;
    double Ts = pow ( pow ( p_In->P, 1. - Gamma ) * pow ( In.T + TK, Gamma ) / pow ( Ps, 1. - Gamma ), 1. / Gamma ) - TK;
    double Ros = Ps * Mw_Mix / ( RP * ( Ts + TK ));
    h_in = h_H2O( In.T ); // Дж/кг
    double h_Ts = h_H2O( Ts ); // Дж/кг
    double dH = h_in - h_Ts;
    double Ws = sqrt ( 2 * dH / Loose );
    G0 = p_In->Flow_mol * Mw_Mix / 3600.;
    Sp = G0 / ( Ros * Ws );
    }
//
  Wp = 0;
  G0 = p_In->Flow_mol * Mw_Mix / 3600.; 
  double Tb = -260.;
  double Te = In.T;
  double Tp = Te;
  Pp = p_In->P;
  Lam = Gamma / ( 1. - Gamma );
  PTO = p_In->P * pow ( In.T + TK, Lam );
/*******
  double dh; 
    {
    double Tp = Ts;
    double h_Tp = h_H2O( Tp ); // Дж/кг
    double Pp = PTO / pow ( Tp + TK, Lam );
    double Ro = Pp * Mw / ( RP * ( Tp + TK ));
    Wp = G0 / ( Ro * Sp ); 
    dh = h_in - ( h_Tp + 0.5 * Wp * Wp );
    }
  void * F = Fopen ( "P.csv", "wt" );
  for ( double Tp = -200; Tp < Te; Tp += 1 )
    {
    double h_Tp = h_H2O( Tp ); // Дж/кг
    double Pp = PTO / pow ( Tp + TK, Lam );
    double Ro = Pp * Mw / ( RP * ( Tp + TK ));
    Wp = G0 / ( Ro * Sp ); 
    dh = h_in - ( h_Tp + 0.5 * Wp * Wp );
    Fprintf ( F, "%5.3lf;%5.3lf;%5.4lf;%5.0lf;%8.0lf;\n", Tp, Pp, Ro, Wp, dh );  
    }
  Fclose ( F );
********/
  double dh0 = Calc_dh_0( Te );
  Te -= 2.;
  while ( Te > -250 )
    {
    double dh = Calc_dh_0( Te );
    if ( dh < dh0 )
      break;
    dh0 = dh;
    Te -= 2.;
    }
  double dh;
  while ( Te - Tb > 1e-6 )
    {
    Tp = 0.5 * ( Te + Tb );
    dh = Calc_dh_0( Tp );
    if ( dh > 0. )
      Te = Tp;
    else
      Tb = Tp;
    }
  _Pp = Pp;
  _Tp = Tp;
  _Wp = Wp;
  if ( Simple )
    {
    double Ksi = dt * 3600. / Tau;
  //  double Ro3 = P3 * ::Mw ( &CompMix ) / ( T3 + TK );
    if ( !_finite ( P_cam ))
      P_cam = 0.;
    if ( Pp < P_cam_min )
      Pp = P_cam_min;
    P_cam += Ksi * ( Pp - P_cam );
    if ( P_cam > P_cam_max )
      P_cam = P_cam_max;
    _dP_cam = P_cam;
    _dP_cam_mm = P_cam * 760.;
    return;
    }
  // Wp
  Add.PH( p_Podsos );
  CComp * pCompAdd = p_Podsos->Get_Comp();
  double Mw_Add = ::Mw ( pCompAdd );
  double Gi = p_Podsos->Flow_mol * Mw_Add / 3600.;
  if ( Gi < 0. )
    Gi = 0.;
  if ( CalcCp )
    {
    Cp_H2O = Cp_IG ( In.T, pWater ) * 1e3 / Mw_H2O;
    if ( p_Podsos->Flow_mol > 1. )
      Cp_Add = Cp_IG ( Add.T, pCompAdd ) * 1e3 / Mw_Add;
    }
//
  W3 = G0 * Wp / ( ( 1. + k_Mix ) *( G0 + Gi ));
// 
  double S3 = S_S * Sp;
  double G3 = G0 + Gi;
  double hSumm = G0 * ( h_in + 0.5 * Wp * Wp ) + Gi * h_Add( Add.T );
  double T3 = ( G0 * Cp_H2O * Tp + Gi * Cp_Add * Add.T ) / ( G0 * Cp_H2O + Gi * Cp_Add );
  if ( G3 > 1e-6 )
    T3 = ( G3 * T3 + k_Atm * T_air )/( G3 + k_Atm );
  double Ro3 = G3 /( W3 * S3 );
  double Mw_mix =  G3 / ( G0 / Mw_H2O + Gi / Mw_Add );
  double P3 = RP * Ro3 * ( T3 + TK ) / Mw_Mix;
  _P3 = P3;
  _T3 = T3;
  _W3 = W3;
 double Ksi = dt * 3600. / Tau;
//  double Ro3 = P3 * ::Mw ( &CompMix ) / ( T3 + TK );
  if ( !_finite ( P_cam ))
    P_cam = 0.;
  if ( P3 < P_cam_min )
    P3 = P_cam_min;
  P_cam += Ksi * ( P3 - P_cam );
  if ( P_cam > P_cam_max )
    P_cam = P_cam_max;
  _dP_cam = P_cam;
  _dP_cam_mm = P_cam * 760.;
  }
