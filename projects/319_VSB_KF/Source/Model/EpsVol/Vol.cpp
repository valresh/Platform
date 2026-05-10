#include "stdafx.h"
#define DLL_EpsVol
#include "EpsVol.h"
#include "Err.h"
#include "CommProc.h"
#include "SR.h"
#include "Data.h"
#include "SetData.h"
#include "SysDataTypes.h"
#include "Connect.h"



CSVol_W::CSVol_W()
	{
	memset ( this, 0, sizeof ( CSVol_W ));
	}

CSVol_P::CSVol_P()
	{
	T_min = 1.;
	T_max = 1000.;
//	Step_T = 1.;
	Vol = 1.;
	Omega_L = 0.;
  P_max = 300.;
  UseTs = false;
	}

CSVol::CSVol( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
	{
	CLEAR(Kappa_W)
	CLEAR(X_W)
	CLEAR(Y_W)
	CLEAR(Z_W)
	pComp = NULL;
  Model = "SVol";
  Ts = NULL;
  kTs = 100;
  KoefTs = 1000.;
  LevelLiq = 1.;
	}

int CSVol::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		P = 1.;
		T = T_air;
		return 1;
		}
	return 0;
	}

double CSVol::Summ( double P, double Eps )
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

double CSVol::CalcEps_PT( double P, double T )
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

void CSVol::Set_XY_Eps( double Eps )
	{
	}

void CSVol::Set_XYP( double L, double Ksi )
	{
	double Sx = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
      if ( NoInLiq[n] )
        X_W[n] = 0.;
      else
			  X_W[n] = M_comm * Z_W[n] / ( Kappa_W[n] + L );
			Sx += X_W[n];
			}
		}
	if ( fabs ( Sx - 1. ) > 1e-6 )
		{
		double N;
    if ( Sx > 1e-6 )
      N = 1. / Sx;
    else
      N = 0.;
		FINITE(N)
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				X_W[n] *= N;
			}
		}
//	
	double Sy = 0.;
	if ( L > 0.999999 * M_comm )
		{
		V = 0.;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				{
        if ( NoInLiq[n] )
          Y_W[n] = Z_W[n];
        else
				  Y_W[n] = P_sat[n] * X_W[n];
				Sy += Y_W[n];
				}
			}
		}
	else
		{
		V = M_comm - L;
		double vol = Vol - Omega_L * L;
		if(vol < 0.1)
			vol = 0.1;
		double RT = RP * ( T + TK );
		P = V * RT / vol;
		double iP = 1. / P;
		if ( L <= 0. )
			{
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( WorkComp[n] )
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
				if ( WorkComp[n] )
					{
          if ( NoInLiq[n] )
            Y_W[n] = M_comm * Z_W[n] * iP / Ksi;
          else
					  Y_W[n] = iP * P_sat[n] * X_W[n];
					Sy += Y_W[n];
					}
				}
			}
		}
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

void CSVol::Calc_H( )
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
    if ( UseTs && Ts )
      {
      double dt = pSys->dt;
      double Ts = CalcGradT( dt, T );
      H_liq ( Ts );
      }
    else
		  H_liq ( );
		h_liq = dH_liq;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				h_liq += X_W[n] * h_Liq[n];
			}
		}
  else
    {
    if ( UseTs && Ts )
      {
      for ( int n = 0; n < kTs; n++ )
        Ts[n] = T;
      }
    }
  h_sol = 0.;
/************
  if ( UseSolidFlow )
    {
    if ( L == 0. )
      H_liq( );
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( IsComp[n])
        {
        if ( pTypeComp[n] & _Solid_ )
          h_sol += X_W[n] * h_Liq[n];
        }
      }
    double M = M_sol + M_comm;
    H = V * h_gas + L * h_liq + M * h_sol;
    }
  else
********/
	  H = V * h_gas + L * h_liq;
	}

