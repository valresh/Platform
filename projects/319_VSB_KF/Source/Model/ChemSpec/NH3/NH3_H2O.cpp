#include "stdafx.h"
#if 0
#include "math.h"
#define DLL_Chem
#include "Chem.h"
#include "CommProc.h"
#include "NH3_H2O.h"
#include "Macro.h"


//const double R_kg = 0.46151805; //kJ / kg K; 
const double T_0 = 500.;
const double Ro_0 = 15.;
const double C1_3 = 1. / 3.; 
const double iRo_H2O = 18.015483901442206 / 322.;
const double iRo_NH3 = 17.03026 / 225.;
int CNH3_H2O::kIter = 0;

void CNH3_H2O::SetX ( double x_NH3_mol )
	{
	x = x_NH3_mol;
	x1 = 1. - x;
	double Tc_12 = 0.9648407 * 0.5 * ( Tc_H2O + Tc_NH3 );
	T_1 = x1 * x1 * Tc_H2O + x * x * Tc_NH3;
	dT_1dx = 2. * ( x * Tc_NH3 - x1 * Tc_H2O );
	if ( x > 0. && x1 > 0. )
		{
		double Gamma = 1.125455;
		double x_Gamma = pow ( x, Gamma );
		T_1 += 2. * x * ( 1. - x_Gamma ) * Tc_12;
		dT_1dx += 2. * ( 1. - ( Gamma + 1. ) * x_Gamma ) * Tc_12;
		}
	double Roc_12i = 1.2395117 * 0.5 * ( iRo_H2O + iRo_NH3 );
	Ro_1i = x1 * x1 * iRo_H2O + x * x * iRo_NH3;
	dRo_1idx = 2. * ( x * iRo_NH3 - x1 * iRo_H2O );
	if ( x > 0. && x1 > 0. )
		{
		double Gamma = 0.8978069;
		double x_Gamma = pow ( x, Gamma );
		Ro_1i += 2. * x * ( 1. - x_Gamma ) * Roc_12i;
		dRo_1idx += 2. * ( 1. - ( Gamma + 1. ) * x_Gamma ) * Roc_12i;
		}
	Mu = x1 * Mw_H2O + x * Mw_NH3;
	}

int CNH3_H2O::Calc ( double Ro_kmol_m3, double TC )
	{
// F0
	T_K = TC + TK;
	t0 = T_0 / T_K;
	double log_t0 = log ( t0 );
	double t0_i = 1. / t0;
	d0 = Ro_kmol_m3 / Ro_0;
	double d0_i = 1. / d0;
//
	F0 = log ( d0 );
	mu_H2O = F0;
	mu_NH3 = F0;
	F0_d = d0_i;
	F0_dd = -d0_i * d0_i;
	F0_t = 0.;
	F0_tt = 0.;
	F0_dt = 0.;
	if ( x1 > 0. )
		{
		double F0_x1 = -7.720435 + 8.649358 * t0 + 3.006320 * log ( t0 ) + 
			             log ( x1 );
		double F0_x1_t = 8.649358 + 3.006320 / t0;
		double F0_x1_tt = -3.006320 / ( t0 * t0 );
		EXP_0(0.012436,1.666)
		EXP_0(0.97315,4.578)
		EXP_0(1.279500,10.018)
		EXP_0(0.969560,11.964)
		EXP_0(0.248730,35.600)
/////////////////////////////////////////////
		mu_H2O += F0_x1;
		F0 += x1 * F0_x1;
		F0_t += x1 * F0_x1_t;
		F0_tt += x1 * F0_x1_tt;
		}
	if ( x > 0. )
		{
		double F0_x = -16.444285 + 4.036946 * t0 - log ( t0 ) + 
			             log ( x );
		double F0_x_t = 4.036946 - t0_i;
		double F0_x_tt = t0_i * t0_i;
		POW_0(10.69955,C1_3)
		POW_0(-1.775436,-1.5)
		POW_0(0.82374034,-1.75)
/////////////////////////////////////////////
		mu_NH3 += F0_x;
		F0 += x * F0_x;
		F0_t += x * F0_x_t;
		F0_tt += x * F0_x_tt;
		}
///////////////////////////////////////////////////
	t = T_1 / T_K;
	d = Ro_kmol_m3 * Ro_1i;
	log_t = log ( t );
	ti = 1. / t;
	ti_t = -ti * ti;
	log_d = log ( d );
	di = 1. / d;
	di_d = -di * di;
	d_2 = d * d;
	d_3 = d_2 * d;
///////////////////////////////////////////////////
//
	Fr = 0.;
	Fr_d = 0.; 
	Fr_x = 0.; 
	Fr_t = 0.; 
	Fr_dd = 0.; 
	Fr_tt = 0.; 
	Fr_dt = 0.;
///////////////////////////////////////////////////
	if ( x1 > 0. )
		{
		CalcH2O();
		Fr += x1 * FH2O;
		Fr_x -= FH2O; 
		Fr_d += x1 * FH2O_d; 
		Fr_t += x1 * FH2O_t; 
		Fr_dd += x1 * FH2O_dd; 
		Fr_tt += x1 * FH2O_tt; 
		Fr_dt += x1 * FH2O_dt;
		}
	if ( x > 0. )
		{
		CalcNH3();
		Fr += x * FNH3;
		Fr_x += FNH3; 
		Fr_d += x * FNH3_d; 
		Fr_t += x * FNH3_t; 
		Fr_dd += x * FNH3_dd; 
		Fr_tt += x * FNH3_tt; 
		Fr_dt += x * FNH3_dt;
		}
	if ( x > 0. && x1 > 0. )
		{
		CalcDelta();
		Fr += DeltaF;
		Fr_d += DeltaF_d; 
		Fr_t += DeltaF_t; 
		Fr_dd += DeltaF_dd; 
		Fr_tt += DeltaF_tt; 
		Fr_dt += DeltaF_dt;
		}
/////////////////////////////////////////////////
	double RT = RG * T_K;
	P = Ro_kmol_m3 * RT * ( 1. + d * Fr_d ) * iPascal * 1e3;
	if ( P > 250. )
		return 1;
	if ( P < 0. )
		return -1;
	h = RT * ( 1. + t0 * F0_t + t * Fr_t + d * Fr_d );
	if ( CalcChem_H )
		h += x1 * Hp_F_chem_H2O + x * Hp_F_chem_NH3;
	else
		h += x1 * Hp_F_0_H2O + x * Hp_F_0_NH3;
	Cv = -RG * ( t0 * t0 * F0_tt + t * t * Fr_tt );
	double Ch = 1. + d * ( Fr_d + t * Fr_dt ); 
	double Zn = 1. + 2. * d * Fr_d + d * d * Fr_dd; 
	Cp = Cv + RG * Ch * Ch / Zn;
	s = RG * ( t0 * F0_t + t * Fr_t - F0 - Fr );
//
	F_fi = Fr_x + d / Ro_1i * dRo_1idx * Fr_d + t / T_1 * dT_1dx * Fr_t;
	Z = 1. + d * Fr_d;
	if ( x1 > 0. )
		{
		double Psi = Fr + d * Fr_d - x * F_fi;
		mu_H2O += Psi;
		Fi_H2O = exp ( Psi ) / Z;
		}
	else
		Fi_H2O = 1.;
	if ( x > 0. )
		{
		double Psi = Fr + d * Fr_d + x1 * F_fi;
		mu_NH3 += Psi;
		Fi_NH3 = exp ( Psi ) / Z;
		}
	else
		Fi_NH3 = 1.;
	mu_H2O *= RT;
	mu_H2O += mu_0_H2O;
	mu_NH3 *= RT;
	mu_NH3 += mu_0_NH3;
	return 0;
	}

