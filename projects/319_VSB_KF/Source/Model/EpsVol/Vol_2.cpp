#include "stdafx.h"
#define DLL_EpsVol
#include "EpsVol.h"
#include "Err.h"
#include "CommProc.h"
#include "SR.h"
#include "Data.h"
#include "SetData.h"



CSVol_2::CSVol_2( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
	{
	CLEAR(Kappa_W)
	CLEAR(X_W)
	CLEAR(Y_W)
	CLEAR(Z_W)
	pComp = NULL;
	Ksi = 0.;
	Lwater = 0.;
	Pzwater = 0.;
	}

int CSVol_2::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		P = 1.;
		T = T_air;
		}
	return 0;
	}

double CSVol_2::Summ( double P, double Eps )
	{
	double S = 0.;
	double iP = 1. / P;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
			double q = iP * P_sat[n] - 1.;
			double Zn = 1. / ( 1. + Eps * q );
			S += q * Z_W[n] * Zn;
			}
		}
	return S;
	}

double CSVol_2::CalcEps_PT( double P, double T )
	{
	ASS( P > 0. )
	Pz ( T );
	if ( Summ ( P, 0. ) <= 0. )
		{
		return 0.;
		}
	if ( Summ ( P, 1. ) >= 0. )
		{
		return 1.;
		}
	double Eb = 0.;
	double Ee = 1.;
	double Eps;
	while ( Ee - Eb > 1e-9 )
		{
		Eps = 0.5 * ( Ee + Eb );
		if ( Summ ( P, Eps ) < 0. )
			Ee = Eps;
		else
			Eb = Eps;
		}
	return Eps;
	}

void CSVol_2::Set_XY_Eps( double Eps )
	{
	}

void CSVol_2::Set_XYP( double L )
	{
	double Sx = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] && n != G_H2O )
			{
			X_W[n] = M_comm * Z_W[n] / ( Kappa_W[n] + L_oil );
			Sx += X_W[n];
			}
		}
	double Sy = 0.;
	if ( L > 0.999999 * M_comm )
		{
		V = 0.;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] && n != G_H2O )
				{
				Y_W[n] = P_sat[n] * X_W[n];
				Sy += Y_W[n];
				}
			}
		}
	else
		{
		V = M_comm - L;
		double vol = Vol - Omega_L * L;
		double RT = RP * ( T + TK );
		P = V / Ksi;
		double iP = 1. / P;
		if ( L <= 0. )
			{
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( WorkComp[n])
					{
					Y_W[n] = Z_W[n];
					Sy += Y_W[n];
					}
				}
			}
		else
			{
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( WorkComp[n] && n != G_H2O )
					{
					Y_W[n] = iP * P_sat[n] * X_W[n];
					Sy += Y_W[n];
					}
				}
			}
		}
	bool IsWater = true;
	if ( IsWater )
		{
		if ( Lwater > 0. )
			{
			double N_Oil = ( L - Lwater ) / L;
			Y_W[G_H2O] = Pzwater / P;
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( WorkComp[n] && n != G_H2O )
					{
					X_W[n] *= N_Oil;
					}
				}
			Sx *= N_Oil;
			X_W[G_H2O] = Lwater / L;
			Sx += X_W[G_H2O];
			}
		else
			{
			X_W[G_H2O] = 0.;
			Y_W[G_H2O] = M_comm * Z_W[G_H2O] / ( Ksi * P );
			}
		Sy += Y_W[G_H2O];
		}
	if ( fabs ( Sx - 1. ) > 1e-6 )
		{
		double N = 1. / Sx;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				X_W[n] *= N;
			}
		}
//	
	if ( fabs ( Sy - 1. ) > 1e-6 )
		{
		double N = 1. / Sy;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				Y_W[n] *= N;
			}
		}
	}

void CSVol_2::Calc_H( )
	{
	h_gas = 0.;
	h_liq = 0.;
	if ( V > 0. )
		{
		H_gas( );
		h_gas = 0.;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				h_gas += Y_W[n] * h_Gas[n];
			}
		}
	if ( L > 0. )
		{
		H_liq ( );
		h_liq = 0.;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				h_liq += X_W[n] * h_Liq[n];
			}
		}
	H = V * h_gas + L * h_liq;
	}

