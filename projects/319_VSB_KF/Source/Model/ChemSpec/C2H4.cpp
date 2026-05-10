#include "stdafx.h"
#include "Chem.h"
#include "Err.h"


struct CC2H4 : public CReal
  {
  virtual double  C_IG ( double TC );
  virtual double  H_IG ( double TC );
  };

CReal * Create_C2H4()
  {
  return new CC2H4();
  }

static double Coef[8][2] = {
 -6.387880,	106.5104, //A 0
	184.4019,	13.73260, //B 1
	-112.9718,-2.628481,//C 2
	28.49593,	0.174595, //D 3
	0.315540,	-26.14469,//E 4
	48.17332,	-35.36237,//F 5
	163.1568,	275.0424, //G 6
  52.46694,	52.46694 };// H 7

double CC2H4::C_IG ( double TC )
  {
  double T = TC + TK;
  double t = T / 1000.;
  int N = 0;
  if ( T > 1200. )
    N = 1;
  double Cp = ( ( Coef[3][N] * t + Coef[2][N] ) * t + Coef[1][N] ) * t + Coef[0][N] + Coef[4][N] / ( t * t );
  return Cp;
  }

double CC2H4::H_IG ( double TC )
  {
  double T = TC + TK;
  double t = T / 1000.;
  int N = 0;
  if ( T > 1200. )
    N = 1;
  double H = ((( Coef[3][N] / 4. * t + Coef[2][N] / 3. ) * t + Coef[1][N] / 2 ) * t + Coef[0][N] ) * t  - Coef[4][N] / t;
  H *= 1000.;
  if ( CalcChem_H )
    return H + Hp_F;
  return H + Hp_F_0;
  }


