#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Err.h"

struct IN_DLL CGas_MDEA : public CGas 
  {
  static double H2S_A;
  static double H2S_B;
  static double H2S_T1;
  static double H2S_K1; 
  static double H2S_T2;
  static double H2S_K2;
  CGas_MDEA();
  void Init();
  static bool ApprRead();
  static double X_H2S ( double T, double c_H2S, double c_MEA );
  static double P_H2S ( double T, double c_H2S, double c_MEA );
  static double Gamma_H2S ( double T, double c_H2S, double c_MEA );
  double C_free ( double T, double c_H2S, double c_MDEA );
  //====================================================
  double C_IG ( double TC );
  double H_IG ( double TC );
  double S_IG( double P, double TC );
  void S_IG( double P, double TC, double &S, double & dSdT );
  double H_v ( double TC );
  double H_l ( double TC );
  double C_liq ( double TC );
  double H_vap ( double T_K );
  double dGobr( double TC );
  double dHobr( double TC );
  double Pz ( double T_C );
  bool Ro_liq ( double T_C, double & Ro );
  static void Calc_AB();
  double dSobr( double Tc ){ ASS(0); return 0; };
  double H_Chem ( double TC ){ ASS(0); return 0; };
  double dH_Chem ( ){ ASS(0); return 0; };
  double G_IG( double P, double TC ){ ASS(0); return 0; };
  bool To_m3_liq ( double TC, double & To_m3 ){ ASS(0); return 0; };
  };

CGas_MDEA MDEA;
int G_MDEA;

static double kA = 0.;
static double kB = 0.;  // K = kA * exp ( -kB * TC );
//
static double Tk[4] = { 40., 60., 80., 100., };
static double Kk[4] = { 1628., 155., 10., 1.24, };
//

void SetMDEA( )
  {
  MDEA.Init();
  ASS(CGas_MDEA::ApprRead());
  pGas[G_MDEA] = &MDEA;
  }

// Теплота реакции, кДж/кг
// H2S 1047
// CO2 1340
CGas_MDEA::CGas_MDEA()
  {
	Mw = 119.16;
	Name = "МДЭА";
  ChemFormula = "H13C5O2N";
//
  kB = log( Kk[0] / Kk[3] ) / ( Tk[3] - Tk[0] );
  kA = Kk[0] * exp ( kB * Tk[0] );
  double D = kA * exp ( -kB * Tk[3] ) / Kk[3];
  }

//Cp_v 	-16.817	0.74349	-0.00055635	0.00000021535	-0.000000000034622
const double CpA_v = -16.817;
const double CpB_v = 0.74349;
const double CpC_v = -0.00055635;
const double CpD_v = 0.00000021535;	
const double CpE_v = -0.000000000034622;	

double CGas_MDEA::C_IG ( double TC )
	{
	double T = TC + TK;
	double Cp = ((( CpE_v * T + CpD_v ) * T + CpC_v ) * T + CpB_v ) * T + CpA_v;
	return Cp;
	}

const double HA_v = CpA_v;
const double HB_v = CpB_v / 2.;
const double HC_v = CpC_v / 3.;
const double HD_v = CpD_v / 4.;	
const double HE_v = CpE_v / 5.;	
static double dH_0 = 0.;	

double CGas_MDEA::H_IG ( double TC )
	{
	double T = TC + TK;
	double H = (((( HE_v * T + HD_v ) * T + HC_v ) * T + HB_v ) * T + HA_v ) * T + dH_0;
	return H;
	}

double CGas_MDEA::H_v ( double TC )
	{
	double T = TC + TK;
	double H = (((( HE_v * T + HD_v ) * T + HC_v ) * T + HB_v ) * T + HA_v ) * T + dH_0;
	return H;
	}

double CGas_MDEA::H_vap ( double Tc )
	{
// 124400	678	0.396
	double T = Tc + TK;
	if ( T >= 678. )
		return 0.;
	double Hvap = 124400. * pow ( 1. - T / 678., 0.396 );
	return Hvap;
	}

double CGas_MDEA::S_IG( double P, double TC )
	{
	ASS(FALSE);
	return 0.;
	}

void CGas_MDEA::S_IG( double P, double TC, double & S, double & dSdT )
	{
	ASS(FALSE);
	}

const double D1_MDEA = 26.13691;
const double D2_MDEA = -7588.516;
const double log_Pa = log ( Pascal );

double CGas_MDEA::Pz ( double Tc )
	{
	double T = Tc + TK;
	double logPz_MDEA = D1_MDEA	+ D2_MDEA / T;
	double Pz = exp ( logPz_MDEA - log_Pa );
	return Pz;
	}

//CpA	CpB	CpC	CpD
//Cp_l 
const double CpA_l = 183.04995951;
const double CpB_l = 1.2299804662;
const double CpC_l = -0.0031521256527;
const double CpD_l = 3.5579994789e-06;	
double CGas_MDEA::C_liq ( double TC )
	{
	double T = TC + TK;
	double Cp = (( CpD_l * T + CpC_l ) * T + CpB_l ) * T + CpA_l;
	return Cp;
	}

