#pragma once

#include "BaseModel.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#include <QFile>
#else
#undef IN_DLL
#define IN_DLL __declspec(dllimport)
#include "math.h"
#include "BaseInt.h"
#include "Interface/CompInterface.h"
#undef IN_DLL
#ifdef DLL_Chem
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

enum
  {
  K_GAS = 128,
  CMOL_GAS = 1024,
  ISCOMP_GAS = 128,
  K_SOLID = 64,
  CMASS_SOLID = 512,
  ISCOMP_SOLID = 64,
  K_VIRTUAL = 32,
  C_VIRTUAL = 256,
  IS_VIRTUAL = 32,
  };

IN_DLL extern int LAST_GAS;
IN_DLL extern int LAST_SOLID;
IN_DLL extern int LAST_VIRT;
////
IN_DLL extern int G_CH4;
IN_DLL extern int G_C2H6;
IN_DLL extern int G_C3H8;
IN_DLL extern int G_C4H10N;
IN_DLL extern int G_C4H10I;
IN_DLL extern int G_C5H12N;
IN_DLL extern int G_C5H12I;
IN_DLL extern int G_C6H14N;
IN_DLL extern int G_C7H16N;
IN_DLL extern int G_C8H18N;
IN_DLL extern int G_C9H20N;
IN_DLL extern int G_C10H22N;
IN_DLL extern int G_HE;
IN_DLL extern int G_AR;
IN_DLL extern int G_N2;
IN_DLL extern int G_O2;
IN_DLL extern int G_H2O;
IN_DLL extern int G_CH3OH;
IN_DLL extern int G_METHANOL;
IN_DLL extern int G_OIL;
IN_DLL extern int G_CO;
IN_DLL extern int G_CO2;
IN_DLL extern int G_COKE;
IN_DLL extern int G_MDEA;
IN_DLL extern int G_K2CO3;
IN_DLL extern int G_C6N;
IN_DLL extern int G_H2;
IN_DLL extern int G_H2S;
IN_DLL extern int G_NH3;
//
IN_DLL extern int G_СуммаS;
IN_DLL extern int G_Меркаптаны;
IN_DLL extern int G_Сульфиды;
IN_DLL extern int G_Дисульфиды;
IN_DLL extern int G_Тиофены;
IN_DLL extern int G_Олефины;
//
IN_DLL extern int G_ИПБ; 

IN_DLL extern bool CalcChem_H;
enum
{

};
const double RG = 8.314471;	// kJ/kMol K
const double RP = 0.084784873375448; // Для расчета P в атм
const double M3 = 22.4136; // Моль в нм3 Р = 1.0332, T = 273.16
const double iM3 = 0.0446157689973944;	// нм3 в Моль  Р = 1.0332, T = 273.16
const double Pascal = 98066.5;
const double Pstd = 1.0332379888952;
const double iPascal = 1.0197266112955116733203828053699e-5;
const double Tc_H2O = 647.096;	// K
const double Roc_H2O = 322.;	// kg/m3
const double Mw_H2O = 18.015483901442206;	// kg/kMol
const double Mw_CO2 = 44.0096006393433;
const double iMw_H2O = 0.055507806810559571367992514639901;	// kMol/kg
const double Tc_NH3 = 405.40;	// K
const double Roc_NH3 = 225.;	// kg/m3
const double Mw_NH3 = 17.03026;	// kg/kMol
const double iMw_NH3 = 0.058719009574721701254120606496906;		// kMol/kg
const double Ro_NH3 = 682.8;
const double R_NH3 = 1371.2;// kJ/kg 
const double T_boil_NH3 = -33.5;// 1.013 bar
const double Cp_NH3 = 4.48; //кДж/кгK

const double MPa_atm = 10.197266112955116733203828053699;
const double iMPa_atm = 0.0980655;
const double TK = 273.15;

#define MAX_COMPONENT 128

struct CComponentData
  {
  int ID;// ID в [PROP].[dbo].[COMMON]
  char Formula[64];
  char Descr[256];
  char Name[64];
  double Tb;
  };
// 

