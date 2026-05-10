#pragma once

#include "Chem.h"
#include "BaseInt.h"
#include "CommProc.h"
#include "Extensions.h"

#undef IN_DLL
#ifdef DLL_EpsVol
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif

struct IN_DLL CSBase_W
	{
  double P, T;
  double Gamma[K_GAS];
	double dH_liq;
  };

struct IN_DLL CSBase_Data
	{
	double X_W[K_GAS];	// жидкость
	double Y_W[K_GAS];	// газ
	double Z_W[K_GAS];	// Z = eps * X + (1-eps)*X
	double h_Liq[K_GAS];
	double h_Gas[K_GAS];
	double P_sat[K_GAS];
	bool IsComp[K_GAS];
  };

#undef new
/////////////////////////////////////////////////////////////
class IN_DLL CSBase : public CSBase_W, public CSBase_Data
	{
	public:
  static int ID_NH3;
	DWORD TypeMix;
	CComp * pComp;
	double Mw_gas, Mw_liq, Mw_sol, Mw_summ;
	double T_old;
	double Step_T;
	int k_Iter;
	bool O_K;
	double Henry[K_GAS];
	double HenryPrim;
	double k_Gamma;
	bool GAS;
	bool LIQ;
	bool H2S_MEA;
  int OW_Z;
  int OW;
	double * pExtGamma;
  IGamma * pGamma;
  int kIter;
  bool MixH2ONH3;
  DWORD UseComp;
  bool WorkComp[K_GAS];
  bool NoInLiq[K_GAS];
  double SummUse;
  bool Use_H_liq;
  bool BP;
//
	CSBase();
	void Pz( double T );
	void H_liq ( );
  void H_liq ( double Ts );
	void H_gas ( );
	void SetComp ( CComp * pComp );
	void SetComp ( bool WorkComp[], double Cmol[] );
	void SetGamma();
	void Calc_Mw();
	void SetCompData ( bool * _IsComp, double * Cmol_X, double * Cmol_Y,
						         double * h_Liq, double * h_Gas );
	double S_liq ( int N, double P, double T );
	double S_gas ( int N, double P, double T );
  NEW
	};
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
struct IN_DLL CSEps_P
	{
	double T_min;
	double T_max;
//	double Step_T;
	double P_max;
	CSEps_P();
	};

struct IN_DLL CSEps_W
	{
//
	double E, h_gas, h_liq, h;
	CSEps_W();
	};

class IN_DLL CSEps	: public CSBase, public CSEps_W, public CSEps_P, public IBaseModel
	{
	double Calc_S( CSEps & Src );
	void Calc_S( double & s );
	void S( double & s );
	public:
  double h_Liquid, h_Solid; //h с учетом долей жидкой и твердой частей
	double Q_W[K_GAS];
	double K_W[K_GAS];
	private:
//
	double CalcEps_PT( double P, double T, double Eps0 );
	double CalcP_ET( double E, double T );
	void Set_XY( );
	void Calc_H( );
	double Summ( double Eps );
	double Summ( double P, double T, double Eps );
	void SdS( double Eps, double  & S, double &dSdE );
	void H( );
	void HE( );
	public:
  NEW
	CSEps( char * Name, IBaseModel * pMain );
	CSEps( char * Name );
	void PT( double P, double T, CComp * pComp );
	void PT( double P, double T, bool WorkComp[], double Cmol[] );
  void PE( double P, double E, CComp * pComp, double T0 = 0. );
	void PH( struct CFlow * pFlow, double T0 = 0. );
	void PH( double P, double H, CComp * pComp, double T0 = 0. );
  void PH_ext( double P, double H_before, double KS, double T_ext, CComp * pComp, double &dH, double T0 = 0. );
	void PH_corp( double P, double H, CComp * pComp, double Ksi_corp, 
		            double T0 = 0. );
	void PH_mass( double P, double H, CComp * pComp, double Cp_mass, double Teff_mass, 
		            double T0 = 0. );
	void PH_mix ( double P, double H, double Flow,
		            CComp * pComp, 
		            double Cp_pass, double T0_pass, 
		            double T0 = 0. );
	void PS( CSEps & Src, double P_out );
  void TE( double T, double E, CComp * pComp );
//
  int SaveState ( );
  int RestoreState ( char * StrName );
	int GetParams( char * StrName );
  int SetData( int TypeData, void * pData );
  double Eps_gas();
	};

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
struct IN_DLL CSVol_P
	{
	double T_min;
	double T_max;
//	double Step_T;
	double Vol, Omega_L;
	double P_max;
  bool UseTs;
	CSVol_P();
	};

