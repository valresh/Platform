#pragma once


#define ERR_T_MAX 0x01
#define ERR_T_MIN 0x02
#define ERR_P_MAX 0x04
#define ERR_P_MIN 0x08
#define ERR_H_MAX 0x10
#define ERR_H_MIN 0x20
#define ERR_X_MAX 0x40

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef CHEM_SPEC_EXPORTS
#define CHEM_SPEC_API Q_DECL_EXPORT
#else
#define CHEM_SPEC_API Q_DECL_IMPORT
#endif
#else
#ifdef CHEM_SPEC_EXPORTS
#define CHEM_SPEC_API __declspec(dllexport)
#else
#define CHEM_SPEC_API __declspec(dllimport)
#endif
#endif

class CHEM_SPEC_API CWater
  {
  public:
  double dH0_chem, dH0_0, _H;
  int Err;
  CWater();
  double Cpv ( double Tc, double P );
  double Cpl ( double Tc, double P );
  double Hv ( double Tc, double P );
  double Hl ( double Tc, double P );
  double Hv_97 ( double Tc, double P );
  double Hl_97 ( double Tc, double P );
  double Cpv_97 ( double Tc, double P );
  double Cpl_97 ( double Tc, double P );
  double Pz_97 ( double Tc );
  double Tz_97 ( double P_atm );
  double Pz ( double Tc );
  double Tz ( double P_atm );
  double Tv ( double Hv, double P, double T0 );
  double Tl ( double Hl, double P, double T0 );
  double Tv_97 ( double Hv, double P, double T0 );
  double Tl_97 ( double Hl, double P, double T0 );
  double Sv ( double Tc, double P );
  double Sl ( double Tc, double P );
  double Rov ( double Tc, double P );
  double Rol ( double Tc, double P );
  void SetH();
  bool Psat_Appr ( double T_C, double &P, double &dPdT );
  double Pmin( );
  void iPsat_Appr ( double P, double &T_C, double &dTdP );
  bool Vap_Appr ( double T_C, double &Hv, double &dHvdT );
  bool Liq_Appr ( double T_C, double &Hl, double &dHldT );
  };


class CHEM_SPEC_API CWater_appr
	{
	public:
	DWORD * pErr;
//
	static bool ReadData();
	double P_z ( double TC );
	double T_z ( double Patm );
	double H_v_P ( double TC, double P );
	double H_l_P ( double TC, double P );
	double Cp_v_P ( double TC, double P );
	double Cp_l_P ( double TC, double P );
	};

extern CHEM_SPEC_API CWater Water;
extern CHEM_SPEC_API CWater_appr Water_appr;

