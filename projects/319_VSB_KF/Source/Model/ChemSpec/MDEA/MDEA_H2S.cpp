#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"

struct CMDEA_H2S : public CGammaData
  {
  int G_MDEA;
  int G_H2S;
  CMDEA_H2S( );
  virtual void Init( );
  bool ApprRead();
  virtual int Save();
  virtual int Restore( int Size );
  virtual void Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq );
  double H_H2S ( double TC );
  double P_H2S ( double TC, double c_CO2, double c_MDEA );
  double Gamma_H2S ( double TC, double c_CO2, double c_MDEA );
  double C_free ( double T, double c_CO2, double c_MDEA );
  NEW
  };


int CMDEA_H2S::Save()
  {
  return 1;
  }
int CMDEA_H2S::Restore( int Size )
  {
  return 1;
  }
void CMDEA_H2S::Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq )
  {
  }

void CMDEA_H2S::Init( )
  {
  CGammaData::Init();
  G_MDEA = GetGasID( "ДЭА" );
  if ( G_MDEA < 0 )
    G_MDEA = GetGasID( "DEA" );
  ASS( G_MDEA >= 0 )
  G_H2S = GetGasID( "HYDROGEN SULFIDE" );
  ASS( G_H2S >= 0 )
  ASS(ApprRead())
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

double CMDEA_H2S::P_H2S ( double TC, double c_H2S, double c_MDEA )
	{
  double C = c_MDEA * ( 1. - c_MDEA );
  double X = c_H2S;
	double T = TC + TK;
	double H = Henry_H2S * exp ( 342.595 - 13236.8 / T - 55.0551 * log ( T ) + 0.0595651 * T );
  return X * H;
	}

// Размерность на выходе любая
double CMDEA_H2S::Gamma_H2S ( double TC, double c_H2S, double c_MDEA )
	{
	double C_gas = Head.Appr ( TC, c_H2S, c_MDEA );
	return C_gas / c_H2S;
	}

// Размерность на выходе = Размерность на входе
double CMDEA_H2S::C_free ( double T, double c_H2S, double c_MDEA )
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

bool CMDEA_H2S::ApprRead()
	{
	Char<512>Path;
  Path.Prt ( "%sDATA\\APPR\\MDEA.dat", SYSTEM_PATH );
	FILE * F;
	if ( fopen_s ( &F, Path, "rb" ) != 0 )
		return false;
	fread ( &Head, sizeof ( Head ), 1, F );
	fread ( ApprMDEA, 8, 101*101, F );
	fclose ( F );
	return true;
	}

/////////////////////////////////////////////////////////////////
struct DEA_H2S : public CReal
  {
  DEA_H2S();
  void Init();
  };

DEA_H2S::DEA_H2S()
  {
  Mw = 105.14 + 34.082;
  }

void DEA_H2S::Init()
  {
  Mw = 105.14 + 34.082;
  }

CReal * Create_DEA_H2S()
  {
  return new DEA_H2S();
  }
