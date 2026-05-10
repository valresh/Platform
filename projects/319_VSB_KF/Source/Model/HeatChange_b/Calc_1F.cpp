#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"

//const double Cp_Trub_max = 20000.;
//const double Cp_Vol_max = 20000.;

void CHeatChange::Set_1F( )
  {
  Heat.S_Trub_In.T_min = T_min;
  Heat.S_Trub_Out.T_min = T_min;
  Heat.S_Vol_In.T_min = T_min;
  Heat.S_Vol_Out.T_min = T_min;
  Heat.S_Trub_In.T_max = T_max;
  Heat.S_Trub_Out.T_max = T_max;
  Heat.S_Vol_In.T_max = T_max;
  Heat.S_Vol_Out.T_max = T_max;
  Heat.pIn_Trub = In_Trub;
  Heat.pOut_Trub = Out_Trub;
  Heat.pIn_Vol = In_Vol;
  Heat.pOut_Vol = Out_Vol;
  }

void CHeatChange::Calc_1F( double dt )
  {
  SET_BP BreakPoint;
  Heat.S_Trub_In.T_min = T_min;
  Heat.S_Trub_Out.T_min = T_min;
  Heat.S_Vol_In.T_min = T_min;
  Heat.S_Vol_Out.T_min = T_min;
  Heat.S_Trub_In.T_max = T_max;
  Heat.S_Trub_Out.T_max = T_max;
  Heat.S_Vol_In.T_max = T_max;
  Heat.S_Vol_Out.T_max = T_max;
//  K_Rel = 1.;
  SET_BP BreakPoint;
  if ( Reg_T_Z_Trub.Use != 0. && pSys->bWorkReg )
	  {
	  Heat.M_Trub = 0.;
    Heat.M_Vol = 0.;
//
    Heat.T_Vol_Z = 0.;
    Heat.T_Trub_Z = Reg_T_Z_Trub.Value;
    Heat.Err_Tz = false;
    Heat.KS = KS();
    Heat.Eps_Q = 0.;
    SET_BP BreakPoint;
    Heat.Calc ( dt );
    if ( !Heat.Err_Tz )
      {
      double K = Heat.KS / KS();
      if ( fabs ( K - 1. ) > 1e-6 )
        KKK();
      K_Rel *= K;
      if ( K_Rel < 0.001 )
        KKK();
      if ( K_Rel > 1000. )
        KKK();
      Reg_T_Z_Trub.NotRelease = false;
      }
    else
      {
      ModMsg ( "#Невозможно выполнить задание Ттруб = %4.1lf в объекте '%s'", Reg_T_Z_Trub.Value, ObjName.Str );
      Reg_T_Z_Trub.NotRelease = true;
      Reg_T_Z_Trub.Off();
      }
	  }
  else
	  {
	  if ( Reg_T_Z_Vol.Use && pSys->bWorkReg )
		  {
      Heat.M_Trub = 0.;
      Heat.M_Vol = 0.;
      //
      Heat.T_Vol_Z = Reg_T_Z_Vol.Value;
      Heat.T_Trub_Z = 0.;
      Heat.Err_Tz = false;
      Heat.KS = KS();
      Heat.Eps_Q = 0.;
      SET_BP BreakPoint;
      Heat.Calc ( dt );
      if ( !Heat.Err_Tz )
        {
        double K = Heat.KS / KS();
        if ( fabs ( K - 1. ) > 1e-6 )
          KKK();
        K_Rel *= K;
        if ( K_Rel < 0.001 )
          KKK();
        if ( K_Rel > 1000. )
          KKK();
        }
      else
        {
        ModMsg ( "#Невозможно выполнить задание Тоб = %4.1lf в объекте '%s'", Reg_T_Z_Vol.Value, ObjName.Str );
        Reg_T_Z_Vol.NotRelease = true;
        Reg_T_Z_Vol.Off();
        }
		  }
	  else
		  {
      Heat.T_Vol_Z = 0.;
      Heat.T_Trub_Z = 0.;
      Heat.Err_Tz = false;
      Heat.Eps_Q = 0.;
      Heat.KS = KS();
//
      SET_BP BreakPoint;
      Heat.Calc ( dt );
		  }
    }
  _T_Trub_in = T_in_Trub = Heat.T_in_Trub;
  _T_Trub_out = T_out_Trub = Heat.T_out_Trub;
  _T_Vol_in = T_in_Vol = Heat.T_in_Vol;
  _T_Vol_out = T_out_Vol = Heat.T_out_Vol;
  _E_Trub_in = Heat.S_Trub_In.E;
  _E_Trub_out = Heat.S_Trub_Out.E;
  _E_Vol_in = Heat.S_Vol_In.E;
  _E_Vol_out = Heat.S_Vol_Out.E;
  _h_Trub_in = Heat.S_Trub_In.h;
  _h_Trub_out = Heat.S_Trub_Out.h;
  _h_Vol_in = Heat.S_Vol_In.h;
  _h_Vol_out = Heat.S_Vol_Out.h;
  }