void CSVol::PT( double _P, double _T, CComp * _pComp )
	{
	COUNTER
	//if(::T_min != T_MIN)
	//  T_min = ::T_min;
	//if(::T_max != T_MAX)
	//  T_max = ::T_max;
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
	M_comm = P * Vol / RT;
	V = M_comm * Eps;
	L = M_comm - V;
	FINITE(L)
	double vol = Vol - Omega_L * L;
	if(vol < 0.1)
		vol = 0.1;
	double Ksi = vol / RT;
	double iP = 1. / P;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] && !NoInLiq[n] )
			{
			double Kappa = Ksi * P_sat[n];
			Kappa_W[n] = Kappa;
			}
		}
	Set_XYP( L, Ksi );
	Calc_H( );
	SetGamma();
	}

double CSVol::Summ_L( double Ksi, double L )
	{
	double S = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] && !NoInLiq[n] )
			{
//			double Zn = 1. / ( Kappa_W[n] + L );
			S += Z_W[n] / ( Kappa_W[n] + L );
			}
		}
	return S * M_comm;
	}

void CSVol::Summ_L( double Ksi, double L, double & S, double & dSdL )
	{
	double Summ = 0.;
	double dSumm = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] && !NoInLiq[n] )
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


double CSVol::Calc_L( double T, double L0 )
	{
	if ( !_finite ( L0 ))
		L0 = M_comm;
	Pz ( T );
  double Ps = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( WorkComp[n] && !NoInLiq[n])
      {
      Ps += P_sat[n] * X_W[n];
      }
    }
	double vol = Vol - Omega_L * L0;
	if(vol < 0.1)
		vol = 0.1;
	double RT = RP * ( T + TK );
	double Ksi = vol / RT;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
      if ( NoInLiq[n] )
        Kappa_W[n] = -1.; // Условно
      else
			  Kappa_W[n] = Ksi * P_sat[n];
			}
		}
  if ( L0 == 0. )
    {
    double S = Summ_L ( Ksi, 0. );
    if ( S <= 1. )
      {
      k_Iter = 0;
      return 0.;
      }
    }
	double L = L0;
	double d_L = 1e10;
	bool OK = true;
	int kIter = 0;
	double F, S, dSdL;
	while ( fabs ( d_L ) > 1e-9 )
		{
		kIter++;
		Summ_L( Ksi, L, S, dSdL );
		F = 1. - S;
		d_L = F / dSdL;
		L += d_L;
		if ( !_finite( L ) || L < 0. || L > M_comm || kIter > 10 )
			{
			OK = false;
      L = L0;
			break;
			}
		}
	k_Iter = kIter;
	if ( OK && _finite ( L ))
		return L;
	kIter = 0;
	double Lb = L0;
	double Le = L0;
	double Sb = Summ_L ( Ksi, Lb );
	double Se = Sb;
	double dL = 0.01 * M_comm;
 	while ( Sb < 1. && kIter < 50 )
		{
		kIter++;
		Le = Lb;
		Se = Sb;
		if ( Lb == 0. )
			return 0.;
		Lb -= dL;
		if ( Lb < 0. )
			Lb = 0.;
		Sb = Summ_L ( Ksi, Lb );
		if ( Sb > 1. )
			break;
		}
//  if ( kIter >= 40 )
//    KKK();
	while ( Se > 1. && kIter < 50 )
		{
		kIter++;
		Lb = Le;
		Sb = Se;
		if ( Le == M_comm )
			return M_comm;
		Le += dL;
		if ( Le > M_comm )
			Le = M_comm;
		Se = Summ_L ( Ksi, Le );
		if ( Se < 1. )
			break;
		}
//  if ( kIter >= 40 )
//    KKK();
	if ( L0 < Lb )
		L0 = Lb;
	if ( L0 > Le )
		L0 = Le;
  double SS = 0.5 * ( Le + Lb );
  if ( SS < 1. )
    SS = 1.;
  kIter = 0;
	while ( Le - Lb > 1e-9 * SS && kIter++ < 20 )
		{
		L = 0.5 * ( Le + Lb );
		if ( Summ_L ( Ksi, L ) < 1. )
			Le = L;
		else
			Lb = L;
		}
  FINITE(L)