double CGas_MDEA::H_l ( double TC )
	{
	double T = TC + TK;
	if ( T >= 678. )
		return H_IG ( TC );
	double Hvap = 124400. * pow ( 1. - T / 678., 0.396 );
	return H_IG ( TC ) - Hvap;
	}

void CGas_MDEA::Init()
	{
	dH_0 = 0.;	
	double dH_25 = H_IG ( 25. );
	dH_0 = -dH_25;
	}

double CGas_MDEA::dGobr( double TC )
	{
	ASS(FALSE);
	return 0.;
	}

double CGas_MDEA::dHobr( double TC )
	{
	ASS(FALSE);
	return 0.;
	}

bool CGas_MDEA::Ro_liq ( double T_C, double & Ro )
	{
	Ro = 1015.;
	return true;
	}

////////////////////////////////////////////////////////////
static double ApprMDEA[101][101];
struct CApprHead
	{
	double w_MDEA;
	double C_min;
	double C_max;
	double l_C_min;
	double l_C_max;
	double dl_C;
	double T_min;
	double T_max;
	double dT;
	int kC;
	int kT;
	double Appr ( double T, double c_H2S, double c_MEA );
	};


double Henry_H2S = 1.;
double H_H2S ( double TC )
	{
	double T = TC + TK;
	double H = Henry_H2S * exp ( 342.595 - 13236.8 / T - 55.0551 * log ( T ) + 0.0595651 * T ) * iMw_H2O;
	return H;
	}

static CApprHead Head;

double CGas_MDEA::P_H2S ( double TC, double c_H2S, double c_MDEA )
	{
/***********
	if ( c_MDEA < 1e-6 || c_H2S < 1e-6 )
		return 0.;
	double C_gas = Head.Appr ( TC, c_H2S, c_MDEA );
	double T = TC + TK;
	double H = exp ( 342.595 - 13236.8 / T - 55.0551 * log ( T ) + 0.0595651 * T );
	double Pz = H * C_gas * iMu_H2O;
  return Pz;	//Ф-ла для концентраций mol/kg
****/
  double K = kA * exp ( -kB * TC );
  double C = c_MDEA * ( 1. - c_MDEA );
  double X = c_H2S / ( 1. + C * K );
	double T = TC + TK;
	double H = Henry_H2S * exp ( 342.595 - 13236.8 / T - 55.0551 * log ( T ) + 0.0595651 * T );
  return X * H;
	}

// Размерность на выходе любая
double CGas_MDEA::Gamma_H2S ( double TC, double c_H2S, double c_MDEA )
	{
	double C_gas = Head.Appr ( TC, c_H2S, c_MDEA );
	return C_gas / c_H2S;
	}

// Размерность на выходе = Размерность на входе
double CGas_MDEA::C_free ( double T, double c_H2S, double c_MDEA )
	{
	return Head.Appr ( T, c_H2S, c_MDEA );
	}

// Размерность на выходе = Размерность на входе
double CApprHead::Appr ( double T, double c_H2S, double c_MDEA )
	{
	double C = c_H2S / c_MDEA;
	if ( C < C_min )
		C = C_min;
	if ( C > C_max )
		C = C_max;
	double l_C = log ( C );
	double D_C = ( l_C - l_C_min )/ dl_C;
	int nC = (int)D_C;
	if ( nC >= kC )
		nC = kC - 1;
	if ( T < T_min )
		T = T_min;
	if ( T > T_max )
		T = T_max;
	double D_T = ( T - T_min ) / dT;
	int nT = (int)D_T;
	if ( nT >= kT )
		nT = kT - 1;
	double d_C = D_C - nC; 
	double d_T = D_T - nT;
	double f00 = ApprMDEA[nT][nC];
	double f01 = ApprMDEA[nT][nC+1];
	double f10 = ApprMDEA[nT+1][nC];
	double f11 = ApprMDEA[nT+1][nC+1];
	double App = f00 * ( 1. - d_C ) * ( 1. - d_T ) +
							 f01 * d_C * ( 1. - d_T ) + 
							 f10 * ( 1. - d_C ) * d_T + 
							 f11 * d_C * d_T;
	double Cads = exp ( App ) * c_H2S;
	return Cads;
 	}

bool CGas_MDEA::ApprRead()
	{
	char Path[256];
	lstrcpy ( Path, GetBasePath());
	lstrcat ( Path, "Data\\Appr\\");
	lstrcat ( Path, "MDEA.dat" );
	FILE * F;
	if ( fopen_s ( &F, Path, "rb" ) != 0 )
		return false;
	fread ( &Head, sizeof ( Head ), 1, F );
	fread ( ApprMDEA, 8, 101*101, F );
	fclose ( F );
	return true;
	}
