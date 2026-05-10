#include "stdafx.h"
#include "math.h"
#include "Chem.h"
#include "IAPWS.h"
#include "Err.h"
#include "CommProc.h"
#include "Water.h"

CWater_appr Water_appr;

struct CHappr_P
	{
	double a0, a1, a2, a3;
	double Err_max, Err_med;
	};

struct CHappr_Hv_P : public CHappr_P
	{
	double Pmax;
	};

struct CHappr_Hl_P : public CHappr_P
	{
	double Pmin;
	};

struct CPappr_T
	{
	double Pz, A, B;
	};

/***********
struct CHappr_T
	{
	double a0, a1, a2, a3, a4;
	double Err_max, Err_med;
	};

struct CHappr_Hv_T : public CHappr_T
	{
	double Tmin;
	};

struct CHappr_Hl_T : public CHappr_T
	{
	double Tmax;
	};

struct CHappr_Hv_Ti : public CHappr_T
	{
	double Tmin;
	double Hmin, Hmax;
	};

struct CHappr_Hl_Ti : public CHappr_T
	{
	double Tmax;
	double Hmin, Hmax;
	};
***********/

struct CHappr_H_Z
	{
	double h0, Cp;
	};

static CHappr_Hv_P apprHv_P[1001];
static CHappr_Hl_P apprHl_P[365];// // P < 200
static CPappr_T apprPs_T[374];
static CHappr_H_Z	apprHv_Z[374]; // На линии конденсации
static CHappr_H_Z	apprHl_Z[374]; // На линии конденсации
static bool MustRead = true;

/*********
static CHappr_Hv_P apprHv_P[701];
static CHappr_Hl_P apprHl_P[311];//[374];	 // P < 100
static CPappr_T apprPs_T[374];
//
static double P_appr[1001];
static CHappr_Hv_T apprHv_T[1001];
static CHappr_Hl_T apprHl_T[1001];
static CHappr_Hv_Ti apprHv_Ti[1001];
static CHappr_Hl_Ti apprHl_Ti[1001];
static CHappr_H_Z	apprHv_Z[374]; // На линии конденсации
static CHappr_H_Z	apprHl_Z[374]; // На линии конденсации
***********/
///////////////////////////////////////////////////////////////////
bool CWater_appr::ReadData()
	{
  MustRead = false;
	char Path[256];
	lstrcpy ( Path, PROJECT_ROOT );
	lstrcat ( Path, "Data\\Appr\\");
	lstrcat ( Path, "H2O.dat" );
  FILE * F = NULL;
  if ( fopen_s ( &F, Path, "rb" ) != 0 )
		return false;
#define READ(Arr) fread ( Arr, sizeof ( Arr ), 1, F );
	READ ( apprPs_T )
	READ ( apprHl_P )
	READ ( apprHv_P )
	//READ ( P_appr )
	//READ ( apprHv_T )
	//READ ( apprHl_T )
	//READ ( apprHv_Ti )
	//READ ( apprHl_Ti )
	READ ( apprHv_Z )
	READ ( apprHl_Z )
	fclose ( F );
	return true;
	}

bool CWater::Vap_Appr ( double T_C, double &Hv, double &dHvdT )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	if ( T_C >= 373. )
		return false;
	if ( T_C < 0. )
		return false;
	int n = (int)T_C;
	double d = T_C - n;
	double h0 = apprHv_Z[n].h0;
	double Cp = apprHv_Z[n].Cp;
	Hv = h0 + d * Cp;
	if ( CalcChem_H )
		Hv += dH0_chem;
	else
		Hv += dH0_0;
	dHvdT = Cp;
	return true;
	}

bool CWater::Liq_Appr ( double T_C, double &Hl, double &dHldT )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	if ( T_C >= 373. )
		return false;
	if ( T_C < 0. )
		return false;
	int n = (int)T_C;
	double d = T_C - n;
	double h0 = apprHl_Z[n].h0;
	double Cp = apprHl_Z[n].Cp;
	Hl = h0 + d * Cp;
	if ( CalcChem_H )
		Hl += dH0_chem;
	else
		Hl += dH0_0;
	dHldT = Cp;
	return true;
	}