void CNH3_H2O::CalcNH3()
	{
	FNH3 = 0.;
	FNH3_d = 0.; 
	FNH3_t = 0.; 
	FNH3_dd = 0.; 
	FNH3_tt = 0.; 
	FNH3_dt = 0.;
//
	POW_NH3(-1.8588140e+00,1.5,1.)
	POW_NH3(4.5544310e-02,-0.5,2.)
	POW_NH3(7.2385480e-01,0.5,1.)
	POW_NH3(1.2294700e-02,1.,4.)
	POW_NH3(2.1418820e-11,3.,15.)
//
//
	EXP_NH3(-1.4300200e-02,0.,3.,1)
	EXP_NH3(3.4413240e-01,3.,3.,1)
	EXP_NH3(-2.8735710e-01,4.,1.,1)
	EXP_NH3(2.3525890e-05,4.,8.,1)
	EXP_NH3(-3.4971110e-02,5.,2.,1)
	EXP_NH3(1.8311170e-03,5.,8.,2)
//
	EXP_NH3(2.3978520e-02,3.,1.,2)
	EXP_NH3(-4.0853750e-02,6.,1.,2)
	EXP_NH3(2.3792750e-01,8.,2.,2)
	EXP_NH3(-3.5489720e-02,8.,3.,2)
	EXP_NH3(-1.8237290e-01,10.,2.,2)
	EXP_NH3(2.2815560e-02,10.,4.,2)
	EXP_NH3(-6.6634440e-03,5.,3.,3)
	EXP_NH3(-8.8474860e-03,7.5,1.,3)
	EXP_NH3(2.2726350e-03,15.,2.,3)
	EXP_NH3(-5.5886550e-04,30.,4.,3)
	}