void CSVol_2::PT( double _P, double _T, CComp * _pComp )
	{
	COUNTER
	SetComp ( _pComp );
	P = _P;
  if ( P > P_max )
    P = P_max;
	T = _T;
	if ( T < T_min )
		T = T_min;
	if ( T > T_max )
		T = T_max;
//
	double Eps = CalcEps_PT( P, T );
	double RT = RP * ( T + TK );
	M_comm = P * Vol / ( Eps * RT + Omega_L * ( 1. - Eps ));
	V = M_comm * Eps;
	L = M_comm - V;
	double vol = Vol - Omega_L * L;
	Ksi = vol / RT;
	double iP = 1. / P;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
			double Kappa = Ksi * P_sat[n];
			Kappa_W[n] = Kappa;
			}
		}
	Set_XYP( L );
	Calc_H( );
	SetGamma();
	}

double CSVol_2::Summ_L( double Ksi, double L )
	{
	double S = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] && n != G_H2O)
			{
//			double Zn = 1. / ( Kappa_W[n] + L );
			S += Z_W[n] / ( Kappa_W[n] + L );
			}
		}
	S *= M_comm;
	return S;
	}

void CSVol_2::Summ_L( double Ksi, double L, double & S, double & dSdL )
	{
	double Summ = 0.;
	double dSumm = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] && n != G_H2O )
			{
			double Zn = 1. / ( Kappa_W[n] + L );
			double C = Z_W[n] * Zn;
			Summ += C;
			dSumm -= C * Zn;
			}
		}
	S = Summ * M_comm;
	dSdL = dSumm * M_comm;
	}


double CSVol_2::Calc_L( double T, double L0 )
	{
	Pz ( T );
	double vol = Vol - Omega_L * L0;
	double RT = RP * ( T + TK );
	Ksi = vol / RT;
	double M_oil = M_comm;
	bool IsWater = true;
	if ( IsWater )
		{
		Pzwater = P_sat[G_H2O];
		double Mwater = M_comm * Z_W[G_H2O];
		M_oil -= Mwater;
		Lwater = Mwater - Ksi * Pzwater;
		if ( Lwater < 0. )
			Lwater = 0.;
		}
	else
		Lwater = 0.;
	if ( !_finite ( L0 ))
		L0 = M_oil;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] && n != G_H2O )
			{
			Kappa_W[n] = Ksi * P_sat[n];
			}
		}
	if ( Summ_L( Ksi, 0. ) < 1. )
		{
// Нефть испарена
		L_oil = 0.;
		return Lwater;
		}
	L_oil = L0;
	double d_L = 1e10;
	bool OK = true;
	int kIter = 0;
	double F, S, dSdL;
	while ( fabs ( d_L ) > 1e-9 )
		{
		kIter++;
		Summ_L( Ksi, L_oil, S, dSdL );
		F = 1. - S;
		d_L = F / dSdL;
		L_oil += d_L;
		if ( L_oil > M_oil )
			L_oil = M_oil;
		if ( !_finite( L_oil ) || L_oil < 0. || L_oil > M_oil || kIter > 10 )
			{
			OK = false;
			break;
			}
		}
	k_Iter = kIter;
	if ( OK && _finite ( L_oil ))
		return L_oil + Lwater;
	kIter = 0;
	double Lb = L0;
	double Le = L0;
	double Sb = Summ_L ( Ksi, Lb );
	double Se = Sb;
	double dL = 0.01 * M_oil;
 	while ( Sb < 1. )
		{
		kIter++;
		Le = Lb;
		Se = Sb;
		if ( Lb == 0. )
			return Lwater;
		Lb -= dL;
		if ( Lb < 0. )
			Lb = 0.;
		Sb = Summ_L ( Ksi, Lb );
		if ( Sb > 1. )
			break;
		}
	while ( Se > 1. )
		{
		kIter++;
		Lb = Le;
		Sb = Se;
		if ( Le == M_oil )
			return M_oil + Lwater;
		Le += dL;
		if ( Le > M_oil )
			Le = M_oil;
		Se = Summ_L ( Ksi, Le );
		if ( Se < 1. )
			break;
		}
	if ( L0 < Lb )
		L0 = Lb;
	if ( L0 > Le )
		L0 = Le;
	while ( Le - Lb > 1e-9 )
		{
		kIter++;
		L_oil = 0.5 * ( Le + Lb );
		if ( Summ_L ( Ksi, L_oil ) < 1. )
			Le = L_oil;
		else
			Lb = L_oil;
		}
	k_Iter += kIter;
	return L_oil + Lwater;
	}