double CWater_appr::P_z ( double TC )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	if ( TC >= 373. )
		{
		*pErr |= ERR_T_MAX;
		return 1000.;
		}
	if ( TC <= 0. )
		{
		*pErr |= ERR_T_MIN;
		return 0.001;
		}
	int n = (int)TC;
	double T = TC + TK;
	return exp ( apprPs_T[n].A + apprPs_T[n].B / T );
	}


bool CWater::Psat_Appr ( double TC, double &P, double &dPdT )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	Err = 0;
	if ( TC >= 373. )
		{
		Err |= ERR_T_MAX;
		TC = 372.9;
		}
	if ( TC <= 0.1 )
		{
		Err |= ERR_T_MIN;
		TC = 0.1;
		}
	int n = (int)TC;
	double T = TC + TK;
	P = exp ( apprPs_T[n].A + apprPs_T[n].B / T );
	dPdT = -P * apprPs_T[n].B / ( T * T );
	return true;
	}

double CWater::Pmin( )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	return apprPs_T[0].Pz;
	}

void CWater::iPsat_Appr ( double P, double &T_C, double &dTdP )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	Err = 0;
	if ( P < apprPs_T[0].Pz )
		{
		Err |= ERR_P_MIN;
		P = apprPs_T[0].Pz;
		}
	if ( P > apprPs_T[373].Pz )
		{
		Err |= ERR_P_MAX;
		P = apprPs_T[373].Pz;
		}
	int nb = 0;
	int ne = 373;
	int n;
	while ( ne - nb > 1 )
		{
		n = ( nb + ne ) >> 1;
		if ( P < apprPs_T[n].Pz )
			ne = n;
		else
			nb = n;
		}
//
	double A = apprPs_T[nb].A;
	double B = apprPs_T[nb].B;
	double Z = 1. / ( log ( P ) - A );
	double T = B * Z;
	dTdP = -B * Z * Z / P;
	T_C = T - TK;
	}

double CWater_appr::T_z ( double P )
	{
  if ( MustRead )
  ASS(CWater_appr::ReadData());
	*pErr = 0;
	if ( P < apprPs_T[0].Pz )
		{
		*pErr |= ERR_P_MIN;
		P = apprPs_T[0].Pz;
		}
	if ( P > apprPs_T[373].Pz )
		{
		*pErr |= ERR_P_MAX;
		P = apprPs_T[373].Pz;
		}
	int nb = 0;
	int ne = 373;
	int n;
	while ( ne - nb > 1 )
		{
		n = ( nb + ne ) >> 1;
		if ( P < apprPs_T[n].Pz )
			ne = n;
		else
			nb = n;
		}
//
	double A = apprPs_T[nb].A;
	double B = apprPs_T[nb].B;
	double T = B / ( log ( P ) - A );
	return T - TK;
	}

double CWater_appr::H_v_P ( double TC, double P )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	if ( TC >= 1000. )
		{
		*pErr |= ERR_T_MAX;
		TC = 999.9;
		}
	if ( TC <= 0. )
		{
		*pErr |= ERR_T_MIN;
		TC = 0.001;
		}
	if ( TC < 800. )
		{
		if ( P >= 200. )
			{
			*pErr |= ERR_P_MAX;
			P = 200.;
			}
		}
	else
		{
		if ( P >= 100. )
			{
			*pErr |= ERR_P_MAX;
			P = 100.;
			}
		}
	if ( TC >= 998. )
		{
		*pErr |= ERR_T_MAX;
		TC = 998.;
		}
	int n = (int)TC;
	if ( n > 998 )
		n = 998;
	CHappr_Hv_P & h = apprHv_P[n];
	CHappr_Hv_P & h1 = apprHv_P[n+1];
	if ( P - h1.Pmax > 1e-3 )
		{
		*pErr |= ERR_P_MAX;
		P = h1.Pmax;
		}
	double dP = P - h.Pmax;
	double H = (( h.a3 * dP + h.a2 ) * dP + h.a1 ) * dP + h.a0; 
	double dP1 = P - h1.Pmax;
	double H1 = (( h1.a3 * dP1 + h1.a2 ) * dP1 + h1.a1 ) * dP1 + h1.a0;
	double a = TC - n;
	double a1 = 1. - a;
	double hv = a1 * H + a * H1;
	return hv;
	}