bool CalcTeta ( double T1_in, double T1_out, double T2_in, double T2_out, double kPr, double & Teta );
bool CalcTeta ( double T1_in, double T1_out, double T2_in, double T2_out, double kPr, double & Teta, double & dTeta_T1, double & dTeta_T2 );



//bool Calc_dQ( double h2_out, 
//              double h2_in, double h1_in, double F2, double F1,
//              double kAntiFlow, double KS,
//              CFlow * pHot_out, CSEps	* S_Hot_In,  CSEps * S_Cool_In,  CSEps	* S_Hot_Out, CSEps	* S_Cool_Out,
//              double & dQ )
bool Calc_dQ( double h2_out, 
              double h2_in, double h1_in, double F2, double F1,
              double kAntiFlow, double KS,
              double _h1_out, double _h2_out,
              double a1, double a2,
              CFlow * pHot_out, CSEps	* S_Hot_In,  CSEps * S_Cool_In,  CSEps	* S_Hot_Out, CSEps	* S_Cool_Out,
              double & dQ )
  {
//  double h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
  if ( S_Cool_Out->T > S_Hot_In->T + 0.001 )
    return false;
  double h1_out = ( F1 * h1_in + F2 * ( h2_in - h2_out ) + a1 * _h1_out + a2 * ( _h2_out - h2_out )) / ( F1 + a1 );
  S_Hot_Out->PH( pHot_out->P, h1_out, pHot_out->Get_Comp());
  if ( S_Hot_Out->T < S_Cool_In->T + 0.001 )
    return false;
  if ( S_Hot_Out->T > S_Hot_In->T + 0.001 )
    return false;
  double Teta;
  if ( !TestTemp ( S_Hot_In->T, S_Cool_In->T, S_Hot_Out->T, S_Cool_Out->T ))
    KKK();
  ASS( CalcTeta ( S_Hot_In->T, S_Hot_Out->T, S_Cool_In->T, S_Cool_Out->T, kAntiFlow, Teta ));
  double Q = KS * Teta;
//  dQ = F2 * ( h2_in - h2_out ) + Q;
  dQ = F2 * ( h2_in - h2_out ) + Q - a2 * ( h2_out - _h2_out );
  return true;
  }

double dTdH ( CSEps	* Eps )
  {
  double dh_l = 0.;
  double dh_g = 0.;
  Eps->Calc_Mw();
  double T = Eps->T;
  double E = Eps->E;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( Eps ->IsComp[c] )
      {
      if ( E > 0. )
        dh_g += Eps->Y_W[c] * pGas[c]->C_IG( T );
      if ( E < 1. )
        dh_l += Eps->X_W[c] * pGas[c]->dH_1_dTC( T );
      FINITE(dh_l)
      }
    }
  double dH_dT = E * dh_g + ( 1. - E ) * dh_l;
  return 1. / dH_dT;
  }