//  if ( kIter >= 20 )
//    KKK();
	k_Iter += kIter;
	return L;
	}

void CSVol::HH( )
	{
  if ( L > M_comm )
    L = M_comm;
	L = Calc_L( T, L );
	V = M_comm - L; 
	double vol = Vol - Omega_L * L;
	if(vol < 0.1)
		vol = 0.1;
	double RT = RP * ( T + TK );
	double Ksi = vol / RT;
	P = V / Ksi;
	Set_XYP( L, Ksi );
	Calc_H( );
	}

double CSVol::CalcGradT( double dt, double T0 )
  {
  if ( LevelLiq < 0.05 )
    {
    for ( int i = 0; i < kTs; i++ )
      Tmp_Ts[i] = T0;
    return T0;
    }
  double A[1000];
  double B[1000];
  A[0] = 0.;
  B[0] = T0;
  double Ksi = KoefTs * dt / LevelLiq;
  double a = 1. + 2. * Ksi;
  double b = Ksi;
  for ( int i = 1; i <= kTs - 2; i++ )
    {
    double Z = 1./ ( a - b * A[i-1] ); 
    A[i] = b * Z;
    B[i] = ( Ts[i] + b * B[i-1] ) * Z;
    }
  Tmp_Ts[kTs-1] = ( Ts[kTs-1] + Ksi * B[kTs-2]) / ( 1. + Ksi - Ksi * A[kTs-2] );
  double T_med = Tmp_Ts[kTs-1];
  for ( int i = kTs - 1; i > 0; i-- )
    {
    Tmp_Ts[i-1] = A[i-1] * Tmp_Ts[i] + B[i-1];
    T_med += Tmp_Ts[i-1];
    }
  ASS(fabs(Tmp_Ts[0] - T0) < 1e-8)
  T_med /= kTs;
  return T_med;
  }


double * CSVol::Tmp_Ts = NULL;

void CSVol::MH_Corp_heat(  double _M, double Hz, CComp * _pComp, 
                           double Cp_Vol, 
	                         double k_Heat, double T_Heat,
									         double & Q_Heat,
									         double k_ToAir,
									         double T0 )
	{
	COUNTER
	//if(::T_min != T_MIN)
	//  T_min = ::T_min;
	//if(::T_max != T_MAX)
	//  T_max = ::T_max;
	SetComp ( _pComp );
  M_comm = _M;
  M_sol = 0.;
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
        if ( UseTs && Ts )
          memmove ( Ts, Tmp_Ts, 8 * kTs );
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
        if ( UseTs && Ts )
          memmove ( Ts, Tmp_Ts, 8 * kTs );
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
    if ( UseTs && Ts )
      memmove ( Ts, Tmp_Ts, 8 * kTs );
		return;
		}
	int kIter = 0;
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		double idH = 1. / ( He - Hb );
    FINITE(idH)
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
  HH( );
	SetGamma();
	Q_Heat = k_Heat * ( T - T_Heat );
  if ( UseTs && Ts )
    memmove ( Ts, Tmp_Ts, 8 * kTs );
	}

void CSVol::MT( double _M, double _T, CComp * _pComp )
	{
	SetComp ( _pComp );
	M_comm = _M;
	T = _T;
	HH( );
	SetGamma();
	}


