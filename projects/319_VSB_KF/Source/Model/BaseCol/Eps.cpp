#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol.h"
#include "Chem.h"
#include "Connect.h"
#include "Water.h"

double CEpsEps_P::Q_W[K_GAS];
double CEpsEps_P::K_W[K_GAS];
double CEpsEps_P::X_W[K_GAS];
double CEpsEps_P::Y_W[K_GAS];
double CEpsEps_P::Z_W[K_GAS];
double CEpsEps_P::h_Liq[K_GAS];
double CEpsEps_P::h_Gas[K_GAS];
double CEpsEps_P::P_sat[K_GAS];

CEpsEps_P::CEpsEps_P()
	{
  WaterCol = false;
  NH3Col = false;
  Azeotrope = false;
  HenryPrim = 100.;
	T_old = -1.;
	Step_T = 5.;
	E = 0.;
  h_gas = 0.; 
  h_liq = 0.; 
  h = 0.;
  P = 0.;
  GammaPz = 1.;
	}


CEpsEps::CEpsEps( )
	{
	T_old = -100.;
	Step_T = 1.;
  pStage = NULL;
	}


void CEpsEps::SdS( double Eps, double &S, double &dSdE )
	{
	double Sp = 0.;
	double Sm = 0.;
	double dSp = 0.;
	double dSm = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
			{
      if ( ( n == G_H2O || n == G_CH3OH ) && !NH3Col && !WaterCol && !Azeotrope )
        {
        if ( Eps > Z_W[n] / K_W[n] || Eps >= 1. )
          {
          // Вода - пар
          if ( Eps > 1e-9 )
            {
            double C = Z_W[n] / Eps;
            Sp += C;
            dSp -= C / Eps;
            }
          }
        else
          {
          // Вода - есть жидкая фаза
          if (( 1. - Eps ) > 1e-9 )
            {
            double C = ( K_W[n] - Z_W[n] ) / ( 1. - Eps );
            Sm -= C;
            dSm -= C / ( 1. - Eps );
            }
          }
        continue;
        }
			double q = Q_W[n];
//      if ( q < -0.999999 )
//        q = -0.999999;
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
double CEpsEps::Summ( double Eps )
	{
	double S = 0.;
	double Sp = 0.;
	double Sm = 0.;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
			{
      if ( ( n == G_H2O || n == G_CH3OH ) && !NH3Col && !WaterCol )
        {
        if ( Eps > Z_W[n] / K_W[n] )
          {
// Вода - пар
          Sp += Z_W[n] / Eps;
          }
        else
          {
// Вода - есть жидкая фаза
		      if(Eps < 1.0)
			      Sm -= ( K_W[n] - Z_W[n] ) / ( 1. - Eps );
          }
        continue;
        }
			double q = Q_W[n];
			double Zn = 1. + Eps * q;
      if ( Zn < 1e-8 )
        Zn = 1e8;
      else
        Zn = 1. / Zn;
			if ( q > 0. )
				Sp += q * Z_W[n] * Zn;
			else
				Sm -= q * Z_W[n] * Zn;
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


double CEpsEps::CalcEps_PT( double P, double T, double Eps0 )
	{
	if ( P < 0.001 )
    P = 0.001;
	double iP = 1. / P;
	Pz ( T );
	bool All_plus = true;
	bool All_minus = true;
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
			{
			K_W[n] = P_sat[n] * iP;
      FINITE(K_W[n])
      if ( K_W[n] < 1e-5 ) 
        K_W[n] = 1e-5;
			Q_W[n] = K_W[n] - 1.;
			if ( All_plus && Q_W[n] < 0. )
				All_plus = false;
			if ( All_minus && Q_W[n] > 0. )
				All_minus = false;
			}
		}
	if ( All_plus )
		{
		return 1.;
		}
	if ( All_minus )
		{
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
	FINITE(Eps)
	return Eps;
	}

void CEpsEps::Set_XY( )
	{
	double Sx = 0.;
	double Sy = 0.;
	if ( E >= 1. )
		{
		E = 1.;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
        if ( ( n == G_H2O || n == G_CH3OH ) && !NH3Col && !WaterCol && !Azeotrope )
          {
          Y_W[n] = Z_W[n];
          X_W[n] = 0.;
          Sy += Y_W[n];
          Sx += X_W[n];
          continue;
          }
				Y_W[n] = Z_W[n];
//				X_W[n] = Y_W[n] / P_sat[n];
        X_W[n] = 0.;
				Sy += Y_W[n];
				Sx += X_W[n];
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
				if ( IsComp[n] )
					{
          if ( ( n == G_H2O || n == G_CH3OH ) && !NH3Col && !WaterCol && !Azeotrope)
            {
            X_W[n] = Z_W[n];
            Y_W[n] = X_W[n] * P_sat[n];
            Sy += Y_W[n];
            Sx += X_W[n];
            continue;
            }
					X_W[n] = Z_W[n];
					Y_W[n] = X_W[n] * P_sat[n];
					Sy += Y_W[n];
					Sx += X_W[n];
          FINITE(X_W[n])
					}
				}
			}
		else
			{
			for ( int n = 0; n < LAST_GAS; n++ )
				{
				if ( IsComp[n] )
					{
          if ( ( n == G_H2O || n == G_CH3OH ) && !NH3Col && !WaterCol && !Azeotrope)
            {
            if ( E > Z_W[n] / K_W[n] )
              {
              // Вода - пар
              Y_W[n] = Z_W[n] / E;
              X_W[n] = 0.;
              }
            else
              {
              // Вода - есть жидкая фаза
              X_W[n] = ( Z_W[n] - K_W[n] * E ) / ( 1. - E );
              Y_W[n] = K_W[n];
              }
            Sy += Y_W[n];
            Sx += X_W[n];
            continue;
            }
					X_W[n] = Z_W[n] / ( 1. + E * Q_W[n] );
					Y_W[n] = K_W[n] * X_W[n];
					Sx += X_W[n];
					Sy += Y_W[n];
          FINITE(X_W[n])
					}
				}
			}
		}
	if ( fabs ( Sx - 1. ) > 1e-4 )
		{
		double N = 0.;
    if ( Sx > 1e-10 )
      N = 1. / Sx;
    else
      KKK();
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				X_W[n] *= N;
				}
			}
		}
	if ( fabs ( Sy - 1. ) > 1e-4 )
		{
    double N = 0.;
    if ( Sy > 1e-10 )
      N = 1. / Sy;
    else
      KKK();
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				{
				Y_W[n] *= N;
				}
			}
		}
	}

