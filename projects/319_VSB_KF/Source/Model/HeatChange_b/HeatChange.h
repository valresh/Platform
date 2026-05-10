#pragma once

#include "BaseInt.h"
#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "EpsVol.h"
#include "UniHeatChange.h"
#include "UniHeat.h"
#include "IntReg.h"

struct _W
  {
	double T_in_Trub, T_out_Trub;
	double T_in_Vol, T_out_Vol;
  double dP_Hydro;
  double kMix_trub, kMix_vol;
  double Xi_Vol, Xi_Trub;
  double k_Trub;  // F_out = k * Fin
  double k_Vol;
  double Q;
  _W();
  };

struct CEtalon
  {
  double D;
  double S;
  double Re_Etalon;
  double Pr_Etalon;
  double Nu_Etalon;
  double Alfa_Etalon;
  double Xi_Etalon;
  double Re;
  double Pr;
  double Nu;
  double Alfa;
  double Xi;
  CEtalon();
  void CalcLiqCoef ( CFlow * pFlow, CSEps * pEps, double & Alfa_Rel, double & Xi_Rel );
  void CalcGasCoef ( CFlow * pFlow, CSEps * pEps, double & Alfa_Rel, double & Xi_Rel );
  void CalcLiqEtalon ( CFlow * pFlow, CSEps * pEps );
  void CalcGasEtalon ( CFlow * pFlow, CSEps * pEps );
  void CalcMixEtalon ( CFlow * pFlow, CSEps * pEps );
  void CalcMixCoef ( CFlow * pFlow, CSEps * pEps, double & Alfa_Rel, double & Xi_Rel );
  void CalcH2OCoef( );
  };

class CHeatChange : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CHeatChange( char * ObjName );
	virtual ~CHeatChange();
// Интерфейс IBaseModel
	int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * StrName );
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
	NEW
	void DrawObj ( struct CDrawObjData * pDraw );
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
  CFlow * In_Trub_0;
  CFlow * In_Vol_0;
  CFlow * In_Vol;
  CFlow * In_Trub;
  CFlow * Out_Vol;
	CFlow * Out_Trub;
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "HeatChange_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
/////////////////////////////////////////////////
	CUniHeatChange Heat;
//
  int ID_Comp_Vol;
  CComp * pCompVol;
  int ID_Comp_Trub;
  CComp * pCompTrub;
//
  bool Omega_Defect;
  double k_Omega;
  double Def_Trub;
  bool Перемешивание;
  double K_Defect;
//
  bool Def_Zasor;
//
  double FlowQ;
  double kAntiFlow;
//
  double K_heat;
  CEtalon Etalon_Trub;
  CEtalon Etalon_Vol;
//
  void Calc ( double dt );
  double KS()
    {
    double K = K_heat * S * 3.6 * K_Rel * K_Defect;
    return K;
    };
  //
  void CalcH2OCoef ( );
  void CalcCoef ( );
  void CalcEtalonTrub( );
  void CalcEtalonVol( );
  void Mixture( );
  void Set_1F( );
  void Calc_1F( double dt );
  void CalcTemp( double dt, CFlow * pHot_in, CFlow * pCool_in, CFlow * pHot_out, CFlow * pCool_out, 
    CSEps	* S_Hot_In,  CSEps * S_Cool_In,  CSEps	* S_Hot_Out, CSEps	* S_Cool_Out );
  bool bRegTask;
	};

const double Cp_Trub_max = 20000.;
const double Cp_Vol_max = 20000.;
const double Cp_Trub_min = 10.;
const double Cp_Vol_min = 10.;