void CNH3_H2O::CalcH2O()
	{
	FH2O = 0.;
	FH2O_d = 0.; 
	FH2O_t = 0.; 
	FH2O_dd = 0.; 
	FH2O_tt = 0.; 
	FH2O_dt = 0.;
//
//
	POW_H2O(1.,-0.5,0.12533547935523e-1)
	POW_H2O(1.,0.875,0.78957634722828e1)
	POW_H2O(1.,1.,-0.87803203303561e1)
	POW_H2O(2.,0.5,0.31802509345418)
	POW_H2O(2.,0.75,-0.26145533859358)
	POW_H2O(3.,0.375,-0.78199751687981e-2)
	POW_H2O(4.,1.,0.88089493102134e-2)
//
//
	double d_1 = d;
	double d_2 = d_1 * d_1;
	double d_3 = d_2 * d_1;
	double d_4 = d_2 * d_2;
	double d_6 = d_3 * d_3;
//
	EXP_H2O(1,1,4,-0.66856572307965)
	EXP_H2O(1,1,6,0.20433810950965)
	EXP_H2O(1,1,12,-0.66212605039687e-4)
	EXP_H2O(1,2,1,-0.19232721156002)
	EXP_H2O(1,2,5,-0.25709043003438)
	EXP_H2O(1,3,4,0.16074868486251)
	EXP_H2O(1,4,2,-0.40092828925807e-1)
	EXP_H2O(1,4,13,0.39343422603254e-6)
	EXP_H2O(1,5,9,-0.75941377088144e-5)
	EXP_H2O(1,7,3,0.56250979351888e-3)
	EXP_H2O(1,9,4,-0.15608652257135e-4)
	EXP_H2O(1,10,11,0.11537996422951e-8)
	EXP_H2O(1,11,4,0.36582165144204e-6)
	EXP_H2O(1,13,13,-0.13251180074668e-11)
	EXP_H2O(1,15,1,-0.62639586912454e-9)
	EXP_H2O(2,1,7,-0.10793600908932)
	EXP_H2O(2,2,1,0.17611491008752e-1)
	EXP_H2O(2,2,9,0.22132295167546)
	EXP_H2O(2,2,10,-0.40247669763528)
	EXP_H2O(2,3,10,0.58083399985759)
	EXP_H2O(2,4,3,0.49969146990806e-2)
	EXP_H2O(2,4,7,-0.31358700712549e-1)
	EXP_H2O(2,4,10,-0.74315929710341)
	EXP_H2O(2,5,10,0.47807329915480)
	EXP_H2O(2,6,6,0.20527940895948e-1)
	EXP_H2O(2,6,10,-0.13636435110343)
	EXP_H2O(2,7,10,0.14180634400617e-1)
	EXP_H2O(2,9,1,0.83326504880713e-2)
	EXP_H2O(2,9,2,-0.29052336009585e-1)
	EXP_H2O(2,9,3,0.38615085574206e-1)
	EXP_H2O(2,9,4,-0.20393486513704e-1)
	EXP_H2O(2,9,8,-0.16554050063734e-2)
	EXP_H2O(2,10,6,0.19955571979541e-2)
	EXP_H2O(2,10,9,0.15870308324157e-3)
	EXP_H2O(2,12,8,-0.16388568342530e-4)
	EXP_H2O(3,3,16,0.43613615723811e-1)
	EXP_H2O(3,4,22,0.34994005463765e-1)
	EXP_H2O(3,4,23,-0.76788197844621e-1)
	EXP_H2O(3,5,23,0.22446277332006e-1)
	EXP_H2O(4,14,10,-0.62689710414685e-4)
	EXP_H2O(6,3,50,-0.55711118565645e-9)
	EXP_H2O(6,6,44,-0.19905718354408)
	EXP_H2O(6,6,46,0.31777497330738)
	EXP_H2O(6,6,50,-0.11841182425981)
//
	EXP2_H2O(3,0,-0.31306260323435e2,20,150,1.21,1)
	EXP2_H2O(3,1,0.31546140237781e2,20,150,1.21,1)
	EXP2_H2O(3,4,-0.25213154341695e4,20,250,1.25,1)
//
	double a2 = 2. * 3.5;
  double b = 0.85;
	double B = 0.2;
	double n = -0.14874640856724;
	double C = 28;
	double D = 700;
	double A = 0.32;
	double Beta = 0.3;
	double iBeta = 1. / Beta;
// Psi
  double d1 = d - 1.;
  double ad1 = fabs ( d1 );
  double t1 = t - 1.;
//
	double d2 = C * d1 * d1;
	double d2_d = 2. * C * d1;
	double d2_dd = 2. * C;
	double t2 = D * t1 * t1;
	double t2_t = 2. * D * t1;
	double t2_tt = 2. * D;
	double L_Psi = -d2 - t2;
	double L_Psi_d = -d2_d;
	double L_Psi_dd = -d2_dd;
	double L_Psi_t = -t2_t;
	double L_Psi_tt = -t2_tt;
//----------------------------
	double Teta = -t1;
	double Teta_d = 0.;
	double Teta_dd = 0.;
	if ( ad1 > 1e-10 )
		{
		double d1_i = 1. / d1;
		double Pow = A * pow ( ad1, iBeta );
		double Pow_d = iBeta * Pow * d1_i;
		double Pow_dd = ( iBeta - 1. ) * Pow_d * d1_i;
		Teta += Pow;
		Teta_d = Pow_d;
		Teta_dd = Pow_dd;
		}
//----------------------------
	double Delta = Teta * Teta; 
	double Delta_t = -2. * Teta;
	double Delta_dt = -2. * Teta_d;
	double Delta_tt = -2.;
	double Delta_d = 2. * Teta * Teta_d;
	double Delta_dd = 2. * ( Teta_d * Teta_d + Teta * Teta_dd );
	if ( ad1 > 1e-10 )
		{
		double d1_i = 1. / d1;
		double Pow = B * pow ( ad1, a2 );
		double Pow_d = a2 * Pow * d1_i;
		double Pow_dd = ( a2 - 1. ) * Pow_d * d1_i;
		Delta += Pow;
		Delta_d += Pow_d;
		Delta_dd += Pow_dd;
		}
	double Delta_i = 1. / Delta;
	double Delta_2i = Delta_i * Delta_i;
	double bDelta_i = Delta_i * b;
	double bDelta_2i = Delta_2i * b;
	double F = b * log ( Delta ) + log_d + L_Psi;
	double F_d = bDelta_i * Delta_d + di + L_Psi_d;
	double F_dd = bDelta_i * Delta_dd - bDelta_2i * Delta_d * Delta_d + 
		            di_d + L_Psi_dd;
	double F_t = bDelta_i * Delta_t + L_Psi_t;
	double F_tt = bDelta_i * Delta_tt - bDelta_2i * Delta_t * Delta_t + 
		            L_Psi_tt;
	double F_dt = bDelta_i * Delta_dt - bDelta_2i * Delta_d * Delta_t; 
	double V = n * exp ( F );
  double V_t = F_t * V;
  double V_tt = ( F_tt + F_t * F_t ) * V;
  double V_d = F_d * V;
  double V_dd = ( F_dd + F_d * F_d ) * V;
  double V_dt = F_d * F_t * V;
	FH2O += V;
	FH2O_d += V_d;
	FH2O_dd += V_dd;
	FH2O_dt += V_dt;
	FH2O_t += V_t;
	FH2O_tt += V_tt;
////////////////////////////////////////////////
//	a = 3.5;
	b = 0.95;
//	B = 0.2;
	n = 0.31806110878444;
	C = 32.;
	D = 800.;
//	A = 0.32;
//	Gamma = 0.3;
	d2 = C * d1 * d1;
	d2_d = 2. * C * d1;
	d2_dd = 2. * C;
	t2 = D * t1 * t1;
	t2_t = 2. * D * t1;
	t2_tt = 2. * D;
	L_Psi = -d2 - t2;
	L_Psi_d = -d2_d;
	L_Psi_dd = -d2_dd;
	L_Psi_t = -t2_t;
	L_Psi_tt = -t2_tt;
//----------------------------
	Teta = -t1;
	Teta_d = 0.;
	Teta_dd = 0.;
	if ( ad1 > 1e-10 )
		{
		double d1_i = 1. / d1;
		double Pow = A * pow ( ad1, iBeta );
		double Pow_d = iBeta * Pow * d1_i;
		double Pow_dd = ( iBeta - 1. ) * Pow_d * d1_i;
		Teta += Pow;
		Teta_d = Pow_d;
		Teta_dd = Pow_dd;
		}
//----------------------------
	Delta = Teta * Teta; 
	Delta_t = -2. * Teta;
	Delta_dt = -2. * Teta_d;
	Delta_tt = -2.;
	Delta_d = 2. * Teta * Teta_d;
	Delta_dd = 2. * ( Teta_d * Teta_d + Teta * Teta_dd );
	if ( ad1 > 1e-10 )
		{
		double d1_i = 1. / d1;
		double Pow = B * pow ( ad1, a2 );
		double Pow_d = a2 * Pow * d1_i;
		double Pow_dd = ( a2 - 1. ) * Pow_d * d1_i;
		Delta += Pow;
		Delta_d += Pow_d;
		Delta_dd += Pow_dd;
		}
	Delta_i = 1. / Delta;
	Delta_2i = Delta_i * Delta_i;
	bDelta_i = Delta_i * b;
	bDelta_2i = Delta_2i * b;
	F = b * log ( Delta ) + log_d + L_Psi;
	F_d = bDelta_i * Delta_d + di + L_Psi_d;
	F_dd = bDelta_i * Delta_dd - bDelta_2i * Delta_d * Delta_d + 
	       di_d + L_Psi_dd;
	F_t = bDelta_i * Delta_t + L_Psi_t;
	F_tt = bDelta_i * Delta_tt - bDelta_2i * Delta_t * Delta_t + 
		            L_Psi_tt;
	F_dt = bDelta_i * Delta_dt - bDelta_2i * Delta_d * Delta_t; 
	V = n * exp ( F );
  V_t = F_t * V;
  V_tt = ( F_tt + F_t * F_t ) * V;
  V_d = F_d * V;
  V_dd = ( F_dd + F_d * F_d ) * V;
  V_dt = F_d * F_t * V;
//
	FH2O += V;
	FH2O_d += V_d;
	FH2O_dd += V_dd;
	FH2O_dt += V_dt;
	FH2O_t += V_t;
	FH2O_tt += V_tt;
/////////////////////////////////////////////////
	}