void CEpsEps::Calc_H( )
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
			if ( IsComp[n] )
				h_gas += Y_W[n] * h_Gas[n];
			}
		}
	if ( E < 1. )
		{
		H_liq( );
		h_liq = *dH_liq;
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
        {
				h_liq += X_W[n] * h_Liq[n];
        }
			}
		}
	h = E * h_gas + ( 1. - E ) * h_liq;
	//for ( int n = 0; n < LAST_GAS; n++ )
	//	{
	//	if ( IsComp[n] && h_Liq[n] > h_Gas[n] )
	//		KKK();;
	//	}
	}

//++
void CEpsEps::H( )
	{
	if ( !_finite ( E ))
		E = 0.;
	E = CalcEps_PT( P, *T, E );
	Calc_H( );
	}

void CEpsEps::PH( double _P, double hz, double Cmol[], double T0 )
	{
	SetComp ( Cmol );
	P = _P;
	if ( T0 != 0. )
		*T = T0;
  //if ( pT_max )
  //  T_max = *pT_max;
  //if ( pT_min )
  //  T_min = *pT_min;
	if ( *T <= T_min )
		*T = T_min + Step_T;
	if ( *T >= T_max )
		*T = T_max - Step_T;
	double Tb = *T;
	double Te = *T;
	H( );
  double kT = 100000. * pCol->kInertHeat;
  double hT = h + kT * ( *T - T0 ) + ( A_T_ext_heat * *T - B_T_ext_heat );
	double Hb = hT;
	double Eb = E;
	double He = hT;
	double Ee = E;
	if ( hT > hz )
		{
		while ( hT > hz )
			{
			Te = *T;
			He = hT;
			Ee = E;
			if ( *T <= T_min )
				{
				SetGamma();
        A_T_ext_heat = 0.;
        B_T_ext_heat = 0.;	
        T_ext_heat = *T;
        return;
				}
			*T -= Step_T;
			if ( *T < T_min )
				*T = T_min;
			Tb = *T;
			H( );
      hT = h + kT * ( *T - T0 ) + ( A_T_ext_heat * *T - B_T_ext_heat );
			Hb = hT;
			Eb = E;
			}
		}
	else
		{
		while ( hT < hz )
			{
			Tb = *T;
			Hb = hT;
			Eb = E;
			if ( *T >= T_max )
				{
        A_T_ext_heat = 0.;
        B_T_ext_heat = 0.;	
				SetGamma();
        T_ext_heat = *T;
				return;
				}
			*T += Step_T;
			if ( *T > T_max )
				*T = T_max;
			Te = *T;
			H( );
      hT = h + kT * ( *T - T0 ) + ( A_T_ext_heat * *T - B_T_ext_heat );
			He = hT;
			Ee = E;
			}
		}
//
	double dT = Te - Tb;
	if ( fabs ( dT ) < 1e-6	)
		{
// Te = Tb
		*T = 0.5 * ( Te + Tb );
    A_T_ext_heat = 0.;
    B_T_ext_heat = 0.;	
    T_ext_heat = *T;
		SetGamma();
		return;
		}
  if ( Ee - Eb > 0.1 )
		{
// Разрыв
		while ( Te - Tb > 1e-6 )
			{
			*T = 0.5 * ( Tb + Te );
			H( );
      hT = h + kT * ( *T - T0 ) + ( A_T_ext_heat * *T - B_T_ext_heat );
			if ( hT < hz )
				{
				Tb = *T;
				Hb = hT;
				Eb = E;
				}
			else
				{
				Te = *T;
				He = hT;
				Ee = E;
				}
			}
		if ( He - Hb < 1e-3 )
			{
      A_T_ext_heat = 0.;
      B_T_ext_heat = 0.;	
      T_ext_heat = *T;
			SetGamma();
			return;
			}
	// Интерполяция E
		E = Eb + ( hz - Hb ) / ( He - Hb ) * ( Ee - Eb );
		Calc_H( );
    A_T_ext_heat = 0.;
    B_T_ext_heat = 0.;	
    T_ext_heat = *T;
		SetGamma();
		return;
		}
// Непрерывный случай
	int kIter = 0;
	double A, A1;
	while ( fabs ( dT ) > 1e-6 )
		{
		kIter++;
		double idH = 1. / ( He - Hb );
		A = ( hz - Hb ) * idH;
		A1 = 1. - A;
		double T_new = A * Te + A1 * Tb;
		dT = T_new - *T;
		*T = T_new;
		H( );
    hT = h + kT * ( *T - T0 ) + ( A_T_ext_heat * *T - B_T_ext_heat );
		if ( hT < hz )
			{
			Tb = *T;
			Hb = hT;
			}
		else
			{
			Te = *T;
			He = hT;
			}
		}
  A_T_ext_heat = 0.;
  B_T_ext_heat = 0.;	
  T_ext_heat = *T;
	SetGamma();
	}

