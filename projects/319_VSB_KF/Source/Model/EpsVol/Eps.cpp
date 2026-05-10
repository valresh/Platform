#include "stdafx.h"
#define DLL_EpsVol
#include "EpsVol.h"
#include "Err.h"
#include "CommProc.h"
#include "SR.h"
#include "Data.h"
#include "SetData.h"
#include "Connect.h"

#define STEP_T \
  if ( Step_T < 0.1 && kIter > 5 )\
  Step_T = 0.1;\
  if ( Step_T < 1. && kIter > 10 )\
  Step_T = 1.;\
  if ( Step_T < 5. && kIter > 15 )\
  Step_T = 5.;

CSEps_W::CSEps_W()
	{
	memset ( this, 0, sizeof ( CSEps_W ));
	}

CSEps_P::CSEps_P()
	{
	T_min = -80;
	T_max = 1000.;
//	Step_T = 1.;
    P_max = 300.;
	}

CSEps::CSEps( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
	{
	CLEAR(Q_W)
	CLEAR(K_W)
	CLEAR(X_W)
	CLEAR(Y_W)
	CLEAR(Z_W)
	pComp = NULL;
    ObjName = Name;
    Model = "Eps";
    UseComp = 0xFFFFFFFF;
    h_Liquid = h_Solid = 0.;
	}

CSEps::CSEps( char * Name ) : IBaseModel ( false )
	{
	CLEAR(Q_W)
	CLEAR(K_W)
	CLEAR(X_W)
	CLEAR(Y_W)
	CLEAR(Z_W)
	pComp = NULL;
	ObjName = Name;
    Model = "Eps";
    UseComp = 0xFFFFFFFF;
    h_Liquid = h_Solid = 0.;
	}

int CSEps::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
		P = 1.;
		T = T_air;
		return 1;
		}
	return 0;
	}


void CSEps::SdS( double Eps, double &S, double &dSdE )
	{
	double Sp = 0.;
	double Sm = 0.;
	double dSp = 0.;
	double dSm = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
      if ( NoInLiq[n] )
        {
        if ( Eps > 1e-3 )
          {
          Sp += Z_W[n] / Eps;
          dSp -= Z_W[n] / ( Eps * Eps );
          }
        else
          {
          Sp += Z_W[n] * 1000.;
          dSp = 0.;
          }
        }
      else
        {
			  double q = Q_W[n];
			  double Zn = 1. / ( 1. + Eps * q );
			  double C = q * Z_W[n] * Zn;
			  if ( q > 0. )
				  {
				  Sp += C;
				  dSp -= C * Zn * q;
				  }
			  else
				  {
				  Sm -= C;
				  dSm += C * Zn * q;
				  }
        }
			}
		}
	S = 0.;
	dSdE = 0.;
	if ( Sp > 1. )
		{
		S = log ( Sp );
		dSdE = dSp / Sp;
		}
	else
		{
		S = Sp - 1.;
		dSdE = dSp;
		}
	if ( Sm > 1. ) 
		{
		S -= log ( Sm );
		dSdE -= dSm / Sm;
		}
	else
		{
		S -= Sm - 1.;
		dSdE -= dSm;
		}
	}
double CSEps::Summ( double Eps )
	{
	double S = 0.;
	double Sp = 0.;
	double Sm = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
      if ( NoInLiq[n] )
        {
        if ( Eps > 1e-3 )
          Sp += Z_W[n] / Eps;
        else
          Sp += Z_W[n] * 1000.;
        }
      else
        {
			  double q = Q_W[n];
			  double Zn = 1. / ( 1. + Eps * q );
			  if ( q > 0. )
				  Sp += q * Z_W[n] * Zn;
			  else
				  Sm -= q * Z_W[n] * Zn;  
        }
//			S += q * Z_W[n] * Zn;
			}
		}
	if ( Sp > 1. )
		S += log ( Sp );
	else
		S += Sp - 1.;
	if ( Sm > 1. ) 
		S -= log ( Sm );
	else
		S -= Sm - 1.;
	return S;
	}

double CSEps::Summ( double P, double T, double Eps )
	{
	double iP = 1. / P;
	double S = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
      if ( NoInLiq[n] )
        {
        if ( Eps > 1e-3 )
          S += Z_W[n] / Eps;
        else
          S += Z_W[n] * 1000.;
        }
      else
        {
			  double q = P_sat[n] * iP - 1.;
			  double Zn = 1. / ( 1. + Eps * q );
			  S += q * Z_W[n] * Zn;
        }
			}
		}
	return S;
	}

