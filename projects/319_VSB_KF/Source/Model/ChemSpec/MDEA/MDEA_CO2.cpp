#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"


struct CMDEA_CO2 : public CGammaData
  {
  int G_MDEA;
  int G_CO2;
  CMDEA_CO2( );
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

static double ApprMDEA[101][101];
struct CApprMDEAHead
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
  double Appr ( double T, double c_CO2, double c_MEA );
  };
static CApprMDEAHead Head;

  int CMDEA_CO2::Save()
  {
    return 1;
  }
  int CMDEA_CO2::Restore( int Size )
  {
    return 1;
  }

bool CMDEA_CO2::ApprRead()
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

void CMDEA_CO2::Init( )
  {
  CGammaData::Init();
  G_MDEA = GetGasID( "ДЭА" );
  if ( G_MDEA < 0 )
    G_MDEA = GetGasID( "DEA" );
  ASS( G_MDEA >= 0 )
    G_CO2 = GetGasID( "CARBON DIOXIDE" );
  ASS( G_CO2 >= 0 )
    ASS(ApprRead())
  }

static double Henry_CO2 = 0.1;
static double lkPa = log ( 98.0655 );
double CMDEA_CO2::H_CO2 ( double TC )
  {
  double T = TC + TK;
  double H = Henry_CO2 * 0.001 * exp ( 170.7126 - 8477.771 / T - 21.95743 * log ( T ) + 0.005781 * T - lkPa );
  return H;
  }

void CMDEA_CO2::Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq )
  {
  if ( G_MDEA < 0 || G_CO2 < 0 )
    return;
  double C_MDEA = 0.;
  if ( IsComp[G_MDEA] )
    C_MDEA = Cmol[G_MDEA];
  double C_CO2 = 0.;
  if ( IsComp[G_CO2] )
    C_CO2 = Cmol[G_CO2];
  double G = 1.;
  if ( C_MDEA > 1e-6 )
    G = Gamma_CO2 ( T, C_CO2, C_MDEA );
  Gamma[G_CO2] += 0.5 * ( G - Gamma[G_CO2] );
  }


double CApprMDEAHead::Appr ( double T, double c_CO2, double c_MDEA )
  {
  double C = c_CO2 / c_MDEA;
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
  double Cads = exp ( App ) * c_CO2;
  return Cads;
  }

double CMDEA_CO2::P_CO2 ( double TC, double c_CO2, double c_MDEA )
  {
  double C_gas;
  if ( c_MDEA < 1e-6 )
    C_gas = c_CO2;
  else
    C_gas = Head.Appr ( TC, c_CO2, c_MDEA );
  double T = TC + TK;
  double H = 0.001 * exp ( 170.7126 - 8477.771 / T - 21.95743 * log ( T ) + 0.005781 * T - lkPa );
  return H * C_gas;
  }

double CMDEA_CO2::Gamma_CO2 ( double TC, double c_CO2, double c_MDEA )
  {
  double C_gas = Head.Appr ( TC, c_CO2, c_MDEA );
  return C_gas / c_CO2;
  }

double CMDEA_CO2::C_free ( double T, double c_CO2, double c_MDEA )
  {
  return Head.Appr ( T, c_CO2, c_MDEA );
  }

/////////////////////////////////////////////////////////////////
struct DEA_CO2 : public CReal
  {
  DEA_CO2();
  void Init();
  };

DEA_CO2::DEA_CO2()
  {
  Mw = 105.14 + 44.01;
  }

void DEA_CO2::Init()
  {
  Mw = 105.14 + 44.01;
  }

CReal * Create_DEA_CO2()
  {
  return new DEA_CO2();
  }