struct IN_DLL CSVol_W
	{
//
	double h_gas, h_liq;
	double M_comm, V, L, H;
  double M_sol, h_sol;
	CSVol_W();
	};

class IN_DLL CSVol	: public CSBase, public CSVol_W, public CSVol_P, public IBaseModel
	{
	double Kappa_W[K_GAS];
  static double * Tmp_Ts;
//
	double CalcEps_PT( double P, double T );
	double Calc_L( double T, double L0 );
	void Set_XYP( double L, double Ksi );
	void Set_XY_Eps( double Eps );
	void Calc_H( );
	double Summ( double P, double Eps );
	double Summ_L( double Ksi, double L );
	void Summ_L( double Ksi, double L, double & S, double & dSdL );
  double CalcGradT( double dt, double T0 );
	void HH( );
	public:
  double * Ts;
  int kTs;
  double KoefTs;
  double LevelLiq;
  NEW
	CSVol( char * Name, IBaseModel * pMain );
	void PT( double P, double T, CComp * pComp );
	void MH( double M_comm, double H, CComp * pComp, double T0 = 0. );
	void MT( double M_comm, double T, CComp * pComp );
	void MH_Corp( double M_comm, double H, CComp * pComp, double Cp_Korp, double T0 = 0 );
	void MH_Corp_heat( double M_comm, double H, CComp * pComp, double Cp_Korp, 
		                 double k_Heat, double T_Heat,
										 double & Q_Heat,
										 double k_ToAir = 0., 
										 double T0 = 0. );
	void SetP( double P );
//
  int SaveState ( );
  int RestoreState ( char * StrName );
	int GetParams( char * );
  int SetData( int TypeData, void * pData );
  void InitGradT( int kTs, double KoefTs );
  void ChangeGradT( int kTs, double _KoefTs );
	};

// P = Pwater + Poil
class IN_DLL CSVol_2	: public CSBase, public CSVol_W, public CSVol_P, public IBaseModel
	{
	double Kappa_W[K_GAS];
	double Ksi, Lwater, L_oil, Pzwater;
//
	double CalcEps_PT( double P, double T );
	double Calc_L( double T, double L0 );
	void Set_XYP( double L );
	void Set_XY_Eps( double Eps );
	void Calc_H( );
	double Summ( double P, double Eps );
	double Summ_L( double Ksi, double L );
	void Summ_L( double Ksi, double L, double & S, double & dSdL );
	void HH( );
	public:
  NEW
	CSVol_2( char * Name, IBaseModel * pMain );
	void PT( double P, double T, CComp * pComp );
	void MH( double M_comm, double H, CComp * pComp, double T0 = 0. );
	void MT( double M_comm, double T, CComp * pComp );
	void MH_Corp( double M_comm, double H, CComp * pComp, double Cp_Korp, double T0 = 0 );
	void MH_Corp_heat( double M_comm, double H, CComp * pComp, double Cp_Korp, 
		                 double k_Heat, double T_Heat,
										 double & Q_Heat,
										 double k_ToAir = 0., 
										 double T0 = 0. );
	void SetP( double P );
//
  int SaveState ( );
  int RestoreState ( char * StrName );
	int GetParams( char * );
  int SetData( int TypeData, void * pData );
	};