double CSEps::CalcP_ET( double E, double T )
	{
	if ( P < 0.001 )
    P = 0.001;
	Pz( T );
	double Pb = P;
	double Pe = P;
	double S = Summ( P, T, E );
	if ( S < 0. )
		{
		while ( S < 0. )
			{
			Pe = Pb;
			Pb *= 0.98;
			S = Summ( Pb, T, E );
			}
		}
	else
		{
		while ( S > 0. )
			{
			Pb = Pe;
			Pe *= 1.02;
			S = Summ( Pe, T, E );
			}
		}
	double P;
	while ( Pe - Pb > 1e-4 )
		{
		P = 0.5 * ( Pe + Pb );
		S = Summ( P, T, E );
		if ( S < 0. )
			Pe = P;
		else
			Pb = P;
		}
	return P;
	}


double CSEps::CalcEps_PT( double P, double T, double Eps0 )
	{
	if ( P < 0.001 )
    P = 0.001;
	double iP = 1. / P;
	Pz ( T );
	bool All_plus = true;
	bool All_minus = true;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( WorkComp[n] )
			{
      if ( NoInLiq[n] )
        {
        All_minus = false;
        continue;
        }
			K_W[n] = P_sat[n] * iP;
			Q_W[n] = K_W[n] - 1.;
      if ( Q_W[n] < -0.999999 )
        Q_W[n] = -0.999999;
			if ( All_plus && Q_W[n] < 0. )
				All_plus = false;
			if ( All_minus && Q_W[n] > 0. )
				All_minus = false;
			}
		}
	if ( All_plus )
		{
		k_Iter = 0;
		return 1.;
		}
	if ( All_minus )
		{
		k_Iter = 0;
		return 0.;
		}
	double Eb = Eps0;
	double Ee = Eps0;
	double Sb = Summ ( Eb );
	double Se = Sb;
	int kIter = 0;
 	while ( Sb < 0. )
		{
		kIter++;
		Ee = Eb;
		Se = Sb;
		if ( Eb == 0. )
			return 0.;
		Eb -= 0.01;
		if ( Eb < 0. )
			Eb = 0.;
		Sb = Summ ( Eb );
		if ( Sb > 0. )
			break;
		}
	while ( Se > 0. )
		{
		kIter++;
		Eb = Ee;
		Sb = Se;
		if ( Ee == 1. )
			return 1.;
		Ee += 0.01;
		if ( Ee > 1. )
			Ee = 1.;
		Se = Summ ( Ee );
		if ( Se < 0. )
			break;
		}
	if ( Eps0 < Eb )
		Eps0 = Eb;
	if ( Eps0 > Ee )
		Eps0 = Ee;

	double Eps = Eps0;
	double d_Eps = 1e10;
	double S, dSdE;
	bool OK = true;
	while ( fabs ( d_Eps ) > 1e-9 )
		{
		kIter++;
		SdS( Eps, S, dSdE );
		d_Eps = -S / dSdE;
		Eps += d_Eps;
		if ( !_finite( Eps ) || Eps < 0. || Eps > 1. || kIter > 20 )
			{
			OK = false;
			break;
			}
		}
	k_Iter = kIter;
	O_K = OK;
	if ( OK )
		return Eps;
//	double S;
//	int kIter = 0;
	Eps = 0.5 * ( Ee + Eb );
	while ( Ee - Eb > 1e-9 )
		{
		kIter++;
		Eps = 0.5 * ( Ee + Eb );
		S = Summ ( Eps );
		if ( S < 0. )
			Ee = Eps;
		else
			Eb = Eps;
		}
	k_Iter += kIter;
	return Eps;
	}


DWORD ID;
void CSEps::Set_XY( )
	{
	double Sx = 0.;
	double Sy = 0.;
	if ( E >= 1. )
		{
		E = 1.;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				{
				Y_W[n] = Z_W[n];
        if ( NoInLiq[n] )
          X_W[n] = 0.;
        else
          {
          if ( P_sat[n] > 1e-6 )
				    X_W[n] = Y_W[n] / P_sat[n];
          else
            X_W[n] = 0.;
          }
				Sy += Y_W[n];
				Sx += X_W[n];
        if ( !_finite(Y_W[n]))
          KKK();
				}
			}
		}
	else
		{
		if ( E <= 0. )
			{
			E = 0.;
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( WorkComp[n] )
					{
          if ( NoInLiq[n] )
            {
// Если есть NoInLiq - E > 0 !!!
// Сюда не должны попасть
            Y_W[n] = Z_W[n];
            X_W[n] = 0.;
            }
          else
            {
            X_W[n] = Z_W[n];
					  Y_W[n] = X_W[n] * P_sat[n];
            }
					Sy += Y_W[n];
					Sx += X_W[n];
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
            {
            X_W[n] = 0.;
            Y_W[n] = Z_W[n] / E;
            }
          else
            {
					  X_W[n] = Z_W[n] / ( 1. + E * Q_W[n] );
					  Y_W[n] = K_W[n] * X_W[n];
            }
					Sx += X_W[n];
					Sy += Y_W[n];
					}
				}
			}
		}
	if ( fabs ( Sx - 1. ) > 1e-4 && Sx > 1e-6 )
		{
		double N = 1. / Sx;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				{
				X_W[n] *= N;
				}
			}
		}
	if ( fabs ( Sy - 1. ) > 1e-4 && Sy > 1e-6 )
		{
		double N = 1. / Sy;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				{
				Y_W[n] *= N;
				}
			}
		}
	}

