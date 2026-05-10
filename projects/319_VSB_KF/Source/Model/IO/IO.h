#pragma once

#include "BaseInt.h"
#include "Data.h"
#include "Defects.h"
#include "EpsVol.h"
#include "ACS_Def.h"
#include "IntReg.h"
#include "ChemBase.h"


struct _W
  {
  double Cmol[K_GAS];
  bool IsComp[K_GAS];
  double P_src, T_src, h_Flow_src;
  double Mw_src;
  double Control_flow;
//
  double P_in, P_out;
  double F_in, F_out;
  double Ps;
  double _P_in, _P_out;
  double _F_in, _F_out;
  _W();
  };

class CIO : public IBaseModel, public IUniModel, public _W
{
//
public:
	CIO( char * ObjName );
	virtual ~CIO();
// Интерфейс IBaseModel
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
	int SetProp ( struct CObjProps * pProp );
	int GetProp( );
///////////////////////////////////////////////////
	int Work( double dt );
  int TestCtrlPoints( int kNodes, struct CObjectPoint ** _ppNodes );
///////////////////////////////////////////////////
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  NEW
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
	CSEps State;
	CComp * pComp;
	int ID_Comp;
  int CompDefect;
//  CSolid * pSolid;
/////////////////////////////////////////////////
	enum eType { None, IO, In, Out, Atm };
	eType Type;
	CFlow * pFlow_in;
	CFlow * pFlow_out;
//	CScript Script;
	bool FirstStepT; 
	double Omega_Defect;
	int StateDir; // Направление потока расчета State
	bool Закупорен;
	int kStep;
	bool SetFlow;
	double Omega_min_Z;
	double Omega_max_Z;
	double k_Omega_Z;

	void Calc ( double dt );
	void SetComp( );
	void ReadType( );
	void Flow_In ( double dt, CFlow * pFlow );
	void Flow_Out ( double dt, CFlow * pFlow );
	void DrawObj ( struct CDrawObjData * pDraw );
	virtual void AddResources (CGDIResourceMgr *pResMgr);
	virtual void DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr);
	void Matr_1_In( double dt, double * A, double * B );
	void Matr_1_Out( double dt, double * A, double * B );
	void Reg_In( );
	void Reg_Out( );
	void SavePTC( );
	bool RestPTC( );
  void Set_PTE(  );
};

#define mOmega 1e4
