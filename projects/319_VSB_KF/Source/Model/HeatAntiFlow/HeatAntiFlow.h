#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "SR.h"
#include "Data.h"
#include "Chem.h"
#include "Defects.h"
#include "UniHeat.h"
#include "ACS_Def.h"
#include "IntReg.h"
#include "Extensions.h"

struct _W
  {
	double T_in_Trub, T_out_Trub;
	double T_in_Vol, T_out_Vol;
	double Cp_Trub, Cp_Vol;
  double dP_Hydro;
//
  double P_def_inVol;
  double P_def_outVol;
  double P_def_Trub;
  double P_def_outTrub;
  double h_inVol;
  double h_outVol;
  double h_inTrub;
  double h_outTrub;
  double dP_Circ;
  double P_Аневризма;
//
  _W();
  };

class CHeatAntiFlow : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CHeatAntiFlow( char * ObjName );
	virtual ~CHeatAntiFlow();
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
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Points.h"
/////////////////////////////////////////////////
  CFlow * In_Trub;
	CFlow * Out_Trub;
  CFlow * In_Vol;
	CFlow * Out_Vol;
//
  CFlow * Trub_0;
  CFlow * Trub_1;
  CFlow * Vol_0;
  CFlow * Vol_1;
//
  int ID_In_Trub;
  int ID_Out_Trub;
  int ID_In_Vol;
  int ID_Out_Vol;
  CComp * Comp_Vol;
  CComp * Comp_Trub;
  int ID_Comp_Vol;
  int ID_Comp_Trub;
/////////////////////////////////////////////////
/////////////////////////////////////////////////
	CAntiFlow Heat;
//
  bool Heat_Defect;
  double k_Heat;
//
  bool Omega_Defect;
  double k_Omega;
  double Def_Trub;
  double Def_outTrub;
  double M_inVol;
  double M_outVol;
  double M_inTrub;
  double M_outTrub;
  double FlowQ;
//
  CFlow In_t, Out_t, In_v, Out_v;
  double kFlow_1, kFlow_2;
  double kDefectFlow;
  bool DefectMix;
//  double dP_Circ;
  double Переток_Trub; 
  double Переток_Vol; 
  bool FirstMix;
  bool IsCompMix[K_GAS];
  double CmolMix[K_GAS];
  double h_Mix;
  double F_Mix;
  CComp * Comp_Mix;
  int ID_Comp_Mix;
  CComp * Comp_Аневризма;
  int ID_Comp_Аневризма;
  double h_Аневризма;
//  CTr Tr;
//
#include "ACS_Create.h"
#include "ACS.h"
//
  void Calc ( double dt );
	void DrawObj ( struct CDrawObjData * pDraw );
  void Add( double Flow, double h_Flow, CComp * pComp );
  double VarK( );  // 0 - трубы, 1 - объем
	};
