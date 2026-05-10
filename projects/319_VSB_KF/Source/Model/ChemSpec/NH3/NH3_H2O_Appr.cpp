#include "stdafx.h"
#if 0
#include "Connect.h"
#include "NH3.h"
#include "ChemBase.h"
//#include "Chem.h"

extern CGas ** pGas;

#include "math.h"
#define DLL_Chem
#include "Chem.h"
#include "CommProc.h"

//DWORD CNH3_H2O_Appr::Err = 0;
//CNH3_H2O NH3_H2O;
/////////////////////////////////////////////////////////////////////////////
// Апроксимации
struct CPzApp_Head
	{
	int kRec;
	};

struct CPzApp
	{
	double X;
	double Tmax, Pmax;
	double CT[3];
	};

struct CHlApp_Head
	{
	int kRec;
	double Tmin, Tmax;
	};

struct CHlApp
	{
	double X;
	double Tmax, Pmax;
	double Pow;
	double CT[8];
	};

struct CYApp_Head
	{
	int kRec;
	};

struct CYApp
	{
	double X;
	double Tmax, Pmax;
	double CT[10];
	};

struct CRolApp_Head
	{
	int kRec;
	};

struct CRolApp
	{
	double X;
	double Tmax, Pmax;
	double CT[6];
	};

struct CRApp_Head
	{
	int kRec;
	};

struct CRApp
	{
	double X;
	double Tmax, Pmax;
	double CT[6];
	};

/////////////////////////////////////////////////////////////////////////////
static CPzApp_Head PzApp_Head;
#define L_PzApp 101
static CPzApp PzApp[L_PzApp];
//
static CHlApp_Head HlApp_Head;
#define L_HlApp 101
static CHlApp HlApp[L_HlApp];
//
static CRApp_Head RApp_Head;
#define L_RApp 101
static CRApp RApp[L_HlApp];
//
static CYApp_Head YApp_Head;
#define L_YApp 101
static CYApp YApp[L_YApp];
//
static CRolApp_Head RolApp_Head;
#define L_RolApp 101
static CRolApp RolApp[L_RolApp];


