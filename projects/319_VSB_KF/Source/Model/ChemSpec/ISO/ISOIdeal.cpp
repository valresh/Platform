#include "stdafx.h"
#include "Chem.h"
#include "Data.h"
#include "ISO.h"


struct CIdealData
  {
  int c;
  double A1, A2, B, C, D, E, F, G, H, I, J;
  double F0( double Tau );
  double dFdTau( double Tau );
  double dFdTauTau( double Tau );
  };


static CIdealData IdealData[K_BASE] = {
  1,23.26530,-2801.72907,3.50031,0.13732,662.738,-0.14660,680.562,0.90066,1740.06,0,0,
  2,26.35604,-4902.17152,3.50002,2.04452,919.306,-1.06044,865.070,2.03366,483.553,0.01393,341.109,
  3,35.53603,-15999.69151,4.00088,0.76315,820.659,0.00460,178.410,8.74432,1062.82,-4.46921,1090.53,
  4,42.42766,-23639.65301,4.00263,4.33939,559.314,1.23722,223.284,13.1974,1031.38,-6.01989,1071.29,
  5,50.40669,-31236.63551,4.02939,6.60569,479.856,3.19700,200.893,19.1921,955.312,-8.37267,1027.29,
  6,42.22997,-38957.80933,4.33944,9.44893,468.270,6.89406,183.636,24.4618,1914.10,14.7824,903.185,
  7,39.99940,-38525.50276,4.06714,8.97575,438.270,5.25156,198.018,25.1423,1905.02,16.1388,893.765,
  8,48.37597,-45215.83000,4,8.95043,178.670,21.8360,840.538,33.4032,1774.25,0,0,
  9,48.86978,-51198.30946,4,11.7618,292.503,20.1101,910.237,33.1688,1919.37,0,0,
  10,52.69477,-52746.83318,4,11.6977,182.326,26.8142,859.207,38.6164,1826.59,0,0,
  11,57.77391,-57104.81056,4,13.7266,169.789,30.4707,836.195,43.5561,1760.46,0,0,
  12,62.95591,-60546.76385,4,15.6865,158.922,33.8029,815.064,48.1731,1693.07,0,0,
  13,67.79407,-66600.12837,4,18.0241,156.854,38.1235,814.882,53.3415,1693.79,0,0,
  14,71.63669,-74131.45483,4,21.0069,164.947,43.4931,836.264,58.3657,1750.24,0,0,
  15,18.77280,-5836.94370,2.47906,0.95806,228.734,0.45444,326.843,1.56039,1651.71,-1.3756,1671.69,
  16,22.49931,-2318.32269,3.50146,1.07558,2235.71,1.01334,1116.69,0,0,0,0,
  17,23.15547,-2635.24412,3.50055,1.02865,1550.45,0.00493,704.525,0,0,0,0,
  18,27.27642,-7766.73308,4.00392,0.01059,268.795,0.98763,1141.41,3.06904,2507.37,0,0,
  19,27.28069,-6069.03587,4,3.11942,1833.63,1.00243,847.181,0,0,0,0,
  20,15.74399,-745.37500,2.5,0,0,0,0,0,0,0,0,
  21,15.74399,-745.37500,2.5,0,0,0,0,0,0,0,0};


double CIdealData::F0( double Tau )
  {
  double S = 
    A1 + 
    A2 * Tau + 
    B * log( Tau );
    if ( C != 0.)
      S += C * log( sinh( D * Tau ));
    if ( E != 0.)
      S -= E * log( cosh( F * Tau ));
    if ( G != 0.)
      S += G * log( sinh( H * Tau ));
    if ( I != 0.)
      S -= I * log( cosh( J * Tau ));
    FINITE(S)
  return S;
  }


double CIdealData::dFdTau( double Tau )
  {
  double S = 
    A2 + 
    ( B - 1. )/ Tau;
    if ( C != 0.)
      S += C * D * cosh ( D * Tau )/sinh( D * Tau );
    if ( E != 0.)
      S -= E * F * sinh ( F * Tau )/cosh( F * Tau );
    if ( G != 0.)
      S += G * H * cosh ( H * Tau )/sinh( H * Tau );
    if ( I != 0.)
      S -= I * J * sinh ( J * Tau )/cosh( J * Tau );
  FINITE(S)
  return S;
  }

double CIdealData::dFdTauTau( double Tau )
  {
  double S, X;
  S = -( B - 1. )/ ( Tau * Tau );
  if ( C > 0. )
    {
    X = D / sinh( D * Tau );
    S -= C * X * X;
    }
//
  if ( E > 0. )
    {
    X = F / cosh( F * Tau );
    S -= E * X * X;
    }
//
  if ( G > 0. )
    {
    X = H / sinh( H * Tau );
    S -= G * X * X;
    }
//
  if ( I > 0. )
    {
    X = J / cosh( J * Tau );
    S -= I * X * X;
    }
  return S;
  }

double CISO::F0( int i )
  {
  return IdealData[i].F0( Tau );
  }

double CISO::dF0dTauTau( int i )
  {
  return IdealData[i].dFdTauTau( Tau );
  }

double CISO::dF0dTau( int i )
  {
  return IdealData[i].dFdTau( Tau );
  }

double CISO::F0( )
  {
  double Tau0 = 1. / 298.15;
  double F = log( Tau0 / Tau ) + log( Delta / DeltaN );
  for ( int i = 0; i < K_BASE; i++ )
    {
    if ( X[i] == 0. )
      continue;
    F += X[i] * ( IdealData[i].F0( Tau ) + log( X[i] ));
    FINITE(F)
    }
  return F;
  }

double CISO::dF0dTau( )
  {
  double dF = 0.;
  for ( int i = 0; i < K_BASE; i++ )
    {
    if ( X[i] == 0. )
      continue;
    double Fi = IdealData[i].dFdTau( Tau );
    if ( !_finite(Fi))
      {
      Fi = IdealData[i].dFdTau( Tau );
      }
    dF += X[i] * Fi;

    }
  return dF;
  }

double CISO::dF0dTauTau( )
  {
  double dF = 0.;
  for ( int i = 0; i < K_BASE; i++ )
    {
    if ( X[i] == 0. )
      continue;
    dF += X[i] * IdealData[i].dFdTauTau( Tau );
    }
  return dF;
  }
