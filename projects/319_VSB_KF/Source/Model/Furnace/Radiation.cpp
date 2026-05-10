#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "SysDataTypes.h"


extern double Sigma;

CFireT::CFireT()
  {
  k = 1000000000.;
  Text = 500.;
  U = Text + TK;
  M = k / ( 2. * U * U * U );
  U4 = U * U * U * U;
  Told = NULL;
  }

void CFireT::Set_U( double _k, double _Text )
  {
  Text = _Text;
  k = _k;
  U = Text + TK;
  M = k / ( 2. * U * U * U );
  U4 = U * U * U * U;
  Step_T = 1.;
  }

void CFireT::SetOld( double T )
  {
  //  Step_T = fabs ( Told - T );
  //  if ( Step_T < 1. )
  Step_T = 1.;
  *Told = T;
  }

double CFireT::F( double TC )
  {
  double T = TC + TK;
  double W = 0.5 * log(( T + U )/fabs( T - U )) + atan ( T / U );
  return M * W;
  }

void CFireT::Set_T0( double _T0 )
  {
  T0 = _T0; 
  F0 = F ( T0 );
  }

void CFireT::X_dX( double TC, double & Q, double & dQ_dT )
  {
  Q = F( TC ) - F0;
  double T = TC + TK;
  double T2 = T * T;
  double U2 = U * U;
  dQ_dT = -k /( T2 * T2 - U2 * U2 );
  }

double CFireT::X_T( double TC )
  {
  return F( TC ) - F0;
  }

double CFireT::T_X_cool( double XT )
  {
  double T = *Told;
  if ( T <= Text )
    T = Text + Step_T;
  if ( T >= T0 )
    T = T0 - Step_T;
  double Tb = T;
  double Te = T;
  //
  double X = X_T( T );
  if ( fabs ( X - XT ) > 0.2 )
    {
    // Пополам
    Tb = Text;
    Te = T0;
    while ( Te - Tb > Step_T )
      {
      T = 0.5 * ( Tb + Te );
      X = X_T( T );
      if ( X > XT )
        Tb = T;
      else
        Te = T;
      }
    }
  else
    {
    double Xb = X;
    double Xe = X;
    if ( X < XT )
      {
      while ( X < XT )
        {
        Te = Tb;
        Xe = Xb;
        if ( T <= Text )
          {
          SetOld( Text );
          return Text;
          }
        T -= Step_T;
        if ( T < Text )
          T = Text + 0.01;
        Tb = T;
        X = X_T( T );
        Xb = X;
        }
      }
    else
      {
      while ( X > XT )
        {
        Tb = Te;
        Xb = Xe;
        if ( T >= T0 )
          {
          SetOld( T0 );
          return T0;
          }
        T += Step_T;
        if ( T > T0 )
          T = T0 - 0.01;
        Te = T;
        X = X_T( T );
        Xe = X;
        }
      }
    }
  double dT = Te - Tb;
  int kIter = 0;
  bool OK = true;
  while ( fabs ( dT ) > 1e-6 )
    {
    kIter++;
    double Q, dQdT; 
    X_dX( T, Q, dQdT );
    dT = ( XT - Q ) / dQdT;
    T += dT;
    if ( T > T0 || T < Text || kIter > 10)
      {
      OK = false;
      break;
      }
    }
  if ( OK )
    {
    SetOld( T );
    return T;
    }
  while ( Te - Tb > 1e-6 )
    {
    T = 0.5 * ( Tb + Te );
    X = X_T( T );
    if ( X < XT )
      Tb = T;
    else
      Te = T;
    }
  SetOld( T );
  return T;
  }

