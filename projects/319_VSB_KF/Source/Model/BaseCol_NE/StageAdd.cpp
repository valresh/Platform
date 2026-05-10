#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"

#if 0
const double kGamma = 0.1;

int G_GAS, G_ABS;
double A_GAS, B_GAS;
CGamma::CGamma()
  {
  G_GAS = G_ABS = -1;
  A_Gamma = B_Gamma = 0.;
  A_Henry = B_Henry = 0.;
  T_PNT_0 = 40.;
  k_PNT_0 = 0.001;
  T_PNT_1 = 100.;
  k_PNT_1 = 100.;
  h_PNT_0 = 0.001; 
  h_PNT_1 = 100.;
  New = false;
  }

void CGamma::CalcCoef( )
  {
  double L_0 = log ( k_PNT_0 );
  double L_1 = log ( k_PNT_1 );
  B_Gamma = ( L_1 - L_0 ) / ( 1. / ( T_PNT_1 + TK ) - 1. / ( T_PNT_0 + TK ));
  A_Gamma = L_0 - B_Gamma / ( T_PNT_0 + TK );
  if ( New )
    {
    L_0 = log ( h_PNT_0 );
    L_1 = log ( h_PNT_1 );
    B_Henry = ( L_1 - L_0 ) / ( 1. / ( T_PNT_1 + TK ) - 1. / ( T_PNT_0 + TK ));
    A_Henry = L_0 - B_Henry / ( T_PNT_0 + TK );
    }
  }

bool CGamma::CalcGamma( double TC, bool IsComp[], double X[], double Gamma[]  )
  {
  ASS(G_GAS>=0)
  ASS(G_ABS>=0)
  if ( !IsComp[G_GAS] || !IsComp[G_ABS])
    return false;
  double X_GAS = X[G_GAS];
  double z0 = X_GAS;
  double X_ABS = X[G_ABS];
  double z1 = X_ABS;
  if ( New )
    {
    if ( z0 < 1e-6 )
      {
      Gamma[G_GAS] += kGamma * ( 1e-6 - Gamma[G_GAS] );
      return true;
      }
    double K = exp ( A_Gamma + B_Gamma / ( TC + TK ));
// K = Gamma*K2CO3/KHCO3^2
// x = CO2
// y = K2CO3
// z = KHCO3
    double x0 = X_GAS;
    double y0 = X_ABS;
    double a = 1. - K;
    double b = y0 + ( 2. * K - 1. ) * x0;
    double c = -K * x0 * x0;
    double det = b * b - 4. * a * c;
    if ( det < 0. )
      det = 0.;
    det = sqrt ( det );
    double x = -2. * c / ( b + det );
    double henry = exp ( A_Henry + B_Henry / ( TC + TK ));
    if ( 1e-5 * x0 > x )
      {
      Gamma[G_GAS] += kGamma * ( henry * 1e-5 - Gamma[G_GAS] );
      return true;
      }
    double gamma = x / x0;
    FINITE(gamma)
    Gamma[G_GAS] += kGamma * ( henry * gamma - Gamma[G_GAS] );
    }
  else
    {
    double k = exp ( A_Gamma + B_Gamma / ( TC + TK ));
// k =  Cgas * Cкарсол / Ссоединение
// Gamma = Cgas/Cgas0  Cgas0-растворенный CO2, Cgas-свободный CO2
// Ур-е для Gamma
//    double z0 = X_GAS;
//    double z1 = X_ABS;
    double a = z0;
    double b = z1 - z0 + k;
    double c = -k;
    double det = b * b - 4. * a * c;
    if ( det < 0. )
      det = 0.;
    double x;
    if ( fabs ( a ) > fabs ( c ))
      {
      x = ( -b + sqrt ( det )) / ( 2. * a );
      double xx = -2. * c / ( b + sqrt ( det ));
      KKK();
      }
    else
      {
      x = -2. * c / ( b + sqrt ( det ));
      double xx = ( -b + sqrt ( det )) / ( 2. * a );
      KKK();
      }
    Gamma[G_GAS] += kGamma * ( x - Gamma[G_GAS] );
    }
  return true;
  }

#endif