class IN_DLL CGas
  {
  public:
  double Mw, Tb;
  double Tcr, Pcr, Vcr, Rocr, Zcr, Omega;     // K, kPa, m3/kmol,
  char Name[128];    //Химическое название
  char Alt_Name[128];
  char Formula[16];
  bool   AsWater;
  int    Numb;
  double k_C, k_H, k_O;
  double GlobalGamma;
  struct CLinApprCp * pApprCp;
//
  enum eFunctions { 
    eHv   = 0x0001, // C_IG, H_IG,
    eHl   = 0x0002, // H_l, Hvap
    ePz   = 0x0004, // Pz
    eRo   = 0x0008, // Ro_liq, To_m3_liq
    eS    = 0x0010, // S_IG, G_IG 
    eChem = 0x0020, // H_Chem, dH_Chem
    eObr  = 0x0040, // dHobr, dSObr, dGobr 
    };
  DWORD  Function; // показывает какие функции реализованы 
  CGas();
  virtual void   Init() = 0; //0 
  virtual double C_IG ( double TC ) = 0;	// Теплоемкость газа при постоянном давлении
  virtual double C_Liq ( double TC ) = 0;	// Теплоемкость жидкости при постоянном давлении
  virtual double H_IG ( double TC ) = 0;	// Энтальпия
  virtual double H_l ( double TC ) = 0;
  virtual double  dH_1_dTC (double TC) = 0;
  virtual double Pz ( double Tc ) = 0;
  virtual double H_vap ( double TC ) = 0;
  virtual bool   Ro_liq ( double T_C, double & Ro ) = 0;
  virtual bool   To_m3_liq ( double TC, double & To_m3 ) = 0;
  virtual double Vis_gas ( double P, double TC ) { return 0.; };	
  virtual double Vis_liq ( double TC ) { return 0.; };	
  virtual double Lam_gas ( double P, double TC ) { return 0.; };	
  virtual double Lam_liq ( double TC ) { return 0.; };	
//
  virtual double S_IG( double P, double TC ) { return 0.; };	// Энтропия
  virtual void   S_IG( double P, double TC, double & S, double & dSdT ) { S = 0.; dSdT = 0.; };
  virtual double G_IG( double P, double TC ) { return 0.; };	// Энергия Гиббса
//
  virtual double dHobr( double Tc ) { return 0.; };
  virtual double dSobr( double Tc ) { return 0.; };
  virtual double dGobr( double Tc ) { return 0.; };
//
  virtual double H_Chem ( double TC ) { return 0.; };
  virtual double dH_Chem ( ) { return 0.; };
//
#ifdef LINUX
  virtual int ReadData( QFile * hFile ) = 0;
#else
  virtual int ReadData( HANDLE hFile ) = 0;
#endif
  virtual void   SetS_G( double S_298 );
  virtual void   Print ( void * F );
  virtual void   GetParams ( IBaseModel * pMain ) = 0;
  virtual struct CSpecialData * GetSpecialData ( char * Params ) { return NULL; };
  };

struct CRealDBData
  {
#undef INT
#define INT(Var) int Var;
#undef DOUBLE
#define DOUBLE(Var) double Var;
#undef CHAR
#define CHAR(Var) char Var[64];
#undef SKIP
#define SKIP(Var)
  CHAR(AltName)
#include "./chem/RealData.h"
#include "./chem/Antoine.h"
#include "./chem/GasLam.h"
#include "./chem/GasVis.h"
#include "./chem/GObr.h"
#include "./chem/HObr.h"
#include "./chem/HenryW.h"
#include "./chem/Hvap.h"
#include "./chem/IdealGasCp.h"
#include "./chem/LiqLam.h"
#include "./chem/LiquidCp.h"
#include "./chem/LiquidDensity.h"
#include "./chem/LiqVis.h"
#include "./chem/SoklidCp.h"
#include "./chem/SolLam.h"
  //#include "SolubilityW.h"
  //#include "SurfaceTension.h"
  };

