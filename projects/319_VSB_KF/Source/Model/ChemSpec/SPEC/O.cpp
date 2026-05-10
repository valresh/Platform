#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"

#if 0
const double C_RadicalO = 10.;

struct CRadicalO : public CGas 
  {
  CRadicalO();
  double dHobr( double Tc );
  double dSobr( double Tc );
  double dGobr( double Tc );
  void Init();
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
  double C_liq ( double TC );
  double Pz ( double T_C );
  double dPz_dT ( double T_C );
  bool Ro_liq ( double T_C, double & Ro );
  bool To_m3_liq ( double TC, double & To_m3 );
  };


CRadicalO RadicalO;
CGas * pRadicalO;
int G_RadicalO;

CRadicalO::CRadicalO()
  {
  Mw = 8.;//
  Alt_Name = Name = "Радикал O";
  pRadicalO = this;
  }

double CRadicalO::C_IG ( double TC )
  {
  return C_RadicalO;
  }

double CRadicalO::H_IG ( double TC )
  {
  return C_RadicalO * ( TC - 25.);
  }

double CRadicalO::S_IG( double P, double TC )
  {
  return 0.;
  }

void CRadicalO::S_IG( double P, double TC, double &S, double & dSdT )
  {
  ASS(0)
  }

double CRadicalO::H_v ( double TC )
  {
  return C_RadicalO * ( TC - 25.);
  }

double CRadicalO::H_l ( double TC )
  {
  return C_RadicalO * ( TC - 25.);
  }

double CRadicalO::C_liq ( double TC )
  {
  return C_RadicalO;
  }

double CRadicalO::Pz ( double T_C )
  {
  return 0.0001;
  }

double CRadicalO::dPz_dT ( double T_C )
  {
  return 0.;
  }

bool CRadicalO::Ro_liq ( double T_C, double & Ro )
  {
  Ro = 1000.;
  return true;
  }

bool CRadicalO::To_m3_liq ( double TC, double & To_m3 )
  {
  To_m3 = 0.012; // Mw == 12; Ro = 1000
  return true;
  }

double CRadicalO::dHobr( double TC )
  {
  return C_RadicalO * ( TC - 25.);
  }

double CRadicalO::dSobr( double Tc )
  {
  return 0.;
  }

double CRadicalO::dGobr( double Tc )
  {
  return 0.;
  }
void CRadicalO::Init()
  {
  return;
  }
double CRadicalO::H_Chem ( double TC )
  {
  return C_RadicalO * ( TC - 25.);
  }

double CRadicalO::dH_Chem ( )
  {
  return 0.;
  }

double CRadicalO::G_IG( double P, double TC )
  {
  return 0.;
  }

double CRadicalO::H_vap ( double TC )
  {
  return 0.;
  }

#endif
