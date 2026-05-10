#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
#include "UniHeat.h"
#include "EpsVol.h"
#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "Control.h"
#include "IntReg.h"

struct CPseudo_Water_W
  {
  double M_Water;
  double Level_Water;
  };

struct CPseudo_Water : public CPseudo_Water_W
  {
  double Hydro_Water;
  double Omega_Water;
  double L_WaterZ;
  double Max_Water;
  double Eps_Water;
  CAnalogD * pLev;
//
  CPseudo_Water( IBaseModel * pMain );
  void Calc( double dt, CFlow * pBaseFlow );
  void Out( double dt, double T, CFlow * pFlowWater );
  void Matr( double P, double Mp[], double Mf[], double V[] );
  int GetParams( IBaseModel * pMain );
  int SaveState ( );
  int RestoreState ( char * StrName );
  };

struct _W
  {
  double P;
  double T_gas, T_liq;
  double M_Liq, M_Gas, Level;
  double h_gas_mix, h_liq_mix;
  bool IsComp[K_GAS];
  double X[K_GAS], Y[K_GAS];
  double h_Gas[K_GAS], h_Liq[K_GAS];
  double To_m3;
//
  double k_Flow;
  double M_Water;
  double F_bott;
  double Eps[3];
  double Cp_liq, Cp_gas;
  _W();
  };

class CSepLiq : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CSepLiq( char * ObjName );
	virtual ~CSepLiq();
// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  void DrawObj ( struct CDrawObjData * pDraw );
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "SepLiq_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  CFlow * pInHeat;
  CFlow * pOutHeat;	
  CFlow * pFlow[3];
  int ID_Comp_Out[3];
  CComp * pComp_Out[3];
  CSEps * State[3]; 
  double Eps_Gas[3]; 
//  CSEps EpsOutGas; 
//  CSEps EpsOutLiq; 
  CHeat Heat;
  double Henry[K_GAS];
  double P_sat[K_GAS];
  double V_cond[K_GAS];
  int kStep; 
  double FlowQ;
  double Flow_out_liq;
//  CLevelControl LevCtrl;
  double Level_Z;
  bool PseudoWater;
  CPseudo_Water * pPW;
  double X_mass[K_GAS], Y_mass[K_GAS];
  bool Use_mass;
  double X_mol[K_GAS], Y_mol[K_GAS];
  bool Use_mol;
  bool NoInLiq_N2;
  double Ksi;
  double Q_heat;
  double Mw_gas;
  double Mw_liq;
  double M3_liq;
  double VolumeZ;
  bool Разгерметизация;
  double Cp_liq;
  double Cp_gas;
  double Eps_h[3];
//
  void Calc ( double dt );
  void CalcH( );
  void CalcLiq( double dt );
  void Razgerm( );
  void DrawState ( void * pData );
  void CalcEps( );
  void SummComp ( double F, CComp * pComp, double F_add, CComp * pComp_add );
	};