class IN_DLL CReal : public CGas, public CRealDBData
  {
  public:
    //
  static double Max_Pz;
  double Cp_A, Cp_B, Cp_C, Cp_D, Cp_E, CpTmin, CpTmax;
  double Cp_A_liq, Cp_B_liq, Cp_C_liq, Cp_D_liq, CpTmin_liq, CpTmax_liq;
  double A_A, A_B, A_C, A_D, A_E, A_Tmin, A_Tmax;
  double Hobr_A, Hobr_B, Hobr_C, Hobr_Tmin, Hobr_Tmax, H298;
  double Gobr_A, Gobr_B, Gobr_C, Gobr_Tmin, Gobr_Tmax, G298;
  double Hvap_A, Hvap_B, Hvap_N, Hvap_Tmin, Hvap_Tmax;
  double RoLiq_A, RoLiq_B, RoLiq_N, RoLiq_Tmin, RoLiq_Tmax, RoLiq_Tc;
  double Hp_A, Hp_B, Hp_C, Hp_D, Hp_E, Hp_F, Hp_F_0;
  double S_A, S_B, S_C, S_D, S_E, S_G;
  double GasVis_A, GasVis_B, GasVis_C, GasVis_Tmin, GasVis_Tmax;
  double LiqVis_A, LiqVis_B, LiqVis_C, LiqVis_D, LiqVis_Tmin, LiqVis_Tmax;
  double GasLam_A, GasLam_B, GasLam_C, GasLam_Tmin, GasLam_Tmax;
  double LiqLam_A, LiqLam_B, LiqLam_C, LiqLam_D, LiqLam_Tmin, LiqLam_Tmax;
  CReal();
  virtual double  dHobr( double Tc );
  virtual double  dSobr( double Tc );
  virtual double  dGobr( double Tc );
  virtual void    Init();
  virtual double  C_IG ( double TC );
  virtual double  C_Liq ( double TC );
  virtual double  H_IG ( double TC );
  virtual double  H_Chem ( double TC );
  virtual void    SetS_G( double S_298 );
  virtual double  S_IG( double P, double TC );
  virtual void    S_IG( double P, double TC, double & S, double & dSdT );
  virtual double  G_IG( double P, double TC );
  virtual double  Pz ( double Tc );
  virtual double  dPz_dT ( double Tc );
  virtual double  H_v ( double TC );
  virtual double  H_l ( double TC );
  virtual double  dH_1_dTC (double TC);
  virtual bool    Ro_liq ( double T_C, double & Ro );
  virtual bool    To_m3_liq ( double TC, double & To_m3 );
  virtual double Vis_gas ( double P, double TC );
  virtual double Vis_gas_2 ( double P, double TC );
//  virtual double Vis_gas_3 ( double P, double TC );
  virtual double Vis_liq ( double TC );
  virtual double Lam_gas ( double P, double TC );
  virtual double Lam_liq ( double TC );
  virtual double  dH_Chem( );
  virtual double  H_vap ( double TC );
  virtual void    Print ( void * F );
  virtual void    Set( CReal * pReal );
  virtual void   GetParams ( IBaseModel * pMain );
#ifdef LINUX
  int ReadData( QFile * hFile );
#else
  int ReadData( HANDLE hFile );
#endif
  bool CHO();
  NEW
  };

struct CPseudoDBData
  {
  double Tb;
  };