void CNH3_H2O::CalcDelta()
	{
	DeltaF = 0.;
	DeltaF_d = 0.; 
	DeltaF_t = 0.; 
	DeltaF_dd = 0.; 
	DeltaF_tt = 0.; 
	DeltaF_dt = 0.;
	double DeltaF_x = 0.;
	if ( x <= 0. || x1 <= 0. )
		return ;
/////////////////////////////////////////////////////////////
	double D = 0.;
	double D_d = 0.;
	double D_dd = 0.;
	double D_dt = 0.;
	double D_t = 0.;
	double D_tt = 0.;
	POW_DELTA(-1.855822e-02,1.5,4.)
////////////////////////////////////////////////
	EXP_DELTA(5.258010e-02,0.5,5.,1)
	EXP_DELTA(3.552874e-10,6.5,15.,1)
	EXP_DELTA(5.451379e-06,1.75,12.,1)
	EXP_DELTA(-5.998546e-13,15.,12,1)
	EXP_DELTA(-3.687808e-06,6.,15.,2)
//
	DeltaF += D;
	DeltaF_d += D_d;
	DeltaF_t += D_t;
	DeltaF_dd += D_dd;
	DeltaF_tt += D_tt;
	DeltaF_dt += D_dt;
	D = 0.;
	D_d = 0.;
	D_dd = 0.;
	D_dt = 0.;
	D_t = 0.;
	D_tt = 0.;
	EXP_DELTA(0.2586192,-1.,4.,1)
	EXP_DELTA(-1.368072e-08,4.,15.,1)
	EXP_DELTA(1.226146e-02,3.5,4.,1)
	EXP_DELTA(-7.181443e-02,0.,5.,1)
	EXP_DELTA(9.970849e-02,-1.,6.,2)
	EXP_DELTA(1.0584086e-03,8.,10.,2)
	EXP_DELTA(-0.1963687,7.5,6.,2)
	DeltaF += x * D;
	DeltaF_d += x * D_d;
	DeltaF_t += x * D_t;
	DeltaF_dd += x * D_dd;
	DeltaF_tt += x * D_tt;
	DeltaF_dt += x * D_dt;
	DeltaF_x += D;
	D = 0.;
	D_d = 0.;
	D_dd = 0.;
	D_dt = 0.;
	D_t = 0.;
	D_tt = 0.;
	EXP_DELTA(-0.7777897,4.,2.,2)
	double xx = x * x;
	DeltaF += xx * D;
	DeltaF_d += xx * D_d;
	DeltaF_t += xx * D_t;
	DeltaF_dd += xx * D_dd;
	DeltaF_tt += xx * D_tt;
	DeltaF_dt += xx * D_dt;
	DeltaF_x += 2. * x * D;
//
	double Gamma = 0.5248379;
	double x_Gamma = pow ( x, Gamma );
	double K = x * ( 1. - x_Gamma );
	Fr_x += ( 1. - ( Gamma + 1. ) * x_Gamma ) * DeltaF + 
		       K * DeltaF_x; 
	DeltaF *= K;
	DeltaF_t *= K;
	DeltaF_tt *= K;
	DeltaF_d *= K;
	DeltaF_dd *= K;
	DeltaF_dt *= K;
	}