void CEpsEps::SetComp ( double Cmol[] )
	{
	memmove ( Z_W, Cmol, CMOL_GAS );
	}

void CEpsEps::Pz( double T )
	{
  double Ps = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
	  {
		if ( !IsComp[n] )
      continue;
    char * Name = pGas[n]->Name;
		P_sat[n] = Henry[n] * Gamma[n] * GammaPz * pGas[n]->Pz ( T );
    Ps += X_W[n] * P_sat[n];
    }
  KKK();
	}

void CEpsEps::H_liq( )
	{
  if ( NH3Col )
    {
    ASS(G_NH3>=0)
    ASS(G_H2O>=0)
    double X_H2O = X_W[G_H2O];
    double X_NH3 = X_W[G_NH3];
    bool Calc_H2O_NH3 = true;
    double X = X_H2O + X_NH3;
    if ( X < 1e-4 )
      Calc_H2O_NH3 = false;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( IsComp[n] )
        h_Liq[n] = pGas[n]-> H_l ( *T );
      }
/*
    if ( !Calc_H2O_NH3 )
      {
      dH_liq = 0.;
      return;
      }
    double x = X_NH3 / X;
    double Hl = CNH3_H2O_Appr::H_l_sat( x, P * X, *T );
    double Hl_id = 
      ( 1. - x ) * h_Liq[G_H2O] + x * h_Liq[G_NH3];
    dH_liq = 0.5 * dH_liq + 0.5 * X * ( Hl - Hl_id );
*/
    return;
    }
  if ( WaterCol )
    {
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( IsComp[n] )
        {
        if ( n == G_H2O )
          {
          if ( pSys->WaterIAPWS )
            h_Liq[n] = Water.Hl( *T, P );
          else
            h_Liq[n] = pGas[n]-> H_l ( *T );
          }
        else
          {
          if ( pGas[n]->AsWater )
            h_Liq[n] = pGas[n]-> H_l ( *T );
          else
            h_Liq[n] = pGas[n]-> H_IG ( *T );
          }
        }
      }
    return;
    }
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      h_Liq[n] = pGas[n]-> H_l ( *T );
    }
	}

