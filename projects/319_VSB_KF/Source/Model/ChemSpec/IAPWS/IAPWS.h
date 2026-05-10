#pragma once

struct CIAPWSApprData
	{
	int I, J;
	double n;
	};

class CIAPWSAppr
  {
  int Imin;
  int Imax;
  int Jmin;
  int Jmax;
  int K;
  int I[100];
  int J[100];
  double dI[100];
  double dJ[100];
  double N[100];
//
  double _x, _y;
  double vIm[100];
  double vIp[100];
  double vJp[100];
  double vJm[100];
  void SetV ( double x, double y );
  public:
  CIAPWSAppr( int kData, CIAPWSApprData ApprData[] );
  double Appr ( double x, double y );
  double Appr_x ( double x, double y );
  double Appr_xx ( double x, double y );
  double Appr_y ( double x, double y );
  double Appr_yy ( double x, double y );
  double Appr_xy ( double x, double y );
  };

#define Rkg 0.461526 // дж / кг
//#define Pascal 98065.5
//#define MPa_atm 10.197266112955116733203828053699
//#define iMPa_atm 0.0980655
#define TP 647.096 // Критическая точка воды (К)
//#define TK 273.15

//Tc = 647.096 K 
//Pc = 22.064 MPa 
//Roc = 322 kg m-3
//h - kJ / kg
//u - kJ / kg
//s - kJ / kg K
//Cp - kJ /  kg K
class CIF_97
	{
	double Pi, Tau, RT;
	double x, y;
  int LastError;
  public:
// 1
  void SetXY_1( double T_K, double P_MPa );
  double G_1 ( double T_K, double P_MPa );
  double G_1p ( double T_K, double P_MPa );
  double G_1t ( double T_K, double P_MPa );
  double G_1pp ( double T_K, double P_MPa );
  double G_1tt ( double T_K, double P_MPa );
  double G_1pt ( double T_K, double P_MPa );
  double T_1h ( double P_MPa, double H_kJ_kg );
  double T_1s ( double P_MPa, double S_kJ_kg );
	double Ro_1 ( double T_K, double P_MPa );
	double U_1 ( double T_K, double P_MPa );
	double S_1 ( double T_K, double P_MPa );
	double H_1 ( double T_K, double P_MPa );
	double Cp_1 ( double T_K, double P_MPa );
	double Cv_1 ( double T_K, double P_MPa );
// 2
  void SetXY_2( double T_K, double P_MPa );
  double G_2 ( double T_K, double P_MPa );
  double G_2p ( double T_K, double P_MPa );
  double G_2t ( double T_K, double P_MPa );
  double G_2pp ( double T_K, double P_MPa );
  double G_2tt ( double T_K, double P_MPa );
  double G_2pt ( double T_K, double P_MPa );
  double T_2h ( double P_MPa, double H_kJ_kg );
  double T_2ah ( double P_MPa, double H_kJ_kg );
  double T_2bh ( double P_MPa, double H_kJ_kg );
  double T_2ch ( double P_MPa, double H_kJ_kg );
	double Ro_2 ( double T_K, double P_MPa );
	double U_2 ( double T_K, double P_MPa );
	double S_2 ( double T_K, double P_MPa );
	double H_2 ( double T_K, double P_MPa );
	double Cp_2 ( double T_K, double P_MPa );
	double Cv_2 ( double T_K, double P_MPa );
  double B2C ( double H_kJ_kg );
// 3
  void SetXY_3( double T_K, double Ro_kg_m3 );
  double G_3 ( double T_K, double Ro_kg_m3 );
  double G_3p ( double T_K, double Ro_kg_m3 );
  double G_3t ( double T_K, double Ro_kg_m3 );
  double G_3pp ( double T_K, double Ro_kg_m3 );
  double G_3tt ( double T_K, double Ro_kg_m3 );
  double G_3pt ( double T_K, double Ro_kg_m3 );
	double U_3 ( double T_K, double Ro_kg_m3 );
	double S_3 ( double T_K, double Ro_kg_m3 );
	double H_3 ( double T_K, double Ro_kg_m3 );
	double Cp_3 ( double T_K, double Ro_kg_m3 );
	double Cv_3 ( double T_K, double Ro_kg_m3 );
//
  public:
  double Ro_3 ( double T_K, double P_MPa ); 
  double Ro_3_liq ( double T_K, double P_MPa ); 
  double Ro_3_gas ( double T_K, double P_MPa ); 
// 4
  double P_4 ( double T_K );
  void P_4 ( double T_K, double & P, double & dPdT );
  double T_4 ( double P_MPa );
// 5
  void SetXY_5( double T_K, double P_MPa );
  double G_5 ( double T_K, double P_MPa );
  double G_5p ( double T_K, double P_MPa );
  double G_5t ( double T_K, double P_MPa );
  double G_5pp ( double T_K, double P_MPa );
  double G_5tt ( double T_K, double P_MPa );
  double G_5pt ( double T_K, double P_MPa );
	double Ro_5 ( double T_K, double P_MPa );
	double U_5 ( double T_K, double P_MPa );
	double S_5 ( double T_K, double P_MPa );
	double H_5 ( double T_K, double P_MPa );
	double Cp_5 ( double T_K, double P_MPa );
	double Cv_5 ( double T_K, double P_MPa );
//
  int Region ( double T_K, double P_MPa );
	public:
/////////////////////////////////////////////////
  CIF_97();
  virtual void Error( int N );
  int GetLastError( ) { return LastError; };
	double P_3 ( double T_K, double Ro_kg_m3 );
///////////////////////////////////////////////
	double P_23_MPa( double T_K );
	double T_23_Kp ( double P_MPa );
	double T_23_Km ( double P_MPa );
	public:
// Однофазные состояния
	double Ro ( double T_K, double P_MPa );
	double U  ( double T_K, double P_MPa );
	double S  ( double T_K, double P_MPa );
	double H  ( double T_K, double P_MPa );
	double Cp ( double T_K, double P_MPa );
	double Cv ( double T_K, double P_MPa );
  //
	double H_par ( double T_K, double P_MPa );
	double Ro_par( double T_K, double P_MPa );
	double Cv_par( double T_K, double P_MPa );
	double Cp_par( double T_K, double P_MPa );
  //
	double Ro_mol ( double T_C, double P_atm );
	double U_mol  ( double T_C, double P_atm );
	double S_mol  ( double T_C, double P_atm );
	double H_mol  ( double T_C, double P_atm );
	double Cp_mol ( double T_C, double P_atm );
	double Cv_mol ( double T_C, double P_atm );
// 2-х фазные состояния
	double Rop( double Tc )// Вода в равновесии с паром!!!
		{
		return Ro_liq ( Tc + 273.15 );
		};
	double Ro_liq ( double T_K );
	double U_liq ( double T_K );
	double S_liq ( double T_K );
	double H_liq ( double T_K );
	double Cp_liq ( double T_K );
	double Cv_liq ( double T_K );
	double Ro_gas ( double T_K );
	double U_gas ( double T_K );
	double S_gas ( double T_K );
	double H_gas ( double T_K );
	double Cp_gas ( double T_K );
	double Cv_gas ( double T_K );
//
	double Pz ( double T_K );
  // Сложные состояния
	double XH ( double T_K, double P_MPa, double dPar );
	double XRo( double T_K, double P_MPa, double dPar );
	double XCv( double T_K, double P_MPa, double dPar );
	double XCp( double T_K, double P_MPa, double dPar );
	};