void CSEps::Calc_H( )
	{
	Set_XY( );
	h_gas = 0.;
	h_liq = 0.;
	if ( E > 0. )
		{
		h_gas = 0.;
		H_gas ( );
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
        {
 //       Fsend ( "Pipe.csv", "%10.6lf;%10.1lf;%s;\n", Y_W[n], h_Gas[n], pGas[n]->Name );
				h_gas += Y_W[n] * h_Gas[n];
        }
			}
		}
	if ( E < 1. )
		{
		H_liq( );
		h_liq = dH_liq;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				h_liq += X_W[n] * h_Liq[n];
			}
		}
/*
  double h_sol = 0.;
  if ( UseSolidFlow )
    {
    if ( E >= 1. )
      H_liq( );
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( pTypeComp[n] )
        h_sol += X_W[n] * h_Liq[n];
      }
    }
*/
  h_Liquid = ( E * h_gas + ( 1. - E ) * h_liq ) * SummUse;
  h_Solid = 0.;//h_sol;
	h = h_Liquid + h_Solid;
  FINITE(h)
	//for ( int n = 0; n < LAST_GAS; n++ )
	//	{
	//	if ( WorkComp[n] && h_Liq[n] > h_Gas[n] )
	//		KKK();;
	//	}
	}

void CSEps::PT( double _P, double _T, bool WorkComp[], double Cmol[] )
	{
	COUNTER
	//if(::T_min != T_MIN)
	//  T_min = ::T_min;
	//if(::T_max != T_MAX)
	//  T_max = ::T_max;
	SetComp ( WorkComp, Cmol );
	P = _P;
	T = _T;
	if ( T < T_min )
		T = T_min;
	if ( T > T_max )
		T = T_max;
	H( );
	SetGamma();
	}

void CSEps::PT( double _P, double _T, CComp * _pComp )
	{
	COUNTER
  //if(::T_min != T_MIN)
	 // T_min = ::T_min;
  //if(::T_max != T_MAX)
	 // T_max = ::T_max;
  if ( _pComp == NULL )
    return;
	SetComp ( _pComp );
	P = _P;
	T = _T;
	if ( T < T_min )
		T = T_min;
	if ( T > T_max )
		T = T_max;
	H( );
	SetGamma();
	}

void CSEps::TE( double _T, double _E, CComp * _pComp )
  {
  COUNTER
  //if(::T_min != T_MIN)
  //  T_min = ::T_min;
  //if(::T_max != T_MAX)
  //  T_max = ::T_max;
  if ( _pComp == NULL )
    return;
  SetComp ( _pComp );
  E = _E;
  T = _T;
  if ( T < T_min )
    T = T_min;
  if ( T > T_max )
    T = T_max;
  P = CalcP_ET( E, T );
  H( );
  SetGamma();
  }

