#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"

double k0, k1;
double k0G, k1G;

void CalcKr( double TC )
  {
  double T = TC + TK;
  double logKp1 = 8.752 * log ( T ) +
    (-22635.63 - 29.76863 * T - 0.00526926 * T * T + 0.4927824e-6 * T * T * T +
    8.736e-12 * T * T * T * T ) / T;
  if ( logKp1 > 100. )
    logKp1 = 100.;
  double PP0 = 1.;//P / P0;
  k0 = exp ( logKp1 ) / ( PP0 * PP0 );
  double logKp4 = -0.768535 * log ( T ) +
    (4943.27 - 1.5062 * T +
    30.1018e-4 * T * T - 9.6605e-7 * T * T * T + 1.475e-10 * T * T * T * T ) / T;
  if ( logKp4 > 100. )
    logKp4 = 100.;
  k1 = exp ( logKp4 );
  }

void CalcKrG( double TC )
  {
  double T = TC + TK;
  double RT = RG * T;
  double CH4, H2O, CO, CO2, H2;
#define G(COMP) COMP = pGas[G_##COMP]->dGobr( TC );
  G(CH4)
  G(H2O)
  G(CO)
  G(CO2)
  G(H2)
  double G0 = CH4 + H2O - CO - 3. * H2;
  double G1 = CO + H2O - CO2 - H2;
  G0 /= RT;
  G1 /= RT;
  if ( G0 > 100. )
    G0 = 100.;
  if ( G1 > 100. )
    G1 = 100.;
  if ( G0 < -100. )
    G0 = -100.;
  if ( G1 < -100. )
    G1 = -100.;
  k0G = exp ( G0 );
  k1G = exp ( G1 );
  }

void Compare ( double TC )
  {
  CalcKr( TC );
  CalcKrG( TC );
  double D0 = k0G / k0;
  double D1 = k1G / k1;
  }

void TestReforming()
  {
  G_CH4 = GetGasID( "Метан" );	 
  G_C2H6 = GetGasID( "Этан" ); 
  G_C3H8 = GetGasID( "Пропан" ); 
  G_C4H10N = GetGasID( "Бутан" );
  G_C5H12N = GetGasID( "Пентан" );
  G_CO2 = GetGasID( "Диоксид углерода" ); 
  G_H2O = GetGasID( "Вода" );	 
  G_CO = GetGasID( "Монооксид углерода" );	 
  G_H2 = GetGasID( "Водород" );	 
  Compare ( 25. );
  Compare ( 700. );
  Compare ( 200. );
  Compare ( 500. );
  Compare ( 800. );
  Compare ( 1000. );
  }