double CWater_appr::Cp_v_P ( double TC, double P )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	if ( TC >= 700. )
		{
		*pErr |= ERR_T_MAX;
		TC = 699.9;
		}
	if ( TC <= 0. )
		{
		*pErr |= ERR_T_MIN;
		TC = 0.001;
		}
	if ( TC < 800. )
		{
		if ( P >= 200. )
			{
			*pErr |= ERR_P_MAX;
			P = 200.;
			}
		}
	else
		{
		if ( P >= 100. )
			{
			*pErr |= ERR_P_MAX;
			P = 100.;
			}
		}
	if ( TC >= 998. )
		{
		*pErr |= ERR_T_MAX;
		TC = 998.;
		}
	int n = (int)TC;
	if ( n > 998 )
		n = 998;
	CHappr_Hv_P & h = apprHv_P[n];
	CHappr_Hv_P & h1 = apprHv_P[n+1];
	if ( P - h1.Pmax > 1e-3 )
		{
		*pErr |= ERR_P_MAX;
		P = h1.Pmax;
		}
	double dP = P - h.Pmax;
	double H = (( h.a3 * dP + h.a2 ) * dP + h.a1 ) * dP + h.a0; 
	double dP1 = P - h1.Pmax;
	double H1 = (( h1.a3 * dP1 + h1.a2 ) * dP1 + h1.a1 ) * dP1 + h1.a0;
	double Cpv = H1 - H;
	return Cpv;
	}

double CWater_appr::H_l_P ( double TC, double P )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	if ( TC >= 373. )
		return H_v_P ( TC, P );
	if ( TC <= 0. )
		{
		*pErr |= ERR_T_MIN;
		TC = 0.001;
		}
	if ( P >= 200. )
		{
		*pErr |= ERR_P_MAX;
		P = 200.;
		}
	if ( TC >= 363. )
		{
		*pErr |= ERR_T_MAX;
		TC = 363.;
		}
	int n = (int)TC;
	if ( n > 363 )
		n = 363;
	CHappr_Hl_P & h = apprHl_P[n];
	CHappr_Hl_P & h1 = apprHl_P[n+1];
	if ( P - h.Pmin < -1e-3 )
		{
		*pErr |= ERR_P_MIN;
		P = h.Pmin;
		}
	double dP = P - h.Pmin;
	double H = (( h.a3 * dP + h.a2 ) * dP + h.a1 ) * dP + h.a0; 
	double dP1 = P - h1.Pmin;
	double H1 = (( h1.a3 * dP1 + h1.a2 ) * dP1 + h1.a1 ) * dP1 + h1.a0;
	double a = TC - n;
	double a1 = 1. - a;
	double hl = a1 * H + a * H1;
	return hl;
	}

double CWater_appr::Cp_l_P ( double TC, double P )
	{
  if ( MustRead )
    ASS(CWater_appr::ReadData());
	if ( TC >= 373. )
		return H_v_P ( TC, P );
	if ( TC <= 0. )
		{
		*pErr |= ERR_T_MIN;
		TC = 0.001;
		}
	if ( P >= 200. )
		{
		*pErr |= ERR_P_MAX;
		P = 200.;
		}
	if ( TC >= 363. )
		{
		*pErr |= ERR_T_MAX;
		TC = 363.;
		}
	int n = (int)TC;
	if ( n > 363 )
		n = 363;
	CHappr_Hl_P & h = apprHl_P[n];
	CHappr_Hl_P & h1 = apprHl_P[n+1];
	if ( P - h.Pmin < -1e-3 )
		{
		*pErr |= ERR_P_MIN;
		P = h.Pmin;
		}
	double dP = P - h.Pmin;
	double H = (( h.a3 * dP + h.a2 ) * dP + h.a1 ) * dP + h.a0; 
	double dP1 = P - h1.Pmin;
	double H1 = (( h1.a3 * dP1 + h1.a2 ) * dP1 + h1.a1 ) * dP1 + h1.a0;
	double Cpl = H1 - H;
	return Cpl;
	}