bool Calc_dQ_dH( double h2_out, 
                 double h2_in, double h1_in, double F2, double F1,
                 double kAntiFlow, double KS,
                 CFlow * pHot_out, CFlow * pCool_out, CSEps	* S_Hot_In,  CSEps * S_Cool_In,  CSEps	* S_Hot_Out, CSEps	* S_Cool_Out,
                 double & dQ, double & dQ_dh2 )
  {
  double h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
  double dh1_out = -F2 / F1;
  S_Hot_Out->PH( pHot_out->P, h1_out, pHot_out->Get_Comp());
  if ( S_Hot_Out->T < S_Cool_In->T + 0.001 )
    return false;
/////////////////////
  double dT1_dH1 = dTdH ( S_Hot_Out );
  S_Cool_Out->PH( pCool_out->P, h2_out, pCool_out->Get_Comp());
  double dT2_dH2 = dTdH ( S_Cool_Out );
  double Teta;
  double dTeta_T1, dTeta_T2;
  ASS( CalcTeta ( S_Hot_In->T, S_Hot_Out->T, S_Cool_In->T, S_Cool_Out->T, kAntiFlow, Teta, dTeta_T1, dTeta_T2 ));
  double Q = KS * Teta;
  dQ = F2 * ( h2_in - h2_out ) + Q;
  dQ_dh2 = -F2 + KS * ( dTeta_T1 * dT1_dH1 * dh1_out + dTeta_T2 * dT2_dH2 );
  return true;
  }