bool CNH3_H2O_Appr::ReadData()
	{
	char Path[256];
#define READ(DATA,Type) \
		{\
		lstrcpy ( Path, PROJECT_ROOT );\
    lstrcat ( Path, "Data\\Appr\\");\
		lstrcat ( Path, #DATA".head" );\
		FILE * F = NULL;\
		if ( fopen_s ( &F, Path, "rb" ) != 0 )\
			return false;\
		fread ( &Type##_Head, sizeof ( Type##_Head ), 1, F );\
		fclose ( F );\
		ASSS( Type##_Head.kRec == L_##Type );\
		lstrcpy ( Path, PROJECT_ROOT );\
    lstrcat ( Path, "Data\\Appr\\");\
		lstrcat ( Path, #DATA".dat" );\
		if ( fopen_s ( &F, Path, "rb" ) != 0 )\
			return false;\
		fread ( &Type, sizeof ( Type ), 1, F );\
		fclose ( F );\
		}
////////////////////////////////////////////
  READ(NH3_H2O_Pz,PzApp)
  READ(NH3_H2O_R,RApp)
  READ(NH3_H2O_Hl,HlApp)
  READ(NH3_H2O_Y,YApp)
  READ(NH3_H2O_Rol,RolApp)
	return true;
	}

void TestNH3H2O()
  {
  double H2O = 0.99;
  double NH3 = 0.01;
  double T = 20.;
  double P = CNH3_H2O_Appr::P_sat ( NH3, T );
  double h_mix = CNH3_H2O_Appr::H_l_sat ( NH3, P, T );
  double h_NH3 = pGas[G_NH3]->H_v( T ) - pGas[G_NH3]->H_l( T );
  double h_id = H2O * CNH3_H2O_Appr::H_l_sat ( 0., P, T ) + NH3 * ( CNH3_H2O_Appr::H_l_sat ( 1., P, T ) + h_NH3 );
  double dH = h_mix - h_id;
  KKK();
  }

double CNH3_H2O_Appr::P_sat ( double x_NH3, double TC )
	{
	static double C = 35.7;
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CPzApp * pApp = &PzApp[x];
	if ( TC > pApp -> Tmax )
		return 1e10; // За критической точкой
	if ( TC < -50. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -50.;
		}
  if ( TC > 150. )
    {
    //		ON_ERR(ERR_T_MIN)
    TC = 150.;
    }
	double T = TC + TK - C;
	double iT = 1. / T;
	double iT2 = iT * iT;
	double Log_Px = ( pApp->CT[2] * iT + pApp->CT[1] ) * iT + pApp->CT[0];
	double dX = x_NH3100 - x;
	if ( dX < 1e-5 || x == 100 )
		return exp ( Log_Px );
	pApp = &PzApp[x+1];
	double Log_Px1 = ( pApp->CT[2] * iT + pApp->CT[1] ) * iT + pApp->CT[0];
	double Log_P = dX * Log_Px1 + ( 1. - dX ) * Log_Px;
	return exp ( Log_P );
	}

double CNH3_H2O_Appr::H_v ( double x_NH3, double P, double TC, double * pCp )
	{
/*
  CNH3_H2O NH3_H2O;
	NH3_H2O.SetX ( x_NH3 );
	double Pz = P_sat ( x_NH3, TC );
	if ( P > Pz )
		{
		P = Pz;
//		return H_v_sat ( x_NH3, Pz, TC );
		}
	double RT = RP * ( TC + TK );
	double Ro_gas_0 = P / RT;
	double Ro_gas = NH3_H2O.Ro_gas_mol( P, TC, Ro_gas_0 );
	ASSS(Ro_gas >0.)
	if ( pCp )
		*pCp = NH3_H2O.Cp;
	FINITE(NH3_H2O.h)
	return NH3_H2O.h;
*/
	double h = x_NH3 * pGas[G_NH3]->H_IG ( TC ) + ( 1. - x_NH3 ) * pGas[G_H2O]->H_IG ( TC );
	return h;
	}

double CNH3_H2O_Appr::R_sat ( double x_NH3, double TC )
	{
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CRApp * pApp = &RApp[x];
	double Tmax = pApp -> Tmax;
	if ( TC > Tmax )
		return 0.; // За критической точкой
	if ( TC < -20. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -20.;
		}
	double T = TC + TK;
	double Tm = Tmax + TK + 1.;
	double T1 = log ( 1. - T / Tm );
	double * CT = pApp -> CT;
	double log_Rx = 
		CT[0] + 
		( CT[2] * T1 + CT[1] ) * T1 + 
		(( CT[5] * T + CT[4] ) * T + CT[3] ) * T;
//
	double dX = x_NH3100 - x;
	if ( dX < 1e-5 || x == 100 )
		return exp ( log_Rx );
	pApp = &RApp[x+1];
	Tmax = pApp -> Tmax;
	Tm = Tmax + TK + 1.;
	T1 = log ( 1. - T / Tm );
	CT = pApp -> CT;
	double log_Rx1 = 
		CT[0] + 
		( CT[2] * T1 + CT[1] ) * T1 + 
		(( CT[5] * T + CT[4] ) * T + CT[3] ) * T;
	double log_R = dX * log_Rx1 + ( 1. - dX ) * log_Rx;
	return exp ( log_R );
	}

double CNH3_H2O_Appr::dR_satdT ( double x_NH3, double TC )
	{
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CRApp * pApp = &RApp[x];
	double Tmax = pApp -> Tmax;
	if ( TC > Tmax )
		return 0.; // За критической точкой
	if ( TC < -20. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -20.;
		}
	double T = TC + TK;
	double Tm = Tmax + TK + 1.;
	double A1 =	 1. - T / Tm;
	double T1 = log ( A1 );
	double * CT = pApp -> CT;
	double log_Rx = 
		CT[0] + 
		( CT[2] * T1 + CT[1] ) * T1 + 
		(( CT[5] * T + CT[4] ) * T + CT[3] ) * T;
	double dlog_RxdT = 
		-( 2. * CT[2] * T1 + CT[1] ) / ( A1 * Tm ) + 
		 ( 3. * CT[5] * T + 2. * CT[4] ) * T + CT[3];
//
	double dX = x_NH3100 - x;
	if ( dX < 1e-5 || x == 100 )
		return exp ( log_Rx ) * dlog_RxdT;
	pApp = &RApp[x+1];
	Tmax = pApp -> Tmax;
	Tm = Tmax + TK + 1.;
	A1 =	 1. - T / Tm;
	T1 = log ( A1 );
	CT = pApp -> CT;
	double log_Rx1 = 
		CT[0] + 
		( CT[2] * T1 + CT[1] ) * T1 + 
		(( CT[5] * T + CT[4] ) * T + CT[3] ) * T;
	double dlog_Rx1dT = 
		-( 2. * CT[2] * T1 + CT[1] ) / ( A1 * Tm ) + 
		 ( 3. * CT[5] * T + 2. * CT[4] ) * T + CT[3];
	double dR = dX * exp ( log_Rx1 ) * dlog_Rx1dT + 
		    ( 1. - dX ) * exp ( log_Rx ) * dlog_RxdT;
	return dR;
	}

double CNH3_H2O_Appr::H_v_sat ( double x_NH3, double P, double TC )
	{
	double h = x_NH3 * pGas[G_NH3]->H_IG ( TC ) + ( 1. - x_NH3 ) * pGas[G_H2O]->H_IG ( TC );
	return h;
/*
	double Hl = H_l_sat ( x_NH3, P, TC );
	if ( Err == ERR_T_MAX	)
		return Hl;
	double R = R_sat ( x_NH3, TC );
	return Hl + R;
*/
	}

double CNH3_H2O_Appr::H_l_sat ( double x_NH3, double P, double TC )
	{
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CHlApp * pApp = &HlApp[x];
	double Tmax = pApp -> Tmax;
	if ( TC > Tmax )
		{
		return H_v ( x_NH3, P, TC, NULL ); // За критической точкой
		}
	if ( TC < -20. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -20.;
		}
	double T = TC + TK;
	double Tm = Tmax + TK + 1.;
	double T1 = 1. - T / Tm;
	double * CT = pApp -> CT;
	double t = T * 1e-2;
	double Hlx = 
		(((( CT[5] * t + CT[4] ) * t + CT[3] ) * t + CT[2] ) * t + CT[1] ) * t + CT[0] +
		CT[6] * pow ( T1, pApp ->Pow ) +
		CT[7] / t;
//
	double dX = x_NH3100 - x;
	if ( dX < 1e-5 || x == 100 )
		{
		//if ( CalcChem_H )
		//	Hlx += x_NH3 * CNH3_H2O::Hp_F_chem_NH3 + 
		//	       ( 1. - x_NH3 ) * CNH3_H2O::Hp_F_chem_H2O;
		//else
		//	Hlx += x_NH3 * CNH3_H2O::Hp_F_0_NH3 + 
		//	       ( 1. - x_NH3 ) * CNH3_H2O::Hp_F_0_H2O;
		return Hlx;
		}
	pApp = &HlApp[x+1];
	Tmax = pApp -> Tmax;
	if ( TC > Tmax )
		{
		return H_v ( x_NH3, P, TC, NULL ); // За критической точкой
		}
	Tm = Tmax + TK + 1.;
	T1 = 1. - T / Tm;
	CT = pApp -> CT;
	double Hlx1 = 
		(((( CT[5] * t + CT[4] ) * t + CT[3] ) * t + CT[2] ) * t + CT[1] ) * t + CT[0] +
		CT[6] * pow ( T1, pApp ->Pow ) +
		CT[7] / t;
	double Hl = dX * Hlx1 + ( 1. - dX ) * Hlx;
	//if ( CalcChem_H )
	//	Hl += x_NH3 * CNH3_H2O::Hp_F_chem_NH3 + 
	//	       ( 1. - x_NH3 ) * CNH3_H2O::Hp_F_chem_H2O;
	//else
	//	Hl += x_NH3 * CNH3_H2O::Hp_F_0_NH3 + 
	//	       ( 1. - x_NH3 ) * CNH3_H2O::Hp_F_0_H2O;
	return Hl;
	}

double CNH3_H2O_Appr::dH_v_dT ( double x_NH3, double P, double TC )
	{
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CHlApp * pApp = &HlApp[x];
	double Tmax = pApp -> Tmax;
	if ( TC > Tmax )
		{
		return 0.; // За критической точкой
		}
	double Cl = dH_l_dT ( x_NH3, P, TC );
	double dRdT = dR_satdT ( x_NH3, TC );
	return Cl + dRdT;
	}

double CNH3_H2O_Appr::dH_l_dT ( double x_NH3, double P, double TC )
	{
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CHlApp * pApp = &HlApp[x];
	double Tmax = pApp -> Tmax;
	if ( TC > Tmax )
		{
		return 0.; // За критической точкой
		}
	if ( TC < -20. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -20.;
		}
	double T = TC + TK;
	double Tm = Tmax + TK + 1.;
	double T1 = 1. - T / Tm;
	double * CT = pApp -> CT;
	double t = T * 1e-2;
	double Cpx = 
		(((( 5. * CT[5] * t + 4. * CT[4] ) * t + 3. * CT[3] ) * t + 2. * CT[2] ) * t + CT[1] ) * 1e-2 -
		CT[6] * pApp ->Pow * pow ( T1, pApp ->Pow ) / ( T1 * Tm ) -
		CT[7] * 1e-2 / ( t * t );
//
	double dX = x_NH3100 - x;
	if ( dX < 1e-5 || x == 100 )
		return Cpx;
	pApp = &HlApp[x+1];
	Tmax = pApp -> Tmax;
	Tm = Tmax + TK + 1.;
	T1 = 1. - T / Tm;
	CT = pApp -> CT;
	double Cpx1 = 
		(((( 5. * CT[5] * t + 4. * CT[4] ) * t + 3. * CT[3] ) * t + 2. * CT[2] ) * t + CT[1] ) * 1e-2 -
		CT[6] * pApp ->Pow * pow ( T1, pApp ->Pow ) / ( T1 * Tm ) -
		CT[7] * 1e-2 / ( t * t );
	double Cp = dX * Cpx1 + ( 1. - dX ) * Cpx;
	return Cp;
	}

double CNH3_H2O_Appr::Rol_l_sat ( double x_NH3, double TC )
	{
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CRolApp * pApp = &RolApp[x];
	double Tmax = pApp -> Tmax;
	if ( TC > Tmax )
		return 0.; // За критической точкой
	if ( TC < -20. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -20.;
		}
	double T = TC + TK;
	double Tm = Tmax + TK + 1.;
	double T1 = log ( 1. - T / Tm );
	double * CT = pApp -> CT;
	double log_Rx = 
		CT[0] + 
		( CT[2] * T1 + CT[1] ) * T1 + 
		(( CT[5] * T + CT[4] ) * T + CT[3] ) * T;
//
	double dX = x_NH3100 - x;
	if ( dX < 1e-5 || x == 100 )
		return exp ( log_Rx );
	pApp = &RolApp[x+1];
	Tmax = pApp -> Tmax;
	Tm = Tmax + TK + 1.;
	T1 = log ( 1. - T / Tm );
	CT = pApp -> CT;
	double log_Rx1 = 
		CT[0] + 
		( CT[2] * T1 + CT[1] ) * T1 + 
		(( CT[5] * T + CT[4] ) * T + CT[3] ) * T;
	double log_R = dX * log_Rx1 + ( 1. - dX ) * log_Rx;
	return exp ( log_R );
	}

double CNH3_H2O_Appr::Y_sat ( double x_NH3, double TC )
	{
	if ( x_NH3 < 0. )
		x_NH3 = 0.;
	if ( x_NH3 > 1. )
		x_NH3 = 1.;
	double x_NH3100 = x_NH3 * 100.;
	int x = (int)x_NH3100;
	CYApp * pApp = &YApp[x];
	if ( TC < -20. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -20.;
		}
	if ( TC > pApp -> Tmax )
		{
//		ON_ERR(ERR_T_MAX)
		TC = pApp -> Tmax;
		}
	double T = TC * 1e-2;
	double Tn[10];
	Tn[0] = 1.;
	double Yx = pApp->CT[0];
	for ( int n = 1; n < 10; n++ )
		{
		Tn[n]	= Tn[n-1] * T;
		Yx += Tn[n] * pApp->CT[n];
		}
	double dX = x_NH3100 - x;
	if ( dX < 1e-5 || x == 100 )
		{
		if ( Yx < 0. )
			Yx = 0.;
		if ( Yx > 1. )
			Yx = 1.;
		return Yx;
		}
	pApp = &YApp[x+1];
	double Yx1 = pApp->CT[0];
	for ( int n = 1; n < 10; n++ )
		{
		Yx1 += Tn[n] * pApp->CT[n];
		}
	double Y = dX * Yx1 + ( 1. - dX ) * Yx;
	if ( Y < 0. )
		Y = 0.;
	if ( Y > 1. )
		Y = 1.;
	return Y;
	}


void TestNH3H2O()
  {
  double H2O = 0.;
  double NH3 = 1.;
  double T = 40.;
//
  double P = pNH3_H2O_Appr->P_sat ( NH3, T );
  double P_NH3 = pGas[G_NH3]->Pz( T );
  double P_H2O = pGas[G_H2O]->Pz( T );
//  P_NH3/P;
//  P_H2O/P;
//
  double hl_mix = pNH3_H2O_Appr->H_l_sat ( NH3, P, T );
  double hl_NH3 = pGas[G_NH3]->H_l( T );
  double hl_H2O = pGas[G_H2O]->H_l( T );
//  hl_NH3/hl_mix;
//  hl_H2O/hl_mix;
  //
  double hv_mix = pNH3_H2O_Appr->H_v ( NH3, P, T );
  double hv_NH3 = pGas[G_NH3]->H_IG( T );
  double hv_H2O = pGas[G_H2O]->H_IG( T );
//  hv_NH3/hv_mix;
//  hv_H2O/hv_mix;
//  double h_id = H2O * pNH3_H2O_Appr->H_l_sat ( 0., P, T ) + NH3 * ( pNH3_H2O_Appr->H_l_sat ( 1., P, T ));
//  double dH = h_mix - h_id;
  KKK();
  }

#endif