/*******************
static double l_Pmin = log ( 0.01 );
static double l_Pmax = log ( 100. );
static double dP = ( l_Pmax - l_Pmin ) / 1000.;
double CWater_appr::H_v_T ( double TC, double P )
	{
	if ( P < 0.0101 )
		{
		*pErr |= ERR_P_MIN;
		P = 0.0101;
		}
	if ( P > 99.99 )
		{
		*pErr |= ERR_P_MAX;
		P = 99.99;
		}
	double l_P = log ( P );
	int n = (int)(( l_P - l_Pmin ) / dP );
	CHappr_Hv_T & h = apprHv_T[n];
	CHappr_Hv_T & h1 = apprHv_T[n+1];
	if ( TC < h.Tmin - 2. )
		{
		*pErr |= ERR_T_MIN;
		TC = h.Tmin - 2.;
		}
	double dT = TC - h.Tmin;
	double H = ((( h.a4 * dT + h.a3 ) * dT + h.a2 ) * dT + h.a1 ) * dT + h.a0; 
	double dT1 = TC - h1.Tmin;
	double H1 = ((( h1.a4 * dT1 + h1.a3 ) * dT1 + h1.a2 ) * dT1 + h1.a1 ) * dT1 + h1.a0;
	double a = ( P - P_appr[n] ) / ( P_appr[n+1] - P_appr[n] );
	double a1 = 1. - a;
	double hv = a1 * H + a * H1;
	return hv;
	}

double CWater_appr::Cp_v_T ( double TC, double P )
	{
	if ( P < 0.0101 )
		{
		*pErr |= ERR_P_MIN;
		P = 0.0101;
		}
	if ( P > 99.99 )
		{
		*pErr |= ERR_P_MAX;
		P = 99.99;
		}
	double l_P = log ( P );
	int n = (int)(( l_P - l_Pmin ) / dP );
	CHappr_Hv_T & h = apprHv_T[n];
	CHappr_Hv_T & h1 = apprHv_T[n+1];
	if ( TC < h.Tmin - 2. )
		{
		*pErr |= ERR_T_MIN;
		TC = h.Tmin - 2.;
		}
	double Cp = (( 4. * h.a4 * TC + 3. * h.a3 ) * TC + 2. * h.a2 ) * TC + h.a1; 
	double Cp1 = (( 4. * h1.a4 * TC + 3. * h1.a3 ) * TC + 2. * h1.a2 ) * TC + h1.a1;
	double a = ( P - P_appr[n] ) / ( P_appr[n+1] - P_appr[n] );
	double a1 = 1. - a;
	double Cpv = a1 * Cp + a * Cp1;
	return Cpv;
	}

double CWater_appr::H_l_T ( double TC, double P )
	{
	if ( P < 0.0101 )
		{
		*pErr |= ERR_P_MIN;
		P = 0.0101;
		}
	if ( P > 99.99 )
		{
		*pErr |= ERR_P_MAX;
		P = 99.99;
		}
	double l_P = log ( P );
	int n = (int)(( l_P - l_Pmin ) / dP );
	CHappr_Hl_T & h = apprHl_T[n];
	CHappr_Hl_T & h1 = apprHl_T[n+1];
	if ( TC > h1.Tmax )
		{
		*pErr |= ERR_T_MAX;
		TC = h1.Tmax;
		}
	double H = ((( h.a4 * TC + h.a3 ) * TC + h.a2 ) * TC + h.a1 ) * TC + h.a0; 
	double H1 = ((( h1.a4 * TC + h1.a3 ) * TC + h1.a2 ) * TC + h1.a1 ) * TC + h1.a0;
	double a = ( P - P_appr[n] ) / ( P_appr[n+1] - P_appr[n] );
	double a1 = 1. - a;
	double hl = a1 * H + a * H1;
	return hl;
	}

double CWater_appr::Cp_l_T ( double TC, double P )
	{
	if ( P < 0.0101 )
		{
		*pErr |= ERR_P_MIN;
		P = 0.0101;
		}
	if ( P > 99.99 )
		{
		*pErr |= ERR_P_MAX;
		P = 99.99;
		}
	double l_P = log ( P );
	int n = (int)(( l_P - l_Pmin ) / dP );
	CHappr_Hl_T & h = apprHl_T[n];
	CHappr_Hl_T & h1 = apprHl_T[n+1];
	if ( TC > h1.Tmax )
		{
		*pErr |= ERR_T_MAX;
		TC = h1.Tmax;
		}
	double Cp = (( 4. * h.a4 * TC + 3. * h.a3 ) * TC + 2. * h.a2 ) * TC + h.a1; 
	double Cp1 = (( 4. * h1.a4 * TC + 3. * h1.a3 ) * TC + 2. * h1.a2 ) * TC + h1.a1;
	double a = ( P - P_appr[n] ) / ( P_appr[n+1] - P_appr[n] );
	double a1 = 1. - a;
	double Cpl = a1 * Cp + a * Cp1;
	return Cpl;
	}

double CWater_appr::iH_v_T ( double H, double P, double T0 )
	{
	*pLimit = false;
	double T = T0 + 2;
	double dT = 1e10;
	while ( fabs ( dT ) > 1e-5 )
		{
		double h = H_v_T ( T, P );
		if ( *pLimit )
			break;
		double dH = H - h;
		dT = dH / Cp_v_T ( T, P );
		if ( *pLimit )
			break;
		T += LimdX ( dT, 5. );
		}
	return T;
	}

double CWater_appr::iH_v_T ( double H, double P )
	{
	if ( P < 0.0101 )
		{
		*pErr |= ERR_P_MIN;
		P = 0.0101;
		}
	if ( P > 99.99 )
		{
		*pErr |= ERR_P_MAX;
		P = 99.99;
		}
	double l_P = log ( P );
	int n = (int)(( l_P - l_Pmin ) / dP );
	CHappr_Hv_Ti & h = apprHv_Ti[n];
	CHappr_Hv_Ti & h1 = apprHv_Ti[n+1];
//
	if ( H < h.Hmin )
		{
		*pErr |= ERR_H_MIN;
		H = h.Hmin;
		}
	if ( H > h.Hmax )
		{
		*pErr |= ERR_H_MAX;
		H = h.Hmax;
		}
	if ( H < h1.Hmin )
		{
		*pErr |= ERR_H_MIN;
		H = h1.Hmin;
		}
	if ( H > h1.Hmax )
		{
		*pErr |= ERR_H_MAX;
		H = h1.Hmax;
		}
	double dH = H - h.Hmin;
	double T = ((( h.a4 * dH + h.a3 ) * dH + h.a2 ) * dH + h.a1 ) * dH + h.a0; 
	double dH1 = H - h1.Hmin;
	double T1 = ((( h1.a4 * dH1 + h1.a3 ) * dH1 + h1.a2 ) * dH1 + h1.a1 ) * dH1 + h1.a0;
	double a = ( P - P_appr[n] ) / ( P_appr[n+1] - P_appr[n] );
	double a1 = 1. - a;
	double Tv = a1 * T + a * T1;
	if ( Tv < h.Tmin )
		{
		*pErr |= ERR_T_MIN;
		Tv = h.Tmin;
		}
	if ( Tv < h1.Tmin )
		{
		*pErr |= ERR_T_MIN;
		Tv = h1.Tmin;
		}
	return Tv;
	}

double CWater_appr::iH_l_T ( double H, double P, double T0 )
	{
	*pErr = 0;
	double T = T0;
	double dT = 1e10;
	while ( fabs ( dT ) > 1e-5 )
		{
		double h = H_l_T ( T, P );
		if ( *pErr )
			break;
		double dH = H - h;
		dT = dH / Cp_l_T ( T, P );
		if ( *pErr )
			break;
		T += LimdX ( dT, 5. );
		}
	return T;
	}

double CWater_appr::iH_l_T ( double H, double P )
	{
	*pErr = 0;
	if ( P < 0.0101 )
		{
		*pErr |= ERR_P_MIN;
		P = 0.0101;
		}
	if ( P > 99.99 )
		{
		*pErr |= ERR_P_MAX;
		P = 99.99;
		}
	double l_P = log ( P );
	int n = (int)(( l_P - l_Pmin ) / dP );
	CHappr_Hl_Ti & h = apprHl_Ti[n];
	CHappr_Hl_Ti & h1 = apprHl_Ti[n+1];
//
	if ( H < h.Hmin )
		{
		*pErr |= ERR_H_MIN;
		H = h.Hmin;
		}
	if ( H > h.Hmax )
		{
		*pErr |= ERR_H_MAX;
		H = h.Hmax;
		}
	if ( H < h1.Hmin )
		{
		*pErr |= ERR_H_MIN;
		H = h1.Hmin;
		}
	if ( H > h1.Hmax )
		{
		*pErr |= ERR_H_MAX;
		H = h1.Hmax;
		}
	double dH = H - h.Hmin;
	double T = ((( h.a4 * dH + h.a3 ) * dH + h.a2 ) * dH + h.a1 ) * dH + h.a0; 
	double dH1 = H - h1.Hmin;
	double T1 = ((( h1.a4 * dH1 + h1.a3 ) * dH1 + h1.a2 ) * dH1 + h1.a1 ) * dH1 + h1.a0;
	double a = ( P - P_appr[n] ) / ( P_appr[n+1] - P_appr[n] );
	double a1 = 1. - a;
	double Tl = a1 * T + a * T1;
	if ( Tl > h.Tmax )
		{
		*pErr |= ERR_T_MAX;
		Tl = h.Tmax;
		}
	if ( Tl > h1.Tmax )
		{
		*pErr |= ERR_T_MAX;
		Tl = h1.Tmax;
		}
	return Tl;
	}
*************/