double CFireT::T_X_heat( double XT )
  {
  double T = *Told;
  if ( T >= Text )
    T = Text - Step_T;
  if ( T <= T0 )
    T = T0 + Step_T;
  double Tb = T;
  double Te = T;
  //
  double X = X_T( T );
  if ( fabs ( X - XT ) > 0.2 )
    {
    // Пополам
    Tb = T0;
    Te = Text;
    while ( Te - Tb > Step_T )
      {
      T = 0.5 * ( Tb + Te );
      X = X_T( T );
      if ( X < XT )
        Tb = T;
      else
        Te = T;
      }
    }
  else
    {
    double Xb = X;
    double Xe = X;
    if ( X > XT )
      {
      while ( X > XT )
        {
        Te = Tb;
        Xe = Xb;
        if ( T >= Text )
          {
          SetOld( Text );
          return Text;
          }
        T -= Step_T;
        if ( T > Text )
          T = Text - 0.01;
        Tb = T;
        X = X_T( T );
        Xb = X;
        }
      }
    else
      {
      while ( X < XT )
        {
        Tb = Te;
        Xb = Xe;
        if ( T <= T0 )
          {
          SetOld( T0 );
          return T0;
          }
        T += Step_T;
        if ( T < T0 )
          T = T0 + 0.01;
        Te = T;
        X = X_T( T );
        Xe = X;
        }
      }
    }
  double dT = Te - Tb;
  int kIter = 0;
  bool OK = true;
  while ( fabs ( dT ) > 1e-6 )
    {
    kIter++;
    double Q, dQdT; 
    X_dX( T, Q, dQdT );
    dT = ( XT - Q ) / dQdT;
    T += dT;
    if ( T < T0 || T > Text || kIter > 10)
      {
      OK = false;
      break;
      }
    }
  if ( OK )
    {
    SetOld( T );
    return T;
    }
  while ( Te - Tb > 1e-6 )
    {
    T = 0.5 * ( Tb + Te );
    X = X_T( T );
    if ( X < XT )
      Tb = T;
    else
      Te = T;
    }
  SetOld( T );
  return T;
  }

double CFireT::T_X( double XT )
  {
  double T = T0;
  if ( T0 > Text )
    return T_X_cool( XT );
  else
    return T_X_heat( XT );
  }


double CFurnace::Rad_Wall( )
  {
  double S = kSrad * S_Wall * Sigma;
  double Tw = T_Wall + TK;
  double Tw2 = Tw * Tw;
  return S * Tw2 * Tw2;
  }

void CFurnace::CalcT_Rad( double dt )
  {
// Излучение
  double E_fors = 0.;
  for ( int n = 0; n < kFors; n++ )
    {
    CRadFors * pF = pFire[n];
    E_fors += pF->Q_Rad;
    }
  double E_trub = 0.;
  double Q_Продукт = 0.;
  for ( int n = 0; n < kProduct; n++ )
    {
    CRadTube * pT = pTube[n];
    E_trub += pT->QRad();
    Q_Продукт += pT->Q_Продукт;
    }
  double E_wall = Rad_Wall( );
//
  double D_fors = E_trub * a_tf + E_wall * a_wf;
  double D_trub = E_fors * a_ft + E_wall * a_wt + E_trub * a_tt;
  double D_wall = E_fors * a_fw + E_trub * a_tw + E_wall * a_ww;
//
  double Test_Trub = ( D_trub - E_trub - Q_Продукт ) / Q_Продукт;
  double Q_F = E_fors - D_fors;
  double Test_Fors = Q_F / Q_Продукт;
//
  Q_wall = D_wall - E_wall;
  FINITE(Q_wall)
  Q_fors = E_fors - D_fors;
  double Heat_fors = D_fors / kFors;
  for ( int n = 0; n < kFors; n++ )
    {
    CRadFors * pF = pFire[n];
    pF->Q_ext_Rad = Heat_fors;
    }                                     
  Q_trub = D_trub - E_trub;
  double Heat_trub = D_trub;
  for ( int n = 0; n < kProduct; n++ )
    {
    CRadTube * pT = pTube[n];
    pT->Q_ext_Rad = W_Tube[n] * Heat_trub;
    }
// Охлаждение стены
  double Heat_wall = D_wall - E_wall;
  double adt = Cool_Wall * dt;
  double T_Wall_new = ( Cp_Wall * T_Wall + Heat_wall * dt + T_air * adt ) / ( Cp_Wall + adt );
  T_Wall += LimdX ( T_Wall_new - T_Wall, 5 );
  Q_air = Cool_Wall * ( T_Wall - T_air );
  Q_summ = Q_fors - Q_trub - Q_wall - Q_air;
  }