/*
double CNH3_H2O::Pdew_iter ( double y_NH3, double TC, double Ro0,
		                         CThermoData * pLiq, CThermoData * pGas )
	{
	double Ro_Liq, Ro_Gas, Cp_Liq, Cp_Gas, h_Liq, s_Liq, h_Gas, s_Gas;
	double x_NH3;
	double Ro_0_liq = 50.;
	double RT = RG * ( TC + TK );
	double F;
	double Fi_H2O_liq = 1.;
	double Fi_NH3_liq = 1.;
	double Fi_H2O_gas = 1.;
	double Fi_NH3_gas = 1.;
	if ( pLiq )
		{
		Fi_H2O_liq = pLiq ->Fi_H2O_liq;
		Fi_NH3_liq = pLiq ->Fi_NH3_liq;
		}
	double Rob = Ro0;
	double Roe = Ro0;
	int kIter = 0;
	for ( int k = 0; k < 100; k++ )
		{
		kIter++;
		SetX ( y_NH3 );
		Ro_Gas = Rob;
		int Res = Calc ( Rob, TC );
		if ( Res == 0 )
			{
			F = ( 1. - y_NH3 ) * Fi_H2O / Fi_H2O_liq + y_NH3 * Fi_NH3 / Fi_NH3_liq - 1.;
			F = -F;
			}
		else
			{
			if ( Res > 0 )
				F = -1.;
			else
				F = 1.;
			}
		if ( F > 0. )
			{
			while ( F > 0. )
				{
				Rob = Roe;
				Roe *= 1.01;
				Res = Calc ( Roe, TC );
				if ( Res == 0 )
					{
					F = ( 1. - y_NH3 ) * Fi_H2O / Fi_H2O_liq + y_NH3 * Fi_NH3 / Fi_NH3_liq - 1.;
					F = -F;
					}
				else
					{
					if ( Res > 0 )
						F = -1.;
					else
						F = 1.;
					}
				}
			}
		else
			{
			while ( F < 0. )
				{
				Roe = Rob;
				Rob *= 0.99;
				Res = Calc ( Rob, TC );
				if ( Res == 0 )
					{
					F = ( 1. - y_NH3 ) * Fi_H2O / Fi_H2O_liq + y_NH3 * Fi_NH3 / Fi_NH3_liq - 1.;
					F = -F;
					}
				else
					{
					if ( Res > 0 )
						F = -1.;
					else
						F = 1.;
					}
				}
			}
		while ( Roe - Rob > 1e-14 )
			{
			Ro_Gas = 0.5 * ( Rob + Roe );
			Res = Calc ( Ro_Gas, TC );
			if ( Res == 0 )
				{
				F = ( 1. - y_NH3 ) * Fi_H2O / Fi_H2O_liq + y_NH3 * Fi_NH3 / Fi_NH3_liq - 1.;
				F = -F;
				}
			else
				{
				if ( Res > 0 )
					F = -1.;
				else
					F = 1.;
				}
			if ( F < 0. )
				Roe = Ro_Gas;
			else
				Rob = Ro_Gas;
			}
		Calc ( Ro_Gas, TC );
		double Ro_Gas_kg = Ro_Gas * Mu;
		h_Gas = h;
		s_Gas = s;
		Fi_H2O_gas = Fi_H2O;
		Fi_NH3_gas = Fi_NH3;
		Cp_Gas = Cp;
		x_NH3 = y_NH3 * Fi_NH3 / Fi_NH3_liq;
		if ( x_NH3 < 0. )
			x_NH3 = 0.;
		if ( y_NH3 > 1. )
			x_NH3 = 1.;
		SetX ( x_NH3 );
		double Ro_0_liq = 60.;
		Ro_Liq = Ro_liq_mol ( P, TC, Ro_0_liq );
		Cp_Liq = Cp;
		if ( Ro_Liq == 0. )
			{
			h_Liq = 0.;
			s_Liq = 0.;
			break;
			}
		h_Liq = h;
		s_Liq = s;
		bool Break = true;
		if ( x_NH3 > 0. )
			{
			if ( fabs ( Fi_NH3_liq - Fi_NH3 ) > 1e-7 )
				Break = false;
			}
		if ( x_NH3 < 1. )
			{
			if ( fabs ( Fi_H2O_liq - Fi_H2O ) > 1e-7 )
				Break = false;
			}
		Fi_NH3_liq = 0.99 * Fi_NH3_liq + 0.01 * Fi_NH3;
		Fi_H2O_liq = 0.99 * Fi_H2O_liq + 0.01 * Fi_H2O;
		if ( Break )
			break;
		}
	if ( pLiq )
		{
		pLiq->P = P;
		pLiq->T = TC;
		pLiq->h = h_Liq;
		pLiq->s = s_Liq; 
		pLiq->Ro = Ro_Liq;
		pLiq->x_NH3 = x_NH3;
		pLiq->Fi_H2O_gas = Fi_H2O_gas;
		pLiq->Fi_NH3_gas = Fi_NH3_gas;
		pLiq->Fi_H2O_liq = Fi_H2O_liq;
		pLiq->Fi_NH3_liq = Fi_NH3_liq;
		pLiq->kIter = kIter;
		pLiq->Cp = Cp_Liq;
		pLiq->F = F;
		}
	if ( pGas )
		{
		pGas->P = P;
		pGas->T = TC;
		pGas->h = h_Gas;
		pGas->s = s_Gas; 
		pGas->Ro = Ro_Gas;
		pGas->x_NH3 = y_NH3;
		pGas->Fi_H2O_gas = Fi_H2O_gas;
		pGas->Fi_NH3_gas = Fi_NH3_gas;
		pGas->Fi_H2O_liq = Fi_H2O_liq;
		pGas->Fi_NH3_liq = Fi_NH3_liq;
		pGas->kIter = kIter;
		pGas->Cp = Cp_Gas;
		pGas->F = F;
		}
	return P;
	}
*/

