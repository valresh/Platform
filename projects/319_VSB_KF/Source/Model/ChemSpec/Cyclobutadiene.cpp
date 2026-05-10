#include "stdafx.h"
#include "Chem.h"
#include "Err.h"


struct CCyclobutadiene : public CReal
{
	virtual double  C_IG ( double TC );
	virtual double  H_IG ( double TC );
};

CReal * Create_C4H4()
{
	return new CCyclobutadiene();
}
/**/
static double Coef[][2] = 
{
	{25.334408172796799, 116.4},
	{61.491276893473525, 27.5},
	{289.43062807659010, 0.0},
	{-345.62926128529222, 0.0},
	{110.10307687080885, 0.0},
};

double CCyclobutadiene::C_IG ( double TC )
{
	double T = TC + TK;
	double t = T / 1000.;
	int N = 0;
	if ( T > 1500. )
		N = 1;
	double Cp = (((Coef[4][N] * t + Coef[3][N]) * t + Coef[2][N]) * t + Coef[1][N]) * t + Coef[0][N]; 
	return Cp;
}

double CCyclobutadiene::H_IG ( double TC )
{
	double T = TC + TK;
	double t = T / 1000.;
	int N = 0;
	if ( T > 1500. )
		N = 1;
	double H = ((((Coef[4][N] / 5.0 * t + Coef[3][N] / 4.0) * t + Coef[2][N] / 3.0) * t + Coef[1][N] / 2.0) * t + Coef[0][N]) * t; 
	H *= 1000.;
	if ( CalcChem_H )
		return H + Hp_F;
	return H + Hp_F_0;
}
