#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "EpsVol.h"
#include "Control.h"

struct CNode
  {
  int Numb;
  char Type;
  CStr Name;
  CObjectPoint * pOP;
  };

struct _W
  {
  double Flow_io;
  double P, T, h_Flow;
  bool IsComp[K_GAS];
  double Cmol[K_GAS];
  double To_kg, To_m3;
  double Flow_iter;
  double h_summ[K_GAS];
  double Cp_summ[K_GAS];
  double Pio;
  _W();
  };

class CCollector : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CCollector( char * ObjName );
	virtual ~CCollector();
// Интерфейс IUniModel
  int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams(  char * StrName );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  void DrawObj ( struct CDrawObjData * pDraw );
///////////////////////////////////////////////////
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  NEW
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Collector_Defects.h"
//#include "ACS_Create.h"
//#include "ACS.h"
/////////////////////////////////////////////////
//  CSelVar Var_1;
  CNode * pNodes;
  double * Omega;
//  double * kOmega;
  CFlow ** pFlow;
  CSEps ** pStateIn;
  CSEps State;
  double * Flow_kg;
  double * FlowSumm;
  int kIO;
  int NumbIn;
  int NumbOut;
  CComp * pComp;
  int ID_Comp;
  CComp * pCompZ;
  int ID_CompZ;
  int kStep;
  double Flow_IO;
  int Defect;
  double dP_old;
  CStableControl Stable;
  double Flow_Ctrl;
  char * PipeNames;
  char NameIn[64];
  char NameOut[64];
//
  double h_gas;
  double h_liq;
  double h_eps;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
  void Set_h_summ( CSEps * pS, double * h_summ );
  double Calc_dH( CSEps * pS );
  double Calc_H( CSEps * pS );
	};