double CNH3_H2O::Pdew_iter ( double y_NH3, double TC, double Ro0,
		                         CThermoData * pLiq, CThermoData * pGas )
	{
	double Ro_Liq, Ro_Gas, Cp_Liq, Cp_Gas, h_Liq, s_Liq, h_Gas, s_Gas;
	double x_NH3 = pLiq ->x_NH3;
	double Ro_0_liq = 50.;
	double RT = RG * ( TC + TK );
	double F;
	double Fi_H2O_gas = 1.;
	double Fi_NH3_gas = 1.;
	double Fi_H2O_liq;
	double Fi_NH3_liq;
	if ( pLiq )
		{
		Fi_H2O_gas = pLiq ->Fi_H2O_gas;
		Fi_NH3_gas = pLiq ->Fi_NH3_gas;
		}
	double Rob = Ro0;
	double Roe = Ro0;
	int kIter = 0;
	for ( int k = 0; k < 200; k++ )
		{
		kIter++;
		SetX ( x_NH3 );
		Ro_Liq = Rob;//Ro_liq_mol ( P, TC, Ro_0_liq );
		int Res = Calc ( Rob, TC );
		if ( Res == 0 )
			{
			F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
			}
		else
			{
			if ( Res > 0 )
				F = -1.;
			else
				F = 1.;
			}
		if ( F > 0. )
			{
			while ( F > 0. )
				{
				Rob = Roe;
				Roe *= 1.01;
				Res = Calc ( Roe, TC );
				if ( Res == 0 )
					F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
				else
					{
					if ( Res > 0 )
						F = -1.;
					else
						F = 1.;
					}
				}
			}
		else
			{
			while ( F < 0. )
				{
				Roe = Rob;
				Rob *= 0.99;
				Res = Calc ( Rob, TC );
				if ( Res == 0 )
					F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
				else
					{
					if ( Res > 0 )
						F = -1.;
					else
						F = 1.;
					}
				}
			}
		while ( Roe - Rob > 1e-14 )
			{
			Ro_Liq = 0.5 * ( Rob + Roe );
			Res = Calc ( Ro_Liq, TC );
			if ( Res == 0 )
				F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
			else
				{
				if ( Res > 0 )
					F = -1.;
				else
					F = 1.;
				}
			if ( F < 0. )
				Roe = Ro_Liq;
			else
				Rob = Ro_Liq;
			}
		Calc ( Ro_Liq, TC );
		double Ro_Liq_kg = Ro_Liq * Mu;
		h_Liq = h;
		s_Liq = s;
		Fi_H2O_liq = Fi_H2O;
		Fi_NH3_liq = Fi_NH3;
		Cp_Liq = Cp;
		//y_NH3 = x_NH3 * Fi_NH3 / Fi_NH3_gas;
		//if ( y_NH3 < 0. )
		//	y_NH3 = 0.;
		//if ( y_NH3 > 1. )
		//	y_NH3 = 1.;
		x_NH3 = 0.5 * x_NH3 + 0.5 * y_NH3 * Fi_NH3_gas / Fi_NH3;
		if ( x_NH3 < 0. )
			x_NH3 = 0.;
		if ( x_NH3 > 1. )
			x_NH3 = 1.;
		SetX ( y_NH3 );
		double Ro_0_gas = P * iMPa_atm * 1e3 / RT;
		Ro_Gas = Ro_gas_mol ( P, TC, Ro_0_gas );
		Cp_Gas = Cp;
		if ( Ro_Gas == 0. )
			{
			h_Gas = 0.;
			s_Gas = 0.;
			break;
			}
		h_Gas = h;
		s_Gas = s;
		bool Break = true;
		if ( x_NH3 > 0. )
			{
			if ( fabs ( Fi_NH3_gas - Fi_NH3 ) > 1e-7 )
				Break = false;
			}
		if ( x_NH3 < 1. )
			{
			if ( fabs ( Fi_H2O_gas - Fi_H2O ) > 1e-7 )
				Break = false;
			}
		//Fi_NH3_gas = 0.9 * Fi_NH3_gas + 0.1 * Fi_NH3;
		//Fi_H2O_gas = 0.9 * Fi_H2O_gas + 0.1 * Fi_H2O;
		Fi_NH3_gas = Fi_NH3;
		Fi_H2O_gas = Fi_H2O;
		if ( Break )
			break;
		}
	if ( pLiq )
		{
		pLiq->P = P;
		pLiq->T = TC;
		pLiq->h = h_Liq;
		pLiq->s = s_Liq; 
		pLiq->Ro = Ro_Liq;
		pLiq->x_NH3 = x_NH3;
		pLiq->Fi_H2O_gas = Fi_H2O_gas;
		pLiq->Fi_NH3_gas = Fi_NH3_gas;
		pLiq->Fi_H2O_liq = Fi_H2O_liq;
		pLiq->Fi_NH3_liq = Fi_NH3_liq;
		pLiq->kIter = kIter;
		pLiq->Cp = Cp_Liq;
		pLiq->F = F;
		}
	if ( pGas )
		{
		pGas->P = P;
		pGas->T = TC;
		pGas->h = h_Gas;
		pGas->s = s_Gas; 
		pGas->Ro = Ro_Gas;
		pGas->x_NH3 = y_NH3;
		pGas->Fi_H2O_gas = Fi_H2O_gas;
		pGas->Fi_NH3_gas = Fi_NH3_gas;
		pGas->Fi_H2O_liq = Fi_H2O_liq;
		pGas->Fi_NH3_liq = Fi_NH3_liq;
		pGas->kIter = kIter;
		pGas->Cp = Cp_Gas;
		pGas->F = F;
		}
	return P;
	}

