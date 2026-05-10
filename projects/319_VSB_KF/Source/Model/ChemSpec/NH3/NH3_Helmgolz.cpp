#include "stdafx.h"
#if 0
#include "math.h"
#define DLL_Chem
#include "Chem.h"
#include "Macro.h"
#include "CommProc.h"
#include "NH3_H2O.h"

const double T_0 = 500.;
const double Ro_0 = 15.;
const double C1_3 = 1. / 3.; 
const double iRo_NH3 = 17.03026 / 225.;

int CNH3Helmholtz::Calc ( double Ro_kmol_m3, double TC )
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
	F0_d = d0_i;
	F0_dd = -d0_i * d0_i;
	F0_t = 0.;
	F0_tt = 0.;
	F0_dt = 0.;
	double F0_x = -16.444285 + 4.036946 * t0 - log ( t0 );
	double F0_x_t = 4.036946 - t0_i;
	double F0_x_tt = t0_i * t0_i;
	POW_0(10.69955,C1_3)
	POW_0(-1.775436,-1.5)
	POW_0(0.82374034,-1.75)
/////////////////////////////////////////////
	F0 += F0_x;
	F0_t += F0_x_t;
	F0_tt += F0_x_tt;
///////////////////////////////////////////////////
	t = Tc_NH3 / T_K;
	d = Ro_kmol_m3 * iRo_NH3;
	log_t = log ( t );
	ti = 1. / t;
	ti_t = -ti * ti;
	log_d = log ( d );
	di = 1. / d;
	di_d = -di * di;
	d_2 = d * d;
	d_3 = d_2 * d;
///////////////////////////////////////////////////
	CalcNH3();
	Fr = FNH3;
	Fr_d = FNH3_d; 
	Fr_t = FNH3_t; 
	Fr_dd = FNH3_dd; 
	Fr_tt = FNH3_tt; 
	Fr_dt = FNH3_dt;
/////////////////////////////////////////////////
	double RT = RG * T_K;
	P = Ro_kmol_m3 * RT * ( 1. + d * Fr_d ) * iPascal * 1e3;
	if ( P > 250. )
		return 1;
	if ( P < 0. )
		return -1;
	h = RT * ( 1. + t0 * F0_t + t * Fr_t + d * Fr_d );
	if ( CalcChem_H )
		h += Hp_F_chem;
	else
		h += Hp_F_0;
	Cv = -RG * ( t0 * t0 * F0_tt + t * t * Fr_tt );
	double Ch = 1. + d * ( Fr_d + t * Fr_dt ); 
	double Zn = 1. + 2. * d * Fr_d + d * d * Fr_dd; 
	Cp = Cv + RG * Ch * Ch / Zn;
	s = RG * ( t0 * F0_t + t * Fr_t - F0 - Fr );
//
	Z = 1. + d * Fr_d;
	Fi_NH3 = exp ( Fr + d * Fr_d ) / Z;
	return 0;
	}

void CNH3Helmholtz::CalcNH3()
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

const double kP_0 = RG * iPascal * 1e3;

double CNH3Helmholtz::Ro_liq_mol ( double Patm, double TC, double Ro0 )
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

double CNH3Helmholtz::Ro_gas_mol ( double Patm, double TC, double Ro0 )
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
		if ( dF < 0. )
			return 0.;
		dF *= kP;
		dRo = LimdX ( -F / dF, 0.01 );
		if ( dRo < -0.1 * Ro_mol )
			dRo = -0.1 * Ro_mol;
		Ro_mol += dRo;
		}
	return Ro_mol;
	}