class CGasEx : public CReal
  {
  public:
    virtual  double Cpv ( double Tc, double P ) = 0;
    virtual  double Cpl ( double Tc, double P ) = 0;
    virtual  double Hv ( double Tc, double P ) = 0;
    virtual  double Hl ( double Tc, double P ) = 0;
    virtual  double Sv ( double Tc, double P ) = 0;
    virtual  double Sl ( double Tc, double P ) = 0;
    virtual  double Rov ( double Tc, double P ) = 0;
    virtual  double Rol ( double Tc, double P ) = 0;
  };


struct CWaterAppr : public CGasEx
  {
  CWaterAppr();
  void Init(); //0 
  double C_IG ( double TC );	//Функция нахождения теплоемкости при постоянном давлении
  double H_IG ( double TC );	//Функция нахождения энтальпии
  double H_l ( double TC );
  double Pz ( double Tc );
  double H_vap ( double TC );
  bool   Ro_liq ( double T_C, double & Ro );
  bool   To_m3_liq ( double TC, double & To_m3 );

  double S_IG( double P, double TC );	
  void   S_IG( double P, double TC, double & S, double & dSdT ) { ASS(0); S = 0.; dSdT = 0.; }
  double G_IG( double P, double TC );

  double dHobr( double Tc ) { return 0.; };
  double dSobr( double Tc ) { return 0.; };
  double dGobr( double Tc ) { return 0.; };

  double H_Chem ( double TC ) { return 0.; };
  double dH_Chem ( ) { return 0.; };

  int ReadData( _HANDLE hFile );
  //
  double Cpv ( double Tc, double P );
  double Cpl ( double Tc, double P );
  double Hv ( double Tc, double P );
  double Hl ( double Tc, double P );
  double Sv ( double Tc, double P );
  double Sl ( double Tc, double P );
  double Rov ( double Tc, double P );
  double Rol ( double Tc, double P );
  NEW
  };

