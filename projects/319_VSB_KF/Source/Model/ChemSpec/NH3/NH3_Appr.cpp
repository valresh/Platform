#include "stdafx.h"
#if 0
#include "math.h"
#define DLL_Chem
#include "Chem.h"
#include "CommProc.h"

void CallOnErr_NH3 ( DWORD Error )
	{
	ASSS(FALSE)
	}

//DWORD CNH3_Appr::Err = 0;
bool CNH3_Appr::H_v_sat_IG = true;

CNH3 NH3;
/////////////////////////////////////////////////////////////////////////////
// Апроксимации
struct CPzApp
	{
	double Tmax, Pmax;
	double CT[4];
	};

struct CHlApp
	{
	double Tmax, Pmax;
	double CT[8];
	};
struct CRApp
	{
	double Tmax, Pmax;
	double CT[4];
	};

struct CRolApp
	{
	double Tmax, Pmax;
	double CT[6];
	};

/////////////////////////////////////////////////////////////////////////////
static CPzApp PzApp;
//
static CHlApp HlApp;
//
static CRApp RApp;
//
static CRolApp RolApp;


bool CNH3_Appr::ReadData()
	{
	char Path[256];
#define READ(DATA,Type) \
		{\
		lstrcpy ( Path, PROJECT_ROOT );\
    lstrcat ( Path, "Data\\Appr\\");\
		lstrcat ( Path, #DATA".dat" );\
		FILE * F = NULL;\
		if ( fopen_s ( &F, Path, "rb" ) != 0 )\
			return false;\
		fread ( &Type, sizeof ( Type ), 1, F );\
		fclose ( F );\
		}
////////////////////////////////////////////
  READ(NH3_Pz,PzApp)
  READ(NH3_R,RApp)
  READ(NH3_Hl,HlApp)
  READ(NH3_Rol,RolApp)
	return true;
	}

double CNH3_Appr::P_sat ( double TC )
	{
	static double C = 35.7;
	if ( TC > PzApp.Tmax )
		return 10000.; // За критической точкой
	if ( TC < -50. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -50.;
		}
	double T = TC + TK - C;
	double iT = 1. / T;
	double iT2 = iT * iT;
	double Log_P = (( PzApp.CT[3] * iT + PzApp.CT[2] ) * iT + PzApp.CT[1] ) * iT + PzApp.CT[0];
	return exp ( Log_P );
	}

double CNH3_Appr::H_v_sat ( double P, double TC )
	{
	if ( H_v_sat_IG )
		return pGas[G_NH3] -> H_IG ( TC );
	double Tmax = HlApp.Tmax;
	if ( TC > Tmax )
		return H_v ( P, TC, NULL ); // За критической точкой
	double Hl = H_l_sat ( P, TC );
	double R = R_sat ( TC );
	double H = Hl + R;
	return H;
	}

double CNH3_Appr::R_sat ( double TC )
	{
	double Tmax = RApp.Tmax;
	if ( TC > Tmax )
		return 0.; // За критической точкой
	if ( TC < -50. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -50.;
		}
	double T = TC + TK;
	double T1 = log ( 1. - T / Tc_NH3 );
	double * CT = RApp.CT;
	double log_Rx = 
		CT[0] + CT[1] * T1 + ( CT[3] * T + CT[2] ) * T;
//
	return exp ( log_Rx );
	}

double CNH3_Appr::H_v ( double P, double TC, double * pCp )
	{
	double H = pGas[G_NH3] -> H_IG ( TC );
	if ( pCp )
		*pCp = pGas[G_NH3] -> C_IG ( TC );
	return H;
	//double RT = RP * ( TC + TK );
	//double Ro_gas_0 = P / RT;
	//double Ro_gas = NH3.Ro_gas_mol( P, TC, Ro_gas_0 );
	//if ( Ro_gas <= 0. )
 //   {
 //   NH3.Calc ( 1e-6, TC );
	//  double H = NH3.h;
 //   return H;
 //   }
/*
	int kIter = 0;
	while( Ro_gas == 0. && kIter++ < 5 )
		{
		Ro_gas_0 *= 0.9;
		Ro_gas = NH3.Ro_gas_mol( P, TC, Ro_gas_0 );
		}
	if ( Ro_gas <= 0. )
    return 
*/
	//if ( pCp )
	//	*pCp = NH3.Cp;
	//double H = NH3.h;
	//return H;
	}

double CNH3_Appr::H_l_sat ( double P, double TC )
	{
	double Tmax = HlApp.Tmax;
	if ( TC > Tmax )
		{
		return H_v ( P, TC, NULL ); // За критической точкой
		}
	if ( TC < -50. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -50.;
		}
	double T = TC + TK;
	double T1 = 1. - T / Tc_NH3;
	double * CT = HlApp.CT;
	double t = T * 1e-2;
	double Hl = 
		((((( CT[6] * t + CT[5] ) * t + CT[4] ) * t + CT[3] ) * t + CT[2] ) * t + CT[1] ) * t + CT[0] +
		CT[7] * pow ( T1, 0.3984567435774519 );
//
	//if ( CalcChem_H )
	//	Hl += CNH3::Hp_F_chem;
	//else
	//	Hl += CNH3::Hp_F_0;
	return Hl;
	}

double CNH3_Appr::Rol_l_sat ( double TC )
	{
	double Tmax = RolApp.Tmax;
	if ( TC > Tmax )
		return 0.; // За критической точкой
	if ( TC < -50. )
		{
//		ON_ERR(ERR_T_MIN)
		TC = -50.;
		}
	double T = TC + TK;
	double T1 = log ( 1. - T / Tc_NH3 );
	double * CT = RolApp.CT;
	double log_Rx = 
		CT[0] + 
		( CT[2] * T1 + CT[1] ) * T1 + 
		(( CT[5] * T + CT[4] ) * T + CT[3] ) * T;
//
	return exp ( log_Rx );
	}
#endif