/************
double CNH3Helmholtz::Psat ( double TC, double P0,
	                  CThermoData * pLiq, CThermoData * pGas )
	{
	double Ro_Liq, Ro_Gas, h_Liq, s_Liq, h_Gas, s_Gas;
	double Ro_0_liq = 50.;
	double RT = RG * ( TC + TK );
	double F;
	double Fi_NH3_gas = 1.;
	double Pb = P0;
	double Pe = P0;
	for ( int k = 0; k < 100; k++ )
		{
		Ro_Liq = Ro_liq_mol ( P, TC, Ro_0_liq );
		Ro_0_liq = Ro_Liq;
		F = Fi_NH3 - Fi_NH3_gas;
		if ( F > 0. )
			{
			while ( F > 0. )
				{
				Pb = Pe;
				Pe *= 1.1;
				Ro_Liq = Ro_liq_mol ( Pe, TC, Ro_0_liq );
				Ro_0_liq = Ro_Liq;
				F = Fi_NH3 - Fi_NH3_gas;
				}
			}
		else
			{
			while ( F < 0. )
				{
				Pe = Pb;
				Pb *= 0.9;
				Ro_Liq = Ro_liq_mol ( Pb, TC, Ro_0_liq );
				Ro_0_liq = Ro_Liq;
				F = Fi_NH3 - Fi_NH3_gas;
				}
			}
		while ( Pe - Pb > 1e-8 )
			{
			P = 0.5 * ( Pb + Pe );
			Ro_Liq = Ro_liq_mol ( P, TC, Ro_0_liq );
			Ro_0_liq = Ro_Liq;
			F = Fi_NH3 - Fi_NH3_gas;
			if ( F < 0. )
				Pe = P;
			else
				Pb = P;
			}
		h_Liq = h;
		s_Liq = s;
		double Ro_0_gas = P * iMPa_atm * 1e3 / RT;
		Ro_Gas = Ro_gas_mol ( P, TC, Ro_0_gas );
		if ( Ro_Gas == 0. )
			break;
		h_Gas = h;
		s_Gas = s;
		if ( fabs ( Fi_NH3_gas - Fi_NH3 ) < 1e-7 )
			break;
		Fi_NH3_gas = Fi_NH3;
		}
	if ( pLiq )
		{
		pLiq->P = P;
		pLiq->T = TC;
		pLiq->h = h_Liq;
		pLiq->s = s_Liq; 
		pLiq->Ro = Ro_Liq;
		pLiq->x_NH3 = 1.;
		}
	if ( pGas )
		{
		pGas->P = P;
		pGas->T = TC;
		pGas->h = h_Gas;
		pGas->s = s_Gas; 
		pGas->Ro = Ro_Gas;
		pGas->x_NH3 = 1.;
		}
	return P;
	}
************/

double CNH3Helmholtz::Psat_iter ( double TC, double Ro0,
		                     CThermoData * pLiq, CThermoData * pGas )
	{
	double Ro_Liq, Ro_Gas, Cp_Liq, Cp_Gas, h_Liq, s_Liq, h_Gas, s_Gas;
	double Ro_0_liq = Ro0;
	double RT = RG * ( TC + TK );
	double F;
	double Fi_NH3_gas = 1.;
	double Fi_NH3_liq;
	if ( pLiq )
		{
		Fi_NH3_gas = pLiq ->Fi_NH3_gas;
		}
	double Rob = Ro0;
	double Roe = Ro0;
	int kIter = 0;
	for ( int k = 0; k < 100; k++ )
		{
		kIter++;
		Ro_Liq = Rob;
		int Res = Calc ( Rob, TC );
		if ( Res == 0 )
			{
			F = Fi_NH3 - Fi_NH3_gas;
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
					F = Fi_NH3 - Fi_NH3_gas;
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
					F = Fi_NH3 - Fi_NH3_gas;
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
				F = Fi_NH3 - Fi_NH3_gas;
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
		Fi_NH3_liq = Fi_NH3;
		Cp_Liq = Cp;
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
		if ( fabs ( Fi_NH3_gas - Fi_NH3 ) < 1e-7 )
			break;
		Fi_NH3_gas = Fi_NH3;
		}
	if ( pLiq )
		{
		pLiq->P = P;
		pLiq->T = TC;
		pLiq->h = h_Liq;
		pLiq->s = s_Liq; 
		pLiq->Ro = Ro_Liq;
		pLiq->x_NH3 = 1.;
		pLiq->Fi_H2O_gas = 1.;
		pLiq->Fi_NH3_gas = Fi_NH3_gas;
		pLiq->Fi_H2O_liq = 1.;
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
		pGas->x_NH3 = 1.;
		pGas->Fi_H2O_gas = 1.;
		pGas->Fi_NH3_gas = Fi_NH3_gas;
		pGas->Fi_H2O_liq = 1.;
		pGas->Fi_NH3_liq = Fi_NH3_liq;
		pGas->kIter = kIter;
		pGas->Cp = Cp_Gas;
		pGas->F = F;
		}
	return P;
	}

double CNH3Helmholtz::Hp_F_chem = 0.;
double CNH3Helmholtz::Hp_F_0 = 0.;
void CNH3Helmholtz::SetH()
	{
	CNH3Helmholtz NH3;
	NH3.Ro_gas_mol ( Pstd, 25., 1e-4 );
	double H_25 = NH3.h;
  double Hst = pGas[G_NH3]->dHobr ( 25. );
	Hp_F_chem = Hst - H_25;
	Hp_F_0 = -H_25;
	}
#endif