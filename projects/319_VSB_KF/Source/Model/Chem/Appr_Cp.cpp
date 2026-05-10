#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"



CLinApprCp::CLinApprCp(  )
  {
  }

bool CLinApprCp::Init( double _T_min, double _T_max, double _Step, double Cp[] )
  {
  T_min = _T_min;
  T_max = _T_max;
  Step = _Step;
  kData = (int)(( T_max - T_min ) / Step) + 1;
  if ( kData < 2 || kData > 128 )
    return false;
  for ( int n = 1; n < kData; n++ )
    {
    if ( Cp[n] < Cp[n-1] )
      return false;
    }
  H0 = NewArr( double, kData );
  T0 = NewArr( double, kData );
  a = NewArr( double, kData );
  b = NewArr( double, kData );
  double H = 0.;
  double T = T_min;
  for ( int n = 0; n < kData - 1; n++ )
    {
// Cp = a * x + b  x = T - T0[n]
// H = a * x^2 / 2 + b * x
    a[n] = ( Cp[n+1] - Cp[n] ) / Step;
    b[n] = Cp[n];
    T0[n] = T;
    H0[n] = H;
    double x = Step;
    H += ( 0.5 * a[n] * x + b[n] ) * x;
    T += Step;
    }
  H_norm = 0.;
  double H25 = H_IG( 25. );
  H_norm = H25;
  H25 = H_IG( 25. );
  Cp_max = Cp[kData-1];
  return true;
  }

double CLinApprCp::C_IG ( double TC )
  {
  double T = TC + TK;
  if ( T >= T_max )
    T = T_max - 1e-8;
  if ( T < T_min )
    T = T_min + 1e-8;
  int n = (int)( ( T - T_min ) / Step );
  if ( n >= kData )
    n = kData - 1;
  double x = T - T0[n];
  double C = a[n] * x + b[n];
  return C;
  }

double CLinApprCp::H_IG ( double TC )
  {
  double T = TC + TK;
  double dT = 0;
  if ( T >= T_max )
    {
    dT = T - T_max;
    T = T_max - 1e-8;
    }
  int n = (int)( ( T - T_min ) / Step );
  if ( n >= kData )
    n = kData - 1;
  double x = T - T0[n];
  double H = ( 0.5 * a[n] * x + b[n] ) * x + H0[n] - H_norm;
  if ( dT > 0. )
    H += Cp_max * dT;
  return H;
  }