class IN_DLL CPseudo : public CGas
  {
  public:
  double nC;
	double Ro_20_4, Ro, sq_Ro;
  double dH_v25;
  int kC, kH;
  double PzA, PzB, PzC;
  double Pz_B;
  double Pz_A; 
  double SG, TbK, Kw;
  double Cp_A2, Cp_A1, Cp_A0;
  double Hv_A2, Hv_A1, Hv_A0, Hv_25;
  double Cp_L_a;
  double Cp_L_b;
  double Cp_L_c;
///
  CPseudo( );
  virtual double dHobr( double Tc );
  virtual double dSobr( double Tc );
  virtual double dGobr( double Tc );
  virtual void   Init();
  virtual double H_Chem ( double TC );
  virtual double dH_Chem ( );
  virtual double S_IG( double P, double TC );
  virtual void   S_IG( double P, double TC, double & S, double & dSdT );
  virtual double G_IG( double P, double TC );
  virtual double Vis_gas ( double P, double TC );
//  virtual double Vis_gas_2 ( double P, double TC );
  virtual double Vis_liq ( double TC );
//  virtual double Vis_liq_2 ( double TC );
  virtual double Lam_gas ( double P, double TC );
//  virtual double Lam_gas_2 ( double P, double TC );
  virtual double Lam_liq ( double TC );
//  virtual double Lam_liq_2 ( double TC );
//
  virtual double C_IG ( double TC );
  virtual double C_IG_appr ( double TC );
  virtual double C_Liq ( double TC );	
  virtual double C_Liq_appr ( double TC );	
  virtual double C_Liq_appr_2 ( double TC );	
  virtual double H_IG ( double TC );
  virtual double  H_IG_appr ( double TC );
  virtual double H_v ( double TC );
  virtual double H_l ( double TC );
  virtual double dH_1_dTC (double TC);
	virtual double Pz ( double T_C );
  virtual double Pz_appr ( double T_C );
  virtual double Pz_appr_2 ( double T_C );
  virtual double dPz_dT ( double Tc );
	virtual bool   Ro_liq ( double T_C, double & Ro );
  virtual double Ro_liq ( double T_C );
  virtual bool   To_m3_liq ( double TC, double & To_m3 );
  virtual double H_vap ( double TC );
  virtual double H_vap_appr ( double TC, int Variant );
  virtual void Print ( void * F );
  virtual void   GetParams ( IBaseModel * pMain );
#ifdef LINUX
  int ReadData( QFile * hFile );
#else
  int ReadData( HANDLE hFile );
#endif
///////////////////////////////////////
  bool Mw_Tb( char * Appr, double Tb );
  void Tc_Pc ( char * Appr, double Tb );
  void Test( void * F );
  void Init_appr();
  NEW
  };

struct CSolidDBData
  {
  char Name[64];
  double Ro_A, Ro_B, Cp_A, Cp_B;
  };

class IN_DLL CSolid
  {
  public:
    char Name[64];
    double Ro_A, Ro_B, Cp_A, Cp_B;
    CSolid( );
    virtual void Init(); //0 
    virtual double Cp ( double TC );	// Cp кДж/кг C
    virtual double H  ( double TC );	// Cp * T кДж/кг
    virtual double Ro ( double T_C ); // Ro кг/м3
    virtual void GetParams ( IBaseModel * pMain );
#ifdef LINUX
    int ReadData( QFile * hFile );
#else
    int ReadData( HANDLE hFile );
#endif
  NEW
  };

struct CVirtualDBData
  {
  char Name[64];
  char Param[64];
  };

class IN_DLL CVirtual
  {
  public:
  char Name[64];
//
//  CVirtual( );
//  virtual void Init(); //0 
//  int ReadData( HANDLE hFile );
  NEW
  };


struct IN_DLL I_Addition
  {
  const char * AdditionName;
  I_Addition * pNext;
  virtual void Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq ) = 0;
  virtual void Init( void * pData ) = 0;
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void UpdateParams( ) = 0;
  };

struct IN_DLL I_AdditionList
  {
  I_Addition * pFirst;
  I_Addition * pLast;
  virtual void Init( ) = 0;
  virtual void InitAdditions( void * pData ) = 0;
  virtual void Add( I_Addition * pAdd ) = 0;
  virtual void Gamma(  void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq ) = 0;
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void UpdateParams( ) = 0;
  };

#define MAX_GAMMA 8
extern IN_DLL struct IGamma * pGamma[MAX_GAMMA];

//CGas * GetExternComponent( const char * Dll, const char * CompName );
typedef 
  CReal * (*tGetRealComp) ( const char * CompName );
typedef 
  CGas * (*tGetGasComp) ( const char * CompName );
typedef 
  I_Addition * (*tGetAddition) ( const char * AddName );

IN_DLL I_Addition * LoadAddition( const char * Dll, const char * AddName ); 
IN_DLL CReal * LoadComponent( const char * Dll, const char * CompName ); 
IN_DLL CGas * LoadGasComp( const char * Dll, const char * CompName ); 
IN_DLL void InitSpecS ( double Tb ); 
