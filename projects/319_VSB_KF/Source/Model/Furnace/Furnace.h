#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
#include "Chem.h"
#include "EpsVol.h"
#include "Defects.h"
#include "ACS_Def.h"
//#include "ShowNode.h"

struct CFireT
  {
  double k, Text, U, T0, F0, M, U4;
  double *Told, Step_T;
  void Set_T0( double T0 );
  void Set_U( double k, double Text );
  double F( double TC );
  double X_T( double TC );
  void X_dX( double TC, double & F, double & dF_dT );
  double T_X ( double X );
  double T_X_cool( double XT );
  double T_X_heat( double XT );
  void SetOld( double T );
  NEW
  CFireT();
  };

struct CDepression
  {
  double Mw_gas, Mw_mazut, Mw_steam, Mw_air, Mw_smoke, Mw_podsos;  
  double Flow_topl_gas, Flow_mazut, Flow_steam, Flow_air_fors, Flow_air_add;
  double Omega_podsos, Omega_out_smoke;
  CDepression();
  };

struct _W
  {
  double P_Под;
  double O_Air;
  double O_Gas;
  double O_Mazut;
  double O_Steam;
  double O_Pilot;
  double O_Add_Gas;
  double H_Smoke;
  double T_Pereval;
  bool IsComp_Smoke[K_GAS];
  double Cmol_Smoke[K_GAS];
  double P_Верх;
//
  double T_Wall;
  double Flow_Smoke;
  double MinWorkGas;
  _W();
  };

class CForsLogic
	{
	public:
	char Name[32];
	int TimerWork;
	double TimerOpenGas;
	bool IsFire;
	bool IsZapal;
	double Zapal_Time;
  double Flow_Fors_Gas;
  double Flow_Fors_Zapal;
  double Flow_Fors_PSA;
  double Flow_Fors_Steam;
  double Flow_Fors_Air;
  CForsunka * pFors;
//
	CForsLogic( );
	void Init ( char * Name );
	void Work ( double Flow_Fors_Air,
              double Flow_Fors_Gas, 
							double Flow_Fors_Zapal,
		          double Flow_Fors_Mazut, 
		          double Flow_Fors_Steam, 
              bool NoZapal, 
			  double MinWorkGas);
	};

struct CRadFors_W
  {
  //
  double T0, T;
  //=================================
  double Flow_gas, h_gas, To_kg_gas;
  double Flow_mazut_full, Flow_mazut, h_mazut, To_kg_mazut;
  double Flow_steam, h_steam;
  double Flow_air, h_air;
  double Flow_Steam_Rad, h_Steam_Rad;
  double Flow_in;
  //=================================
  double Flow_Smoke, Q;
  double T_in;
  double H_Smoke_0, H_Smoke;
///////////////////////////////
  bool IsComp[K_GAS];
  double Topl_Cmol[K_GAS];
  double Smoke_Cmol[K_GAS];
  double KPD;
	bool IsFire;
  double Q_wall;
  double Q_ext_Rad;
  double Q_Rad;
  //
  };

class CRadFors : public CRadFors_W, public IBaseModel // 1 форсунка
  {
  public:
	class CFurnace * pPech;
  static CComp * pComp_Gas;
  static CComp * pComp_Mazut;
  static CComp * pComp_Progar;
// CForsunka * pFors;
	double Q_fire;
	double k_Fire;
	double k_Air;
	bool IsProgar;
	double F_Progar;	// Згоревшая нефть
	double h_Progar;
	double Air_Need;
  bool IsCondensat;
  double kCondensat;
	CIG_Mix  Smoke;
	CIG_Mix  Topl;
  class CRadTube ** pTube;
  CForsLogic Logic;
  double T_max;
  double Flow_gas_eff;
  double F_Smoke_Nom_Mol;
//
  double A_wall, B_wall, Q_wall;
  CFireT * pFireRad;
  static double S_Fors;
  static double Cp;
//
//
  CRadFors( char * Name, IBaseModel * pMain );
  void Init( );
  void CalcT ( double dt );
  void NoFire( );
  void CalcT0( );
  void Step( double dt );
//  void CalcE( );
  void CalcQfire( );
	int SaveState( );
	int RestoreState( char * StrName );
  int GetParams( char *  );
  void CreateRad( );
  };