void CSEps::PE( double _P, double Eps, CComp * _pComp, double T0 )
  {
  //if(::T_min != T_MIN)
	 // T_min = ::T_min;
  //if(::T_max != T_MAX)
	 // T_max = ::T_max;
  SetComp ( _pComp );
  P = _P;
  if ( P > P_max )
    P = P_max;
  if ( T0 != 0. )
    T = T0;
  if ( T <= T_min )
    T = T_min + Step_T;
  if ( T >= T_max )
    T = T_max - Step_T;
  if ( !_finite( T ))
    T = T_air;
  double Tb = T;
  double Te = T;
  E = CalcEps_PT( P, T, E );
  double Eb = E;
  double Ee = E;
  if ( E > Eps )
    {
    while ( E > Eps )
      {
      Te = Tb;
      Ee = Eb;
      if ( T <= T_min )
        {
        SetGamma();
        return;
        }
      T -= Step_T;
      if ( T < T_min )
        T = T_min;
      Tb = T;
      E = CalcEps_PT( P, T, E );
      Eb = E;
      }
    }
  else
    {
    while ( E < Eps )
      {
      Tb = Te;
      Eb = Ee;
      if ( T >= T_max )
        {
        SetGamma();
        return;
        }
      T += Step_T;
      if ( T > T_max )
        T = T_max;
      Te = T;
      E = CalcEps_PT( P, T, E );
      Ee = E;
      }
    }
  double dT = Te - Tb;
  if ( fabs ( dT ) < 1e-6	)
    {
    // Te = Tb
    T = 0.5 * ( Te + Tb );
    Calc_H( );
    SetGamma();
    return;
    }
  if ( Ee - Eb > 0.1 )
    {
    // Разрыв
    while ( Te - Tb > 1e-6 )
      {
      T = 0.5 * ( Tb + Te );
      if ( E < Eps )
        {
        Tb = T;
        Eb = E;
        }
      else
        {
        Te = T;
        Ee = E;
        }
      }
    if ( Ee - Eb < 1e-6 )
      {
      Calc_H( );
      SetGamma();
      return;
      }
    // Интерполяция E
    E = Eps;
    Calc_H( );
    SetGamma();
    return;
    }
  // Непрерывный случай
  int kIter = 0;
  double A, A1;
  while ( fabs ( dT ) > 1e-6 )
    {
    kIter++;
    if ( kIter > 5 )
      KKK();
    double idH = 1. / ( Ee - Eb );
    A = ( Eps - Eb ) * idH;
    A1 = 1. - A;
    double T_new = A * Te + A1 * Tb;
    dT = T_new - T;
    T = T_new;
    E = CalcEps_PT( P, T, E );
    if ( E < Eps )
      {
      Tb = T;
      }
    else
      {
      Te = T;
      }
    }
  Calc_H( );
  SetGamma();
  }


void CSEps::H( )
	{
	if ( !_finite ( E ))
		E = 0.;
	if ( GAS )
		{
		Pz ( T );
		E = 1.;
		}
	else
		{
		if ( LIQ )
			{
			Pz ( T );
			E = 0.;
			}
		else
			E = CalcEps_PT( P, T, E );
		}
	Calc_H( );
	}

void CSEps::HE( )
	{
	P = CalcP_ET( E, T );
	Calc_H( );
	}

void CSEps::PH( struct CFlow * pFlow, double T0 )
	{
	if( pFlow == NULL )
    return;
	if ( pFlow -> ID_COMP == 0 )
		{
		pFlow -> ID_COMP = ID_N2;
		}
	PH( pFlow->P, pFlow->h_Flow, pFlow->Get_Comp(), T0 );
	}

double CSEps::Eps_gas()
  {
  double E_gas = E * SummUse;
  return E_gas;
  }

