#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"



struct CSALT : public CGas 
  {
  CSALT();
  double dHobr( double Tc );
  double dSobr( double Tc );
  double dGobr( double Tc );
  void Init();          //+
  double H_Chem ( double TC );
  double dH_Chem ( );
  double G_IG( double P, double TC );  //+
  double H_vap ( double TC );
  double C_IG ( double TC );
  double H_IG ( double TC );
  double S_IG( double P, double TC );
  void S_IG( double P, double TC, double &S, double & dSdT );
  double H_v ( double TC );
  double H_l ( double TC );
  double  dH_1_dTC (double TC);
  double C_Liq ( double TC ); //+
  double Pz ( double T_C );
  double dPz_dT ( double T_C );
  bool Ro_liq ( double T_C, double & Ro );
  bool To_m3_liq ( double TC, double & To_m3 );
  void GetParams ( IBaseModel * pMain );
  int ReadData( HANDLE hFile );
  };


CSALT SALT;
CGas * pSALT;

const double Cp_Salt = 1. * 87.08; // 1кДж/кг 
const double Ro_Salt = 1930.;

CGas * Create_Salt()
  {
  return &SALT;
  }

CSALT::CSALT()
  {
  //KNO3		101	53	53.53
  //NaNO2		69	40	27.6
  //NaNO3		85	7	  5.95
  //                87.08
  lstrcpy ( Name, "Соль" );
  pSALT = this;
  Tcr = 1000.;
  //  pGas[G_SALT] = this;
  //  Type = PhazeAsWater;
  }

double CSALT::C_IG ( double TC )
  {
  return Cp_Salt;
  }

double CSALT::H_IG ( double TC )
  {
  return Cp_Salt * ( TC - 25.);
  }

double CSALT::S_IG( double P, double TC )
  {
  return 0.;
  }

void CSALT::S_IG( double P, double TC, double &S, double & dSdT )
  {
  ASS(0)
  }

double CSALT::H_v ( double TC )
  {
  return Cp_Salt * ( TC - 25.);
  }

double CSALT::H_l ( double TC )
  {
  return Cp_Salt * ( TC - 25.);
  }


double  CSALT::dH_1_dTC (double TC)
  {
  return Cp_Salt;
  }


double CSALT::C_Liq ( double TC )
  {
  return Cp_Salt;
  }

double CSALT::Pz ( double T_C )
  {
  return 0.0001;
  }

double CSALT::dPz_dT ( double T_C )
  {
  return 0.;
  }

bool CSALT::Ro_liq ( double T_C, double & Ro )
  {
  Ro = Ro_Salt;
  return true;
  }
bool CSALT::To_m3_liq ( double TC, double & To_m3 )
  {
  To_m3 = Mw / Ro_Salt;
  return true;
  }

double CSALT::dHobr( double TC )
  {
  ASS(0)
  return 0.;
  }

double CSALT::dSobr( double Tc )
  {
  ASS(0)
  return 0.;
  }

double CSALT::dGobr( double Tc )
  {
  ASS(0)
  return 0.;
  }

void CSALT::Init()
  {
  return;
  }

double CSALT::H_Chem ( double TC )
  {
  ASS(0)
  return Cp_Salt * ( TC - 25.);
  }

double CSALT::dH_Chem ( )
  {
  ASS(0)
  return 0.;
  }

double CSALT::G_IG( double P, double TC )
  {
  ASS(0)
  return 0.;
  }

double CSALT::H_vap ( double TC )
  {
  return 0.;
  }

void CSALT::GetParams ( IBaseModel * pMain )
  {
  ASS(0)
  }

int CSALT::ReadData( HANDLE hFile )
  {
  // Ключ уже прочитан
  DWORD Lr;
  CRealDBData DBData;
  ReadFile ( hFile, &DBData, sizeof ( DBData ), &Lr, NULL );
  lstrcpy ( Name, "Соль" );
  Mw = 87.08;
  Tcr = 1000.;
  return 0;
  }