CGas * Create_Water_Appr()
  {
  return new CWaterAppr();
  }

CWaterAppr::CWaterAppr()
  {
  Init();
  }

void CWaterAppr::Init()
  {
  GlobalGamma = 1.;
  Function = eHv|eHl|ePz|eChem;
  Mw = Mw_H2O;
  Tb = 100.;
  Tcr = 647.;
  Pcr = 220.64; // bar
  Vcr = 0.; 
  Rocr = 17.9; // Моль/л
  Zcr = 1.;
  Omega = 1.;
  lstrcpy ( CGas::Name, "WATER" );
  lstrcpy ( CGas::Alt_Name, "Вода" );
  lstrcpy ( CGas::Formula, "H2O" );
  AsWater = true;
  Numb = -1;
  k_C = 0.;
  k_H = 2.;
  k_O = 1.;
  ASS(CWater_appr::ReadData())
  }

const double Pvap = 1.;
const double Pliq = 100.;

double CWaterAppr::C_IG ( double TC )
  {
  return Water_appr.Cp_v_P( TC, Pvap );
  }

double CWaterAppr::H_IG ( double TC )
  {
  return Water_appr.H_v_P ( TC, Pvap );
  }

double CWaterAppr::H_l ( double TC )
  {
  return Water_appr.H_l_P ( TC, Pliq );
  }