void CSVol::MH_Corp( double _M, double Hz, CComp * _pComp, double Cp_Vol, double T0 )
	{
	COUNTER
	//if(::T_min != T_MIN)
	//  T_min = ::T_min;
	//if(::T_max != T_MAX)
	//  T_max = ::T_max;
	SetComp ( _pComp );
  M_comm = _M;
  M_sol = 0.;
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

void CSVol::MH( double _M, double Hz, CComp * _pComp, double T0 )
	{
	ASS(FALSE);//Переделать деление пополам 
	//if(::T_min != T_MIN)
	//  T_min = ::T_min;
	//if(::T_max != T_MAX)
	//  T_max = ::T_max;
	SetComp ( _pComp );
  M_comm = _M;
  M_sol = 0.;
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

void CSVol::SetP( double _Pzad )
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

int CSVol::SaveState ( )
	{
	S_CLASS("Base",CSBase_W)
	S_CLASS("Work",CSVol_W)
  if ( UseTs && Ts )
    {
    ASS(pSRFile->WStruct("GradT", kTs * 8, Ts ));
    }
	return 0;
	}

int CSVol::RestoreState ( char * StrName )
	{
  COPY_FIRST
	R_CLASS("Base",CSBase_W)
	R_CLASS("Work",CSVol_W)
  if ( UseTs && Ts )
    {
    COPY_FIRST
    if ( lstrcmp ( "GradT", StrName ) == 0 )
      {
      ASS(pSRFile->RStruct( kTs * 8, Ts ));
      return 0;
      }
    }
	return 0;
	}

int CSVol::GetParams( char * )
{
#include "IO_Parms.h"
	TAB( "Параметры", 5 )
		PARM ( T_max, "T_max" ) 
		PARM_D ( k_Gamma, "k_Gamma", 0.5 )
		PARM ( MixH2ONH3, "Использовать расширения" ) 
		PARM ( UseTs, "Градиент Т" )
		PARM ( kTs, "Число точек Т" )
		PARM ( KoefTs, "Теплопроводность" )
	ETAB
//	if ( pChemAdditions )
//		pChemAdditions->GetParams( this );
	TAB( "Состояние", 5 )
		PARM ( P, "#Давление" )
		PARM ( T, "#Температура" ) 
		PARM ( M_comm, "#M_comm" ) 
		PARM ( V, "#V" ) 
		PARM ( L, "#L" ) 
		//	PARM ( T_min, "T_min" )
		PARM ( Gamma[G_H2O], "#Gamma H2O" )
		PARM ( dH_liq, "#dH_liq" )
		char Name[256];
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
			sprintf_s ( Name, sizeof(Name), "H %s", pGas[n] ->Name );
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
		TAB( "Ts", 5 )
		if ( Ts )
		{
			for ( int n = 0; n < kTs; n++ )
			{
				sprintf_s ( Name, sizeof(Name), "#Ts[%d]", n );
				PARM ( Ts[n], Name );
			}
		}
		ETAB
	ETAB
	return 0;
}

void CSVol::InitGradT( int _kTs, double _KoefTs )
  {
  kTs = _kTs;
  KoefTs = _KoefTs;
  if ( kTs < 20 )
    kTs = 20;
  if ( kTs > 200 )
    kTs = 200;
  if ( Tmp_Ts == NULL )
    Tmp_Ts = NewArr (double,1000);
  if( Ts == NULL )
    {
    Ts = NewArr (double,kTs);
    for ( int n = 0; n < kTs; n++ )
      Ts[n] = T;
    }
  UseTs = true;
  }

void CSVol::ChangeGradT( int _kTs, double _KoefTs )
  {
  if ( Ts == NULL )
    {
    InitGradT( _kTs, _KoefTs );
    return;
    }
  KoefTs = _KoefTs;
  if ( _kTs == kTs )
    return;
  double * _Ts = NewArr (double,_kTs);
  if ( kTs < _kTs )
    {
    for ( int n = 0; n < kTs; n++ )
      _Ts[n] = Ts[n];
    double T = Ts[kTs-1];
    for ( int n = kTs; n < _kTs; n++ )
      _Ts[n] = T;
    }
  else
    {
    for ( int n = 0; n < _kTs; n++ )
      _Ts[n] = Ts[n];
    }
  kTs = _kTs;
  Ts = _Ts;
  }
