#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"


struct CCOKE : public CGas 
  {
  CCOKE();
  double dHobr( double Tc );
  double dSobr( double Tc );
  double dGobr( double Tc );
  void Init();
  double  dH_1_dTC (double TC);
  double H_Chem ( double TC );
  double dH_Chem ( );
  double G_IG( double P, double TC );
  double H_vap ( double TC );
  double C_IG ( double TC );
  double H_IG ( double TC );
  double S_IG( double P, double TC );
  void S_IG( double P, double TC, double &S, double & dSdT );
  double H_v ( double TC );
  double H_l ( double TC );
  double C_Liq ( double TC );
  double Pz ( double T_C );
  double dPz_dT ( double T_C );
  bool Ro_liq ( double T_C, double & Ro );
  bool To_m3_liq ( double TC, double & To_m3 );
  int ReadData( _HANDLE hFile );
  void GetParams ( IBaseModel * pMain ){;};
  NEW
  };
  
  int CCOKE::ReadData( _HANDLE hFile )
  {
    return 0;
  }

CGas * Create_C()
  {
  return new CCOKE;
  }

const double C_Coke = 10.;

CCOKE::CCOKE()
  {
  Mw = 12.;//
  lstrcpy( Name, "COKE" );
//  pGas[G_COKE] = this;
//  Type = PhazeAsWater;
  }

double CCOKE::C_IG ( double TC )
  {
  return C_Coke;
  }

double CCOKE::dH_1_dTC (double TC)
  {
  return C_Coke;
  }

double CCOKE::H_IG ( double TC )
  {
  return C_Coke * ( TC - 25.);
  }

double CCOKE::S_IG( double P, double TC )
  {
  return 0.;
  }

void CCOKE::S_IG( double P, double TC, double &S, double & dSdT )
  {
  ASS(0)
  }

double CCOKE::H_v ( double TC )
  {
  return C_Coke * ( TC - 25.);
  }

double CCOKE::H_l ( double TC )
  {
  return C_Coke * ( TC - 25.);
  }

double CCOKE::C_Liq ( double TC )
  {
  return C_Coke;
  }

double CCOKE::Pz ( double T_C )
  {
  return 0.0001;
  }

double CCOKE::dPz_dT ( double T_C )
  {
  return 0.;
  }

bool CCOKE::Ro_liq ( double T_C, double & Ro )
  {
  Ro = 1000.;
  return true;
  }
bool CCOKE::To_m3_liq ( double TC, double & To_m3 )
  {
  To_m3 = 0.012; // Mw == 12; Ro = 1000
  return true;
  }

double CCOKE::dHobr( double TC )
  {
  return C_Coke * ( TC - 25.);
  }
  
double CCOKE::dSobr( double Tc )
  {
  return 0.;
  }

double CCOKE::dGobr( double Tc )
  {
  return 0.;
  }
void CCOKE::Init()
  {
  return;
  }
double CCOKE::H_Chem ( double TC )
  {
  return C_Coke * ( TC - 25.);
  }

double CCOKE::dH_Chem ( )
  {
  return 0.;
  }

double CCOKE::G_IG( double P, double TC )
  {
  return 0.;
  }

double CCOKE::H_vap ( double TC )
  {
  return 0.;
  }