void CUniHeatChange::CalcTemp( double dt )
  {
  SET_BP BreakPoint;
//
  double F1 = fabs ( Params.pHot_in->Flow_mol );
  double F2 = fabs ( Params.pCool_in->Flow_mol );
  double h1_in = Params.S_Hot_In->h;
  double h1_out = Params.S_Hot_Out->h;
  double h2_in = Params.S_Cool_In->h;
  double h2_out = Params.S_Cool_Out->h;
  double T_hot_in = Params.S_Hot_In->T;
  double T_hot_out = Params.S_Hot_Out->T;
  double T_cool_in = Params.S_Cool_In->T;
  double T_cool_out = Params.S_Cool_Out->T;
  double _h1_out = h1_out;
  double _h2_out = h2_out;
  double a1 = Slow_1 * 1e6 * dt;
  double a2 = Slow_2 * 1e6 * dt;
//
  double h2_b, h2_e;
  double dQ = 0;
  double Q = 0;
  int kIter = 0;
//
  bool OK = false;
  if ( First_Step )
    {
    h2_b = h2_in;
    Params.S_Cool_Out->PT( Params.pCool_out->P, Params.S_Hot_In->T, Params.pCool_out->Get_Comp());
    h2_e = Params.S_Cool_Out->h;
    First_Step = false;
    }
  else
    {
// Ньютон
    double dh2_out = 1;
    double Fn; 
    double dFn, Fn1; 
    double hn = h2_out;
    int kIter = 0;
    bool Prt = false;
    while ( fabs ( dh2_out ) > 1e-3 && kIter++ < 10 )
      {
      OK = Calc_dQ( hn, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
                    _h1_out, _h2_out, a1, a2,
                    Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn );
      if ( !OK )
        break;
      OK = Calc_dQ( hn + 1., h2_in, h1_in, F2, F1, kAntiFlow, KS, 
                    _h1_out, _h2_out, a1, a2,
                    Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn1 );
      if ( !OK )
        break;
      //double Fn2, Fn3,Fn4,Fn5,Fn6,Fn7;
      //OK = Calc_dQ( hn + 1e-4, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
      //  _h1_out, _h2_out, a1, a2,
      //  Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn2 );
      //OK = Calc_dQ( hn + 1e-3, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
      //  _h1_out, _h2_out, a1, a2,
      //  Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn3 );
      //OK = Calc_dQ( hn + 1e-2, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
      //  _h1_out, _h2_out, a1, a2,
      //  Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn4 );
      //OK = Calc_dQ( hn + 1e-1, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
      //  _h1_out, _h2_out, a1, a2,
      //  Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn5 );
      //OK = Calc_dQ( hn + 1e0, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
      //  _h1_out, _h2_out, a1, a2,
      //  Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn6 );
      //OK = Calc_dQ( hn + 1e1, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
      //  _h1_out, _h2_out, a1, a2,
      //  Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn7 );
      //double dFn2 = ( Fn2 - Fn ) / 1e-4;
      //double dFn3 = ( Fn3 - Fn ) / 1e-3;
      //double dFn4 = ( Fn4 - Fn ) / 1e-2;
      //double dFn5 = ( Fn5 - Fn ) / 1e-1;
      //double dFn6 = ( Fn6 - Fn ) / 1e-0;
      //double dFn7 = ( Fn7 - Fn ) / 1e1;
//
      dFn = ( Fn1 - Fn );
      dh2_out = -Fn / dFn;
      hn += dh2_out;
      OK = Calc_dQ( hn, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
        _h1_out, _h2_out, a1, a2,
        Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn );
      KKK();
      }
    //if ( Prt )
    //  {
    //  Prt = false;
    //  void * F = Fopen ( "TE.csv", "wt" );
    //  for ( double dh = -0.01; dh < 0.01; dh += 0.0001 )
    //    {
    //    OK = Calc_dQ( hn + dh, h2_in, h1_in, F2, F1, kAntiFlow, KS, Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn );
    //    Fprintf ( F, "%7.4lf;%7.4lf;%7.5lf;%7.5lf;%7.5lf;%7.5lf;\n", dh, Fn, Params.S_Hot_Out->E, Params.S_Hot_Out->T, Params.S_Cool_Out->E, Params.S_Cool_Out->T );
    //    }
    //  Fclose ( F );
    //  }
    if ( OK && kIter < 10 )
      {
      h2_out = hn;
      Params.S_Cool_Out->PH( Params.pCool_out->P, h2_out, Params.pCool_out->Get_Comp());
      h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
      Params.S_Hot_Out->PH( Params.pHot_out->P, h1_out, Params.pHot_out->Get_Comp());
      if ( Params.S_Cool_Out->T < Params.S_Hot_In->T - 0.01 )
        return;
      h2_out = Params.S_Cool_Out->h;
      }
    OK = true;
// Трапеции
    hn = h2_out;
    OK = Calc_dQ( hn, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
                  _h1_out, _h2_out, a1, a2,
                  Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn );
    if ( OK )
      {
      double Fn1; 
      double hn1;
      if ( Fn > 0. )
        {
        hn1 = h2_out + 100.;
        OK = Calc_dQ( hn1, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
                      _h1_out, _h2_out, a1, a2,
                      Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn1 );
        }
      else
        {
        hn1 = h2_out - 100.;
        OK = Calc_dQ( hn1, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
                      _h1_out, _h2_out, a1, a2,
                      Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, Fn1 );
        }
      if ( OK )
        {
        double h = 0.;
        double F = 0.;
        double dh_min = 10;
        int kIter = 0;
        while ( fabs ( dh_min ) > 1e-3  && kIter++ < 10 )
          {
          h = ( Fn * hn1 - Fn1 * hn ) / ( Fn - Fn1 );
          OK = Calc_dQ( h, h2_in, h1_in, F2, F1, kAntiFlow, KS, 
                        _h1_out, _h2_out, a1, a2,
                        Params.pHot_out, Params.S_Hot_In,  Params.S_Cool_In,  Params.S_Hot_Out, Params.S_Cool_Out, F );
          if ( !OK )
            break;
          double dhn = fabs ( h - hn );
          double dhn1 = fabs ( h - hn1 );
          if ( dhn < dhn1 )
            {
            dh_min = dhn;
            hn1 = h;
            Fn1 = F;
            }
          else
            {
            dh_min = dhn1;
            hn = h;
            Fn = F;
            }
          }
        if ( OK && kIter < 10 )
          {
          h2_out = h;
          Params.S_Cool_Out->PH( Params.pCool_out->P, h2_out, Params.pCool_out->Get_Comp());
          h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
          Params.S_Hot_Out->PH( Params.pHot_out->P, h1_out, Params.pHot_out->Get_Comp());
          if ( Params.S_Cool_Out->T < Params.S_Hot_In->T - 0.01 )
            return;
          h2_out = Params.S_Cool_Out->h;
          }
        }
      }
    }
  OK = true;
//  First_Step = true;
  if ( First_Step )
    {
    h2_b = h2_in;
    Params.S_Cool_Out->PT( Params.pCool_out->P, Params.S_Hot_In->T, Params.pCool_out->Get_Comp());
    h2_e = Params.S_Cool_Out->h;
    First_Step = false;
    }
  else
    {
    h2_b = h2_e = h2_out;
    h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
    Params.S_Hot_Out->PH( Params.pHot_out->P, h1_out, Params.pHot_out->Get_Comp());
    ASS( CalcTeta ( Params.S_Hot_In->T, Params.S_Hot_Out->T, Params.S_Cool_In->T, Params.S_Cool_Out->T, kAntiFlow, Teta ));
    Q = KS * Teta;
    dQ = F2 * ( h2_in - h2_out ) + Q;
    if ( dQ < 0 )
      {
// Уменьшаем h2
      while( dQ < 0 && kIter++ < 20 )
        {
        h2_e = h2_out; 
        h2_out -= 100.;
        h2_b = h2_out;
        h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
        Params.S_Hot_Out->PH( Params.pHot_out->P, h1_out, Params.pHot_out->Get_Comp());
        if ( Params.S_Hot_Out->T < Params.S_Cool_In->T )
          break;
        ASS( CalcTeta ( Params.S_Hot_In->T, Params.S_Hot_Out->T, Params.S_Cool_In->T, Params.S_Cool_Out->T, kAntiFlow, Teta ));
        Q = KS * Teta;
        dQ = F2 * ( h2_in - h2_out ) + Q;
        }
      }
    else
      {
// Увеличиваем h2
      while( dQ > 0 && kIter++ < 20 )
        {
        h2_b = h2_out;
        h2_out += 100.;
        h2_e = h2_out; 
        h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
        Params.S_Hot_Out->PH( Params.pHot_out->P, h1_out, Params.pHot_out->Get_Comp());
        if ( Params.S_Hot_Out->T < Params.S_Cool_In->T )
          break;
        ASS( CalcTeta ( Params.S_Hot_In->T, Params.S_Hot_Out->T, Params.S_Cool_In->T, Params.S_Cool_Out->T, kAntiFlow, Teta ));
        Q = KS * Teta;
        dQ = F2 * ( h2_in - h2_out ) + Q;
        }
      }
    }
// F2 * ( h2_in - h2_out ) + KS * Teta = 0;
// Трапеции
// Деление пополам
  while ( h2_e - h2_b > 1e-3 && kIter++ < 20 )
    {
    h2_out = 0.5 * ( h2_e + h2_b );
    Params.S_Cool_Out->PH( Params.pCool_out->P, h2_out, Params.pCool_out->Get_Comp());
    h1_out = h1_in + F2 / F1 * ( h2_in - h2_out );
    Params.S_Hot_Out->PH( Params.pHot_out->P, h1_out, Params.pHot_out->Get_Comp());
    if ( Params.S_Hot_Out->T < Params.S_Cool_In ->T + 0.001 )
      {
      h2_e = h2_out;
      continue;
      }
    if ( Params.S_Hot_Out->T > Params.S_Hot_In->T + 0.001 )
      {
      h2_e = h2_out;
      continue;
      }
    ASS( CalcTeta ( Params.S_Hot_In->T, Params.S_Hot_Out->T, Params.S_Cool_In->T, Params.S_Cool_Out->T, kAntiFlow, Teta ));
    Q = KS * Teta;
    dQ = F2 * ( h2_in - h2_out ) + Q;
    if ( dQ < 0 )
      h2_e = h2_out;
    else
      h2_b = h2_out;
    }
  //double Tst1 = F1 * ( h1_in - h1_out ) - F2 * ( h2_out - h2_in );
  double Tst2;
  if(Q != 0.0)
	  Tst2 = dQ / Q * 100.;
  else
	  Tst2 = 0.0;
  Eps_Q = Tst2;
  KKK();
  }

