#include "stdafx.h"
#include "Chem.h"
#include "Err.h"


struct CC5H10 : public CReal
{
	virtual double  C_IG ( double TC );
	virtual double  H_IG ( double TC );
};

CReal * Create_C5H10()
{
	return new CC5H10();
}

static double Coef[][2] = 
{
	{77.317164201989073, 213.19327},
	{271.74604091011287, 65.8},
	{209.53242761505780, 0.0},
	{-514.12828784497401, 0.0},
	{229.78947079306491, 0.0},
};

double CC5H10::C_IG ( double TC )
{
	double t = TC / 1000.;
	int N = 0;
	if ( t > 1.227 )
		N = 1;
	double Cp = (((Coef[4][N] * t + Coef[3][N]) * t + Coef[2][N]) * t + Coef[1][N]) * t + Coef[0][N]; 
	return Cp;
}

double CC5H10::H_IG ( double TC )
{
	double t = TC / 1000.;
	int N = 0;
	if ( t > 1.227 )
		N = 1;
	double H = ((((Coef[4][N] / 5.0 * t + Coef[3][N] / 4.0) * t + Coef[2][N] / 3.0) * t + Coef[1][N] / 2.0) * t + Coef[0][N]) * t; 
	H *= 1000.;
	if ( CalcChem_H )
		return H + Hp_F;
	return H + Hp_F_0;
}