double CNH3_H2O::Psat_iter ( double x_NH3, double TC, double Ro0,
		                         CThermoData * pLiq, CThermoData * pGas )
	{
	double Ro_Liq, Ro_Gas, Cp_Liq, Cp_Gas, h_Liq, s_Liq, h_Gas, s_Gas;
	double y_NH3;
	double Ro_0_liq = 50.;
	double RT = RG * ( TC + TK );
	double F;
	double Fi_H2O_gas = 1.;
	double Fi_NH3_gas = 1.;
	double mu_H2O_liq, mu_NH3_liq;
	double mu_H2O_gas, mu_NH3_gas;
	double Fi_H2O_liq;
	double Fi_NH3_liq;
	double F_fi_liq;
	double F_fi_gas;
	if ( pLiq )
		{
		Fi_H2O_gas = pLiq ->Fi_H2O_gas;
		Fi_NH3_gas = pLiq ->Fi_NH3_gas;
		}
	double Rob = Ro0;
	double Roe = Ro0;
	int kIter = 0;
	for ( int k = 0; k < 100; k++ )
		{
		kIter++;
		SetX ( x_NH3 );
		Ro_Liq = Rob;//Ro_liq_mol ( P, TC, Ro_0_liq );
		int Res = Calc ( Rob, TC );
		if ( Res == 0 )
			{
			F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
			}
		else
			{
			if ( Res > 0 )
				F = -1.;
			else
				F = 1.;
			}
		if ( F > 0. )
			{
			while ( F > 0. )
				{
				Rob = Roe;
				Roe *= 1.01;
				Res = Calc ( Roe, TC );
				if ( Res == 0 )
					F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
				else
					{
					if ( Res > 0 )
						F = -1.;
					else
						F = 1.;
					}
				}
			}
		else
			{
			while ( F < 0. )
				{
				Roe = Rob;
				Rob *= 0.99;
				Res = Calc ( Rob, TC );
				if ( Res == 0 )
					F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
				else
					{
					if ( Res > 0 )
						F = -1.;
					else
						F = 1.;
					}
				}
			}
		while ( Roe - Rob > 1e-14 )
			{
			Ro_Liq = 0.5 * ( Rob + Roe );
			Res = Calc ( Ro_Liq, TC );
			if ( Res == 0 )
				F = ( 1. - x_NH3 ) * Fi_H2O / Fi_H2O_gas + x_NH3 * Fi_NH3 / Fi_NH3_gas - 1.;
			else
				{
				if ( Res > 0 )
					F = -1.;
				else
					F = 1.;
				}
			if ( F < 0. )
				Roe = Ro_Liq;
			else
				Rob = Ro_Liq;
			}
		Calc ( Ro_Liq, TC );
	  F_fi_liq = F_fi;
		mu_H2O_liq = mu_H2O;
		mu_NH3_liq = mu_NH3;
		double Ro_Liq_kg = Ro_Liq * Mu;
		h_Liq = h;
		s_Liq = s;
		Fi_H2O_liq = Fi_H2O;
		Fi_NH3_liq = Fi_NH3;
		Cp_Liq = Cp;
		y_NH3 = x_NH3 * Fi_NH3 / Fi_NH3_gas;
		if ( y_NH3 < 0. )
			y_NH3 = 0.;
		if ( y_NH3 > 1. )
			y_NH3 = 1.;
		SetX ( y_NH3 );
		double Ro_0_gas = P * iMPa_atm * 1e3 / RT;
		Ro_Gas = Ro_gas_mol ( P, TC, Ro_0_gas );
	  F_fi_gas = F_fi;
		mu_H2O_gas = mu_H2O;
		mu_NH3_gas = mu_NH3;
		Cp_Gas = Cp;
		if ( Ro_Gas == 0. )
			{
			h_Gas = 0.;
			s_Gas = 0.;
			break;
			}
		h_Gas = h;
		s_Gas = s;
		bool Break = true;
		if ( x_NH3 > 0. )
			{
			if ( fabs ( Fi_NH3_gas - Fi_NH3 ) > 1e-7 )
				Break = false;
			}
		if ( x_NH3 < 1. )
			{
			if ( fabs ( Fi_H2O_gas - Fi_H2O ) > 1e-7 )
				Break = false;
			}
		Fi_NH3_gas = Fi_NH3;
		Fi_H2O_gas = Fi_H2O;
		if ( Break )
			break;
		}
	if ( pLiq )
		{
		pLiq->P = P;
		pLiq->T = TC;
		pLiq->h = h_Liq;
		pLiq->s = s_Liq; 
		pLiq->Ro = Ro_Liq;
		pLiq->x_NH3 = x_NH3;
		pLiq->Fi_H2O_gas = Fi_H2O_gas;
		pLiq->Fi_NH3_gas = Fi_NH3_gas;
		pLiq->Fi_H2O_liq = Fi_H2O_liq;
		pLiq->Fi_NH3_liq = Fi_NH3_liq;
		pLiq->kIter = kIter;
		pLiq->Cp = Cp_Liq;
		pLiq->F = F;
		pLiq->mu_H2O = mu_H2O_liq;
		pLiq->mu_NH3 = mu_NH3_liq;
		}
	if ( pGas )
		{
		pGas->P = P;
		pGas->T = TC;
		pGas->h = h_Gas;
		pGas->s = s_Gas; 
		pGas->Ro = Ro_Gas;
		pGas->x_NH3 = y_NH3;
		pGas->Fi_H2O_gas = Fi_H2O_gas;
		pGas->Fi_NH3_gas = Fi_NH3_gas;
		pGas->Fi_H2O_liq = Fi_H2O_liq;
		pGas->Fi_NH3_liq = Fi_NH3_liq;
		pGas->kIter = kIter;
		pGas->Cp = Cp_Gas;
		pGas->F = F;
		pGas->mu_H2O = mu_H2O_gas;
		pGas->mu_NH3 = mu_NH3_gas;
		}
	return P;
	}