void CSVol_2::HH( )
	{
	L = Calc_L( T, L );
	V = M_comm - L; 
	double vol = Vol - Omega_L * L;
	double RT = RP * ( T + TK );
	double Ksi = vol / RT;
	P = V / Ksi;
	Set_XYP( L );
	Calc_H( );
	}

void CSVol_2::MH_Corp_heat( double _M, double Hz, CComp * _pComp, double Cp_Vol, 
	                 double k_Heat, double T_Heat,
									 double & Q_Heat,
									 double k_ToAir,
									 double T0 )
	{
	COUNTER
	SetComp ( _pComp );
	M_comm = _M;
	if ( T0 != 0. )
		T = T0;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double T_Vol = T;
	double Tb = T;
	double Te = T;
	HH( );
	double Hsumm = H + Cp_Vol * ( T - T_Vol ) + k_Heat * ( T - T_Heat ) + 
		             k_ToAir * ( T - T_air );
	double Hb = Hsumm;
	double He = Hsumm;
	if ( Hsumm > Hz )
		{
		while ( Hsumm > Hz )
			{
			Te = Tb;
			He = Hb;
			if ( T <= T_min )
				{
				SetGamma();
				Q_Heat = k_Heat * ( T - T_Heat );
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			HH( );
			Hsumm = H + Cp_Vol * ( T - T_Vol ) + k_Heat * ( T - T_Heat ) + 
				      k_ToAir * ( T - T_air );
			Hb = Hsumm;
			}
		}
	else
		{
		while ( Hsumm < Hz )
			{
			Tb = Te;
			Hb = He;
			if ( T >= T_max )
				{
				SetGamma();
				Q_Heat = k_Heat * ( T - T_Heat );
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			HH( );
			Hsumm = H + Cp_Vol * ( T - T_Vol ) + k_Heat * ( T - T_Heat ) + 
				      k_ToAir * ( T - T_air );
			He = Hsumm;
			}
		}
	double dT = Te - Tb;
	if ( fabs ( dT ) < 1e-6	)
		{
// Te = Tb
		T = 0.5 * ( Te + Tb );
		Q_Heat = k_Heat * ( T - T_Heat );
		SetGamma();
		return;
		}
	int kIter = 0;
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		double idH = 1. / ( He - Hb );
		A = ( Hz - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
		dT = T_new - T;
		T = T_new;
		HH( );
		Hsumm = H + Cp_Vol * ( T - T_Vol ) + k_Heat * ( T - T_Heat ) + 
			      k_ToAir * ( T - T_air );
		if ( Hsumm < Hz )
			{
			Tb = T;
			Hb = Hsumm;
			}
		else
			{
			Te = T;
			He = Hsumm;
			}
		}
	SetGamma();
	Q_Heat = k_Heat * ( T - T_Heat );
	}

void CSVol_2::MT( double _M, double _T, CComp * _pComp )
	{
	SetComp ( _pComp );
	M_comm = _M;
	T = _T;
	HH( );
	SetGamma();
	}


void CSVol_2::MH_Corp( double _M, double Hz, CComp * _pComp, double Cp_Vol, double T0 )
	{
	COUNTER
	SetComp ( _pComp );
	M_comm = _M;
	if ( T0 != 0. )
		T = T0;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double T_Vol = T;
	double Tb = T;
	double Te = T;
	HH( );
	double Hsumm = H + Cp_Vol * ( T - T_Vol );
	double Hb = Hsumm;
	double He = Hsumm;
	if ( Hsumm > Hz )
		{
		while ( Hsumm > Hz )
			{
			Te = Tb;
			He = Hb;
			if ( T <= T_min )
				{
				SetGamma();
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			HH( );
			Hsumm = H + Cp_Vol * ( T - T_Vol );
			Hb = Hsumm;
			}
		}
	else
		{
		while ( Hsumm < Hz )
			{
			Tb = Te;
			Hb = He;
			if ( T >= T_max )
				{
				SetGamma();
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			HH( );
			Hsumm = H + Cp_Vol * ( T - T_Vol );
			He = Hsumm;
			}
		}
	double dT = Te - Tb;
	if ( fabs ( dT ) < 1e-6	)
		{
// Te = Tb
		T = 0.5 * ( Te + Tb );
		SetGamma();
		return;
		}
	int kIter = 0;
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		double idH = 1. / ( He - Hb );
		A = ( Hz - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
		dT = T_new - T;
		T = T_new;
		HH( );
		Hsumm = H + Cp_Vol * ( T - T_Vol );
		if ( Hsumm < Hz )
			{
			Tb = T;
			Hb = Hsumm;
			}
		else
			{
			Te = T;
			He = Hsumm;
			}
		}
	SetGamma();
	}

void CSVol_2::MH( double _M, double Hz, CComp * _pComp, double T0 )
	{
	ASS(FALSE);//Переделать деление пополам 
	SetComp ( _pComp );
	M_comm = _M;
	if ( T0 != 0. )
		T = T0;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double Tb = T;
	double Te = T;
	HH( );
	if ( H > Hz )
		{
		while ( H > Hz )
			{
			Te = T;
			if ( T <= T_min )
				{
				SetGamma();
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			HH( );
			}
		}
	else
		{
		while ( H < Hz )
			{
			Tb = T;
			if ( T >= T_max )
				{
				SetGamma();
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			HH( );
			}
		}
	while ( Te - Tb > 1e-6 )
		{
		T = 0.5 * ( Tb + Te );
		HH( );
		if ( H < Hz )
			{
			Tb = T;
			}
		else
			{
			Te = T;
			}
		}
	}

void CSVol_2::SetP( double _Pzad )
	{
	if ( L >= M_comm )
		return;
	double Pzad = 0.9 * P + 0.1 * _Pzad;
	double RT = RP * ( T + TK );
	double Ksi = Pzad * Vol / ( RT * V + Pzad * Omega_L * L );
	FINITE(Ksi)
	M_comm *= Ksi;
	L *= Ksi;
	V *= Ksi;
	H *= Ksi;
	}

int CSVol_2::SaveState ( )
	{
	S_CLASS("Base",CSBase_W)
	S_CLASS("Work",CSVol_W)
	return 0;
	}

int CSVol_2::RestoreState ( char * StrName )
	{
	R_CLASS("Base",CSBase_W)
	R_CLASS("Work",CSVol_W)
	return 0;
	}

int CSVol_2::GetParams( char *  )
{
char Name[256];
#include "IO_Parms.h"
	PARM ( T_max, "T_max" ) 
	PARM_D ( k_Gamma, "k_Gamma", 0.5 )
	PARM ( H2S_MEA, "H2S+MEA" )
	TAB( "Состояние", 5 )
		PARM ( P, "#Давление" )
		PARM ( T, "#Температура" ) 
		PARM ( M_comm, "#M_comm" ) 
		PARM ( V, "#V" ) 
		PARM ( L, "#L" ) 
		//	PARM ( T_min, "T_min" )
		PARM ( dH_liq, "#dH_liq" )
		TAB( "X", 5 )
		for ( int n = 0; n < LAST_GAS; n++ )
		{
			if ( !WorkComp[n] )
				continue;
			sprintf_s ( Name, sizeof(Name), "#X %s", pGas[n] ->Name );
			PARM ( X_W[n], Name );
		}
		ETAB
		TAB( "Y", 5 )
		for ( int n = 0; n < LAST_GAS; n++ )
		{
			if ( !WorkComp[n] )
				continue;
			sprintf_s ( Name, sizeof(Name), "#Y %s", pGas[n] ->Name );
			PARM ( Y_W[n], Name );
		}
		ETAB
		TAB( "Z", 5 )
		for ( int n = 0; n < LAST_GAS; n++ )
		{
			if ( !WorkComp[n] )
				continue;
			sprintf_s ( Name, sizeof(Name), "#Z %s", pGas[n] ->Name );
			PARM ( Z_W[n], Name );
		}
		ETAB
		TAB( "Gamma", 5 )
		for ( int n = 0; n < LAST_GAS; n++ )
		{
			if ( !WorkComp[n] )
				continue;
			sprintf_s ( Name, sizeof(Name), "#G %s", pGas[n] ->Name );
			PARM ( Gamma[n], Name );
		}
		ETAB
		TAB( "Henry", 5 )
		for ( int n = 0; n < LAST_GAS; n++ )
		{
			if ( !WorkComp[n] )
				continue;
			sprintf_s ( Name, sizeof(Name), "#H %s", pGas[n] ->Name );
			PARM ( Henry[n], Name );
		}
		ETAB
		TAB( "Pz", 5 )
		for ( int n = 0; n < LAST_GAS; n++ )
		{
			if ( !WorkComp[n] )
				continue;
			sprintf_s ( Name, sizeof(Name), "#Pz %s", pGas[n] ->Name );
			PARM ( P_sat[n], Name );
		}
		ETAB
	ETAB
	return 0;
}