void CEpsEps::H_gas( )
	{
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
			{
			if ( n == G_H2O )
        {
        if ( pSys->WaterIAPWS )
          h_Gas[n] = Water.Hv ( *T, P );
        else
          h_Gas[n] = pGas[n]-> H_IG ( *T );
        }
			else
 			  h_Gas[n] = pGas[n]-> H_IG ( *T );
			}
		}
	}

void CEpsEps::SetGamma()
	{
	static double k_Gamma = 0.5;
	static double k_Gamma1 = 1. - k_Gamma;
	if ( T_old == -100. )
		{
		Step_T = 1.;
		T_old = *T;
		}
	else
		{
		Step_T = fabs ( T_old - *T );
		if ( Step_T < 0.01 )
			Step_T = 0.01;
		if ( Step_T > 5. )
			Step_T = 5.;
		T_old = *T;
		}
  bool IsMix = false;
  int k = G_NH3;
  if ( pCol->pColAdditions )
    pCol->pColAdditions->Gamma( &pStage->Numb, P, *T, IsComp, X_W, P_sat, Gamma, *dH_liq );
#if 0
  if ( G_H2O >= 0 && G_NH3 >= 0 && NH3Col )
    {
    if ( IsComp[G_H2O] && IsComp[G_NH3] )
      {
      double x_H2O = X_W[G_H2O];
      double x_NH3 = X_W[G_NH3];
      double Sx = x_H2O + x_NH3;
      if ( Sx < 1e-4 )
        {
        Gamma[G_H2O] = k_Gamma * Gamma[G_H2O] + k_Gamma1;
        Gamma[G_NH3] = k_Gamma * Gamma[G_NH3] + k_Gamma1;
        dH_liq = k_Gamma * dH_liq;
        }
      else
        {
        double N = 1. / Sx;
        x_H2O *= N;
        x_NH3 *= N;
        double Psat = CNH3_H2O_Appr::P_sat( x_NH3, *T );
        double y_HN3 = CNH3_H2O_Appr::Y_sat( x_NH3, *T );
        double P_NH3 = Psat * y_HN3;
        double P_H2O = Psat - P_NH3;
        double Pz_NH3;
        if ( x_NH3 > 1e-6 )
          Pz_NH3 = P_NH3 / x_NH3;
        else
          Pz_NH3 = 0.;
        double Pz_H2O;
        double x1 = 1. - x_NH3;
        if ( x1 > 1e-6 )
          Pz_H2O = P_H2O / x1;
        else
          Pz_H2O = 0.;
        double k_NH3 = Pz_NH3 / P_sat[G_NH3];
        double k_H2O = Pz_H2O / P_sat[G_H2O];
        if ( Gamma[G_NH3] < 1e-5 )
          Gamma[G_NH3] = 1.;
        if ( Gamma[G_H2O] < 1e-5 )
          Gamma[G_H2O] = 1.;
        Gamma[G_NH3] = ( k_Gamma + k_Gamma1 * k_NH3 ) * Gamma[G_NH3];
        Gamma[G_H2O] = ( k_Gamma + k_Gamma1 * k_H2O ) * Gamma[G_H2O];
        double Hl = CNH3_H2O_Appr::H_l_sat( x_NH3, P * Sx, *T );
        double Hl_id = 
          x_H2O * h_Liq[G_H2O] + x_NH3 * h_Liq[G_NH3];
        dH_liq = k_Gamma * dH_liq + k_Gamma1 * Sx * ( Hl - Hl_id );
        }
      return;
      }
    else
      {
      Gamma[G_H2O] = k_Gamma * Gamma[G_H2O] + k_Gamma1;
      Gamma[G_NH3] = k_Gamma * Gamma[G_NH3] + k_Gamma1;
      dH_liq = k_Gamma * dH_liq;
      }
    }
#endif
  }
int CEpsEps::GetParams( IBaseModel * pMain )
	{
#define this pMain 
	#include "IO_Parms.h"
  TAB("Eps",5)
	PARM ( P, "#Давление" )
	PARM ( *T, "#Температура" ) 
	PARM ( E, "#Eps" ) 
  PARM ( HenryPrim, "#HenryPrim" )
	PARM ( h, "#h" ) 
	PARM ( h_gas, "#h_gas" ) 
	PARM ( h_liq, "#h_liq" ) 
	PARM ( T_min, "#T_min" )
	PARM ( T_max, "#T_max" ) 
  ETAB
	return 0;
	}

