#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"


struct CMEA_CO2 : public CGammaData
  {
  int G_MEA;
  int G_CO2;
  CMEA_CO2( );
  virtual void Init( );
  bool ApprRead();
  virtual int Save();
  virtual int Restore( int Size );
  virtual void Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq );
  double H_CO2 ( double TC );
  double P_CO2 ( double TC, double c_CO2, double c_MDEA );
  double Gamma_CO2 ( double TC, double c_CO2, double c_MDEA );
  double C_free ( double T, double c_CO2, double c_MDEA );
  NEW
  };

  int CMEA_CO2::Save()
  {
    return 1;
  }
  int CMEA_CO2::Restore( int Size )
  {
    return 1;
  }
  void CMEA_CO2::Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq )
  {
  }

void CMEA_CO2::Init( )
  {
  CGammaData::Init();
  G_MEA = GetGasID( "МЭА" );
  if ( G_MEA < 0 )
    G_MDEA = GetGasID( "MEA" );
  ASS( G_MEA >= 0 )
  G_CO2 = GetGasID( "CARBON DIOXIDE" );
  ASS( G_CO2 >= 0 )
  ASS(ApprRead())
  }

static double ApprMEA[101][101];
struct CApprMEAHead
  {
  double w_MEA;
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
  double Appr ( double T, double c_CO2, double c_MEA );
  };


static double Henry_CO2 = 0.1;
static double lkPa = log ( 98.0655 );
double CMEA_CO2::H_CO2 ( double TC )
  {
  double T = TC + TK;
  double H = Henry_CO2 * 0.001 * exp ( 170.7126 - 8477.771 / T - 21.95743 * log ( T ) + 0.005781 * T - lkPa );
  return H;
  }

static CApprMEAHead Head;

double CMEA_CO2::P_CO2 ( double TC, double c_CO2, double c_MEA )
  {
  double C_gas = Head.Appr ( TC, c_CO2, c_MEA );
  double T = TC + TK;
  double H = 0.001 * exp ( 170.7126 - 8477.771 / T - 21.95743 * log ( T ) + 0.005781 * T - lkPa );
  return H * C_gas;
  }

double CMEA_CO2::Gamma_CO2 ( double TC, double c_CO2, double c_MEA )
  {
  double C_gas = Head.Appr ( TC, c_CO2, c_MEA );
  return C_gas / c_CO2;
  }

double CMEA_CO2::C_free ( double T, double c_CO2, double c_MEA )
  {
  return Head.Appr ( T, c_CO2, c_MEA );
  }

double CApprMEAHead::Appr ( double T, double c_CO2, double c_MEA )
  {
  double C = c_CO2 / c_MEA;
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
  double f00 = ApprMEA[nT][nC];
  double f01 = ApprMEA[nT][nC+1];
  double f10 = ApprMEA[nT+1][nC];
  double f11 = ApprMEA[nT+1][nC+1];
  double App = f00 * ( 1. - d_C ) * ( 1. - d_T ) +
    f01 * d_C * ( 1. - d_T ) + 
    f10 * ( 1. - d_C ) * d_T + 
    f11 * d_C * d_T;
  double Cads = exp ( App ) * c_CO2;
  return Cads;
  }

bool CMEA_CO2::ApprRead()
  {
  Char<512>Path;
  Path.Prt ( "%sDATA\\APPR\\MEA.dat", SYSTEM_PATH );
  FILE * F = fopen ( Path, "rb" );
  if ( F == NULL )
    return false;
  fread ( &Head, sizeof ( Head ), 1, F );
  fread ( ApprMEA, 8, 101*101, F );
  fclose ( F );
  return true;
  }

