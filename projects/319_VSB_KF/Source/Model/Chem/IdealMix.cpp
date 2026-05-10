#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"

//
CIG_Mix::CIG_Mix()
  {
  IsComp = NULL;
  Cmol = NULL;
  Mw = 0.;
  T_min = -80.;
  T_max = 3000.;
  Mazut_liq = false;
  }

void CIG_Mix::Set( double _Cmol[K_GAS], bool _IsComp[K_GAS] )
  {
  Cmol = _Cmol;
  IsComp = _IsComp;
  }

void CIG_Mix::Set( CComp * pComp )
  {
  Cmol = pComp->Cmol;
  IsComp = pComp->IsComp;
  }

double CIG_Mix::H ( double T_C )
  {
  double h = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double c = Cmol[n];
      h += c * pGas[n] -> H_IG ( T_C );
      }
    }
  return h;
  }

double CIG_Mix::dH ( double T_out, double T_in  )
  {
  double dh = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double c = Cmol[n];
      dh += c * ( pGas[n] -> H_IG ( T_out ) - pGas[n] -> H_IG ( T_in ) );
      }
    }
  return dh;
  }

double CIG_Mix::Cp ( double T_C )
  {
  double cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double c = Cmol[n];
      cp += c * pGas[n] -> C_IG ( T_C );
      }
    }
  return cp;
  }

void CIG_Mix::dH_Cp( double T_C, double & H, double & Cp, bool CalcCp )
  {
  double h = 0.;
  double m = 0.;
  double cp = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      double c = Cmol[n];
      h += c * pGas[n] -> H_IG ( T_C );
      m += c * pGas[n] ->Mw;
      if ( CalcCp )
        cp += c * pGas[n] -> C_IG ( T_C );
      }
    }
  Mw = m;
  H = h;
  if ( CalcCp )
    Cp = cp;
  }

//double CIG_Mix::S( double T_C, double P )
//  {
//  double s = 0.;
//  double m = 0.;
//  for ( int n = 0; n < LAST_GAS; n++ )
//    {
//    if ( IsComp[n] )
//      {
//      double c = Cmol[n];
//      s += c * pGas[n] -> S_IG ( P, T_C );
//      m += c * pGas[n] ->Mw;
//      }
//    }
//  Mw = m;
//  return s;
//  }

double CIG_Mix::TH ( double H, double T_0 )
  {
  double T = T_0;
  double dT = 1e10;
  double h, cp;
  int kIter = 0;
  while ( fabs ( dT ) > 1e-5 && kIter++ < 20 )
    {
    dH_Cp( T, h, cp, true );
    double f = H - h;
    dT = f / cp;
    if ( dT > 100. )
      dT = 100.;
    T += dT;
    if ( T < T_min )
      {
      T = T_min;
      break;
      }
    if ( T > T_max )
      {
      T = T_max;
      break;
      }
    }
  return T;
  }

double CIG_Mix::M()
  {
  double S = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      S += Cmol[n] * pGas[n] -> Mw;
    }
  return S;
  }

double CIG_Mix::Norm()
  {
  double S = 0.;
  double Sm = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      ASS(Cmol[n]>=0.)
        S += Cmol[n];
      Sm += Cmol[n] * pGas[n] -> Mw;
      }
    }
  if ( S < 1e-5 ) 
    {
    Mw = 0.;
    return S;
    }
  if ( fabs ( S - 1. ) < 1e-8 )
    {
    Mw = Sm;
    return S;
    }
  double Si = 1. / S;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      Cmol[n] *= Si;
      }
    }
  Mw = Sm * Si;
  return S; 
  }

double CIG_Mix::Set( double _Cmol[K_GAS] )
  {
  memcpy ( Cmol, _Cmol, LAST_GAS * 8 );
  return Norm();
  }