const double Eps = 0.5;
bool TestTemp ( double T1_in, double T2_in, double T1_out, double T2_out )
  {
  int Err = 0;
  if ( T1_in > T2_in )
    {
// T1 нагревает T2
    if ( T1_out > T1_in + Eps )
      Err += 1;
    if ( T1_out < T2_in - Eps )
      Err += 2;
    if ( T2_out < T2_in - Eps )
      Err += 4;
    if ( T2_out > T1_in + Eps )
      Err += 8;
    }
  else
    {
// T2 нагревает T1
    if ( T2_out > T2_in + Eps )
      Err += 1;
    if ( T2_out < T1_in - Eps )
      Err += 2;
    if ( T1_out < T1_in - Eps )
      Err += 4;
    if ( T1_out > T2_in + Eps )
      Err += 8;
    }
  if ( Err == 0 )
    return true;
  return false;
  }



void CUniHeatChange::CalcTrubZ( double dt, double T_Trub_Z, CFlow * pVol_in, CFlow * pTrub_in, CFlow * pVol_out, CFlow * pTrub_out,  
                                CSEps	* S_Vol_In,  CSEps *	S_Trub_In,  CSEps	* S_Vol_Out, CSEps	* S_Trub_Out )
  {
  double F_vol = fabs ( pVol_in->Flow_mol );
  double F_trub = fabs ( pTrub_in->Flow_mol );
  double h_vol_in = S_Vol_In->h;
  double h_vol_out = S_Vol_Out->h;
  double h_trub_in = S_Trub_In->h;
  double h_trub_out = S_Trub_Out->h;
  double T_vol_in = S_Vol_In->T;
  double T_vol_out = S_Vol_Out->T;
  double T_trub_in = S_Trub_In->T;
  double T_trub_out = S_Trub_Out->T;
  if ( !TestTemp ( T_vol_in, T_trub_in, T_vol_out, T_trub_out ))
    KKK();
///
  if ( T_Trub_Z > T_trub_in )
    {
// Нагрев от vol
    if ( T_Trub_Z > T_vol_in - 1. )
      T_Trub_Z = T_vol_in - 1.;
    }
  else
    {
// Охлаждение от vol
    if ( T_Trub_Z < T_vol_in + 1. )
      T_Trub_Z = T_vol_in + 1.;
    }
 
  if ( S_Trub_In->T < T_Trub_Z )
    {
// Труба нагревается
    S_Trub_Out->PT ( pTrub_out->P, T_Trub_Z, pTrub_out->Get_Comp());
    h_trub_out = S_Trub_Out->h;
    Q = F_trub * ( h_trub_out - h_trub_in );
    h_vol_out = h_vol_in - Q / F_vol;
    S_Vol_Out->PH ( pVol_out->P, h_vol_out, pVol_out->Get_Comp());
    if ( S_Vol_Out->T < T_trub_in + 0.01 )
      {
      Err_Tz = true;
//      CalcTemp( dt, pVol_in, pTrub_in, pVol_out, pTrub_out, &S_Vol_In, &S_Trub_In, &S_Vol_Out, &S_Trub_Out );
      }
    else
      {
      CalcTeta ( S_Trub_In->T, S_Trub_Out->T, S_Vol_In->T, S_Vol_Out->T, kAntiFlow, Teta );
      KS = fabs ( Q ) / Teta;
      Err_Tz = false;
      }
    }
  else
    {
// Труба охлаждается
    S_Trub_Out->PT ( pTrub_out->P, T_Trub_Z, pTrub_out->Get_Comp());
    h_trub_out = S_Trub_Out->h;
    Q = F_trub * ( h_trub_in - h_trub_out );
    h_vol_out = h_vol_in + Q / F_vol;
    S_Vol_Out->PH ( pVol_out->P, h_vol_out, pVol_out->Get_Comp());
    if ( S_Vol_Out->T > T_trub_in - 0.01 )
      {
      Err_Tz = true;
      }
    else
      {
      CalcTeta ( S_Trub_In->T, S_Trub_Out->T, S_Vol_In->T, S_Vol_Out->T, kAntiFlow, Teta );
      KS = fabs ( Q ) / Teta;
      Err_Tz = false;
      }
    }
  if ( Расчет_через_Cp )
    CalcTrubZ_Cp( dt );
  }

