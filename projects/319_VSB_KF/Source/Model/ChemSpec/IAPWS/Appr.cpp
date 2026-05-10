#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"


CIAPWSAppr::CIAPWSAppr( int kData, CIAPWSApprData ApprData[] )
  {
  K = kData;
  Imin = 0;
  Imax = 0;
  Jmin = 0;
  Jmax = 0;
  _x = -1.;
  _y = -1.;
  memset ( vIm, 0, sizeof ( vIm ));
  memset ( vIp, 0, sizeof ( vIp ));
  memset ( vJp, 0, sizeof ( vJp ));
  memset ( vJm, 0, sizeof ( vJm ));
// 
  CIAPWSApprData * pData = ApprData;
  for ( int n = 0; n < K; n++ )
    {
    I[n] = pData->I;
    dI[n] = I[n];
    J[n] = pData->J;
    dJ[n] = J[n];
    N[n] = pData->n;
    if ( I[n] > 0 )
      {
      if ( Imax < I[n] )
        Imax = I[n];
      }
    else
      {
      if ( Imin > I[n] )
        Imin = I[n];
      }
    if ( J[n] > 0 )
      {
      if ( Jmax < J[n] )
        Jmax = J[n];
      }
    else
      {
      if ( Jmin > J[n] )
        Jmin = J[n];
      }
    pData++;
    }
  if ( Jmin < 0 )
    {
    Jmin = -Jmin;
    Jmin += 2;
    }
  if ( Imin < 0 )
    {
    Imin = -Imin;
    Imin += 2;
    }
  }

void CIAPWSAppr::SetV ( double x, double y )
  {
  if ( x != _x )
    {
    _x = x;
    vIp[0] = 1.;
    for ( int n = 1; n <= Imax; n++ )
      vIp[n] = vIp[n-1] * x;
    vIm[0] = 1.;
    double xi = 1. / x;
    for ( int n = 1; n <= Imin; n++ )
      vIm[n] = vIm[n-1] * xi;
    }
  if ( y != _y )
    {
    _y = y;
    vJp[0] = 1.;
    for ( int n = 1; n <= Jmax; n++ )
      vJp[n] = vJp[n-1] * y;
    vJm[0] = 1.;
    double yi = 1. / y;
    for ( int n = 1; n <= Jmin; n++ )
      vJm[n] = vJm[n-1] * yi;
    }
  }

double CIAPWSAppr::Appr ( double x, double y )
  {
  SetV ( x, y );
  double A = 0.;
  for ( int n = 0; n < K; n++ )
    {
    int i = I[n];
    double vi;
    if ( i >= 0 )
      vi = vIp[i];
    else
      vi = vIm[-i];
    int j = J[n];
    double vj;
    if ( j >= 0 )
      vj = vJp[j];
    else
      vj = vJm[-j];
    A += ( N[n] * vj ) * vi;
    }
  return A;
  }

double CIAPWSAppr::Appr_x ( double x, double y )
  {
  SetV ( x, y );
  double A = 0.;
  for ( int n = 0; n < K; n++ )
    {
    int i = I[n];
    if ( i == 0 )
      continue;
    int i1 = i - 1;
    double vi;
    if ( i1 >= 0 )
      vi = vIp[i1];
    else
      vi = vIm[-i1];
    int j = J[n];
    double vj;
    if ( j >= 0 )
      vj = vJp[j];
    else
      vj = vJm[-j];
    A += dI[n] * ( N[n] * vj ) * vi;
    }
  return A;
  }

double CIAPWSAppr::Appr_xx ( double x, double y )
  {
  SetV ( x, y );
  double A = 0.;
  for ( int n = 0; n < K; n++ )
    {
    int i = I[n];
    if ( i == 0 || i == 1 )
      continue;
    int i2 = i - 2;
    double vi;
    if ( i2 >= 0 )
      vi = vIp[i2];
    else
      vi = vIm[-i2];
    int j = J[n];
    double vj;
    if ( j >= 0 )
      vj = vJp[j];
    else
      vj = vJm[-j];
    A += dI[n] * ( dI[n] - 1. ) * ( N[n] * vj ) * vi;
    }
  return A;
  }

double CIAPWSAppr::Appr_y ( double x, double y )
  {
  SetV ( x, y );
  double A = 0.;
  for ( int n = 0; n < K; n++ )
    {
    int j = J[n];
    if ( j == 0 )
      continue;
    int j1 = j - 1;
    double vj;
    if ( j1 >= 0 )
      vj = vJp[j1];
    else
      vj = vJm[-j1];
    int i = I[n];
    double vi;
    if ( i >= 0 )
      vi = vIp[i];
    else
      vi = vIm[-i];
    A += dJ[n] * ( N[n] * vj ) * vi;
    }
  return A;
  }

double CIAPWSAppr::Appr_yy ( double x, double y )
  {
  SetV ( x, y );
  double A = 0.;
  for ( int n = 0; n < K; n++ )
    {
    int j = J[n];
    if ( j == 0 || j == 1 )
      continue;
    int j2 = j - 2;
    double vj;
    if ( j2 >= 0 )
      vj = vJp[j2];
    else
      vj = vJm[-j2];
    int i = I[n];
    double vi;
    if ( i >= 0 )
      vi = vIp[i];
    else
      vi = vIm[-i];
    A += dJ[n] * ( dJ[n] - 1. ) * ( N[n] * vj ) * vi;
    }
  return A;
  }
double CIAPWSAppr::Appr_xy ( double x, double y )
  {
  SetV ( x, y );
  double A = 0.;
  for ( int n = 0; n < K; n++ )
    {
    int i = I[n];
    if ( i == 0 )
      continue;
    int j = J[n];
    if ( j == 0 )
      continue;
    int i1 = i - 1;
    double vi;
    if ( i1 >= 0 )
      vi = vIp[i1];
    else
      vi = vIm[-i1];
    int j1 = j - 1;
    double vj;
    if ( j1 >= 0 )
      vj = vJp[j1];
    else
      vj = vJm[-j1];
    A += dI[n] * dJ[n] * ( N[n] * vj ) * vi;
    }
  return A;
  }