const double kP_0 = RG * iPascal * 1e3;

double CNH3_H2O::Ro_liq_mol ( double Patm, double TC, double Ro0 )
	{
	double Ro_mol = Ro0;
	double T_K = TC + TK;
	double kP = kP_0 * T_K;
	double dRo = 1e10;
	while ( fabs ( dRo ) > 1e-10 )
		{
		Calc ( Ro_mol, TC );
		double F = P - Patm;
		double dF = 1. + 2. * d * Fr_d + d * d * Fr_dd;
		dF *= kP;
		dRo = LimdX ( -F / dF, 1. );
		Ro_mol += dRo;
		}
	return Ro_mol;
	}

double CNH3_H2O::Ro_gas_mol ( double Patm, double TC, double Ro0 )
	{
	if ( Patm > 1000. )
		return 0.;
	double Ro_mol = Ro0;
	double T_K = TC + TK;
	double kP = kP_0 * T_K;
	kIter = 0;
	double dRo = 1e10;
	double dP = 1e10;
	while ( fabs ( dP ) > 1e-5 )
		{
		kIter++;
		Calc ( Ro_mol, TC );
		double F = P - Patm;
		dP = F;
		if ( P < 0. )
			{
			Ro_mol *= 0.5;
			dP = 1.;
			continue;
			}
		double Pt = kP * d * ( 1. + d * Fr_d ) / ( P * Ro_1i );
		double dF = 1. + 2. * d * Fr_d + d * d * Fr_dd; 
		if ( dF < 0. && F > 0. )
			{
//			h = 0.;
//			return 0.;
			}
		dF *= kP;
		double D = -F / dF;
		dRo = LimdX ( -F / dF, Ro_mol );
		Ro_mol += dRo;
		FINITE(Ro_mol)
		if ( kIter > 40 )
			break;
		}
	if ( kIter > 20 )
    {
    Fsend ( "NH3_H2O.txt", "x=%7.5lf P=%7.5lf T=%7.5lf dRo=12.5le\n",
        x, Patm, TC, dRo );
    return Ro0;
    }
	return Ro_mol;
	}

double CNH3_H2O::Hp_F_chem_H2O = 0.;
double CNH3_H2O::Hp_F_0_H2O = 0.;
double CNH3_H2O::Hp_F_chem_NH3 = 0.;
double CNH3_H2O::Hp_F_0_NH3 = 0.;
double CNH3_H2O::mu_0_H2O = 0.;
double CNH3_H2O::mu_0_NH3 = 0.;

void CNH3_H2O::SetH()
	{
	CNH3_H2O NH3_H2O;
	NH3_H2O.SetX ( 0. );
	double Ro = NH3_H2O.Ro_gas_mol ( 0.01, 25., 1e-4 );
	double H_25 = NH3_H2O.h;
  double Hst = pGas[G_H2O]->dHobr ( 25. );
	Hp_F_chem_H2O = Hst - H_25;
	Hp_F_0_H2O = -H_25;
	Ro = NH3_H2O.Ro_gas_mol ( 0.01, 25., Ro );
	mu_0_H2O = -NH3_H2O.mu_H2O;
//
	NH3_H2O.SetX ( 1. );
	Ro = NH3_H2O.Ro_gas_mol ( Pstd, 25., 1e-4 );
	H_25 = NH3_H2O.h;
  Hst = pGas[G_NH3]->dHobr ( 25. );
	Hp_F_chem_NH3 = Hst - H_25;
	Hp_F_0_NH3 = -H_25;
	Ro = NH3_H2O.Ro_gas_mol ( Pstd, 25., Ro );
	mu_0_NH3 = -NH3_H2O.mu_NH3;
	}

#endif