void CUniHeatChange::CalcVolZ( double dt, double T_Vol_Z, CFlow * pVol_in, CFlow * pTrub_in, CFlow * pVol_out, CFlow * pTrub_out,  
                               CSEps	* S_Vol_In,  CSEps *	S_Trub_In,  CSEps	* S_Vol_Out, CSEps	* S_Trub_Out )
  {
  double F_vol = fabs ( pVol_in->Flow_mol );
  double F_trub = fabs ( pTrub_in->Flow_mol );
  double h_vol_in = S_Vol_In->h;
  double h_vol_out = S_Vol_Out->h;
  double h_trub_in = S_Trub_In->h;
  double h_trub_out = S_Trub_Out->h;
  double T_vol_in = S_Vol_In->T;
  double T_vol_out = S_Vol_Out->T;
  double T_trub_in = S_Trub_In->T;
  double T_trub_out = S_Trub_Out->T;
  if ( !TestTemp ( T_vol_in, T_trub_in, T_vol_out, T_trub_out ))
    KKK();
//
  if ( T_Vol_Z > T_vol_in )
    {
    // Нагрев от trub
    if ( T_Vol_Z > T_trub_in - 1. )
      T_Vol_Z = T_trub_in - 1.;
    }
  else
    {
    // Охлаждение от trub
    if ( T_Vol_Z < T_trub_in + 1. )
      T_Vol_Z = T_trub_in + 1.;
    }
  if ( S_Vol_In->T < T_Vol_Z )
    {
// Объем нагревается
    S_Vol_Out->PT ( pVol_out->P, T_Vol_Z, pVol_out->Get_Comp());
    h_vol_out = S_Vol_Out->h;
    Q = F_vol * ( h_vol_out - h_vol_in );
    h_trub_out = h_trub_in - Q / F_trub;
    S_Trub_Out->PH ( pTrub_out->P, h_trub_out, pTrub_out->Get_Comp());
    if ( S_Trub_Out->T < T_vol_in + 0.01 )
      {
      Err_Tz = true;
      }
    else
      {
      CalcTeta ( S_Trub_In->T, S_Trub_Out->T, S_Vol_In->T, S_Vol_Out->T, kAntiFlow, Teta );
      KS = fabs ( Q ) / Teta;
      Err_Tz = false;
      }
    }
  else
    {
// Объем охлаждается
    S_Vol_Out->PT ( pVol_out->P, T_Vol_Z, pVol_out->Get_Comp());
    h_vol_out = S_Vol_Out->h;
    Q = F_vol * ( h_vol_in - h_vol_out );
    h_trub_out = h_trub_in + Q / F_trub;
    S_Trub_Out->PH ( pTrub_out->P, h_trub_out, pTrub_out->Get_Comp());
    if ( S_Trub_Out->T > T_vol_in - 0.01 )
      {
      Err_Tz = true;
      }
    else
      {
      CalcTeta ( S_Trub_In->T, S_Trub_Out->T, S_Vol_In->T, S_Vol_Out->T, kAntiFlow, Teta );
      KS = fabs ( Q ) / Teta;
      Err_Tz = false;
      }
    }
  if ( Расчет_через_Cp )
    CalcVolZ_Cp( dt );
  }