///////////////////////////////////////////////////////////////////////////
struct CRadTube_W
  {
  double T_in, T_out;
  double h_Out_Gas;
  double Eps_Gas;
	double Q_ext_Rad; // Приход
  double Q_wall;
  double Q_Rad; // Излучение
  double E_Coke;
  CRadTube_W();
  };

class CRadTube : public CRadTube_W, public IBaseModel 
  {
  double F ( double d, double T );
  public:
//
  class CFurnace * pPech;
  CFlow * pFlow_in;
  CFlow * pFlow_out;
	double Cp_Tube;
	double h_ext;
	CSEps In;
	CSEps Out;
	CSEps OutZ;
  double TZ;
  double hOutZ;
  bool Calc_H;
  double F_Прогар;
  double O_Прогар;
//
  double A_wall, B_wall;
  static double S_Trub;
  double Q_Продукт;
//
  CRadTube( char * ObjName, IBaseModel * pPech );
  void Step( double dt );
  void CalcT ( double dt );
  double dH();
  double QRad();
	int SaveState( );
	int RestoreState( char * StrName );
  int GetParams( char * );
  };

struct CNodeTube
  {
  CStr In;
  CStr Out;
  };

struct CSpecPnt
  {
  CSpecPnt * pNext;
  int Type, n1, n2;
  CAnalogD * pA; 
  double rnd;
  CSpecPnt()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  void operator = ( double V )
    {
    if ( pA )
      *pA = V * rnd;
    }
  NEW
  };

class CFurnace : public IBaseModel, public IUniModel, public _W, public CDepression
	{
//
public:
	CFurnace( char * ObjName, char * Param );
	virtual ~CFurnace();
// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * );
  int SetProp ( struct CObjProps * pProp );
  int GetProp( );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  ///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  CExtern_Pnt * GetPoint( const char * PntName );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Furnace_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  CRadTube ** pTube;
  CNodeTube * pNodeTube;
  double * W_Tube;
  int kProduct;
  CRadFors ** pFire;
  bool * IsZapalFire;
  bool * IsWorkFire;
  int kFors;
  CFlow * pTopl_Gas;
  CFlow * pPilot;
  CFlow * pAdd_Gas;
  CFlow * pMazut;
  CFlow * pMazutSteam;
  CFlow * pSteam;
  CFlow * pAir;
  CFlow * pSmoke;
  bool IsProgar;
  int nProgar;
  int kGasVent;
  int kGas2Vent;
  int kPilotVent;
  int kStep;
  double T_fire;
//
  CIG_Mix Smoke;
  CComp * pCompSmoke;
  int ID_CompSmoke;
  CComp * pCompGasZ;
  int ID_CompGasZ;
  //CShowNode ShowNode;
  double kFlowIn;
//
  CDiscrete ** Горит; 
  CDiscrete ** Горит_пилот; 
  CDiscrete ** Открыть_пилот;
  CDiscrete ** Розжиг_пилот;
  CDiscrete ** Отказ_пилот;
  CDiscrete ** Отказ_пламя;
  CAnalog ** Интенсивность_пламени;
  CAnalog ** Интенсивность_пилот;
  CAnalog ** Температура_на_выходе_труб;
  CAnalog ** Температура_змеевика; 
  double k_Zm[8];
  CDiscrete ** Конц_откр_газ; 
  CDiscrete ** Конц_закр_газ; 
//  CAnalogD ** T_Zm; 
//  double * rnd_T_Zm; 
  CSpecPnt * pFirstPnt;
//  int kT_Zm;
//???  double Gas[4]; 
//???  double Pilot[4];
  double Fuel[4]; 
  double Eff[4]; 
//
  double P_out_smoke;
  double F_out_smoke;
  double P_fors;
//
  double FlowQ;
  double CalcEff( double FlowGas );
  double VentGas ( CForsunka * pFors );
  double VentGas2 ( CForsunka * pFors );
  double VentPilot ( int n, CForsunka * pFors );
//
  void Calc ( double dt );
  void CalcPress( double dt );
  void CalcT_Wall( double dt );
  void CalcT_Rad( double dt );
  void CalcCoef( );
  double Rad_Wall( );
	};