void CSEps::PH_mass( double _P, double hz, CComp * _pComp, double Cp_mass, double Teff_mass, 
		                 double T0 )
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
	if ( T0 != 0. )
		T = T0;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double _T = T;
	double Tb = T;
	double Te = T;
	H( );
	double Hsumm = h + Cp_mass * T - Teff_mass;
	double Hb = Hsumm;
	double Eb = E;
	double He = Hsumm;
	double Ee = E;
	if ( Hsumm > hz )
		{
		while ( Hsumm > hz )
			{
			Te = Tb;
			He = Hb;
			Ee = Eb;
			if ( T <= T_min )
				{
				SetGamma();
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			H( );
			Hsumm = h + Cp_mass * T - Teff_mass;
			Hb = Hsumm;
			Eb = E;
			}
		}
	else
		{
		while ( Hsumm < hz )
			{
			Tb = Te;
			Hb = He;
			Eb = Ee;
			if ( T >= T_max )
				{
				SetGamma();
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			H( );
			Hsumm = h + Cp_mass * T - Teff_mass;
			He = Hsumm;
			Ee = E;
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
  if ( Ee - Eb > 0.1 )
		{
// Разрыв
		double Eps_dT = 1e-6;
		if ( Eps_dT * Cp_mass > 0.01 )
			Eps_dT = 0.01 / Cp_mass;
		if ( Eps_dT < 1e-12 )
			Eps_dT = 1e-12;
		while ( Te - Tb > Eps_dT )
			{
			T = 0.5 * ( Tb + Te );
			H( );
			Hsumm = h + Cp_mass * T - Teff_mass;
			if ( Hsumm < hz )
				{
				Tb = T;
				Hb = Hsumm;
				Eb = E;
				}
			else
				{
				Te = T;
				He = Hsumm;
				Ee = E;
				}
			}
		if ( He - Hb < 1e-3 )
			{
			SetGamma();
			return;
			}
	// Интерполяция E
		E = Eb + ( hz - Hb ) / ( He - Hb ) * ( Ee - Eb );
		Calc_H( );
		SetGamma();
		return;
		}
// Непрерывный случай
	int kIter = 0;
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		if ( kIter > 5 )
			KKK();
		double idH = 1. / ( He - Hb );
		A = ( hz - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
		dT = T_new - T;
		T = T_new;
		H( );
		Hsumm = h + Cp_mass * T - Teff_mass;
		if ( Hsumm < hz )
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

void CSEps::PH_mix( double _P, double hz, double Flow, CComp * _pComp, double Cp_pass, double T0_pass, 
		                double T0 )
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
	if ( T0 != 0. )
		T = T0;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double _T = T;
	double Tb = T;
	double Te = T;
	H( );
	double Hsumm = h * Flow + Cp_pass * ( T - T0_pass );
	double Hb = Hsumm;
	double Eb = E;
	double He = Hsumm;
	double Ee = E;
	if ( Hsumm > hz )
		{
		while ( Hsumm > hz )
			{
			Te = Tb;
			He = Hb;
			Ee = Eb;
			if ( T <= T_min )
				{
				SetGamma();
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			H( );
			Hsumm = h * Flow + Cp_pass * ( T - T0_pass );
			Hb = Hsumm;
			Eb = E;
			}
		}
	else
		{
		while ( Hsumm < hz )
			{
			Tb = Te;
			Hb = He;
			Eb = Ee;
			if ( T >= T_max )
				{
				SetGamma();
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			H( );
			Hsumm = h * Flow + Cp_pass * ( T - T0_pass );
			He = Hsumm;
			Ee = E;
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
  if ( Ee - Eb > 0.1 )
		{
// Разрыв
		double Eps_dT = 1e-6;
		if ( Eps_dT * Cp_pass > 0.01 )
			Eps_dT = 0.01 / Cp_pass;
		if ( Eps_dT < 1e-12 )
			Eps_dT = 1e-12;
		while ( Te - Tb > Eps_dT )
			{
			T = 0.5 * ( Tb + Te );
			H( );
			Hsumm = h * Flow + Cp_pass * ( T - T0_pass );
			if ( Hsumm < hz )
				{
				Tb = T;
				Hb = Hsumm;
				Eb = E;
				}
			else
				{
				Te = T;
				He = Hsumm;
				Ee = E;
				}
			}
		if ( He - Hb < 1e-3 )
			{
			SetGamma();
			return;
			}
	// Интерполяция E
		E = Eb + ( hz - Hb ) / ( He - Hb ) * ( Ee - Eb );
		Calc_H( );
		SetGamma();
		return;
		}
// Непрерывный случай
	int kIter = 0;
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		if ( kIter > 5 )
			KKK();
		double idH = 1. / ( He - Hb );
		A = ( hz - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
		dT = T_new - T;
		T = T_new;
		H( );
		Hsumm = h * Flow + Cp_pass * ( T - T0_pass );
		if ( Hsumm < hz )
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

//#define HZ ( hz_0 - Ksi_corp * ( T - _T ))
void CSEps::PH_corp( double _P, double hz, CComp * _pComp, double Ksi_corp, 
		                 double T0 )
	{
	if ( _pComp == NULL )
		_pComp = pN2;
	COUNTER
	//if(::T_min != T_MIN)
	//  T_min = ::T_min;
	//if(::T_max != T_MAX)
	//  T_max = ::T_max;
	SetComp ( _pComp );
	P = _P;
  if ( P > P_max )
    P = P_max;
	if ( T0 != 0. )
		T = T0;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double _T = T;
	double Tb = T;
	double Te = T;
	H( );
	double Hsumm = h + Ksi_corp * ( T - _T );
	double Hb = Hsumm;
	double Eb = E;
	double He = Hsumm;
	double Ee = E;
  kIter = 0;
	if ( Hsumm > hz )
		{
		while ( Hsumm > hz )
			{
      kIter++;
      STEP_T
			Te = Tb;
			He = Hb;
			Ee = Eb;
			if ( T <= T_min )
				{
				SetGamma();
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			H( );
			Hsumm = h + Ksi_corp * ( T - _T );
			Hb = Hsumm;
			Eb = E;
			}
		}
	else
		{
		while ( Hsumm < hz )
			{
      kIter++;
      STEP_T
			Tb = Te;
			Hb = He;
			Eb = Ee;
			if ( T >= T_max )
				{
				SetGamma();
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			H( );
			Hsumm = h + Ksi_corp * ( T - _T );
			He = Hsumm;
			Ee = E;
			}
		}
  if ( kIter > 100 )
    KKK();
	double dT = Te - Tb;
	if ( fabs ( dT ) < 1e-6	)
		{
// Te = Tb
		T = 0.5 * ( Te + Tb );
		SetGamma();
		return;
		}
  if ( Ee - Eb > 0.1 )
		{
// Разрыв
		while ( Te - Tb > 1e-6 )
			{
      kIter++;
			T = 0.5 * ( Tb + Te );
			H( );
			Hsumm = h + Ksi_corp * ( T - _T );
			if ( Hsumm < hz )
				{
				Tb = T;
				Hb = Hsumm;
				Eb = E;
				}
			else
				{
				Te = T;
				He = Hsumm;
				Ee = E;
				}
			}
		if ( He - Hb < 1e-3 )
			{
			SetGamma();
			return;
			}
	// Интерполяция E
		E = Eb + ( hz - Hb ) / ( He - Hb ) * ( Ee - Eb );
		Calc_H( );
		SetGamma();
		return;
		}
// Непрерывный случай
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		if ( kIter > 5 )
			KKK();
		double idH = 1. / ( He - Hb );
		A = ( hz - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
		dT = T_new - T;
		T = T_new;
		H( );
		Hsumm = h + Ksi_corp * ( T - _T );
		if ( Hsumm < hz )
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


void CSEps::PH( double _P, double hz, CComp * _pComp, double T0 )
	{
	COUNTER
  //if(::T_min != T_MIN)
	 // T_min = ::T_min;
  //if(::T_max != T_MAX)
	 // T_max = ::T_max;
  if ( _pComp == NULL )
    _pComp = pN2;
	SetComp ( _pComp );
	P = _P;
  if ( P > P_max )
    P = P_max;
	if ( T0 != 0. )
		T = T0;
  if ( Step_T < 0.01 )
    Step_T = 0.01;
  if ( !_finite ( T ))
    T = T_air;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double Tb = T;
	double Te = T;
	H( );
	double Hb = h;
	double Eb = E;
	double He = h;
	double Ee = E;
  kIter = 0;
  double Tbeg = T;
	if ( h > hz )
		{
		while ( h > hz )
			{
      kIter++;
      STEP_T
      Te = T;
			He = h;
			Ee = E;
			if ( T <= T_min )
				{
				SetGamma();
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			H( );
			Hb = h;
			Eb = E;
			}
		}
	else
		{
		while ( h < hz )
			{
      kIter++;
      STEP_T
			Tb = T;
			Hb = h;
			Eb = E;
			if ( T >= T_max )
				{
				SetGamma();
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			H( );
			He = h;
			Ee = E;
			}
		}
  double Tchange = T - Tbeg;
  if ( kIter > 100 )
    KKK();
//
	double dT = Te - Tb;
	if ( fabs ( dT ) < 1e-6	)
		{
// Te = Tb
		T = 0.5 * ( Te + Tb );
		SetGamma();
		return;
		}
  if ( Ee - Eb > 0.1 )
		{
// Разрыв
		while ( Te - Tb > 1e-6 )
			{
			T = 0.5 * ( Tb + Te );
			H( );
			if ( h < hz )
				{
				Tb = T;
				Hb = h;
				Eb = E;
				}
			else
				{
				Te = T;
				He = h;
				Ee = E;
				}
			}
		if ( He - Hb < 1e-3 )
			{
			SetGamma();
			return;
			}
	// Интерполяция E
		E = Eb + ( hz - Hb ) / ( He - Hb ) * ( Ee - Eb );
		Calc_H( );
		SetGamma();
		return;
		}
// Непрерывный случай
	double A, A1;
	while ( fabs ( dT ) > 1e-6 && fabs ( Te - Tb ) > 1e-6 )
		{
		kIter++;
		if ( kIter > 5 )
			KKK();
    if ( fabs ( He - Hb ) < 1e-8 )
      break;
		double idH = 1. / ( He - Hb );
		A = ( hz - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
    dT = T_new - T;
    T = T_new;
		H( );
		if ( h < hz )
			{
			Tb = T;
			Hb = h;
			}
		else
			{
			Te = T;
			He = h;
			}
		}
	SetGamma();
	}

void CSEps::PH_ext( double _P, double h_before, double KS, double T_ext, CComp * _pComp, double &dH, double T0 )
	{
	COUNTER
  //if(::T_min != T_MIN)
	 // T_min = ::T_min;
  //if(::T_max != T_MAX)
	 // T_max = ::T_max;
  if ( _pComp == NULL )
    _pComp = pN2;
	SetComp ( _pComp );
	P = _P;
  if ( P > P_max )
    P = P_max;
	if ( T0 != 0. )
		T = T0;
	if ( T <= T_min )
		T = T_min + Step_T;
	if ( T >= T_max )
		T = T_max - Step_T;
	double Tb = T;
	double Te = T;
	H( );
  h += KS * ( T - T_ext );
	double Hb = h;
	double Eb = E;
	double He = Hb;
	double Ee = E;
  kIter = 0;
	if ( h > h_before )
		{
		while ( h > h_before )
			{
      kIter++;
			Te = T;
			He = h;
			Ee = E;
			if ( T <= T_min )
				{
        dH = KS * ( T - T_ext );
				SetGamma();
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			H( );
      h += KS * ( T - T_ext );
			Hb = h;
			Eb = E;
			}
		}
	else
		{
		while ( h < h_before )
			{
      kIter++;
			Tb = T;
			Hb = h;
			Eb = E;
			if ( T >= T_max )
				{
        dH = KS * ( T - T_ext );
				SetGamma();
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			H( );
      h += KS * ( T - T_ext );
			He = h;
			Ee = E;
			}
		}
//
	double dT = Te - Tb;
	if ( fabs ( dT ) < 1e-6	)
		{
// Te = Tb
		T = 0.5 * ( Te + Tb );
    dH = KS * ( T - T_ext );
		SetGamma();
		return;
		}
  if ( Ee - Eb > 0.1 )
		{
// Разрыв
		while ( Te - Tb > 1e-6 )
			{
			T = 0.5 * ( Tb + Te );
			H( );
      h += KS * ( T - T_ext );
			if ( h < h_before )
				{
				Tb = T;
				Hb = h;
				Eb = E;
				}
			else
				{
				Te = T;
				He = h;
				Ee = E;
				}
			}
		if ( He - Hb < 1e-3 )
			{
      dH = KS * ( T - T_ext );
			SetGamma();
			return;
			}
	// Интерполяция E
		E = Eb + ( h_before - Hb ) / ( He - Hb ) * ( Ee - Eb );
		Calc_H( );
    dH = KS * ( T - T_ext );
		SetGamma();
		return;
		}
// Непрерывный случай
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		double idH = 1. / ( He - Hb );
		A = ( h_before - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
		dT = T_new - T;
		T = T_new;
		H( );
    h += KS * ( T - T_ext );
		if ( h < h_before )
			{
			Tb = T;
			Hb = h;
			}
		else
			{
			Te = T;
			He = h;
			}
		}
  dH = KS * ( T - T_ext );
	SetGamma();
	}

void CSEps::S( double & s )
	{
	E = CalcEps_PT( P, T, E );
	Calc_S( s );
	}

void CSEps::Calc_S( double & s )
	{
	Set_XY( );
	double s_gas = 0.;
	double s_liq = 0.;
	if ( E > 0. )
		{
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				s_gas += Y_W[n] * S_gas ( n, P * Y_W[n], T );
			}
		}
	if ( E < 1. )
		{
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( WorkComp[n] )
				s_liq += X_W[n] * S_liq ( n, P, T );
			}
		}
	s = E * s_gas + ( 1. - E ) * s_liq;
	}

double CSEps::Calc_S( CSEps & Src )
	{
	double s_gas = 0.;
	double s_liq = 0.;
	if ( Src.E > 0. )
		{
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( Src.WorkComp[n] )
				s_gas += Src.Y_W[n] * S_gas ( n, Src.P * Src.Y_W[n], Src.T );
			}
		}
	if ( Src.E < 1. )
		{
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( Src.WorkComp[n] )
				s_liq += Src.X_W[n] * S_liq ( n, Src.P, Src.T );
			}
		}
	double s = Src.E * s_gas + ( 1. - Src.E ) * s_liq;
	return s;
	}

void CSEps::PS( CSEps & Src, double P_out )
	{
	SetComp ( Src.pComp );
	//if(::T_min != T_MIN)
	//  T_min = ::T_min;
	//if(::T_max != T_MAX)
	//  T_max = ::T_max;
	double s_in = Calc_S ( Src );
//
	if ( T < T_min )
		T = T_min;
	if ( T > T_max )
		T = T_max;
	double Tb = T;
	double Te = T;
	P = P_out;
  if ( P > P_max )
    P = P_max;
	double s_out;
	S( s_out );
	double Sb = s_out;
	double Eb = E;
	double Se = s_out;
	double Ee = E;
	if ( s_out > s_in )
		{
		while ( s_out > s_in )
			{
			Te = T;
			Se = s_out;
			Ee = E;
			if ( T <= T_min )
				{
				Calc_H( );
				return;
				}
			T -= Step_T;
			if ( T < T_min )
				T = T_min;
			Tb = T;
			S( s_out );
			Sb = s_out;
			Eb = E;
			}
		}
	else
		{
		while ( s_out < s_in )
			{
			Tb = T;
			Sb = s_out;
			Eb = E;
			if ( T >= T_max )
				{
				Calc_H( );
				return;
				}
			T += Step_T;
			if ( T > T_max )
				T = T_max;
			Te = T;
			S( s_out );
			Se = s_out;
			Ee = E;
			}
		}
	while ( Te - Tb > 1e-6 )
		{
		T = 0.5 * ( Tb + Te );
		S( s_out );
		if ( s_out < s_in )
			{
			Tb = T;
			Sb = s_out;
			Eb = E;
			}
		else
			{
			Te = T;
			Se = s_out;
			Ee = E;
			}
		}
	if ( Se - Sb < 1e-5 )
		{
		Calc_H( );
		return;
		}
// Интерполяция E
	E = Eb + ( s_in - Sb ) / ( Se - Sb ) * ( Ee - Eb );
	Calc_S( s_out );
	Calc_H( );
	}


int CSEps::SaveState ( )
	{
	S_CLASS("Base",CSBase_W)
	S_CLASS("Work",CSEps_W)
	return 0;
	}

int CSEps::RestoreState ( char * StrName )
	{
	R_CLASS("Base",CSBase_W)
  COPY_FIRST
	R_CLASS("Work",CSEps_W)
	return 0;
	}

int CSEps::GetParams( char * StrName )
{
    char Name[1024];
#include "IO_Parms.h"
	TAB( "Параметры", 5 )
		PARM ( T_min, "T_min" )
		PARM ( T_max, "T_max" ) 
		PARM_B ( GAS, "Газ", false )
		PARM_B ( LIQ, "Жидкость", false )
		PARM_L ( OW_Z, "Тип среды", "-;Нефть;Вода;" )
		PARM ( k_Iter, "k_Iter" )
		PARM ( H2S_MEA, "H2S+MEA" )
		PARM ( MixH2ONH3, "Использовать расширения" ) 
		PARM_D ( CSBase::k_Gamma, "k_Gamma", 0.5 )
//		if ( pChemAdditions )
//			pChemAdditions->GetParams( this );
		TAB( "Henry", 5 )
			for ( int n = 0; n < LAST_GAS; n++ )
			{
				//if ( !WorkComp[n] )
				//	continue;
                sprintf ( Name, "H %s", pGas[n] ->Name );
				PARM_D ( Henry[n], Name, 1. );
			}
		ETAB
	ETAB
	TAB( "Состояние", 5 )
		PARM ( P, "#Давление" )
		PARM ( T, "#Температура" ) 
		PARM ( E, "#Eps" ) 
		PARM ( h, "#h" ) 
		PARM ( dH_liq, "#dH_liq" )
		PARM ( Gamma[G_H2O], "#Gamma H2O" )
		PARM ( dH_liq, "#dH_liq" )
		PARM ( kIter, "#kIter" )
		TAB( "X", 5 )
			for ( int n = 0; n < LAST_GAS; n++ )
			{
				if ( !WorkComp[n] )
					continue;
                sprintf ( Name, "#X %s", pGas[n] ->Name );
				PARM ( X_W[n], Name );
			}
		ETAB
		TAB( "Y", 5 )
			for ( int n = 0; n < LAST_GAS; n++ )
			{
				if ( !WorkComp[n] )
					continue;
                sprintf ( Name, "#Y %s", pGas[n] ->Name );
				PARM ( Y_W[n], Name );
			}
		ETAB
		TAB( "Z", 5 )
			for ( int n = 0; n < LAST_GAS; n++ )
			{
				if ( !WorkComp[n] )
					continue;
                sprintf ( Name, "#Z %s", pGas[n] ->Name );
				PARM ( Z_W[n], Name );
			}
		ETAB
		TAB( "Gamma", 5 )
			for ( int n = 0; n < LAST_GAS; n++ )
			{
				if ( !WorkComp[n] )
					continue;
                sprintf ( Name, "#G %s", pGas[n] ->Name );
				PARM ( Gamma[n], Name );
			}
		ETAB
		TAB( "Pz", 5 )
			for ( int n = 0; n < LAST_GAS; n++ )
			{
				if ( !WorkComp[n] )
					continue;
                sprintf ( Name, "#Pz %s", pGas[n] ->Name );
				PARM ( P_sat[n], Name );
			}
		ETAB
	ETAB
	return 0;
}

