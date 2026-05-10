#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
#include "Extensions.h"
#include "Data.h"
#include "Chem.h"
#include "EpsVol.h"
#include "Defects.h"
#include "ACS_Def.h"

#define MAX_FORS 64
struct _W
  {
  double P_Под;
  double H_Smoke;
  double T_Pereval;
  bool IsComp_Smoke[K_GAS];
  double Cmol_Smoke[K_GAS];
  double P_Верх;
//
  double T_Wall;
  double P_Air[MAX_FORS];
//
  double Mu[7];
//
  double Flow_Smoke;
  double Mu_Smoke_in;
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
  bool SetFirePilot;
  bool SetFireMain;
//
  double MIN_GAS_AIR;
  double MIN_MAZUT_AIR;
  double MIN_WORK_GAS;
  double MIN_WORK_MAZUT;
  double MIN_ZAPAL;
//
	CForsLogic( );
	void Init ( char * Name );
	void Work ( double Flow_Fors_Air,
              double Flow_Fors_Gas, 
							double Flow_Fors_Zapal,
		          double Flow_Fors_Mazut, 
		          double Flow_Fors_Steam, 
              bool NoZapal );
	};

struct CRadFors_W
  {
  //
  double T0, T;
  //=================================
  double Flow_gas, h_gas;
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
  double Flow_Smoke_in, h_Smoke_in;
  double Flow_Add, h_Add;
  double A_wall, B_wall;
  CRadFors_W()
    {
    memset ( this, 0, sizeof ( *this ));
    }
  //
  };


class CRadFors : public CRadFors_W, public IBaseModel //
  {
  public:
	class CFurnace * pPech;
  CComp * pComp_Gas;
  CComp * pComp_Mazut;
  CComp * pComp_Add;
  CComp * pComp_Smoke_in;
  static CComp * pComp_Progar;
//CForsunka * pFors;
	double Q_fire;
	double k_Fire;
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
  bool GasAsMasut;
  bool CalcChem;
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
  void dH_Cp(  bool IsComp[], double Cmol[], double T_C, double & H, double & Cp, bool CalcCp );
  double TH (  bool IsComp[], double Cmol[], double kH, double kT, double C, double T_0 );
  };

///////////////////////////////////////////////////////////////////////////
struct CRadTube_W
  {
  double T_in, T_out;
  double h_Out_Gas;
  double Eps_Gas;
	double Q_Rad;
  double Q_wall;
  double kFlow;
  double A_wall, B_wall;
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
  IPyrolysis * pPyrolysis;
  IBaseModel * pBasePyrolysis;
  bool InitPyrolysis;
//
  CRadTube( char * ObjName, IBaseModel * pPech );
  void InitTube();
  void Step( double dt );
  void CalcT ( double dt );
  double dH();
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

const int N_SMOKE = 0;
const int N_GAS =   1;
const int N_PILOT = 2;
const int N_MAZUT = 3;
const int N_STEAM = 4;
const int N_AIR =   5;
const int N_ADD =   6;
const int N_SMOKE_IN =   7;

class CFurnace : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CFurnace( char * ObjName, LPCTSTR Params );
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
  int kForsZ;
  CFlow ** pTopl_Gas;
  CFlow ** pPilot;
  CFlow ** pFlow_Smoke_in;
//  CFlow * pAdd_Gas;
  CFlow ** pMazut;
  CFlow ** pMazutSteam;
  CFlow ** pAir;
  CFlow ** pAdd;
  CFlow * pSteam;
  CFlow ** pSmoke;
  double * Pz;
  double * Pz_mm;
  double * Flow_Air;
  DWORD * Use;
  bool IsProgar;
  int nProgar;
  int kGasVent;
  int kPilotVent;
  int kStep;
  double T_fire;
//
  CIG_Mix Smoke;
  CComp * pCompSmoke;
  int ID_CompSmoke;
  double Flow_Smoke_Real;
  int G_SO2, G_SO3;
//
  CDiscrete ** Горит; 
  CDiscrete ** Горит_пилот; 
  CDiscrete ** Открыть_пилот;
  CDiscrete ** Розжиг_пилот;
  CDiscrete ** Отказ_пилот;
  CDiscrete ** Отказ_пламя;
  CAnalog ** Интенсивность_пламени;
  CAnalog ** Интенсивность_пилот;
//  CHs * pBurnAll;
//  CAnalogD ** T_Zm; 
//  double * rnd_T_Zm; 
  CSpecPnt * pFirstPnt;
  int kIn_Product;
  int kOut_Product;
  int k_Gas;
  int k_Smoke;
  int k_Smoke_in;
  int k_Pilot;
  int k_Air;
  int k_Add;
  int k_Mazut;
  int k_Steam;
  double kFlowIn;
//
  bool MatrixModel;
  int N_Matrix;
  int p_Gas;
  int p_Smoke;
  int p_Smoke_in;
  int p_Pilot;
  int p_Air;
  int p_Add;
  int p_Mazut;
  int p_Steam;
  int NumbFlow[8];
  double Omega[8];
  double * P_Out;
  double * P_Smoke;
  double * F_Smoke_m3;
  double * P_Gas;
  double * F_Gas_m3;
  double * F_Mazut_kg;
  double * F_Steam_kg;
  double * P_Pilot;
  double * F_Pilot_m3;
//??  double * P_Air;
  double * F_Air_m3;
  double * F_Air_podsos_m3;
  ICreatePyrolysis * pCreatePyrolysis;
  bool CalcChem;
  bool NewCalc;
//
  double FlowQ;
  double VentGas ( CForsunka * pFors );
  double VentPilot ( int n, CForsunka * pFors );
//
  void Calc ( double dt );
  void CalcPress( double dt );
  void CalcT_Wall( double dt );
	};

const double VentMin = 0.001;
