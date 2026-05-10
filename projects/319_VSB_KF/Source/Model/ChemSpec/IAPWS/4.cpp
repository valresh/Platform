#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"

static double n1 = 0.11670521452767e4;
static double n2 = -0.72421316703206e6;
static double n3 = -0.17073846940092e2;
static double n4 = 0.12020824702470e5;
static double n5 = -0.32325550322333e7;
static double n6 = 0.14915108613530e2;
static double n7 = -0.48232657361591e4;
static double n8 = 0.40511340542057e6;
static double n9 = -0.23855557567849;
static double n10 = 0.65017534844798e3;

double CIF_97::P_4 ( double T_K )
  {
  Tau = T_K + n9 / ( T_K - n10 );
  double A = ( Tau + n1 ) * Tau + n2;
  double B = ( n3 * Tau + n4 ) * Tau + n5;
  double C = ( n6 * Tau + n7 ) * Tau + n8;
#if 0
  double E = ( B * B - 4. * A * C );
  double D = 2. * C / ( -B + sqrt ( E ));
#else
  B = 2. / B;
  A *= B; C *= B;
  double D;
  double E = ( 1.0 - A * C );
  if ( E < 0. ) E = -E;
  if ( B < 0 )
    D = C / ( -1.0 - sqrt ( E ) );
  else
    D = C / ( -1.0 + sqrt ( E ) );
#endif
  double D2 = D * D;
  Pi = D2 * D2;
  return Pi;
  }

void CIF_97::P_4 ( double T_K, double & P, double & dPdT )
	{
	double w = ( T_K - n10 );
  Tau = T_K + n9 / w;
  double dTau = 1. - n9 / ( w * w );
  double A = ( Tau + n1 ) * Tau + n2;
  double dA = 2. * Tau + n1;
  double B = ( n3 * Tau + n4 ) * Tau + n5;
  double dB = 2. * n3 * Tau + n4;
  double C = ( n6 * Tau + n7 ) * Tau + n8;
  double dC = 2. * n6 * Tau + n7;
	double Det = sqrt ( B * B - 4. * A * C );
	double dDet = ( B * dB - 2. * dA * C - 2. * A * dC ) / Det;
	double Zn = -B + Det;
	double dZn = -dB + dDet;
  double D = 2. * C / Zn;
  double dD = 2. * ( dC / Zn - C * dZn / ( Zn * Zn ));
  double D2 = D * D;
  P = D2 * D2;
	dPdT = 4. * D2 * D * dD;
	}

double CIF_97::T_4 ( double P_MPa )
  {
  Pi = sqrt ( sqrt ( P_MPa ) );
  double E = ( Pi + n3 ) * Pi + n6;
  double F = ( n1 * Pi + n4 ) * Pi + n7;
  double G = ( n2 * Pi + n5 ) * Pi + n8;
  double D = 2. * G / ( -F - sqrt ( F * F - 4. * E * G ));
  Tau = 0.5 * ( n10 + D - 
    sqrt ( ( n10 + D ) * ( n10 + D ) - 4. * ( n9 + n10 * D )));
  return Tau;
  }