double CWaterAppr::Pz ( double Tc )
  {
  return GlobalGamma * Water_appr.P_z ( Tc );
  }

double CWaterAppr::H_vap ( double TC )
  {
  return H_IG ( TC ) - H_l ( TC );
  }
int CWaterAppr::ReadData( _HANDLE hFile )
  {
  return CReal::ReadData( hFile );
  }

double CWaterAppr::Cpv ( double Tc, double P )
  {
  return Water_appr.Cp_v_P (  Tc,  P );
  }
double CWaterAppr::Cpl ( double Tc, double P )
  {
  return Water_appr.Cp_l_P (  Tc,  P );
  }
double CWaterAppr::Hv ( double Tc, double P )
  {
  return Water_appr.H_v_P(  Tc,  P );
  }
double CWaterAppr::Hl ( double Tc, double P )
  {
  return Water_appr.H_l_P (  Tc,  P );
  }


bool CWaterAppr::Ro_liq ( double T_C, double & Ro )
  {
  Ro = Water.Rol ( T_C, Pliq );
  return true;
  }

bool CWaterAppr::To_m3_liq ( double TC, double & To_m3 )
  {
  double Ro = Water.Rol ( TC, Pliq );
  To_m3 = Mw / Ro;
  return true;
  }

double CWaterAppr::S_IG( double P, double TC )
  {
  return Water.Sv ( TC, P );
  }

double CWaterAppr::G_IG( double P, double TC )
  {
  return H_IG ( TC ) - ( TC + TK ) * S_IG ( P, TC );
  }

double CWaterAppr::Sv ( double Tc, double P )
  {
  return Water.Sv (  Tc,  P );
  }
double CWaterAppr::Sl ( double Tc, double P )
  {
  return Water.Sl (  Tc,  P );
  }
double CWaterAppr::Rov ( double Tc, double P )
  {
  return Water.Rov (  Tc,  P );
  }
double CWaterAppr::Rol ( double Tc, double P )
  {
  return Water.Rol (  Tc,  P );
  }

/*
https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method

\ln P_{r}=f^{{(0)}}+\omega \cdot f^{{(1)}}

  {\displaystyle f^{(0)}=5.92714-{\frac {6.09648}{T_{r}}}-1.28862\cdot \ln T_{r}+0.169347\cdot T_{r}^{6}}f^{{(0)}}=5.92714-{\frac  {6.09648}{T_{r}}}-1.28862\cdot \ln T_{r}+0.169347\cdot T_{r}^{6}

  {\displaystyle f^{(1)}=15.2518-{\frac {15.6875}{T_{r}}}-13.4721\cdot \ln T_{r}+0.43577\cdot T_{r}^{6}}f^{{(1)}}=15.2518-{\frac  {15.6875}{T_{r}}}-13.4721\cdot \ln T_{r}+0.43577\cdot T_{r}^{6}

  with

    {\displaystyle P_{r}={\frac {P}{P_{c}}}}P_{r}={\frac  {P}{P_{c}}} (reduced pressure) and {\displaystyle T_{r}={\frac {T}{T_{c}}}}T_{r}={\frac  {T}{T_{c}}} (reduced temperature).

*/
